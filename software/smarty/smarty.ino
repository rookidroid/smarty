/*
 *
 *    This sketch is for the smarty rover
 *
 *    ----------
 *    Copyright (C) 2022 - PRESENT  Zhengyu Peng
 *    Website: https://zpeng.me
 *
 *    `                      `
 *    -:.                  -#:
 *    -//:.              -###:
 *    -////:.          -#####:
 *    -/:.://:.      -###++##:
 *    ..   `://:-  -###+. :##:
 *           `:/+####+.   :##:
 *    .::::::::/+###.     :##:
 *    .////-----+##:    `:###:
 *     `-//:.   :##:  `:###/.
 *       `-//:. :##:`:###/.
 *         `-//:+######/.
 *           `-/+####/.
 *             `+##+.
 *              :##:
 *              :##:
 *              :##:
 *              :##:
 *              :##:
 *               .+:
 *
 */

#include <WiFi.h>
#include <WiFiUdp.h>

// #include <ESP32Servo.h>

/**
 * @brief WiFi parameters
 */
// set WiFi credentials
#ifndef APSSID
#define APSSID "smartyrobot_cathy"
#define APPSK "smartyrobot_cathy"
#endif
const char *ssid = APSSID;
const char *password = APPSK;

char packetBuffer[4096 + 1];   // buffer to hold incoming packet
unsigned int localPort = 1234; // local port to listen on

WiFiUDP Udp;

const int MID = 0;
const int MIN = -255;
const int MAX = 255;
const int MARGIN = 80;

const int center_var_x = 2048;
const int center_var_y = 2048;
const float scale_x_upper = (float)(MAX - MID) / (4096.0 - (float)center_var_x);
const float scale_x_lower = (float)(MAX - MID) / ((float)center_var_x);
const float scale_y_upper = (float)(MAX - MID) / (4096.0 - (float)center_var_y);
const float scale_y_lower = (float)(MAX - MID) / ((float)center_var_y);

int x_val = center_var_x;
int y_val = center_var_y;

float main_dutycycle = 0;
float offset_dutycycle = 0;

int left_duty;
int right_duty;

/**
 * @brief Motor pins
 */
#define PIN_R2 25 // left motor pin 1
#define PIN_R1 33 // left motor pin 2
#define PIN_L2 27 // right motor pin 1
#define PIN_L1 26 // right motor pin 2

#define PIN_LED1 18
#define PIN_LED2 19


/**
 * @brief PWM channels
 */

const int pwmFreq = 5000; // PWM frequency
const int pwmResolution = 8; // PWM resolution

int right_speed = 0;
int left_speed = 0;

bool rover;

int str_idx;
String inputString = "";

/**
 * @brief Set the right motor speed
 * 
 * @param speed motor speed, -255 ~ 255
 */
void set_right_motor(int speed)
{
  if (speed >= 0)
  {
    ledcWrite(PIN_R1, speed);
    ledcWrite(PIN_R2, 0);
  }
  else
  {
    ledcWrite(PIN_R1, 0);
    ledcWrite(PIN_R2, -speed);
  }
}

/**
 * @brief Set the left motor speed
 * 
 * @param speed motor speed, -255 ~ 255
 */
void set_left_motor(int speed)
{
  if (speed >= 0)
  {
    ledcWrite(PIN_L1, speed);
    ledcWrite(PIN_L2, 0);
  }
  else
  {
    ledcWrite(PIN_L1, 0);
    ledcWrite(PIN_L2, -speed);
  }
}

/**
 * @brief Initial setup
 * 
 */
