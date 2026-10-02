#include "main.h"
#include "robot.hpp"
#include "routines.hpp"

void runAutonomous() {
    pros::lcd::set_text(1, "Autonomous: no routine selected");
    // 完成硬件配置和调参后，把自动动作写在这里。
    // 坐标用英寸，角度用度，超时用毫秒。
    // 最后的 false 表示等待动作结束或超时，再执行下一行。
    // chassis.setPose(0, 0, 0);
    // chassis.moveToPoint(0, 12, 2000, {}, false);
    // chassis.turnToHeading(90, 1500, {}, false);
}
