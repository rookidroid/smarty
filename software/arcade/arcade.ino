/*
 *
 *    This sketch is for the WiFi joystick
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

// GPIO pin number for the LEDs
#define PIN_GREEN 0
#define PIN_YELLOW 1

// GPIO pin number for the joystick
#define JS_UP 16
#define JS_DOWN 17
#define JS_LEFT 19
#define JS_RIGHT 18

#define BT_UP 13
#define BT_DOWN 12
#define BT_LEFT 14
#define BT_RIGHT 15

// PWM channels
#define PWM_GREEN 0
#define PWM_YELLOW 1

// WiFi parameters
const char *ssid = "smartyrobot";
const char *password = "smartyrobot";
boolean connected = false;

// UDP
WiFiUDP udp;
const char *udpAddress = "192.168.4.1";
const int udpPort = 1234;

// PWM properties
const int pwmFreq = 5000;
const int pwmResolution = 8;

// joystick values
int valX = 0;
int valY = 0;
int valSw = 0;

// old joystick values
int js_up = 1;
int js_down = 1;
int js_left = 1;
int js_right = 1;

int bt_up = 1;
int bt_down = 1;
int bt_left = 1;
int bt_right = 1;

// new joystick values
int js_up_new = 1;
int js_down_new = 1;
int js_left_new = 1;
int js_right_new = 1;

int bt_up_new = 1;
int bt_down_new = 1;
int bt_left_new = 1;
int bt_right_new = 1;

void setup()
{
  // initilize hardware serial:
  Serial.begin(115200);

  pinMode(JS_UP, INPUT_PULLUP);
  pinMode(JS_DOWN, INPUT_PULLUP);
  pinMode(JS_LEFT, INPUT_PULLUP);
  pinMode(JS_RIGHT, INPUT_PULLUP);
  pinMode(BT_UP, INPUT_PULLUP);
  pinMode(BT_DOWN, INPUT_PULLUP);
  pinMode(BT_LEFT, INPUT_PULLUP);
  pinMode(BT_RIGHT, INPUT_PULLUP);

  // configure LED PWM functionalitites
  pinMode(PIN_GREEN, OUTPUT);
  pinMode(PIN_YELLOW, OUTPUT);

  // blink LEDs
  analogWrite(PIN_GREEN, 2);
  analogWrite(PIN_YELLOW, 2);
  delay(1000);
  analogWrite(PIN_GREEN, 0);

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print('.');
    delay(500);
  }
  Serial.print("Connected! IP address: ");
  Serial.println(WiFi.localIP());

  analogWrite(PIN_YELLOW, 0);
  analogWrite(PIN_GREEN, 2);
}

void loop()
{
  js_up_new = digitalRead(JS_UP);
  js_down_new = digitalRead(JS_DOWN);
  js_left_new = digitalRead(JS_LEFT);
  js_right_new = digitalRead(JS_RIGHT);

  bt_up_new = digitalRead(BT_UP);
  bt_down_new = digitalRead(BT_DOWN);
  bt_left_new = digitalRead(BT_LEFT);
  bt_right_new = digitalRead(BT_RIGHT);

  if (js_up != js_up_new || js_down != js_down_new || js_left != js_left_new || js_right != js_right_new)
  {
    js_up = js_up_new;
    js_down = js_down_new;
    js_left = js_left_new;
    js_right = js_right_new;
    if ((js_up + js_down + js_left + js_right) == 4)
    {
      udp.beginPacket(udpAddress, udpPort);
      udp.printf("L0:R0:");
      udp.endPacket();
    }
    else if ((js_up + js_down + js_left + js_right) == 2)
    {
      if (js_up == 0 && js_left == 0)
      {
        udp.beginPacket(udpAddress, udpPort);
        udp.printf("L-128:R-255:");
        udp.endPacket();
      }
      else if (js_up == 0 && js_right == 0)
      {
        udp.beginPacket(udpAddress, udpPort);
        udp.printf("L-255:R-128:");
        udp.endPacket();
      }
      else if (js_down == 0 && js_left == 0)
      {
        udp.beginPacket(udpAddress, udpPort);
        udp.printf("L128:R255:");
        udp.endPacket();
      }
      else if (js_down == 0 and js_right == 0)
      {
        udp.beginPacket(udpAddress, udpPort);
        udp.printf("L255:R128:");
        udp.endPacket();
      }
      else
      {
        udp.beginPacket(udpAddress, udpPort);
        udp.printf("L0:R0:");
        udp.endPacket();
      }
    }
    else
    {
      if (js_up == 0)
      {
        udp.beginPacket(udpAddress, udpPort);
        udp.printf("L-255:R-255:");
        udp.endPacket();
      }
      else if (js_down == 0)
      {
        udp.beginPacket(udpAddress, udpPort);
        udp.printf("L255:R255:");
        udp.endPacket();
      }
      else if (js_left == 0)
      {
        udp.beginPacket(udpAddress, udpPort);
        udp.printf("L255:R-255:");
        udp.endPacket();
      }
      else if (js_right == 0)
      {
        udp.beginPacket(udpAddress, udpPort);
        udp.printf("L-255:R255:");
        udp.endPacket();
      }
    }
  }
  else if (bt_up != bt_up_new || bt_down != bt_down_new|| bt_left != bt_left_new || bt_right != bt_right_new)
  {
    bt_up = bt_up_new;
    bt_down = bt_down_new;
    bt_left = bt_left_new;
    bt_right = bt_right_new;
    if ((bt_up+bt_down+bt_left+bt_right) == 4)
    {
      udp.beginPacket(udpAddress, udpPort);
      udp.printf("L0:R0:");
      udp.endPacket();
    }
    else if ((bt_up+bt_down+bt_left+bt_right) == 2)
    {
        if (bt_up == 0 && bt_left == 0)
        {
          udp.beginPacket(udpAddress, udpPort);
          udp.printf("L-128:R-255:");
          udp.endPacket();
        }
        else if (bt_up == 0 && bt_right == 0)
        {
          udp.beginPacket(udpAddress, udpPort);
          udp.printf("L-255:R-128:");
          udp.endPacket();
        }
        else if (bt_down == 0 && bt_left == 0)
        {
          udp.beginPacket(udpAddress, udpPort);
          udp.printf("L128:R255:");
          udp.endPacket();
        }
        else if (bt_down == 0 && bt_right == 0)
        {
          udp.beginPacket(udpAddress, udpPort);
          udp.printf("L255:R128:");
          udp.endPacket();
        }
        else
        {
          udp.beginPacket(udpAddress, udpPort);
          udp.printf("L0:R0:");
          udp.endPacket();
        }
    }
    else
    {
        if (bt_up == 0)
        {
          udp.beginPacket(udpAddress, udpPort);
          udp.printf("L-255:R-255:");
          udp.endPacket();
        }
        else if (bt_down == 0)
        {
          udp.beginPacket(udpAddress, udpPort);
          udp.printf("L255:R255:");
          udp.endPacket();
        }
        else if (bt_left == 0)
        {
          udp.beginPacket(udpAddress, udpPort);
          udp.printf("L255:R-255:");
          udp.endPacket();
        }
        else if (bt_right == 0)
        {
          udp.beginPacket(udpAddress, udpPort);
          udp.printf("L-255:R255:");
          udp.endPacket();
        }
    }
  }
}
