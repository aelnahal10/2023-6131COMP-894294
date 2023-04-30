#ifndef FAULT_H
#define FAULT_H


#define blueLed 25
#define redLed 16
#define greenLed 26

#define rotaryClk 4
#define rotaryDT 2
#define rotarySW 0
#define constraintsPushButton 14

bool tempCheck(double temp);
bool humidityCheck(double humidity);
void faultCheck(double temp, double humidity);
void faultAlert();

#endif
