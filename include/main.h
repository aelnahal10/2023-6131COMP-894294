#ifndef main_h
#define main_h
#include <Arduino.h>
#include "DHTesp.h" 

#define rotaryClk 4
#define rotartyDT 2
#define rotarySW 0
#define dht_pin 17
#define blueLed 25
#define redLed 16
#define greenLed 26
#define constraintsPushButton 14


bool tempCheck(double temp); //checks if temp is in range
bool humidityCheck(double humidity); // checks if humidity is in range
void faultCheck(double temp, double humidity); // checks if there is any sesnor reading is outside of range and sets fault flag
void faultAlert(); //turn on/off correct LED based of fault flags 
void rotaryCallBack(); 
void pushButtonCallBack();


#endif