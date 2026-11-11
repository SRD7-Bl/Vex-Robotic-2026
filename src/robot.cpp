//这个文件负责实际对象定义：创建这个硬件对象以及进行初始化机器人
#include "main.h"
#include "robot.hpp"
#include "robot_config.hpp"
#include <vector>

// TODO：底盘尺寸和轮速仍未填写，先完成配置再运行。
// 构造时不统一指定齿轮盒；initializeRobot() 中按列表分别配置各电机。
// 底盘定义
pros::MotorGroup leftMotors(leftMotorPorts);
pros::MotorGroup rightMotors(rightMotorPorts);

// IMU定义
pros::Imu imu(imuPort);

// Rotational Sensor + Odometry定义
pros::Rotation verticalEnc(verticalRotationPort);
pros::Rotation horizontalEnc(horizontalRotationPort);
lemlib::TrackingWheel vertical(&verticalEnc, trackingWheelDiameter, verticalOffset);
lemlib::TrackingWheel horizontal(&horizontalEnc, trackingWheelDiameter, horizontalOffset);

// Mechanism Motor定义
pros::Motor LiftMotorLeft(liftMotorLeftPort, MechanismGearset);
pros::Motor LiftMotorRight(liftMotorRightPort, MechanismGearset);
pros::Motor IntakeMotorLeft(intakeMotorLeftPort, MechanismGearset);
pros::Motor IntakeMotorRight(intakeMotorRightPort, MechanismGearset);
pros::Motor ClampMotor(clampMotorPort, MechanismGearset);
pros::Motor ToggleMotor(toggleMotorPort,MechanismGearset);



//Drivertrain 组装
lemlib::Drivetrain drivetrain(&leftMotors, &rightMotors, trackWidth,
                             driveWheelDiameter, driveWheelRpm, horizontalDrift);

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



void stopRobot() {
    chassis.cancelAllMotions();
    leftMotors.move(0);
    rightMotors.move(0);
    
    IntakeMotorLeft.move(0);
    IntakeMotorRight.move(0);
    LiftMotorLeft.move(0);
    LiftMotorRight.move(0);
    ClampMotor.move(0);
    ToggleMotor.move(0);
    
    // 以后新增机构时，在这里加入机构停止代码。
}

void initializeRobot() {
    pros::lcd::initialize();
    // 齿轮盒列表与端口列表顺序一致，分别配置各电机。
    leftMotors.set_gearing(std::vector<pros::MotorGearset>(leftMotorGearsets));
    rightMotors.set_gearing(std::vector<pros::MotorGearset>(rightMotorGearsets));
    stopRobot();
    chassis.calibrate();

    //LiftMotorLeft.set_brake_mode(pros::E_MOTOR_BRAKE_HOLD);
    //LiftMotorRight.set_brake_mode(pros::E_MOTOR_BRAKE_HOLD);
    //IntakeMotorLeft.set_brake_mode(pros::E_MOTOR_BRAKE_HOLD);
    //IntakeMotorRight.set_brake_mode(pros::E_MOTOR_BRAKE_HOLD);

    static pros::Task screenTask([] {
        while (true) {
            const auto pose = chassis.getPose();
            pros::lcd::print(2, "X: %.2f", pose.x);
            pros::lcd::print(3, "Y: %.2f", pose.y);
            pros::lcd::print(4, "Heading: %.2f", pose.theta);
            pros::delay(screenPeriodMs);
        }
    });
}
