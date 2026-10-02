//这个文件负责对象声明：让其他文件先知道这些名字
#pragma once

#include "lemlib/api.hpp"

// 声明 robot.cpp 中唯一的底盘对象，其他文件可以直接调用它。
extern lemlib::Chassis chassis;
extern pros::Motor IntakeMotorLeft;
extern pros::Motor IntakeMotorRight;
extern pros::Motor LiftMotorLeft;
extern pros::Motor LiftMotorRight;
extern pros::Motor ClampMotor;

void initializeRobot();
void stopRobot();
