#include <DHTesp.h>
#include <WiFi.h>
#include <ArduinoJson.h>
#include <Preferences.h>
#include <WebServer.h>
#include "main.h"


int *constraints[4];
int constraintsIndex = 0; 
int rotaryCount = 0;
int rotaryClkPinLast = LOW;
int rotaryClkPinCurrent = LOW;
float trTemp;
float trHumidity;
unsigned long last_transmission_time = 0;

void rotaryCallBack() // call back for pins interrupts
{        
               // check if current clk is different from previous clk value
  int  tempNewVal;
  rotaryClkPinCurrent = digitalRead(rotaryClk);
  if (rotaryClkPinCurrent != rotaryClkPinLast)
  { // check if current DT is different from previous DT value
    if (digitalRead(rotaryDT) != rotaryClkPinLast)
    {
      tempNewVal = constraints[constraintsIndex];
      tempNewVal++;

    }
    else
    { 
      tempNewVal = constraints[constraintsIndex];
      tempNewVal--;
    }
    check_and_set_value(constraints, tempNewVal, constraintsIndex);
  }

  if (digitalRead(rotarySW) != 1)
  {
    rotaryCount = 0;
  }

  rotaryClkPinLast = rotaryClkPinCurrent;
}

void pushButtonCallBack()
{
  constraintsIndex++;
  if(constraintsIndex == 4)
  {
    constraintsIndex = 0;
  }
  switch (constraintsIndex) {
      case 0:
        Serial.println("You are modifying the min temprature constraint")        
        break;
      case 1:
        Serial.println("You are modifying the max temprature constraint") 
        break;
      case 2:
        Serial.println("You are modifying the min humidity constraint") 
        break;
      case 3:
        Serial.println("You are modifying the max humidity constraint") 
        break;
    }
}


bool is_min_valid_constraints(int min_value, int max_value) {
  if (min_value > max_value) {
    Serial.println("Error: minimum value is greater than maximum value.");
    return false;
  }
  return true;
}

bool is_max_valid_constraints(int min_value, int max_value) {
  if (min_value > max_value) {
    Serial.println("Error: maximum value is smaller than minimum value.");
    return false;
  }
  return true;
}

void check_and_set_value(int* constraints, int new_value, int var_index) {

    // value is valid, set new value and prompt user
    bool varCheck;
    switch (var_index) {
      case 0:
        varCheck = is_min_valid_constraints(constraints[0], constraints[1]);
        break;
      case 1:
        varCheck = is_max_valid_constraints(constraints[0], constraints[1]);
        break;
      case 2:
        varCheck = is_min_valid_constraints(constraints[2], constraints[3]);
        break;
      case 3:
        varCheck = is_maxn_valid_constraints(constraints[2], constraints[3]);
        break;
    }
    if(varCheck){
      constraints[var_index] = new_value;
      Serial.print("New value for variable ");
      Serial.print(": ");
      Serial.println(new_value);
    }
  
}


void setup()
{
  Serial.begin(9600);
  pinMode(blueLed, OUTPUT);
  pinMode(redLed, OUTPUT);
  pinMode(rotaryClk, INPUT);
  pinMode(rotaryDT, INPUT);
  pinMode(rotarySW, INPUT_PULLUP);
  pinMode(constraintsPushButton, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(rotaryClk), rotaryCallBack, CHANGE);
  attachInterrupt(digitalPinToInterrupt(rotaryDT), rotaryCallBack, CHANGE);
  constraints[0] = &tempMin;
  constraints[1] = &tempMax;
  constraints[2] = &humidityMin;
  constraints[3] = &humidityMax;
  // use this instead:
  setupDHT(dht_pin);
  
  attachInterrupt(digitalPinToInterrupt(constraintsPushButton), pushButtonCallBack, RISING);

  setupWiFi(ssid, password);
}
// the loop function runs over and over again forever
void loop()
{

  // sensor sampling time can not be less than one sec according to senseor data sheet; Not available when one second exactly.
  currentTime = millis();
  unsigned long current_time2 = millis();

  if (currentTime > time_ms1500 + 1500)
  {
    time_ms1500 = currentTime;
    humidity = getHumidity();
    temperature = getTemperature();
    faultCheck(temperature, humidity);
  }

  if (currentTime > timee_ms5000 + 5000) // display readings every 5 seconds
  {
    Serial.print("T = ");
    Serial.println(temperature);
    Serial.print("RH = ");
    Serial.println(humidity);
    timee_ms5000 = currentTime;
    Serial.println(rotaryCount); // testing 
  }

  if (currentTime > time_ms10000 + 10000) // set other readings readings every 10 seconds
  {
   trTemp = temperature;
   trHumidity = humidity;
  }
  faultAlert();
  
  // Transmit the readings in a batch every 30-60 seconds
  if (current_time2 - last_transmission_time >= 30000 && current_time2 - last_transmission_time < 60000)
  {
    last_transmission_time = current_time2;
    transmitData(trTemp,trHumidity);
  }
}