#include <ros/ros.h>
#include "arm_pkg/handle_info.h"
#include "controlcan.h"
#include "canbus_driver.h"
#include <atomic>
#include <vector>
#include <algorithm>

// --- 可視化消息 ---
#include <sensor_msgs/JointState.h>

// --- Trac-IK & KDL 核心庫 ---
#include <trac_ik/trac_ik.hpp>
#include <kdl/chainfksolverpos_recursive.hpp>
#include <kdl/frames.hpp>

CanbusDriver driver;

// --- 運動學對象 ---
boost::shared_ptr<TRAC_IK::TRAC_IK> trac_ik_solver;
KDL::Chain kdl_chain;
boost::shared_ptr<KDL::ChainFkSolverPos_recursive> fk_solver;

// --- 全局實時狀態 ---
struct RobotState {
    KDL::Frame pose;      // J6相對於Base的位姿
    KDL::JntArray joints; // 當前關節弧度
    bool is_valid;        // FK狀態
} g_robot_state;

// --- Mode 3 規劃目標 ---
KDL::Frame g_target_pose;   
bool g_cartesian_initialized = false;

// --- 控制標誌 ---
int global_model_button = 0;    
int last_model_button = 0;      
int current_reset_joint = 0;    

// ======================= [核心參數配置] =======================

// 1. 基礎減速比係數 (J1-J5)
const double SCALE_DEFAULT = 654784.0 / (90.0 - 54.3878);

// 2. 第六關節專用係數
const double SCALE_J6 = SCALE_DEFAULT; 

// 3. 關節係數數組
const double JOINT_SCALES[6] = {
    SCALE_DEFAULT, SCALE_DEFAULT, SCALE_DEFAULT, 
    SCALE_DEFAULT, SCALE_DEFAULT, SCALE_J6 
};

// 4. 零位偏置
double get_joint_offset(int i) {
    return 1000000.0 - JOINT_SCALES[i] * 54.3878;
}

// 5. 硬件映射與方向
const uint8_t MOTOR_ID_MAP[6] = {1, 2, 3, 4, 5, 6}; 
const double JOINT_DIRECTION[6] = {-1.0, -1.0, 1.0, 1.0, -1.0, -1.0}; 

// 速度限制參數分離
const double MAX_SPEED_VEL_MODE[6] = {200.0, 150.0, 150.0, 300.0, 300.0, 500.0};
// 位置模式下給大一點，避免濾波後的指令追不上
const uint16_t MAX_SPEED_POS_MODE[6] = {3000, 3000, 3000, 3000, 3000, 3000}; 

// ================================================================

std::vector<double> feedback_joint_angles(6, 0.0); 

// 指令容器
double speed_cmd[6] = {0.0}; 
int32_t pos_cmd[6]  = {0}; 

// [新增] 濾波專用變量：存儲上一次平滑後的目標角度（弧度）
double smoothed_target_rad[6] = {0.0};
// [新增] 濾波係數 (0.0 ~ 1.0)：越小越平滑但延遲越高；越大響應越快但抖動越大
// 0.08 是一個平衡點，既能防抖又不至於感覺太肉
const double FILTER_ALPHA = 0.08; 

// --- 輔助函數 ---
double get_ctrl_val(double val, double deadzone = 20.0) {
    if (std::abs(val) > deadzone) {
        return (val > 0 ? val - deadzone : val + deadzone) / (100.0 - deadzone);
    }
    return 0.0;
}

// 角度轉脈衝
int32_t angle_to_pulse(int joint_idx, double angle_deg) {
    double raw = (angle_deg / JOINT_DIRECTION[joint_idx]) * JOINT_SCALES[joint_idx] + get_joint_offset(joint_idx);
    return (int32_t)raw;
}

