#include "sensor.h"

DHTesp dht;

void setupDHT(int dht_pin) {
  dht.setup(dht_pin, DHTesp::DHT11);
}

float getTemperature() {
  return dht.getTemperature();
}

float getHumidity() {
  return dht.getHumidity();
}
