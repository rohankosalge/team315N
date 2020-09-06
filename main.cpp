/*----------------------------------------------------------------------------*/
/*                                                                            */
/*    Module:       main.cpp                                                  */
/*    Author:       rohankosalge                                              */
/*    Created:      Sun Jul 05 2020                                           */
/*    Description:  V5 project                                                */
/*                                                                            */
/*----------------------------------------------------------------------------*/

// ---- START VEXCODE CONFIGURED DEVICES ----
// Robot Configuration:
// [Name]               [Type]        [Port(s)]
// BottomRollers        motor         10              
// TopRollers           motor         2               
// RightIntake          motor         5               
// LeftIntake           motor         1               
// Controller1          controller                    
// LeftFront            motor         19              
// LeftBack             motor         20              
// RightFront           motor         4               
// RightBack            motor         3               
// VisionSensor         vision        12              
// ---- END VEXCODE CONFIGURED DEVICES ----

#include "vex.h"

using namespace vex;

velocityUnits vu;

static void blueballs();
static void redballs();

// SETTINGS
double TURN_SENSE = 0.75;
double DRIVE_SENSE = 1;

bool intakes_on = false;

int axis3val;
int axis4val;

int axis2val;

bool rollerslaunch = false;
bool rollerspoop = false;


void blueballs(){
  int bruh = 1;
  if (bruh == 1){
    if (VisionSensor.takeSnapshot(VisionSensor__SIG_BLUE)){
      Brain.Screen.clearScreen();
      Brain.Screen.print("shoot blue");

      //LeftIntake.setStopping(coast);
      //RightIntake.setStopping(coast);
      TopRollers.spin(forward, 100, pct);
      BottomRollers.spin(forward, 100, pct);
    }else if (VisionSensor.takeSnapshot(VisionSensor__SIG_RED)){
      Brain.Screen.clearScreen();
      Brain.Screen.print("poop red");
      TopRollers.spin(reverse,100,pct);
      BottomRollers.spin(forward, 100, pct);
    }else{
      Brain.Screen.print("nothing in my fov");
    }
  }

  Brain.Screen.newLine();
}

void redballs(){
  int bruh = 1;
  if (bruh == 1){
    if (VisionSensor.takeSnapshot(VisionSensor__SIG_RED)){
      Brain.Screen.newLine();
      Brain.Screen.print("shoot red");
       TopRollers.spin(forward, 100, pct);
      BottomRollers.spin(forward, 100, pct);


    }else if (VisionSensor.takeSnapshot(VisionSensor__SIG_BLUE)){
      Brain.Screen.newLine();
      Brain.Screen.print("poop blue");
      TopRollers.spin(reverse,100,pct);
      BottomRollers.spin(forward, 100, pct);
      
    }else{
      Brain.Screen.newLine();
      Brain.Screen.print("nothing in my fov");
      
    }
  } 
}


int main() {
  // Initializing Robot Configuration. DO NOT REMOVE!
  vexcodeInit();

  // if you = 0 = you are blue
  // if you = 1 = you are red 

  int you = 0;

  while(true){

    // This is the code for the intakes.
    // L2 will start the intakes moving at full speed.
    // R2 stops the intakes.
    // a boolean controls the intakes current state.

    while(Controller1.ButtonL2.pressing()){
      if(intakes_on == false){
        // we only start moving if the boolean isn't switched on yet.
        // if it is then the intakes are already moving. 
        LeftIntake.spin(forward, 100, pct);
        RightIntake.spin(forward, 100, pct);
        intakes_on = true; 
        // switch it on afterwards.
      }
    }

    while(Controller1.ButtonR2.pressing()){
      if(intakes_on == true){
        // we only stop if the boolean isn't switched off yet.
        // if it is then the intakes are still not moving.
        LeftIntake.stop();
        RightIntake.stop();
        intakes_on = false;
        // switch it off afterwards.
      }
    }

    // This is the code for the driving. 
    // We will have a one-joystick drive (Axes 3 and 4).
    // This is the same code from last year. 

    // these values track the position of the axes.
    // axes positions control the velocity.
    axis3val = Controller1.Axis3.position(percent);
    axis4val = Controller1.Axis4.position(percent);

    // implement the sensitivity constants to the percentages.
    axis3val*=DRIVE_SENSE;
    axis4val*=TURN_SENSE;

    if(axis3val == 0 && axis4val == 0){
      // if axes aren't moving, then nothing happens. Stop all drive motors.
      LeftFront.stop();
      RightFront.stop();
      LeftBack.stop();
      RightBack.stop();
    }else if(axis3val != 0 and axis4val == 0){
      // if the turning axis is not detected and driving axis is
      // then drive at axis 3 (driving) velocity.
      LeftFront.spin(forward, axis3val, pct);
      RightFront.spin(forward, axis3val, pct);
      LeftBack.spin(forward, axis3val, pct);
      RightBack.spin(forward, axis3val, pct);
    }else if(axis3val == 0 and axis4val != 0){
      // if the driving axis is not detected and turning axis is
      // then turn at axis 4 (turning) velocity.
      LeftFront.spin(forward, axis4val, pct);
      RightFront.spin(reverse, axis4val, pct);
      LeftBack.spin(forward, axis4val, pct);
      RightBack.spin(reverse, axis4val, pct);
    }

    // This is the code for the rollers. 
    // We used the left joystick for the driving. 
    // And we will use the other for controlling the rollers. 

    // this variable controls the velocity of the rollers.
    axis2val = Controller1.Axis2.position(percent);

    if(axis2val == 0){
      // if Axis 2 is not moving then nothing happens. Stop the roller motors.
      BottomRollers.stop();
      TopRollers.stop();
    }else if(axis2val > 0){
      // if Axis 2 is moving forward then we are in launching mode.
      // Bottom rollers & Top rollers both move forward at Axis 2 speed.
      BottomRollers.spin(forward, axis2val, pct);
      TopRollers.spin(forward, axis2val, pct);
    }else if(axis2val < 0){
      // if Axis 2 is moving backward then we are in pooping mode.
      // Bottom rollers moves forward and Top Rollers moves backward at Axis 2 speed.
      BottomRollers.spin(forward, -axis2val, pct);
      TopRollers.spin(reverse, -axis2val, pct);
    }
    
    if (you == 0){
      blueballs();
    }else{
      redballs();
    }
  }
}
