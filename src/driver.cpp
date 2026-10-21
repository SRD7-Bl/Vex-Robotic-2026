#include "main.h"
#include "robot.hpp"
#include "robot_config.hpp"
#include "routines.hpp"

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
const int INTAKEOUTSPEED = -127;

const int FLIPPINGTIME = 500; // Unit:ms

bool IsFlipping = false;
int StartFlipping = 0;

void runDriver() {
    pros::Controller controller(pros::E_CONTROLLER_MASTER);
    pros::lcd::set_text(1, "Mode: driver (arcade)");
    while (!pros::competition::is_disabled() &&
           !pros::competition::is_autonomous()) {
        
        //摇杆
        const int forward = controller.get_analog(pros::E_CONTROLLER_ANALOG_RIGHT_X);
        const int turn = controller.get_analog(pros::E_CONTROLLER_ANALOG_LEFT_Y);
        chassis.arcade(-forward, -turn);

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
        if(controller.get_digital(pros::E_CONTROLLER_DIGITAL_L1)){
            IntakeMotorLeft.move(INTAKEINSPEED);
            IntakeMotorRight.move(INTAKEINSPEED);
            ClampMotor.move(127);
        } else if(controller.get_digital(pros::E_CONTROLLER_DIGITAL_L2)){
            IntakeMotorLeft.move(INTAKEOUTSPEED);
            IntakeMotorRight.move(INTAKEOUTSPEED);
            ClampMotor.move(127); //Clamp负责张开爪子，所以take in和take out都需要127
        } else {
            IntakeMotorLeft.move(0);
            IntakeMotorRight.move(0);
            ClampMotor.move(0);
        }

        // X
        if(controller.get_digital(pros::E_CONTROLLER_DIGITAL_X) && !IsFlipping){
            IsFlipping = true;
            StartFlipping = pros::millis();
        }
        if(IsFlipping && pros::millis() - StartFlipping >= FLIPPINGTIME){
            IsFlipping = false;
            ToggleMotor.move(0);
        } else {
            ToggleMotor.move(127);
        }

        
        pros::delay(driverPeriodMs);
    }
    stopRobot();
}
