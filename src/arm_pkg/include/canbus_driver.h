#ifndef CANBUS_DRIVER_H
#define CANBUS_DRIVER_H

#include "controlcan.h"
#include <mutex>
#include <atomic>
#include <thread>
#include <cstdint>
#include <cstdio>
#include <functional>

// 定义回调函数类型：参数是 (数据指针, 数据长度, 通道号)
using CanCallback = std::function<void(const VCI_CAN_OBJ*, int, int)>;

class CanbusDriver {
public:
    CanbusDriver();
    ~CanbusDriver();

    bool init();
    void stop();

    // 注册接收回调函数
    void set_callback(CanCallback cb);

    void send_single_motor_position_cmd(uint8_t id, int32_t pos, uint16_t max_speed);
    void send_single_motor_speed_cmd(uint8_t id, int32_t speed);
    void send_multi_motor_position_cmd(const uint8_t id[6], const int32_t pos[6], const uint16_t max_speed[6]);

    void get_single_motor_position_cmd(uint8_t *data,int32_t &pos);
    void request_motor_position(uint8_t id);
private:
    void receive_loop();

    std::mutex can_mutex_;
    std::atomic<bool> run_flag_;
    std::thread recv_thread_;
    CanCallback rx_cb_;
};

#endif // CANBUS_DRIVER_H