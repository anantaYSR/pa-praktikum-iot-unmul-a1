#ifdef ESP32
  #include <WiFi.h>
#else
  #include <ESP8266WiFi.h>
#endif
#include <WiFiClientSecure.h>
#include <UniversalTelegramBot.h>   // Universal Telegram Bot Library written by Brian Lough: https://github.com/witnessmenow/Universal-Arduino-Telegram-Bot
#include <ArduinoJson.h>
#include <DHT.h>
#include <LiquidCrystal_I2C.h>
#include <PubSubClient.h>

// Replace with your network credentials
const char* ssid = "Bayu";
const char* password = "bayu1234";

const char* mqtt_server = "public.cloud.shiftr.io";
const char* mqtt_username = "ananta-2109106024";
const char* mqtt_password = "dhu9iayH2ZvPSlNi";

// Initialize Telegram BOT
#define BOTtoken "5049346753:AAFpcl158jbt5YBv_dD7vJUfzafPuflWc7k"  // your Bot Token (Get from Botfather)

#define MQ_PIN A0
#define DHTPIN D4
#define DHTTYPE DHT11
// Use @myidbot to find out the chat ID of an individual or a group
// Also note that you need to click "start" on a bot before it can
// message you
#define CHAT_ID "-4195546758"

#ifdef ESP8266
  X509List cert(TELEGRAM_CERTIFICATE_ROOT);
#endif

WiFiClientSecure client;
WiFiClient espClient;
PubSubClient pubClient(espClient);
UniversalTelegramBot bot(BOTtoken, client);
LiquidCrystal_I2C lcd(0x27, 16, 2);

// Checks for new messages every 1 second.
int botRequestDelay = 1000;
unsigned long lastTimeBotRan;
DHT dht(DHTPIN, DHTTYPE);

float baseline_ppm = 0;
void readSensor() {
  unsigned int sensorValue = analogRead(MQ_PIN);  // Read the analog value from sensor
  float voltage = sensorValue * (3.3 / 1023.0);
  float ppm = (voltage - 0.22) / 0.1;
  float t = dht.readTemperature();

  // GAS
  if(baseline_ppm == 0) {
    baseline_ppm = ppm;
  } else {
    if(ppm > 12.2) {
      pubClient.publish("/alert", "on");
      bot.sendMessage(CHAT_ID, "BOCOR GASNYA LUR!!!!!!!!!");
    } else {
      pubClient.publish("/alert", "off");
    }
  }
  baseline_ppm = ppm;

  // SUHU
  if(t >= 30) {
    pubClient.publish("/alert", "on");
    bot.sendMessage(CHAT_ID, "SUHU DEVICE TERLALU TINGGI, SEGERA CEK!!!!");
  } else {
    pubClient.publish("/alert", "off");
  }
  lcd.clear();
  lcd.setCursor(2, 0); // Set the cursor on the third column and first row.
  lcd.print("PPM GAS: " + String(ppm)); // Print the string "Hello World!"
  lcd.setCursor(2, 1); //Set the cursor on the third column and the second row (counting starts at 0!).
  lcd.print("SUHU: " + String(t) + "°C");
}

// Handle what happens when you receive new messages
void handleNewMessages(int numNewMessages) {
 Serial.println("handleNewMessages");
  Serial.println(String(numNewMessages));
  for (int i=0; i<numNewMessages; i++) {
    // Chat id of the requester
    String chat_id = String(bot.messages[i].chat_id);
    if (chat_id != CHAT_ID){
      bot.sendMessage(chat_id, "Unauthorized user", "");
      continue;
    }
    
    // Print the received message
    String text = bot.messages[i].text;
    Serial.println(text);

    String from_name = bot.messages[i].from_name;

    if (text == "/start") {
      String welcome = "Selamat datang abangku, " + from_name + ".\n";
      welcome += "Gunakan perintah dibawah ini untuk mengendalikan alat\n\n";
      welcome += "/off untuk mematikan relay \n";
      welcome += "/dht untuk meminta status DHT (Suhu dan Kelembapan)";
      bot.sendMessage(chat_id, welcome, "");
    }
    
    if (text == "/off") {
      bot.sendMessage(chat_id, "Relay sudah mati", "");
      pubClient.publish("/alert", "off");
    }

    if (text == "/dht") {
      float h = dht.readHumidity();
      float t = dht.readTemperature();
      String message = "";
      message += "Suhu : "+ String(t) + "°C\n";
      message += "Kelembapan : "+ String(h) + " %";
      bot.sendMessage(chat_id, message, "");
    }
  }
}

void messageReceived(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived in topic: ");
  Serial.println(topic);
}

void setup() {
  Serial.begin(115200);

  // LCD
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);


  dht.begin();
  // Connect to Wi-Fi
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi..");
    lcd.print("Connecting to WiFi.."); 
  }

  // MQTT CLIENT
  pubClient.setServer(mqtt_server, 1883);
  pubClient.setCallback(messageReceived);

  lcd.clear();
  lcd.print("Connecting to MQTT...");
  Serial.print("Connecting to MQTT...");
  while (!pubClient.connect("ESP8266Client", mqtt_username, mqtt_password)) {
    Serial.print(".");
    delay(1000);
  }
  lcd.print("Connected to MQTT...");
  Serial.println("\nConnected to MQTT");
  pubClient.subscribe("/alert");

  #ifdef ESP8266
    configTime(0, 0, "pool.ntp.org");      // get UTC time via NTP
    client.setTrustAnchors(&cert); // Add root certificate for api.telegram.org
  #endif

  // ALL CONNECTED
  Serial.println(WiFi.localIP());
  lcd.clear();
  lcd.print(WiFi.localIP()); 
  bot.sendMessage(CHAT_ID, "Device tersambung!");
}

void loop() {
  readSensor();
  if (millis() > lastTimeBotRan + botRequestDelay)  {
    int numNewMessages = bot.getUpdates(bot.last_message_received + 1);
    while(numNewMessages) {
      Serial.println("got response");
      handleNewMessages(numNewMessages);
      numNewMessages = bot.getUpdates(bot.last_message_received + 1);
    }
    lastTimeBotRan = millis();
  }
}
