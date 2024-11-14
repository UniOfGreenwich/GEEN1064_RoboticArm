/***********************************************************************************************************************
  Author: Seb Blair(CompEng0001)
  Date: 14/11/2024
  Version: 2.1.5
  License: GNU Lesser General Public License
  Documentation: https://github.com/UniOfGreenwich/GEEN1064_RoboticArm
  Acknowledgements: TinkerKit Braccio base libraries -> https://github.com/arduino-org/arduino-library-braccio
  Notes: DO NOT MODIFY THIS FILE WITHOUT INSTRUCTION
***********************************************************************************************************************/

#include "RoboticArm.h"

// Initialize Servo objects
Servo base;
Servo shoulder;
Servo elbow;
Servo wrist_rot;
Servo wrist_ver;
Servo gripper;

// Initialize global variables
int step_base, step_shoulder, step_elbow, step_wrist_ver, step_wrist_rot, step_gripper;
int inputNum;
char c;
String Command;

void initialiseRoboticArm() {
  // Open serial for communication
  Serial.begin(9600);
  pinMode(LED_BUILTIN, OUTPUT);
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
  Serial.println("The robotoic arm will only move when an excepted command is inputted:");
  Serial.println("");
  Serial.println("Allowed values for each servo are as follows:");
  Serial.println("");
  Serial.println(" * Base           = B and ALLOWED values are 0 to 180");
  Serial.println(" * Shoulder       = S and ALLOWED values are 15 to 165");
  Serial.println(" * Elbow          = E and ALLOWED values are 0 to 180");
  Serial.println(" * Wrist vertical = V and ALLOWED values are 0 to 180");
  Serial.println(" * Wrist Rotation = R and ALLOWED values are 0 to 180");
  Serial.println(" * Gripper        = G and ALLOWED values are 10 to 73");
  Serial.println(" * Home Position  = HOM");
  Serial.println("");
  Serial.println(" * Eg Base to 60 degrees = B60");
  Serial.println("");
  Serial.println("The robotic arm is ready to receive commands when the led is ON.");
  Serial.println("When the light is OFF when your command is being processed");
  Serial.println("");
  Serial.println("If in doubt refer to documentation or ask for help!");
  Serial.println("");
  Serial.println("##################### END OF OPERATING INFORMATION ####################");
  delay(1000);

  Serial.println("");
  Serial.println("In Home position: B90 S90 E180 V180 R90 G10");
}

void serialListener() {
  // wait for light to show so that you know it is ready to receive commands
  digitalWrite(LED_BUILTIN, HIGH);
  while (Serial.available()) {
    delay(100);
    c = Serial.read();  // reading the string sent by google voice
    if (c == '#') {
      break;
    }
    Command += c;
  }
  if (Command.length() > 2) {
    // Turn light off to show that a command is being processed
    digitalWrite(LED_BUILTIN, LOW);
    Command.trim();  //Get rid of any whitespace
    //Serial.println(Command);
    processCommand();  // Action on the String Command to find out which motors to move
  }
}

/**
 * processCommand actions the String Command by findings the indentifier and then the angle ie B60
 */
void processCommand() {
  // Move Elbow
  if (Command.startsWith("E")) {
    int newAngle = whichMotor(Command, "E");
    moveElbow(20, newAngle);
    delay(1000);
    Command = "";
  }

  // Move Shoulder
  else if (Command.startsWith("S")) {
    int newAngle = whichMotor(Command, "S");
    moveShoulder(20, newAngle);
    delay(1000);
    Command = "";
  }

  // Move Base
  else if (Command.startsWith("B")) {
    int newAngle = whichMotor(Command, "B");
    moveBase(20, newAngle);
    delay(1000);
    Command = "";
  }

  // Move WritsVer
  else if (Command.startsWith("V")) {
    int newAngle = whichMotor(Command, "V");
    moveWrist_Ver(20, newAngle);
    delay(1000);
    Command = "";
  }

  // Move wristRot
  else if (Command.startsWith("R")) {
    int newAngle = whichMotor(Command, "R");
    moveWrist_Rot(20, newAngle);
    delay(1000);
    Command = "";
  }

  // Move Gripper
  else if (Command.startsWith("G")) {
    int newAngle = whichMotor(Command, "G");
    moveGripper(20, newAngle);
    delay(1000);
    Command = "";
  } else if (Command.startsWith("H")) {
    homePosition();
    delay(1000);
    Command = "";
  }
  // print Positions
  else if (Command.startsWith("P")) {
    printPosition();
    delay(1000);
    Command = "";
  }
  // Inform user to enter a legal command
  else {
    Serial.println("Please enter a motor letter joined with an angle eg B60: ");
    Command = "";
  }
}

