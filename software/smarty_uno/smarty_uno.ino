#include <WiFi.h>
#include <WiFiUdp.h>

#include <ESP32Servo.h>

#ifndef APSSID
#define APSSID "tank"
#define APPSK "tank1234"
#endif

/* Set these to your desired credentials. */
const char *ssid = APSSID;
const char *password = APPSK;

// buffers for receiving and sending data
char packetBuffer[4096 + 1];             // buffer to hold incoming packet,
char ReplyBuffer[] = "acknowledged\r\n"; // a string to send back

unsigned int localPort = 1234; // local port to listen on

WiFiUDP Udp;

// GPIO pin number for the LEDs
#define PIN_L1 27
#define PIN_L2 26
#define PIN_R1 25
#define PIN_R2 33


// PWM channels
#define PWM_L1 0
#define PWM_L2 1
#define PWM_R1 2
#define PWM_R2 3


// PWM properties
const int pwmFreq = 5000;
const int pwmResolution = 8;



void setup()
{
  Serial.begin(921600);

  // configure LED PWM functionalitites
    pinMode(PIN_L1, OUTPUT);
    pinMode(PIN_L2, OUTPUT);
    pinMode(PIN_R1, OUTPUT);
    pinMode(PIN_R2, OUTPUT);

    ledcSetup(PWM_L1, pwmFreq, pwmResolution);
    ledcAttachPin(PIN_L1, PWM_L1);

    ledcSetup(PWM_L2, pwmFreq, pwmResolution);
    ledcAttachPin(PIN_L2, PWM_L2);

    ledcSetup(PWM_R1, pwmFreq, pwmResolution);
    ledcAttachPin(PIN_R1, PWM_R1);

    ledcSetup(PWM_R2, pwmFreq, pwmResolution);
    ledcAttachPin(PIN_R2, PWM_R2);
}

void loop()
{
  ledcWrite(PWM_L1, 255);
  ledcWrite(PWM_L2, 0);
  ledcWrite(PWM_R1, 255);
  ledcWrite(PWM_R2, 0);
//  if (stringComplete)  
//  {
//    // R_motor.run(RELEASE);
//    // L_motor.run(RELEASE);
//
//    if (inputString == "connected" || inputString == "disconnected")
//    {
//      //      Serial.print("stop");
//      R_motor.run(RELEASE); // turns L motor on
//      L_motor.run(RELEASE); // turns R motor on
//    }else if(inputString == "L0"){
//      L_motor.run(RELEASE);
//    }else if(inputString == "R0"){
//      R_motor.run(RELEASE);
//    }
//    else if (inputString[0] == 'L')
//    {
//      if (inputString[1] == '-')
//      {
//        sLength = inputString.length();
//        tempStr = inputString.substring(2, sLength);
//        motorSpeed = tempStr.toInt();
//        L_motor.setSpeed(motorSpeed);
//        //        Serial.print(motorSpeed);
//        L_motor.run(BACKWARD);
//      }
//      else
//      {
//        sLength = inputString.length();
//        tempStr = inputString.substring(1, sLength);
//        motorSpeed = tempStr.toInt();
//        L_motor.setSpeed(motorSpeed);
//        //        Serial.print(motorSpeed);
//        L_motor.run(FORWARD);
//      }
//    }
//    else if (inputString[0] == 'R')
//    {
//      if (inputString[1] == '-')
//      {
//        sLength = inputString.length();
//        tempStr = inputString.substring(2, sLength);
//        motorSpeed = tempStr.toInt();
//        R_motor.setSpeed(motorSpeed);
//        //        Serial.print(motorSpeed);
//        R_motor.run(BACKWARD);
//      }
//      else
//      {
//        sLength = inputString.length();
//        tempStr = inputString.substring(1, sLength);
//        motorSpeed = tempStr.toInt();
//        R_motor.setSpeed(motorSpeed);
//        //        Serial.print(motorSpeed);
//        R_motor.run(FORWARD);
//      }
//    }
//    stringComplete = false;
//    inputString = "";
//  }
}

/*
  SerialEvent occurs whenever a new data comes in the hardware serial RX. This
  routine is run between each time loop() runs, so using delay inside loop can
  delay response. Multiple bytes of data may be available.
*/
//void serialEvent()
//{
//  while (Serial.available())
//  {
//    // get the new byte:
//    char inChar = (char)Serial.read();
//
//    // if the incoming character is a newline, set a flag so the main loop can
//    // do something about it:
//    if (inChar == ':')
//    {
//      stringComplete = true;
//    }
//    else if (inChar != '\n')
//    {
//      // add it to the inputString:
//      inputString += inChar;
//    }
//  }
//}
