#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>

// ═══════════════════════════════════════════════════════════════════════════
// VigiLERT Motorcycle Module - MINIMAL SECURITY VERSION
// ═══════════════════════════════════════════════════════════════════════════
// ✅ Focus: Engine authorization and physical key blocking
// ✅ Minimal code for testing relay security system
// ═══════════════════════════════════════════════════════════════════════════

// WiFi Configuration
const char* ssid = "DPWH";
const char* password = "12345678900";

// Firebase Configuration
const String firebaseHost = "https://vigilance-shield-default-rtdb.firebaseio.com";
const String userUID = "MnzBjTBslZNijOkq732PE91hHa23";
const String firebaseAuth = "";

// Pin Assignments
const int relayPin = 13;              // Starter relay (GPIO 13)
const int ignitionRelayPin = 27;      // Ignition relay (GPIO 27) - CRITICAL FOR SECURITY
const int buzzerPin = 12;
const int lightIndicatorPin = 2;
const int vibrationSensorPin = 15;

// ✅ SECURITY: Engine authorization system (like SMS command system)
bool systemEngineEnabled = false;  // Engine LOCKED by default
bool engineAuthorized = false;     // Requires explicit authorization
unsigned long lastAuthorizationTime = 0;
const unsigned long AUTHORIZATION_TIMEOUT = 300000;  // 5 minutes timeout

// Engine control
bool engineRunning = false;
bool crashDetected = false;
bool alcoholDetected = false;

// Helmet connection
bool helmetConnected = false;
unsigned long lastHelmetUpdateTime = 0;

// WiFi connection
unsigned long lastWiFiConnected = 0;

// Timing
unsigned long lastHeartbeat = 0;
const unsigned long HEARTBEAT_INTERVAL = 1000;  // 1 second

// Starter safety
unsigned long starterStartTime = 0;
bool starterActive = false;
const unsigned long STARTER_TIMEOUT = 3000;      // 3 seconds max

void setup() {
  Serial.begin(115200);
  delay(100);
  
  Serial.println("\n╔════════════════════════════════════════╗");
  Serial.println("║   MOTORCYCLE SECURITY - MINIMAL TEST   ║");
  Serial.println("╠════════════════════════════════════════╣");
  Serial.println("║ 🔒 Engine authorization system         ║");
  Serial.println("║ 🛡️ Physical key blocking test         ║");
  Serial.println("║ ⚡ Dual relay security system         ║");
  Serial.println("╚════════════════════════════════════════╝\n");

  // Initialize pins
  pinMode(relayPin, OUTPUT);
  pinMode(ignitionRelayPin, OUTPUT);
  pinMode(buzzerPin, OUTPUT);
  pinMode(lightIndicatorPin, OUTPUT);
  pinMode(vibrationSensorPin, INPUT);

  // ✅ CRITICAL: Initialize relays to OFF (HIGH for ACTIVE-LOW relay)
  digitalWrite(relayPin, HIGH);           // Starter OFF (Active LOW)
  digitalWrite(ignitionRelayPin, HIGH);   // Ignition OFF (Active LOW) - BLOCKS PHYSICAL KEY
  digitalWrite(buzzerPin, LOW);
  digitalWrite(lightIndicatorPin, LOW);
  
  delay(100);
  Serial.println("\n[SETUP] ═══════════════════════════════════");
  Serial.println("[SETUP] DUAL RELAY SECURITY SYSTEM:");
  Serial.println("[SETUP] GPIO 13: Starter Relay (temporary)");
  Serial.println("[SETUP] GPIO 27: Ignition Relay (continuous)");
  Serial.printf("[SETUP] Starter Relay: %d (HIGH/OFF)\n", digitalRead(relayPin));
  Serial.printf("[SETUP] Ignition Relay: %d (HIGH/OFF)\n", digitalRead(ignitionRelayPin));
  Serial.println("[SETUP] 🔒 ENGINE LOCKED - Authorization required");
  Serial.println("[SETUP] 🔒 PHYSICAL KEY BLOCKED - Ignition relay OFF");
  Serial.println("[SETUP] ═══════════════════════════════════\n");

  connectToWiFi();
  
  Serial.println("\n📋 SERIAL COMMANDS:");
  Serial.println("   AUTHORIZE ENGINE / DISABLE ENGINE");
  Serial.println("   START ENGINE / STOP ENGINE");
  Serial.println("   TEST RELAYS (test physical key blocking)");
  Serial.println("   FORCE RELAY OFF (emergency block)");
  Serial.println("   STATUS\n");
  
  Serial.println("🔒 SECURITY NOTICE:");
  Serial.println("   Engine is LOCKED by default for security");
  Serial.println("   Use 'AUTHORIZE ENGINE' to enable engine start");
  Serial.println("   Physical key is BLOCKED until authorized\n");
}