/**
 * Indentifies the right motor to move by searching the String Command and checking against the input motor
 * @param l_Command is the Command received from the serial monitor
 * @param l_motor is the motor indentifier that must match parent condition.
 * @return param angle is the new angle the identified motor will position to.
 */
int whichMotor(String l_Command, String l_Motor) {
  int lastPos = l_Command.lastIndexOf(l_Motor);
  int lengthCom = l_Command.length();
  String angleString = l_Command.substring(lastPos + 1, lengthCom);
  int angle = angleString.toInt();

  //For Degbugging
  //Serial.print("Angle in string = ");
  //Serial.println(angleString);
  //Serial.print("Angle as int = ");
  //Serial.println(angle);
  return angle;
}

/**
 * @brief Moves a servo to a specified target position with controlled delay and position constraints.
 *
 * This function incrementally moves a servo motor from its current position to a specified target position.
 * It limits the movement within defined minimum and maximum boundaries and allows for controlled delay 
 * between movements for smooth transitions. The function also prints the final position for debugging.
 *
 * @param servo The servo motor object to be controlled.
 * @param currentPosition A reference to the current position variable of the servo.
 * @param targetPosition The desired target angle for the servo.
 * @param stepDelay Delay in milliseconds between each step, used to control movement speed.
 * @param minPosition The minimum allowed position for the servo, typically between 0 and 180.
 * @param maxPosition The maximum allowed position for the servo, typically between 0 and 180.
 * @param servoName A character string used for debugging output to identify which servo is being moved.
 *
 * @note The function constrains the target position within the specified min and max positions.
 *       It will increment or decrement the servo's position by one degree until the target is reached.
 */
void moveServo(Servo &servo, int &currentPosition, int targetPosition, int stepDelay, int minPosition, int maxPosition, const char *servoName) {
  if (stepDelay > 30) stepDelay = 30;
  if (stepDelay < 10) stepDelay = 10;
  targetPosition = constrain(targetPosition, minPosition, maxPosition);  // Ensure target position is within bounds

  // Continue moving until the servo reaches the target position
  while (currentPosition != targetPosition) {
    servo.write(currentPosition);

    // Adjust position by one step
    if (targetPosition > currentPosition) {
      currentPosition++;
    } else if (targetPosition < currentPosition) {
      currentPosition--;
    }

    delay(stepDelay);  // Delay between movements for smooth motion
  }

  // Debugging
  Serial.print(servoName);
  Serial.print(" is at: ");
  Serial.println(currentPosition);
}

/**
 * @brief Moves the servo to a specified target position with controlled delay.
 *
 * This function serves as a wrapper around `moveServo` to control the servo. It sets 
 * specific movement boundaries and delay parameters for the motor, allowing for smooth 
 * movement within the specified range.
 *
 * @param stepDelay Delay in milliseconds between each step, controlling the speed of movement.
 * @param vBase The target position for the servo, constrained between 0 and 180 degrees.
 *
 * This function calls `moveServo`, passing the specific parameters for the servo:
 * - The `base` servo object
 * - The `step_base` variable that tracks the servo's current position
 * - The specified minimum (0) and maximum (180) bounds for the servo
 * - The name "Base" for debug output
 *
 * @note The target position (`vBase`) is automatically constrained within 0 to 180 degrees.
 */
void moveBase(int stepDelay, int vBase) {
  moveServo(base, step_base, vBase, stepDelay, 0, 180, "Base");
}

/**
 * @brief Moves the servo to a specified target position with controlled delay.
 *
 * This function serves as a wrapper around `moveServo` to control the servo. It sets 
 * specific movement boundaries and delay parameters for the motor, allowing for smooth 
 * movement within the specified range.
 *
 * @param stepDelay Delay in milliseconds between each step, controlling the speed of movement.
 * @param vShoulder The target position for the servo, constrained between 15 and 165 degrees.
 *
 * This function calls `moveServo`, passing the specific parameters for the servo:
 * - The `shoulder` servo object
 * - The `step_shoulder` variable that tracks the servo's current position
 * - The specified minimum (15) and maximum (165) bounds for the servo
 * - The name "Shoulder" for debug output
 *
 * @note The target position (`vShoulder`) is automatically constrained within 15 to 165 degrees.
 */
