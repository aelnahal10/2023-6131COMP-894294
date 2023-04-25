#ifndef FAULT_H
#define FAULT_H

// Fault-related global variables and functions from your code
int tempMin = 0;
int tempMax = 5;
int humidityMin = 0;
int humidityMax = 5;
unsigned long timee_ms5000 = 0;
unsigned long time_ms500 = 0;
unsigned long time_ms1500 = 0;
unsigned long time_ms10000 = 0;
unsigned long currentTime = 0;
unsigned int globalFaultFlag = 0;
bool toggleFlag = 0;
bool tempFaultflag = 0;
bool humidityFaultFlag = 0;
float humidity;
float temperature;

bool tempCheck(double temp);
bool humidityCheck(double humidity);
void faultCheck(double temp, double humidity);
void faultAlert();

#endif
