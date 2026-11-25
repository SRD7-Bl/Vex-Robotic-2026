#include "main.h"
#include "robot.hpp"
#include "routines.hpp"
#include "field_coordinates.hpp"

const int FLIPTIME = 700; //ms
const int LIFTUPSPEED = 127;
const int LIFTDOWNSPEED = -127;

const int INTAKEINSPEED = 127;
const int INTAKEOUTSPEED = -20;

const float MARGIN = 7.5f;

char StartPoint = 'R'; // L --left / R --right / U --up / B --bottom

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
void score(int T){

    stopMechanism();
    liftUp(T);

    //move(60,0,330);
    liftDown(300);

    ClampMotor.move(-127);
    LiftMotorLeft.move(LIFTDOWNSPEED);
    LiftMotorRight.move(LIFTDOWNSPEED);
    pros::delay(500);

    //move(-80,0,120);

    stopMechanism();
}

void TimeBased(){
    chassis.setPose(0,0,0); 
    move(100,0,50);
    liftUp(30);

    pros::delay(100);

    //chassis.moveToPoint(20,0, 4000, {.maxSpeed = 60}, false);

    //score();
    
    IntakeMotorLeft.move(INTAKEINSPEED);
    IntakeMotorRight.move(INTAKEINSPEED);
    pros::delay(100);
    stopMechanism();

    toggleFlip();
    pros::delay(50);
    
    
    move(127,0,195);
    pros::delay(75);

    chassis.turnToHeading(90,1500,{}, false);
    pros::delay(75);

    move(120,0,180);
    pros::delay(75);
    
    score(75);
    pros::delay(75);

    
    /*
    move(-127,0,160);
    pros::delay(150);

    chassis.turnToHeading(45,1500,{}, false);
    pros::delay(75);

    ClampMotor.move(127);
    LiftMotorLeft.move(LIFTDOWNSPEED);
    LiftMotorRight.move(LIFTDOWNSPEED);
    IntakeMotorLeft.move(INTAKEINSPEED);
    IntakeMotorRight.move(INTAKEINSPEED);
    pros::delay(200);

    move(120,0,340);
    pros::delay(50);

    move(61,0,1000);
    pros::delay(50);

    //move(127,0,100);
    //pros::delay(50);

    chassis.turnToHeading(180,1500,{}, false);
    liftUp(200);
    pros::delay(50);

    move(100,0,200);
    pros::delay(50);
    
    score(125);
    pros::delay(50);

    move(-127,0,180);
    pros::delay(200);

    chassis.turnToHeading(135,1500,{}, false);
    pros::delay(50);
    
    ClampMotor.move(127);
    LiftMotorLeft.move(LIFTDOWNSPEED);
    LiftMotorRight.move(LIFTDOWNSPEED);
    IntakeMotorLeft.move(INTAKEINSPEED);
    IntakeMotorRight.move(INTAKEINSPEED);
    pros::delay(200);

    move(120,0,300);
    pros::delay(100);

    move(57,0,1000);
    pros::delay(50);
    liftUp(300);
    pros::delay(75);

    
    chassis.turnToHeading(-90,1500,{}, false);
    pros::delay(75);

    move(100,0,250);
    pros::delay(100);
    
    score(170);
    pros::delay(100);
    


    */

    /*
    chassis.turnToHeading(0,1500,{}, false);

    ClampMotor.move(127);
    LiftMotorLeft.move(LIFTDOWNSPEED);
    LiftMotorRight.move(LIFTDOWNSPEED);
    IntakeMotorLeft.move(INTAKEINSPEED);
    IntakeMotorRight.move(INTAKEINSPEED);
    pros::delay(250);

    move(107,0,200);
    pros::delay(200);

    move(57,0,530);
    pros::delay(150);

    LiftMotorLeft.move(LIFTUPSPEED);
    LiftMotorRight.move(LIFTUPSPEED);
    move(-107,0,250);
    stopMechanism();
    pros::delay(250);

    chassis.turnToHeading(-90,1500,{}, false);
    pros::delay(100);

    move(100,0,200);
    pros::delay(100);
    
    score();
    pros::delay(100);
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

void CoordinateBased(){
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

    move(100,0,50);
    liftUp(30);

    pros::delay(100);
    
    IntakeMotorLeft.move(INTAKEINSPEED);
    IntakeMotorRight.move(INTAKEINSPEED);
    pros::delay(100);
    stopMechanism();

    toggleFlip();
    pros::delay(50);

    chassis.moveToPoint(117.3,70.2,1000,{},false);
    pros::delay(100);

    chassis.turnToHeading(0, 2000, {}, false);
    pros::delay(100);

    liftUp(100);
    pros::delay(100);

    chassis.moveToPoint(blueGoalRight.x,blueGoalRight.y - MARGIN,1000,{},false);
    pros::delay(100);

    score(100);
}

void PIDTest(){
    chassis.setPose(0, 0, 0);
    //chassis.turnToHeading(90, 3000, {.maxSpeed = 127}, false);
    chassis.moveToPoint(0, 24, 4000, {.maxSpeed = 127}, false);
}
void runAutonomous(){
    pros::lcd::set_text(1, "Autonomous: no routine selected");
    // 完成硬件配置和调参后，把自动动作写在这里。
    // 坐标用英寸，角度用度，超时用毫秒。
    // 最后的 false 表示等待动作结束或超时，再执行下一行。
    // chassis.setPose(0, 0, 0);
    // chassis.moveToPoint(0, 12, 2000, {}, false);
    // chassis.turnToHeading(90, 1500, {}, false);

    CoordinateBased();
}