void moveShoulder(int stepDelay, int vShoulder) {
  moveServo(shoulder, step_shoulder, vShoulder, stepDelay, 15, 165, "Shoulder");
}

/**
 * @brief Moves the servo to a specified target position with controlled delay.
 *
 * This function serves as a wrapper around `moveServo` to control the servo. It sets 
 * specific movement boundaries and delay parameters for the motor, allowing for smooth 
 * movement within the specified range.
 *
 * @param stepDelay Delay in milliseconds between each step, controlling the speed of movement.
 * @param vElbow The target position for the servo, constrained between 0 and 180 degrees.
 *
 * This function calls `moveServo`, passing the specific parameters for the servo:
 * - The `elbow` servo object
 * - The `step_elbow` variable that tracks the servo's current position
 * - The specified minimum (0) and maximum (180) bounds for the servo
 * - The name "Elbow" for debug output
 *
 * @note The target position (`vElbow`) is automatically constrained within 0 to 180 degrees.
 */
void moveElbow(int stepDelay, int vElbow) {
  moveServo(elbow, step_elbow, vElbow, stepDelay, 0, 180, "Elbow");
}

/**
 * @brief Moves the servo to a specified target position with controlled delay.
 *
 * This function serves as a wrapper around `moveServo` to control the servo. It sets 
 * specific movement boundaries and delay parameters for the motor, allowing for smooth 
 * movement within the specified range.
 *
 * @param stepDelay Delay in milliseconds between each step, controlling the speed of movement.
 * @param vWrist_Ver The target position for the servo, constrained between 0 and 180 degrees.
 *
 * This function calls `moveServo`, passing the specific parameters for the servo:
 * - The `vWrist_Ver` servo object
 * - The `step_wrist_ver` variable that tracks the servo's current position
 * - The specified minimum (0) and maximum (180) bounds for the servo
 * - The name "Wrist Vertical" for debug output
 *
 * @note The target position (`vWrist_Ver`) is automatically constrained within 0 to 180 degrees.
 */
void moveWrist_Ver(int stepDelay, int vWrist_Ver) {
  moveServo(wrist_ver, step_wrist_ver, vWrist_Ver, stepDelay, 0, 180, "Wrist Vertical");
}

/**
 * @brief Moves the servo to a specified target position with controlled delay.
 *
 * This function serves as a wrapper around `moveServo` to control the servo. It sets 
 * specific movement boundaries and delay parameters for the motor, allowing for smooth 
 * movement within the specified range.
 *
 * @param stepDelay Delay in milliseconds between each step, controlling the speed of movement.
 * @param vWrist_rot The target position for the servo, constrained between 0 and 180 degrees.
 *
 * This function calls `moveServo`, passing the specific parameters for the servo:
 * - The `vWrist_rot` servo object
 * - The `step_wrist_rot` variable that tracks the servo's current position
 * - The specified minimum (0) and maximum (180) bounds for the servo
 * - The name "vWrist_rot" for debug output
 *
 * @note The target position (`Wrist Rotation`) is automatically constrained within 0 to 180 degrees.
 */
void moveWrist_Rot(int stepDelay, int vWrist_rot) {
  moveServo(wrist_rot, step_wrist_rot, vWrist_rot, stepDelay, 0, 180, "Wrist Rotation");
}

/**
 * @brief Moves the servo to a specified target position with controlled delay.
 *
 * This function serves as a wrapper around `moveServo` to control the servo. It sets 
 * specific movement boundaries and delay parameters for the motor, allowing for smooth 
 * movement within the specified range.
 *
 * @param stepDelay Delay in milliseconds between each step, controlling the speed of movement.
 * @param vgripper The target position for the servo, constrained between 10 and 73 degrees.
 *
 * This function calls `moveServo`, passing the specific parameters for the servo:
 * - The `vgripper` servo object
 * - The `step_gripper` variable that tracks the servo's current position
 * - The specified minimum (10) and maximum (73) bounds for the servo
 * - The name "Gripper" for debug output
 *
 * @note The target position (`vgripper`) is automatically constrained within 10 to 73 degrees.
 */
