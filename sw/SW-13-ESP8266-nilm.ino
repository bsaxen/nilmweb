//============================================
// main.cpp
// SW-13-ESP12E-nilm.ino
// 2016-10-08
//============================================
#include "nabton/NabtonClient.h"
#include "../config/configuration.h"

//Add the modules you want to use
//#include "nabton/GpsModule.h"

using namespace nabton;
NabtonClient* nabtonClient;

#ifdef D_ESP8266
    ADC_MODE(ADC_VCC);
#endif

double* data_ = NULL;
int nSids_;

// CUSTOM variables here
//const byte httpLed = 2;
//const byte measLed = 3;
//const byte interruptPin = 0;

const byte httpLed = 12;
const byte measLed = 13;
const byte interruptPin = 14;
// - END OF - CUSTOM variables

int timeToCheckStatus = 0;
unsigned long t1,t2,dt;
float elpow = 0.0;
int ledOn = 1;

void measure()
{
    ledOn++;
    if(ledOn > 1)ledOn = 0;
    if(ledOn==1)digitalWrite(measLed,LOW);
    if(ledOn==0)digitalWrite(measLed,HIGH);
    t2 = t1;
    t1 = millis();
    dt = t1 - t2;
    elpow = 3600./dt*1000.;
}



void setup() {

    pinMode(httpLed, OUTPUT);
    pinMode(measLed, OUTPUT);
    pinMode(interruptPin, INPUT_PULLUP);
    digitalWrite(httpLed,LOW);
    digitalWrite(measLed,LOW);


       nabtonClient = new NabtonClient(CONFIG_APPLICATION_NAME, CONFIG_CLIENT_TYPE);
  //     //Initialize serial support
  //     Serial.begin(CONFIG_BAUDRATE);
  //     delay(100);
  //     //Initialize wifi support
       WifiManager::getInstance(CONFIG_SSID, CONFIG_SSID_PP, CONFIG_BROKER_IP,
                                CONFIG_BROKER_PORT, CONFIG_HOST, CONFIG_HOST_PORT,
                                CONFIG_LOW_POWER_MODE);

       digitalWrite(httpLed,HIGH);
       delay(300);
       digitalWrite(httpLed,LOW);
       delay(300);
       digitalWrite(httpLed,HIGH);
       delay(300);
       digitalWrite(httpLed,LOW);
       delay(300);
       digitalWrite(httpLed,HIGH);
       delay(300);
       digitalWrite(httpLed,LOW);
       delay(3000);

       nSids_ = sizeof(CONFIG_SIDS)/sizeof(CONFIG_SIDS[0]);
       data_ = new double[nSids_];
  //
  //     // ########################################################################
  //     //    Now you have the basics all set up. Send logs to your computer either
  //     //    over Serial or WifiManager.
  //     // ########################################################################
  //     //ULOG_DEBUG << "This is a debug message over serial port";
  //     //WLOG_DEBUG << "This is a debug message over wifi";
  //
  // MID = 3 MAILBOX
  //nabtonClient->sendNabtonDataPackage(CONFIG_APPLICATION_ID,3,1,(int*)CONFIG_SIDS,data_);
  attachInterrupt(interruptPin, measure, FALLING);
}

void loop() {
      timeToCheckStatus++;
      // Check every 10s. Monitors Wifi connection and more. Reconnects if lost
      if(timeToCheckStatus == (int)(10000/CONFIG_REPORT_FREQUENCY_MILLISECONDS))
      {
        timeToCheckStatus = 0;
        nabtonClient->updateStatus();
      }
      delay(CONFIG_REPORT_FREQUENCY_MILLISECONDS);
      digitalWrite(httpLed,HIGH);
      data_[0] = double(elpow);
      nabtonClient->sendNabtonDataPackage(CONFIG_APPLICATION_ID,1,1,(int*)CONFIG_SIDS,data_,false);
      digitalWrite(httpLed,LOW);
}
