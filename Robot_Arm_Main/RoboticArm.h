/***********************************************************************************************************************
  Author: Seb Blair(CompEng0001)
  Date: 14/11/2024
  Version: 2.1.5
  License: GNU Lesser General Public License
  Documentation: https://github.com/UniOfGreenwich/GEEN1064_RoboticArm
  Acknowledgements: TinkerKit Braccio base libraries -> https://github.com/arduino-org/arduino-library-braccio
***********************************************************************************************************************/

#ifndef ROBOTIC_ARM_H
#define ROBOTIC_ARM_H

#include <Arduino.h>
// Required library for Servo control
#include <Servo.h>

// Constants
// The software PWM is connected to PIN 12. You cannot use the pin 12 if you are using a Braccio shield V4 or newer
#define SOFT_START_CONTROL_PIN 12
//Low and High Limit Timeout for the Software PWM
#define LOW_LIMIT_TIMEOUT 2000
#define HIGH_LIMIT_TIMEOUT 6000

// Servo objects
extern Servo base;
extern Servo shoulder;
extern Servo elbow;
extern Servo wrist_rot;
extern Servo wrist_ver;
extern Servo gripper;

// Global variables for servo positions and commands
extern int step_base, step_shoulder, step_elbow, step_wrist_ver, step_wrist_rot, step_gripper;
extern int inputNum;
extern char c;
extern String Command;

// Function prototypes
void initialiseRoboticArm();
void serialListener();
void processCommand();
int whichMotor(String l_Command, String l_Motor);
void moveServo(Servo &servo, int &currentPosition, int targetPosition, int stepDelay, int minPosition, int maxPosition, const char *servoName);
void moveBase(int stepDelay, int vBase);
void moveShoulder(int stepDelay, int vShoulder);
void moveElbow(int stepDelay, int vElbow);
void moveWrist_Ver(int stepDelay, int vWrist_Ver);
void moveWrist_Rot(int stepDelay, int vWrist_rot);
void moveGripper(int stepDelay, int vgripper);
void homePosition();
void printPosition();
void roboticArmBegin();
void roboticArmMovement(int stepDelay, int vBase, int vShoulder, int vElbow, int vWrist_ver, int vWrist_rot, int vgripper);
void softStart(int soft_start_level);
void softwarePWM(int high_time, int low_time);

#endif  // ROBOTIC_ARM_H