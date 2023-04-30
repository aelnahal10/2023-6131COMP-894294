#ifndef TRANSMISSION_H
#define TRANSMISSION_H
#include <ArduinoJson.h>
#include <WiFi.h>



void setupWiFi(const char *ssid, const char *password);
void transmitData(float temperature, float humidity);

#endif
