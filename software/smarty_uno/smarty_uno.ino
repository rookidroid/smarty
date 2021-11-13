#include <AFMotor.h>
//you'll need an Adafruit Motor shield V1 https://goo.gl/7MvZeo

AF_DCMotor R_motor(2); // defines Right motor connector
AF_DCMotor L_motor(1); // defines Left motor connector

// defines variables
long duration;

void setup()
{
  Serial.begin(9600); // sets up Serial library at 9600 bps

  //changes the following values to make the robot  drive as straight as possible

  L_motor.setSpeed(200); // sets L motor speed
  R_motor.setSpeed(140); // sets R motor speed

  R_motor.run(RELEASE); //turns L motor on
  L_motor.run(RELEASE); //turns R motor on
}

void loop()
{
  // R_motor.run(RELEASE);
  // L_motor.run(RELEASE);

  //go backward
  R_motor.run(FORWARD);
  L_motor.run(FORWARD);

  delay(1000);
  R_motor.run(BACKWARD);
  L_motor.run(BACKWARD);
  delay(1000);
}
