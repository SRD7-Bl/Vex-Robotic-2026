#include "main.h"
#include "robot.hpp"
#include "routines.hpp"

void initialize() {
    initializeRobot();
}

void disabled() {
    stopRobot();
}

void competition_initialize() {
    // Add autonomous selection here when multiple routines exist.
}

void autonomous() {
    stopRobot();
    runAutonomous();
    stopRobot();
}

void opcontrol() {
    stopRobot();
    runDriver();
    stopRobot();
}
