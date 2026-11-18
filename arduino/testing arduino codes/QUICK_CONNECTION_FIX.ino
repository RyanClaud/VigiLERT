// ═══════════════════════════════════════════════════════════════════════════
// VIGILERT QUICK CONNECTION FIX
// ═══════════════════════════════════════════════════════════════════════════
// ✅ Minimal code to restore motorcycle dashboard connection
// ✅ Based on your exact working configuration
// ✅ No safety features - just connection restoration
// ✅ Upload this first to verify connection, then upgrade to safety version
// ═══════════════════════════════════════════════════════════════════════════

#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>

// WiFi Configuration
const char* ssid = "DPWH";
const char* password = "12345678900";

// Firebase Configuration
const String firebaseHost = "https://vigilance-shield-default-rtdb.firebaseio.com";
const String userUID = "MnzBjTBslZNijOkq732PE91hHa23";

// Firebase Paths - EXACT SAME AS YOUR WORKING VERSION
const String devicePath = "/helmet_public/" + userUID + "/devices/motorcycle.json";
const String buttonPath = "/" + userUID + "/engineControl/startButton.json";

// Pin Assignments
const int relayPin = 13;
const int ignitionRelayPin = 27;
const int buzzerPin = 12;

// Core States
bool engineRunning = false;
bool engineAllowed = false;

// Timing
const unsigned long HEARTBEAT_INTERVAL = 500;
const unsigned long BUTTON_CHECK_INTERVAL = 200;
unsigned long lastHeartbeat = 0;
unsigned long lastButtonCheck = 0;

// HTTP client
HTTPClient http;

void setup() {
  Serial.begin(115200);
  delay(500);
  
  Serial.println("\n╔════════════════════════════════════════════════════════════╗");
  Serial.println("║           VIGILERT QUICK CONNECTION FIX                   ║");
  Serial.println("╠════════════════════════════════════════════════════════════╣");
  Serial.println("║ 🔗 Minimal code to restore dashboard connection           ║");
  Serial.println("║ ⚡ Based on your exact working configuration               ║");
  Serial.println("║ 🎯 Upload this first to verify connection works           ║");
  Serial.println("╚════════════════════════════════════════════════════════════╝\n");

  // Initialize pins
  pinMode(relayPin, OUTPUT);
  pinMode(ignitionRelayPin, OUTPUT);
  pinMode(buzzerPin, OUTPUT);

  // Initialize relays to BLOCK
  digitalWrite(relayPin, LOW);
  digitalWrite(ignitionRelayPin, LOW);
  
  engineRunning = false;
  engineAllowed = false;
  
  Serial.println("[SETUP] 🔒 Relays initialized to BLOCK");
  Serial.printf("[SETUP] Starter: GPIO %d = BLOCKED\n", relayPin);
  Serial.printf("[SETUP] Ignition: GPIO %d = BLOCKED\n", ignitionRelayPin);

  // Connect to WiFi
  connectToWiFi();
  
  if (WiFi.status() == WL_CONNECTED) {
    testFirebaseConnection();
    
    // Send immediate connection heartbeat
    for (int i = 0; i < 3; i++) {
      Serial.printf("Sending connection heartbeat %d/3...\n", i + 1);
      sendConnectionHeartbeat();
      delay(1000);
    }
  }

  Serial.println("\n🚀 QUICK CONNECTION FIX READY!");
  Serial.println("📋 Commands: START, STOP, STATUS, CONNECT");
  Serial.println("🖥️ Check dashboard - should show 'Motorcycle: Connected'\n");
}

void loop() {
  unsigned long currentTime = millis();
  
  // Check WiFi connection
  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("⚠️ WiFi disconnected! Reconnecting...");
    connectToWiFi();
    return;
  }
  
  // Send heartbeat
  if (currentTime - lastHeartbeat >= HEARTBEAT_INTERVAL) {
    sendConnectionHeartbeat();
    lastHeartbeat = currentTime;
  }

  // Check dashboard button
  if (currentTime - lastButtonCheck >= BUTTON_CHECK_INTERVAL) {
    checkDashboardButton();
    lastButtonCheck = currentTime;
  }

  // Handle serial commands
  handleSerialCommands();

  yield();
}

