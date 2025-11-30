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
const int alcoholThreshold = 600;

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
  Serial.println("║   HELMET FIXED VERSION                 ║");
  Serial.println("╠════════════════════════════════════════╣");
  Serial.println("║ ✅ Multi-path alcohol data sync        ║");
  Serial.println("║ ✅ Dashboard compatibility fixed       ║");
  Serial.println("╚════════════════════════════════════════╝\n");
  
  connectToWiFi();
  
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
                alcoholValue, alcoholThreshold, 
                alcoholDetected ? "DANGER ⚠️" : "SAFE ✓");
  
  // Update LED and Buzzer
  digitalWrite(ledPin, alcoholDetected ? HIGH : LOW);
  
  if (alcoholDetected) {
    playBeepAlert();
  } else {
    noTone(buzzerPin);
  }
  
  // ✅ FIX: Send to multiple paths for dashboard compatibility
  sendAlcoholToFirebase(alcoholValue, alcoholDetected);
  
  // Send heartbeat every 2 seconds
  if (millis() - lastHeartbeat >= heartbeatInterval) {
    sendHelmetHeartbeat(true);
    lastHeartbeat = millis();
  }
  
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
  Serial.println("\n[HEARTBEAT] ═══════════════════════════════");
  
  // ✅ Use 64-bit integer to prevent overflow
  uint64_t timestamp = 1700000000000ULL + (uint64_t)millis();
  
  Serial.printf("[HEARTBEAT] Timestamp: %llu\n", timestamp);
  
  StaticJsonDocument<128> doc;
  doc["status"] = isActive ? "On" : "Off";
  doc["lastHeartbeat"] = timestamp;
  doc["timestamp"] = timestamp;
  
  String payload;
  serializeJson(doc, payload);
  
  Serial.printf("[HEARTBEAT] JSON: %s\n", payload.c_str());
  
  HTTPClient http;
  String url = firebaseHost + "/helmet_public/" + userUID + "/devices/helmet.json?auth=" + firebaseAuth;
  http.begin(url);
  http.addHeader("Content-Type", "application/json");
  
  int code = http.PUT(payload);
  
  if (code == HTTP_CODE_OK) {
    Serial.printf("[HEARTBEAT] ✓ SUCCESS! HTTP: %d\n", code);
  } else {
    Serial.printf("[HEARTBEAT] ✗ FAILED! HTTP: %d\n", code);
  }
  
  Serial.println("[HEARTBEAT] ═══════════════════════════════\n");
  
  http.end();
}

void sendAlcoholToFirebase(int alcoholVal, bool alcoholStatus) {
  Serial.println("\n[ALCOHOL] ═══════════════════════════════");
  
  uint64_t timestamp = 1700000000000ULL + (uint64_t)millis();
  String statusText = alcoholStatus ? "Danger" : "Safe";
  
  Serial.printf("[ALCOHOL] Sending to Firebase...\n");
  Serial.printf("[ALCOHOL] Value: %d | Status: %s\n", alcoholVal, statusText.c_str());
  
  // ✅ FIX: Send to MULTIPLE paths for dashboard compatibility
  
  // Path 1: /helmet_public/{userUID}/alcohol/status (main path)
  StaticJsonDocument<128> doc1;
  doc1["sensorValue"] = alcoholVal;
  doc1["status"] = statusText;
  doc1["timestamp"] = timestamp;
  
  String payload1;
  serializeJson(doc1, payload1);
  
  HTTPClient http1;
  String url1 = firebaseHost + "/helmet_public/" + userUID + "/alcohol/status.json?auth=" + firebaseAuth;
  http1.begin(url1);
  http1.addHeader("Content-Type", "application/json");
  
  int code1 = http1.PUT(payload1);
  
  if (code1 == HTTP_CODE_OK) {
    Serial.printf("[ALCOHOL] ✓ Path 1 SUCCESS: /helmet_public/.../alcohol/status\n");
  } else {
    Serial.printf("[ALCOHOL] ✗ Path 1 FAILED: HTTP %d\n", code1);
  }
  
  http1.end();
  
  // Path 2: /{userUID}/alcohol/status/status (dashboard compatibility)
  HTTPClient http2;
  String url2 = firebaseHost + "/" + userUID + "/alcohol/status/status.json?auth=" + firebaseAuth;
  http2.begin(url2);
  http2.addHeader("Content-Type", "application/json");
  
  // ✅ Send just the status string for this path
  String payload2 = "\"" + statusText + "\"";
  
  int code2 = http2.PUT(payload2);
  
  if (code2 == HTTP_CODE_OK) {
    Serial.printf("[ALCOHOL] ✓ Path 2 SUCCESS: /.../alcohol/status/status\n");
  } else {
    Serial.printf("[ALCOHOL] ✗ Path 2 FAILED: HTTP %d\n", code2);
  }
  
  http2.end();
  
  // Path 3: /{userUID}/alcohol (root alcohol path)
  StaticJsonDocument<128> doc3;
  doc3["sensorValue"] = alcoholVal;
  doc3["status"] = statusText;
  doc3["timestamp"] = timestamp;
  
  String payload3;
  serializeJson(doc3, payload3);
  
  HTTPClient http3;
  String url3 = firebaseHost + "/" + userUID + "/alcohol.json?auth=" + firebaseAuth;
  http3.begin(url3);
  http3.addHeader("Content-Type", "application/json");
  
  int code3 = http3.PUT(payload3);
  
  if (code3 == HTTP_CODE_OK) {
    Serial.printf("[ALCOHOL] ✓ Path 3 SUCCESS: /.../alcohol\n");
  } else {
    Serial.printf("[ALCOHOL] ✗ Path 3 FAILED: HTTP %d\n", code3);
  }
  
  http3.end();
  
  Serial.println("[ALCOHOL] ═══════════════════════════════\n");
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
