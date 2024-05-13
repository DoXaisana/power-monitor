#ifndef includes_h
#define includes_h

#include <PZEM004Tv30.h>      // PZEM 004 v3.0 Library
#include <SoftwareSerial.h>   // Software TX/RX 
#include <ESP8266WiFi.h>      // WiFI library
#include "StackArray.h"
#include <ESPDateTime.h>
#include <WiFiManager.h>
#include <PubSubClient.h>
#include "mqtt.h"

typedef struct {
  int phase;
  float voltage;
  float current;
  float power;
  float energy;
  float frequency;
  float pf;
  DateTimeClass timestamp;
} pzemStruct;

#endif