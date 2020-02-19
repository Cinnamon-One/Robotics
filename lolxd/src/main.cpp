// ---- START VEXCODE CONFIGURED DEVICES ----
// Robot Configuration:
// [Name]               [Type]        [Port(s)]
// frontRight          motor         10              
// frontLeft           motor         20              
// backLeft            motor         19              
// backRight           motor         11              
// Controller1          controller                    
// intakeLeft          motor         2               
// intakeRight         motor         1               
// arms                 motor         5               
// Vision               vision        13              
// Potentiometer        pot           A               
// Ultrasonic           sonar         C, D            
// ramp                 motor         14              
// ---- END VEXCODE CONFIGURED DEVICES ----
/*----------------------------------------------------------------------------*/
/*                                                                            */
/*    Module:       main.cpp                                                  */
/*    Author:       C:\Users\efrag                                            */
/*    Created:      Thu Oct 03 2019                                           */
/*    Description:  V5 project                                                */
/*                                                                            */
/*----------------------------------------------------------------------------*/

/*
Robot Length: ~20''
Tile Length:  ~24''
*/

#include "vex.h"
#include "robot-config.h"
#include "frame-class.h"

using namespace vex;

competition Competition;

// Speed variables
int speedBase = 75;
int speed = speedBase;

// Distance variables
float rotations = 360;
double firstTile = 2.08 * rotations; //THIS MEASUREMENT IS NOT SET
double oneTile = 2.32 * rotations; //THIS MEASUREMENT IS NOT SET

// Screen functions

void print(double n) {
  Controller1.Screen.clearScreen();
  Controller1.Screen.setCursor(1, 1);
  Controller1.Screen.print(n);
}
void print(double n, double cursorX, double cursorY) {
  Controller1.Screen.setCursor(cursorY, cursorX);
  Controller1.Screen.print(n);
}
void print(char* n, double cursorX, double cursorY) {
  Controller1.Screen.setCursor(cursorY, cursorX);
  Controller1.Screen.print(n);
}
void printBrain(double n) {
  Brain.Screen.clearScreen();
  Brain.Screen.setCursor(1, 1);
  Brain.Screen.print(n);
}
void printBrain(double n, double cursorX, double cursorY) {
  Brain.Screen.setCursor(cursorY, cursorX);
  Brain.Screen.print(n);
}
void printBrain(char* n, double cursorX, double cursorY) {
  Brain.Screen.setCursor(cursorY, cursorX);
  Brain.Screen.print(n);
}

void clearScreen() {
  Controller1.Screen.clearLine();
  Controller1.Screen.setCursor(1, 1);
}

// Autonomous helper functions
void baseVelocity(double n) {
  frontLeft.setVelocity(n, percent);
  frontRight.setVelocity(n, percent);
  backRight.setVelocity(n, percent);
  backLeft.setVelocity(n, percent);
}
void moveTurns(double n) {
  frontRight.rotateFor(n, turns, false);
  backRight.rotateFor(n, turns, false);
  frontLeft.rotateFor(n, turns, false);
  backLeft.rotateFor(n, turns);
}
void moveTurns(double nl, double nr) {
  frontRight.rotateFor(nr, turns, false);
  backRight.rotateFor(nr, turns, false);
  frontLeft.rotateFor(nl, turns, false);
  backLeft.rotateFor(nl, turns);
}
void moveDegrees(double n) {
  frontRight.spinFor(n, degrees, false);
  backRight.spinFor(n, degrees, false);
  frontLeft.spinFor(n, degrees, false);
  backLeft.spinFor(n, degrees);
}
void moveDegrees(double nl, double nr) {
  frontRight.spinFor(nr, degrees, false);
  backRight.spinFor(nr, degrees, false);
  frontLeft.spinFor(nl, degrees, false);
  backLeft.spinFor(nl, degrees);
}

// Ramp movement
void rampUp() {
  ramp.spin(forward);
}
void rampDown() {
  ramp.spin(reverse);
}
void rampStop() {
  ramp.setVelocity(0, percent);
  ramp.spin(forward);
}

// Arm movement
void armsUp() {
  arms.setVelocity(100, percent);
  arms.spin(forward);
}
void armsDown() {
  arms.setVelocity(100, pct);
  arms.spin(reverse);
}

// Intake movement
void intake(double p) {
  intakeLeft.setVelocity(p * speed / 100, percent);
  intakeRight.setVelocity(p, percent);
  intakeLeft.spin(forward);
  intakeRight.spin(forward);
}
void outtake(double p) {
  intakeLeft.setVelocity(p * speed / 100, percent);
  intakeRight.setVelocity(p * speed / 100, percent);
  intakeLeft.spin(reverse);
  intakeRight.spin(reverse);
}
void stoptake() {
  intakeLeft.setVelocity(0, percent);
  intakeRight.setVelocity(0, percent);
  intakeLeft.spin(forward);
  intakeRight.spin(forward);
}

