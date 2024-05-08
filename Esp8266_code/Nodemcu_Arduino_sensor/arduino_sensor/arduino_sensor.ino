#include <ArduinoJson.h>  // Thêm thư viện ArduinoJson

#define NUM_SENSORS 10
int sensorPins[NUM_SENSORS] = {2, 3, 4, 5, 6, 7, 8, 9, 10, 11};

void setup() {
  Serial.begin(9600);
  for (int i = 0; i < NUM_SENSORS; i++) {
    pinMode(sensorPins[i], INPUT);
  }
}

void loop() {
  StaticJsonDocument<256> doc; // Tạo một JSON document với kích thước đủ dùng

  // Đọc giá trị từ cảm biến và thêm vào document
  for (int i = 0; i < NUM_SENSORS; i++) {
    // Tạo key dưới dạng sensor1, sensor2, ...
    String key = "sensor" + String(i + 1);
    doc[key] = digitalRead(sensorPins[i]);
  }

  // Serialize JSON và gửi qua Serial
  serializeJson(doc, Serial);
  Serial.println(); // Kết thúc chuỗi với một dòng mới để Raspberry Pi có thể đọc từng dòng

  delay(1000); // Đợi một giây trước khi gửi lần tiếp theo
}
