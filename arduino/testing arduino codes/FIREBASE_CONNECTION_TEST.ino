// VigiLERT Firebase Connection Test - ENHANCED
// This simplified version tests Firebase connectivity with detailed diagnostics

#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>

// WiFi Configuration
const char* ssid = "DPWH";
const char* password = "12345678900";

// Firebase Configuration
const String firebaseHost = "https://vigilance-shield-default-rtdb.firebaseio.com";
const String userUID = "MnzBjTBslZNijOkq732PE91hHa23";

// Firebase paths for testing
const String testPath = "/helmet_public/" + userUID + "/test.json";
const String devicePath = "/helmet_public/" + userUID + "/devices/motorcycle.json";
const String buttonPath = "/" + userUID + "/engineControl/startButton.json";
const String livePath = "/helmet_public/" + userUID + "/live.json";
const String alcoholPath = "/helmet_public/" + userUID + "/alcohol/status.json";

HTTPClient http;
bool engineRunning = false;

void setup() {
  Serial.begin(115200);
  delay(1000);
  
  Serial.println("\n╔════════════════════════════════════════╗");
  Serial.println("║     FIREBASE CONNECTION TEST v2.0      ║");
  Serial.println("║        Enhanced Diagnostics            ║");
  Serial.println("╚════════════════════════════════════════╝\n");

  // Show system info
  Serial.printf("ESP32 Chip ID: %llX\n", ESP.getEfuseMac());
  Serial.printf("Free Heap: %d bytes\n", ESP.getFreeHeap());
  Serial.printf("Flash Size: %d bytes\n", ESP.getFlashChipSize());
  Serial.println();

  // Connect to WiFi
  connectToWiFi();
  
  if (WiFi.status() == WL_CONNECTED) {
    // Run comprehensive Firebase tests
    runComprehensiveTests();
  } else {
    Serial.println("❌ Cannot proceed without WiFi connection");
  }
}

void loop() {
  // Continuous monitoring every 3 seconds
  static unsigned long lastCheck = 0;
  if (millis() - lastCheck > 3000) {
    
    // Check WiFi status
    if (WiFi.status() != WL_CONNECTED) {
      Serial.println("⚠️ WiFi disconnected! Attempting reconnection...");
      connectToWiFi();
      lastCheck = millis();
      return;
    }
    
    // Send heartbeat
    sendHeartbeat();
    
    // Check dashboard button
    checkDashboardButton();
    
    lastCheck = millis();
  }
  
  delay(100);
}

void connectToWiFi() {
  Serial.println("[WIFI] Connecting to: " + String(ssid));
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  
  int attempts = 0;
  while (WiFi.status() != WL_CONNECTED && attempts < 30) {
    delay(500);
    Serial.print(".");
    attempts++;
  }
  
  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("\n[WIFI] ✅ Connected!");
    Serial.printf("[WIFI] IP: %s\n", WiFi.localIP().toString().c_str());
    Serial.printf("[WIFI] Signal: %d dBm\n", WiFi.RSSI());
    Serial.printf("[WIFI] Gateway: %s\n", WiFi.gatewayIP().toString().c_str());
    Serial.printf("[WIFI] DNS: %s\n", WiFi.dnsIP().toString().c_str());
  } else {
    Serial.println("\n[WIFI] ❌ Connection failed!");
    Serial.println("[WIFI] Check network credentials and signal strength");
  }
}

void runComprehensiveTests() {
  Serial.println("\n🧪 RUNNING COMPREHENSIVE FIREBASE TESTS...\n");
  
  // Test 1: Basic connectivity
  Serial.println("📡 Test 1: Basic Firebase connectivity...");
  testBasicConnectivity();
  
  delay(1000);
  
  // Test 2: All required paths
  Serial.println("\n📂 Test 2: Testing all Firebase paths...");
  testAllPaths();
  
  delay(1000);
  
  // Test 3: Write operations
  Serial.println("\n✏️ Test 3: Testing write operations...");
  testWriteOperations();
  
  delay(1000);
  
  // Test 4: Large payload test
  Serial.println("\n📦 Test 4: Testing large payload (sensor data)...");
  testLargePayload();
  
  Serial.println("\n✅ All tests completed!");
  Serial.println("💡 Monitor the continuous heartbeat and button checks below...\n");
}