void connectToWiFi() {
  Serial.println("[WIFI] Connecting to: " + String(ssid));
  WiFi.mode(WIFI_STA);
  WiFi.setAutoReconnect(true);
  WiFi.begin(ssid, password);
  
  int attempts = 0;
  while (WiFi.status() != WL_CONNECTED && attempts < 20) {
    delay(500);
    Serial.print(".");
    attempts++;
  }
  
  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("\n[WIFI] ✅ Connected!");
    Serial.printf("[WIFI] IP: %s\n", WiFi.localIP().toString().c_str());
    Serial.printf("[WIFI] Signal: %d dBm\n", WiFi.RSSI());
  } else {
    Serial.println("\n[WIFI] ❌ Failed!");
  }
}

void testFirebaseConnection() {
  Serial.println("[FIREBASE] Testing connection...");
  
  http.begin(firebaseHost + devicePath);
  http.setTimeout(5000);
  
  int httpCode = http.GET();
  
  if (httpCode == 200 || httpCode == HTTP_CODE_OK) {
    Serial.println("[FIREBASE] ✅ Connection successful!");
  } else {
    Serial.printf("[FIREBASE] ⚠️ HTTP %d - %s\n", httpCode, http.errorToString(httpCode).c_str());
  }
  
  http.end();
}

void sendConnectionHeartbeat() {
  if (WiFi.status() != WL_CONNECTED) return;

  // Simple heartbeat - same structure as your working version
  StaticJsonDocument<400> doc;
  doc["status"] = "On";  // CRITICAL: Must be "On" for dashboard connection
  doc["lastHeartbeat"] = (uint64_t)(1700000000000ULL + millis());
  doc["timestamp"] = (uint64_t)(1700000000000ULL + millis());
  
  // Engine status
  doc["engineRunning"] = engineRunning;
  doc["engineAllowed"] = engineAllowed;
  doc["relayState"] = digitalRead(relayPin);
  doc["ignitionState"] = digitalRead(ignitionRelayPin);
  
  // System info
  doc["freeHeap"] = ESP.getFreeHeap();
  doc["wifiSignal"] = WiFi.RSSI();
  doc["uptime"] = millis();
  doc["ipAddress"] = WiFi.localIP().toString();
  
  // Connection info
  doc["connected"] = true;
  doc["deviceType"] = "motorcycle";
  doc["version"] = "quick_fix_v1.0";
  
  String payload;
  serializeJson(doc, payload);
  
  unsigned long startTime = millis();
  
  http.begin(firebaseHost + devicePath);
  http.addHeader("Content-Type", "application/json");
  http.setTimeout(3000);
  
  int httpCode = http.PUT(payload);
  unsigned long duration = millis() - startTime;
  
  if (httpCode == 200 || httpCode == HTTP_CODE_OK) {
    Serial.printf("[HEARTBEAT] ✅ %lums | Engine:%s | Connected:YES\n", 
                  duration, 
                  engineRunning ? "ON" : "OFF");
  } else {
    Serial.printf("[HEARTBEAT] ❌ HTTP %d (%lums)\n", httpCode, duration);
  }
  
  http.end();
}

void checkDashboardButton() {
  if (WiFi.status() != WL_CONNECTED) return;

  http.begin(firebaseHost + buttonPath);
  http.setTimeout(2000);
  
  int httpCode = http.GET();
  
  if (httpCode == 200 || httpCode == HTTP_CODE_OK) {
    String response = http.getString();
    bool buttonPressed = (response.indexOf("true") != -1);
    
    static bool lastButtonState = false;
    if (buttonPressed && !lastButtonState) {
      Serial.println("\n🖥️ DASHBOARD BUTTON PRESSED!");
      
      if (!engineRunning) {
        Serial.println("🚀 Starting engine via dashboard");
        startEngine();
      } else {
        Serial.println("🛑 Stopping engine via dashboard");
        stopEngine();
      }
      
      // Clear button
      http.end();
      http.begin(firebaseHost + buttonPath);
      http.addHeader("Content-Type", "application/json");
      http.setTimeout(2000);
      http.PUT("false");
      http.end();
      
      // Force immediate heartbeat
      sendConnectionHeartbeat();
      
      lastButtonState = true;
    } else if (!buttonPressed) {
      lastButtonState = false;
    }
  }
  
  http.end();
}

