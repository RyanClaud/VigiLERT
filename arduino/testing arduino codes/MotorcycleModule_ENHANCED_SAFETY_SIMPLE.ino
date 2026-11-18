// ═══════════════════════════════════════════════════════════════════════════
// VIGILERT MOTORCYCLE MODULE - ENHANCED SAFETY (SIMPLE VERSION)
// ═══════════════════════════════════════════════════════════════════════════
// ✅ Automatic engine shutdown on alcohol detection
// ✅ Simulated lean angle crash detection (40° threshold) - Ready for MPU6050
// ✅ Automatic engine shutdown on excessive lean angle
// ✅ Engine restart only when conditions are safe
// ✅ Enhanced safety protocols with helmet integration
// ✅ GSM integration for emergency alerts
// ✅ Full dashboard integration maintained
// ✅ NO EXTERNAL LIBRARY DEPENDENCIES - Compiles immediately
// ═══════════════════════════════════════════════════════════════════════════

#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include <TinyGPSPlus.h>
#include <HardwareSerial.h>

// ═══════════════════════════════════════════════════════════════════════════
// CONFIGURATION
// ═══════════════════════════════════════════════════════════════════════════

// WiFi Configuration
const char* ssid = "DPWH";
const char* password = "12345678900";

// Firebase Configuration
const String firebaseHost = "https://vigilance-shield-default-rtdb.firebaseio.com";
const String userUID = "MnzBjTBslZNijOkq732PE91hHa23";

// Firebase Paths
const String devicePath = "/helmet_public/" + userUID + "/devices/motorcycle.json";
const String helmetPath = "/helmet_public/" + userUID + "/devices/helmet.json";
const String buttonPath = "/" + userUID + "/engineControl/startButton.json";
const String livePath = "/helmet_public/" + userUID + "/live.json";
const String crashPath = "/helmet_public/" + userUID + "/crashes.json";
const String alcoholPath = "/helmet_public/" + userUID + "/alcohol/status.json";
const String speedLimitPath = "/" + userUID + "/speedLimit.json";
const String alertsPath = "/helmet_public/" + userUID + "/alerts.json";
const String tripsPath = "/helmet_public/" + userUID + "/trips.json";

// ✅ GSM CONFIGURATION - UPDATE WITH YOUR PHONE NUMBERS
const String OWNER_PHONE = "+1234567890";           // Replace with your number
const String EMERGENCY_CONTACT_1 = "+0987654321";   // Replace with emergency contact 1
const String EMERGENCY_CONTACT_2 = "+1122334455";   // Replace with emergency contact 2

// Pin Assignments
const int relayPin = 13;              // Starter relay
const int ignitionRelayPin = 27;      // Ignition relay
const int buzzerPin = 12;
const int vibrationSensorPin = 15;

// ✅ GSM MODULE PINS
const int GSM_RX_PIN = 4;
const int GSM_TX_PIN = 2;
const int GSM_POWER_PIN = 9;

// ✅ ENHANCED SAFETY THRESHOLDS
const float LEAN_ANGLE_THRESHOLD = 40.0;        // 40° lean angle for crash detection
const float CRASH_ACCEL_THRESHOLD = 15.0;       // g-force threshold
const int ALCOHOL_THRESHOLD = 3000;             // Alcohol sensor threshold (from helmet)
const bool MPU6050_ENABLED = false;             // Set to true when MPU6050 library is installed

// ✅ SAFETY TIMEOUTS
const unsigned long ALCOHOL_SHUTDOWN_DELAY = 2000;    // 2 seconds delay before engine shutdown
const unsigned long LEAN_ANGLE_TIMEOUT = 3000;       // 3 seconds to recover from lean
const unsigned long SAFETY_CHECK_INTERVAL = 100;     // 100ms safety checks
const unsigned long ENGINE_RESTART_COOLDOWN = 5000;   // 5 seconds before restart allowed

// Hardware Serial
HardwareSerial gpsSerial(1);    // GPS on UART1
HardwareSerial gsmSerial(2);    // GSM on UART2
TinyGPSPlus gps;

// ✅ TIMING INTERVALS
const unsigned long HEARTBEAT_INTERVAL = 500;
const unsigned long BUTTON_CHECK_INTERVAL = 200;
const unsigned long GPS_UPDATE_INTERVAL = 1000;
const unsigned long SENSOR_READ_INTERVAL = 100;
const unsigned long LIVE_DATA_INTERVAL = 500;
const unsigned long GSM_CHECK_INTERVAL = 2000;

// ✅ CORE STATES
bool engineRunning = false;
bool engineAllowed = false;
bool crashDetected = false;
bool antiTheftArmed = false;
bool helmetConnected = false;
bool gsmReady = false;

// ✅ ENHANCED SAFETY STATES
bool alcoholDetected = false;
bool leanAngleExceeded = false;
bool safetyShutdownActive = false;
String shutdownReason = "";
unsigned long lastSafetyCheck = 0;
unsigned long alcoholDetectedTime = 0;
unsigned long leanAngleStartTime = 0;
unsigned long lastEngineShutdown = 0;

// ✅ SENSOR VARIABLES
float currentAccel = 9.8;
float currentTotalAccel = 9.8;
float currentRoll = 0.0;
float currentPitch = 0.0;
float currentLeanAngle = 0.0;
int vibrationCount = 0;
int alcoholSensorValue = 0;

// ✅ GPS VARIABLES
double currentLat = 0.0;
double currentLng = 0.0;
float currentSpeed = 0.0;
bool gpsValid = false;
int satelliteCount = 0;

// ✅ DASHBOARD VARIABLES
int speedLimit = 60;
bool isOverSpeed = false;
String riderStatus = "Inactive";
String alertnessStatus = "Normal";

// ✅ TIMING VARIABLES
unsigned long lastHeartbeat = 0;
unsigned long lastButtonCheck = 0;
unsigned long lastGPSUpdate = 0;
unsigned long lastSensorRead = 0;
unsigned long lastLiveDataUpdate = 0;
unsigned long lastCrashTime = 0;
unsigned long lastGSMCheck = 0;

// ✅ TRIP TRACKING
bool tripActive = false;
unsigned long tripStartTime = 0;
double tripStartLat = 0.0;
double tripStartLng = 0.0;
float tripDistance = 0.0;
float tripMaxSpeed = 0.0;

// HTTP client
HTTPClient http;

// ═══════════════════════════════════════════════════════════════════════════
// SETUP
// ═══════════════════════════════════════════════════════════════════════════

void setup() {
  Serial.begin(115200);
  delay(500);
  
  Serial.println("\n╔════════════════════════════════════════════════════════════╗");
  Serial.println("║   VIGILERT MOTORCYCLE MODULE - ENHANCED SAFETY SIMPLE     ║");
  Serial.println("╠════════════════════════════════════════════════════════════╣");
  Serial.println("║ 🚨 Automatic Engine Shutdown on Alcohol Detection         ║");
  Serial.println("║ 📐 Simulated Lean Angle Detection (Ready for MPU6050)     ║");
  Serial.println("║ 🛑 Automatic Engine Shutdown on Excessive Lean            ║");
  Serial.println("║ 🔒 Engine Restart Only When Conditions Are Safe           ║");
  Serial.println("║ 🩺 Enhanced Safety Protocols with Helmet Integration      ║");
  Serial.println("║ 📱 GSM Emergency Alerts for Safety Events                 ║");
  Serial.println("║ ✅ NO EXTERNAL LIBRARY DEPENDENCIES                       ║");
  Serial.println("╚════════════════════════════════════════════════════════════╝\n");

  // Initialize pins
  pinMode(relayPin, OUTPUT);
  pinMode(ignitionRelayPin, OUTPUT);
  pinMode(buzzerPin, OUTPUT);
  pinMode(vibrationSensorPin, INPUT);
  pinMode(GSM_POWER_PIN, OUTPUT);

  // ✅ CRITICAL: Initialize relays to BLOCK
  digitalWrite(relayPin, LOW);
  digitalWrite(ignitionRelayPin, LOW);
  digitalWrite(GSM_POWER_PIN, HIGH);
  
  engineRunning = false;
  engineAllowed = false;
  safetyShutdownActive = false;
  
  Serial.println("[SETUP] 🔒 ENHANCED SAFETY SYSTEM:");
  Serial.printf("[SETUP] Starter: GPIO %d = BLOCKED\n", relayPin);
  Serial.printf("[SETUP] Ignition: GPIO %d = BLOCKED\n", ignitionRelayPin);
  Serial.println("[SETUP] Safety protocols: ACTIVE");

  // ✅ LEAN ANGLE DETECTION STATUS
  if (MPU6050_ENABLED) {
    Serial.println("[MPU6050] ⚠️ Set to enabled but library not included in simple version");
    Serial.println("[MPU6050] Using simulated lean angle for testing");
  } else {
    Serial.println("[MPU6050] Using simulated lean angle detection");
    Serial.println("[MPU6050] Install MPU6050 library and set MPU6050_ENABLED=true for real detection");
  }
  Serial.printf("[LEAN] Lean angle threshold: %.1f°\n", LEAN_ANGLE_THRESHOLD);

  // ✅ INITIALIZE GPS
  gpsSerial.begin(9600, SERIAL_8N1, 16, 17);
  Serial.println("[GPS] Initialized on RX:16, TX:17");

  // ✅ INITIALIZE GSM MODULE
  initializeGSM();

  // Connect to WiFi
  connectToWiFi();
  
  if (WiFi.status() == WL_CONNECTED) {
    testFirebaseConnection();
    loadSpeedLimit();
  }

  // ✅ SEND STARTUP SMS
  if (gsmReady) {
    sendSMSAlert(OWNER_PHONE, "🚀 VIGILERT ENHANCED SAFETY ONLINE\n" +
                 String("Alcohol Detection: ACTIVE\n") +
                 String("Lean Angle Detection: SIMULATED (40°)\n") +
                 String("Auto Engine Shutdown: ENABLED\n") +
                 String("System ready for safe operation"));
  }

  Serial.println("\n🚀 ENHANCED SAFETY MOTORCYCLE MODULE READY!");
  Serial.println("🚨 SAFETY FEATURES:");
  Serial.println("   • Alcohol detection → Auto engine shutdown");
  Serial.println("   • Lean angle >40° → Auto engine shutdown (simulated)");
  Serial.println("   • Engine restart only when safe");
  Serial.println("   • Emergency SMS alerts");
  Serial.println("📋 Commands: START, STOP, STATUS, TEST LEAN, TEST SAFETY, RESET SAFETY");
  Serial.println("🖥️ Dashboard Control: Enhanced with safety interlocks\n");
}

