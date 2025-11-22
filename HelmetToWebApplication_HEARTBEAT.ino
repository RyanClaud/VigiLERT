#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include <TinyGPSPlus.h>
#include <HardwareSerial.h>
#include <TimeLib.h>

// ✅ FIX: Use same WiFi network as motorcycle to eliminate latency differences
const char* ssid = "Kupal";
const char* password = "DEEABE7H406";

// Firebase Config
const String firebaseHost = "https://vigilance-shield-default-rtdb.firebaseio.com";
const String userUID = "MnzBjTBslZNijOkq732PE91hHa23";
const String firebaseAuth = "";

// Sensor & Output Pins
const int mq3AnalogPin = 34;
const int ledPin = 2;
const int buzzerPin = 5;

// Calibration Threshold
const int alcoholThreshold = 2000;

// Beep Settings
unsigned long previousBeepMillis = 0;
const long beepInterval = 20;
const int beepDuration = 1500;
const int beepFrequency = 1200;

// ✅ REAL-TIME: Faster heartbeat for real-time monitoring
unsigned long lastHeartbeat = 0;
const long heartbeatInterval = 2000; // Send heartbeat every 2 seconds for real-time updates

// GPS Setup
TinyGPSPlus gps;
HardwareSerial gpsSerial(1);

void setup() {
  pinMode(mq3AnalogPin, INPUT);
  pinMode(ledPin, OUTPUT);
  pinMode(buzzerPin, OUTPUT);
  digitalWrite(ledPin, LOW);
  noTone(buzzerPin);

  Serial.begin(115200);
  connectToWiFi();
  delay(1000);

  gpsSerial.begin(9600, SERIAL_8N1, 16, 17);
  
  // Send initial heartbeat
  sendHelmetHeartbeat(true);
  lastHeartbeat = millis();
}

void loop() {
  if (WiFi.status() != WL_CONNECTED) connectToWiFi();

  while (gpsSerial.available()) gps.encode(gpsSerial.read());

  // Read alcohol sensor
  int alcoholValue = analogRead(mq3AnalogPin);
  bool alcoholDetected = alcoholValue > alcoholThreshold;

  // ✅ Enhanced Serial Monitor with Threshold
  Serial.printf("\n[ALCOHOL] Sensor Value: %d | Threshold: %d | Status: %s\n", 
                alcoholValue, alcoholThreshold, alcoholDetected ? "DANGER ⚠️" : "SAFE ✓");
  
  if (alcoholDetected) {
    Serial.printf("   ⚠️ ALCOHOL DETECTED! Value %d exceeds threshold %d\n", alcoholValue, alcoholThreshold);
  }

  // Update LED and Buzzer
  digitalWrite(ledPin, alcoholDetected ? HIGH : LOW);
  if (alcoholDetected) playBeepAlert();
  else noTone(buzzerPin);

  // Send data to Firebase
  sendAlcoholToFirebase(alcoholValue, alcoholDetected);
  sendHelmetStatusToFirebase(true);

  // ✅ REAL-TIME: Send heartbeat every 2 seconds for faster status updates
  if (millis() - lastHeartbeat >= heartbeatInterval) {
    sendHelmetHeartbeat(true);
    lastHeartbeat = millis();
  }

  // ✅ REAL-TIME: Faster updates for real-time monitoring (1 second instead of 5)
  delay(1000);
}

void connectToWiFi() {
  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi...");
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }
  Serial.println("\n✅ Connected to WiFi");
}

void sendHelmetHeartbeat(bool isActive) {
  // Send heartbeat to /devices/helmet with Unix timestamp
  StaticJsonDocument<128> doc;
  doc["status"] = isActive ? "On" : "Off";
  
  // Use GPS time if available, otherwise use millis as fallback
  unsigned long timestamp;
  if (gps.time.isValid() && gps.date.isValid()) {
    tmElements_t tm;
    tm.Year = gps.date.year() - 1970;
    tm.Month = gps.date.month();
    tm.Day = gps.date.day();
    tm.Hour = gps.time.hour();
    tm.Minute = gps.time.minute();
    tm.Second = gps.time.second();
    timestamp = makeTime(tm) * 1000UL; // Convert to milliseconds
  } else {
    // Fallback: Use epoch + millis (approximate)
    timestamp = 1700000000000UL + millis(); // Rough Unix timestamp
  }
  
  doc["lastHeartbeat"] = timestamp;
  doc["timestamp"] = timestamp;
  
  String payload;
  serializeJson(doc, payload);
  
  HTTPClient http;
  String url = firebaseHost + "/helmet_public/" + userUID + "/devices/helmet.json?auth=" + firebaseAuth;
  http.begin(url);
  http.addHeader("Content-Type", "application/json");
  int code = http.PUT(payload);
  
  if (code == HTTP_CODE_OK) {
    Serial.printf("[FIREBASE] Helmet heartbeat sent: %s at %lu\n", isActive ? "On" : "Off", timestamp);
  } else {
    Serial.printf("[FIREBASE] Helmet heartbeat failed: %d\n", code);
  }
  http.end();
}

void sendAlcoholToFirebase(int alcoholVal, bool alcoholStatus) {
  StaticJsonDocument<128> doc;
  doc["sensorValue"] = alcoholVal;
  doc["status"] = alcoholStatus ? "Danger" : "Safe";
  doc["timestamp"] = millis();

  String payload;
  serializeJson(doc, payload);

  HTTPClient http;
  String url = firebaseHost + "/helmet_public/" + userUID + "/alcohol/status.json?auth=" + firebaseAuth;
  http.begin(url);
  http.addHeader("Content-Type", "application/json");
  int code = http.PUT(payload);
  
  if (code == HTTP_CODE_OK) {
    Serial.println("[FIREBASE] Alcohol status updated");
  } else {
    Serial.printf("[FIREBASE] Failed to update alcohol status: %d\n", code);
  }
  http.end();
}

void sendHelmetStatusToFirebase(bool isActive) {
  String status = isActive ? "ON" : "OFF";
  unsigned long now = millis();

  StaticJsonDocument<64> doc;
  doc["status"] = status;
  doc["lastActive"] = now;

  String payload;
  serializeJson(doc, payload);

  HTTPClient http;
  String url = firebaseHost + "/helmet_public/" + userUID + "/helmetStatus/status.json?auth=" + firebaseAuth;
  http.begin(url);
  http.addHeader("Content-Type", "application/json");
  int code = http.PUT(payload);
  
  if (code == HTTP_CODE_OK) {
    Serial.printf("[FIREBASE] Helmet status set to: %s\n", status.c_str());
  } else {
    Serial.printf("[FIREBASE] Failed to update helmet status: %d\n", code);
  }
  http.end();
}

void playBeepAlert() {
  unsigned long currentMillis = millis();
  if (currentMillis - previousBeepMillis >= beepInterval) {
    previousBeepMillis = currentMillis;
    tone(buzzerPin, beepFrequency, beepDuration);
  }
}