void loop() {
  // ✅ SECURITY: Check authorization timeout
  if (systemEngineEnabled && engineAuthorized) {
    unsigned long timeSinceAuth = millis() - lastAuthorizationTime;
    if (timeSinceAuth >= AUTHORIZATION_TIMEOUT) {
      systemEngineEnabled = false;
      engineAuthorized = false;
      
      Serial.println("\n🔒 ENGINE AUTHORIZATION EXPIRED!");
      Serial.println("💡 5-minute timeout reached - engine locked for security");
      
      // Force stop engine if running
      if (engineRunning) {
        Serial.println("🛑 Stopping engine due to authorization expiry");
        stopEngine();
      }
      
      // Expiry alert
      for (int i = 0; i < 4; i++) {
        digitalWrite(buzzerPin, HIGH);
        delay(150);
        digitalWrite(buzzerPin, LOW);
        delay(150);
      }
    }
  }

  // Send heartbeat every 1 second
  if (millis() - lastHeartbeat >= HEARTBEAT_INTERVAL) {
    sendMotorcycleHeartbeat(true);
    lastHeartbeat = millis();
  }

  // Serial commands
  if (Serial.available()) {
    String cmd = Serial.readStringUntil('\n');
    cmd.trim();
    cmd.toUpperCase();
    
    if (cmd == "AUTHORIZE ENGINE" || cmd == "ENABLE ENGINE") {
      systemEngineEnabled = true;
      engineAuthorized = true;
      lastAuthorizationTime = millis();
      Serial.println("✅ ENGINE SYSTEM AUTHORIZED!");
      Serial.println("💡 Engine can now be started via dashboard or physical key");
      Serial.println("💡 Authorization expires in 5 minutes for security");
      
      // Success beeps
      for (int i = 0; i < 3; i++) {
        digitalWrite(buzzerPin, HIGH);
        delay(200);
        digitalWrite(buzzerPin, LOW);
        delay(100);
      }
    }
    else if (cmd == "DISABLE ENGINE" || cmd == "LOCK ENGINE") {
      systemEngineEnabled = false;
      engineAuthorized = false;
      
      // Force stop engine if running
      if (engineRunning) {
        stopEngine();
      }
      
      Serial.println("🔒 ENGINE SYSTEM LOCKED!");
      Serial.println("💡 Engine cannot be started until authorized");
      Serial.println("💡 Physical key is also blocked");
      
      // Lock beeps
      for (int i = 0; i < 2; i++) {
        digitalWrite(buzzerPin, HIGH);
        delay(300);
        digitalWrite(buzzerPin, LOW);
        delay(200);
      }
    }
    else if (cmd == "START ENGINE") {
      startEngine();
    }
    else if (cmd == "STOP ENGINE") {
      stopEngine();
    }
    else if (cmd == "TEST RELAYS" || cmd == "RELAY TEST") {
      Serial.println("\n🧪 TESTING RELAY OPERATION");
      Serial.println("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━");
      
      Serial.println("Step 1: Both relays OFF (should block engine)");
      digitalWrite(relayPin, HIGH);        // Starter OFF
      digitalWrite(ignitionRelayPin, HIGH); // Ignition OFF
      Serial.printf("Starter Relay GPIO %d = %d (HIGH/OFF)\n", relayPin, digitalRead(relayPin));
      Serial.printf("Ignition Relay GPIO %d = %d (HIGH/OFF)\n", ignitionRelayPin, digitalRead(ignitionRelayPin));
      Serial.println("🔒 Physical key should NOT start engine now");
      Serial.println("⏳ Test physical key for 10 seconds...");
      delay(10000);
      
      Serial.println("\nStep 2: Ignition relay ON (should allow engine start)");
      digitalWrite(ignitionRelayPin, LOW);  // Ignition ON
      Serial.printf("Ignition Relay GPIO %d = %d (LOW/ON)\n", ignitionRelayPin, digitalRead(ignitionRelayPin));
      Serial.println("✅ Physical key should start engine now");
      Serial.println("⏳ Test physical key for 10 seconds...");
      delay(10000);
      
      Serial.println("\nStep 3: Return to secure state");
      digitalWrite(ignitionRelayPin, HIGH); // Ignition OFF
      Serial.printf("Ignition Relay GPIO %d = %d (HIGH/OFF)\n", ignitionRelayPin, digitalRead(ignitionRelayPin));
      Serial.println("🔒 Physical key should NOT start engine again");
      
      Serial.println("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━");
      Serial.println("🧪 RELAY TEST COMPLETE");
      Serial.println("💡 If physical key still works when ignition relay is OFF,");
      Serial.println("💡 check wiring: ignition relay must be in SERIES with ignition coil");
    }
    else if (cmd == "FORCE RELAY OFF" || cmd == "BLOCK ENGINE") {
      digitalWrite(relayPin, HIGH);        // Starter OFF
      digitalWrite(ignitionRelayPin, HIGH); // Ignition OFF
      systemEngineEnabled = false;
      engineAuthorized = false;
      
      Serial.println("🔒 RELAYS FORCED OFF - ENGINE BLOCKED");
      Serial.printf("Starter Relay GPIO %d = %d (HIGH/OFF)\n", relayPin, digitalRead(relayPin));
      Serial.printf("Ignition Relay GPIO %d = %d (HIGH/OFF)\n", ignitionRelayPin, digitalRead(ignitionRelayPin));
      Serial.println("🔒 Physical key should be completely blocked");
      Serial.println("💡 If engine still starts, check ignition relay wiring");
      
      // Force stop engine if running
      if (engineRunning) {
        engineRunning = false;
        Serial.println("🛑 Engine status set to STOPPED");
      }
    }
    else if (cmd == "STATUS") {
      printSystemStatus();
    }
  }

  // ✅ STARTER SAFETY: Check starter timeout
  checkStarterTimeout();
  
  yield();  // Allow WiFi/system tasks to run
}

