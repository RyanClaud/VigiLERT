// ═══════════════════════════════════════════════════════════════════════════
// VIGILERT MOTORCYCLE CONNECTION DIAGNOSTIC
// ═══════════════════════════════════════════════════════════════════════════
// ✅ Diagnose and fix motorcycle pairing connection issues
// ✅ Test Firebase connectivity and device registration
// ✅ Verify dashboard integration paths
// ✅ Enhanced connection stability and error reporting
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

// ✅ CRITICAL FIREBASE PATHS - These must match exactly
const String devicePath = "/helmet_public/" + userUID + "/devices/motorcycle.json";
const String helmetPath = "/helmet_public/" + userUID + "/devices/helmet.json";
const String buttonPath = "/" + userUID + "/engineControl/startButton.json";
const String livePath = "/helmet_public/" + userUID + "/live.json";

// Pin Assignments
const int relayPin = 13;
const int ignitionRelayPin = 27;
const int buzzerPin = 12;

// Connection tracking
int connectionAttempts = 0;
int successfulConnections = 0;
bool lastConnectionStatus = false;

HTTPClient http;

void setup() {
  Serial.begin(115200);
  delay(1000);
  
  Serial.println("\n╔════════════════════════════════════════════════════════════╗");
  Serial.println("║        VIGILERT MOTORCYCLE CONNECTION DIAGNOSTIC           ║");
  Serial.println("╠════════════════════════════════════════════════════════════╣");
  Serial.println("║ 🔍 Diagnose motorcycle pairing connection issues          ║");
  Serial.println("║ 🔗 Test Firebase connectivity and device registration      ║");
  Serial.println("║ 📊 Verify dashboard integration paths                     ║");
  Serial.println("║ 🛠️ Enhanced connection stability and error reporting       ║");
  Serial.println("╚════════════════════════════════════════════════════════════╝\n");

  // Initialize pins
  pinMode(relayPin, OUTPUT);
  pinMode(ignitionRelayPin, OUTPUT);
  pinMode(buzzerPin, OUTPUT);
  
  digitalWrite(relayPin, LOW);
  digitalWrite(ignitionRelayPin, LOW);
  
  Serial.println("[SETUP] Hardware initialized");
  
  // Connect to WiFi with detailed diagnostics
  connectToWiFiDiagnostic();
  
  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("\n🚀 STARTING COMPREHENSIVE FIREBASE DIAGNOSTIC...\n");
    
    // Run comprehensive diagnostic
    runFirebaseDiagnostic();
    
    Serial.println("\n✅ DIAGNOSTIC COMPLETE - Starting continuous monitoring...\n");
  } else {
    Serial.println("\n❌ WiFi connection failed - cannot proceed with Firebase diagnostic");
  }
}

void loop() {
  // Continuous connection monitoring
  if (WiFi.status() == WL_CONNECTED) {
    // Test connection every 5 seconds
    static unsigned long lastTest = 0;
    if (millis() - lastTest > 5000) {
      testMotorcycleConnection();
      lastTest = millis();
    }
    
    // Send heartbeat every 2 seconds
    static unsigned long lastHeartbeat = 0;
    if (millis() - lastHeartbeat > 2000) {
      sendDiagnosticHeartbeat();
      lastHeartbeat = millis();
    }
  } else {
    Serial.println("⚠️ WiFi disconnected! Attempting reconnection...");
    connectToWiFiDiagnostic();
    delay(5000);
  }
  
  // Handle serial commands
  if (Serial.available()) {
    String cmd = Serial.readStringUntil('\n');
    cmd.trim();
    cmd.toUpperCase();
    
    if (cmd == "TEST") {
      runFirebaseDiagnostic();
    }
    else if (cmd == "PATHS") {
      printFirebasePaths();
    }
    else if (cmd == "CONNECT") {
      testMotorcycleConnection();
    }
    else if (cmd == "HEARTBEAT") {
      sendDiagnosticHeartbeat();
    }
    else if (cmd == "STATS") {
      printConnectionStats();
    }
  }
  
  delay(100);
}