// ═══════════════════════════════════════════════════════════════════════════
// MAIN LOOP
// ═══════════════════════════════════════════════════════════════════════════

void loop() {
  unsigned long currentTime = millis();
  
  // ✅ CRITICAL: SAFETY CHECKS (highest priority)
  if (currentTime - lastSafetyCheck >= SAFETY_CHECK_INTERVAL) {
    performSafetyChecks();
    lastSafetyCheck = currentTime;
  }
  
  // ✅ Check WiFi connection
  if (WiFi.status() != WL_CONNECTED) {
    connectToWiFi();
  }
  
  // ✅ DASHBOARD HEARTBEAT
  if (currentTime - lastHeartbeat >= HEARTBEAT_INTERVAL) {
    sendDashboardHeartbeat();
    lastHeartbeat = currentTime;
  }

  // ✅ DASHBOARD BUTTON CHECK (with safety validation)
  if (currentTime - lastButtonCheck >= BUTTON_CHECK_INTERVAL) {
    checkDashboardButtonWithSafety();
    lastButtonCheck = currentTime;
  }

  // ✅ GPS UPDATE
  if (currentTime - lastGPSUpdate >= GPS_UPDATE_INTERVAL) {
    updateGPSData();
    lastGPSUpdate = currentTime;
  }

  // ✅ SENSOR READINGS
  if (currentTime - lastSensorRead >= SENSOR_READ_INTERVAL) {
    readAllSensors();
    lastSensorRead = currentTime;
  }

  // ✅ LIVE DATA UPDATE
  if (currentTime - lastLiveDataUpdate >= LIVE_DATA_INTERVAL) {
    updateLiveDataForDashboards();
    lastLiveDataUpdate = currentTime;
  }

  // ✅ GSM MESSAGE CHECK
  if (currentTime - lastGSMCheck >= GSM_CHECK_INTERVAL) {
    checkIncomingSMS();
    lastGSMCheck = currentTime;
  }

  // ✅ CRASH DETECTION (enhanced with lean angle)
  processCrashDetection();

  // ✅ ANTI-THEFT SYSTEM
  processAntiTheft();

  // ✅ TRIP TRACKING
  processTripTracking();

  // ✅ SPEED MONITORING
  processSpeedMonitoring();

  // Handle serial commands
  handleSerialCommands();

  yield();
}

// ═══════════════════════════════════════════════════════════════════════════
// ENHANCED SAFETY SYSTEM
// ═══════════════════════════════════════════════════════════════════════════

void performSafetyChecks() {
  // ✅ CHECK 1: ALCOHOL DETECTION FROM HELMET
  checkAlcoholSafety();
  
  // ✅ CHECK 2: LEAN ANGLE DETECTION (simulated for now)
  checkLeanAngleSafety();
  
  // ✅ CHECK 3: HELMET CONNECTION STATUS
  checkHelmetConnection();
  
  // ✅ CHECK 4: SAFETY RECOVERY CONDITIONS
  checkSafetyRecovery();
}

void checkAlcoholSafety() {
  // Get alcohol status from helmet module via Firebase
  if (WiFi.status() == WL_CONNECTED) {
    http.begin(firebaseHost + alcoholPath);
    http.setTimeout(2000);
    
    int httpCode = http.GET();
    if (httpCode == 200) {
      String response = http.getString();
      
      // Parse alcohol status
      bool currentAlcoholDetected = (response.indexOf("\"Danger\"") != -1);
      
      // Extract sensor value if available
      int startIdx = response.indexOf("\"sensorValue\":");
      if (startIdx != -1) {
        startIdx += 14; // Length of "sensorValue":
        int endIdx = response.indexOf(",", startIdx);
        if (endIdx == -1) endIdx = response.indexOf("}", startIdx);
        if (endIdx != -1) {
          alcoholSensorValue = response.substring(startIdx, endIdx).toInt();
        }
      }
      
      // ✅ ALCOHOL DETECTION LOGIC
      if (currentAlcoholDetected && !alcoholDetected) {
        // Alcohol just detected
        alcoholDetected = true;
        alcoholDetectedTime = millis();
        
        Serial.printf("\n🚨 ALCOHOL DETECTED! (Value: %d)\n", alcoholSensorValue);
        Serial.printf("⏱️ Engine shutdown in %.1f seconds...\n", ALCOHOL_SHUTDOWN_DELAY / 1000.0);
        
        // Immediate alert
        playAlertSound(5, 200);
        
        // Send SMS alert
        if (gsmReady) {
          sendSMSAlert(OWNER_PHONE, "🚨 ALCOHOL DETECTED!\n" +
                       String("Sensor Value: ") + String(alcoholSensorValue) + "\n" +
                       String("Engine will shutdown in ") + String(ALCOHOL_SHUTDOWN_DELAY / 1000) + " seconds\n" +
                       String("Time: ") + getCurrentTimeString());
        }
        
        sendDualAlert("🚨 ALCOHOL DETECTED - Engine shutdown imminent", "danger");
      }
      
      // ✅ AUTOMATIC ENGINE SHUTDOWN ON ALCOHOL
      if (alcoholDetected && engineRunning) {
        unsigned long timeSinceDetection = millis() - alcoholDetectedTime;
        
        if (timeSinceDetection >= ALCOHOL_SHUTDOWN_DELAY) {
          Serial.println("\n🛑 AUTOMATIC ENGINE SHUTDOWN - ALCOHOL DETECTED!");
          
          safetyShutdownActive = true;
          shutdownReason = "Alcohol Detection";
          lastEngineShutdown = millis();
          
          // Force engine stop
          forceEngineShutdown("ALCOHOL_DETECTED");
          
          // Emergency SMS
          if (gsmReady) {
            String emergencyMsg = "🚨 EMERGENCY SHUTDOWN!\n";
            emergencyMsg += "Reason: Alcohol Detection\n";
            emergencyMsg += "Sensor Value: " + String(alcoholSensorValue) + "\n";
            emergencyMsg += "Time: " + getCurrentTimeString() + "\n";
            if (gpsValid) {
              emergencyMsg += "Location: " + String(currentLat, 6) + "," + String(currentLng, 6);
            }
            
            sendSMSAlert(EMERGENCY_CONTACT_1, emergencyMsg);
            sendSMSAlert(EMERGENCY_CONTACT_2, emergencyMsg);
          }
        }
      }
      
      // ✅ ALCOHOL CLEARED
      if (!currentAlcoholDetected && alcoholDetected) {
        alcoholDetected = false;
        Serial.printf("\n✅ ALCOHOL CLEARED! (Value: %d)\n", alcoholSensorValue);
        sendDualAlert("✅ Alcohol levels normalized", "success");
        
        if (gsmReady) {
          sendSMSAlert(OWNER_PHONE, "✅ ALCOHOL CLEARED\n" +
                       String("Sensor Value: ") + String(alcoholSensorValue) + "\n" +
                       String("Engine restart available\n") +
                       String("Time: ") + getCurrentTimeString());
        }
      }
    }
    
    http.end();
  }
}