void startEngine() {
  Serial.println("\n[ENGINE] startEngine() called");
  
  // ✅ 0. SYSTEM AUTHORIZATION CHECK (like SMS command system)
  if (!systemEngineEnabled) {
    Serial.println("\n❌ ENGINE START BLOCKED - SYSTEM NOT AUTHORIZED!");
    Serial.println("💡 Engine system is disabled for security");
    Serial.println("💡 Use 'AUTHORIZE ENGINE' command to enable");
    Serial.println("💡 This prevents unauthorized engine start even with physical key");
    
    digitalWrite(relayPin, HIGH);      // HIGH = OFF for ACTIVE-LOW relay
    digitalWrite(ignitionRelayPin, HIGH);  // HIGH = OFF for ACTIVE-LOW relay
    
    // Authorization required alert (5 urgent beeps)
    for (int i = 0; i < 5; i++) {
      digitalWrite(buzzerPin, HIGH);
      delay(100);
      digitalWrite(buzzerPin, LOW);
      delay(100);
    }
    
    return;
  }

  Serial.println("\n✅ Starting engine...");
  
  // ✅ DUAL RELAY SYSTEM: Active LOW control
  // Step 1: Enable ignition (so engine can run after starting)
  digitalWrite(ignitionRelayPin, LOW);  // LOW = ON for active-low relay
  Serial.println("✅ Ignition relay ON");
  delay(500); // Wait for ignition to stabilize
  
  // Step 2: Engage starter motor (temporary)
  digitalWrite(relayPin, LOW);  // LOW = ON for active-low relay
  starterActive = true;
  starterStartTime = millis();
  Serial.println("✅ Starter relay ON (will auto-disengage after 3 seconds)");
  
  engineRunning = true;
  
  Serial.printf("✅ Starter Relay GPIO %d = %d (LOW/ON)\n", relayPin, digitalRead(relayPin));
  Serial.printf("✅ Ignition Relay GPIO %d = %d (LOW/ON)\n", ignitionRelayPin, digitalRead(ignitionRelayPin));
}

