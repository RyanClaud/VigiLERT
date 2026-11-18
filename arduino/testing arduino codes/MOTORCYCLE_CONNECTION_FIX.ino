#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>

// ═══════════════════════════════════════════════════════════════════════════
// MOTORCYCLE CONNECTION FIX - Diagnostic Version
// ═══════════════════════════════════════════════════════════════════════════
// ✅ Fixed: Always send status = "On" for device connection
// ✅ Separate engineRunning field for engine state
// ✅ Enhanced debugging for connection issues
// ═══════════════════════════════════════════════════════════════════════════

// WiFi Configuration
const char* ssid = "DPWH";
const char* password = "12345678900";

// Firebase Configuration
const String firebaseHost = "https://vigilance-shield-default-rtdb.firebaseio.com";
const String userUID = "MnzBjTBslZNijOkq732PE91hHa23";
const String firebaseAuth = "";

// Pin Assignments
const int relayPin = 13;
const int ignitionRelayPin = 27;
const int buzzerPin = 12;
const int lightIndicatorPin = 2;

// Engine control
bool engineRunning = false;
bool engineAllowed = false;

// Timing
unsigned long lastHeartbeat = 0;
const unsigned long HEARTBEAT_INTERVAL = 1000;  // 1 second for testing

// Firebase paths
const String devicePath = "/helmet_public/" + userUID + "/devices/motorcycle.json?auth=" + firebaseAuth;
const String buttonPath = "/" + userUID + "/engineControl/startButton.json?auth=" + firebaseAuth;

// HTTP client
HTTPClient http;

void setup() {
  Serial.begin(115200);
  delay(100);
  
  Serial.println("\n╔════════════════════════════════════════╗");
  Serial.println("║  MOTORCYCLE CONNECTION FIX - TEST     ║");
  Serial.println("╠════════════════════════════════════════╣");
  Serial.println("║ 🔧 Fixed device status reporting      ║");
  Serial.println("║ 🔍 Enhanced connection debugging      ║");
  Serial.println("║ ⚡ Fast heartbeat for testing         ║");
  Serial.println("╚════════════════════════════════════════╝\n");

  // Initialize pins
  pinMode(relayPin, OUTPUT);
  pinMode(ignitionRelayPin, OUTPUT);
  pinMode(buzzerPin, OUTPUT);
  pinMode(lightIndicatorPin, OUTPUT);

  // Initialize relays to BLOCK
  digitalWrite(relayPin, LOW);           // Starter BLOCKED
  digitalWrite(ignitionRelayPin, LOW);   // Ignition BLOCKED
  digitalWrite(buzzerPin, LOW);
  digitalWrite(lightIndicatorPin, LOW);
  
  engineRunning = false;
  engineAllowed = false;
  
  Serial.println("[SETUP] 🔒 Engine blocked by default");
  Serial.printf("[SETUP] Starter: GPIO %d = %d (BLOCKED)\n", relayPin, digitalRead(relayPin));
  Serial.printf("[SETUP] Ignition: GPIO %d = %d (BLOCKED)\n", ignitionRelayPin, digitalRead(ignitionRelayPin));

  // Connect to WiFi
  connectToWiFi();
  
  // Send initial heartbeat
  sendDiagnosticHeartbeat();
  lastHeartbeat = millis();

  Serial.println("\n🚀 MOTORCYCLE CONNECTION FIX READY!");
  Serial.println("📋 Commands: START, STOP, STATUS, TEST");
  Serial.println("🔍 Watch dashboard for connection status changes\n");
}

void loop() {
  unsigned long currentTime = millis();
  
  // Send heartbeat every second
  if (currentTime - lastHeartbeat >= HEARTBEAT_INTERVAL) {
    sendDiagnosticHeartbeat();
    lastHeartbeat = currentTime;
  }

  // Check dashboard button
  checkDashboardButton();

  // Handle serial commands
  handleSerialCommands();

  yield();
}

