#include "fault.h"
#include "main.h"
#include "sensor.h"
#include <DHTesp.h>
// Fault-related global variables and functions from your code
// ...

bool tempCheck(double temp)
{
  bool status;
  if (temp > tempMax || temp < tempMin)
  {
    status = 1;
  }

  else
  {
    status = 0;
  }
  return status;
}

bool humidityCheck(double humidity)
{
  bool status;
  if (humidity > humidityMax || humidity < humidityMin)
  {
    status = 1;
  }

  else
  {
    status = 0;
  }
  return status;
}

void faultCheck(double temp, double humidity)
{
  bool tempStatus = tempCheck(temp);
  bool humidityStatus = humidityCheck(humidity);
  if (tempStatus && humidityStatus)
  {
    globalFaultFlag = 1;
  }
  else
  {
    globalFaultFlag = 0;
    if (tempStatus)
    {
      tempFaultflag = 1;
    }
    else
    {
      tempFaultflag = 0;
    }
    if (humidityStatus)
    {
      humidityFaultFlag = 1;
    }
    else
    {
      humidityFaultFlag = 0;
    }
  }
}

void faultAlert()
{
  if (globalFaultFlag == 1 && (currentTime > time_ms500 + 500)) // blink blue and red every 0.5 sec
  {
    time_ms500 = currentTime;
    toggleFlag = !toggleFlag;
    digitalWrite(greenLed, 0);
    digitalWrite(blueLed, toggleFlag);
    digitalWrite(redLed, toggleFlag);
  }
  else if (globalFaultFlag == 0 && tempFaultflag) // if Temprature is out of range light blue
  {
    digitalWrite(redLed, 0);
    digitalWrite(greenLed, 0);
    digitalWrite(blueLed, 1);
  }

  else if (globalFaultFlag == 0 && humidityFaultFlag) // if humidty is out of range light red
  {
    digitalWrite(blueLed, 0);
    digitalWrite(greenLed, 0);
    digitalWrite(redLed, 1);
  }
  else if ((globalFaultFlag | tempFaultflag | humidityFaultFlag) == 0) // if there is nothing out of range light green
  {
    time_ms500 = currentTime;
    digitalWrite(greenLed, 1);
    digitalWrite(blueLed, 0);
    digitalWrite(redLed, 0);
  }
}

