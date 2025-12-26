#include "ros/ros.h"
#include "std_msgs/UInt16MultiArray.h"
#include <serial/serial.h>
#include "arm_pkg/sbus_parse.h"
#include <iostream>
#include "arm_pkg/handle_info.h"

int sbus_channel[16] = {0};
std::vector<uint8_t> sbus_buffer;
double sbus_success_time;
sbus_app sbus_handle;


inline double clamp(double v, double lo, double hi)
{
    if (v < lo) return lo;
    if (v > hi) return hi;
    return v;
}

static inline int map_switch(int val)
{
    switch (val) {
        case SWITCH_UP:  return 1;
        case SWITCH_MIDDLE: return 2;
        case SWITCH_DOWN:  return 3;
        default:   return 0;   // 未知值
    }
}



void decode_sbus(uint8_t* d, int channels[16]) {
    channels[0]  = ((d[1]    | d[2]<<8)                & 0x07FF);
    channels[1]  = ((d[2]>>3 | d[3]<<5)                & 0x07FF);
    channels[2]  = ((d[3]>>6 | d[4]<<2 | d[5]<<10)     & 0x07FF);
    channels[3]  = ((d[5]>>1 | d[6]<<7)                & 0x07FF);
    channels[4]  = ((d[6]>>4 | d[7]<<4)                & 0x07FF);
    channels[5]  = ((d[7]>>7 | d[8]<<1 | d[9]<<9)      & 0x07FF);
    channels[6]  = ((d[9]>>2 | d[10]<<6)               & 0x07FF);
    channels[7]  = ((d[10]>>5| d[11]<<3)               & 0x07FF);
    channels[8]  = ((d[12]   | d[13]<<8)               & 0x07FF);
    channels[9]  = ((d[13]>>3| d[14]<<5)               & 0x07FF);
    channels[10] = ((d[14]>>6| d[15]<<2 | d[16]<<10)   & 0x07FF);
    channels[11] = ((d[16]>>1| d[17]<<7)               & 0x07FF);
    channels[12] = ((d[17]>>4| d[18]<<4)               & 0x07FF);
    channels[13] = ((d[18]>>7| d[19]<<1 | d[20]<<9)    & 0x07FF);
    channels[14] = ((d[20]>>2| d[21]<<6)               & 0x07FF);
    channels[15] = ((d[21]>>5| d[22]<<3)               & 0x07FF);
}

void uart_protocol(serial::Serial& ser)
{
    size_t len = ser.available();

    if (len > 0) {
        // 临时读缓冲
        std::vector<uint8_t> tmp(len);
        ser.read(tmp.data(), len);

        // 追加到总 buffer
        sbus_buffer.insert(sbus_buffer.end(), tmp.begin(), tmp.end());
    }

    // 尝试解析完整帧
    while (sbus_buffer.size() >= 25) {

        if (sbus_buffer[0] == 0x0F) {
            if (sbus_buffer[24] == 0x00) {
                // 解析成功
                decode_sbus(sbus_buffer.data(), sbus_channel);
                sbus_buffer.erase(sbus_buffer.begin(), sbus_buffer.begin() + 25);
                sbus_success_time = ros::Time::now().toSec();
            } else {
                // 帧尾不对，丢 1 字节
                sbus_buffer.erase(sbus_buffer.begin());
            }
        } else {
            // 无效帧头，丢弃一个字节
            sbus_buffer.erase(sbus_buffer.begin());
        }
    }

    // 防止异常导致缓冲区无限增长
    if (sbus_buffer.size() > 200) {
        sbus_buffer.clear();
    }

    // 检查是否超时
    if(ros::Time::now().toSec() - sbus_success_time > 0.3)
    {
        memset(sbus_channel, 0, sizeof(sbus_channel));
        sbus_handle.connected = 0;
    }
    else sbus_handle.connected = 1;

}



