//这个文件负责对象声明：让其他文件先知道这些名字
/*
robot.hpp	Shared Hardware Object Declarations — Declares shared hardware objects, allowing other source files to reference them.
robot_config.hpp	Hardware Configuration — Centralizes port assignments, hardware settings, and control parameters, such as PID constants.
robot.cpp	Hardware Object Definitions & Initialization — Instantiates shared hardware objects and implements robot initialization procedures.
routines.hpp	Function Interface Declarations — Declares reusable control functions, making them accessible across different source files.
main.cpp	Competition Lifecycle Management — Coordinates program execution and dispatches the appropriate functions based on the current competition mode.
autonomous.cpp	Autonomous Control Logic — Implements predefined autonomous routines and coordinates their movement and mechanism sequences.
driver.cpp	Driver Control Logic — Processes controller inputs and translates them into drivetrain and mechanism commands.
*/
#pragma once

#include "lemlib/api.hpp"

// 声明 robot.cpp 中唯一的底盘对象，其他文件可以直接调用它。
extern lemlib::Chassis chassis;
extern pros::Motor IntakeMotorLeft;
extern pros::Motor IntakeMotorRight;
extern pros::Motor LiftMotorLeft;
extern pros::Motor LiftMotorRight;
extern pros::Motor ClampMotor;
extern pros::Motor ToggleMotor;


void initializeRobot();
void stopRobot();
