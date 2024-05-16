#ifdef ESP32
  #include <WiFi.h>
#else
  #include <ESP8266WiFi.h>
#endif
#include <WiFiClientSecure.h>
#include <UniversalTelegramBot.h> 
#include <ArduinoJson.h>
#include <PubSubClient.h>

const char* ssid = "Bayu";
const char* password = "bayu1234";

const char* mqtt_server = "public.cloud.shiftr.io";
const char* mqtt_username = "ananta-2109106024";
const char* mqtt_password = "dhu9iayH2ZvPSlNi";

WiFiClient espClient;
PubSubClient client(espClient);

#define RELAY_PIN D6       // Pin yang terhubung ke relay


unsigned long lastMillis = 0;
const long interval = 1000;

bool sensorActive = true; // Status awal sensor aktif
bool relayState = false; // Status awal relay mati

void connect() {
  Serial.print("Connecting to Wi-Fi");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(1000);
  }
  Serial.println("\nConnected to Wi-Fi");

  Serial.print("Connecting to MQTT...");
  while (!client.connect("ESP8266Client", mqtt_username, mqtt_password)) {
    Serial.print(".");
    delay(1000);
  }
  Serial.println("\nConnected to MQTT");

  client.subscribe("/alert");
}

void messageReceived(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived in topic: ");
  Serial.println(topic);

  // Convert byte array to string
  String payloadStr = "";
  for (int i = 0; i < length; i++) {
    payloadStr += (char)payload[i];
  }

  if (strcmp(topic, "/alert") == 0) {
    if (payloadStr == "on") {
      digitalWrite(RELAY_PIN, LOW); // Turn on the relay
      relayState = true;
      Serial.println("Sensor suhu diaktifkan");
      sensorActive = true;
    } else if (payloadStr == "off") {
      digitalWrite(RELAY_PIN, HIGH); // Turn off the relay
      relayState = false;
      Serial.println("Sensor suhu dinonaktifkan");
      sensorActive = false;
    }
  }
}

void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid, password);

  client.setServer(mqtt_server, 1883);
  client.setCallback(messageReceived);

  pinMode(RELAY_PIN, OUTPUT);
  digitalWrite(RELAY_PIN, HIGH); // Pastikan relay mati saat boot

  connect();
}

void loop() {
  client.loop();
  if (!client.connected()) {
    connect();
  }
  if(relayState) {
    digitalWrite(RELAY_PIN, LOW);
  }
}
