本项目是一个基于 ROS 1 (Noetic) 的六轴机械臂控制系统，专为 Raspberry Pi / Jetson 等嵌入式平台设计。
系统集成了 TRAC-IK 高性能逆运动学求解器、CAN 总线电机驱动以及 SBUS 遥控器解析，支持关节空间控制、笛卡尔空间运动控制（IK）以及自动归位功能。

核心功能
多模式控制架构：
Mode 1 (自动复位)：一键执行关节逐个归零，自动回到初始姿态。
Mode 2 (关节控制)：独立控制 6 个关节的速度，适合调试与粗调。
Mode 3 (笛卡尔 IK 控制)：利用 TRAC-IK 实时解算，直接控制末端工具在 XYZ 空间平移及旋转（J6 独立控制夹爪）。
混合解算算法：集成 trac_ik_lib，并发运行 KDL（牛顿-拉夫逊法）和 NLOPT（SQP 优化），解决传统 KDL 在奇异点解算失败的问题。
嵌入式优化：针对树莓派环境优化，支持通过 Workspace Overlay 链接外部 serial 库。
安全保护：内置低通滤波平滑算法、速度死区控制及电机错误清除机制。

硬件清单
计算平台：Raspberry Pi 4B / 5 或 NVIDIA Jetson (需运行 Ubuntu 20.04 + ROS Noetic)。
通信模块：
USB-CAN 适配器：支持 ZLG 协议 (如 USBCAN-2C/II)。
USB-TTL 模块：用于连接 SBUS 接收机。
输入设备：支持 SBUS 协议的航模遥控器（如 Futaba, RadioLink）。
机械臂：6 自由度 CAN 总线机械臂。

依赖环境
OS: Ubuntu 20.04 (Focal)
ROS: Noetic
外部工作空间: serial_ws (位于 /home/pi/serial_ws) - 必须存在，用于提供串口通信库。
编译与安装 (关键步骤)
由于本项目依赖外部的 serial 库环境，请务必严格按照以下顺序操作：
