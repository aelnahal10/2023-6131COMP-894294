#include "transmission.h"



void setupWiFi(const char *ssid, const char *password) {
  WiFi.begin(ssid, password);
  Serial.println("Connecting to WiFi...");

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED)
  {
    delay(1000);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
}

void transmitData(float temperature, float humidity) {
  DynamicJsonDocument json(128);
    json["temperature"] = temperature;
    json["humidity"] = humidity;
    String json_string;
    serializeJson(json, json_string);

    // Connect to the server and send the readings
    WiFiClient client;
    if (client.connect(server_ip, server_port))
    {
      Serial.println("Connected to server");

      client.print("POST /readings HTTP/1.1\r\n");
      client.print("Host: ");
      client.print(server_ip);
      client.print("\r\n");
      client.print("Content-Type: application/json\r\n");
      client.print("Content-Length: ");
      client.print(json_string.length());
      client.print("\r\n");
      client.print("Connection: close\r\n");
      client.print("\r\n");
      client.print(json_string);

      // for debuging
      while (client.connected() && !client.available())
      {
        Serial.println("waiting for server");
        delay(500);
      }
      while (client.available())
      {
        String line = client.readStringUntil('\r');
        Serial.println(line);
      }
      client.stop();
    }
    else
    {
      Serial.println("Failed to connect to server");
    }
  
}
