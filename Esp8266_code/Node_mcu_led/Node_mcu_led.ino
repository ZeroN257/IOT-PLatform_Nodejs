#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <Arduino.h> 

const char* ssid = "Ngu";
const char* password = "123123123";

const char* mqtt_server = "192.168.83.246";
const int mqtt_port = 1883;

// Pins for LEDs
const int LED1_PIN = D1;
const int LED2_PIN = D2;
const int LED3_PIN = D3;
const int LED4_PIN = D4;

WiFiClient espClient;
PubSubClient client(espClient);

void setup_wifi() {
  delay(10);
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");

  String message;
  for (int i = 0; i < length; i++) {
    message += (char)payload[i];
  }
  Serial.println(message);

  // Control 4 LEDs
  if (strcmp(topic, "nguyen2572004/led/led1") == 0) {
    digitalWrite(LED1_PIN, message == "0" ? LOW : HIGH);
  } else if (strcmp(topic, "nguyen2572004/led/led2") == 0) {
    digitalWrite(LED2_PIN, message == "0" ? LOW : HIGH);
  } else if (strcmp(topic, "nguyen2572004/led/led3") == 0) {
    digitalWrite(LED3_PIN, message == "0" ? LOW : HIGH);
  } else if (strcmp(topic, "nguyen2572004/led/led4") == 0) {
    digitalWrite(LED4_PIN, message == "0" ? LOW : HIGH);
  }
}

void reconnect() {
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    if (client.connect("ESP8266Client")) {
      Serial.println("connected");
      // Subscribe to LED topics
      client.subscribe("nguyen2572004/led/led1");
      client.subscribe("nguyen2572004/led/led2");
      client.subscribe("nguyen2572004/led/led3");
      client.subscribe("nguyen2572004/led/led4");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      delay(5000);
    }
  }
}

void setup() {
  // Initialize LED pins as outputs
  pinMode(LED1_PIN, OUTPUT);
  pinMode(LED2_PIN, OUTPUT);
  pinMode(LED3_PIN, OUTPUT);
  pinMode(LED4_PIN, OUTPUT);

  Serial.begin(115200);
  setup_wifi();
  client.setServer(mqtt_server, mqtt_port);
  client.setCallback(callback);
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();
}
