#pragma once

#include "api.h"
#include <cstdint>
#include <initializer_list>

// 完成底盘参数配置后才能运行。
// 电机数量按实际填写；负端口表示反转。空列表/0 表示尚未配置。
inline constexpr std::initializer_list<std::int8_t> leftMotorPorts{17, 18, -19};
inline constexpr std::initializer_list<std::int8_t> rightMotorPorts{-1, 2, 20};
// 与上面的端口顺序一一对应：19、1 号为绿色，其余为蓝色。
// 同一电机组混用齿轮盒，不能给整组统一设置一个 gearset。
inline constexpr std::initializer_list<pros::MotorGearset> leftMotorGearsets{
    pros::MotorGearset::blue, pros::MotorGearset::blue, pros::MotorGearset::green};
inline constexpr std::initializer_list<pros::MotorGearset> rightMotorGearsets{
    pros::MotorGearset::green, pros::MotorGearset::blue, pros::MotorGearset::blue};
inline constexpr std::uint8_t imuPort = 10;

// 沿用去年“一纵一横 Rotation 追踪轮 + IMU”的结构。
// 反转时使用负端口。
inline constexpr std::int8_t verticalRotationPort = 16;
inline constexpr std::int8_t horizontalRotationPort = 3;
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