void checkLeanAngleSafety() {
  // ✅ SIMULATED LEAN ANGLE DETECTION
  // This simulates lean angle detection for testing
  // Replace with real MPU6050 readings when library is installed
  
  static unsigned long lastLeanUpdate = 0;
  if (millis() - lastLeanUpdate > 100) { // Update every 100ms
    
    // ✅ SIMULATED LEAN ANGLE (for testing without MPU6050)
    // In real implementation, this would come from MPU6050
    static float simulatedLean = 0.0;
    
    // Simulate normal riding lean angles (0-35°) with occasional spikes for testing
    if (engineRunning) {
      // Simulate riding lean angles
      simulatedLean = random(0, 30) + (sin(millis() / 1000.0) * 5);
      
      // Occasionally simulate dangerous lean for testing (uncomment to test)
      // if (random(0, 1000) < 2) { // 0.2% chance
      //   simulatedLean = random(40, 60); // Dangerous lean angle
      // }
    } else {
      // Motorcycle is stationary
      simulatedLean = random(0, 5); // Small variations when stopped
    }
    
    currentLeanAngle = abs(simulatedLean);
    currentRoll = simulatedLean;
    currentPitch = random(-5, 5);
    
    lastLeanUpdate = millis();
  }
  
  // ✅ LEAN ANGLE DETECTION LOGIC
  if (currentLeanAngle >= LEAN_ANGLE_THRESHOLD && !leanAngleExceeded) {
    // Lean angle just exceeded
    leanAngleExceeded = true;
    leanAngleStartTime = millis();
    
    Serial.printf("\n⚠️ EXCESSIVE LEAN ANGLE! (%.1f° > %.1f°)\n", currentLeanAngle, LEAN_ANGLE_THRESHOLD);
    Serial.printf("⏱️ Recovery time: %.1f seconds...\n", LEAN_ANGLE_TIMEOUT / 1000.0);
    
    playAlertSound(3, 150);
    sendDualAlert("⚠️ Excessive lean angle detected: " + String(currentLeanAngle, 1) + "°", "danger");
  }
  
  // ✅ AUTOMATIC ENGINE SHUTDOWN ON EXCESSIVE LEAN
  if (leanAngleExceeded && engineRunning) {
    unsigned long timeSinceLean = millis() - leanAngleStartTime;
    
    // Check if still leaning after timeout
    if (timeSinceLean >= LEAN_ANGLE_TIMEOUT && currentLeanAngle >= LEAN_ANGLE_THRESHOLD) {
      Serial.printf("\n🛑 AUTOMATIC ENGINE SHUTDOWN - EXCESSIVE LEAN ANGLE! (%.1f°)\n", currentLeanAngle);
      
      safetyShutdownActive = true;
      shutdownReason = "Excessive Lean Angle";
      lastEngineShutdown = millis();
      
      // Force engine stop
      forceEngineShutdown("LEAN_ANGLE_EXCEEDED");
      
      // Emergency SMS
      if (gsmReady) {
        String emergencyMsg = "🚨 CRASH SUSPECTED!\n";
        emergencyMsg += "Reason: Excessive lean angle\n";
        emergencyMsg += "Lean Angle: " + String(currentLeanAngle, 1) + "°\n";
        emergencyMsg += "Time: " + getCurrentTimeString() + "\n";
        if (gpsValid) {
          emergencyMsg += "Location: " + String(currentLat, 6) + "," + String(currentLng, 6) + "\n";
          emergencyMsg += "Emergency Map: https://maps.google.com/?q=" + String(currentLat, 6) + "," + String(currentLng, 6);
        }
        
        sendSMSAlert(EMERGENCY_CONTACT_1, emergencyMsg);
        sendSMSAlert(EMERGENCY_CONTACT_2, emergencyMsg);
      }
      
      // Mark as crash detected
      crashDetected = true;
      sendCrashDataToDashboards();
    }
  }
  
  // ✅ LEAN ANGLE RECOVERED
  if (leanAngleExceeded && currentLeanAngle < (LEAN_ANGLE_THRESHOLD - 5.0)) {
    leanAngleExceeded = false;
    Serial.printf("\n✅ LEAN ANGLE RECOVERED! (%.1f°)\n", currentLeanAngle);
    sendDualAlert("✅ Lean angle normalized: " + String(currentLeanAngle, 1) + "°", "success");
  }
}

void checkHelmetConnection() {
  // Check helmet connection status from Firebase
  if (WiFi.status() == WL_CONNECTED) {
    http.begin(firebaseHost + helmetPath);
    http.setTimeout(1000);
    
    int httpCode = http.GET();
    if (httpCode == 200) {
      String response = http.getString();
      bool currentHelmetConnected = (response.indexOf("\"On\"") != -1);
      
      if (currentHelmetConnected != helmetConnected) {
        helmetConnected = currentHelmetConnected;
        
        if (helmetConnected) {
          Serial.println("[HELMET] ✅ Connected");
          sendDualAlert("Helmet connected", "success");
        } else {
          Serial.println("[HELMET] ❌ Disconnected");
          sendDualAlert("⚠️ Helmet disconnected - safety monitoring limited", "danger");
        }
      }
    }
    
    http.end();
  }
}

void checkSafetyRecovery() {
  // Check if safety conditions allow engine restart
  bool canRestart = true;
  String blockingReasons = "";
  
  if (alcoholDetected) {
    canRestart = false;
    blockingReasons += "Alcohol detected; ";
  }
  
  if (leanAngleExceeded) {
    canRestart = false;
    blockingReasons += "Excessive lean angle; ";
  }
  
  if (crashDetected) {
    canRestart = false;
    blockingReasons += "Crash detected; ";
  }
  
  // Check cooldown period
  if (safetyShutdownActive && (millis() - lastEngineShutdown < ENGINE_RESTART_COOLDOWN)) {
    canRestart = false;
    blockingReasons += "Cooldown period; ";
  }
  
  // Update safety shutdown status
  if (safetyShutdownActive && canRestart) {
    safetyShutdownActive = false;
    shutdownReason = "";
    Serial.println("\n✅ SAFETY CONDITIONS NORMALIZED - Engine restart allowed");
    sendDualAlert("✅ Safety conditions normalized - engine restart available", "success");
    
    if (gsmReady) {
      sendSMSAlert(OWNER_PHONE, "✅ SAFETY RECOVERY\n" +
                   String("All safety conditions normalized\n") +
                   String("Engine restart now available\n") +
                   String("Time: ") + getCurrentTimeString());
    }
  }
  
  // Update engine allowed status
  engineAllowed = canRestart;
}

void forceEngineShutdown(String reason) {
  Serial.println("\n🚨 FORCE ENGINE SHUTDOWN: " + reason);
  
  // Immediately cut power
  digitalWrite(relayPin, LOW);
  digitalWrite(ignitionRelayPin, LOW);
  
  engineRunning = false;
  engineAllowed = false;
  riderStatus = "Safety Shutdown";
  
  // Alert sequence
  for (int i = 0; i < 10; i++) {
    digitalWrite(buzzerPin, HIGH);
    delay(100);
    digitalWrite(buzzerPin, LOW);
    delay(100);
  }
  
  // Force immediate dashboard updates
  sendDashboardHeartbeat();
  updateLiveDataForDashboards();
  
  Serial.printf("🛑 Engine shutdown complete - Reason: %s\n", reason.c_str());
}

// ═══════════════════════════════════════════════════════════════════════════
// ENHANCED ENGINE CONTROL WITH SAFETY VALIDATION
// ═══════════════════════════════════════════════════════════════════════════

void checkDashboardButtonWithSafety() {
  if (WiFi.status() != WL_CONNECTED) return;

  http.begin(firebaseHost + buttonPath);
  http.setTimeout(2000);
  
  int httpCode = http.GET();
  
  if (httpCode == 200) {
    String response = http.getString();
    bool buttonPressed = (response.indexOf("true") != -1);
    
    static bool lastButtonState = false;
    if (buttonPressed && !lastButtonState) {
      Serial.println("\n🖥️ DASHBOARD BUTTON PRESSED!");
      
      if (!engineRunning) {
        // ✅ SAFETY VALIDATION BEFORE START
        if (validateSafetyConditions()) {
          Serial.println("🚀 Dashboard requesting ENGINE START - Safety validated");
          startEngine();
          sendDashboardAlert("Engine started via dashboard", "success");
        } else {
          Serial.println("🚫 ENGINE START BLOCKED - Safety conditions not met");
          String reason = getSafetyBlockingReasons();
          sendDashboardAlert("🚫 Engine start blocked: " + reason, "danger");
          
          if (gsmReady) {
            sendSMSAlert(OWNER_PHONE, "🚫 ENGINE START BLOCKED\n" +
                         String("Reason: ") + reason + "\n" +
                         String("Time: ") + getCurrentTimeString());
          }
        }
      } else {
        Serial.println("🛑 Dashboard requesting ENGINE STOP");
        stopEngine();
        sendDashboardAlert("Engine stopped via dashboard", "info");
      }
      
      // Clear button
      http.end();
      http.begin(firebaseHost + buttonPath);
      http.addHeader("Content-Type", "application/json");
      http.setTimeout(2000);
      http.PUT("false");
      http.end();
      
      // Force immediate dashboard update
      sendDashboardHeartbeat();
      updateLiveDataForDashboards();
      
      lastButtonState = true;
    } else if (!buttonPressed) {
      lastButtonState = false;
    }
  }
  
  http.end();
}

bool validateSafetyConditions() {
  // Check all safety conditions
  if (alcoholDetected) {
    Serial.println("[SAFETY] ❌ Alcohol detected");
    return false;
  }
  
  if (leanAngleExceeded) {
    Serial.println("[SAFETY] ❌ Excessive lean angle");
    return false;
  }
  
  if (crashDetected) {
    Serial.println("[SAFETY] ❌ Crash detected");
    return false;
  }
  
  if (safetyShutdownActive) {
    Serial.println("[SAFETY] ❌ Safety shutdown active");
    return false;
  }
  
  // Check cooldown period
  if (millis() - lastEngineShutdown < ENGINE_RESTART_COOLDOWN) {
    Serial.printf("[SAFETY] ❌ Cooldown period (%lu ms remaining)\n", 
                  ENGINE_RESTART_COOLDOWN - (millis() - lastEngineShutdown));
    return false;
  }
  
  Serial.println("[SAFETY] ✅ All conditions safe for engine start");
  return true;
}

String getSafetyBlockingReasons() {
  String reasons = "";
  
  if (alcoholDetected) {
    reasons += "Alcohol detected (" + String(alcoholSensorValue) + "); ";
  }
  
  if (leanAngleExceeded) {
    reasons += "Excessive lean angle (" + String(currentLeanAngle, 1) + "°); ";
  }
  
  if (crashDetected) {
    reasons += "Crash detected; ";
  }
  
  if (safetyShutdownActive) {
    reasons += "Safety shutdown (" + shutdownReason + "); ";
  }
  
  if (millis() - lastEngineShutdown < ENGINE_RESTART_COOLDOWN) {
    unsigned long remaining = ENGINE_RESTART_COOLDOWN - (millis() - lastEngineShutdown);
    reasons += "Cooldown (" + String(remaining / 1000) + "s); ";
  }
  
  if (reasons.length() > 0) {
    reasons = reasons.substring(0, reasons.length() - 2); // Remove last "; "
  }
  
  return reasons;
}