void connectToWiFi() {
  Serial.println("\n[WIFI] Connecting...");
  WiFi.mode(WIFI_STA);
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

void sendDiagnosticHeartbeat() {
  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("[HEARTBEAT] ❌ WiFi not connected");
    return;
  }

  unsigned long startTime = millis();
  
  // ✅ FIXED: Always send status = "On" for device connection
  StaticJsonDocument<256> doc;
  doc["status"] = "On";  // ✅ DEVICE is always "On" when connected
  doc["lastHeartbeat"] = (uint64_t)(1700000000000ULL + millis());
  doc["timestamp"] = (uint64_t)(1700000000000ULL + millis());
  doc["engineRunning"] = engineRunning;  // ✅ SEPARATE field for engine state
  doc["engineAllowed"] = engineAllowed;
  doc["relayState"] = digitalRead(relayPin);
  doc["ignitionState"] = digitalRead(ignitionRelayPin);
  
  String payload;
  serializeJson(doc, payload);
  
  Serial.println("\n[HEARTBEAT] 📤 Sending to Firebase:");
  Serial.println("Path: " + devicePath);
  Serial.println("Data: " + payload);
  
  http.begin(firebaseHost + devicePath);
  http.addHeader("Content-Type", "application/json");
  
  int httpCode = http.PUT(payload);
  unsigned long responseTime = millis() - startTime;
  
  if (httpCode == HTTP_CODE_OK) {
    String response = http.getString();
    Serial.printf("[HEARTBEAT] ✅ Success! HTTP %d (%lu ms)\n", httpCode, responseTime);
    Serial.println("Response: " + response);
  } else {
    Serial.printf("[HEARTBEAT] ❌ Failed! HTTP %d (%lu ms)\n", httpCode, responseTime);
    if (http.getString().length() > 0) {
      Serial.println("Error: " + http.getString());
    }
  }
  
  http.end();
}

void checkDashboardButton() {
  static unsigned long lastButtonCheck = 0;
  if (millis() - lastButtonCheck < 1000) return;  // Check every second
  
  if (WiFi.status() != WL_CONNECTED) return;

  http.begin(firebaseHost + buttonPath);
  int httpCode = http.GET();
  
  if (httpCode == HTTP_CODE_OK) {
    String response = http.getString();
    bool buttonPressed = (response.indexOf("true") != -1);
    
    static bool lastButtonState = false;
    if (buttonPressed && !lastButtonState) {
      Serial.println("\n🖥️ DASHBOARD BUTTON PRESSED!");
      
      if (!engineRunning) {
        startEngine();
      } else {
        stopEngine();
      }
      
      // Clear button
      http.begin(firebaseHost + buttonPath);
      http.addHeader("Content-Type", "application/json");
      http.PUT("false");
      http.end();
      
      lastButtonState = true;
    } else if (!buttonPressed) {
      lastButtonState = false;
    }
  }
  
  http.end();
  lastButtonCheck = millis();
}

void startEngine() {
  Serial.println("\n✅ STARTING ENGINE...");
  
  digitalWrite(ignitionRelayPin, HIGH);  // HIGH = ALLOW
  digitalWrite(relayPin, HIGH);          // HIGH = ALLOW
  
  engineRunning = true;
  engineAllowed = true;
  
  Serial.printf("✅ Starter Relay GPIO %d = %d (ALLOW)\n", relayPin, digitalRead(relayPin));
  Serial.printf("✅ Ignition Relay GPIO %d = %d (ALLOW)\n", ignitionRelayPin, digitalRead(ignitionRelayPin));
  Serial.println("✅ Physical key should now work!");
  
  // Send immediate heartbeat to update dashboard
  sendDiagnosticHeartbeat();
}

void stopEngine() {
  Serial.println("\n🛑 STOPPING ENGINE...");
  
  digitalWrite(ignitionRelayPin, LOW);   // LOW = BLOCK
  digitalWrite(relayPin, LOW);           // LOW = BLOCK
  
  engineRunning = false;
  engineAllowed = false;
  
  Serial.printf("🛑 Starter Relay GPIO %d = %d (BLOCK)\n", relayPin, digitalRead(relayPin));
  Serial.printf("🛑 Ignition Relay GPIO %d = %d (BLOCK)\n", ignitionRelayPin, digitalRead(ignitionRelayPin));
  Serial.println("🔒 Physical key is now BLOCKED!");
  
  // Send immediate heartbeat to update dashboard
  sendDiagnosticHeartbeat();
}

