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
    
    // waits for your commands via the serial monitor and then executes them
    //serialListener();
    moveShoulder(20,160);
    delay(2000);
    moveShoulder(20,10);
    delay(2000);

  for(int i= 0; i<160; i++){

    moveShoulder(20,i);
    delay(500);

  }
}