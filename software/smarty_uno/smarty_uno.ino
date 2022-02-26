#include <AFMotor.h>
// you'll need an Adafruit Motor shield V1 https://goo.gl/7MvZeo

AF_DCMotor R_motor(2); // defines Right motor connector
AF_DCMotor L_motor(1); // defines Left motor connector

// defines variables
long duration;

String inputString = "";     // a String to hold incoming data
bool stringComplete = false; // whether the string is complete

int sLength;
String tempStr;
int motorSpeed;

void setup()
{

  Serial.begin(115200); // sets up Serial library at 9600 bps

  //  Serial.println("start");

  // changes the following values to make the robot  drive as straight as possible

  L_motor.setSpeed(200); // sets L motor speed
  R_motor.setSpeed(140); // sets R motor speed

  R_motor.run(RELEASE); // turns L motor on
  L_motor.run(RELEASE); // turns R motor on
}

void loop()
{
  if (stringComplete)
  {
    // R_motor.run(RELEASE);
    // L_motor.run(RELEASE);

    if (inputString == "connected" || inputString == "disconnected")
    {
      //      Serial.print("stop");
      R_motor.run(RELEASE); // turns L motor on
      L_motor.run(RELEASE); // turns R motor on
    }else if(inputString == "L0"){
      L_motor.run(RELEASE);
    }else if(inputString == "R0"){
      R_motor.run(RELEASE);
    }
    else if (inputString[0] == 'L')
    {
      if (inputString[1] == '-')
      {
        sLength = inputString.length();
        tempStr = inputString.substring(2, sLength);
        motorSpeed = tempStr.toInt();
        L_motor.setSpeed(motorSpeed);
        //        Serial.print(motorSpeed);
        L_motor.run(BACKWARD);
      }
      else
      {
        sLength = inputString.length();
        tempStr = inputString.substring(1, sLength);
        motorSpeed = tempStr.toInt();
        L_motor.setSpeed(motorSpeed);
        //        Serial.print(motorSpeed);
        L_motor.run(FORWARD);
      }
    }
    else if (inputString[0] == 'R')
    {
      if (inputString[1] == '-')
      {
        sLength = inputString.length();
        tempStr = inputString.substring(2, sLength);
        motorSpeed = tempStr.toInt();
        R_motor.setSpeed(motorSpeed);
        //        Serial.print(motorSpeed);
        R_motor.run(BACKWARD);
      }
      else
      {
        sLength = inputString.length();
        tempStr = inputString.substring(1, sLength);
        motorSpeed = tempStr.toInt();
        R_motor.setSpeed(motorSpeed);
        //        Serial.print(motorSpeed);
        R_motor.run(FORWARD);
      }
    }
    stringComplete = false;
    inputString = "";
  }
}

/*
  SerialEvent occurs whenever a new data comes in the hardware serial RX. This
  routine is run between each time loop() runs, so using delay inside loop can
  delay response. Multiple bytes of data may be available.
*/
void serialEvent()
{
  while (Serial.available())
  {
    // get the new byte:
    char inChar = (char)Serial.read();

    // if the incoming character is a newline, set a flag so the main loop can
    // do something about it:
    if (inChar == ':')
    {
      stringComplete = true;
    }
    else if (inChar != '\n')
    {
      // add it to the inputString:
      inputString += inChar;
    }
  }
}