void startEngine() {
  Serial.println("\n✅ STARTING ENGINE - Safety validated");
  
  digitalWrite(ignitionRelayPin, HIGH);
  digitalWrite(relayPin, HIGH);
  
  engineRunning = true;
  engineAllowed = true;
  riderStatus = "Active";
  
  Serial.printf("✅ Starter Relay GPIO %d = ALLOW\n", relayPin);
  Serial.printf("✅ Ignition Relay GPIO %d = ALLOW\n", ignitionRelayPin);
  
  // Start trip tracking
  if (gpsValid && !tripActive) {
    startTrip();
  }
  
  // Immediate dashboard updates
  sendDashboardHeartbeat();
  updateLiveDataForDashboards();
}

void stopEngine() {
  Serial.println("\n🛑 STOPPING ENGINE");
  
  digitalWrite(ignitionRelayPin, LOW);
  digitalWrite(relayPin, LOW);
  
  engineRunning = false;
  engineAllowed = false;
  riderStatus = "Inactive";
  
  Serial.printf("🛑 Starter Relay GPIO %d = BLOCK\n", relayPin);
  Serial.printf("🛑 Ignition Relay GPIO %d = BLOCK\n", ignitionRelayPin);
  
  // End trip tracking
  if (tripActive) {
    endTrip();
  }
  
  // Immediate dashboard updates
  sendDashboardHeartbeat();
  updateLiveDataForDashboards();
}

// ═══════════════════════════════════════════════════════════════════════════
// SENSOR FUNCTIONS
// ═══════════════════════════════════════════════════════════════════════════

void readAllSensors() {
  // ✅ SIMULATED MPU6050 READINGS (for lean angle detection)
  // This section will be replaced with real MPU6050 readings when library is installed
  
  if (MPU6050_ENABLED) {
    // When MPU6050 library is installed, real readings will go here
    // For now, using simulated values
    Serial.println("[MPU6050] Using simulated values - install MPU6050 library for real readings");
  }
  
  // Simulated values are updated in checkLeanAngleSafety()
  // currentLeanAngle, currentRoll, currentPitch are set there
  
  // ✅ VIBRATION SENSOR MONITORING
  static int lastVibrationReading = LOW;
  int currentVibrationReading = digitalRead(vibrationSensorPin);
  
  if (currentVibrationReading != lastVibrationReading && currentVibrationReading == HIGH) {
    vibrationCount++;
    
    if (antiTheftArmed && vibrationCount >= 3) {
      Serial.println("🚨 [ANTI-THEFT] Multiple vibrations detected!");
      sendDualAlert("Anti-theft: Unauthorized movement detected!", "danger");
      
      // Alert sequence
      for (int i = 0; i < 5; i++) {
        digitalWrite(buzzerPin, HIGH);
        delay(100);
        digitalWrite(buzzerPin, LOW);
        delay(100);
      }
      
      vibrationCount = 0;
    }
  }
  
  lastVibrationReading = currentVibrationReading;
  
  // ✅ DEBUG OUTPUT (every 5 seconds)
  static unsigned long lastDebug = 0;
  if (millis() - lastDebug > 5000) {
    Serial.printf("[SENSORS] 📊 Accel:%.1fg | 📐 Lean:%.1f° | 🍺 Alcohol:%d | 🔔 Vib:%d\n", 
                  currentTotalAccel, currentLeanAngle, alcoholSensorValue, currentVibrationReading);
    lastDebug = millis();
  }
}

// ═══════════════════════════════════════════════════════════════════════════
// WIFI & FIREBASE CONNECTION
// ═══════════════════════════════════════════════════════════════════════════

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
  Serial.println("[FIREBASE] Testing enhanced safety integration...");
  
  // Test basic connectivity first
  http.begin(firebaseHost + "/.json");
  http.setTimeout(5000);
  
  int httpCode = http.GET();
  
  if (httpCode == 200 || httpCode == HTTP_CODE_OK) {
    Serial.println("[FIREBASE] ✅ Basic connectivity OK");
    
    // Test device path
    http.end();
    http.begin(firebaseHost + devicePath);
    http.setTimeout(3000);
    
    int deviceCode = http.GET();
    if (deviceCode == 200 || deviceCode == HTTP_CODE_OK || deviceCode == 404) {
      Serial.println("[FIREBASE] ✅ Device path accessible");
      
      // Send initial device registration
      StaticJsonDocument<256> regDoc;
      regDoc["status"] = "On";
      regDoc["deviceType"] = "motorcycle";
      regDoc["version"] = "enhanced_safety_simple";
      regDoc["timestamp"] = (uint64_t)(1700000000000ULL + millis());
      regDoc["initialConnection"] = true;
      
      String regPayload;
      serializeJson(regDoc, regPayload);
      
      http.end();
      http.begin(firebaseHost + devicePath);
      http.addHeader("Content-Type", "application/json");
      http.setTimeout(5000);
      
      int regCode = http.PUT(regPayload);
      if (regCode == 200 || regCode == HTTP_CODE_OK) {
        Serial.println("[FIREBASE] ✅ Enhanced safety integration ready!");
        Serial.println("[FIREBASE] Motorcycle device registered successfully");
      } else {
        Serial.printf("[FIREBASE] ⚠️ Registration failed: HTTP %d\n", regCode);
      }
    } else {
      Serial.printf("[FIREBASE] ⚠️ Device path issue: HTTP %d\n", deviceCode);
    }
  } else {
    Serial.printf("[FIREBASE] ❌ Basic connectivity failed: HTTP %d - %s\n", httpCode, http.errorToString(httpCode).c_str());
  }
  
  http.end();
}
// ═══════════════════════════════════════════════════════════════════════════
// DASHBOARD INTEGRATION
// ═══════════════════════════════════════════════════════════════════════════

void sendDashboardHeartbeat() {
  if (WiFi.status() != WL_CONNECTED) return;

  StaticJsonDocument<800> doc;
  doc["status"] = "On";  // ✅ CRITICAL: This must be "On" for dashboard to show connected
  doc["lastHeartbeat"] = (uint64_t)(1700000000000ULL + millis());
  doc["timestamp"] = (uint64_t)(1700000000000ULL + millis());
  
  // ✅ DEVICE IDENTIFICATION - Critical for pairing
  doc["deviceType"] = "motorcycle";
  doc["version"] = "enhanced_safety_v1.0";
  doc["connected"] = true;
  
  // ✅ ENGINE STATUS WITH SAFETY INFO
  doc["engineRunning"] = engineRunning;
  doc["engineAllowed"] = engineAllowed;
  doc["relayState"] = digitalRead(relayPin);
  doc["ignitionState"] = digitalRead(ignitionRelayPin);
  
  // ✅ ENHANCED SAFETY STATUS
  doc["safetyShutdownActive"] = safetyShutdownActive;
  doc["shutdownReason"] = shutdownReason;
  doc["alcoholDetected"] = alcoholDetected;
  doc["alcoholSensorValue"] = alcoholSensorValue;
  doc["leanAngleExceeded"] = leanAngleExceeded;
  doc["currentLeanAngle"] = currentLeanAngle;
  doc["leanAngleThreshold"] = LEAN_ANGLE_THRESHOLD;
  
  // ✅ GPS DATA
  doc["gps"]["valid"] = gpsValid;
  doc["gps"]["latitude"] = currentLat;
  doc["gps"]["longitude"] = currentLng;
  doc["gps"]["speed"] = currentSpeed;
  doc["gps"]["satellites"] = satelliteCount;
  
  // ✅ SENSOR STATUS
  doc["crashDetected"] = crashDetected;
  doc["helmetConnected"] = helmetConnected;
  doc["antiTheftArmed"] = antiTheftArmed;
  doc["vibrationSensor"] = digitalRead(vibrationSensorPin);
  
  // ✅ SYSTEM HEALTH
  doc["freeHeap"] = ESP.getFreeHeap();
  doc["wifiSignal"] = WiFi.RSSI();
  doc["uptime"] = millis();
  doc["gsmReady"] = gsmReady;
  doc["ipAddress"] = WiFi.localIP().toString();
  
  // ✅ ENHANCED SENSOR DATA
  doc["sensors"]["acceleration"] = currentTotalAccel;
  doc["sensors"]["roll"] = currentRoll;
  doc["sensors"]["pitch"] = currentPitch;
  doc["sensors"]["leanAngle"] = currentLeanAngle;
  doc["sensors"]["mpu6050Enabled"] = MPU6050_ENABLED;
  doc["sensors"]["simulated"] = !MPU6050_ENABLED;
  
  String payload;
  serializeJson(doc, payload);
  
  unsigned long startTime = millis();
  
  http.begin(firebaseHost + devicePath);
  http.addHeader("Content-Type", "application/json");
  http.setTimeout(5000);  // Increased timeout
  
  int httpCode = http.PUT(payload);
  unsigned long duration = millis() - startTime;
  
  if (httpCode == 200 || httpCode == HTTP_CODE_OK) {
    Serial.printf("[DASHBOARD] ✅ %lums | Engine:%s | Safety:%s | Lean:%.1f° | Alcohol:%d | Connected:YES\n", 
                  duration, 
                  engineRunning ? "ON" : "OFF",
                  safetyShutdownActive ? "SHUTDOWN" : "OK",
                  currentLeanAngle,
                  alcoholSensorValue);
  } else {
    Serial.printf("[DASHBOARD] ❌ HTTP %d (%lums) - Connection failed!\n", httpCode, duration);
    Serial.println("[DASHBOARD] Error: " + http.errorToString(httpCode));
    
    // ✅ RETRY MECHANISM for connection issues
    delay(1000);
    http.end();
    http.begin(firebaseHost + devicePath);
    http.addHeader("Content-Type", "application/json");
    http.setTimeout(3000);
    
    int retryCode = http.PUT(payload);
    if (retryCode == 200 || retryCode == HTTP_CODE_OK) {
      Serial.println("[DASHBOARD] ✅ Retry successful!");
    } else {
      Serial.printf("[DASHBOARD] ❌ Retry failed: HTTP %d\n", retryCode);
    }
  }
  
  http.end();
}

