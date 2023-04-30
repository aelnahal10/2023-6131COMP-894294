#ifndef SENSOR_H
#define SENSOR_H

#include <DHTesp.h>

#define dht_pin 17

void setupDHT(int dht_pin);
float getTemperature();
float getHumidity();

#endif