void setup()
{
  Serial.begin(115200);

  WiFi.softAP(ssid, password);

  IPAddress myIP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(myIP);

  Udp.begin(localPort);

  // configure LED PWM functionalitites
  pinMode(PIN_L1, OUTPUT);
  pinMode(PIN_L2, OUTPUT);
  pinMode(PIN_R1, OUTPUT);
  pinMode(PIN_R2, OUTPUT);

  ledcAttach(PIN_L1, pwmFreq, pwmResolution);
  ledcAttach(PIN_L2, pwmFreq, pwmResolution);

  ledcAttach(PIN_R1, pwmFreq, pwmResolution);
  ledcAttach(PIN_R2, pwmFreq, pwmResolution);

  pinMode(PIN_LED1, OUTPUT);
  pinMode(PIN_LED2, OUTPUT);
  ledcAttach(PIN_LED1, pwmFreq, pwmResolution);
  ledcAttach(PIN_LED2, pwmFreq, pwmResolution);

  ledcWrite(PIN_LED1, 128);
  ledcWrite(PIN_LED2, 128);

  set_left_motor(0);
  set_right_motor(0);
}

/**
 * @brief Loop
 * 
 */
void loop()
{
  int packetSize = Udp.parsePacket();
  if (packetSize)
  {
    rover = false;
    // read the packet into packetBufffer
    int n = Udp.read(packetBuffer, 4096);
    packetBuffer[n] = 0;

    for (str_idx = 0; str_idx < n; str_idx++)
    {
      char inChar = packetBuffer[str_idx];
      //      Serial.print(inChar);
      //      Serial.print("\n");

      if (inChar != '\n' && inChar != ':')
      {
        // add it to the inputString:
        inputString += inChar;
      }
      else if (inChar == ':')
      {
        if (inputString[0] == 'X')
        {
          int sLength = inputString.length();
          String tempStr = inputString.substring(1, sLength);
          x_val = tempStr.toInt();
          Serial.print('X');
          Serial.print(x_val);
          Serial.print("\n");
          rover = true;
        }
        else if (inputString[0] == 'Y')
        {
          int sLength = inputString.length();
          String tempStr = inputString.substring(1, sLength);
          y_val = tempStr.toInt();
          Serial.print('Y');
          Serial.print(y_val);
          Serial.print("\n");
          rover = true;
        }
        else if (inputString[0] == 'L')
        {
          int sLength = inputString.length();
          String tempStr = inputString.substring(1, sLength);
          left_speed = tempStr.toInt();
          set_left_motor(left_speed);
        }
        else if (inputString[0] == 'R')
        {
          int sLength = inputString.length();
          String tempStr = inputString.substring(1, sLength);
          right_speed = tempStr.toInt();
          set_right_motor(right_speed);
        }

        inputString = "";
      }
    }
  }

  if (rover)
    {
      if (y_val >= center_var_y)
      {
        main_dutycycle = (float)(center_var_y - y_val) * scale_y_upper;
      }
      else
      {
        main_dutycycle = (float)(center_var_y - y_val) * scale_y_lower;
      }
  
      if (x_val >= center_var_x)
      {
        offset_dutycycle = (float)(center_var_x - x_val) * scale_x_upper;
      }
      else
      {
        offset_dutycycle = (float)(center_var_x - x_val) * scale_x_lower;
      }
  
      left_duty = round((main_dutycycle + offset_dutycycle) - MID);
      right_duty = round((main_dutycycle - offset_dutycycle) - MID);
  
      if (left_duty >= MIN && left_duty <= MAX)
      {
        if (abs(left_duty)<MARGIN)
        {
          set_left_motor(0);
        }
        else
        {
          set_left_motor(left_duty);
        }
      }
      else if (left_duty < MIN)
      {
        set_left_motor(MIN);
      }
      else if (left_duty > MAX)
      {
        set_left_motor(MAX);
      }
  
      if (right_duty >= MIN && right_duty <= MAX)
      {
        if (abs(right_duty)<MARGIN)
        {
          set_right_motor(0);
        }
        else
        {
          set_right_motor(right_duty);
        }
      }
      else if (right_duty < MIN)
      {
        set_right_motor(MIN);
      }
      else if (right_duty > MAX)
      {
        set_right_motor(MAX);
      }
    }
}