void stopEngine() {
  Serial.println("\n🛑 Stopping engine...");
  
  // ✅ DUAL RELAY SYSTEM: Cut ignition (this stops the engine)
  digitalWrite(ignitionRelayPin, HIGH);  // HIGH = OFF for active-low relay
  Serial.println("🛑 Ignition relay OFF (engine stops)");
  
  // ✅ Ensure starter is also off
  digitalWrite(relayPin, HIGH);  // HIGH = OFF for active-low relay
  Serial.println("🛑 Starter relay OFF");
  
  engineRunning = false;
  
  Serial.printf("🛑 Starter Relay GPIO %d = %d (HIGH/OFF)\n", relayPin, digitalRead(relayPin));
  Serial.printf("🛑 Ignition Relay GPIO %d = %d (HIGH/OFF)\n", ignitionRelayPin, digitalRead(ignitionRelayPin));
}

// ✅ STARTER SAFETY: Check starter timeout in main loop
void checkStarterTimeout() {
  if (starterActive && (millis() - starterStartTime >= STARTER_TIMEOUT)) {
    // Disengage starter after timeout
    digitalWrite(relayPin, HIGH);  // HIGH = OFF for ACTIVE-LOW relay
    starterActive = false;
    Serial.println("✅ Starter disengaged (3-second timeout)");
    Serial.printf("✅ Starter Relay GPIO %d = %d (OFF)\n", relayPin, digitalRead(relayPin));
    
    // Ignition relay stays ON to keep engine running
    // Only stopEngine() will turn off ignition
  }
}

bool sendMotorcycleHeartbeat(bool isActive) {
  if (WiFi.status() != WL_CONNECTED) return false;

  uint64_t timestamp = 1700000000000ULL + (uint64_t)millis();
  
  StaticJsonDocument<128> doc;
  doc["status"] = isActive ? "On" : "Off";
  doc["lastHeartbeat"] = timestamp;
  doc["timestamp"] = timestamp;
  
  String payload;
  serializeJson(doc, payload);
  
  HTTPClient http;
  String url = firebaseHost + "/helmet_public/" + userUID + "/devices/motorcycle.json?auth=" + firebaseAuth;
  http.begin(url);
  http.addHeader("Content-Type", "application/json");
  
  int code = http.PUT(payload);
  http.end();
  
  if (code == HTTP_CODE_OK) {
    lastWiFiConnected = millis();
    return true;
  }
  return false;
}

void connectToWiFi() {
  Serial.println("\n[WIFI] Connecting...");
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
    lastWiFiConnected = millis();
  } else {
    Serial.println("\n[WIFI] ❌ Failed!");
  }
}

void printSystemStatus() {
  Serial.println("\n━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━");
  Serial.println("🔍 SYSTEM STATUS");
  Serial.println("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━");
  
  // ✅ Authorization status
  Serial.printf("System Authorized: %s\n", systemEngineEnabled ? "YES ✅" : "NO ❌");
  if (systemEngineEnabled) {
    unsigned long timeRemaining = AUTHORIZATION_TIMEOUT - (millis() - lastAuthorizationTime);
    Serial.printf("Authorization Expires: %lu minutes\n", timeRemaining / 60000);
  }
  
  Serial.printf("Engine: %s\n", engineRunning ? "RUNNING" : "STOPPED");
  Serial.printf("Starter Relay: GPIO %d = %d (%s)\n", relayPin, digitalRead(relayPin), 
                digitalRead(relayPin) ? "OFF" : "ON");
  Serial.printf("Ignition Relay: GPIO %d = %d (%s)\n", ignitionRelayPin, digitalRead(ignitionRelayPin), 
                digitalRead(ignitionRelayPin) ? "OFF" : "ON");
  Serial.printf("WiFi: %s\n", WiFi.status() == WL_CONNECTED ? "CONNECTED" : "DISCONNECTED");
  
  Serial.println("\n🔒 SECURITY STATUS:");
  Serial.printf("Physical Key: %s\n", digitalRead(ignitionRelayPin) ? "BLOCKED 🔒" : "ALLOWED ✅");
  Serial.printf("Dashboard Control: %s\n", systemEngineEnabled ? "ENABLED ✅" : "BLOCKED 🔒");
  
  Serial.println("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━");
}