void testBasicConnectivity() {
  http.begin(firebaseHost + testPath);
  http.setTimeout(10000);
  
  int httpCode = http.GET();
  
  Serial.printf("   Firebase Host: %s\n", firebaseHost.c_str());
  Serial.printf("   Test Path: %s\n", testPath.c_str());
  Serial.printf("   HTTP Response: %d", httpCode);
  
  if (httpCode > 0) {
    String response = http.getString();
    Serial.printf(" | Length: %d bytes\n", response.length());
    Serial.printf("   Response: %s\n", response.c_str());
    
    if (httpCode == 200 || httpCode == HTTP_CODE_OK) {
      Serial.println("   ✅ Basic connectivity: SUCCESS");
    } else {
      Serial.println("   ⚠️ Unexpected response code");
    }
  } else {
    Serial.printf(" | Error: %s\n", http.errorToString(httpCode).c_str());
    Serial.println("   ❌ Basic connectivity: FAILED");
  }
  
  http.end();
}

void testAllPaths() {
  String paths[] = {
    devicePath,
    buttonPath, 
    livePath,
    alcoholPath
  };
  
  String pathNames[] = {
    "Device Status",
    "Button Control",
    "Live Data",
    "Alcohol Status"
  };
  
  for (int i = 0; i < 4; i++) {
    Serial.printf("   Testing %s: %s\n", pathNames[i].c_str(), paths[i].c_str());
    
    http.begin(firebaseHost + paths[i]);
    http.setTimeout(5000);
    
    int httpCode = http.GET();
    
    if (httpCode == 200 || httpCode == HTTP_CODE_OK) {
      Serial.printf("   ✅ %s: SUCCESS (HTTP %d)\n", pathNames[i].c_str(), httpCode);
    } else {
      Serial.printf("   ❌ %s: FAILED (HTTP %d - %s)\n", 
                    pathNames[i].c_str(), httpCode, http.errorToString(httpCode).c_str());
    }
    
    http.end();
    delay(200);
  }
}

void testWriteOperations() {
  // Test simple write
  StaticJsonDocument<200> testDoc;
  testDoc["timestamp"] = millis();
  testDoc["test"] = "write_operation";
  testDoc["status"] = "testing";
  
  String payload;
  serializeJson(testDoc, payload);
  
  Serial.printf("   Payload: %s\n", payload.c_str());
  
  http.begin(firebaseHost + testPath);
  http.addHeader("Content-Type", "application/json");
  http.setTimeout(10000);
  
  int httpCode = http.PUT(payload);
  
  if (httpCode == 200 || httpCode == HTTP_CODE_OK) {
    Serial.println("   ✅ Write operation: SUCCESS");
    String response = http.getString();
    Serial.printf("   Response: %s\n", response.c_str());
  } else {
    Serial.printf("   ❌ Write operation: FAILED (HTTP %d - %s)\n", 
                  httpCode, http.errorToString(httpCode).c_str());
  }
  
  http.end();
}

