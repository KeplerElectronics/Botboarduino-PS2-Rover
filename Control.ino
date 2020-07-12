//*********************************************************************************
// www.Lynxmotion.com
// Basic code for 2WD rover using continuous rotation servos, controlled via PS2
// Right now, the library does NOT support hot pluggable controllers, meaning 
// you must always either restart your Arduino after you connect the controller, 
// or call config_gamepad(pins) again after connecting the controller.
//*********************************************************************************

#include <PS2X_lib.h>  //for v1.6
#include <Servo.h> 
#include <Wire.h>
#include <Adafruit_MotorShield.h>
#include "utility/Adafruit_MS_PWMServoDriver.h"

//create Adafruit Motorshield Object
Adafruit_MotorShield AFMS = Adafruit_MotorShield();

//Create DC Motor object #1 is which port is used. 
Adafruit_DCMotor *LeftMotor = AFMS.getMotor(1);
Adafruit_DCMotor *RightMotor = AFMS.getMotor(2);
// create PS2 Controller Class
PS2X ps2x; 
int error = 0; 
byte type = 0;
byte vibrate = 0;

#define Deadzone 10 //PS2 analog joystick Deadzone

int speed1 =128, speed2 = 128;

byte M1L = 5;// PWM
byte M1R = 3;// PWM
byte M2L = 6;// PWM
byte M2R = 9;// PWM

void setup(){
  delay(50);
  Serial.begin(57600);
  AFMS.begin();
  LeftMotor->setSpeed(200);
  RightMotor->setSpeed(200);
  
 
 pinMode(7, INPUT);
 
 error = ps2x.config_gamepad(9,7,6,8, true, true);   

  //**************PAY ATTENTION*************
  pinMode(M1L, OUTPUT);                                // Establishes LEDPin as an output so the LED can be seen
  pinMode(M1R, OUTPUT);
  pinMode(M2L, OUTPUT);                                // Establishes LEDPin as an output so the LED can be seen
  pinMode(M2R, OUTPUT);
  error = ps2x.config_gamepad(9,7,6,8, false, true);   //setup pins and settings:  GamePad(clock, command, attention, data, Pressures?, Rumble?) check for error

  if(error == 0)
    Serial.println("Controller found! You may now send commands");

  else if(error == 1)
    Serial.println("No controller found, check wiring, see readme.txt to enable debug. visit www.billporter.info for troubleshooting tips");

  else if(error == 2)
    Serial.println("Controller found but not accepting commands. see readme.txt to enable debug. Visit www.billporter.info for troubleshooting tips");

  else if(error == 3)
    Serial.println("Controller refusing to enter Pressures mode, may not support it. ");

type = ps2x.readType(); 
  switch(type) 
  {
  case 0:
    Serial.println("Unknown Controller type");
    break;
  case 1:
    Serial.println("DualShock Controller Found");
    break;
  }
}

void loop()
{
  if(error == 1) //skip loop if no controller found
    return; 

  else { //DualShock Controller

    ps2x.read_gamepad(false, vibrate);          //read controller and set large motor to spin at 'vibrate' speed
//RIGHT
    if(ps2x.Analog(PSS_LY) >= 170 && ps2x.Analog(PSS_LY) <= 255)
    {
      speed2 = map(ps2x.Analog(PSS_RY),140 , 255, 0 , 255);
      RightMotor->run(FORWARD);
    }
    else if(ps2x.Analog(PSS_LY) >= 0 && ps2x.Analog(PSS_LY) <= 85)
    {
      speed2 = map(ps2x.Analog(PSS_RY),0 ,120 , 255 , 0);
      RightMotor->run(BACKWARD);
    }  
    else 
    {
      RightMotor->run(RELEASE);
    } 

 //LEFT
    if(ps2x.Analog(PSS_RY) >= 170 && ps2x.Analog(PSS_RY) <= 255)
    {
      speed2 = map(ps2x.Analog(PSS_RY),140 , 255, 0 , 255);
      LeftMotor->run(BACKWARD);
    }

    else if(ps2x.Analog(PSS_RY) >= 0 && ps2x.Analog(PSS_RY) <= 85)
    {
      speed2 = map(ps2x.Analog(PSS_RY),0 ,120 , 255 , 0);
      LeftMotor->run(FORWARD);
    }
    else
   {
      LeftMotor->run(RELEASE);
    }  
  }
}