void connectToWiFiDiagnostic() {
  Serial.println("[WIFI] 🔍 DIAGNOSTIC CONNECTION ATTEMPT");
  Serial.println("[WIFI] SSID: " + String(ssid));
  Serial.println("[WIFI] Password length: " + String(strlen(password)));
  
  WiFi.mode(WIFI_STA);
  WiFi.setAutoReconnect(true);
  WiFi.begin(ssid, password);
  
  Serial.print("[WIFI] Connecting");
  int attempts = 0;
  while (WiFi.status() != WL_CONNECTED && attempts < 30) {
    delay(500);
    Serial.print(".");
    attempts++;
    
    if (attempts % 10 == 0) {
      Serial.println();
      Serial.printf("[WIFI] Attempt %d/30 - Status: %d\n", attempts, WiFi.status());
    }
  }
  
  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("\n[WIFI] ✅ CONNECTION SUCCESSFUL!");
    Serial.printf("[WIFI] IP Address: %s\n", WiFi.localIP().toString().c_str());
    Serial.printf("[WIFI] Signal Strength: %d dBm\n", WiFi.RSSI());
    Serial.printf("[WIFI] Gateway: %s\n", WiFi.gatewayIP().toString().c_str());
    Serial.printf("[WIFI] DNS: %s\n", WiFi.dnsIP().toString().c_str());
  } else {
    Serial.println("\n[WIFI] ❌ CONNECTION FAILED!");
    Serial.printf("[WIFI] Final Status: %d\n", WiFi.status());
    Serial.println("[WIFI] Status meanings:");
    Serial.println("  0 = WL_IDLE_STATUS");
    Serial.println("  1 = WL_NO_SSID_AVAIL");
    Serial.println("  3 = WL_CONNECTED");
    Serial.println("  4 = WL_CONNECT_FAILED");
    Serial.println("  6 = WL_DISCONNECTED");
  }
}

void runFirebaseDiagnostic() {
  Serial.println("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━");
  Serial.println("🔍 FIREBASE CONNECTION DIAGNOSTIC");
  Serial.println("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━");
  
  // Test 1: Basic Firebase connectivity
  Serial.println("\n📋 TEST 1: Basic Firebase Connectivity");
  testBasicFirebaseConnection();
  
  // Test 2: Device path accessibility
  Serial.println("\n📋 TEST 2: Device Path Accessibility");
  testDevicePathAccess();
  
  // Test 3: Helmet path check
  Serial.println("\n📋 TEST 3: Helmet Path Check");
  testHelmetPathAccess();
  
  // Test 4: Write motorcycle device data
  Serial.println("\n📋 TEST 4: Write Motorcycle Device Data");
  writeMotorcycleDeviceData();
  
  // Test 5: Read back motorcycle data
  Serial.println("\n📋 TEST 5: Read Back Motorcycle Data");
  readMotorcycleDeviceData();
  
  // Test 6: Dashboard button path
  Serial.println("\n📋 TEST 6: Dashboard Button Path");
  testButtonPathAccess();
  
  // Test 7: Live data path
  Serial.println("\n📋 TEST 7: Live Data Path");
  testLiveDataPath();
  
  Serial.println("\n━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━");
}

void testBasicFirebaseConnection() {
  Serial.println("🔗 Testing basic Firebase connectivity...");
  Serial.println("Host: " + firebaseHost);
  
  http.begin(firebaseHost + "/.json");
  http.setTimeout(10000);
  
  unsigned long startTime = millis();
  int httpCode = http.GET();
  unsigned long duration = millis() - startTime;
  
  Serial.printf("Response Code: %d\n", httpCode);
  Serial.printf("Response Time: %lu ms\n", duration);
  
  if (httpCode == 200 || httpCode == HTTP_CODE_OK) {
    Serial.println("✅ Firebase is accessible");
    String response = http.getString();
    Serial.printf("Response length: %d characters\n", response.length());
  } else {
    Serial.println("❌ Firebase connection failed");
    Serial.println("Error: " + http.errorToString(httpCode));
  }
  
  http.end();
}

void testDevicePathAccess() {
  Serial.println("🔗 Testing motorcycle device path...");
  Serial.println("Path: " + devicePath);
  
  http.begin(firebaseHost + devicePath);
  http.setTimeout(5000);
  
  unsigned long startTime = millis();
  int httpCode = http.GET();
  unsigned long duration = millis() - startTime;
  
  Serial.printf("Response Code: %d\n", httpCode);
  Serial.printf("Response Time: %lu ms\n", duration);
  
  if (httpCode == 200 || httpCode == HTTP_CODE_OK) {
    Serial.println("✅ Device path is accessible");
    String response = http.getString();
    Serial.println("Current data: " + response);
  } else if (httpCode == 404) {
    Serial.println("⚠️ Device path doesn't exist yet (will be created)");
  } else {
    Serial.println("❌ Device path access failed");
    Serial.println("Error: " + http.errorToString(httpCode));
  }
  
  http.end();
}

