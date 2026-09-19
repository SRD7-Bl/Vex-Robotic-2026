#include "main.h"
#include "robot.hpp"
#include "routines.hpp"

namespace routines {
void autonomous() {
    if (!robot::drive::ready()) {
        pros::lcd::set_text(1, "Autonomous skipped: no chassis");
        return;
    }
    pros::lcd::set_text(1, "Autonomous: no routine selected");
    // 确认起点、方向并完成实机调试后，再启用自己的动作。
    // 以下仅展示写法，默认不执行；每个动作会等待结束或超时。
    // robot::drive::setPose(0, 0, 0);
    // robot::drive::moveToPoint(0, 12, 2000);
    // robot::drive::turnToHeading(90, 1500);
    // 不自动运行去年模板的示例路线或 example.txt。
}
} // namespace routines
