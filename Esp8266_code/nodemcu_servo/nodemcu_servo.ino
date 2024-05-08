#include <ESP8266WiFi.h>
#include <Servo.h>
#include <PubSubClient.h>

// WiFi credentials
const char* ssid = "Ngu";
const char* password = "123123123";

// MQTT Server (ThingsBoard) details
const char* mqtt_server = "demo.thingsboard.io";
const char* token = "jm09Cs2kO84qOKJHga9B";

// Initialize the WiFi and MQTT Client objects
WiFiClient espClient;
PubSubClient client(espClient);

Servo myservo;  // Servo object for controlling the servo

// Pins connected to the HC-SR04 sensor
const int trigPin = D2;
const int echoPin = D3;

void setup_wifi() {
  delay(10);
  // Connect to WiFi network
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
}

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    if (client.connect("ESP8266Client", token, NULL)) {
      Serial.println("connected");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

void setup() {
  Serial.begin(115200);
  setup_wifi(); // Connect to Wi-Fi
  client.setServer(mqtt_server, 1883); // MQTT server setup

  myservo.attach(D1); // Attach servo to D1 pin of NodeMCU
  pinMode(trigPin, OUTPUT); // Set trigPin as an Output
  pinMode(echoPin, INPUT);  // Set echoPin as an Input
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  long duration, distance;
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH);
  distance = duration * 0.034 / 2;

  Serial.print("Distance: ");
  Serial.print(distance);
  Serial.println(" cm");

  // Determine door status based on servo position
  String doorStatus = (myservo.read() == 180) ? "Close" : "Open";

  // Determine object detection status based on distance
  String objectDetect = (distance < 20) ? "Yes" : "No";

  // Move servo based on distance
  if (distance < 20) {
    myservo.write(0); // Rotate servo to 0 degrees
  } else {
    myservo.write(180); // Rotate servo to 180 degrees
  }

  // Prepare JSON payload string with additional key-value pairs
  String payload = "{\"distance\":" + String(distance) + 
                   ",\"servo\":" + String(myservo.read()) +
                   ",\"doorStatus\":\"" + doorStatus + 
                   "\",\"objectDetect\":\"" + objectDetect + "\"}";
                   
  client.publish("v1/devices/me/telemetry", (char*) payload.c_str());

  Serial.println(payload); // Print payload to Serial Monitor for debugging

  delay(1000); // Wait for a second before measuring again
}

