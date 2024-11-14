/***********************************************************************************************************************
  Author: Seb Blair(CompEng0001)
  Date: 14/11/2024
  Version: 2.1.5
  Useage: To control the robotic arm via each servo individually in one sketch
          Input a motor and desired angle into the serial montior and the robotic arm will move. 
          Follow on Serial montior instructions
***********************************************************************************************************************/

#include "RoboticArm.h"

/*Code here one once during startup*/
void setup() {
    initialiseRoboticArm();
}

/*Code here runs forever!*/
void loop() {
    // 
    serialListener();
}