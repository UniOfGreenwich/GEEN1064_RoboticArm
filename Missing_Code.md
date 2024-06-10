# Instructions

So you have identified that something is not quite right with the `HomePosition()` function, as the  robotic arm didn’t move. You should have seen in the serial monitor the following output:

```
Why did I not move?
```

In the Arduino IDE, navigate to **line 459** in the `Code_It_Yourself.ino`. You should see the `HomePosition()` function, delete code on **line 461**.

```cpp
...
// Home function here!
void HomePosition()
{
  Serial.println("Why did I not move?"); // line 461
}
...
```

Re-write the `HomePosition()` function to look exactly like below:


```cpp
...
void HomePosition()
{
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
...
```