void testLargePayload() {
  // Simulate complete sensor data
  StaticJsonDocument<1024> sensorDoc;
  sensorDoc["status"] = "On";
  sensorDoc["timestamp"] = millis();
  sensorDoc["engineRunning"] = false;
  sensorDoc["engineAllowed"] = false;
  
  // MPU6050 data
  sensorDoc["mpu6050"]["accelX"] = 0.12;
  sensorDoc["mpu6050"]["accelY"] = -0.05;
  sensorDoc["mpu6050"]["accelZ"] = 9.78;
  sensorDoc["mpu6050"]["totalAccel"] = 9.8;
  sensorDoc["mpu6050"]["roll"] = 1.2;
  sensorDoc["mpu6050"]["pitch"] = -0.8;
  sensorDoc["mpu6050"]["temperature"] = 28.5;
  
  // GPS data
  sensorDoc["gps"]["valid"] = false;
  sensorDoc["gps"]["latitude"] = 0.0;
  sensorDoc["gps"]["longitude"] = 0.0;
  sensorDoc["gps"]["speed"] = 0.0;
  sensorDoc["gps"]["satellites"] = 0;
  
  // Other sensors
  sensorDoc["crashDetected"] = false;
  sensorDoc["alcoholDetected"] = false;
  sensorDoc["helmetConnected"] = false;
  sensorDoc["antiTheftArmed"] = false;
  sensorDoc["vibrationSensor"] = 0;
  
  String sensorPayload;
  serializeJson(sensorDoc, sensorPayload);
  
  Serial.printf("   Payload size: %d bytes\n", sensorPayload.length());
  
  http.begin(firebaseHost + devicePath);
  http.addHeader("Content-Type", "application/json");
  http.setTimeout(10000);
  
  unsigned long startTime = millis();
  int httpCode = http.PUT(sensorPayload);
  unsigned long duration = millis() - startTime;
  
  if (httpCode == 200 || httpCode == HTTP_CODE_OK) {
    Serial.printf("   ✅ Large payload: SUCCESS (%lu ms)\n", duration);
  } else {
    Serial.printf("   ❌ Large payload: FAILED (HTTP %d - %s, %lu ms)\n", 
                  httpCode, http.errorToString(httpCode).c_str(), duration);
  }
  
  http.end();
}

void sendHeartbeat() {
  StaticJsonDocument<400> doc;
  doc["status"] = "On";
  doc["timestamp"] = millis();
  doc["engineRunning"] = engineRunning;
  doc["test"] = true;
  doc["freeHeap"] = ESP.getFreeHeap();
  doc["wifiSignal"] = WiFi.RSSI();
  
  String payload;
  serializeJson(doc, payload);
  
  unsigned long startTime = millis();
  
  http.begin(firebaseHost + devicePath);
  http.addHeader("Content-Type", "application/json");
  http.setTimeout(5000);
  
  int httpCode = http.PUT(payload);
  unsigned long duration = millis() - startTime;
  
  if (httpCode == 200 || httpCode == HTTP_CODE_OK) {
    Serial.printf("[HEARTBEAT] ✅ Success (%lu ms) | Engine: %s | Heap: %d\n", 
                  duration, engineRunning ? "ON" : "OFF", ESP.getFreeHeap());
  } else {
    Serial.printf("[HEARTBEAT] ❌ Failed (HTTP %d - %s, %lu ms)\n", 
                  httpCode, http.errorToString(httpCode).c_str(), duration);
  }
  
  http.end();
}

void checkDashboardButton() {
  http.begin(firebaseHost + buttonPath);
  http.setTimeout(3000);
  
  int httpCode = http.GET();
  
  if (httpCode == 200 || httpCode == HTTP_CODE_OK) {
    String response = http.getString();
    bool buttonPressed = (response.indexOf("true") != -1);
    
    if (buttonPressed) {
      Serial.println("\n🖱️ DASHBOARD BUTTON DETECTED!");
      
      // Toggle engine state
      engineRunning = !engineRunning;
      Serial.printf("   Engine state changed to: %s\n", engineRunning ? "RUNNING" : "STOPPED");
      
      // Clear the button
      http.end();
      http.begin(firebaseHost + buttonPath);
      http.addHeader("Content-Type", "application/json");
      http.setTimeout(3000);
      
      int clearCode = http.PUT("false");
      if (clearCode == 200 || clearCode == HTTP_CODE_OK) {
        Serial.println("   ✅ Button cleared successfully");
      } else {
        Serial.printf("   ❌ Failed to clear button: HTTP %d\n", clearCode);
      }
      
      // Send immediate status update
      sendHeartbeat();
    }
  } else if (httpCode != -1) {  // Don't log timeouts
    Serial.printf("[BUTTON] ⚠️ HTTP %d - %s\n", httpCode, http.errorToString(httpCode).c_str());
  }
  
  http.end();
}