void startEngine() {
  Serial.println("\n✅ STARTING ENGINE");
  
  digitalWrite(ignitionRelayPin, HIGH);
  digitalWrite(relayPin, HIGH);
  
  engineRunning = true;
  engineAllowed = true;
  
  Serial.printf("✅ Starter Relay GPIO %d = ALLOW\n", relayPin);
  Serial.printf("✅ Ignition Relay GPIO %d = ALLOW\n", ignitionRelayPin);
  
  // Immediate heartbeat update
  sendConnectionHeartbeat();
}

void stopEngine() {
  Serial.println("\n🛑 STOPPING ENGINE");
  
  digitalWrite(ignitionRelayPin, LOW);
  digitalWrite(relayPin, LOW);
  
  engineRunning = false;
  engineAllowed = false;
  
  Serial.printf("🛑 Starter Relay GPIO %d = BLOCK\n", relayPin);
  Serial.printf("🛑 Ignition Relay GPIO %d = BLOCK\n", ignitionRelayPin);
  
  // Immediate heartbeat update
  sendConnectionHeartbeat();
}

void handleSerialCommands() {
  if (Serial.available()) {
    String cmd = Serial.readStringUntil('\n');
    cmd.trim();
    cmd.toUpperCase();
    
    if (cmd == "START") {
      startEngine();
    }
    else if (cmd == "STOP") {
      stopEngine();
    }
    else if (cmd == "STATUS") {
      printStatus();
    }
    else if (cmd == "CONNECT") {
      Serial.println("\n🔄 FORCING CONNECTION...");
      
      // Send multiple heartbeats
      for (int i = 0; i < 5; i++) {
        Serial.printf("Heartbeat %d/5...\n", i + 1);
        sendConnectionHeartbeat();
        delay(500);
      }
      
      Serial.println("✅ Connection attempt completed - check dashboard");
    }
  }
}

void printStatus() {
  Serial.println("\n━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━");
  Serial.println("📊 VIGILERT QUICK CONNECTION FIX - STATUS");
  Serial.println("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━");
  
  // WiFi Status
  Serial.printf("📡 WiFi: %s\n", WiFi.status() == WL_CONNECTED ? "CONNECTED ✅" : "DISCONNECTED ❌");
  if (WiFi.status() == WL_CONNECTED) {
    Serial.printf("    IP: %s | Signal: %d dBm\n", WiFi.localIP().toString().c_str(), WiFi.RSSI());
  }
  
  // Engine Status
  Serial.println("\n🔒 ENGINE STATUS:");
  Serial.printf("Engine Running: %s\n", engineRunning ? "YES ✅" : "NO ❌");
  Serial.printf("Engine Allowed: %s\n", engineAllowed ? "YES ✅" : "NO ❌");
  Serial.printf("Starter Relay: GPIO %d = %s\n", relayPin, digitalRead(relayPin) ? "ALLOW" : "BLOCK");
  Serial.printf("Ignition Relay: GPIO %d = %s\n", ignitionRelayPin, digitalRead(ignitionRelayPin) ? "ALLOW" : "BLOCK");
  
  // Firebase Paths
  Serial.println("\n🔥 FIREBASE CONFIGURATION:");
  Serial.println("Host: " + firebaseHost);
  Serial.println("User UID: " + userUID);
  Serial.println("Device Path: " + devicePath);
  Serial.println("Button Path: " + buttonPath);
  
  // System Health
  Serial.println("\n💾 SYSTEM HEALTH:");
  Serial.printf("Free Heap: %d bytes\n", ESP.getFreeHeap());
  Serial.printf("Uptime: %lu minutes\n", millis() / 60000);
  
  Serial.println("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━");
  Serial.println("📋 COMMANDS: START, STOP, STATUS, CONNECT");
  Serial.println("🎯 PURPOSE: Restore dashboard connection, then upgrade to safety version");
  Serial.println("🖥️ CHECK: Dashboard should show 'Motorcycle: Connected'");
  Serial.println("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━");
}