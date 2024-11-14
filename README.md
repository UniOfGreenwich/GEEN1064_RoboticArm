# GEEN1064 Robotic Arm Project
## Introduction

This repository is for the Year Zero Robotic Arm project, included is Arduino sketches, software and documentation.

**Sketches**
   
   * [Robot_Arm_Manual_Instructions_via_serial](./Robot_Arm_Manual_Instructions_via_serial), gives you manaul control over each servo individually and is designed to be used via the command line.

   * [Robot_Arm_Main](./Robot_Arm_Main), is to be used to automate all planned movements you calculated using the latter sketch.

**A Video on how to do download the project to get access the code in one place**

[![](http://img.youtube.com/vi/nCR-zGrlY7E/0.jpg)](https://youtu.be/nCR-zGrlY7E "How to download the project and open the first sketch")

**Specification** 
   * [Specification](./Specifications.md) - link for specification sheet, please read some useful information inside.

## Installation 

* All you need to do is clone/download (greenButton) and choose download as Zip. 
* Right click an extract at current location (Downloads) or navigate to the ardunio sketch folder on the machine and do it there. 
**If you need help ASK!**

## Construction

* [Digital Construction Manual](./Braccio_QSG.pdf)

* **Wrist and Gripper** needs a finer screw driver than the one provided in the pack.

**Click the image to be redirected to the construction video on youtube**

  [![](http://img.youtube.com/vi/5VkjJXm6bx8/0.jpg)](https://www.youtube.com/watch?time_continue=16&v=Lwb2ppat_bs "Robotic Arm Construction")

> Make sure you use the right numbered servo motors (numbering on the end of the cable near the connector) in the right joints. And be a bit careful because mounting the servos with the screws causes the plastic to "bloom" a bit and it will never be quite as tight if you have to remove them and put them back in again. Although the plastic pentagons are pre-attached to the servo hub, but I found that some were a few degrees off center, so you just have to adjust your code for that,  **AGAIN IF YOU NEED HELP ASK!**.

## Operational Information 

**Click the image to be redirected to a quick video of simple pick and place task, on youtube**

   [![](http://img.youtube.com/vi/SHML6NhQB3I/0.jpg)](https://youtu.be/SHML6NhQB3I "Robotic Arm performing a pick and place action")

### Manual Code

**To avoid damage to servos** serveral key functions are implemented in code, this takes a few seconds be patient after all it is a virtue.

The robotoic arm will only move when an excepted command is inputted:

Allowed values for each servo are as follows:

 * Base           = B and ALLOWED values are 0 to 180
 * Shoulder       = S and ALLOWED values are 15 to 165
 * Elbow          = E and ALLOWED values are 0 to 180
 * Wrist vertical = V and ALLOWED values are 0 to 180
 * Wrist Rotation = R and ALLOWED values are 0 to 180
 * Gripper        = G and ALLOWED values are 10 to 73
 * Home Position  = HOM which is B90 S90 E180 V180 R90 G10

 * Eg Base to 60 degrees = **B60**

The robotic arm is ready to receive commands when the led is ON.
When the light is OFF when your command is being processed.

### Automation code 

The automated controls are a sequence of pre-programmed movements, there are two ways to achieve this under the current version `[2.1.5]`

1. Indepenent motor control:
   - `void moveBase(int stepDelay, int vBase);`
   - `void moveShoulder(int stepDelay, int vShoulder);`
   - `void moveElbow(int stepDelay, int vElbow);`
   - `void moveWrist_Ver(int stepDelay, int vWrist_Ver);`
   - `void moveWrist_Rot(int stepDelay, int vWrist_rot);`
   - `void moveGripper(int stepDelay, int vgripper);`

      For example to move the base servo:

     - `moveBase(20,50); // moves the base to 50 degrees`

   **ORDER OF OPERATION** Robotic Arm moves in this order:

    * base -> shoulder -> elbow -> wrist vertical -> wrist rotation -> gripper 

   **Plan your movements accordingly**

## Acknowledgements 

* The sketch has been developed to use key functionality from [Braccio Robotic Arm](https://github.com/arduino-org/arduino-library-braccio) so students do not have to download the libraries. 
   * Further modification includes Serial I/O
**PLEASE NOTE** that this is only to be used with the Braccio Shield V4 becasue the softstart function protects the servo motors and the junctions from damages

**WARNING** 
There has been a request to change some code in the original source code linked above, [mix-up of wrist values](https://github.com/arduino-org/arduino-library-braccio/pull/4/commits/4ee82eac74b98196bec2413bd87deda3019e3a5c). So please only use the code that is provided in this repository as the change has been incorporated.

## Health and Safety

* DO NOT put any body parts or hair near moving parts and when the robotic arm is in operation 
* DO NOT exceed max load 200g
