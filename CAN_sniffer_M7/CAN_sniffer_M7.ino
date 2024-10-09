#include <mbed.h>
using namespace mbed;
#include <rtos.h>
using namespace rtos;

#include <vector>
using namespace std;

#include "Arduino_H7_Video.h"
#include "Arduino_GigaDisplayTouch.h"

#include "WiFi.h"

#include "can_data.h"
//#include "lvgl_graphics.h"

char ssid[] = "CAN_sniffer";
char pass[] = "123456789";

int status = WL_IDLE_STATUS;
WiFiServer server(80);

Mutex mutex;

Arduino_H7_Video Display(800, 480, GigaDisplayShield);
Arduino_GigaDisplayTouch TouchScreen;

Thread readCANBusThread;
Thread paintDisplayThread;
Thread sendDataWifiThread;

void readCANBus(void);
void paintDisplay(void);
void sendDataWifi(void);

void setup() {
  Serial.begin(115200);

  can.frequency(500000);

  Display.begin();
  TouchScreen.begin();

  if (WiFi.status() == WL_NO_MODULE) {
    Serial.println("Communication with WiFi module failed!");
    while (true);
  }

  Serial.print("Creating access point named: ");
  Serial.println(ssid);

  status = WiFi.beginAP(ssid, pass);
  if (status != WL_AP_LISTENING) {
    Serial.println("Creating access point failed");
    while (true);
  }

  server.begin();

  printWiFiStatus();

  readCANBusThread.start(readCANBus);
  paintDisplayThread.start(paintDisplay);
  sendDataWifiThread.start(sendDataWifi);

  //lv_init();

  //drawContainter();

  //drawButton();

  //drawButtonLabel();
}

void loop() {}

void readCANBus(void) {
  while (1) {
    
    String test = "test";
    mutex.lock();
    canHTMLMessages = test + "<br>" + canHTMLMessages;
    mutex.unlock();
    

    if (can.read(canMsg)) {
      String msg = "ID: " + (String)canMsg.id + ", LEN: " + (String)canMsg.len + ", DATA: ";
      for (int i = 0; i < canMsg.len; ++i) {
        msg += (String)canMsg.data[i] + " ";
      }
      Serial.println(msg);

      mutex.lock();
      canHTMLMessages = msg + "<br>" + canHTMLMessages;
      mutex.unlock();

      switch(canMsg.id) {
        case POSITION_X:
          PositionX = (int64_t)(((int64_t)canMsg.data[0]<<56) | ((int64_t)canMsg.data[1]<<48) | ((int64_t)canMsg.data[2]<<40) | ((int64_t)canMsg.data[3]<<32) | 
                                ((int64_t)canMsg.data[4]<<24) | ((int64_t)canMsg.data[5]<<16) | ((int64_t)canMsg.data[6]<<8) | ((int64_t)canMsg.data[7]));
          break;
        case POSITION_Y:
          PositionY = (int64_t)(((int64_t)canMsg.data[0]<<56) | ((int64_t)canMsg.data[1]<<48) | ((int64_t)canMsg.data[2]<<40) | ((int64_t)canMsg.data[3]<<32) |
                                ((int64_t)canMsg.data[4]<<24) | ((int64_t)canMsg.data[5]<<16) | ((int64_t)canMsg.data[6]<<8) | ((int64_t)canMsg.data[7]));
          break;
        case POSITION_VX:
          PositionVx.erase(PositionVx.begin());
          PositionVx.push_back((int32_t)(((int32_t)canMsg.data[0]<<24) | ((int32_t)canMsg.data[1]<<16) | ((int32_t)canMsg.data[2]<<8) | ((int32_t)canMsg.data[3])));
          break;
        case POSITION_VY:
          PositionVy.erase(PositionVy.begin());
          PositionVy.push_back((int32_t)(((int32_t)canMsg.data[0]<<24) | ((int32_t)canMsg.data[1]<<16) | ((int32_t)canMsg.data[2]<<8) | ((int32_t)canMsg.data[3])));
          break;
        case MOTOR_ACCDIR:
          MotorDir = (int8_t)(canMsg.data[0]);
          
          MotorAcc.erase(MotorAcc.begin());
          MotorAcc.push_back((int8_t)(canMsg.data[1]));
          break;
        case LIDAR_ANGLE:
          LidarAngle = (int16_t)(((int16_t)canMsg.data[0]<<8) | ((int16_t)canMsg.data[1]));
          break;
        case CAMERA_ANGLE:
          CameraAngle = (int16_t)(((int16_t)canMsg.data[0]<<8) | ((int16_t)canMsg.data[1]));
          break;
        default:
          break;
      }
    }

    ThisThread::sleep_for(50);
  }
}

void paintDisplay(void) {
  while (1) {
/*
    switch (displayScreen) {
      case ACC_SCREEN:
        //paintAcceleration();
        break;
      case VEL_SCREEN:
        //paintVelocity();
        break;
      default:
        break;
    }

    lv_timer_handler();
*/
    ThisThread::sleep_for(50);
  }
}


void sendDataWifi(void) {
  while (1) {
    WiFiClient client = server.available();  // Listen for incoming clients

    if (client) {
      Serial.println("new client");

      if (client.connected()) {
        // Read the client request
        String request = client.readStringUntil('\r');
        Serial.println(request);

        // Send the response
        client.println("HTTP/1.1 200 OK");
        client.println("Content-type:text/html");
        client.println();

        client.print("<!DOCTYPE html> <body>");
        client.print(canHTMLMessages);
        client.print("<script> setInterval(function() { location.reload(); }, 1000); </script> </body> </html>");

        client.println();
      }
      
      client.stop();
      Serial.println("client disconnected");    
    }

    ThisThread::sleep_for(100);
  }
}

// ------------------------------------------------------------

void printWiFiStatus() {
  // Print the SSID of the network you're attached to:
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());

  // Print your WiFi shield's IP address:
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);
}
