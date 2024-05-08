#include <ArduinoJson.h>
#include <ESP8266WiFi.h>
#include <PubSubClient.h>

// WiFi settings
const char* ssid = "Ngu";
const char* password = "123123123";

// MQTT Server settings
const char* mqtt_server = "demo.thingsboard.io";
const int mqtt_port = 1883; 
const char* mqtt_user = "PnTO1U7zafrVbwwqn6va"; // Use your ThingsBoard device token here
const char* mqtt_password = "";

WiFiClient espClient;
PubSubClient client(espClient);

void setup_wifi() {
  delay(10);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
  }
}

void reconnect() {
  while (!client.connected()) {
    if (client.connect("NodeMCUClient", mqtt_user, mqtt_password)) {
      // Connected
    } else {
      delay(5000);
    }
  }
}

void setup() {
  Serial.begin(9600);
  setup_wifi();
  client.setServer(mqtt_server, mqtt_port);
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  if (Serial.available()) {
    String content = Serial.readStringUntil('\n'); // Read until newline character

    StaticJsonDocument<512> doc;
    DeserializationError error = deserializeJson(doc, content);
    if (!error) {
      // Calculate the sum of all sensor values
      int total = 0;
      for (JsonPair kv : doc.as<JsonObject>()) { // Iterate over all key-value pairs in the document
        total += kv.value().as<int>(); // Add the value to total
      }

      // Add the total to the document
      doc["total"] = total;

      // Print the received JSON with the total to the Serial Monitor
      Serial.println("Received JSON data with total:");
      serializeJsonPretty(doc, Serial);
      Serial.println(); // Add a newline for better readability

      String payload;
      serializeJson(doc, payload);
      client.publish("v1/devices/me/telemetry", payload.c_str());
    } else {
      // If there was an error in deserialization, print it
      Serial.print("deserializeJson() failed: ");
      Serial.println(error.c_str());
    }
  }
}
