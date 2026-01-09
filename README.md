# 🤖 6-DOF Robotic Arm Control System (ROS Noetic)

[![ROS](https://img.shields.io/badge/ROS-Noetic-blue.svg)](http://wiki.ros.org/noetic)
[![Platform](https://img.shields.io/badge/Platform-Raspberry%20Pi%20%7C%20Jetson-green)]()
[![License](https://img.shields.io/badge/License-BSD-orange)]()

> 一个基于 ROS 1 (Noetic) 的六自由度机械臂控制系统，专为嵌入式平台（如树莓派、Jetson）设计。集成 **TRAC-IK** 高性能逆运动学解算、**CAN 总线**电机驱动及 **SBUS** 遥控器解析。

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
* **混合 IK 解算**：集成 `trac_ik_lib`，并发运行 KDL (Newton-Raphson) 和 NLOPT (SQP)，在奇异点附近表现优于原生 KDL。
* **实时 CAN 通信**：基于 `controlcan` 库封装，实现 200Hz 的电机闭环控制。
* **平滑控制**：内置低通滤波器与速度死区处理，消除机械抖动。
* **安全机制**：具备一键清除电机错误与自动归位功能。

---

## ✨ 核心功能 (Features)

| 模式 (Mode) | 名称 | 描述 |
| :--- | :--- | :--- |
| **Mode 1** | **自动复位 (Homing)** | 机械臂自动寻找零位，逐个关节归零，回到初始姿态。 |
| **Mode 2** | **关节控制 (Joint)** | 通过摇杆直接控制 6 个关节的速度，适合调试与维护。 |
| **Mode 3** | **笛卡尔控制 (IK)** | 控制末端工具在 XYZ 空间平移及旋转 (J1-J5 IK解算, J6 独立控制)。 |

---

## ⚙️ 硬件清单 (Hardware Requirements)

1.  **计算平台**: Raspberry Pi 4B/5 或 NVIDIA Jetson (Ubuntu 20.04).
2.  **通信模块**:
    * **USB-CAN**: 支持 ZLG 协议的适配器 (如 USBCAN-2C/II).
    * **USB-TTL**: 用于连接 SBUS 接收机.
3.  **输入设备**: 支持 SBUS 协议的航模遥控器 (如 Futaba, RadioLink, AT9S).
4.  **执行机构**: 6自由度 CAN 总线驱动机械臂.

---

## ⚡ 快速开始 (Quick Start)

如果您已经配置好 Ubuntu 20.04 和 ROS Noetic 环境，请按以下步骤部署：

### 1. 安装依赖
```bash
# 安装系统级依赖
sudo apt update
sudo apt install ros-noetic-serial ros-noetic-plotjuggler-ros ros-noetic-kdl-parser ros-noetic-nlopt

# (可选) 安装 Catkin Tools
sudo apt install python3-catkin-tools