void testHelmetPathAccess() {
  Serial.println("🔗 Testing helmet device path...");
  Serial.println("Path: " + helmetPath);
  
  http.begin(firebaseHost + helmetPath);
  http.setTimeout(5000);
  
  int httpCode = http.GET();
  
  if (httpCode == 200 || httpCode == HTTP_CODE_OK) {
    Serial.println("✅ Helmet path is accessible");
    String response = http.getString();
    Serial.println("Helmet data: " + response);
    
    // Check if helmet is online
    if (response.indexOf("\"On\"") != -1) {
      Serial.println("✅ Helmet is currently ONLINE");
    } else {
      Serial.println("⚠️ Helmet appears to be OFFLINE");
    }
  } else if (httpCode == 404) {
    Serial.println("⚠️ Helmet not found - make sure helmet module is running");
  } else {
    Serial.println("❌ Helmet path access failed");
  }
  
  http.end();
}

void writeMotorcycleDeviceData() {
  Serial.println("📝 Writing motorcycle device data...");
  
  StaticJsonDocument<512> doc;
  doc["status"] = "On";
  doc["lastHeartbeat"] = (uint64_t)(1700000000000ULL + millis());
  doc["timestamp"] = (uint64_t)(1700000000000ULL + millis());
  doc["deviceType"] = "motorcycle";
  doc["version"] = "diagnostic_v1.0";
  doc["diagnosticMode"] = true;
  
  // Engine status
  doc["engineRunning"] = false;
  doc["engineAllowed"] = false;
  doc["relayState"] = digitalRead(relayPin);
  doc["ignitionState"] = digitalRead(ignitionRelayPin);
  
  // System info
  doc["freeHeap"] = ESP.getFreeHeap();
  doc["wifiSignal"] = WiFi.RSSI();
  doc["uptime"] = millis();
  doc["ipAddress"] = WiFi.localIP().toString();
  
  String payload;
  serializeJson(doc, payload);
  
  Serial.println("Payload: " + payload);
  
  http.begin(firebaseHost + devicePath);
  http.addHeader("Content-Type", "application/json");
  http.setTimeout(10000);
  
  unsigned long startTime = millis();
  int httpCode = http.PUT(payload);
  unsigned long duration = millis() - startTime;
  
  Serial.printf("Write Response Code: %d\n", httpCode);
  Serial.printf("Write Time: %lu ms\n", duration);
  
  if (httpCode == 200 || httpCode == HTTP_CODE_OK) {
    Serial.println("✅ Motorcycle device data written successfully");
    successfulConnections++;
  } else {
    Serial.println("❌ Failed to write motorcycle device data");
    Serial.println("Error: " + http.errorToString(httpCode));
  }
  
  connectionAttempts++;
  http.end();
}

void readMotorcycleDeviceData() {
  Serial.println("📖 Reading back motorcycle device data...");
  
  // Wait a moment for data to propagate
  delay(1000);
  
  http.begin(firebaseHost + devicePath);
  http.setTimeout(5000);
  
  int httpCode = http.GET();
  
  if (httpCode == 200 || httpCode == HTTP_CODE_OK) {
    Serial.println("✅ Successfully read motorcycle device data");
    String response = http.getString();
    Serial.println("Data: " + response);
    
    // Parse and verify data
    StaticJsonDocument<512> doc;
    DeserializationError error = deserializeJson(doc, response);
    
    if (!error) {
      Serial.println("✅ Data is valid JSON");
      Serial.printf("Status: %s\n", doc["status"].as<String>().c_str());
      Serial.printf("Device Type: %s\n", doc["deviceType"].as<String>().c_str());
      Serial.printf("Diagnostic Mode: %s\n", doc["diagnosticMode"].as<bool>() ? "true" : "false");
    } else {
      Serial.println("❌ Data is not valid JSON");
    }
  } else {
    Serial.println("❌ Failed to read motorcycle device data");
    Serial.printf("Error Code: %d\n", httpCode);
  }
  
  http.end();
}

void testButtonPathAccess() {
  Serial.println("🔗 Testing dashboard button path...");
  Serial.println("Path: " + buttonPath);
  
  http.begin(firebaseHost + buttonPath);
  http.setTimeout(5000);
  
  int httpCode = http.GET();
  
  if (httpCode == 200 || httpCode == HTTP_CODE_OK) {
    Serial.println("✅ Button path is accessible");
    String response = http.getString();
    Serial.println("Button state: " + response);
  } else if (httpCode == 404) {
    Serial.println("⚠️ Button path doesn't exist - will be created when needed");
  } else {
    Serial.println("❌ Button path access failed");
  }
  
  http.end();
}