void moveGripper(int stepDelay, int vgripper) {
  moveServo(gripper, step_gripper, vgripper, stepDelay, 10, 73, "Gripper");
}

/**
    This function sets arm to the home position.
*/
void homePosition() {
  //For each step motor this set up the initial degree
  base.write(90);
  delay(1500);
  shoulder.write(90);
  delay(1000);
  elbow.write(180);
  delay(1000);
  wrist_ver.write(180);
  delay(1000);
  wrist_rot.write(90);
  delay(1000);
  gripper.write(10);
  delay(1000);
  //Previous step motor position
  step_base = 90;
  step_shoulder = 90;
  step_elbow = 180;
  step_wrist_ver = 180;
  step_wrist_rot = 90;
  step_gripper = 10;

  Serial.println("In Home position: B90 S90 E180 V180 R90 G10");
}

/**
    This function gets the lastest servo position provided by the user.
*/
void printPosition() {

  Serial.print("B: ");
  Serial.print(step_base);
  Serial.print(" S: ");
  Serial.print(step_shoulder);
  Serial.print(" E: ");
  Serial.print(step_elbow);
  Serial.print(" V: ");
  Serial.print(step_wrist_ver);
  Serial.print(" R: ");
  Serial.print(step_wrist_rot);
  Serial.print(" G: ");
  Serial.println(step_gripper);
}

/**
  Braccio initialization and set intial position
  Modifing this function you can set up the initial position of all the
  servo motors of Braccio
*/
void roboticArmBegin() {
  pinMode(SOFT_START_CONTROL_PIN, OUTPUT);
  digitalWrite(SOFT_START_CONTROL_PIN, LOW);

  // initialization pin Servo motors
  base.attach(11);
  shoulder.attach(10);
  elbow.attach(9);
  wrist_ver.attach(6);
  wrist_rot.attach(5);
  gripper.attach(3);
  softStart(-35);  // delayMicroseconds
  delay(1500);
  //For each step motor this set up the initial degree
  base.write(90);
  delay(1500);
  shoulder.write(90);
  delay(1000);
  elbow.write(180);
  delay(1000);
  wrist_ver.write(180);
  delay(1000);
  wrist_rot.write(90);
  delay(1000);
  gripper.write(10);
  delay(1000);
  //set step motor position
  step_base = 90;
  step_shoulder = 90;
  step_elbow = 180;
  step_wrist_ver = 180;
  step_wrist_rot = 90;
  step_gripper = 10;
}


