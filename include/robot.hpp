#pragma once

#include "lemlib/api.hpp"

// 声明 robot.cpp 中唯一的底盘对象，其他文件可以直接调用它。
extern lemlib::Chassis chassis;

void initializeRobot();
void stopRobot();
