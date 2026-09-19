#include "main.h"
#include "robot.hpp"
#include "robot_config.hpp"
#include "routines.hpp"

namespace routines {
void driver() {
    if (!robot::drive::ready()) {
        pros::lcd::set_text(1, "Driver idle: no chassis");
        return;
    }

    pros::Controller controller(pros::E_CONTROLLER_MASTER);
    pros::lcd::set_text(1, "Mode: driver (arcade)");
    while (!pros::competition::is_disabled() &&
           !pros::competition::is_autonomous()) {
        const int forward = controller.get_analog(pros::E_CONTROLLER_ANALOG_LEFT_Y);
        const int turn = controller.get_analog(pros::E_CONTROLLER_ANALOG_RIGHT_X);
        robot::drive::arcade(forward, turn);
        // 后续的 intake / lift 等机构手柄控制放在这里，不要加入阻塞等待。
        pros::delay(robot::config::driverPeriodMs);
    }
    robot::stop();
}
} // namespace routines