/**
  This functions allows you to control all the servo motors
  
  Robotic Arm moves in this order base -> shoulder -> elbow -> wrist vertical -> wrist rotation -> gripper  

  @param stepDelay The delay between each servo movement
  @param vBase next base servo motor degree
  @param vShoulder next shoulder servo motor degree
  @param vElbow next elbow servo motor degree
  @param vWrist_ver next wrist rotation servo motor degree
  @param vWrist_rot next wrist vertical servo motor degree
  @param vgripper next gripper servo motor degree
*/
void roboticArmMovement(int stepDelay, int vBase, int vShoulder, int vElbow, int vWrist_ver, int vWrist_rot, int vgripper) {
  digitalWrite(LED_BUILTIN, LOW);
  // DO NOT CHANGE VALUES, this avoids dangerous positions for the Braccio
  if (stepDelay > 30) stepDelay = 30;
  if (stepDelay < 10) stepDelay = 10;
  if (vBase < 0) vBase = 0;
  if (vBase > 180) vBase = 180;
  if (vShoulder < 15) vShoulder = 15;
  if (vShoulder > 165) vShoulder = 165;
  if (vElbow < 0) vElbow = 0;
  if (vElbow > 180) vElbow = 180;
  if (vWrist_ver < 0) vWrist_ver = 0;
  if (vWrist_ver > 180) vWrist_ver = 180;
  if (vWrist_rot > 180) vWrist_rot = 180;
  if (vWrist_rot < 0) vWrist_rot = 0;
  if (vgripper < 10) vgripper = 10;
  if (vgripper > 73) vgripper = 73;

  int exit = 1;

  // Until the all motors are in the desired position
  while (exit) {
    // For each servo motor if next degree is not the same of the previuos than do the movement
    if (vBase != step_base) {
      base.write(step_base);
      // One step ahead
      if (vBase > step_base) {
        step_base++;
      }
      // One step beyond
      if (vBase < step_base) {
        step_base--;
      }
    }

    if (vShoulder != step_shoulder) {
      shoulder.write(step_shoulder);
      // One step ahead
      if (vShoulder > step_shoulder) {
        step_shoulder++;
      }
      // One step beyond
      if (vShoulder < step_shoulder) {
        step_shoulder--;
      }
    }

    if (vElbow != step_elbow) {
      elbow.write(step_elbow);
      // One step ahead
      if (vElbow > step_elbow) {
        step_elbow++;
      }
      // One step beyond
      if (vElbow < step_elbow) {
        step_elbow--;
      }
    }

    if (vWrist_ver != step_wrist_ver) {
      wrist_ver.write(step_wrist_ver);
      // One step ahead
      if (vWrist_ver > step_wrist_ver) {
        step_wrist_ver++;
      }
      // One step beyond
      if (vWrist_ver < step_wrist_ver) {
        step_wrist_ver--;
      }
    }

    if (vWrist_rot != step_wrist_rot) {
      wrist_rot.write(step_wrist_rot);
      // One step ahead
      if (vWrist_rot > step_wrist_rot) {
        step_wrist_rot++;
      }
      // One step beyond
      if (vWrist_rot < step_wrist_rot) {
        step_wrist_rot--;
      }
    }

    if (vgripper != step_gripper) {
      gripper.write(step_gripper);
      if (vgripper > step_gripper) {
        step_gripper++;
      }
      // One step beyond
      if (vgripper < step_gripper) {
        step_gripper--;
      }
    }

    //delay between each movement
    delay(stepDelay);

    //It checks if all the servo motors are in the desired position
    if ((vBase == step_base) && (vShoulder == step_shoulder)
        && (vElbow == step_elbow) && (vWrist_ver == step_wrist_ver)
        && (vWrist_rot == step_wrist_rot) && (vgripper == step_gripper)) {
      step_base = vBase;
      step_shoulder = vShoulder;
      step_elbow = vElbow;
      step_wrist_rot = vWrist_rot;
      step_wrist_ver = vWrist_ver;
      step_gripper = vgripper;

      //Debugging
      Serial.print("Base is: ");
      Serial.println(step_base);
      Serial.print("Shoulder is: ");
      Serial.println(step_shoulder);
      Serial.print("Elbow is: ");
      Serial.println(step_elbow);
      Serial.print("Wrist Verticle is: ");
      Serial.println(step_wrist_ver);
      Serial.print("Wrist Rotation is: ");
      Serial.println(step_wrist_rot);
      Serial.print("Gripper is: ");
      Serial.println(step_gripper);

      exit = 0;
    } else {
      exit = 1;
    }
  }
  digitalWrite(LED_BUILTIN, HIGH);
}


/**
    This function, used only with the Braccio Shield V4 and greater,
    turn ON the Braccio softly and save Braccio from brokes.
    The SOFT_START_CONTROL_PIN is used as a software PWM
    @param soft_start_level: the minimum value is -70, , default value is 0 (SOFT_START_DEFAULT)
*/
void softStart(int soft_start_level) {
  long int tmp = millis();
  while (millis() - tmp < LOW_LIMIT_TIMEOUT) {
    softwarePWM(80 + soft_start_level, 450 - soft_start_level);  //the sum should be 530usec
  }
  Serial.println();
  Serial.println("...Patience is a Virtue...");
  Serial.println();

  while (millis() - tmp < HIGH_LIMIT_TIMEOUT) {
    softwarePWM(75 + soft_start_level, 430 - soft_start_level);  //the sum should be 505usec
  }

  digitalWrite(SOFT_START_CONTROL_PIN, HIGH);
}

/**
  Software implementation of the PWM for the SOFT_START_CONTROL_PIN,HIGH
  @param high_time: the time in the logic level high
  @param low_time: the time in the logic level low
*/
void softwarePWM(int high_time, int low_time) {
  digitalWrite(SOFT_START_CONTROL_PIN, HIGH);
  delayMicroseconds(high_time);
  digitalWrite(SOFT_START_CONTROL_PIN, LOW);
  delayMicroseconds(low_time);
}
