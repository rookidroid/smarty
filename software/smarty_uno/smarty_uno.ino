#include <WiFi.h>
#include <WiFiUdp.h>

// #include <ESP32Servo.h>

#ifndef APSSID
#define APSSID "smartyrobot"
#define APPSK "smartyrobot"
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

int right_speed = 0;
int left_speed = 0;
int x_val, y_val;
bool rover;

int str_idx;
String inputString = "";

void set_right_motor(int speed)
{
  if (speed >= 0)
  {
    ledcWrite(PWM_R1, speed);
    ledcWrite(PWM_R2, 0);
  }
  else
  {
    ledcWrite(PWM_R1, 0);
    ledcWrite(PWM_R2, -speed);
  }
}

void set_left_motor(int speed)
{
  if (speed >= 0)
  {
    ledcWrite(PWM_L1, speed);
    ledcWrite(PWM_L2, 0);
  }
  else
  {
    ledcWrite(PWM_L1, 0);
    ledcWrite(PWM_L2, -speed);
  }
}

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

  ledcSetup(PWM_L1, pwmFreq, pwmResolution);
  ledcAttachPin(PIN_L1, PWM_L1);

  ledcSetup(PWM_L2, pwmFreq, pwmResolution);
  ledcAttachPin(PIN_L2, PWM_L2);

  ledcSetup(PWM_R1, pwmFreq, pwmResolution);
  ledcAttachPin(PIN_R1, PWM_R1);

  ledcSetup(PWM_R2, pwmFreq, pwmResolution);
  ledcAttachPin(PIN_R2, PWM_R2);

  set_left_motor(0);
  set_right_motor(0);
}

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
          rover = true;
        }
        else if (inputString[0] == 'Y')
        {
          int sLength = inputString.length();
          String tempStr = inputString.substring(1, sLength);
          y_val = tempStr.toInt();
          rover = true;
        }
        else if (inputString[0] == 'L')
        {
          int sLength = inputString.length();
          String tempStr = inputString.substring(1, sLength);
          left_speed = tempStr.toInt();
          set_left_motor(left_speed);
          // rover = true;
        }
        else if (inputString[0] == 'R')
        {
          int sLength = inputString.length();
          String tempStr = inputString.substring(1, sLength);
          right_speed = tempStr.toInt();
          set_right_motor(right_speed);
          // rover = true;
        }

        inputString = "";
      }
    }
  }

  if (rover)
  {
  }
}