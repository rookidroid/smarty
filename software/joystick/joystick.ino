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
#define PIN_GREEN 27
#define PIN_YELLOW 26
#define PIN_BLUE 25

// GPIO pin number for the joystick
#define PIN_X 35
#define PIN_Y 34
#define PIN_SW 4


// PWM channels
#define PWM_GREEN 0
#define PWM_YELLOW 1
#define PWM_BLUE 2

// WiFi parameters
const char *ssid = "smartyrobot";
const char *password = "smartyrobot";
//const char *ssid = "emmawifi";
//const char *password = "8067868889";
boolean connected = false;

// UDP
WiFiUDP udp;
const char *udpAddress = "192.168.4.1";
//const char *udpAddress = "192.168.1.202";
const int udpPort = 1234;

// PWM properties
const int pwmFreq = 5000;
const int pwmResolution = 8;

// joystick values
int valX = 0;
int valY = 0;
int valSw = 0;

// old joystick values
int valX_old = 0;
int valY_old = 0;
int valSw_old = 0;


void setup()
{
    adcAttachPin(PIN_Y);
    adcAttachPin(PIN_X);

    analogSetClockDiv(64);
  
    // initilize hardware serial:
    Serial.begin(921600);

    // configure joystick pin mode
    pinMode(PIN_Y, ANALOG);
    pinMode(PIN_X, ANALOG);

    pinMode(PIN_SW, INPUT_PULLUP);

    // configure LED PWM functionalitites
    pinMode(PIN_GREEN, OUTPUT);
    pinMode(PIN_YELLOW, OUTPUT);
    pinMode(PIN_BLUE, OUTPUT);

    ledcSetup(PWM_GREEN, pwmFreq, pwmResolution);
    ledcAttachPin(PIN_GREEN, PWM_GREEN);

    ledcSetup(PWM_YELLOW, pwmFreq, pwmResolution);
    ledcAttachPin(PIN_YELLOW, PWM_YELLOW);

    ledcSetup(PWM_BLUE, pwmFreq, pwmResolution);
    ledcAttachPin(PIN_BLUE, PWM_BLUE);

    // blink LEDs
    ledcWrite(PWM_GREEN, 2);
    ledcWrite(PWM_YELLOW, 2);
    ledcWrite(PWM_BLUE, 2);
    delay(1000);
    ledcWrite(PWM_GREEN, 0);
    ledcWrite(PWM_YELLOW, 0);
    ledcWrite(PWM_BLUE, 0);

    // connect to the WiFi network
    connectToWiFi(ssid, password);
}

void loop()
{
    valX = analogRead(PIN_X);
    valY = analogRead(PIN_Y);
    valSw = digitalRead(PIN_SW);

    int temp_valX = floor(valX/32);
    int temp_valY = floor(valY/32);

    int temp_valX_old = floor(valX_old/32);
    int temp_valY_old = floor(valY_old/32);

    // only send UDP when the joystick is moved
    if ((temp_valX != temp_valX_old) || (temp_valY != temp_valY_old) || (valSw != valSw_old))
    {
        valX_old = valX;
        valY_old = valY;
        valSw_old = valSw;
        if (connected)
        {
            ledcWrite(PWM_BLUE, 2);
            // send joystick values to the UDP server
            udp.beginPacket(udpAddress, udpPort);
            udp.printf("X%d:Y%d:S%d:", valX, valY, valSw);
//            udp.printf("X%d:Y%d:S%d:", temp_valX, temp_valY, valSw);
            udp.endPacket();
            ledcWrite(PWM_BLUE, 0);
        }
    }

    delay(10);
}

void connectToWiFi(const char *ssid, const char *pwd)
{
    ledcWrite(PWM_YELLOW, 2);
    Serial.println("Connecting to WiFi network: " + String(ssid));

    // delete old config
    WiFi.disconnect(true);
    // register event handler
    WiFi.onEvent(WiFiEvent);

    // initiate connection
    WiFi.begin(ssid, pwd);

    Serial.println("Waiting for WIFI connection...");
}

// WiFi event handler
//void WiFiEvent(WiFiEvent_t event)
//{
//    switch (event)
//    {
//    case ARDUINO_EVENT_WIFI_STA_GOT_IP:
//        // when connected set
//        Serial.print("WiFi connected! IP address: ");
//        Serial.println(WiFi.localIP());
//        // initializes the UDP state
//        // this initializes the transfer buffer
//        udp.begin(WiFi.localIP(), udpPort);
//        connected = true;
//        ledcWrite(PWM_YELLOW, 0);
//        ledcWrite(PWM_GREEN, 2);
//        break;
//    case ARDUINO_EVENT_WIFI_STA_DISCONNECTED:
//        Serial.println("WiFi lost connection");
//        connected = false;
//        ledcWrite(PWM_YELLOW, 2);
//        ledcWrite(PWM_GREEN, 0);
//        break;
//    default:
//        break;
//    }
//}
