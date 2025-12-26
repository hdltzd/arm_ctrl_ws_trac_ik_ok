#include "canbus_driver.h"
#include <cstring>
#include <chrono>

CanbusDriver::CanbusDriver() : run_flag_(false) {}

CanbusDriver::~CanbusDriver() {
    stop();
}

void CanbusDriver::set_callback(CanCallback cb) {
    rx_cb_ = cb;
}

bool CanbusDriver::init() {
    if (VCI_OpenDevice(VCI_USBCAN2, 0, 0) != 1) {
        printf("Error: Open CAN device failed!\n");
        return false;
    }

    VCI_INIT_CONFIG config;
    config.AccCode = 0;
    config.AccMask = 0xFFFFFFFF;
    config.Filter = 1;
    config.Timing0 = 0x00; // 1Mbaud (根据您的硬件配置调整)
    config.Timing1 = 0x14;
    config.Mode = 0;

    for (int ch = 0; ch < 2; ch++) {
        if (VCI_InitCAN(VCI_USBCAN2, 0, ch, &config) != 1 ||
            VCI_StartCAN(VCI_USBCAN2, 0, ch) != 1) {
            printf("Error: Init/Start CAN%d failed!\n", ch + 1);
            return false;
        }
    }

    run_flag_ = true;
    recv_thread_ = std::thread(&CanbusDriver::receive_loop, this);
    printf("CAN driver started.\n");
    return true;
}

void CanbusDriver::stop() {
    if (run_flag_) {
        run_flag_ = false;
        if (recv_thread_.joinable()) {
            recv_thread_.join();
        }
        VCI_ResetCAN(VCI_USBCAN2, 0, 0);
        VCI_ResetCAN(VCI_USBCAN2, 0, 1);
        VCI_CloseDevice(VCI_USBCAN2, 0);
        printf("CAN driver closed.\n");
    }
}

void CanbusDriver::send_single_motor_position_cmd(uint8_t id, int32_t pos, uint16_t max_speed) {
    if (!run_flag_) return;
    std::lock_guard<std::mutex> lk(can_mutex_);
    VCI_CAN_OBJ send;
    memset(&send, 0, sizeof(send));
    send.ID = id;
    send.DataLen = 7;
    send.Data[0] = 0x58;
    send.Data[1] = pos & 0xFF;
    send.Data[2] = (pos >> 8) & 0xFF;
    send.Data[3] = (pos >> 16) & 0xFF;
    send.Data[4] = (pos >> 24) & 0xFF;
    send.Data[5] = max_speed & 0xFF;
    send.Data[6] = (max_speed >> 8) & 0xFF;
    VCI_Transmit(VCI_USBCAN2, 0, 0, &send, 1);
}

void CanbusDriver::send_single_motor_speed_cmd(uint8_t id, int32_t speed) {
    if (!run_flag_) return;
    std::lock_guard<std::mutex> lk(can_mutex_);
    VCI_CAN_OBJ send;
    memset(&send, 0, sizeof(send));
    send.ID = id;
    send.DataLen = 5;
    send.Data[0] = 0x1D;
    send.Data[1] = speed & 0xFF;
    send.Data[2] = (speed >> 8) & 0xFF;
    send.Data[3] = (speed >> 16) & 0xFF;
    send.Data[4] = (speed >> 24) & 0xFF;

    VCI_Transmit(VCI_USBCAN2, 0, 0, &send, 1);
}

void CanbusDriver::send_multi_motor_position_cmd(const uint8_t id[6], const int32_t pos[6], const uint16_t max_speed[6]) {
    if (!run_flag_) return;
    std::lock_guard<std::mutex> lk(can_mutex_);
    VCI_CAN_OBJ send;
    memset(&send, 0, sizeof(send));
    send.DataLen = 7;

    for (int i = 0; i < 6; i++) {
        send.ID = id[i];
        send.Data[0] = 0x58;
        send.Data[1] = pos[i] & 0xFF;
        send.Data[2] = (pos[i] >> 8) & 0xFF;
        send.Data[3] = (pos[i] >> 16) & 0xFF;
        send.Data[4] = (pos[i] >> 24) & 0xFF;
        send.Data[5] = max_speed[i] & 0xFF;
        send.Data[6] = (max_speed[i] >> 8) & 0xFF;
        VCI_Transmit(VCI_USBCAN2, 0, 0, &send, 1);
    }
}

void CanbusDriver::get_single_motor_position_cmd(uint8_t *data,int32_t &pos) {
    if (data[0] == 0x08) {
    pos = (data[4] << 24) | (data[3] << 16) | (data[2] << 8) | data[1];
    }
}

/**
 * @brief 发送获取电机当前位置的指令 (0x08)
 * @param id 电机CAN ID
 */
void CanbusDriver::request_motor_position(uint8_t id) {
    if (!run_flag_) return; // 检查驱动是否运行
    
    std::lock_guard<std::mutex> lk(can_mutex_); // 线程安全锁
    VCI_CAN_OBJ send;
    memset(&send, 0, sizeof(send));
    
    send.ID = id;           // 目标电机ID
    send.DataLen = 1;       // 1字节指令协议
    send.Data[0] = 0x08;    // 获取电机当前位置的功能码
    
    // 调用接口发送CAN帧
    VCI_Transmit(VCI_USBCAN2, 0, 0, &send, 1); 
}


void CanbusDriver::receive_loop() {
    VCI_CAN_OBJ rec[300];

    while (run_flag_) {
        for (int ch = 0; ch < 2; ch++) {
            int reclen = 0;
            {
                // std::lock_guard<std::mutex> lk(can_mutex_);
                reclen = VCI_Receive(VCI_USBCAN2, 0, ch, rec, 300, 100); 
            }

            if (reclen > 0) {
                // 如果注册了回调，就将数据传给 ROS 节点
                if (rx_cb_) {
                    rx_cb_(rec, reclen, ch);
                }
            }
        }
        // 降低 CPU 占用
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }
}