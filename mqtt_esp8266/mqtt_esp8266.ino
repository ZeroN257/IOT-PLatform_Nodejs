#include <ESP8266WiFi.h>
#include <DHT.h> // Include the DHT sensor library
#include <PubSubClient.h>
#include <Arduino.h> 

const char* ssid = "Ngu";
const char* password = "123123123";

const char* mqtt_server = "192.168.87.246";
const int mqtt_port = 1883;

const int DHT_PIN = D3; // Pin where your DHT11 is connected
const int DHT_TYPE = DHT11; // Type of the DHT sensor

long lastMsg = 0;
char resistanceStr[10];

WiFiClient espClient;
PubSubClient client(espClient);
DHT dht(DHT_PIN, DHT_TYPE);

void setup_wifi() {
  delay(10);
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  randomSeed(analogRead(0)); 

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

  if (strcmp(topic, "nguyen2572004/led/led1") == 0) {
    if (message == "0") {
      digitalWrite(D1, LOW);
    } else if (message == "1") {
      digitalWrite(D1, HIGH);
    }
  } else if (strcmp(topic, "nguyen2572004/led/led3") == 0) {
    if (message == "0") {
      digitalWrite(D2, LOW);
    } else if (message == "1") {
      digitalWrite(D2, HIGH);
    }
  }
}

void reconnect() {
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    if (client.connect("ESP8266Client")) {
      Serial.println("connected");
      client.subscribe("nguyen2572004/led/led1");
      client.subscribe("nguyen2572004/led/led3");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      delay(5000);
    }
  }
}

void setup() {
  pinMode(D1, OUTPUT);
  pinMode(D2, OUTPUT);
  dht.begin(); // Initialize DHT sensor

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

  float humidity = dht.readHumidity();
  float temperature = dht.readTemperature();

  if (!isnan(humidity) && !isnan(temperature)) {
    char temperatureStr[10];
    char humidityStr[10];
    dtostrf(temperature, 3, 1, temperatureStr);
    dtostrf(humidity, 3, 1, humidityStr);

    client.publish("nguyen2572004/dht11/livingroom/temperature", temperatureStr);
    client.publish("nguyen2572004/dht11/livingroom/humidity", humidityStr);
    client.publish("nguyen2572004/dht11/bedroom/temperature", temperatureStr);
    client.publish("nguyen2572004/dht11/bedroom/humidity", humidityStr);

    delay(2000);
  }
  long now = millis();
  if (now - lastMsg > 2000) {
    lastMsg = now;

    // Đọc giá trị resistance từ chân A0
    int resistance = analogRead(A0); // Đọc giá trị từ chân A0
    dtostrf(resistance, 6, 2, resistanceStr); // Chuyển đổi int sang chuỗi

    // Gửi giá trị resistance đến các topic MQTT
    client.publish("nguyen123/potentiometer/bientro1/resistance", resistanceStr);
    client.publish("nguyen2572004/potentiometer/bientro1/resistance", resistanceStr);
    client.publish("nguyen2572004/potentiometer/bientro2/resistance", resistanceStr);
    Serial.println(resistanceStr);
    
  }
}
