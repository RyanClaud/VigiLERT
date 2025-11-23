#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>

// WiFi Config
const char* ssid = "DPWH";
const char* password = "12345678900";

// Firebase Config
const String firebaseHost = "https://vigilance-shield-default-rtdb.firebaseio.com";
const String userUID = "MnzBjTBslZNijOkq732PE91hHa23";
const String firebaseAuth = "";

// Sensor Pins
const int mq3AnalogPin = 34;
const int ledPin = 2;
const int buzzerPin = 5;

// Calibration Threshold
const int alcoholThreshold = 4500;

// Heartbeat
unsigned long lastHeartbeat = 0;
const long heartbeatInterval = 2000;

void setup() {
  pinMode(mq3AnalogPin, INPUT);
  pinMode(ledPin, OUTPUT);
  pinMode(buzzerPin, OUTPUT);
  digitalWrite(ledPin, LOW);
  noTone(buzzerPin);

  Serial.begin(115200);
  delay(1000);
  
  Serial.println("\n╔════════════════════════════════════════╗");
  Serial.println("║   HELMET DIAGNOSTIC VERSION            ║");
  Serial.println("╠════════════════════════════════════════╣");
  Serial.println("║ Testing 64-bit timestamp generation   ║");
  Serial.println("╚════════════════════════════════════════╝\n");
  
  connectToWiFi();
  
  // Test timestamp generation
  testTimestampGeneration();
  
  // Send initial heartbeat
  sendHelmetHeartbeat(true);
  lastHeartbeat = millis();
}

void loop() {
  if (WiFi.status() != WL_CONNECTED) connectToWiFi();

  // Read alcohol sensor
  int alcoholValue = analogRead(mq3AnalogPin);
  bool alcoholDetected = alcoholValue > alcoholThreshold;

  Serial.printf("\n[ALCOHOL] Sensor Value: %d | Threshold: %d | Status: %s\n", 
                alcoholValue, alcoholThreshold, alcoholDetected ? "DANGER ⚠️" : "SAFE ✓");

  // Update LED and Buzzer
  digitalWrite(ledPin, alcoholDetected ? HIGH : LOW);
  if (alcoholDetected) playBeepAlert();
  else noTone(buzzerPin);

  // Send data to Firebase
  sendAlcoholToFirebase(alcoholValue, alcoholDetected);

  // Send heartbeat every 2 seconds
  if (millis() - lastHeartbeat >= heartbeatInterval) {
    sendHelmetHeartbeat(true);
    lastHeartbeat = millis();
  }

  delay(1000);
}

void testTimestampGeneration() {
  Serial.println("\n[TIMESTAMP TEST] ═══════════════════════════════");
  
  // Test 32-bit (old way)
  unsigned long timestamp32 = 1700000000000UL + millis();
  Serial.printf("[TEST] 32-bit timestamp: %lu (digits: %d)\n", 
                timestamp32, String(timestamp32).length());
  
  // Test 64-bit (new way)
  uint64_t timestamp64 = 1700000000000ULL + (uint64_t)millis();
  Serial.printf("[TEST] 64-bit timestamp: %llu (digits: %d)\n", 
                timestamp64, String((unsigned long long)timestamp64).length());
  
  // Test what gets stored in JSON
  StaticJsonDocument<128> testDoc;
  testDoc["test32"] = timestamp32;
  testDoc["test64"] = timestamp64;
  
  String testPayload;
  serializeJson(testDoc, testPayload);
  Serial.printf("[TEST] JSON payload: %s\n", testPayload.c_str());
  
  Serial.println("[TIMESTAMP TEST] ═══════════════════════════════\n");
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
  Serial.println("\n[HEARTBEAT] ═══════════════════════════════");
  Serial.println("[HEARTBEAT] Generating timestamp...");
  
  // ✅ Use 64-bit integer to prevent overflow
  uint64_t timestamp = 1700000000000ULL + (uint64_t)millis();
  
  Serial.printf("[HEARTBEAT] millis(): %lu\n", millis());
  Serial.printf("[HEARTBEAT] Base: 1700000000000\n");
  Serial.printf("[HEARTBEAT] Calculated: %llu\n", timestamp);
  Serial.printf("[HEARTBEAT] Digits: %d\n", String((unsigned long long)timestamp).length());
  
  StaticJsonDocument<128> doc;
  doc["status"] = isActive ? "On" : "Off";
  doc["lastHeartbeat"] = timestamp;
  doc["timestamp"] = timestamp;
  
  String payload;
  serializeJson(doc, payload);
  
  Serial.printf("[HEARTBEAT] JSON payload: %s\n", payload.c_str());
  
  HTTPClient http;
  String url = firebaseHost + "/helmet_public/" + userUID + "/devices/helmet.json?auth=" + firebaseAuth;
  http.begin(url);
  http.addHeader("Content-Type", "application/json");
  int code = http.PUT(payload);
  
  if (code == HTTP_CODE_OK) {
    Serial.printf("[HEARTBEAT] ✓ SUCCESS! HTTP: %d\n", code);
    Serial.printf("[HEARTBEAT] Timestamp sent: %llu\n", timestamp);
    Serial.println("[HEARTBEAT] Motorcycle should detect this!");
  } else {
    Serial.printf("[HEARTBEAT] ✗ FAILED! HTTP: %d\n", code);
  }
  
  Serial.println("[HEARTBEAT] ═══════════════════════════════\n");
  http.end();
}

void sendAlcoholToFirebase(int alcoholVal, bool alcoholStatus) {
  StaticJsonDocument<128> doc;
  doc["sensorValue"] = alcoholVal;
  doc["status"] = alcoholStatus ? "Danger" : "Safe";
  uint64_t timestamp = 1700000000000ULL + (uint64_t)millis();
  doc["timestamp"] = timestamp;

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

void playBeepAlert() {
  static unsigned long previousBeepMillis = 0;
  const long beepInterval = 20;
  const int beepDuration = 1500;
  const int beepFrequency = 1200;
  
  unsigned long currentMillis = millis();
  if (currentMillis - previousBeepMillis >= beepInterval) {
    previousBeepMillis = currentMillis;
    tone(buzzerPin, beepFrequency, beepDuration);
  }
}