// Change body speed
void faster() {
  speed = std::min(speed + 25, 75);
  speed = ((speed == 35) ? 75 : speed);
  print(speed);
}
void slower() {
  speed = std::max(speed - 25, 10);
  speed = ((speed == 50) ? 10 : speed);
  print(speed);
}

void pre_auton() {
  // Motors Resetting 
  // No Code needed in this section  
}

void autonomous() {
  // Variables for movement
  float tile = 2.32; // Changes depending on wheel size

  // Prepare Robot for Auton (Lift ramp)
  basevelocity(speedBase);
  armsUp();
  task::sleep(1200);
  armsDown();
  task::sleep(500);
  intake(75);
  task::sleep(500);

  // Get four blocks and return
  moveTurns(2 * tile);
  task::sleep(100);
  stoptake();
  task::sleep(100);
}

// Used to know when a button starts being pressed
bool upLastFrame = false;
bool downLastFrame = false;
bool xLastFrame = false;

void usercontrol() {
  while(1) {
    float Axis1 = -Controller1.Axis1.value();
    float Axis2 = -Controller1.Axis2.value();
    float Axis3 = Controller1.Axis3.value();
    float Axis4 = -Controller1.Axis4.value();

    // baseVelocity(speed);

    if(!Controller1.ButtonA.pressing()) {
      frontLeft.spin(directionType::fwd, (Axis3 - Axis1) * speed / 100, velocityUnits::pct);
      backLeft.spin(directionType::fwd, (Axis3 - Axis1) * speed / 100, velocityUnits::pct);
      frontRight.spin(directionType::fwd, (Axis3 + Axis1) * speed / 100, velocityUnits::pct);
      backRight.spin(directionType::fwd, (Axis3 + Axis1) * speed / 100, velocityUnits::pct);
    } else {
      frontLeft.spin(directionType::fwd, (Axis3 - Axis4) * speed / 100, velocityUnits::pct);
      backLeft.spin(directionType::fwd, (Axis3 - Axis4) * speed / 100, velocityUnits::pct);
      frontRight.spin(directionType::fwd, (Axis3 + Axis4) * speed / 100, velocityUnits::pct);
      backRight.spin(directionType::fwd, (Axis3 + Axis4) * speed / 100, velocityUnits::pct);

      ramp.setVelocity(100, percent);
      ramp.spin(directionType::fwd, Axis2 * 0.30, velocityUnits::pct);
    }

    // Get the current button states
    bool upPressing = Controller1.ButtonUp.pressing();
    bool downPressing = Controller1.ButtonDown.pressing();
    bool xPressing = Controller1.ButtonX.pressing();

    // If up just got pressed, speed up. If down just got pressed, speed up.
    if(upPressing && !upLastFrame) {
      faster();
    } else if(downPressing && !downLastFrame) {
      slower();
    }

    // Save current button state for next frame
    upLastFrame = upPressing;
    downLastFrame = downPressing;
    xLastFrame = xPressing;

    // L1 raises the arms up, L2 lowers them
    if(Controller1.ButtonL1.pressing()) {
      armsUp();
    } else if(Controller1.ButtonL2.pressing()) {
      armsDown();
    } else {
      arms.stop();
    }

    // R1 intakes and R2 outtakes
    if(Controller1.ButtonR1.pressing()) {
      intake(speed);
    } else if(Controller1.ButtonR2.pressing()) {
      outtake(speed);
    } else {
      stoptake();
    }

    // Print debug info
    if(Brain.Screen.pressing()) {
      /*Brain.Screen.clearScreen();
      printBrain("Ramp:", 1, 1);
      printBrain("Arms:", 1, 2);
      printBrain(round(Potentiometer.angle(degrees)*1.005)-1, 6, 1);
      printBrain(round(PotentiometerH.angle(degrees)*1.005)-1, 6, 2);*/
      Brain.Screen.clearScreen();
      printBrain("a", 1, 1);
      for(int a = 2; a < 13; a++) {
        printBrain("|.........|.........|.........|.........|.....|||||", 1, a);
      }
    }

    task::sleep(20);
  }
}

int main() {

  vexcodeInit();

  Competition.autonomous(autonomous);
  Competition.drivercontrol(usercontrol);

  pre_auton();

  //Prevent main from exiting with an infinite loop.                        
  while(1) {
    task::sleep(100); //Sleep the task for a short amount of time to prevent wasted resources.
  }
}