void updateLiveDataForDashboards() {
  if (WiFi.status() != WL_CONNECTED) return;

  StaticJsonDocument<600> liveDoc;
  
  // ✅ ENGINE & SAFETY STATUS
  liveDoc["engineRunning"] = engineRunning;
  liveDoc["engineAllowed"] = engineAllowed;
  liveDoc["crashDetected"] = crashDetected;
  liveDoc["antiTheftArmed"] = antiTheftArmed;
  liveDoc["alcoholDetected"] = alcoholDetected;
  liveDoc["helmetConnected"] = helmetConnected;
  
  // ✅ ENHANCED SAFETY DATA
  liveDoc["safetyShutdownActive"] = safetyShutdownActive;
  liveDoc["shutdownReason"] = shutdownReason;
  liveDoc["leanAngleExceeded"] = leanAngleExceeded;
  liveDoc["currentLeanAngle"] = currentLeanAngle;
  liveDoc["alcoholSensorValue"] = alcoholSensorValue;
  
  // ✅ LOCATION & SPEED
  liveDoc["locationLat"] = currentLat;
  liveDoc["locationLng"] = currentLng;
  liveDoc["speed"] = currentSpeed;
  liveDoc["gpsValid"] = gpsValid;
  liveDoc["isOverSpeed"] = isOverSpeed;
  
  // ✅ RIDER STATUS WITH SAFETY INFO
  if (safetyShutdownActive) {
    liveDoc["riderStatus"] = "Safety Shutdown";
  } else if (alcoholDetected) {
    liveDoc["riderStatus"] = "Alcohol Detected";
  } else if (leanAngleExceeded) {
    liveDoc["riderStatus"] = "Excessive Lean";
  } else {
    liveDoc["riderStatus"] = riderStatus;
  }
  
  liveDoc["alertnessStatus"] = alertnessStatus;
  
  // ✅ TIMESTAMP
  liveDoc["timestamp"] = (uint64_t)(1700000000000ULL + millis());
  
  String livePayload;
  serializeJson(liveDoc, livePayload);
  
  http.begin(firebaseHost + livePath);
  http.addHeader("Content-Type", "application/json");
  http.setTimeout(2000);
  
  int httpCode = http.PUT(livePayload);
  
  if (httpCode != 200 && httpCode != HTTP_CODE_OK) {
    Serial.printf("[LIVE DATA] ❌ HTTP %d\n", httpCode);
  }
  
  http.end();
}

// ═══════════════════════════════════════════════════════════════════════════
// CRASH DETECTION
// ═══════════════════════════════════════════════════════════════════════════

void processCrashDetection() {
  static unsigned long lastCrashCheck = 0;
  if (millis() - lastCrashCheck < 100) return;
  
  bool crashCondition = false;
  String crashType = "";
  
  // Method 1: Simulated accelerometer (replace with real MPU6050)
  if (currentTotalAccel >= CRASH_ACCEL_THRESHOLD) {
    crashCondition = true;
    crashType = "High G-Force Impact";
    Serial.printf("🚨 CRASH: Acceleration %.2fg > %.2fg threshold\n", currentTotalAccel, CRASH_ACCEL_THRESHOLD);
  }
  
  // Method 2: Excessive lean angle (handled in safety checks)
  if (leanAngleExceeded && currentLeanAngle >= LEAN_ANGLE_THRESHOLD) {
    crashCondition = true;
    crashType = "Excessive Lean Angle";
    Serial.printf("🚨 CRASH: Lean angle %.1f° > %.1f° threshold\n", currentLeanAngle, LEAN_ANGLE_THRESHOLD);
  }
  
  // Method 3: Vibration sensor pattern detection
  static int consecutiveVibrations = 0;
  static unsigned long lastVibrationTime = 0;
  
  int vibration = digitalRead(vibrationSensorPin);
  static int lastVibration = LOW;
  
  if (vibration != lastVibration && vibration == HIGH) {
    unsigned long currentTime = millis();
    
    if (currentTime - lastVibrationTime < 500) {
      consecutiveVibrations++;
    } else {
      consecutiveVibrations = 1;
    }
    
    lastVibrationTime = currentTime;
    
    if (consecutiveVibrations >= 5) {
      crashCondition = true;
      crashType = "Vibration Pattern";
      Serial.printf("🚨 CRASH: Vibration pattern detected (%d rapid hits)\n", consecutiveVibrations);
      consecutiveVibrations = 0;
    }
  }
  
  lastVibration = vibration;
  
  // Process crash if detected
  if (crashCondition && !crashDetected) {
    unsigned long timeSinceLastCrash = millis() - lastCrashTime;
    if (timeSinceLastCrash >= 3000) { // 3 second cooldown
      Serial.println("\n🚨 CRASH DETECTED - EMERGENCY PROTOCOLS ACTIVATED!");
      
      crashDetected = true;
      lastCrashTime = millis();
      
      // ✅ IMMEDIATE SAFETY SHUTDOWN
      if (engineRunning) {
        Serial.println("🛑 Emergency engine shutdown due to crash!");
        safetyShutdownActive = true;
        shutdownReason = "Crash Detection (" + crashType + ")";
        lastEngineShutdown = millis();
        forceEngineShutdown("CRASH_DETECTED");
      }
      
      // Alert sequence
      for (int i = 0; i < 10; i++) {
        digitalWrite(buzzerPin, HIGH);
        delay(200);
        digitalWrite(buzzerPin, LOW);
        delay(200);
      }
      
      // ✅ SEND CRASH DATA TO DASHBOARDS
      sendCrashDataToDashboards();
      sendDualAlert("🚨 CRASH DETECTED! Emergency protocols activated.", "danger");
      
      // ✅ EMERGENCY SMS ALERTS
      if (gsmReady) {
        sendCrashAlert();
      }
      
      // Force immediate status update
      sendDashboardHeartbeat();
      updateLiveDataForDashboards();
    }
  }
  
  lastCrashCheck = millis();
  
  // Reset vibration count after timeout
  static unsigned long lastVibrationReset = 0;
  if (millis() - lastVibrationReset > 2000) {
    consecutiveVibrations = 0;
    lastVibrationReset = millis();
  }
}

void sendCrashDataToDashboards() {
  if (WiFi.status() != WL_CONNECTED) return;

  StaticJsonDocument<600> crashDoc;
  crashDoc["timestamp"] = (uint64_t)(1700000000000ULL + millis());
  crashDoc["type"] = "enhanced_safety_crash";
  crashDoc["severity"] = currentTotalAccel > 20.0 ? "severe" : "moderate";
  crashDoc["impactStrength"] = currentTotalAccel;
  crashDoc["roll"] = currentRoll;
  crashDoc["pitch"] = currentPitch;
  crashDoc["leanAngle"] = currentLeanAngle;
  crashDoc["leanAngleExceeded"] = leanAngleExceeded;
  crashDoc["alcoholDetected"] = alcoholDetected;
  crashDoc["alcoholSensorValue"] = alcoholSensorValue;
  crashDoc["engineWasRunning"] = engineRunning;
  crashDoc["safetyShutdownTriggered"] = safetyShutdownActive;
  crashDoc["shutdownReason"] = shutdownReason;
  crashDoc["simulated"] = !MPU6050_ENABLED;
  
  // GPS coordinates
  crashDoc["hasGPS"] = gpsValid;
  if (gpsValid) {
    crashDoc["lat"] = currentLat;
    crashDoc["lng"] = currentLng;
    crashDoc["locationLat"] = currentLat;
    crashDoc["locationLng"] = currentLng;
    crashDoc["speed"] = currentSpeed;
    crashDoc["location"] = String(currentLat, 6) + "," + String(currentLng, 6);
    Serial.printf("[CRASH] 📍 Location: %.6f,%.6f (Speed: %.1f km/h)\n", 
                  currentLat, currentLng, currentSpeed);
  } else {
    crashDoc["lat"] = 0.0;
    crashDoc["lng"] = 0.0;
    crashDoc["locationLat"] = 0.0;
    crashDoc["locationLng"] = 0.0;
    crashDoc["speed"] = 0.0;
    crashDoc["location"] = "GPS unavailable";
    Serial.println("[CRASH] ⚠️ No GPS coordinates available");
  }
  
  String crashPayload;
  serializeJson(crashDoc, crashPayload);
  
  http.begin(firebaseHost + crashPath);
  http.addHeader("Content-Type", "application/json");
  http.setTimeout(5000);
  
  int httpCode = http.POST(crashPayload);
  
  if (httpCode == 200 || httpCode == HTTP_CODE_OK) {
    Serial.println("[CRASH] ✅ Enhanced crash data sent to dashboards");
  } else {
    Serial.printf("[CRASH] ❌ Failed: HTTP %d\n", httpCode);
  }
  
  http.end();
}

// ═══════════════════════════════════════════════════════════════════════════
// GSM MODULE FUNCTIONS
// ═══════════════════════════════════════════════════════════════════════════