void testLiveDataPath() {
  Serial.println("🔗 Testing live data path...");
  Serial.println("Path: " + livePath);
  
  // Write test live data
  StaticJsonDocument<256> liveDoc;
  liveDoc["engineRunning"] = false;
  liveDoc["engineAllowed"] = false;
  liveDoc["diagnosticMode"] = true;
  liveDoc["timestamp"] = (uint64_t)(1700000000000ULL + millis());
  
  String livePayload;
  serializeJson(liveDoc, livePayload);
  
  http.begin(firebaseHost + livePath);
  http.addHeader("Content-Type", "application/json");
  http.setTimeout(5000);
  
  int httpCode = http.PUT(livePayload);
  
  if (httpCode == 200 || httpCode == HTTP_CODE_OK) {
    Serial.println("✅ Live data path is working");
  } else {
    Serial.println("❌ Live data path failed");
    Serial.printf("Error Code: %d\n", httpCode);
  }
  
  http.end();
}

void testMotorcycleConnection() {
  connectionAttempts++;
  
  // Quick connection test
  http.begin(firebaseHost + devicePath);
  http.setTimeout(3000);
  
  int httpCode = http.GET();
  bool currentStatus = (httpCode == 200 || httpCode == HTTP_CODE_OK);
  
  if (currentStatus) {
    successfulConnections++;
    if (!lastConnectionStatus) {
      Serial.println("🔗 Motorcycle connection RESTORED");
    }
  } else {
    if (lastConnectionStatus) {
      Serial.println("⚠️ Motorcycle connection LOST");
    }
  }
  
  lastConnectionStatus = currentStatus;
  
  // Print periodic status
  if (connectionAttempts % 10 == 0) {
    printConnectionStats();
  }
  
  http.end();
}

void sendDiagnosticHeartbeat() {
  StaticJsonDocument<256> doc;
  doc["status"] = "On";
  doc["lastHeartbeat"] = (uint64_t)(1700000000000ULL + millis());
  doc["timestamp"] = (uint64_t)(1700000000000ULL + millis());
  doc["diagnosticMode"] = true;
  doc["connectionAttempts"] = connectionAttempts;
  doc["successfulConnections"] = successfulConnections;
  doc["successRate"] = connectionAttempts > 0 ? (float)successfulConnections / connectionAttempts * 100.0 : 0.0;
  
  String payload;
  serializeJson(doc, payload);
  
  http.begin(firebaseHost + devicePath);
  http.addHeader("Content-Type", "application/json");
  http.setTimeout(3000);
  
  int httpCode = http.PUT(payload);
  
  if (httpCode == 200 || httpCode == HTTP_CODE_OK) {
    Serial.printf("[HEARTBEAT] ✅ Success (Rate: %.1f%%)\n", 
                  connectionAttempts > 0 ? (float)successfulConnections / connectionAttempts * 100.0 : 0.0);
  } else {
    Serial.printf("[HEARTBEAT] ❌ Failed: HTTP %d\n", httpCode);
  }
  
  http.end();
}

void printFirebasePaths() {
  Serial.println("\n📋 FIREBASE PATHS CONFIGURATION:");
  Serial.println("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━");
  Serial.println("Firebase Host: " + firebaseHost);
  Serial.println("User UID: " + userUID);
  Serial.println();
  Serial.println("Device Path: " + devicePath);
  Serial.println("Helmet Path: " + helmetPath);
  Serial.println("Button Path: " + buttonPath);
  Serial.println("Live Path: " + livePath);
  Serial.println("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━");
}

void printConnectionStats() {
  Serial.println("\n📊 CONNECTION STATISTICS:");
  Serial.println("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━");
  Serial.printf("Total Attempts: %d\n", connectionAttempts);
  Serial.printf("Successful: %d\n", successfulConnections);
  Serial.printf("Failed: %d\n", connectionAttempts - successfulConnections);
  Serial.printf("Success Rate: %.1f%%\n", connectionAttempts > 0 ? (float)successfulConnections / connectionAttempts * 100.0 : 0.0);
  Serial.printf("Current Status: %s\n", lastConnectionStatus ? "CONNECTED ✅" : "DISCONNECTED ❌");
  Serial.printf("WiFi Status: %s\n", WiFi.status() == WL_CONNECTED ? "CONNECTED ✅" : "DISCONNECTED ❌");
  Serial.printf("Free Heap: %d bytes\n", ESP.getFreeHeap());
  Serial.printf("Uptime: %lu minutes\n", millis() / 60000);
  Serial.println("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━");
  Serial.println("📋 Commands: TEST, PATHS, CONNECT, HEARTBEAT, STATS");
  Serial.println("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━");
}