void handleSerialCommands() {
  if (Serial.available()) {
    String cmd = Serial.readStringUntil('\n');
    cmd.trim();
    cmd.toUpperCase();
    
    if (cmd == "START" || cmd == "START ENGINE") {
      startEngine();
    }
    else if (cmd == "STOP" || cmd == "STOP ENGINE") {
      stopEngine();
    }
    else if (cmd == "STATUS") {
      printStatus();
    }
    else if (cmd == "TEST") {
      Serial.println("\n🧪 RUNNING CONNECTION TEST...");
      testConnection();
    }
    else if (cmd == "WIFI") {
      Serial.printf("WiFi Status: %s\n", WiFi.status() == WL_CONNECTED ? "CONNECTED" : "DISCONNECTED");
      if (WiFi.status() == WL_CONNECTED) {
        Serial.printf("IP: %s\n", WiFi.localIP().toString().c_str());
        Serial.printf("Signal: %d dBm\n", WiFi.RSSI());
      }
    }
    else if (cmd == "HELP") {
      Serial.println("\n📋 AVAILABLE COMMANDS:");
      Serial.println("START    - Start engine");
      Serial.println("STOP     - Stop engine");
      Serial.println("STATUS   - Show system status");
      Serial.println("TEST     - Test Firebase connection");
      Serial.println("WIFI     - Show WiFi status");
      Serial.println("HELP     - Show this help");
    }
  }
}

void testConnection() {
  Serial.println("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━");
  Serial.println("🧪 CONNECTION TEST");
  Serial.println("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━");
  
  // Test WiFi
  Serial.printf("1. WiFi: %s\n", WiFi.status() == WL_CONNECTED ? "✅ CONNECTED" : "❌ DISCONNECTED");
  if (WiFi.status() == WL_CONNECTED) {
    Serial.printf("   IP: %s\n", WiFi.localIP().toString().c_str());
    Serial.printf("   Signal: %d dBm\n", WiFi.RSSI());
  }
  
  // Test Firebase write
  Serial.println("\n2. Testing Firebase write...");
  sendDiagnosticHeartbeat();
  
  // Test Firebase read
  Serial.println("\n3. Testing Firebase read...");
  http.begin(firebaseHost + devicePath);
  int httpCode = http.GET();
  
  if (httpCode == HTTP_CODE_OK) {
    String response = http.getString();
    Serial.println("✅ Firebase read successful!");
    Serial.println("Data: " + response);
  } else {
    Serial.printf("❌ Firebase read failed: HTTP %d\n", httpCode);
  }
  
  http.end();
  
  Serial.println("\n4. Dashboard should show:");
  Serial.println("   - Motorcycle: CONNECTED (green)");
  Serial.printf("   - Engine: %s\n", engineRunning ? "RUNNING" : "STOPPED");
  
  Serial.println("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━");
}

void printStatus() {
  Serial.println("\n━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━");
  Serial.println("📊 MOTORCYCLE STATUS");
  Serial.println("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━");
  
  Serial.printf("WiFi: %s\n", WiFi.status() == WL_CONNECTED ? "CONNECTED ✅" : "DISCONNECTED ❌");
  if (WiFi.status() == WL_CONNECTED) {
    Serial.printf("IP: %s\n", WiFi.localIP().toString().c_str());
    Serial.printf("Signal: %d dBm\n", WiFi.RSSI());
  }
  
  Serial.println("\n🔒 ENGINE STATUS:");
  Serial.printf("Engine Running: %s\n", engineRunning ? "YES ✅" : "NO ❌");
  Serial.printf("Engine Allowed: %s\n", engineAllowed ? "YES ✅" : "NO ❌");
  Serial.printf("Starter Relay: GPIO %d = %d (%s)\n", relayPin, digitalRead(relayPin), digitalRead(relayPin) ? "ALLOW" : "BLOCK");
  Serial.printf("Ignition Relay: GPIO %d = %d (%s)\n", ignitionRelayPin, digitalRead(ignitionRelayPin), digitalRead(ignitionRelayPin) ? "ALLOW" : "BLOCK");
  
  Serial.println("\n📡 FIREBASE STATUS:");
  Serial.println("Device Path: " + devicePath);
  Serial.println("Button Path: " + buttonPath);
  Serial.printf("Heartbeat Interval: %lu ms\n", HEARTBEAT_INTERVAL);
  
  Serial.println("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━");
}