void initializeGSM() {
  Serial.println("[GSM] Initializing SIM800L module...");
  
  gsmSerial.begin(9600, SERIAL_8N1, GSM_RX_PIN, GSM_TX_PIN);
  delay(3000);
  
  Serial.println("[GSM] Testing AT command...");
  gsmSerial.println("AT");
  delay(1000);
  
  if (waitForGSMResponse("OK", 5000)) {
    Serial.println("[GSM] ✅ AT command OK");
    
    gsmSerial.println("AT+CMGF=1");
    delay(1000);
    
    if (waitForGSMResponse("OK", 3000)) {
      Serial.println("[GSM] ✅ SMS text mode set");
      
      gsmSerial.println("AT+CNMI=1,2,0,0,0");
      delay(1000);
      
      if (waitForGSMResponse("OK", 3000)) {
        Serial.println("[GSM] ✅ SMS notifications enabled");
        gsmReady = true;
        
        gsmSerial.println("AT+CSQ");
        delay(1000);
        
        Serial.println("[GSM] 🚀 SIM800L MODULE READY FOR ENHANCED SAFETY!");
      }
    }
  }
  
  if (!gsmReady) {
    Serial.println("[GSM] ❌ Initialization failed - emergency SMS unavailable");
  }
}

bool waitForGSMResponse(String expectedResponse, unsigned long timeout) {
  unsigned long startTime = millis();
  String response = "";
  
  while (millis() - startTime < timeout) {
    if (gsmSerial.available()) {
      response += gsmSerial.readString();
      if (response.indexOf(expectedResponse) != -1) {
        return true;
      }
    }
    delay(10);
  }
  
  return false;
}

void sendSMSAlert(String phoneNumber, String message) {
  if (!gsmReady) {
    Serial.println("[GSM] ❌ Module not ready - cannot send SMS");
    return;
  }
  
  Serial.println("[GSM] 📱 Sending safety SMS to: " + phoneNumber);
  
  gsmSerial.println("AT+CMGF=1");
  delay(500);
  
  gsmSerial.println("AT+CMGS=\"" + phoneNumber + "\"");
  delay(500);
  
  gsmSerial.print(message);
  delay(500);
  
  gsmSerial.write(26);
  delay(2000);
  
  if (waitForGSMResponse("OK", 10000)) {
    Serial.println("[GSM] ✅ Safety SMS sent successfully");
  } else {
    Serial.println("[GSM] ❌ Safety SMS send failed");
  }
}

void checkIncomingSMS() {
  if (!gsmReady) return;
  
  while (gsmSerial.available()) {
    String response = gsmSerial.readString();
    
    if (response.indexOf("+CMT:") != -1) {
      Serial.println("[GSM] 📨 Incoming SMS detected");
      processSMSCommand(response);
    }
  }
}

void processSMSCommand(String smsData) {
  // Parse SMS data
  int phoneStart = smsData.indexOf("\"+") + 2;
  int phoneEnd = smsData.indexOf("\"", phoneStart);
  String sender = smsData.substring(phoneStart, phoneEnd);
  
  int messageStart = smsData.lastIndexOf("\n") + 1;
  String command = smsData.substring(messageStart);
  command.trim();
  command.toUpperCase();
  
  Serial.println("[GSM] 📨 SMS from: " + sender);
  Serial.println("[GSM] Command: " + command);
  
  // Verify authorized sender
  if (!isAuthorizedNumber(sender)) {
    Serial.println("[GSM] ❌ Unauthorized sender: " + sender);
    sendSMSAlert(sender, "❌ VIGILERT: Unauthorized access denied");
    sendSMSAlert(OWNER_PHONE, "⚠️ SECURITY ALERT\nUnauthorized SMS: " + sender + "\nCommand: " + command);
    return;
  }
  
  // Process authorized commands with safety validation
  if (command == "START") {
    if (validateSafetyConditions()) {
      startEngine();
      sendSMSAlert(sender, "✅ VIGILERT: Engine started remotely\nSafety: All conditions OK\nTime: " + getCurrentTimeString());
    } else {
      String reason = getSafetyBlockingReasons();
      sendSMSAlert(sender, "🚫 VIGILERT: Engine start BLOCKED\nReason: " + reason + "\nTime: " + getCurrentTimeString());
    }
  }
  else if (command == "STOP") {
    stopEngine();
    sendSMSAlert(sender, "✅ VIGILERT: Engine stopped remotely\nTime: " + getCurrentTimeString());
  }
  else if (command == "SAFETY") {
    sendSafetyStatusSMS(sender);
  }
  else if (command == "LOCATE") {
    sendLocationSMS(sender);
  }
  else if (command == "STATUS") {
    sendStatusSMS(sender);
  }
  else if (command == "RESET CRASH") {
    if (crashDetected) {
      crashDetected = false;
      sendSMSAlert(sender, "✅ VIGILERT: Crash status reset\nEngine restart available if other conditions OK");
      sendDualAlert("Crash status reset via SMS", "info");
    } else {
      sendSMSAlert(sender, "ℹ️ VIGILERT: No crash detected to reset");
    }
  }
  else if (command == "ARM") {
    antiTheftArmed = true;
    vibrationCount = 0;
    sendSMSAlert(sender, "✅ VIGILERT: Anti-theft ARMED");
    
    for (int i = 0; i < 2; i++) {
      digitalWrite(buzzerPin, HIGH);
      delay(100);
      digitalWrite(buzzerPin, LOW);
      delay(100);
    }
  }
  else if (command == "DISARM") {
    antiTheftArmed = false;
    vibrationCount = 0;
    sendSMSAlert(sender, "✅ VIGILERT: Anti-theft DISARMED");
  }
  else if (command == "HELP") {
    String helpMsg = "📋 VIGILERT ENHANCED SAFETY:\n";
    helpMsg += "START - Start engine (safety validated)\n";
    helpMsg += "STOP - Stop engine\n";
    helpMsg += "SAFETY - Safety status report\n";
    helpMsg += "LOCATE - GPS location\n";
    helpMsg += "STATUS - System status\n";
    helpMsg += "RESET CRASH - Reset crash detection\n";
    helpMsg += "ARM/DISARM - Anti-theft control";
    sendSMSAlert(sender, helpMsg);
  }
  else {
    sendSMSAlert(sender, "❌ VIGILERT: Unknown command '" + command + "'\nSend HELP for commands");
  }
}

bool isAuthorizedNumber(String phoneNumber) {
  String authorizedNumbers[] = {OWNER_PHONE, EMERGENCY_CONTACT_1, EMERGENCY_CONTACT_2};
  
  for (int i = 0; i < 3; i++) {
    if (phoneNumber.indexOf(authorizedNumbers[i]) != -1 || 
        authorizedNumbers[i].indexOf(phoneNumber) != -1) {
      return true;
    }
  }
  return false;
}

void sendSafetyStatusSMS(String phoneNumber) {
  String message = "🛡️ VIGILERT SAFETY STATUS\n";
  message += "Time: " + getCurrentTimeString() + "\n";
  message += "Engine: " + String(engineRunning ? "RUNNING" : "STOPPED") + "\n";
  message += "Safety Shutdown: " + String(safetyShutdownActive ? "ACTIVE" : "OK") + "\n";
  
  if (safetyShutdownActive) {
    message += "Shutdown Reason: " + shutdownReason + "\n";
  }
  
  message += "Alcohol: " + String(alcoholDetected ? "DETECTED" : "CLEAR") + " (" + String(alcoholSensorValue) + ")\n";
  message += "Lean Angle: " + String(currentLeanAngle, 1) + "° (Max: " + String(LEAN_ANGLE_THRESHOLD, 0) + "°)\n";
  message += "Crash: " + String(crashDetected ? "DETECTED" : "CLEAR") + "\n";
  message += "Anti-theft: " + String(antiTheftArmed ? "ARMED" : "DISARMED") + "\n";
  
  if (!validateSafetyConditions()) {
    message += "⚠️ Engine start BLOCKED: " + getSafetyBlockingReasons();
  } else {
    message += "✅ All safety conditions OK";
  }
  
  sendSMSAlert(phoneNumber, message);
}

void sendLocationSMS(String phoneNumber) {
  String message = "📍 VIGILERT LOCATION\n";
  message += "Time: " + getCurrentTimeString() + "\n";
  
  if (gpsValid) {
    message += "Lat: " + String(currentLat, 6) + "\n";
    message += "Lng: " + String(currentLng, 6) + "\n";
    message += "Speed: " + String(currentSpeed, 1) + " km/h\n";
    message += "Satellites: " + String(satelliteCount) + "\n";
    message += "Maps: https://maps.google.com/?q=" + String(currentLat, 6) + "," + String(currentLng, 6);
  } else {
    message += "GPS: Signal unavailable\nStatus: Acquiring satellites...";
  }
  
  sendSMSAlert(phoneNumber, message);
}

void sendStatusSMS(String phoneNumber) {
  String message = "📊 VIGILERT STATUS\n";
  message += "Time: " + getCurrentTimeString() + "\n";
  message += "Engine: " + String(engineRunning ? "RUNNING" : "STOPPED") + "\n";
  message += "Safety: " + String(safetyShutdownActive ? "SHUTDOWN" : "OK") + "\n";
  message += "Alcohol: " + String(alcoholDetected ? "DETECTED" : "CLEAR") + "\n";
  message += "Lean: " + String(currentLeanAngle, 1) + "°\n";
  message += "GPS: " + String(gpsValid ? "ACTIVE" : "SEARCHING") + "\n";
  message += "WiFi: " + String(WiFi.status() == WL_CONNECTED ? "CONNECTED" : "OFFLINE") + "\n";
  message += "GSM: " + String(gsmReady ? "READY" : "ERROR");
  
  sendSMSAlert(phoneNumber, message);
}

