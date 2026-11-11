#include "main.h"
#include "robot.hpp"
#include "routines.hpp"

const int FLIPTIME = 850; //ms
const int LIFTUPSPEED = 127;
const int LIFTDOWNSPEED = -127;

const int INTAKEINSPEED = 127;
const int INTAKEOUTSPEED = -90;

void stopMechanism(){
    IntakeMotorLeft.move(0);
    IntakeMotorRight.move(0);
    ToggleMotor.move(0);
    LiftMotorLeft.move(0);
    LiftMotorRight.move(0);
}
void move(int front,int turn, int sec){
    chassis.arcade(front, turn);
    pros::delay(sec);
    chassis.arcade(0, 0);
}
void toggleFlip(){
    ToggleMotor.move(127);
    //LiftMotorLeft.move(40);
    //LiftMotorRight.move(40);
    pros::delay(FLIPTIME);
    ToggleMotor.move(0);
}
void liftUp(int T){
    stopMechanism();
    int startLift = pros::millis();
    while(pros::millis() - startLift < T){
        LiftMotorLeft.move(LIFTUPSPEED);
        LiftMotorRight.move(LIFTUPSPEED);
    }
    stopMechanism();
}
void liftDown(int T){
    stopMechanism();
    int startLift = pros::millis();
    while(pros::millis() - startLift < T){
        LiftMotorLeft.move(LIFTDOWNSPEED);
        LiftMotorRight.move(LIFTDOWNSPEED);
    }
    stopMechanism();
}
void score(){
    
    stopMechanism();
    liftDown(200);

    IntakeMotorLeft.move(INTAKEINSPEED);
    IntakeMotorRight.move(INTAKEINSPEED);

    pros::delay(750);

    stopMechanism();
    liftUp(500);

    move(80,0,500);

    ClampMotor.move(-57);
    liftDown(500);

    pros::delay(750);

    move(-80,0,500);
    stopMechanism();
}
void getPin(){
    stopMechanism();
    int startintake = pros::millis();
    while(pros::millis() - startintake < 1000){
        IntakeMotorLeft.move(INTAKEINSPEED);
        IntakeMotorRight.move(INTAKEINSPEED);
        ClampMotor.move(-127);
    }
    stopMechanism();
}

void runAutonomous(){
    pros::lcd::set_text(1, "Autonomous: no routine selected");
    // 完成硬件配置和调参后，把自动动作写在这里。
    // 坐标用英寸，角度用度，超时用毫秒。
    // 最后的 false 表示等待动作结束或超时，再执行下一行。
    // chassis.setPose(0, 0, 0);
    // chassis.moveToPoint(0, 12, 2000, {}, false);
    // chassis.turnToHeading(90, 1500, {}, false);
    
    
    chassis.setPose(0,0,0); 

    chassis.turnToHeading(90, 3000, {.maxSpeed = 60}, false);

    //score();
    /*
    IntakeMotorLeft.move(INTAKEINSPEED);
    IntakeMotorRight.move(INTAKEINSPEED);
    pros::delay(100);
    stopMechanism();

    toggleFlip();
    pros::delay(100);

    move(127,0,180);
    pros::delay(100);

    chassis.turnToHeading(90,1500,{}, false);
    pros::delay(100);

    move(127,0,165);
    pros::delay(100);
    
    score();
    */

    /*
    move(-127,0,100);
    chassis.turnToHeading(-90,1500,{}, false);

    IntakeMotorLeft.move(INTAKEINSPEED);
    IntakeMotorRight.move(INTAKEINSPEED);
    ClampMotor.move(-127);
    move(60,0,200);
    stopMechanism();
    */

    //getPin();

}

/*
测试：
1. 90/45度转角
2. 抓pin

*/
