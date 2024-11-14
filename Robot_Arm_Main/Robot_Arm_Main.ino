/***********************************************************************************************************************
  Author: Seb Blair
  Date: 14/11/2024
  Version: 2.1.5
  Usage: To control the robotic arm autonomously 
          Once you calculated all of the movements via the manual sketch 
          you can use this one to automate the process.
          Consider one movement to all six servos in one go, so: 
                            //(SD,  BA,    SH,   EL,   WV,   WR,  GR);
          RoboticArmMovement(20,  180,   30,   10,   60,   90,  73);
  License: GNU Lesser General Public License
  Documentation: https://github.com/UniOfGreenwich/GEEN1064_RoboticArm
  License: GNU Lesser General Public License
***********************************************************************************************************************/
#include "RoboticArm.h"

void setup() {
  // Open serial for communication
  Serial.begin(115200);

  // LED for visual indicator of readiness
  digitalWrite(LED_BUILTIN, OUTPUT);

  // Initialization of RoboticArm safely
  roboticArmBegin();

  Serial.println("Please stand back for your saftey....");
  Serial.println("Initialising all servos please wait...");
  Serial.print("..in 3 seconds.");
  delay(1000);
  Serial.print(".. 2 seconds.");
  delay(1000);
  Serial.print(".. 1 seconds");
  delay(1000);
  Serial.println("...STAND BACK");
  delay(1000);
  //initialization of RoboticArm safely
  roboticArmBegin();

  Serial.println("Initialisation complete!");
  Serial.println("");

  // Some instructions to screen
  Serial.println("######################## OPERATING INFORMATION #######################");
  Serial.println("");
  Serial.println("The robotic arm will follow the sequence of instrunctions listed in loop().");
  Serial.println("This will begin when the led turns on");
  Serial.println("REMEMBER: WHEN THE ROBOT IS IN MOTION DO NOT ENTER THE OPERATING ZONE");
  Serial.println("If in doubt refer to documentations or ask for help!");
  Serial.println("");
  Serial.println("##################### END OF OPERATING INFORMATION ####################");
  delay(1000);
}

/*****************************************************************************************************************
  END OF REGION
 *****************************************************************************************************************/

void loop() {
  digitalWrite(LED_BUILTIN, HIGH);  // Ready for first command.
  /*****************************************************************************************************************
    SD = a milliseconds delay between the movement of each servo.  Allowed values from 10 to 30 msec.
    BA = base degrees. Allowed values from 0 to 180 degrees
    SH = shoulder degrees. Allowed values from 15 to 165 degrees
    EL = elbow degrees. Allowed values from 0 to 180 degrees
    WV = wrist vertical degrees. Allowed values from 0 to 180 degrees
    WR = wrist rotation degrees. Allowed values from 0 to 180 degrees
    GR = gripper degrees. Allowed values from 10 to 73 degrees. 10: the toungue is open, 73: the gripper is closed.
  ******************************************************************************************************************/
  Serial.println("");
  Serial.println("First Movement");
  //(SD,  BA,   SH,   EL,   WV,  WR,  GR);
  roboticArmMovement(20, 180, 30, 10, 60, 90, 73);
  //Wait 1 second recommended for power to discharge from the servos
  delay(2000);

  Serial.println("");
  Serial.println("Second Movement");
  Serial.println("");
  //(SD,  BA,  SH,   EL,   WV,  WR,  GR);
  roboticArmMovement(20, 0, 120, 10, 100, 10, 10);
  // Wait 1 second recommended for power to discharge from the servos
  delay(2000);
}