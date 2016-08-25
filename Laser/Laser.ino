/*
Laser Cat Toy:
Simple script to automate a laser pointer attached to two servo motors.
Servo speed, servo angle, and laser on-time are randomly generated.

Laser operated on 3x1.5V batteries at 10mA, low enough for Arduino
  I/O to handle directly.
Servo angle limits chosen based on shelf placement of device.

For future improvements, timers should be added to allow 
  "simultaneous" movement of the servos.
*/

/*
Servo pinouts:
Small(vertical): White(Sig), Red(+), Black(-)
Large(Horizontal): Orange(Signal), Red(+), Brown(-)
*/

#include <VarSpeedServo.h>

//IO pin definitions
const int laser = 3;
VarSpeedServo servoH; //Horizontal movement (z-axis)
const int Hpin = 9;
VarSpeedServo servoV; //Vertical movement (x-y plane)
const int Vpin = 10;

//Variables
double lastLaserTime = 0;
double lastHTime = 0;
double lastVTime = 0;
double currentTime = 0;

int laserTime = 1000;
int servoHTime = 1000;
int servoVTime = 1000;
boolean laserOn = 0;


void setup() {
  //Initialize servos and pins
  servoH.attach(Hpin);
  servoV.attach(Vpin);
  pinMode(laser, OUTPUT);

  servoH.write(0);
  servoV.write(160);
  digitalWrite(laser, HIGH);

  delay(700);
  servoH.detach();
  servoV.detach();
  digitalWrite(laser, LOW);
}


void loop() {

  //Ensures values of each loop interation have same start time
  //versus calling millis() for each object
  currentTime = millis();

  //Set laser active time
  if (currentTime - lastLaserTime >= laserTime) {
    if (laserOn == 0) {
      /*Laser originally operated on 4.5V, so full 5V from I/O pin 
        isn't necessary. Value of 200 provides about 4V (200/255 * 5V),
        which is still plenty 
      */
      analogWrite(laser, 200);
      laserOn = 1;
    }
    else {
      analogWrite(laser, 0);
      laserOn = 0;
    }

    laserTime = random(1000, 15000); //For 1 to 15 second activation
    lastLaserTime = currentTime;
  }

  //Change horizontal servo
  if (currentTime - lastHTime >= servoHTime) {
    servoH.attach(Hpin);
     
    /* slowmove(value, speed), where 'value' is target
      angle and 'speed' is 0-255
    */
    servoH.slowmove(random(0, 140), random(0, 40));
    servoH.detach();
    
    servoHTime = random(1000, 10000);
    lastHTime = currentTime;
  }

  //Change vertical servo
  if (currentTime - lastVTime >= servoVTime) {
    servoH.attach(Hpin);

    servoV.slowmove(random(115, 150), random(0, 40));
    delay(time);
    servoH.detach();
    
    servoVTime = random(1000, 10000);
    lastVTime = currentTime;
  }

}

