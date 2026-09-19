#include "main.h"
#include "robot.hpp"
#include "routines.hpp"

void initialize() {
    robot::initialize();
}

void disabled() {
    robot::stop();
}

void competition_initialize() {
    // Add autonomous selection here when multiple routines exist.
}

void autonomous() {
    robot::stop();
    routines::autonomous();
    robot::stop();
}

void opcontrol() {
    robot::stop();
    routines::driver();
    robot::stop();
}