void handleCallback(const arm_pkg::handle_info::ConstPtr& msg)
{   
    if (!msg->connected) return;
    
    global_model_button = msg->left_putter_a; 
    int speed_button = msg->right_putter_b; 

    // [核心優化 1] 重新定義速度檔位，實現"微動操作"
    double speed_scale = 1.0;
    switch (speed_button) {
        case 1: speed_scale = 0.4; break; // [微動] 40% 速度，極慢
        case 2: speed_scale = 0.8; break; // [正常] 80% 速度
        case 3: speed_scale = 1.5; break; // [快速]
        default: speed_scale = 0.5;
    }   

    // 模式切換檢測
    if (global_model_button != last_model_button) {
        std::fill(std::begin(speed_cmd), std::end(speed_cmd), 0.0);
        if (g_robot_state.is_valid) {
            g_target_pose = g_robot_state.pose;
            g_cartesian_initialized = true;
            // 初始化位置指令和濾波器
            for(int i=0; i<6; i++) {
                pos_cmd[i] = angle_to_pulse(i, feedback_joint_angles[i]);
                // 初始化濾波器為當前真實角度，防止切換時跳變
                smoothed_target_rad[i] = feedback_joint_angles[i] * M_PI / 180.0;
            }
        } else {
            g_cartesian_initialized = false;
        }
        current_reset_joint = 0;
        last_model_button = global_model_button;
        return; 
    }

    // --- Mode 1: 復位模式 ---
    if (global_model_button == 1) {
        std::fill(std::begin(speed_cmd), std::end(speed_cmd), 0.0);
        double HOME_ERROR_THRESHOLD = 1.5;
        double RESET_P_GAIN = 15.0;        

        if (current_reset_joint < 6) {
            int i = current_reset_joint;
            double encoder_zero_target = -get_joint_offset(i) / JOINT_SCALES[i]; 
            double error = encoder_zero_target - feedback_joint_angles[i];
            
            double spd = std::max(-100.0, std::min(100.0, error * RESET_P_GAIN * speed_scale));
            speed_cmd[i] = spd;
            if (std::abs(error) < HOME_ERROR_THRESHOLD) current_reset_joint++;
        }
    }
    // --- Mode 2: 關節獨立控制 ---
    else if (global_model_button == 2) {
        speed_cmd[0] = get_ctrl_val(msg->left_x) * MAX_SPEED_VEL_MODE[0] * speed_scale;
        speed_cmd[1] = get_ctrl_val(msg->left_y) * MAX_SPEED_VEL_MODE[1] * speed_scale;
        speed_cmd[2] = get_ctrl_val(msg->right_x) * MAX_SPEED_VEL_MODE[2] * speed_scale;
        speed_cmd[3] = get_ctrl_val(msg->right_y) * MAX_SPEED_VEL_MODE[3] * speed_scale;
        speed_cmd[4] = get_ctrl_val(msg->left_button_vra) * MAX_SPEED_VEL_MODE[4] * speed_scale;
        speed_cmd[5] = get_ctrl_val(msg->right_button_vrb) * MAX_SPEED_VEL_MODE[5] * speed_scale;
    }
    // --- Mode 3: 工具坐標系控制 (位置控制 + 濾波) ---
    else if (global_model_button == 3) {
        if (!trac_ik_solver || !g_cartesian_initialized) {
            return;
        }

        // 輸入映射
        double tool_vy = get_ctrl_val(msg->left_y);  // 綠 Y
        double tool_vx = -get_ctrl_val(msg->left_x); // 紅 X
        double tool_vz = get_ctrl_val(msg->right_y); // 藍 Z
        double tool_wz = -get_ctrl_val(msg->right_x);         // 旋轉 Z
        double tool_wx = -get_ctrl_val(msg->left_button_vra); // 旋轉 X
        double tool_wy = get_ctrl_val(msg->right_button_vrb); // 旋轉 Y

        if (tool_vx==0 && tool_vy==0 && tool_vz==0 && tool_wx==0 && tool_wy==0 && tool_wz==0) {
            // 靜止
        } else {
            // [核心優化 2] 降低基準步長，大幅減小單次循環的移動量
            // 0.0001 m = 0.1 mm/cycle，配合 500Hz 循環 -> 50mm/s 基准速度
            double pos_step = 0.0001 * speed_scale; 
            double rot_step = 0.0005 * speed_scale; // 旋轉也放慢

            KDL::Vector v_local(tool_vx, tool_vy, tool_vz);
            g_target_pose.p += (g_target_pose.M * v_local) * pos_step;

            KDL::Rotation rot_local_inc = KDL::Rotation::RotX(tool_wx * rot_step) * KDL::Rotation::RotY(tool_wy * rot_step) * KDL::Rotation::RotZ(tool_wz * rot_step);
            g_target_pose.M = g_target_pose.M * rot_local_inc;
        }

        KDL::JntArray q_out(6);
        int rc = trac_ik_solver->CartToJnt(g_robot_state.joints, g_target_pose, q_out);

        if (rc >= 0) {
            for (int i = 0; i < 6; i++) {
                double raw_target_rad = q_out(i);
                
                // 1. 多圈連續性處理
                double current_rad = g_robot_state.joints(i);
                while (raw_target_rad - current_rad > M_PI)  raw_target_rad -= 2 * M_PI;
                while (raw_target_rad - current_rad < -M_PI) raw_target_rad += 2 * M_PI;

                // [核心優化 3] 低通濾波 (Low-Pass Filter)
                // 本次輸出 = 上次輸出 * 0.92 + 本次目標 * 0.08
                // 這能讓突變的指令變得平滑，消除 IK 解的數值噪聲
                smoothed_target_rad[i] = smoothed_target_rad[i] * (1.0 - FILTER_ALPHA) + raw_target_rad * FILTER_ALPHA;

                double target_deg = smoothed_target_rad[i] * 180.0 / M_PI;
                pos_cmd[i] = angle_to_pulse(i, target_deg);
            }
        } else {
            g_target_pose = g_robot_state.pose;
            // 保持模式也同步更新濾波器，防止解除靜止時跳變
            for(int i=0; i<6; i++) {
                smoothed_target_rad[i] = g_robot_state.joints(i);
                pos_cmd[i] = angle_to_pulse(i, feedback_joint_angles[i]);
            }
        }
    }
}

