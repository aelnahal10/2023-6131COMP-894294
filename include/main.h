#ifndef main_h
#define main_h

#include <Arduino.h> 
#include "sensor.h"
#include "fault.h"
#include "transmission.h"


#define rotaryClk 4
#define rotaryDT 2
#define rotarySW 0
#define dht_pin 17
#define blueLed 25
#define redLed 16
#define greenLed 26
#define constraintsPushButton 14

void rotaryCallBack(); 
void pushButtonCallBack();


#endif