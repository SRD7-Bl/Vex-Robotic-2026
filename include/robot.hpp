#pragma once

namespace robot {
void initialize();
// Must cancel active motion and stop every mechanism when hardware is added.
void stop();

namespace drive {
// 只有启用硬件并完成初始化后才为 true；不是运行期间的故障监测。
bool ready();
// Joystick commands use the PROS range [-127, 127].
void arcade(int forward, int turn);
void stop();
// 坐标用英寸，角度用度；无硬件/未就绪时这些调用不执行动作。
void setPose(float x, float y, float heading);
// 顺序执行：等动作结束或超时才返回；返回不保证已到达目标。
void moveToPoint(float x, float y, int timeoutMs);
void turnToHeading(float heading, int timeoutMs);
} // namespace drive
} // namespace robot