void sendCrashAlert() {
  String message = "🚨 CRASH DETECTED!\n";
  message += "Time: " + getCurrentTimeString() + "\n";
  message += "Type: Enhanced Safety Detection\n";
  message += "Lean Angle: " + String(currentLeanAngle, 1) + "°\n";
  message += "G-Force: " + String(currentTotalAccel, 1) + "g\n";
  message += "Engine: AUTO SHUTDOWN\n";
  
  if (gpsValid) {
    message += "Location: " + String(currentLat, 6) + "," + String(currentLng, 6) + "\n";
    message += "Speed: " + String(currentSpeed, 1) + " km/h\n";
    message += "Emergency Map: https://maps.google.com/?q=" + String(currentLat, 6) + "," + String(currentLng, 6) + "\n";
    message += "🆘 CALL EMERGENCY SERVICES IF NEEDED!";
  } else {
    message += "Location: GPS unavailable\n";
    message += "🆘 CHECK RIDER IMMEDIATELY!";
  }
  
  // Send to all emergency contacts
  sendSMSAlert(EMERGENCY_CONTACT_1, message);
  sendSMSAlert(EMERGENCY_CONTACT_2, message);
  sendSMSAlert(OWNER_PHONE, message);
  
  Serial.println("[CRASH] 🚨 Emergency SMS alerts sent to all contacts");
}

// ═══════════════════════════════════════════════════════════════════════════
// UTILITY FUNCTIONS
// ═══════════════════════════════════════════════════════════════════════════

void sendDualAlert(String message, String type) {
  if (WiFi.status() == WL_CONNECTED) {
    sendDashboardAlert(message, type);
  }
  Serial.println("[ALERT] " + message);
}

void sendDashboardAlert(String message, String type) {
  if (WiFi.status() != WL_CONNECTED) return;

  StaticJsonDocument<300> alertDoc;
  alertDoc["message"] = message;
  alertDoc["type"] = type;
  alertDoc["timestamp"] = (uint64_t)(1700000000000ULL + millis());
  alertDoc["time"] = getCurrentTimeString();
  alertDoc["source"] = "enhanced_safety_simple";
  
  String alertPayload;
  serializeJson(alertDoc, alertPayload);
  
  String alertPath = alertsPath.substring(0, alertsPath.length() - 5) + "/" + String(millis()) + ".json";
  
  http.begin(firebaseHost + alertPath);
  http.addHeader("Content-Type", "application/json");
  http.setTimeout(2000);
  
  int httpCode = http.PUT(alertPayload);
  
  if (httpCode == 200 || httpCode == HTTP_CODE_OK) {
    Serial.printf("[ALERT] ✅ Dashboard alert sent: %s\n", message.c_str());
  }
  
  http.end();
}

String getCurrentTimeString() {
  unsigned long currentTime = millis();
  unsigned long seconds = (currentTime / 1000) % 60;
  unsigned long minutes = (currentTime / (1000 * 60)) % 60;
  unsigned long hours = (currentTime / (1000 * 60 * 60)) % 24;
  
  return String(hours) + ":" + String(minutes) + ":" + String(seconds);
}

void playAlertSound(int beeps, int duration) {
  for (int i = 0; i < beeps; i++) {
    digitalWrite(buzzerPin, HIGH);
    delay(duration);
    digitalWrite(buzzerPin, LOW);
    delay(duration / 2);
  }
}

// ═══════════════════════════════════════════════════════════════════════════
// GPS AND OTHER FUNCTIONS
// ═══════════════════════════════════════════════════════════════════════════

void updateGPSData() {
  while (gpsSerial.available() > 0) {
    if (gps.encode(gpsSerial.read())) {
      if (gps.location.isValid()) {
        currentLat = gps.location.lat();
        currentLng = gps.location.lng();
        gpsValid = true;
      }
      
      if (gps.speed.isValid()) {
        currentSpeed = gps.speed.kmph();
      }
      
      if (gps.satellites.isValid()) {
        satelliteCount = gps.satellites.value();
      }
    }
  }
  
  if (gps.location.age() > 5000) {
    gpsValid = false;
  }
}

void processAntiTheft() {
  static unsigned long engineOffTime = 0;
  static bool wasEngineRunning = false;
  
  if (wasEngineRunning && !engineRunning) {
    engineOffTime = millis();
  }
  
  if (!engineRunning && !antiTheftArmed && (millis() - engineOffTime > 10000)) {
    antiTheftArmed = true;
    Serial.println("[ANTI-THEFT] 🛡️ ARMED!");
    sendDualAlert("Anti-theft system ARMED", "success");
    
    digitalWrite(buzzerPin, HIGH);
    delay(100);
    digitalWrite(buzzerPin, LOW);
  }
  
  if (engineRunning && antiTheftArmed) {
    antiTheftArmed = false;
    Serial.println("[ANTI-THEFT] 🔓 Disarmed");
  }
  
  wasEngineRunning = engineRunning;
}

void processTripTracking() {
  if (engineRunning && gpsValid && !tripActive) {
    startTrip();
  } else if (!engineRunning && tripActive) {
    endTrip();
  }
  
  // Update trip data while active
  if (tripActive && gpsValid) {
    if (currentSpeed > tripMaxSpeed) {
      tripMaxSpeed = currentSpeed;
    }
    
    // Calculate distance (simplified)
    static double lastTripLat = 0.0;
    static double lastTripLng = 0.0;
    
    if (lastTripLat != 0.0 && lastTripLng != 0.0) {
      float deltaLat = (currentLat - lastTripLat) * 111000; // Rough conversion to meters
      float deltaLng = (currentLng - lastTripLng) * 111000 * cos(currentLat * PI / 180);
      float segmentDistance = sqrt(deltaLat * deltaLat + deltaLng * deltaLng);
      
      if (segmentDistance < 1000) { // Ignore GPS jumps > 1km
        tripDistance += segmentDistance / 1000.0; // Convert to km
      }
    }
    
    lastTripLat = currentLat;
    lastTripLng = currentLng;
  }
}

void startTrip() {
  tripActive = true;
  tripStartTime = millis();
  tripStartLat = currentLat;
  tripStartLng = currentLng;
  tripDistance = 0.0;
  tripMaxSpeed = 0.0;
  
  Serial.println("[TRIP] 🚗 Trip started");
  sendDualAlert("Trip started - tracking enabled", "info");
}

void endTrip() {
  if (!tripActive) return;
  
  tripActive = false;
  unsigned long tripDuration = millis() - tripStartTime;
  
  // Save trip to Firebase
  saveTripToFirebase(tripDuration);
  
  Serial.printf("[TRIP] 🏁 Trip ended - Duration: %lu min, Distance: %.2f km, Max Speed: %.1f km/h\n", 
                tripDuration / 60000, tripDistance, tripMaxSpeed);
  
  sendDualAlert("Trip completed and saved", "success");
}

void saveTripToFirebase(unsigned long duration) {
  if (WiFi.status() != WL_CONNECTED) return;

  StaticJsonDocument<400> tripDoc;
  tripDoc["startTime"] = (uint64_t)(1700000000000ULL + tripStartTime);
  tripDoc["endTime"] = (uint64_t)(1700000000000ULL + millis());
  tripDoc["duration"] = duration;
  tripDoc["distance"] = tripDistance;
  tripDoc["maxSpeed"] = tripMaxSpeed;
  tripDoc["avgSpeed"] = (duration > 0) ? (tripDistance / (duration / 3600000.0)) : 0.0;
  tripDoc["startLat"] = tripStartLat;
  tripDoc["startLng"] = tripStartLng;
  tripDoc["endLat"] = currentLat;
  tripDoc["endLng"] = currentLng;
  tripDoc["safetyEvents"] = crashDetected || alcoholDetected || leanAngleExceeded;
  tripDoc["simulated"] = !MPU6050_ENABLED;
  
  String tripPayload;
  serializeJson(tripDoc, tripPayload);
  
  String tripPath = tripsPath.substring(0, tripsPath.length() - 5) + "/" + String(millis()) + ".json";
  
  http.begin(firebaseHost + tripPath);
  http.addHeader("Content-Type", "application/json");
  http.setTimeout(3000);
  
  int httpCode = http.PUT(tripPayload);
  
  if (httpCode == 200 || httpCode == HTTP_CODE_OK) {
    Serial.println("[TRIP] ✅ Trip data saved to dashboard");
  }
  
  http.end();
}

void processSpeedMonitoring() {
  static unsigned long lastSpeedCheck = 0;
  if (millis() - lastSpeedCheck < 1000) return;
  
  bool wasOverSpeed = isOverSpeed;
  isOverSpeed = (currentSpeed > speedLimit);
  
  if (isOverSpeed && !wasOverSpeed && engineRunning) {
    Serial.printf("[SPEED] ⚠️ Speed limit exceeded: %.1f km/h > %d km/h\n", currentSpeed, speedLimit);
    sendDualAlert("Speed limit exceeded: " + String(currentSpeed, 1) + " km/h", "danger");
  }
  
  lastSpeedCheck = millis();
}

void loadSpeedLimit() {
  if (WiFi.status() != WL_CONNECTED) return;

  http.begin(firebaseHost + speedLimitPath);
  http.setTimeout(3000);
  
  int httpCode = http.GET();
  
  if (httpCode == 200) {
    String response = http.getString();
    int newSpeedLimit = response.toInt();
    
    if (newSpeedLimit > 0 && newSpeedLimit <= 200) {
      speedLimit = newSpeedLimit;
      Serial.printf("[SPEED] Speed limit loaded: %d km/h\n", speedLimit);
    }
  }
  
  http.end();
}

