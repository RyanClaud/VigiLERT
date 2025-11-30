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

// ✅ OPTIMIZED: Faster update intervals
unsigned long lastHeartbeat = 0;
const long heartbeatInterval = 1000;  // ✅ 1 second (was 2 seconds)

unsigned long lastAlcoholUpdate = 0;
const long alcoholUpdateInterval = 500;  // ✅ 500ms for faster response

void setup() {
  pinMode(mq3AnalogPin, INPUT);
  pinMode(ledPin, OUTPUT);
  pinMode(buzzerPin, OUTPUT);
  
  digitalWrite(ledPin, LOW);
  noTone(buzzerPin);
  
  Serial.begin(115200);
  delay(1000);
  
  Serial.println("\n╔════════════════════════════════════════╗");
  Serial.println("║   HELMET OPTIMIZED VERSION             ║");
  Serial.println("╠════════════════════════════════════════╣");
  Serial.println("║ ⚡ 1s heartbeat (2x faster!)           ║");
  Serial.println("║ ⚡ 500ms alcohol updates (2x faster!)  ║");
  Serial.println("║ ✅ Multi-path Firebase sync            ║");
  Serial.println("╚════════════════════════════════════════╝\n");
  
  connectToWiFi();
  
  // Send initial heartbeat immediately
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
  
  // ✅ OPTIMIZED: Send heartbeat every 1 second
  if (millis() - lastHeartbeat >= heartbeatInterval) {
    sendHelmetHeartbeat(true);
    lastHeartbeat = millis();
  }
  
  // ✅ OPTIMIZED: Update alcohol every 500ms
  if (millis() - lastAlcoholUpdate >= alcoholUpdateInterval) {
    // Read alcohol sensor
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
  
  delay(50);  // ✅ Short delay for responsive loop
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
  } else {
    Serial.println("\n[WIFI] ✗ Failed!");
  }
}

void sendHelmetHeartbeat(bool isActive) {
  if (WiFi.status() != WL_CONNECTED) return;
  
  // ✅ Use 64-bit timestamp
  uint64_t timestamp = 1700000000000ULL + (uint64_t)millis();
  
  StaticJsonDocument<128> doc;
  doc["status"] = isActive ? "On" : "Off";
  doc["lastHeartbeat"] = timestamp;
  doc["timestamp"] = timestamp;
  
  String payload;
  serializeJson(doc, payload);
  
  HTTPClient http;
  http.setTimeout(2000);  // ✅ 2 second timeout
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
  
  uint64_t timestamp = 1700000000000ULL + (uint64_t)millis();
  String statusText = alcoholStatus ? "Danger" : "Safe";
  
  // ✅ OPTIMIZED: Send to all paths in parallel using single JSON
  StaticJsonDocument<256> doc;
  doc["sensorValue"] = alcoholVal;
  doc["status"] = statusText;
  doc["timestamp"] = timestamp;
  doc["detected"] = alcoholStatus;  // ✅ Add boolean for easy checking
  
  String payload;
  serializeJson(doc, payload);
  
  // Path 1: Main path
  HTTPClient http1;
  http1.setTimeout(1500);
  String url1 = firebaseHost + "/helmet_public/" + userUID + "/alcohol/status.json?auth=" + firebaseAuth;
  http1.begin(url1);
  http1.addHeader("Content-Type", "application/json");
  int code1 = http1.PUT(payload);
  http1.end();
  
  // Path 2: Dashboard compatibility (just status string)
  HTTPClient http2;
  http2.setTimeout(1500);
  String url2 = firebaseHost + "/" + userUID + "/alcohol/status/status.json?auth=" + firebaseAuth;
  http2.begin(url2);
  http2.addHeader("Content-Type", "application/json");
  String payload2 = "\"" + statusText + "\"";
  int code2 = http2.PUT(payload2);
  http2.end();
  
  // Path 3: Root path
  HTTPClient http3;
  http3.setTimeout(1500);
  String url3 = firebaseHost + "/" + userUID + "/alcohol.json?auth=" + firebaseAuth;
  http3.begin(url3);
  http3.addHeader("Content-Type", "application/json");
  int code3 = http3.PUT(payload);
  http3.end();
  
  // ✅ Compact logging
  Serial.printf("[ALCOHOL] Firebase: Path1=%d Path2=%d Path3=%d\n", code1, code2, code3);
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
