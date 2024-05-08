#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <DHT.h>

// Cấu hình WiFi
const char* ssid = "Ngu";
const char* password = "123123123";

// Cấu hình MQTT
const char* mqtt_server = "demo.thingsboard.io";
const char* token = "z7DXgntHnqAwxT1spfJk";

// Cấu hình DHT11
#define DHTPIN D4     // Chân kết nối với DHT11
#define DHTTYPE DHT11 // Sử dụng cảm biến DHT11
DHT dht(DHTPIN, DHTTYPE);

// Cấu hình cảm biến lửa và còi hú
const int flamePin = D5; // Chân kết nối với cảm biến lửa
const int buzzerPin = D6; // Chân kết nối với còi hú

WiFiClient espClient;
PubSubClient client(espClient);
void setup() {
  Serial.begin(115200);
  setup_wifi();
  client.setServer(mqtt_server, 1883);
  dht.begin();

  pinMode(flamePin, INPUT);
  pinMode(buzzerPin, OUTPUT);
}

void setup_wifi() {
  delay(10);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("WiFi connected");
}

void reconnect() {
  while (!client.connected()) {
    if (client.connect("ESP8266Client", token, NULL)) {
    } else {
      delay(5000);
    }
  }
}
void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  float humidity = dht.readHumidity();
  float temperature = dht.readTemperature();

  if (isnan(humidity) || isnan(temperature)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }

  int flameDetected = digitalRead(flamePin);
  if (flameDetected == LOW) { // LOW khi phát hiện lửa
    digitalWrite(buzzerPin, HIGH); // Kích hoạt còi hú
  } else {
    digitalWrite(buzzerPin, LOW); // Tắt còi hú
  }

  String payload = "{\"temperature\":" + String(temperature) + ",\"humidity\":" + String(humidity) + ",\"flameDetected\":" + String(flameDetected) + "}";
  client.publish("v1/devices/me/telemetry", (char*) payload.c_str());

  delay(2000); // Đợi 2 giây trước khi đọc và gửi dữ liệu tiếp theo
}
