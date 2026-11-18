#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include <time.h>

// ═══════════════════════════════════════════════════════════════════════════
// VigiLERT Helmet Module - FINAL PRODUCTION VERSION
// ═══════════════════════════════════════════════════════════════════════════
// ✅ Synchronized with Dashboard.vue
// ✅ Fast updates (1s heartbeat, 500ms alcohol)
// ✅ Correct Firebase paths
// ═══════════════════════════════════════════════════════════════════════════

// WiFi Configuration
const char* ssid = "DPWH";
const char* password = "123456789000";

// Firebase Configuration
const String firebaseHost = "https://vigilance-shield-default-rtdb.firebaseio.com";
const String userUID = "DMIARybyNHVhzwAmrx2QQjdpSb13";
const String firebaseAuth = "";

// Hardware Pins
const int mq3AnalogPin = 34;
const int ledPin = 2;
const int buzzerPin = 5;

// Sensor Configuration
const int alcoholThreshold = 1500;

// Update Intervals
unsigned long lastHeartbeat = 0;
const long heartbeatInterval = 1000;  // 1 second

unsigned long lastAlcoholUpdate = 0;
const long alcoholUpdateInterval = 500;  // 500ms

void setup() {
  pinMode(mq3AnalogPin, INPUT);
  pinMode(ledPin, OUTPUT);
  pinMode(buzzerPin, OUTPUT);
  
  digitalWrite(ledPin, LOW);
  noTone(buzzerPin);
  
  Serial.begin(115200);
  delay(1000);
  
  Serial.println("\n╔════════════════════════════════════════╗");
  Serial.println("║   HELMET MODULE - FINAL VERSION        ║");
  Serial.println("╠════════════════════════════════════════╣");
  Serial.println("║ ⚡ 1s heartbeat                        ║");
  Serial.println("║ ⚡ 500ms alcohol updates               ║");
  Serial.println("║ ✅ Dashboard synchronized              ║");
  Serial.println("╚════════════════════════════════════════╝\n");
  
  connectToWiFi();
  
  // Send initial heartbeat
  sendHelmetHeartbeat(true);
  lastHeartbeat = millis();
  
  // Send initial alcohol reading
  int alcoholValue = analogRead(mq3AnalogPin);
  bool alcoholDetected = alcoholValue > alcoholThreshold;
  sendAlcoholToFirebase(alcoholValue, alcoholDetected);
  lastAlcoholUpdate = millis();
}

void loop() {
  if (WiFi.status() != WL_CONNECTED) connectToWiFi();
  
  // Send heartbeat every 1 second
  if (millis() - lastHeartbeat >= heartbeatInterval) {
    sendHelmetHeartbeat(true);
    lastHeartbeat = millis();
  }
  
  // Update alcohol every 500ms
  if (millis() - lastAlcoholUpdate >= alcoholUpdateInterval) {
    int alcoholValue = analogRead(mq3AnalogPin);
    bool alcoholDetected = alcoholValue > alcoholThreshold;
    
    Serial.printf("[ALCOHOL] Value: %d | Threshold: %d | Status: %s\n", 
                  alcoholValue, alcoholThreshold, 
                  alcoholDetected ? "DANGER ⚠️" : "SAFE ✓");
    
    // Update LED and Buzzer
    digitalWrite(ledPin, alcoholDetected ? HIGH : LOW);
    
    if (alcoholDetected) {
      playBeepAlert();
    } else {
      noTone(buzzerPin);
    }
    
    // Send to Firebase
    sendAlcoholToFirebase(alcoholValue, alcoholDetected);
    lastAlcoholUpdate = millis();
  }
  
  delay(50);
}

void connectToWiFi() {
  WiFi.begin(ssid, password);
  Serial.print("[WIFI] Connecting");
  
  int attempts = 0;
  while (WiFi.status() != WL_CONNECTED && attempts < 20) {
    delay(500);
    Serial.print(".");
    attempts++;
  }
  
  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("\n[WIFI] ✓ Connected!");
    Serial.printf("[WIFI] IP: %s\n", WiFi.localIP().toString().c_str());
    configTime(0, 0, "pool.ntp.org", "time.google.com");
    Serial.println("[TIME] ✓ NTP clock sync initialized");
  } else {
    Serial.println("\n[WIFI] ✗ Failed!");
  }
}

static uint64_t lastSyncedEpoch = 0;
static unsigned long lastSyncedMillis = 0;

uint64_t getEpochTimestamp() {
  time_t now;
  struct tm timeinfo;
  if (getLocalTime(&timeinfo, 50)) {
    time(&now);
    lastSyncedEpoch = ((uint64_t)now) * 1000ULL;
    lastSyncedMillis = millis();
    return lastSyncedEpoch + (millis() % 1000);
  }
  if (lastSyncedEpoch > 0) {
    return lastSyncedEpoch + (millis() - lastSyncedMillis);
  }
  return 0;
}

void sendHelmetHeartbeat(bool isActive) {
  if (WiFi.status() != WL_CONNECTED) return;
  
  uint64_t timestamp = getEpochTimestamp();
  
  StaticJsonDocument<128> doc;
  doc["status"] = isActive ? "On" : "Off";
  doc["lastHeartbeat"] = timestamp;
  doc["timestamp"] = timestamp;
  
  String payload;
  serializeJson(doc, payload);
  
  HTTPClient http;
  http.setTimeout(2000);
  String url = firebaseHost + "/helmet_public/" + userUID + "/devices/helmet.json?auth=" + firebaseAuth;
  http.begin(url);
  http.addHeader("Content-Type", "application/json");
  
  int code = http.PUT(payload);
  
  if (code == HTTP_CODE_OK) {
    Serial.printf("[HEARTBEAT] ✓ Sent (timestamp: %llu)\n", timestamp);
  } else {
    Serial.printf("[HEARTBEAT] ✗ Failed: HTTP %d\n", code);
  }
  
  http.end();
}

void sendAlcoholToFirebase(int alcoholVal, bool alcoholStatus) {
  if (WiFi.status() != WL_CONNECTED) return;
  
  uint64_t timestamp = getEpochTimestamp();
  String statusText = alcoholStatus ? "Danger" : "Safe";
  
  // Main path - Dashboard reads from here
  StaticJsonDocument<128> doc;
  doc["sensorValue"] = alcoholVal;
  doc["status"] = statusText;
  doc["timestamp"] = timestamp;
  doc["detected"] = alcoholStatus;
  
  String payload;
  serializeJson(doc, payload);
  
  HTTPClient http;
  http.setTimeout(1500);
  String url = firebaseHost + "/helmet_public/" + userUID + "/alcohol/status.json?auth=" + firebaseAuth;
  http.begin(url);
  http.addHeader("Content-Type", "application/json");
  
  int code = http.PUT(payload);
  
  if (code == HTTP_CODE_OK) {
    Serial.printf("[ALCOHOL] ✓ Firebase updated\n");
  } else {
    Serial.printf("[ALCOHOL] ✗ Firebase failed: HTTP %d\n", code);
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
