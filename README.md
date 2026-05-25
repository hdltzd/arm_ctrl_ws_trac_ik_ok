# 🤖 6-DOF Robotic Arm Control System (ROS Noetic)

[![ROS](https://img.shields.io/badge/ROS-Noetic-blue.svg)](http://wiki.ros.org/noetic)
[![Platform](https://img.shields.io/badge/Platform-Raspberry%20Pi%20%7C%20Jetson-green)]()
[![License](https://img.shields.io/badge/License-BSD-orange)]()

> 一个基于 ROS 1 (Noetic) 的六自由度机械臂控制系统，专为嵌入式平台（如树莓派、Jetson）设计。集成 **TRAC-IK** 高性能逆运动学解算、**CAN 总线**电机驱动及 **SBUS** 遥控器解析。

![image](https://github.com/hdltzd/arm_ctrl_ws_trac_ik_ok/blob/main/img/%E4%BE%A7%E8%A7%8645%E5%9B%BE.png)
![image](https://github.com/hdltzd/arm_ctrl_ws_trac_ik_ok/blob/main/img/%E5%AE%9E%E6%9C%BA%E5%9B%BE.jpg)

---

## 🎬 效果演示

<div align="center">

### 机械臂测试视频

https://github.com/hdltzd/arm_ctrl_ws_trac_ik_ok/blob/main/img/%E6%9C%BA%E6%A2%B0%E8%87%82%E6%B5%8B%E8%AF%95.mp4

*点击查看完整演示*
**🎥 [观看完整演示视频（Bilibili 5分钟）](暂无)** - 包含详细功能讲解

**🌍 [Watch Full Demo (YouTube 5min)](https://www.youtube.com/watch?v=mu_HsMCqezE)** - English subtitles available

### 负载5kg测试视频

https://github.com/hdltzd/arm_ctrl_ws_trac_ik_ok/blob/main/img/%E8%B4%9F%E8%BD%BD5kg%E6%B5%8B%E8%AF%95.mp4

*点击查看完整演示*
**🎥 [观看完整演示视频（Bilibili 5分钟）](暂无)** - 包含详细功能讲解

**🌍 [Watch Full Demo (YouTube 5min)](https://www.youtube.com/watch?v=mu_HsMCqezE)** - English subtitles available

### 机械臂实地测试视频

https://github.com/hdltzd/arm_ctrl_ws_trac_ik_ok/blob/main/img/%E5%AE%9E%E5%9C%B0%E6%B5%8B%E8%AF%95.mp4

*点击查看完整演示*
**🎥 [观看完整演示视频（Bilibili 5分钟）](暂无)** - 包含详细功能讲解

**🌍 [Watch Full Demo (YouTube 5min)](https://www.youtube.com/watch?v=mu_HsMCqezE)** - English subtitles available
</div>

---

## 📖 目录 (Table of Contents)

- [项目简介](#-项目简介-introduction)
- [核心功能](#-核心功能-features)
- [硬件清单](#-硬件清单-hardware-requirements)
- [⚡ 快速开始 (Quick Start)](#-快速开始-quick-start)
- [🛠️ 详细配置](#-详细配置-configuration)
- [🎮 遥控器操作映射](#-遥控器操作映射-controls)
- [📂 文件结构](#-文件结构-file-structure)
- [常见问题](#-常见问题-troubleshooting)

---

## 📝 项目简介 (Introduction)

本项目旨在提供一个轻量级且鲁棒的机械臂底层控制方案。核心节点 `main_ctrl` 运行一个状态机，支持关节空间直接控制和基于笛卡尔空间的逆运动学 (IK) 控制。

**关键特性：**

- **混合 IK 解算**：集成 `trac_ik_lib`，并发运行 KDL (Newton-Raphson) 和 NLOPT (SQP)，在奇异点附近表现优于原生 KDL。
- **实时 CAN 通信**：基于 `controlcan` 库封装，实现 200Hz 的电机闭环控制。
- **平滑控制**：内置低通滤波器与速度死区处理，消除机械抖动。
- **安全机制**：具备一键清除电机错误、J6 防滑落保护与自动归位功能。

### 系统架构

```
RC遥控器 ─→ SBUS接收机 ─→ USB串口 ─→ sbus_parse ─→ main_ctrl ─→ CAN总线 ─→ 6×电机驱动器
                                            │               │
                                            │               ├── TRAC-IK IK求解 (base_link→J5)
                                            │               └── KDL FK求解 (实时末端位姿)
                                            │
                                            └── 发布 handle_info (摇杆/拨杆/旋钮状态)
```

### 仓库组成

| 包 (Package) | 说明 |
| :--- | :--- |
| `arm_pkg` | 机械臂主控包（控制节点、URDF 模型、CAN 驱动、SBUS 解析） |
| `trac_ik` | TRAC-IK 运动学求解库（元包 + lib + MoveIt! 插件 + Python 绑定） |

---

## ✨ 核心功能 (Features)

### 控制模式

| 模式 (Mode) | 拨杆位置 | 名称 | 描述 |
| :--- | :--- | :--- | :--- |
| **Mode 1** | 拨杆 A 上档 | **自动复位 (Homing)** | J1~J5 依次自动运动到预设目标角度，到位后切换下一关节。 |
| **Mode 2** | 拨杆 A 中档 | **关节控制 (Joint)** | 通过摇杆直接控制 6 个关节的速度，适合调试与维护。 |
| **Mode 3** | 拨杆 A 下档 | **笛卡尔控制 (IK)** | 控制末端工具在 XYZ 空间平移及旋转（J1~J5 IK 解算，J6 独立电流控制）。 |

### ROS 话题列表

| 话题 | 类型 | 频率 | 说明 |
| :--- | :--- | :--- | :--- |
| `/joint_states` | `sensor_msgs/JointState` | 200Hz | 6关节位置(rad) + 电流(mA，存入 effort 字段) |
| `/robot_pose` | `geometry_msgs/PoseStamped` | 200Hz | J5 末端在 base_link 中的位姿 |
| `/robot_mode` | `std_msgs/Int32` | 200Hz | 当前控制模式 (1/2/3) |
| `/j6_safety_status` | `std_msgs/Int32` | 200Hz | J6 安全状态 (1=安全, 0=不安全) |
| `/handle_info` | `arm_pkg/handle_info` | 500Hz | 遥控器处理后输入（摇杆/拨杆/旋钮） |
| `/sbus_channels` | `std_msgs/UInt16MultiArray` | 500Hz | SBUS 16 通道原始值 |

### 安全保护

- **J6 末端防滑落**：实时计算 J6 坐标系 Y 轴在世界坐标系中的 Z 分量。若 Z > 0（夹爪朝上），立即禁止 J1~J5 运动，防止末端夹具滑落。
- **IK 预测检查**：Mode 3 中 IK 求解结果在执行前先预测 J6 姿态，不安全则拒绝执行该帧指令。
- **一键清除错误**：拨杆 C 拨到下档位时持续发送电机错误清除指令 (0x0B)。
- **信号超时保护**：SBUS 信号丢失超过 0.3 秒自动断开连接，所有通道归零。

---

## ⚙️ 硬件清单 (Hardware Requirements)

| 硬件 | 型号/规格 | 用途 |
| :--- | :--- | :--- |
| **计算平台** | Raspberry Pi 4B/5 或 NVIDIA Jetson (Ubuntu 20.04) | 运行 ROS 控制程序 |
| **USB-CAN** | 周立功 VCI_USBCAN2 兼容适配器 (如 USBCAN-2C/II) | 与电机驱动器通信 |
| **USB-TTL** | CP2102 / CH340 等 | 连接 SBUS 接收机 |
| **RC 遥控器** | 支持 SBUS 协议的航模遥控器 (Futaba, RadioLink AT9S 等) | 操控输入 |
| **SBUS 接收机** | 输出标准 25 字节 SBUS 帧 | 遥控信号接收 |
| **执行机构** | 6 自由度 CAN 总线驱动机械臂 | 执行动作 |

---

## ⚡ 快速开始 (Quick Start)

如果您已经配置好 Ubuntu 20.04 和 ROS Noetic 环境，请按以下步骤部署：

### 1. 安装依赖

```bash
# 系统级依赖
sudo apt update
sudo apt install ros-noetic-serial ros-noetic-plotjuggler-ros ros-noetic-kdl-parser ros-noetic-nlopt

# 安装 Catkin Tools（推荐）
sudo apt install python3-catkin-tools

# 安装 Eigen3（trac_ik_lib 编译必需）
sudo apt install libeigen3-dev

# 安装 USB-CAN 驱动库
# 将 arm_pkg/lib/libcontrolcan.so 复制到系统库路径
sudo cp ~/catkin_ws/src/arm_pkg/lib/libcontrolcan.so /usr/local/lib/
sudo ldconfig
```

### 2. 编译工作空间

```bash
# 创建并进入工作空间
mkdir -p ~/catkin_ws/src && cd ~/catkin_ws/src

# 克隆代码仓库
git clone https://github.com/Autonomous-Robotics-Lab-Team/arm_pkg.git

# 注：trac_ik 须与 arm_pkg 一起放入 src 目录，确保目录结构如下：
#   ~/catkin_ws/src/arm_pkg/
#   ~/catkin_ws/src/trac_ik/

# 先编译 trac_ik_lib（arm_pkg 编译依赖）
cd ~/catkin_ws
catkin build trac_ik_lib

# 再完整编译
catkin build

# 刷新环境
source devel/setup.bash
```

### 3. 运行测试

```bash
# 赋予 USB 设备权限
sudo chmod 666 /dev/ttyUSB0 /dev/usbcan*

# 启动 SBUS 解析节点（测试遥控器通信）
roslaunch arm_pkg sbus_parse.launch

# 在另一终端打开 PlotJuggler 查看数据
rosrun plotjuggler plotjuggler

# 启动完整系统（SBUS解析 + 主控 + RViz 可视化）
roslaunch arm_pkg arm_ctrl.launch

# GDB 调试模式
roslaunch arm_pkg arm_ctrl.launch debug:=true
```

---

## 🛠️ 详细配置 (Configuration)

### 电机标定参数

在 `arm_pkg/src/main_ctrl.cpp` 中修改：

```cpp
// 减速比系数 (每度对应编码器脉冲数)
const double SCALE_DEFAULT = 654784.0 / (90.0 - 54.3878);

// 零点偏置
double get_joint_offset(int i) {
    return 1000000.0 - JOINT_SCALES[i] * 54.3878;
}

// 关节方向 (1.0 正转, -1.0 反转)
const double JOINT_DIRECTION[6] = {-1.0, -1.0, 1.0, 1.0, -1.0, -1.0};

// 电机 ID 映射
const uint8_t MOTOR_ID_MAP[6] = {1, 2, 3, 4, 5, 6};
```

### 关节限位

编辑 `arm_pkg/urdf/arm.urdf` 中各关节的 `<limit>` 标签：

```xml
<limit lower="-3.14" upper="3.14" effort="100" velocity="100" />
```

### TRAC-IK 参数

在 `main_ctrl.cpp` 第 477 行调整 IK 求解器精度和超时：

```cpp
// 参数: base_link, tip_link, URDF参数, 超时(s), 精度(m)
trac_ik_solver.reset(new TRAC_IK::TRAC_IK("base_link", "J5", "/robot_description", 0.006, 1e-3));
```

### 开机自启 (systemd)

```bash
# 编辑启动脚本中的路径
vim ~/catkin_ws/src/arm_pkg/scripts/arm_ctrl_start.sh

# 安装 systemd 服务
sudo cp ~/catkin_ws/src/arm_pkg/scripts/arm_ctrl.service /etc/systemd/system/
sudo systemctl daemon-reload
sudo systemctl enable arm_ctrl
sudo systemctl start arm_ctrl

# 查看状态和日志
sudo systemctl status arm_ctrl
tail -f /var/log/arm_ctrl.log
```

---

## 🎮 遥控器操作映射 (Controls)

```
         左摇杆                          右摇杆
      [Mode 2: J1底座/J2肩]          [Mode 2: J3肘/J4腕旋转]
      [Mode 3: 前后平移/左右平移]     [Mode 3: 上下平移/绕Z旋转]

      VRA 旋钮                         VRB 旋钮
  [Mode 2: J5 腕俯仰]              [Mode 2: J6 夹爪]
  [Mode 3: 绕X旋转]                [Mode 3: J6 夹爪电流]

┌─────────── 拨杆映射 ───────────┐
│ 拨杆 A （左1）  → 模式切换     │
│   上档 (1) → 复位模式          │
│   中档 (2) → 关节控制模式       │
│   下档 (3) → 笛卡尔控制模式     │
│                                │
│ 拨杆 B （左2）  → 速度挡位     │
│   上档 → 0.4×  (慢)           │
│   中档 → 0.8×  (中)           │
│   下档 → 1.6×  (快)           │
│                                │
│ 拨杆 C （右1）  → 错误清除     │
│   下档 → 持续清除电机错误      │
│                                │
│ 拨杆 D （右2）  → 预留        │
└────────────────────────────────┘
```

---

## 📂 文件结构 (File Structure)

```
src/
├── arm_pkg/                              # 机械臂控制包
│   ├── CMakeLists.txt                    # 构建配置
│   ├── package.xml                       # 包依赖声明
│   ├── include/
│   │   ├── arm_pkg/sbus_parse.h          # SBUS 遥控器数据结构
│   │   ├── canbus_driver.h               # CAN 驱动封装接口
│   │   └── controlcan.h                  # USB-CAN 适配器 SDK 头文件
│   ├── launch/
│   │   ├── arm_ctrl.launch               # 主启动文件 (全部节点 + RViz)
│   │   └── sbus_parse.launch             # 单独 SBUS 解析节点
│   ├── msg/
│   │   └── handle_info.msg               # 自定义消息：遥控器输入
│   ├── src/
│   │   ├── main_ctrl.cpp                 # ★ 主控制节点 (IK/FK/安全/状态机)
│   │   ├── sbus_parse.cpp                # SBUS 协议解析节点
│   │   ├── canbus_driver.cpp             # CAN 总线驱动封装
│   │   └── sub_pub.cpp                   # [废弃] 旧版控制逻辑
│   ├── urdf/
│   │   └── arm.urdf                      # 机械臂 URDF 模型 (SolidWorks 导出)
│   ├── meshes/                           # 各连杆 STL 碰撞网格
│   ├── rviz/
│   │   └── arm.rviz                      # RViz 3D 可视化配置
│   ├── lib/
│   │   └── libcontrolcan.so              # USB-CAN 适配器动态库
│   └── scripts/
│       ├── arm_ctrl_start.sh             # 开机自启脚本
│       └── arm_ctrl.service              # systemd 服务文件
│
└── trac_ik/                              # TRAC-IK 求解库 (第三方)
    ├── trac_ik/                          # 元包 (metapackage)
    ├── trac_ik_lib/                      # 核心求解库 .so
    ├── trac_ik_kinematics_plugin/        # MoveIt! IK 插件
    ├── trac_ik_examples/                 # 使用示例
    └── trac_ik_python/                   # Python SWIG 绑定
```

## 📄 参考与致谢

- [TRAC-IK: An Open-Source Library for Improved Solving of Generic Inverse Kinematics](https://www.researchgate.net/publication/282852814) — P. Beeson, B. Ames, IEEE Humanoids 2015
- [TRACLabs Robotics](https://traclabs.com/) — TRAC-IK 原作者团队
- [ROS Industrial](https://rosindustrial.org/) — KDL 和 URDF 标准
