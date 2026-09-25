#pragma once

#include "api.h"
#include <cstdint>
#include <initializer_list>

// 0 = 只搭框架，不创建底盘硬件、不校准、不发送电机指令。
// 填完今年的真实参数并接好硬件后，再改为 1。
#define ROBOT_ENABLE_CHASSIS 0

namespace robot::config {
// 电机数量按实际填写；负端口表示反转。空列表/0 表示尚未配置。
inline constexpr std::initializer_list<std::int8_t> leftMotorPorts{17, 18, -19};
inline constexpr std::initializer_list<std::int8_t> rightMotorPorts{-1, 2, 20};
// 与上面的端口顺序一一对应：19、1 号为绿色，其余为蓝色。
// 同一电机组混用齿轮盒，不能给整组统一设置一个 gearset。
inline constexpr std::initializer_list<pros::MotorGearset> leftMotorGearsets{
    pros::MotorGearset::blue, pros::MotorGearset::blue, pros::MotorGearset::green};
inline constexpr std::initializer_list<pros::MotorGearset> rightMotorGearsets{
    pros::MotorGearset::green, pros::MotorGearset::blue, pros::MotorGearset::blue};
inline constexpr std::uint8_t imuPort = 0;

// 暂沿用去年“一纵一横 Rotation 追踪轮 + IMU”的结构。
// 如果今年不用这个布局，先修改 robot.cpp 的 TrackingWheel / OdomSensors。
inline constexpr std::int8_t verticalRotationPort = 0;
inline constexpr std::int8_t horizontalRotationPort = 0;
inline constexpr float trackingWheelDiameter = 0; // 英寸
inline constexpr float verticalOffset = 0;        // 英寸，带正负号
inline constexpr float horizontalOffset = 0;      // 英寸，带正负号

inline constexpr float trackWidth = 0;         // 左右驱动轮中心间距，英寸
inline constexpr float driveWheelDiameter = 0; // 英寸；采用对应轮型的实际直径
inline constexpr float driveWheelRpm = 0;      // 传动后轮子的转速，不是电机转速
// 绿色/蓝色混用时，需确认各自的外部传动比后再填写共同的轮速。
inline constexpr float horizontalDrift = 2;    // 去年参考值，需要重新调试

inline constexpr int driverPeriodMs = 10;     // 沿用去年手柄更新周期
inline constexpr int screenPeriodMs = 50;
} // namespace robot::config
