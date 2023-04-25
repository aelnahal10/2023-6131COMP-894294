#ifndef TRANSMISSION_H
#define TRANSMISSION_H
#include <ArduinoJson.h>
#include <WiFi.h>

const char *ssid = "Mo's";
const char *password = "Matrix1.2.3";
const char *server_ip = "192.168.112.216";
const int server_port = 8081;

void setupWiFi(const char *ssid, const char *password);
void transmitData(float temperature, float humidity);

#endif
