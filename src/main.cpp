#include <DHTesp.h>
#include <WiFi.h>
#include <ArduinoJson.h>
#include <Preferences.h>
#include <WebServer.h>
#include "main.h"


int *constraints[4];

int rotaryCount = 0;
int rotaryClkPinLast = LOW;
int rotaryClkPinCurrent = LOW;
float trTemp;
float trHumidity;
unsigned long last_transmission_time = 0;

void rotaryCallBack() // call back for pins interrupts
{                     // check if current clk is different from previous clk value
  rotaryClkPinCurrent = digitalRead(rotaryClk);
  if (rotaryClkPinCurrent != rotaryClkPinLast)
  { // check if current DT is different from previous DT value
    if (digitalRead(rotaryDT) != rotaryClkPinLast)
    {
      rotaryCount++;
    }
    else
    {
      rotaryCount--;
    }
  }

  if (digitalRead(rotarySW) != 1)
  {
    rotaryCount = 0;
  }

  rotaryClkPinLast = rotaryClkPinCurrent;
}

void pushButtonCallBack()
{
  Serial.println("Test");
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