#include <WiFi.h>
#include <HTTPClient.h>

// Replace with your network credentials
const char* ssid = "MO's";
const char* password = "Matrix1.2.3";

// Replace with the IP address of your ESP server
const char* server_ip = "192.168.112.51";

void setup() {
  // Connect to Wi-Fi network
  Serial.begin(115200);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");

  // Send an HTTP POST request with some data to the ESP server
  HTTPClient http;
  http.begin("http://" + String(server_ip) + "/data.php");
  http.addHeader("Content-Type", "application/x-www-form-urlencoded");
  String data = "name=John&email=john@example.com&message=Hello, world!";
  int http_code = http.POST(data);
  if (http_code == HTTP_CODE_OK) {
    Serial.println("Data sent successfully");
  } else {
    Serial.println("Data send failed");
  }
  http.end();
}

void loop() {
  // Display the data received from the ESP server in an HTML/PHP page
  Serial.println("Requesting data from server...");
  WiFiClient client;
  if (client.connect(server_ip, 80)) {
    client.print("GET /data.php HTTP/1.1\r\n");
    client.print("Host: " + String(server_ip) + "\r\n");
    client.print("Connection: close\r\n");
    client.println();
  } else {
    Serial.println("Connection failed");
    return;
  }

  while (client.connected()) {
    String line = client.readStringUntil('\n');
    if (line == "\r") {
      Serial.println("Headers received");
      break;
    }
  }

  while (client.available()) {
    String line = client.readStringUntil('\n');
    Serial.println(line);
  }

  Serial.println("Data received successfully");
  client.stop();

  delay(1000);
}