void process_can_data(const VCI_CAN_OBJ* data, int len, int ch) {
    for (int i = 0; i < len; i++) {
        const VCI_CAN_OBJ& frame = data[i];
        if (frame.Data[0] == 0x08) { 
            int32_t raw_pos = (static_cast<int32_t>(frame.Data[4]) << 24) | 
                              (static_cast<int32_t>(frame.Data[3]) << 16) | 
                              (static_cast<int32_t>(frame.Data[2]) << 8)  | 
                              (static_cast<int32_t>(frame.Data[1]));
            
            int kdl_index = -1;
            for(int j=0; j<6; ++j) {
                if(MOTOR_ID_MAP[j] == frame.ID) { kdl_index = j; break; }
            }
            if (kdl_index >= 0) {
                double scale = JOINT_SCALES[kdl_index];
                double offset = get_joint_offset(kdl_index);
                double angle = (raw_pos - offset) / scale;
                feedback_joint_angles[kdl_index] = angle * JOINT_DIRECTION[kdl_index];
            }
        }
    }
}

void update_robot_realtime_state() {
    if (!fk_solver) return;
    g_robot_state.joints.resize(6);
    for(int i=0; i<6; i++) {
        g_robot_state.joints(i) = feedback_joint_angles[i] * (M_PI / 180.0);
    }
    int rc = fk_solver->JntToCart(g_robot_state.joints, g_robot_state.pose);
    g_robot_state.is_valid = (rc >= 0);
}

int main(int argc, char** argv) {   
    setlocale(LC_ALL, "");
    ros::init(argc, argv, "main_ctrl");
    ros::NodeHandle nh;

    ros::Publisher joint_pub = nh.advertise<sensor_msgs::JointState>("joint_states", 10);
    
    sensor_msgs::JointState joint_msg;
    joint_msg.name = {"J1", "J2", "J3", "J4", "J5", "J6"};
    joint_msg.position.resize(6);

    trac_ik_solver.reset(new TRAC_IK::TRAC_IK("base_link", "J6", "/robot_description", 0.005, 1e-5));
    if (trac_ik_solver->getKDLChain(kdl_chain)) {
        fk_solver.reset(new KDL::ChainFkSolverPos_recursive(kdl_chain));
        ROS_INFO("Solver initialized.");
    } else {
        ROS_ERROR("Failed to init Solver!");
        return -1;
    }
    
    ros::Subscriber sub = nh.subscribe("handle_info", 10, handleCallback);
    driver.set_callback(process_can_data);

    if (!driver.init()) return -1;

    ros::Time last_print_time = ros::Time::now();
    ros::Rate loop_rate(500); 

    while (ros::ok()) {
        update_robot_realtime_state();

        if (global_model_button == 3) {
            // Mode 3: 發送位置指令
            for (int i = 0; i < 6; i++) {
                // 強制轉換 uint16_t 避免編譯警告
                driver.send_single_motor_position_cmd(MOTOR_ID_MAP[i], pos_cmd[i], (uint16_t)MAX_SPEED_POS_MODE[i]);
            }
        } else {
            // Mode 1 & 2: 發送速度指令
            for (int i = 0; i < 6; i++) {
                int32_t final_speed = (int32_t)(speed_cmd[i] * JOINT_DIRECTION[i]);
                driver.send_single_motor_speed_cmd(MOTOR_ID_MAP[i], final_speed);
            }
        }

        for (int i = 0; i < 6; i++) {
            driver.request_motor_position(MOTOR_ID_MAP[i]); 
        }

        joint_msg.header.stamp = ros::Time::now();
        for(int i=0; i<6; i++) joint_msg.position[i] = g_robot_state.joints(i); 
        joint_pub.publish(joint_msg);

        if ((ros::Time::now() - last_print_time).toSec() > 2.0) {
            ROS_INFO("================= ROBOT DEBUG INFO =================");
            ROS_INFO("Mode: %d", global_model_button);
            ROS_INFO("Joints(Deg): %.2f, %.2f, %.2f, %.2f, %.2f, %.2f",
                     feedback_joint_angles[0], feedback_joint_angles[1], feedback_joint_angles[2],
                     feedback_joint_angles[3], feedback_joint_angles[4], feedback_joint_angles[5]);

            if (g_robot_state.is_valid) {
                double r, p, y;
                g_robot_state.pose.M.GetRPY(r, p, y);
                ROS_INFO("Tip Pose (Base): XYZ=[%.3f, %.3f, %.3f] RPY=[%.3f, %.3f, %.3f]", 
                         g_robot_state.pose.p.x(), g_robot_state.pose.p.y(), g_robot_state.pose.p.z(), r, p, y);
            }
            ROS_INFO("----------------------------------------------------");
            last_print_time = ros::Time::now();
        }

        ros::spinOnce();
        loop_rate.sleep();
    }
    return 0;
}