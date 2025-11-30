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

// ✅ FIXED: Calibration Threshold (aligned with dashboard expectation)
// Dashboard expects threshold around 2000
// Your sensor baseline is around 550, so we need to adjust
const int alcoholThreshold = 550;  // Your sensor's detection threshold
const int dangerThreshold = 600;   // When to report "Danger" to dashboard

// Heartbeat
unsigned long lastHeartbeat = 0;
const long heartbeatInterval = 2000;

// ✅ NEW: Track last alcohol state to detect changes
bool lastAlcoholState = false;

void setup() {
  pinMode(mq3AnalogPin, INPUT);
  pinMode(ledPin, OUTPUT);
  pinMode(buzzerPin, OUTPUT);
  digitalWrite(ledPin, LOW);
  noTone(buzzerPin);

  Serial.begin(115200);
  delay(1000);

  Serial.println("\n╔════════════════════════════════════════╗");
  Serial.println("║   HELMET ALCOHOL DETECTION - FIXED    ║");
  Serial.println("╠════════════════════════════════════════╣");
  Serial.println("║ ✅ Threshold aligned with dashboard   ║");
  Serial.println("║ ✅ Enhanced Firebase logging          ║");
  Serial.println("║ ✅ Real-time status updates           ║");
  Serial.println("╚════════════════════════════════════════╝\n");

  connectToWiFi();

  // Test timestamp generation
  testTimestampGeneration();

  // Send initial heartbeat
  sendHelmetHeartbeat(true);
  lastHeartbeat = millis();
  
  // ✅ Send initial alcohol status
  int initialValue = analogRead(mq3AnalogPin);
  sendAlcoholToFirebase(initialValue, false);
  
  Serial.println("\n[SETUP] ✅ Helmet module ready!");
  Serial.printf("[SETUP] Alcohol threshold: %d\n", alcoholThreshold);
  Serial.printf("[SETUP] Danger threshold: %d\n", dangerThreshold);
  Serial.println("[SETUP] Monitoring alcohol levels...\n");
}

void loop() {
  if (WiFi.status() != WL_CONNECTED) connectToWiFi();

  // Read alcohol sensor
  int alcoholValue = analogRead(mq3AnalogPin);
  
  // ✅ FIXED: Determine if alcohol is detected based on YOUR sensor's threshold
  bool alcoholDetected = alcoholValue > dangerThreshold;
  
  // ✅ Detect state changes for better logging
  if (alcoholDetected != lastAlcoholState) {
    if (alcoholDetected) {
      Serial.println("\n🚨🚨🚨 ALCOHOL DETECTION STATE CHANGE: SAFE → DANGER 🚨🚨🚨");
    } else {
      Serial.println("\n✅✅✅ ALCOHOL DETECTION STATE CHANGE: DANGER → SAFE ✅✅✅");
    }
    lastAlcoholState = alcoholDetected;
  }

  Serial.printf("\n[ALCOHOL] Sensor Value: %d | Threshold: %d | Danger: %d | Status: %s\n", 
                alcoholValue, alcoholThreshold, dangerThreshold, 
                alcoholDetected ? "DANGER ⚠️" : "SAFE ✓");
  
  if (alcoholDetected) {
    Serial.printf("   ⚠️ ALCOHOL DETECTED! Value %d exceeds danger threshold %d\n", 
                  alcoholValue, dangerThreshold);
    Serial.println("   📱 Sending DANGER status to Firebase...");
  }

  // Update LED and Buzzer
  digitalWrite(ledPin, alcoholDetected ? HIGH : LOW);
  if (alcoholDetected) {
    playBeepAlert();
  } else {
    noTone(buzzerPin);
  }

  // ✅ Send data to Firebase with enhanced logging
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
  // ✅ Use 64-bit integer to prevent overflow
  uint64_t timestamp = 1700000000000ULL + (uint64_t)millis();

  StaticJsonDocument<128> doc;
  doc["status"] = isActive ? "On" : "Off";
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
    Serial.printf("[HEARTBEAT] ✓ Sent (timestamp: %llu)\n", timestamp);
  } else {
    Serial.printf("[HEARTBEAT] ✗ Failed! HTTP: %d\n", code);
  }

  http.end();
}

void sendAlcoholToFirebase(int alcoholVal, bool alcoholStatus) {
  Serial.println("\n[FIREBASE] ═══════════════════════════════");
  Serial.println("[FIREBASE] Preparing alcohol data...");
  
  uint64_t timestamp = 1700000000000ULL + (uint64_t)millis();
  
  // ✅ CRITICAL: Send status as "Danger" or "Safe" (exact match for dashboard)
  String statusString = alcoholStatus ? "Danger" : "Safe";
  
  StaticJsonDocument<128> doc;
  doc["sensorValue"] = alcoholVal;
  doc["status"] = statusString;  // ✅ Must be exactly "Danger" or "Safe"
  doc["timestamp"] = timestamp;

  String payload;
  serializeJson(doc, payload);

  Serial.printf("[FIREBASE] Sensor Value: %d\n", alcoholVal);
  Serial.printf("[FIREBASE] Status: %s\n", statusString.c_str());
  Serial.printf("[FIREBASE] Timestamp: %llu\n", timestamp);
  Serial.printf("[FIREBASE] JSON Payload: %s\n", payload.c_str());

  HTTPClient http;
  
  // ✅ CRITICAL: Correct Firebase path (must match dashboard listener)
  String url = firebaseHost + "/helmet_public/" + userUID + "/alcohol/status.json?auth=" + firebaseAuth;
  
  Serial.printf("[FIREBASE] URL: %s\n", url.c_str());
  
  http.begin(url);
  http.addHeader("Content-Type", "application/json");
  int code = http.PUT(payload);

  if (code == HTTP_CODE_OK) {
    Serial.println("[FIREBASE] ✅ SUCCESS! Alcohol status updated");
    Serial.println("[FIREBASE] Dashboard should now show:");
    Serial.printf("[FIREBASE]   - Status: %s\n", statusString.c_str());
    Serial.printf("[FIREBASE]   - Sensor Value: %d\n", alcoholVal);
    
    if (alcoholStatus) {
      Serial.println("[FIREBASE] 🚨 Dashboard should show RED DANGER card!");
    } else {
      Serial.println("[FIREBASE] ✅ Dashboard should show GREEN SAFE card");
    }
  } else {
    Serial.printf("[FIREBASE] ❌ FAILED! HTTP Code: %d\n", code);
    Serial.println("[FIREBASE] Troubleshooting:");
    Serial.println("[FIREBASE]   1. Check WiFi connection");
    Serial.println("[FIREBASE]   2. Verify Firebase URL");
    Serial.println("[FIREBASE]   3. Check userUID is correct");
    Serial.println("[FIREBASE]   4. Verify Firebase rules allow write");
  }
  
  Serial.println("[FIREBASE] ═══════════════════════════════\n");
  
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
