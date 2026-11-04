#include "main.h"

// 独立测试：15 号正转，5 号反转，两台均为绿色齿轮盒。
pros::Motor testMotor15(15, pros::MotorGearset::green);
pros::Motor testMotor5(-5, pros::MotorGearset::green);
pros::Controller testController(pros::E_CONTROLLER_MASTER);

void stopTestMotors() {
    testMotor15.move(0);
    testMotor5.move(0);
}

void initialize() {
    pros::lcd::initialize();
    pros::lcd::set_text(0, "Motor test: 15 / reversed 5");
    pros::lcd::set_text(1, "Left stick forward to run");
    stopTestMotors();
}

void disabled() {
    stopTestMotors();
}

void competition_initialize() {
    stopTestMotors();
}

void autonomous() {
    stopTestMotors();
}

void opcontrol() {
    stopTestMotors();
    while (!pros::competition::is_disabled() &&
           !pros::competition::is_autonomous()) {
        const int forward = testController.get_analog(pros::E_CONTROLLER_ANALOG_LEFT_Y);
        // 只响应向前推；回中、死区内或向后推均为零输出。
        const int output = forward > 3 ? forward : 0;
        testMotor15.move(output);
        testMotor5.move(output);
        pros::delay(10);
    }
    stopTestMotors();
}