std::pair<double, double> map_axis(int raw_x, int raw_y,
                                   int min_val = 326, int max_val = 1659,
                                   double deadzone = 10.0)
{
    // 1. 限幅
    raw_x = clamp(raw_x, min_val, max_val);
    raw_y = clamp(raw_y, min_val, max_val);

    // 2. 中位值
    double mid = (min_val + max_val) * 0.5;

    if(raw_x == 0) raw_x = mid;
    if(raw_y == 0) raw_y = mid;

    // 3. 偏移量
    double diff_x = raw_x - mid;
    double diff_y = raw_y - mid;

    // 半程
    double half_range = (max_val - min_val) * 0.5;

    // 有效映射区间  去掉死区
    double effective_range = half_range - deadzone;

    auto process_axis = [&](double diff) {
        // 在死区内部 → 0
        if (std::abs(diff) < deadzone) return 0.0;
 
        // 去除死区偏移，使其从 0 开始线性增长
        double sign = diff > 0 ? 1.0 : -1.0;
        double adjusted = std::abs(diff) - deadzone;  // 去除死区的偏移

        // 映射到 0~100
        double output = (adjusted / effective_range) * 100.0;

        // 加回符号
        output *= sign;

        // 限幅
        return clamp(output, -100.0, 100.0);
    };

    double out_x = process_axis(diff_x);
    double out_y = process_axis(diff_y);

    return {out_x, out_y};
}


void sbus_handle_app()
{
    auto left_xy = map_axis(sbus_channel[3], sbus_channel[2]);
    auto right_xy = map_axis(sbus_channel[0], sbus_channel[1]);

    sbus_handle.left_x = left_xy.first;
    sbus_handle.left_y = left_xy.second;

    sbus_handle.right_x = right_xy.first;
    sbus_handle.right_y = right_xy.second;

    sbus_handle.left_putter_a  = map_switch(sbus_channel[8]);
    sbus_handle.right_putter_b = map_switch(sbus_channel[9]);

    auto button_vr = map_axis(sbus_channel[5], sbus_channel[7], 326,1659,10);
    sbus_handle.left_button_vra = button_vr.first;
    sbus_handle.right_button_vrb = button_vr.second;

}




int main(int argc, char **argv) {
    ros::init(argc, argv, "sbus_parse");
    ros::NodeHandle nh;

    serial::Serial ser;
    bool serial_ok = false;

    ros::Publisher sbus_pub = nh.advertise<std_msgs::UInt16MultiArray>("sbus_channels", 15);
    ros::Publisher handle_pub = nh.advertise<arm_pkg::handle_info>("handle_info", 15);

    ros::Rate loop_rate(500);
    while (ros::ok()) 
    {

        //   尝试连接串口（自动重连）
        if (!serial_ok || !ser.isOpen()) {
            try {
                ser.setPort("/dev/ttyUSB0");
                ser.setBaudrate(115200);
                ser.setParity(serial::parity_none);
                ser.setStopbits(serial::stopbits_one);
                ser.setBytesize(serial::eightbits);
                ser.open();
                ROS_INFO("Serial connected!");
                serial_ok = true;
            }
            catch(...) {
                ROS_WARN_THROTTLE(2.0, "Serial not found, retrying...");
                ros::Duration(0.1).sleep();
            }
        }
        //  正常工作
        try {
            uart_protocol(ser);
        }
        catch(...) {
            ROS_ERROR("Serial error! Closing and retrying...");
            ser.close();
            serial_ok = false;
        }


        // uart_protocol(ser);
        sbus_handle_app();

        
        std_msgs::UInt16MultiArray sbus_msg;
        sbus_msg.data.resize(16);
        for (int i = 0; i < 16; ++i) {
            sbus_msg.data[i] = sbus_channel[i];
        }
        sbus_pub.publish(sbus_msg);
        
        arm_pkg::handle_info handle_msg;
        handle_msg.left_x = sbus_handle.left_x;
        handle_msg.left_y = sbus_handle.left_y;
        handle_msg.right_x = sbus_handle.right_x;
        handle_msg.right_y = sbus_handle.right_y;
        handle_msg.left_putter_a = sbus_handle.left_putter_a;
        handle_msg.right_putter_b = sbus_handle.right_putter_b;
        handle_msg.left_button_vra = sbus_handle.left_button_vra;
        handle_msg.right_button_vrb = sbus_handle.right_button_vrb;
        handle_msg.connected = sbus_handle.connected;
        handle_pub.publish(handle_msg);

        ros::spinOnce();
        loop_rate.sleep();
    }
}