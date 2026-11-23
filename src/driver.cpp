#include "main.h"
#include "robot.hpp"
#include "robot_config.hpp"
#include "routines.hpp"
#include "field_coordinates.hpp"
/*
双摇杆操作方式
左摇杆负责前后，右摇杆负责转向

按键映射表格:
R1/R2 Lift; R1向上 R2向下
L1/L2 Intake; L1 Take in L2 Take out
X: Toggle flipping
*/
const int LIFTUPSPEED = 127;
const int LIFTDOWNSPEED = -127;

const int INTAKEINSPEED = 127;
const int INTAKEOUTSPEED = -37;

const int FLIPPINGTIME = 200; // Unit:ms

bool IsFlipping = false;
int StartFlipping = 0;
int intakeState = 0; //0 neutral，1吸入，-1吐出

void runDriver() {
    IsFlipping = false;

    pros::Controller controller(pros::E_CONTROLLER_MASTER);
    constexpr std::uint32_t controllerDisplayPeriodMs = 100;
    std::uint32_t lastControllerDisplayMs = pros::millis() - controllerDisplayPeriodMs;
    bool displayX = true;
    pros::lcd::set_text(1, "Mode: driver (arcade)");

    switch(StartPoint){
        case 'L':
            chassis.setPose(leftStartPoint.x, leftStartPoint.y, 90);
            break;
        case 'R':
            chassis.setPose(rightStartPoint.x, rightStartPoint.y, -90);
            break;
        case 'U':
            chassis.setPose(topStartPoint.x, topStartPoint.y, 180);
            break;
        case 'B':
            chassis.setPose(bottomStartPoint.x, bottomStartPoint.y, 0);
            break;
    }


    while (!pros::competition::is_disabled() &&
           !pros::competition::is_autonomous()) {

        const int turn = controller.get_analog(pros::E_CONTROLLER_ANALOG_RIGHT_X);
        const int forward = controller.get_analog(pros::E_CONTROLLER_ANALOG_LEFT_Y);
        chassis.arcade(forward, turn);

        // 后续的 intake / lift 等机构手柄控制放在这里，不要加入阻塞等待。
        // R1/R2
        if(controller.get_digital(pros::E_CONTROLLER_DIGITAL_R1)){ 
            LiftMotorLeft.move(LIFTUPSPEED);
            LiftMotorRight.move(LIFTUPSPEED);
        } else if(controller.get_digital(pros::E_CONTROLLER_DIGITAL_R2)){
            LiftMotorLeft.move(LIFTDOWNSPEED);
            LiftMotorRight.move(LIFTDOWNSPEED);
        } else {
            LiftMotorLeft.move(0);
            LiftMotorRight.move(0);
        }

        // L1/L2
        /*
        if(controller.get_digital(pros::E_CONTROLLER_DIGITAL_L1)){
            IntakeMotorLeft.move(INTAKEINSPEED);
            IntakeMotorRight.move(INTAKEINSPEED);
            ClampMotor.move(57);
        } else if(controller.get_digital(pros::E_CONTROLLER_DIGITAL_L2)){
            IntakeMotorLeft.move(INTAKEOUTSPEED);
            IntakeMotorRight.move(INTAKEOUTSPEED);
            ClampMotor.move(57); //Clamp负责张开爪子，所以take in和take out都需要127
        } else {
            IntakeMotorLeft.brake();
            IntakeMotorRight.brake();
            ClampMotor.move(0);
        }
        */

        
       if(controller.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_L1)){
            if(intakeState == 0){
                intakeState = 1;
            } else {
                intakeState = 0;
            }
       }
       if(controller.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_L2)){
            if(intakeState == 0){
                intakeState = -1;
            } else {
                intakeState = 0;
            }
       }
       if(intakeState == 0){
            IntakeMotorLeft.move(0);
            IntakeMotorRight.move(0);
            ClampMotor.move(0);
       } else if (intakeState == 1){
            IntakeMotorLeft.move(INTAKEINSPEED);
            IntakeMotorRight.move(INTAKEINSPEED);
            ClampMotor.move(0);
       } else {
            IntakeMotorLeft.move(INTAKEOUTSPEED);
            IntakeMotorRight.move(INTAKEOUTSPEED);
            ClampMotor.move(-127);
       }
        
        
        /*

        bool Clampstate = false;
        bool intakestate = false;
        if(controller.get_digital(pros::E_CONTROLLER_DIGITAL_L1)){
            intakestate = !intakestate;
        } else if(controller.get_digital(pros::E_CONTROLLER_DIGITAL_L2)){
            Clampstate = !Clampstate;
        }
        if(Clampstate == 1){
            ClampMotor.move(-127);
        } else {
            ClampMotor.move(0);
        }
        if(intakestate){
            IntakeMotorLeft.move(INTAKEINSPEED);
            IntakeMotorRight.move(INTAKEINSPEED);
        } else {
            IntakeMotorLeft.move(0);
            IntakeMotorRight.move(0);
        }
        */

        // X
        if(controller.get_digital(pros::E_CONTROLLER_DIGITAL_X) && !IsFlipping){
            IsFlipping = true;
            StartFlipping = pros::millis();
        }
        if(IsFlipping && pros::millis() - StartFlipping >= FLIPPINGTIME){
            IsFlipping = false;
        }
        if(IsFlipping){
            ToggleMotor.move(127);
        } else {
            ToggleMotor.move(0);
        }

        // 每 100 ms 更新一行，每个坐标每 200 ms 刷新一次；单位为英寸。
        // 轮流写入，避免连续发送手柄屏幕指令；固定宽度覆盖旧数字。
        const std::uint32_t now = pros::millis();
        if (now - lastControllerDisplayMs >= controllerDisplayPeriodMs) {
            const auto pose = chassis.getPose();
            if (displayX) {
                controller.print(0, 0, "X:%8.2f in  ", pose.x);
            } else {
                controller.print(1, 0, "Y:%8.2f in  ", pose.y);
            }
            displayX = !displayX;
            lastControllerDisplayMs = now;
        }
        pros::delay(driverPeriodMs);
    }
    stopRobot();
}