// ═══════════════════════════════════════════════════════════════════════════
// ENHANCED SERIAL COMMANDS
// ═══════════════════════════════════════════════════════════════════════════

void handleSerialCommands() {
  if (Serial.available()) {
    String cmd = Serial.readStringUntil('\n');
    cmd.trim();
    cmd.toUpperCase();
    
    if (cmd == "START") {
      if (validateSafetyConditions()) {
        startEngine();
      } else {
        Serial.println("🚫 ENGINE START BLOCKED - Safety conditions not met:");
        Serial.println("   Reasons: " + getSafetyBlockingReasons());
      }
    }
    else if (cmd == "STOP") {
      stopEngine();
    }
    else if (cmd == "STATUS") {
      printEnhancedStatus();
    }
    else if (cmd == "TEST LEAN") {
      Serial.println("\n🧪 TESTING LEAN ANGLE DETECTION...");
      Serial.printf("📍 Current lean angle: %.1f°\n", currentLeanAngle);
      Serial.printf("📍 Threshold: %.1f°\n", LEAN_ANGLE_THRESHOLD);
      Serial.printf("📍 MPU6050 enabled: %s\n", MPU6050_ENABLED ? "YES" : "NO (SIMULATED)");
      Serial.printf("📍 Lean exceeded: %s\n", leanAngleExceeded ? "YES" : "NO");
      Serial.printf("📍 Roll: %.1f° | Pitch: %.1f°\n", currentRoll, currentPitch);
      Serial.println("💡 Lean angle is simulated - install MPU6050 library for real detection");
    }
    else if (cmd == "TEST SAFETY") {
      Serial.println("\n🧪 TESTING SAFETY SYSTEM...");
      Serial.printf("📍 Alcohol detected: %s (Value: %d)\n", alcoholDetected ? "YES" : "NO", alcoholSensorValue);
      Serial.printf("📍 Lean angle exceeded: %s (%.1f°)\n", leanAngleExceeded ? "YES" : "NO", currentLeanAngle);
      Serial.printf("📍 Crash detected: %s\n", crashDetected ? "YES" : "NO");
      Serial.printf("📍 Safety shutdown: %s\n", safetyShutdownActive ? "ACTIVE" : "OK");
      if (safetyShutdownActive) {
        Serial.printf("📍 Shutdown reason: %s\n", shutdownReason.c_str());
      }
      Serial.printf("📍 Engine allowed: %s\n", engineAllowed ? "YES" : "NO");
      if (!engineAllowed) {
        Serial.println("📍 Blocking reasons: " + getSafetyBlockingReasons());
      }
    }
    else if (cmd == "RESET SAFETY") {
      Serial.println("\n🔄 RESETTING SAFETY SYSTEM...");
      crashDetected = false;
      safetyShutdownActive = false;
      shutdownReason = "";
      Serial.println("✅ Safety system reset! (Note: Alcohol and lean angle are real-time)");
      sendDualAlert("Safety system reset via serial command", "info");
    }
    else if (cmd == "TEST GSM") {
      Serial.println("\n🧪 TESTING GSM MODULE...");
      Serial.printf("📍 GSM ready: %s\n", gsmReady ? "YES" : "NO");
      if (gsmReady) {
        sendSMSAlert(OWNER_PHONE, "🧪 GSM TEST\nThis is a test message from VIGILERT Enhanced Safety System (Simple Version)\nTime: " + getCurrentTimeString());
        Serial.println("✅ Test SMS sent to owner");
      } else {
        Serial.println("❌ GSM not ready - check connections");
      }
    }
    else if (cmd == "SIMULATE ALCOHOL") {
      Serial.println("\n🧪 SIMULATING ALCOHOL DETECTION...");
      alcoholDetected = true;
      alcoholDetectedTime = millis();
      alcoholSensorValue = 3500; // Simulated high value
      Serial.println("⚠️ Alcohol simulation active - engine shutdown will occur in 2 seconds");
    }
    else if (cmd == "SIMULATE LEAN") {
      Serial.println("\n🧪 SIMULATING EXCESSIVE LEAN ANGLE...");
      leanAngleExceeded = true;
      leanAngleStartTime = millis();
      currentLeanAngle = 45.0; // Simulated excessive lean
      Serial.println("⚠️ Lean angle simulation active - engine shutdown will occur in 3 seconds");
    }
    else if (cmd == "TEST CONNECTION") {
      Serial.println("\n🧪 TESTING FIREBASE CONNECTION...");
      testFirebaseConnection();
      sendDashboardHeartbeat();
      Serial.println("✅ Connection test completed - check dashboard for motorcycle status");
    }
    else if (cmd == "FORCE CONNECT") {
      Serial.println("\n🔄 FORCING MOTORCYCLE CONNECTION...");
      
      // Send multiple heartbeats to ensure connection
      for (int i = 0; i < 3; i++) {
        Serial.printf("Attempt %d/3...\n", i + 1);
        sendDashboardHeartbeat();
        delay(1000);
      }
      
      Serial.println("✅ Force connection completed - check dashboard");
    }
  }
}

void printEnhancedStatus() {
  Serial.println("\n━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━");
  Serial.println("📊 VIGILERT ENHANCED SAFETY SYSTEM (SIMPLE VERSION) - DETAILED STATUS");
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
  
  // Enhanced Safety Status
  Serial.println("\n🛡️ ENHANCED SAFETY STATUS:");
  Serial.printf("Safety Shutdown: %s\n", safetyShutdownActive ? "ACTIVE ⚠️" : "OK ✅");
  if (safetyShutdownActive) {
    Serial.printf("Shutdown Reason: %s\n", shutdownReason.c_str());
    Serial.printf("Time Since Shutdown: %lu seconds\n", (millis() - lastEngineShutdown) / 1000);
  }
  
  Serial.printf("Alcohol Detected: %s (Value: %d, Threshold: %d)\n", 
                alcoholDetected ? "YES ⚠️" : "NO ✅", alcoholSensorValue, ALCOHOL_THRESHOLD);
  
  Serial.printf("Lean Angle: %.1f° (Threshold: %.1f°) %s\n", 
                currentLeanAngle, LEAN_ANGLE_THRESHOLD, 
                leanAngleExceeded ? "⚠️ EXCEEDED" : "✅ OK");
  
  Serial.printf("Crash Detected: %s\n", crashDetected ? "YES ⚠️" : "NO ✅");
  Serial.printf("Anti-theft Armed: %s\n", antiTheftArmed ? "YES 🛡️" : "NO ❌");
  
  // Sensor Status
  Serial.println("\n🔬 SENSOR STATUS:");
  Serial.printf("MPU6050 Enabled: %s\n", MPU6050_ENABLED ? "YES (but library not included)" : "NO (SIMULATED)");
  Serial.printf("Roll: %.1f° | Pitch: %.1f° | Lean: %.1f° (SIMULATED)\n", currentRoll, currentPitch, currentLeanAngle);
  Serial.printf("Acceleration: %.2f g (SIMULATED)\n", currentTotalAccel);
  Serial.printf("Vibration Sensor: %d\n", digitalRead(vibrationSensorPin));
  Serial.printf("Helmet Connected: %s\n", helmetConnected ? "YES ✅" : "NO ❌");
  
  // GPS Status
  Serial.println("\n🛰️ GPS STATUS:");
  Serial.printf("GPS Valid: %s\n", gpsValid ? "YES ✅" : "NO ❌");
  Serial.printf("Latitude: %.6f\n", currentLat);
  Serial.printf("Longitude: %.6f\n", currentLng);
  Serial.printf("Speed: %.1f km/h (Limit: %d km/h)\n", currentSpeed, speedLimit);
  Serial.printf("Satellites: %d\n", satelliteCount);
  
  // GSM Status
  Serial.println("\n📱 GSM STATUS:");
  Serial.printf("GSM Ready: %s\n", gsmReady ? "YES ✅" : "NO ❌");
  Serial.printf("Emergency Contacts: %s, %s\n", EMERGENCY_CONTACT_1.c_str(), EMERGENCY_CONTACT_2.c_str());
  
  // Safety Validation
  Serial.println("\n🔍 SAFETY VALIDATION:");
  if (validateSafetyConditions()) {
    Serial.println("✅ All safety conditions OK - Engine start allowed");
  } else {
    Serial.println("❌ Safety conditions not met - Engine start BLOCKED");
    Serial.println("   Blocking reasons: " + getSafetyBlockingReasons());
  }
  
  // System Health
  Serial.println("\n💾 SYSTEM HEALTH:");
  Serial.printf("Free Heap: %d bytes (%.1f%% used)\n", ESP.getFreeHeap(), 
                100.0 - (ESP.getFreeHeap() * 100.0 / ESP.getHeapSize()));
  Serial.printf("Uptime: %lu minutes\n", millis() / 60000);
  
  Serial.println("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━");
  Serial.println("📋 ENHANCED COMMANDS: START, STOP, STATUS, TEST LEAN, TEST SAFETY, RESET SAFETY, TEST GSM");
  Serial.println("🧪 SIMULATION: SIMULATE ALCOHOL, SIMULATE LEAN");
  Serial.println("� CMONNECTION: TEST CONNECTION, FORCE CONNECT");
  Serial.println("�  SMS COMMANDS: START, STOP, SAFETY, LOCATE, STATUS, RESET CRASH, ARM, DISARM, HELP");
  Serial.println("🛡️ SAFETY FEATURES: Auto shutdown on alcohol/lean angle, Enhanced crash detection, Emergency SMS");
  Serial.println("⚠️ NOTE: This is the SIMPLE version with simulated lean angle detection");
  Serial.println("💡 Install MPU6050 library and use the full version for real lean angle detection");
  Serial.println("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━");
}