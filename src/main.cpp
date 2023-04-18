#include "main.h"

DHTesp dht;
int tempMin =-9999;
int tempMax =9999;
int humidityMin =-9999;
int humidityMax =9999;
unsigned long timee_ms5000 = 0;
unsigned long time_ms500 = 0;
unsigned long time_ms1000 = 0;
unsigned long currentTime = 0;
unsigned int globalFaultFlag = 0;
bool toggleFlag = 0;
bool tempFaultflag = 0;
bool humidityFaultFlag = 0;
int *constraints[4];

int *rotaryCount;
int rotaryClkPinLast = LOW;
int rotaryClkPinCurrent = LOW;
float humidity;
float temperature; 
void setup()
{
  Serial.begin(9600);
  pinMode(blueLed, OUTPUT);
  pinMode(redLed, OUTPUT);
  pinMode(rotaryClk, INPUT);
  pinMode(rotartyDT, INPUT);
  pinMode(rotarySW, INPUT_PULLUP);
  pinMode(constraintsPushButton, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(rotaryClk), rotaryCallBack, CHANGE);
  attachInterrupt(digitalPinToInterrupt(rotartyDT), rotaryCallBack, CHANGE);
  constraints[0] = &tempMin;
  constraints[1] = &tempMax;
  constraints[2] = &humidityMin;
  constraints[3] = &humidityMax;
  // use this instead: 
  dht.setup(dht_pin, DHTesp::DHT11);
  attachInterrupt(digitalPinToInterrupt(constraintsPushButton), pushButtonCallBack, RISING);
}

// the loop function runs over and over again forever
void loop()
{ // sensor sampling time can not be less than one sec accourding to data sheet;
  currentTime = millis();
  if (currentTime > time_ms1000 + 1500)
  {
    time_ms1000 = currentTime;
    humidity = dht.getHumidity();
    temperature = dht.getTemperature();
    faultCheck(temperature, humidity); // checks if there is any sesnor reading outside of range and sets fault flag
  }

  if (currentTime > timee_ms5000 + 5000)
  {
    Serial.print("T = ");
    Serial.println(temperature);
    Serial.print("RH = ");
    Serial.println(humidity);
    timee_ms5000 = currentTime;
  }

  faultAlert();
}

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
  bool tempStatus = tempCheck(temp);             // checks temparature if outside of range or not
  bool humidityStatus = humidityCheck(humidity); // checks humidity if outside of range or not
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
  if (globalFaultFlag==1 && (currentTime > time_ms500 + 500))
  {
    time_ms500 = currentTime;
    toggleFlag = !toggleFlag;
    digitalWrite(greenLed, 0);
    digitalWrite(blueLed,toggleFlag);
    digitalWrite(redLed,toggleFlag);
  }
  else if (globalFaultFlag==0 && tempFaultflag) // if Temprature is out of range light blue
  {
    digitalWrite(redLed, 0);
    digitalWrite(greenLed, 0);
    digitalWrite(blueLed, 1);

  }

  else if (globalFaultFlag==0 && humidityFaultFlag) // if humidty is out of range light red
  {
    digitalWrite(blueLed, 0);
    digitalWrite(greenLed, 0);
    digitalWrite(redLed, 1);

  }
  else if ((globalFaultFlag | tempFaultflag | humidityFaultFlag) == 0 ) // if there is nothing out of range light green
  {
    time_ms500 = currentTime;    
    digitalWrite(greenLed, 1);
    digitalWrite(blueLed, 0);
    digitalWrite(redLed, 0);
 
  }
}

void rotaryCallBack() {  
  rotaryClkPinCurrent = digitalRead(rotaryClk);
  if(rotaryClkPinCurrent != rotaryClkPinLast)
  {
    if(digitalRead(rotartyDT) != rotaryClkPinLast)
    {
      rotaryCount++;
      }
      else
      {
        rotaryCount--;
        }
  }
  
  if(digitalRead(rotarySW) != 1)
  {
    rotaryCount=0;
    }
    
    rotaryClkPinLast= rotaryClkPinCurrent;
}


void pushButtonCallBack()
{

Serial.print("tezak hamra");


}