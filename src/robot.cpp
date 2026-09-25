#include "main.h"
#include "lemlib/api.hpp" // IWYU pragma: keep
#include "robot.hpp"
#include "robot_config.hpp"
#include <cmath>
#include <vector>

namespace robot {
namespace {
#if ROBOT_ENABLE_CHASSIS
// 编译时拦住未填写、越界或重复的端口，不能仅打开开关就使用占位参数。
constexpr bool validPorts() {
    bool used[22]{}; //used检查有没有多个hardware接到一个端口里了
    for (auto ports : {config::leftMotorPorts, config::rightMotorPorts}) { //左电机列表/右电机列表
        if (ports.size() == 0) return false; //没有端口
        for (int port : ports) {
            const int physicalPort = port < 0 ? -port : port;
            if (physicalPort < 1 || physicalPort > 21 || used[physicalPort]) return false; //如果占用了或者越界了
            used[physicalPort] = true;
        }
    }
    //static_cast<int> 把不同整数类型同义转换成 int
    for (int port : {static_cast<int>(config::imuPort),
                     static_cast<int>(config::verticalRotationPort),
                     static_cast<int>(config::horizontalRotationPort)}) { // IMU和Rotation port
        const int physicalPort = port < 0 ? -port : port;
        if (physicalPort < 1 || physicalPort > 21 || used[physicalPort]) return false;
        used[physicalPort] = true;
    }
    return true;
}
//static_assert(必须成立的条件, "条件不成立时显示的错误信息");
static_assert(validPorts(), "Fill in unique real motor, IMU and tracking sensor ports in robot_config.hpp");
static_assert(config::leftMotorPorts.size() == config::leftMotorGearsets.size() &&
                  config::rightMotorPorts.size() == config::rightMotorGearsets.size(),
              "Each motor port must have a matching gearset in the same order");
constexpr bool validGearsets() {
    for (auto gearsets : {config::leftMotorGearsets, config::rightMotorGearsets}) {
        for (auto gearset : gearsets) {
            if (gearset != pros::MotorGearset::red && gearset != pros::MotorGearset::green &&
                gearset != pros::MotorGearset::blue) return false;
        }
    }
    return true;
}
static_assert(validGearsets(), "Configure a red, green or blue gearset for every motor");
static_assert(config::trackWidth > 0 && config::driveWheelDiameter > 0 &&
                  config::driveWheelRpm > 0 && config::trackingWheelDiameter > 0,
              "Fill in measured chassis dimensions and wheel RPM in robot_config.hpp");

// 构造时不统一指定齿轮盒；initialize() 中按列表分别配置各电机。
pros::MotorGroup leftMotors(config::leftMotorPorts);
pros::MotorGroup rightMotors(config::rightMotorPorts);
pros::Imu imu(config::imuPort);
pros::Rotation verticalEnc(config::verticalRotationPort);
pros::Rotation horizontalEnc(config::horizontalRotationPort);
lemlib::TrackingWheel vertical(&verticalEnc, config::trackingWheelDiameter, config::verticalOffset);
lemlib::TrackingWheel horizontal(&horizontalEnc, config::trackingWheelDiameter, config::horizontalOffset);

lemlib::Drivetrain drivetrain(&leftMotors, &rightMotors, config::trackWidth,
                             config::driveWheelDiameter, config::driveWheelRpm, config::horizontalDrift);

// 去年模板的 PID 起点，不代表今年已经调好。
// 参数顺序：kP, kI, kD, antiWindup, 小误差范围/时间, 大误差范围/时间, slew。
// 直线误差用英寸，角度误差用度，时间用毫秒。
lemlib::ControllerSettings linearController(10, 0, 3, 3, 1, 100, 3, 500, 20);
lemlib::ControllerSettings angularController(2, 0, 10, 3, 1, 100, 3, 500, 0);
lemlib::OdomSensors sensors(&vertical, nullptr, &horizontal, nullptr, &imu);

// 参数顺序：摇杆死区、最小输出、指数增益；沿用去年参考值。
lemlib::ExpoDriveCurve throttleCurve(3, 10, 1.019);
lemlib::ExpoDriveCurve steerCurve(3, 10, 1.019);
lemlib::Chassis chassis(drivetrain, linearController, angularController,
                        sensors, &throttleCurve, &steerCurve);
bool chassisReady = false;

bool hardwareAvailable() { //检查hardware是否齐全和正常
    if (!imu.is_installed() || !verticalEnc.is_installed() || !horizontalEnc.is_installed()) return false;
    for (double velocity : leftMotors.get_actual_velocity_all()) {
        if (!std::isfinite(velocity)) return false;
    }
    for (double velocity : rightMotors.get_actual_velocity_all()) {
        if (!std::isfinite(velocity)) return false;
    }
    return true;
}
#endif
} // namespace

namespace drive {
bool ready() {
#if ROBOT_ENABLE_CHASSIS
    return chassisReady;
#else
    return false;
#endif
}

void arcade(int forward, int turn) {
#if ROBOT_ENABLE_CHASSIS
    if (ready()) chassis.arcade(forward, turn);
#else
    (void)forward;
    (void)turn;
#endif
}

void stop() {
#if ROBOT_ENABLE_CHASSIS
    chassis.cancelAllMotions();
    leftMotors.move(0);
    rightMotors.move(0);
#endif
}

void setPose(float x, float y, float heading) {
#if ROBOT_ENABLE_CHASSIS
    if (ready()) chassis.setPose(x, y, heading);
#else
    (void)x;
    (void)y;
    (void)heading;
#endif
}

void moveToPoint(float x, float y, int timeoutMs) {
#if ROBOT_ENABLE_CHASSIS
    if (ready() && timeoutMs > 0) chassis.moveToPoint(x, y, timeoutMs, {}, false);
#else
    (void)x;
    (void)y;
    (void)timeoutMs;
#endif
}

void turnToHeading(float heading, int timeoutMs) {
#if ROBOT_ENABLE_CHASSIS
    if (ready() && timeoutMs > 0) chassis.turnToHeading(heading, timeoutMs, {}, false);
#else
    (void)heading;
    (void)timeoutMs;
#endif
}
} // namespace drive

void stop() {
    drive::stop();
    // Add mechanism stop calls here as those modules are introduced.
}

void initialize() {
    pros::lcd::initialize();
#if ROBOT_ENABLE_CHASSIS
    chassisReady = false;
    stop();
    if (!hardwareAvailable()) {
        pros::lcd::set_text(0, "Chassis unavailable: check ports");
        return;
    }
    // set_gearing(vector) 按电机组内的顺序设置每个电机，不改变端口反转。
    if (leftMotors.set_gearing(std::vector<pros::MotorGearset>(config::leftMotorGearsets)) != 1 ||
        rightMotors.set_gearing(std::vector<pros::MotorGearset>(config::rightMotorGearsets)) != 1) {
        pros::lcd::set_text(0, "Motor gearset setup failed");
        stop();
        return;
    }
    pros::lcd::set_text(0, "Calibrating: keep robot still");
    chassis.calibrate();
    if (!hardwareAvailable() || imu.is_calibrating() || !std::isfinite(imu.get_heading())) {
        pros::lcd::set_text(0, "Calibration failed: check IMU");
        stop();
        return;
    }
    chassisReady = true;
    pros::lcd::set_text(0, "Chassis ready");

    // 无硬件模式不会启动定位任务；屏幕每次读取同一份 pose。
    static pros::Task screenTask([] {
        while (true) {
            const auto pose = chassis.getPose();
            pros::lcd::print(2, "X: %.2f", pose.x);
            pros::lcd::print(3, "Y: %.2f", pose.y);
            pros::lcd::print(4, "Heading: %.2f", pose.theta);
            pros::delay(config::screenPeriodMs);
        }
    });
#else
    pros::lcd::set_text(0, "Framework: no chassis configured");
    pros::lcd::set_text(2, "Hardware disabled in config");
#endif
}
} // namespace robot
