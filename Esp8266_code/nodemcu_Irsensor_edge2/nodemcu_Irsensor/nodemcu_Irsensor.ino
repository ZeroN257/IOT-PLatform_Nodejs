#include <ESP8266WiFi.h>
#include <PubSubClient.h>

// Cấu hình WiFi
const char* ssid = "Ngu";
const char* password = "123123123";

// Cấu hình thông tin server MQTT (ThingsBoard)
const char* mqtt_server = "demo.thingsboard.io";
const char* token = "KWJ4VmfgqhuhRiKeKRC0";
const char* topic = "v1/devices/me/telemetry";

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
    if (client.connect("ESP8266Client", token, NULL)) {
    } else {
      delay(5000);
    }
  }
}

void setup() {
  Serial.begin(115200);
  setup_wifi();
  client.setServer(mqtt_server, 1883);

  // Khởi tạo các chân D0-D4 như là input
  pinMode(D0, INPUT);
  pinMode(D1, INPUT);
  pinMode(D2, INPUT);
  pinMode(D3, INPUT);
  pinMode(D4, INPUT);
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  // Đọc trạng thái từ 5 IR sensors
  int sensorValue0 = digitalRead(D0);
  int sensorValue1 = digitalRead(D1);
  int sensorValue2 = digitalRead(D2);
  int sensorValue3 = digitalRead(D3);
  int sensorValue4 = digitalRead(D4);

 // Tạo một chuỗi để in giá trị của tất cả các sensor
  String sensorStates = "Sensor states: " + String(sensorValue0) + ", " + 
                        String(sensorValue1) + ", " + 
                        String(sensorValue2) + ", " + 
                        String(sensorValue3) + ", " + 
                        String(sensorValue4);

  // In chuỗi ra Serial
  Serial.println(sensorStates);
  // Tạo chuỗi JSON để gửi
  String payload = "{\"sensor6\":" + String(sensorValue0) + ",";
  payload += "\"sensor7\":" + String(sensorValue1) + ",";
  payload += "\"sensor8\":" + String(sensorValue2) + ",";
  payload += "\"sensor9\":" + String(sensorValue3) + ",";
  payload += "\"sensor10\":" + String(sensorValue4) + "}";

// In dữ liệu ra Serial trước khi gửi
  Serial.println(payload);


  // Gửi dữ liệu
  client.publish(topic, (char*) payload.c_str());

  delay(1000); // Đợi 1 giây trước khi đọc và gửi dữ liệu tiếp theo
}
