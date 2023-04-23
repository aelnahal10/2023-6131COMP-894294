#ifndef SENSOR_H
#define SENSOR_H

#include <DHTesp.h>


void setupDHT(int dht_pin);
float getTemperature();
float getHumidity();

#endif
