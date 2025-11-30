/* 
 * VigiLERT ESP32 Motorcycle Module - FULL DIAGNOSTIC VERSION
 * ✅ Comprehensive Serial Monitor output for all sensors
 * ✅ Real-time status display
 * ✅ Easy troubleshooting
 */

#include <WiFi.h>
#include <HTTPClient.h>
#include <TinyGPSPlus.h>
#include <HardwareSerial.h>
#include <Wire.h>
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <ArduinoOTA.h>
#include <ArduinoJson.h>
#include <TimeLib.h>

// ======= USER CONFIG =======
const char* ssid = "DPWH";
const char* password = "12345678900";
const String firebaseHost = "https://vigilance-shield-default-rtdb.firebaseio.com";
const String userUID = "MnzBjTBslZNijOkq732PE91hHa23";
const String firebaseAuth = "";
const String ownerPhoneNumber = "+639675715673";

// ======= PIN ASSIGNMENTS =======
const int relayPin = 13;
const int buzzerPin = 12;
const int lightIndicatorPin = 2;
const int vibrationSensorPin = 15;

#define GSM_RX 4
#define GSM_TX 5
HardwareSerial gsmSerial(2);

HardwareSerial gpsSerial(1);
TinyGPSPlus gps;

Adafruit_MPU6050 mpu;
sensors_event_t accel, gyro, temp;

// Engine control
bool engineRunning = false;
bool crashDetected = false;
const float ACCEL_THRESHOLD = 15.0;

// Sensor readings
float currentRoll = 0.0;
float currentTotalAccel = 0.0;

// Timing
unsigned long lastCrashTime = 0;
const unsigned long CRASH_COOLDOWN = 5000;

// Alcohol detection
bool alcoholDetected = false;
bool lastAlcoholState = false;
unsigned long lastAlcoholCheck = 0;
const unsigned long ALCOHOL_CHECK_INTERVAL = 500;

// Auto control
bool autoEngineControl = false;
bool engineStartRequested = false;

// Dashboard button
bool lastDashboardButtonState = false;
unsigned long lastButtonCheck = 0;
const unsigned long BUTTON_CHECK_INTERVAL = 1000;

// Heartbeat
unsigned long lastHeartbeat = 0;
const unsigned long HEARTBEAT_INTERVAL = 2000;

// Anti-theft
bool antiTheftEnabled = false;
bool antiTheftArmed = false;
unsigned long engineOffTime = 0;
const unsigned long ARM_DELAY = 30000;
unsigned long lastTheftAlert = 0;
const unsigned long THEFT_ALERT_COOLDOWN = 300000;
bool theftAlertSent = false;
int theftDetectionCount = 0;
const int THEFT_DETECTION_REQUIRED = 1;
unsigned long lastVibrationTime = 0;
const unsigned long VIBRATION_DEBOUNCE = 500;

// Module status
bool gsmReady = false;
bool gpsReady = false;
bool mpu6050Ready = false;
bool wifiReady = false;

// ✅ NEW: Diagnostic display timing
unsigned long lastDiagnosticDisplay = 0;
const unsigned long DIAGNOSTIC_INTERVAL = 3000;  // Display every 3 seconds

// GPS data
int gpsSatellites = 0;
float gpsLatitude = 0.0;
float gpsLongitude = 0.0;
float gpsSpeed = 0.0;
float gpsAltitude = 0.0;

// GSM data
String gsmSignal = "Unknown";
String gsmOperator = "Unknown";

// Firebase paths
const String crashPath = "/helmet_public/" + userUID + "/crashes.json?auth=" + firebaseAuth;
const String buttonPath = "/" + userUID + "/engineControl/startButton.json?auth=" + firebaseAuth;
const String livePath = "/helmet_public/" + userUID + "/live.json?auth=" + firebaseAuth;
const String alcoholPaths[] = {
  "/" + userUID + "/alcohol/status/status.json?auth=" + firebaseAuth,
  "/" + userUID + "/alcohol/status.json?auth=" + firebaseAuth,
  "/" + userUID + "/alcohol.json?auth=" + firebaseAuth,
  "/helmet_public/" + userUID + "/alcohol.json?auth=" + firebaseAuth,
  "/helmet_public/" + userUID + "/helmetStatus/alcohol.json?auth=" + firebaseAuth,
  "/" + userUID + ".json?auth=" + firebaseAuth
};
const int numAlcoholPaths = 6;

void setup() {
  Serial.begin(115200);
  delay(100);
  
  printStartupBanner();
  
  // Initialize pins
  pinMode(relayPin, OUTPUT);
  pinMode(buzzerPin, OUTPUT);
  pinMode(lightIndicatorPin, OUTPUT);
  pinMode(vibrationSensorPin, INPUT);
  
  digitalWrite(relayPin, HIGH);
  digitalWrite(buzzerPin, LOW);
  digitalWrite(lightIndicatorPin, LOW);
  
  Serial.println("\n[INIT] Initializing hardware modules...\n");
  
  // Initialize MPU6050
  initializeMPU6050();
  
  // Initialize GPS
  initializeGPS();
  
  // Initialize GSM
  initializeGSM();
  
  // Initialize WiFi
  initializeWiFi();
  
  // Print pin configuration
  printPinConfiguration();
  
  // Send initial heartbeat
  sendMotorcycleHeartbeat(true);
  lastHeartbeat = millis();
  
  // Print commands
  printCommands();
  
  Serial.println("\n✅ SETUP COMPLETE - Starting main loop...\n");
  Serial.println("═══════════════════════════════════════════════════════════════\n");
}

void printStartupBanner() {
  Serial.println("\n\n");
  Serial.println("╔═══════════════════════════════════════════════════════════╗");
  Serial.println("║                                                           ║");
  Serial.println("║        VIGILERT MOTORCYCLE MODULE - DIAGNOSTIC            ║");
  Serial.println("║                                                           ║");
  Serial.println("║  📊 Full sensor monitoring and status display            ║");
  Serial.println("║  🔍 Real-time diagnostics for all modules                ║");
  Serial.println("║  ✅ Easy troubleshooting and verification                ║");
  Serial.println("║                                                           ║");
  Serial.println("╚═══════════════════════════════════════════════════════════╝");
  Serial.println();
}

void initializeMPU6050() {
  Serial.println("[MPU6050] Initializing accelerometer/gyroscope...");
  Wire.begin(21, 22);
  
  int attempts = 0;
  while (!mpu.begin() && attempts < 5) {
    Serial.printf("[MPU6050] ❌ Not found (attempt %d/5)\n", attempts + 1);
    delay(1000);
    attempts++;
  }
  
  if (mpu.begin()) {
    mpu6050Ready = true;
    mpu.setAccelerometerRange(MPU6050_RANGE_16_G);
    mpu.setGyroRange(MPU6050_RANGE_2000_DEG);
    mpu.setFilterBandwidth(MPU6050_BAND_21_HZ);
    Serial.println("[MPU6050] ✅ READY - Crash detection active");
    Serial.println("[MPU6050] Range: ±16g, Gyro: ±2000°/s");
  } else {
    Serial.println("[MPU6050] ❌ FAILED - Crash detection disabled");
    Serial.println("[MPU6050] Check I2C connections (SDA:21, SCL:22)");
  }
  Serial.println();
}

void initializeGPS() {
  Serial.println("[GPS] Initializing GPS module...");
  gpsSerial.begin(9600, SERIAL_8N1, 16, 17);
  Serial.println("[GPS] Serial started on RX:16, TX:17");
  Serial.println("[GPS] Waiting for satellite fix...");
  Serial.println("[GPS] This may take 30-60 seconds outdoors");
  Serial.println();
}

void initializeGSM() {
  Serial.println("[GSM] Initializing GSM module...");
  gsmSerial.begin(9600, SERIAL_8N1, GSM_RX, GSM_TX);
  Serial.println("[GSM] Serial started on RX:4, TX:5");
  
  delay(3000);
  
  // Test AT command
  gsmSerial.println("AT");
  delay(1000);
  
  if (gsmSerial.available()) {
    String response = gsmSerial.readString();
    if (response.indexOf("OK") != -1) {
      gsmReady = true;
      Serial.println("[GSM] ✅ READY - Module responding");
      
      // Set SMS mode
      gsmSerial.println("AT+CMGF=1");
      delay(1000);
      
      // Get signal strength
      gsmSerial.println("AT+CSQ");
      delay(1000);
      if (gsmSerial.available()) {
        gsmSignal = gsmSerial.readString();
        Serial.println("[GSM] Signal: " + gsmSignal);
      }
      
      // Get operator
      gsmSerial.println("AT+COPS?");
      delay(1000);
      if (gsmSerial.available()) {
        gsmOperator = gsmSerial.readString();
        Serial.println("[GSM] Operator: " + gsmOperator);
      }
    } else {
      Serial.println("[GSM] ❌ FAILED - No response");
      Serial.println("[GSM] Check: Power, SIM card, antenna");
    }
  } else {
    Serial.println("[GSM] ❌ FAILED - Module not responding");
    Serial.println("[GSM] Check wiring and power supply (needs 2A)");
  }
  Serial.println();
}

void initializeWiFi() {
  Serial.println("[WiFi] Connecting to: " + String(ssid));
  WiFi.begin(ssid, password);
  
  int attempts = 0;
  while (WiFi.status() != WL_CONNECTED && attempts < 20) {
    delay(500);
    Serial.print(".");
    attempts++;
  }
  Serial.println();
  
  if (WiFi.status() == WL_CONNECTED) {
    wifiReady = true;
    Serial.println("[WiFi] ✅ CONNECTED");
    Serial.printf("[WiFi] IP Address: %s\n", WiFi.localIP().toString().c_str());
    Serial.printf("[WiFi] Signal: %d dBm\n", WiFi.RSSI());
  } else {
    Serial.println("[WiFi] ❌ FAILED - Connection timeout");
    Serial.println("[WiFi] Check SSID and password");
  }
  Serial.println();
}

void printPinConfiguration() {
  Serial.println("═══════════════════════════════════════════════════════════════");
  Serial.println("📌 PIN CONFIGURATION");
  Serial.println("═══════════════════════════════════════════════════════════════");
  Serial.printf("  GPIO 13: Relay (Engine Control)    - Current: %d\n", digitalRead(relayPin));
  Serial.printf("  GPIO 12: Buzzer                     - Current: %d\n", digitalRead(buzzerPin));
  Serial.printf("  GPIO 2:  LED Indicator              - Current: %d\n", digitalRead(lightIndicatorPin));
  Serial.printf("  GPIO 15: Vibration Sensor           - Current: %d\n", digitalRead(vibrationSensorPin));
  Serial.println("  GPIO 4:  GSM RX");
  Serial.println("  GPIO 5:  GSM TX");
  Serial.println("  GPIO 16: GPS RX");
  Serial.println("  GPIO 17: GPS TX");
  Serial.println("  GPIO 21: I2C SDA (MPU6050)");
  Serial.println("  GPIO 22: I2C SCL (MPU6050)");
  Serial.println("═══════════════════════════════════════════════════════════════\n");
}

void printCommands() {
  Serial.println("═══════════════════════════════════════════════════════════════");
  Serial.println("📋 AVAILABLE SERIAL COMMANDS");
  Serial.println("═══════════════════════════════════════════════════════════════");
  Serial.println("  START ENGINE    - Start engine (relay ON)");
  Serial.println("  STOP ENGINE     - Stop engine (relay OFF)");
  Serial.println("  STATUS          - Show full system status");
  Serial.println("  GPS             - Show GPS details");
  Serial.println("  GSM             - Show GSM details");
  Serial.println("  MPU             - Show MPU6050 details");
  Serial.println("  SENSORS         - Show all sensor readings");
  Serial.println("  TEST VIBRATION  - Test vibration sensor");
  Serial.println("  TEST SMS        - Send test SMS");
  Serial.println("  TEST RELAY      - Test relay on/off");
  Serial.println("  ARM THEFT       - Arm anti-theft");
  Serial.println("  DISARM THEFT    - Disarm anti-theft");
  Serial.println("  CRASH           - Simulate crash");
  Serial.println("═══════════════════════════════════════════════════════════════\n");
}

void loop() {
  // Heartbeat
  if (millis() - lastHeartbeat >= HEARTBEAT_INTERVAL) {
    sendMotorcycleHeartbeat(true);
    lastHeartbeat = millis();
  }
  
  // ✅ NEW: Display comprehensive diagnostics every 3 seconds
  if (millis() - lastDiagnosticDisplay >= DIAGNOSTIC_INTERVAL) {
    displayComprehensiveDiagnostics();
    lastDiagnosticDisplay = millis();
  }
  
  // Read GPS data
  readGPSData();
  
  // Handle commands
  handleSerialCommands();
  
  // Handle dashboard button
  handleDashboardButton();
  
  // Read MPU6050
  if (mpu6050Ready) {
    mpu.getEvent(&accel, &gyro, &temp);
    
    currentTotalAccel = sqrt(
      accel.acceleration.x * accel.acceleration.x +
      accel.acceleration.y * accel.acceleration.y +
      accel.acceleration.z * accel.acceleration.z
    );
    
    currentRoll = atan2(accel.acceleration.y, accel.acceleration.z) * 180.0 / PI;
    float leanAngle = abs(currentRoll);
    
    // Crash detection
    if (engineRunning && (currentTotalAccel >= ACCEL_THRESHOLD || leanAngle > 40) && !crashDetected) {
      unsigned long timeSinceLastCrash = millis() - lastCrashTime;
      if (timeSinceLastCrash >= CRASH_COOLDOWN) {
        triggerCrashShutdown(currentTotalAccel, currentRoll);
      }
    }
    
    if (crashDetected) {
      bool isUpright = (leanAngle < 30);
      bool isStable = (currentTotalAccel < ACCEL_THRESHOLD - 2.0);
      if (isUpright && isStable) {
        crashDetected = false;
        Serial.println("[CRASH] ✓ Cleared - bike stable");
      }
    }
  }
  
  // Anti-theft
  if (!engineRunning) {
    handleAntiTheftWithVibrationSensor();
  } else {
    if (antiTheftArmed) {
      antiTheftArmed = false;
      theftDetectionCount = 0;
      theftAlertSent = false;
    }
  }
  
  // Alcohol monitoring
  if (millis() - lastAlcoholCheck > ALCOHOL_CHECK_INTERVAL) {
    checkAlcoholStatus();
    lastAlcoholCheck = millis();
  }
  
  // Auto control
  if (autoEngineControl) {
    if (alcoholDetected && engineRunning) {
      stopEngine();
      engineStartRequested = true;
    }
    else if (!alcoholDetected && engineStartRequested && !engineRunning) {
      startEngine();
    }
  }
  
  // Safety override
  if (alcoholDetected) {
    digitalWrite(relayPin, HIGH);
  }
  
  // Firebase updates
  static unsigned long lastFirebaseUpdate = 0;
  if (millis() - lastFirebaseUpdate > 2000) {
    sendLiveToFirebase();
    lastFirebaseUpdate = millis();
  }
  
  delay(50);
}

// ✅ NEW: Comprehensive diagnostic display
void displayComprehensiveDiagnostics() {
  Serial.println("\n╔═══════════════════════════════════════════════════════════════════════════╗");
  Serial.println("║                        SYSTEM DIAGNOSTICS REPORT                          ║");
  Serial.println("╠═══════════════════════════════════════════════════════════════════════════╣");
  
  // Module Status
  Serial.println("║ 📡 MODULE STATUS                                                          ║");
  Serial.println("╠═══════════════════════════════════════════════════════════════════════════╣");
  Serial.printf("║  WiFi:      %s  %-55s ║\n", wifiReady ? "✅" : "❌", 
    wifiReady ? ("IP: " + WiFi.localIP().toString() + " | Signal: " + String(WiFi.RSSI()) + " dBm").c_str() : "Not connected");
  Serial.printf("║  GSM:       %s  %-55s ║\n", gsmReady ? "✅" : "❌", 
    gsmReady ? "Module responding | SMS ready" : "Not responding");
  Serial.printf("║  GPS:       %s  %-55s ║\n", gpsReady ? "✅" : "❌", 
    gpsReady ? ("Satellites: " + String(gpsSatellites) + " | Fix acquired").c_str() : "Waiting for fix");
  Serial.printf("║  MPU6050:   %s  %-55s ║\n", mpu6050Ready ? "✅" : "❌", 
    mpu6050Ready ? "Crash detection active" : "Not detected");
  Serial.println("╠═══════════════════════════════════════════════════════════════════════════╣");
  
  // GPS Data
  Serial.println("║ 🛰️  GPS DATA                                                               ║");
  Serial.println("╠═══════════════════════════════════════════════════════════════════════════╣");
  if (gpsReady) {
    Serial.printf("║  Latitude:   %12.6f°                                              ║\n", gpsLatitude);
    Serial.printf("║  Longitude:  %12.6f°                                              ║\n", gpsLongitude);
    Serial.printf("║  Altitude:   %8.2f m                                                  ║\n", gpsAltitude);
    Serial.printf("║  Speed:      %8.2f km/h                                               ║\n", gpsSpeed);
    Serial.printf("║  Satellites: %d                                                          ║\n", gpsSatellites);
  } else {
    Serial.println("║  No GPS fix - Move to open area with clear sky view                      ║");
  }
  Serial.println("╠═══════════════════════════════════════════════════════════════════════════╣");
  
  // MPU6050 Data
  Serial.println("║ 📊 MOTION SENSOR (MPU6050)                                                ║");
  Serial.println("╠═══════════════════════════════════════════════════════════════════════════╣");
  if (mpu6050Ready) {
    Serial.printf("║  Accel X:    %8.2f m/s²                                               ║\n", accel.acceleration.x);
    Serial.printf("║  Accel Y:    %8.2f m/s²                                               ║\n", accel.acceleration.y);
    Serial.printf("║  Accel Z:    %8.2f m/s²                                               ║\n", accel.acceleration.z);
    Serial.printf("║  Total:      %8.2f m/s² (Threshold: %.1f)                            ║\n", currentTotalAccel, ACCEL_THRESHOLD);
    Serial.printf("║  Roll:       %8.2f°                                                   ║\n", currentRoll);
    Serial.printf("║  Gyro X:     %8.2f °/s                                                ║\n", gyro.gyro.x * 180 / PI);
    Serial.printf("║  Gyro Y:     %8.2f °/s                                                ║\n", gyro.gyro.y * 180 / PI);
    Serial.printf("║  Gyro Z:     %8.2f °/s                                                ║\n", gyro.gyro.z * 180 / PI);
    Serial.printf("║  Temp:       %8.2f °C                                                 ║\n", temp.temperature);
  } else {
    Serial.println("║  MPU6050 not available                                                    ║");
  }
  Serial.println("╠═══════════════════════════════════════════════════════════════════════════╣");
  
  // Digital Sensors
  Serial.println("║ 🔌 DIGITAL SENSORS                                                        ║");
  Serial.println("╠═══════════════════════════════════════════════════════════════════════════╣");
  Serial.printf("║  Vibration:  %s  (GPIO 15: %d)                                          ║\n", 
    digitalRead(vibrationSensorPin) ? "DETECTED" : "None     ", digitalRead(vibrationSensorPin));
  Serial.printf("║  Relay:      %s  (GPIO 13: %d) - Engine %s                          ║\n", 
    digitalRead(relayPin) ? "OFF" : "ON ", digitalRead(relayPin), engineRunning ? "RUNNING" : "STOPPED");
  Serial.printf("║  Buzzer:     %s  (GPIO 12: %d)                                          ║\n", 
    digitalRead(buzzerPin) ? "ON " : "OFF", digitalRead(buzzerPin));
  Serial.printf("║  LED:        %s  (GPIO 2: %d)                                           ║\n", 
    digitalRead(lightIndicatorPin) ? "ON " : "OFF", digitalRead(lightIndicatorPin));
  Serial.println("╠═══════════════════════════════════════════════════════════════════════════╣");
  
  // System Status
  Serial.println("║ ⚙️  SYSTEM STATUS                                                          ║");
  Serial.println("╠═══════════════════════════════════════════════════════════════════════════╣");
  Serial.printf("║  Engine:         %s                                                    ║\n", 
    engineRunning ? "RUNNING ✅" : "STOPPED 🛑");
  Serial.printf("║  Crash:          %s                                                    ║\n", 
    crashDetected ? "DETECTED ⚠️ " : "None ✅    ");
  Serial.printf("║  Alcohol:        %s                                                    ║\n", 
    alcoholDetected ? "DETECTED 🚨" : "Safe ✅    ");
  Serial.printf("║  Anti-Theft:     %s                                                    ║\n", 
    antiTheftArmed ? "ARMED 🛡️  " : "Disarmed   ");
  Serial.printf("║  Auto Control:   %s                                                    ║\n", 
    autoEngineControl ? "Enabled ✅ " : "Disabled   ");
  Serial.printf("║  Uptime:         %lu seconds                                              ║\n", 
    millis() / 1000);
  Serial.println("╚═══════════════════════════════════════════════════════════════════════════╝\n");
}

// ✅ NEW: Read GPS data continuously
void readGPSData() {
  while (gpsSerial.available() > 0) {
    char c = gpsSerial.read();
    gps.encode(c);
  }
  
  if (gps.location.isUpdated()) {
    gpsReady = true;
    gpsLatitude = gps.location.lat();
    gpsLongitude = gps.location.lng();
  }
  
  if (gps.altitude.isUpdated()) {
    gpsAltitude = gps.altitude.meters();
  }
  
  if (gps.speed.isUpdated()) {
    gpsSpeed = gps.speed.kmph();
  }
  
  if (gps.satellites.isUpdated()) {
    gpsSatellites = gps.satellites.value();
  }
}

// ✅ NEW: Enhanced serial command handler
void handleSerialCommands() {
  if (!Serial.available()) return;
  
  String cmd = Serial.readStringUntil('\n');
  cmd.trim();
  cmd.toUpperCase();
  
  Serial.println("\n[COMMAND] Received: " + cmd);
  
  if (cmd == "START ENGINE") {
    startEngine();
  }
  else if (cmd == "STOP ENGINE") {
    stopEngine();
  }
  else if (cmd == "STATUS") {
    displayComprehensiveDiagnostics();
  }
  else if (cmd == "GPS") {
    displayGPSDetails();
  }
  else if (cmd == "GSM") {
    displayGSMDetails();
  }
  else if (cmd == "MPU") {
    displayMPU6050Details();
  }
  else if (cmd == "SENSORS") {
    displayAllSensors();
  }
  else if (cmd == "TEST VIBRATION") {
    testVibrationSensor();
  }
  else if (cmd == "TEST SMS") {
    testSMS();
  }
  else if (cmd == "TEST RELAY") {
    testRelay();
  }
  else if (cmd == "ARM THEFT") {
    antiTheftArmed = true;
    antiTheftEnabled = true;
    Serial.println("[ANTI-THEFT] ✅ ARMED manually");
  }
  else if (cmd == "DISARM THEFT") {
    antiTheftArmed = false;
    antiTheftEnabled = false;
    theftDetectionCount = 0;
    Serial.println("[ANTI-THEFT] ✅ DISARMED");
  }
  else if (cmd == "CRASH") {
    triggerCrashShutdown(20.0, 45.0);
  }
  else {
    Serial.println("[COMMAND] ❌ Unknown command. Type STATUS for help.");
  }
}

void displayGPSDetails() {
  Serial.println("\n╔═══════════════════════════════════════════════════════════╗");
  Serial.println("║                    GPS MODULE DETAILS                     ║");
  Serial.println("╠═══════════════════════════════════════════════════════════╣");
  Serial.printf("║  Status:       %s                                      ║\n", gpsReady ? "✅ READY" : "❌ NO FIX");
  Serial.printf("║  Satellites:   %d                                          ║\n", gpsSatellites);
  Serial.printf("║  Latitude:     %12.6f°                          ║\n", gpsLatitude);
  Serial.printf("║  Longitude:    %12.6f°                          ║\n", gpsLongitude);
  Serial.printf("║  Altitude:     %8.2f m                              ║\n", gpsAltitude);
  Serial.printf("║  Speed:        %8.2f km/h                           ║\n", gpsSpeed);
  Serial.printf("║  HDOP:         %8.2f                                ║\n", gps.hdop.hdop());
  Serial.printf("║  Date:         %02d/%02d/%04d                              ║\n", 
    gps.date.day(), gps.date.month(), gps.date.year());
  Serial.printf("║  Time:         %02d:%02d:%02d UTC                           ║\n", 
    gps.time.hour(), gps.time.minute(), gps.time.second());
  Serial.printf("║  Chars:        %lu processed                           ║\n", gps.charsProcessed());
  Serial.printf("║  Sentences:    %lu valid, %lu failed                  ║\n", 
    gps.sentencesWithFix(), gps.failedChecksum());
  
  if (!gpsReady) {
    Serial.println("╠═══════════════════════════════════════════════════════════╣");
    Serial.println("║  ⚠️  TROUBLESHOOTING:                                     ║");
    Serial.println("║  • Move to open area with clear sky view                 ║");
    Serial.println("║  • Check antenna connection                               ║");
    Serial.println("║  • Wait 30-60 seconds for initial fix                    ║");
    Serial.println("║  • Verify wiring: RX:16, TX:17, VCC, GND                 ║");
  }
  Serial.println("╚═══════════════════════════════════════════════════════════╝\n");
}

void displayGSMDetails() {
  Serial.println("\n╔═══════════════════════════════════════════════════════════╗");
  Serial.println("║                    GSM MODULE DETAILS                     ║");
  Serial.println("╠═══════════════════════════════════════════════════════════╣");
  Serial.printf("║  Status:       %s                                      ║\n", gsmReady ? "✅ READY" : "❌ NOT READY");
  
  if (gsmReady) {
    // Get fresh signal strength
    gsmSerial.println("AT+CSQ");
    delay(500);
    if (gsmSerial.available()) {
      String signal = gsmSerial.readString();
      Serial.println("║  Signal:       " + signal.substring(0, 40) + "║");
    }
    
    // Get operator
    gsmSerial.println("AT+COPS?");
    delay(500);
    if (gsmSerial.available()) {
      String op = gsmSerial.readString();
      Serial.println("║  Operator:     " + op.substring(0, 40) + "║");
    }
    
    // Get SIM status
    gsmSerial.println("AT+CPIN?");
    delay(500);
    if (gsmSerial.available()) {
      String sim = gsmSerial.readString();
      Serial.println("║  SIM:          " + sim.substring(0, 40) + "║");
    }
  } else {
    Serial.println("╠═══════════════════════════════════════════════════════════╣");
    Serial.println("║  ⚠️  TROUBLESHOOTING:                                     ║");
    Serial.println("║  • Check power supply (needs 2A)                          ║");
    Serial.println("║  • Verify SIM card inserted correctly                     ║");
    Serial.println("║  • Check antenna connection                               ║");
    Serial.println("║  • Verify wiring: RX:4, TX:5, VCC, GND                    ║");
    Serial.println("║  • Try AT command manually                                ║");
  }
  Serial.println("╚═══════════════════════════════════════════════════════════╝\n");
}

void displayMPU6050Details() {
  Serial.println("\n╔═══════════════════════════════════════════════════════════╗");
  Serial.println("║                  MPU6050 SENSOR DETAILS                   ║");
  Serial.println("╠═══════════════════════════════════════════════════════════╣");
  
  if (mpu6050Ready) {
    mpu.getEvent(&accel, &gyro, &temp);
    
    Serial.println("║  ACCELEROMETER (m/s²)                                     ║");
    Serial.printf("║    X: %8.2f  Y: %8.2f  Z: %8.2f          ║\n", 
      accel.acceleration.x, accel.acceleration.y, accel.acceleration.z);
    Serial.printf("║    Total: %8.2f (Crash threshold: %.1f)           ║\n", 
      currentTotalAccel, ACCEL_THRESHOLD);
    
    Serial.println("║                                                           ║");
    Serial.println("║  GYROSCOPE (°/s)                                          ║");
    Serial.printf("║    X: %8.2f  Y: %8.2f  Z: %8.2f          ║\n", 
      gyro.gyro.x * 180 / PI, gyro.gyro.y * 180 / PI, gyro.gyro.z * 180 / PI);
    
    Serial.println("║                                                           ║");
    Serial.printf("║  TEMPERATURE: %6.2f °C                                 ║\n", temp.temperature);
    Serial.printf("║  ROLL ANGLE:  %6.2f°                                   ║\n", currentRoll);
    Serial.println("║                                                           ║");
    Serial.println("║  CONFIGURATION:                                           ║");
    Serial.println("║    Range: ±16g                                            ║");
    Serial.println("║    Gyro:  ±2000°/s                                        ║");
    Serial.println("║    Filter: 21 Hz                                          ║");
  } else {
    Serial.println("║  ❌ MPU6050 not detected                                  ║");
    Serial.println("╠═══════════════════════════════════════════════════════════╣");
    Serial.println("║  ⚠️  TROUBLESHOOTING:                                     ║");
    Serial.println("║  • Check I2C connections (SDA:21, SCL:22)                 ║");
    Serial.println("║  • Verify power supply (3.3V or 5V)                       ║");
    Serial.println("║  • Check I2C address (default: 0x68)                      ║");
    Serial.println("║  • Try I2C scanner sketch                                 ║");
  }
  Serial.println("╚═══════════════════════════════════════════════════════════╝\n");
}

void displayAllSensors() {
  Serial.println("\n╔═══════════════════════════════════════════════════════════╗");
  Serial.println("║                   ALL SENSOR READINGS                     ║");
  Serial.println("╠═══════════════════════════════════════════════════════════╣");
  Serial.printf("║  Vibration Sensor (GPIO 15):  %d (%s)                  ║\n", 
    digitalRead(vibrationSensorPin), 
    digitalRead(vibrationSensorPin) ? "VIBRATION" : "None");
  Serial.printf("║  Relay State (GPIO 13):       %d (%s)                  ║\n", 
    digitalRead(relayPin), 
    digitalRead(relayPin) ? "OFF" : "ON");
  Serial.printf("║  Buzzer (GPIO 12):            %d (%s)                  ║\n", 
    digitalRead(buzzerPin), 
    digitalRead(buzzerPin) ? "ON" : "OFF");
  Serial.printf("║  LED (GPIO 2):                %d (%s)                  ║\n", 
    digitalRead(lightIndicatorPin), 
    digitalRead(lightIndicatorPin) ? "ON" : "OFF");
  
  if (mpu6050Ready) {
    Serial.printf("║  MPU6050 Accel Total:         %.2f m/s²                ║\n", currentTotalAccel);
    Serial.printf("║  MPU6050 Roll:                %.2f°                    ║\n", currentRoll);
    Serial.printf("║  MPU6050 Temperature:         %.2f°C                   ║\n", temp.temperature);
  }
  
  if (gpsReady) {
    Serial.printf("║  GPS Latitude:                %.6f°                ║\n", gpsLatitude);
    Serial.printf("║  GPS Longitude:               %.6f°                ║\n", gpsLongitude);
    Serial.printf("║  GPS Speed:                   %.2f km/h                ║\n", gpsSpeed);
    Serial.printf("║  GPS Satellites:              %d                        ║\n", gpsSatellites);
  }
  
  Serial.println("╚═══════════════════════════════════════════════════════════╝\n");
}

void testVibrationSensor() {
  Serial.println("\n[TEST] Testing vibration sensor for 10 seconds...");
  Serial.println("[TEST] Shake the motorcycle to test detection\n");
  
  for (int i = 0; i < 20; i++) {
    int reading = digitalRead(vibrationSensorPin);
    Serial.printf("[TEST] Reading %2d/20: %d %s\n", i+1, reading, 
      reading == HIGH ? "🔴 VIBRATION DETECTED!" : "");
    
    if (reading == HIGH) {
      digitalWrite(buzzerPin, HIGH);
      digitalWrite(lightIndicatorPin, HIGH);
      delay(200);
      digitalWrite(buzzerPin, LOW);
      digitalWrite(lightIndicatorPin, LOW);
      delay(300);
    } else {
      delay(500);
    }
  }
  
  Serial.println("\n[TEST] ✅ Vibration sensor test complete\n");
}

void testSMS() {
  Serial.println("\n[TEST] Testing SMS functionality...");
  
  if (!gsmReady) {
    Serial.println("[TEST] ❌ GSM not ready - cannot send SMS");
    return;
  }
  
  String message = "VigiLERT Test Message\n";
  message += "Time: " + String(millis() / 1000) + "s\n";
  message += "All systems operational!";
  
  Serial.println("[TEST] Sending SMS to: " + ownerPhoneNumber);
  bool success = sendSMS(ownerPhoneNumber, message);
  
  if (success) {
    Serial.println("[TEST] ✅ SMS sent successfully!");
  } else {
    Serial.println("[TEST] ❌ SMS failed");
  }
}

void testRelay() {
  Serial.println("\n[TEST] Testing relay...");
  Serial.println("[TEST] Listen for clicking sound\n");
  
  for (int i = 0; i < 5; i++) {
    Serial.printf("[TEST] Cycle %d/5: Relay ON\n", i+1);
    digitalWrite(relayPin, LOW);
    delay(1000);
    
    Serial.printf("[TEST] Cycle %d/5: Relay OFF\n", i+1);
    digitalWrite(relayPin, HIGH);
    delay(1000);
  }
  
  Serial.println("\n[TEST] ✅ Relay test complete");
  Serial.println("[TEST] Did you hear 5 clicks?\n");
}

// Include all other functions from previous version
void sendMotorcycleHeartbeat(bool isActive) {
  if (WiFi.status() != WL_CONNECTED) return;
  
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
  
  if (code == HTTP_CODE_OK) {
    Serial.printf("[HEARTBEAT] ✓ Sent (timestamp: %llu)\n", timestamp);
  }
  
  http.end();
}

void handleAntiTheftWithVibrationSensor() {
  if (!antiTheftEnabled) {
    antiTheftEnabled = true;
    engineOffTime = millis();
    Serial.println("\n[ANTI-THEFT] 🛡️ Enabled - arming in 30s...");
    return;
  }
  
  if (!antiTheftArmed && (millis() - engineOffTime >= ARM_DELAY)) {
    antiTheftArmed = true;
    theftDetectionCount = 0;
    theftAlertSent = false;
    Serial.println("\n[ANTI-THEFT] 🛡️ ARMED!");
    
    for (int i = 0; i < 2; i++) {
      digitalWrite(buzzerPin, HIGH);
      delay(100);
      digitalWrite(buzzerPin, LOW);
      delay(100);
    }
  }
  
  if (antiTheftArmed) {
    int vibrationDetected = digitalRead(vibrationSensorPin);
    
    if (vibrationDetected == HIGH) {
      unsigned long timeSinceLastVibration = millis() - lastVibrationTime;
      
      if (timeSinceLastVibration >= VIBRATION_DEBOUNCE) {
        theftDetectionCount++;
        lastVibrationTime = millis();
        
        Serial.printf("[ANTI-THEFT] ⚠️ VIBRATION! (Count: %d/%d)\n", 
          theftDetectionCount, THEFT_DETECTION_REQUIRED);
        
        digitalWrite(buzzerPin, HIGH);
        digitalWrite(lightIndicatorPin, HIGH);
        delay(200);
        digitalWrite(buzzerPin, LOW);
        digitalWrite(lightIndicatorPin, LOW);
        
        if (theftDetectionCount >= THEFT_DETECTION_REQUIRED) {
          unsigned long timeSinceLastAlert = millis() - lastTheftAlert;
          
          if (!theftAlertSent || timeSinceLastAlert >= THEFT_ALERT_COOLDOWN) {
            triggerTheftAlert();
            lastTheftAlert = millis();
            theftAlertSent = true;
            theftDetectionCount = 0;
          }
        }
      }
    }
  }
}

void triggerTheftAlert() {
  Serial.println("\n🚨🚨🚨 THEFT ALERT! 🚨🚨🚨");
  
  for (int i = 0; i < 10; i++) {
    digitalWrite(buzzerPin, HIGH);
    digitalWrite(lightIndicatorPin, HIGH);
    delay(100);
    digitalWrite(buzzerPin, LOW);
    digitalWrite(lightIndicatorPin, LOW);
    delay(100);
  }
  
  String location = "Location unavailable";
  if (gpsReady) {
    location = "https://maps.google.com/?q=" + 
               String(gpsLatitude, 6) + "," + 
               String(gpsLongitude, 6);
  }
  
  String message = "VIGILERT THEFT ALERT!\n";
  message += "Unauthorized movement!\n";
  message += location;
  
  Serial.println("[ANTI-THEFT] 📱 Sending SMS...");
  sendSMS(ownerPhoneNumber, message);
}

bool sendSMS(String phoneNumber, String message) {
  if (!gsmReady) return false;
  
  Serial.println("[GSM] 📱 Sending SMS to: " + phoneNumber);
  
  gsmSerial.println("AT+CMGF=1");
  delay(1000);
  
  gsmSerial.println("AT+CMGS=\"" + phoneNumber + "\"");
  delay(1000);
  
  gsmSerial.print(message);
  delay(500);
  
  gsmSerial.write(26);
  delay(5000);
  
  bool success = false;
  if (gsmSerial.available()) {
    String response = gsmSerial.readString();
    Serial.println("[GSM] Response: " + response);
    if (response.indexOf("OK") != -1 || response.indexOf("+CMGS") != -1) {
      success = true;
    }
  }
  
  return success;
}

void triggerCrashShutdown(float impact, float roll) {
  Serial.println("\n⚠️⚠️⚠️ CRASH DETECTED! ⚠️⚠️⚠️");
  Serial.printf("[CRASH] Impact: %.2f g | Roll: %.1f°\n", impact, roll);
  
  digitalWrite(relayPin, HIGH);
  engineRunning = false;
  
  Serial.printf("[CRASH] Relay OFF (GPIO %d = %d)\n", relayPin, digitalRead(relayPin));
  
  sendCrashToFirebase(impact, roll);
  crashDetected = true;
  lastCrashTime = millis();
  
  for (int i = 0; i < 5; i++) {
    digitalWrite(buzzerPin, HIGH);
    digitalWrite(lightIndicatorPin, HIGH);
    delay(200);
    digitalWrite(buzzerPin, LOW);
    digitalWrite(lightIndicatorPin, LOW);
    delay(200);
  }
}

void startEngine() {
  if (alcoholDetected) {
    Serial.println("\n❌ ENGINE START BLOCKED - ALCOHOL!");
    digitalWrite(relayPin, HIGH);
    for (int i = 0; i < 3; i++) {
      digitalWrite(buzzerPin, HIGH);
      delay(200);
      digitalWrite(buzzerPin, LOW);
      delay(200);
    }
    return;
  }
  
  Serial.println("\n✅ Starting engine...");
  digitalWrite(relayPin, LOW);
  engineRunning = true;
  Serial.printf("[ENGINE] Relay ON (GPIO %d = %d)\n", relayPin, digitalRead(relayPin));
  
  if (antiTheftArmed) {
    antiTheftArmed = false;
    antiTheftEnabled = false;
    Serial.println("[ANTI-THEFT] 🔓 Disarmed");
  }
}

void stopEngine() {
  Serial.println("\n🛑 Stopping engine...");
  digitalWrite(relayPin, HIGH);
  engineRunning = false;
  Serial.printf("[ENGINE] Relay OFF (GPIO %d = %d)\n", relayPin, digitalRead(relayPin));
  
  engineOffTime = millis();
  antiTheftEnabled = true;
  antiTheftArmed = false;
  Serial.println("[ANTI-THEFT] 🛡️ Will arm in 30s...");
}

void sendCrashToFirebase(float impact, float roll) {
  if (WiFi.status() != WL_CONNECTED) return;
  
  StaticJsonDocument<256> doc;
  doc["timestamp"] = millis();
  doc["impactStrength"] = impact;
  doc["roll"] = roll;
  doc["leanAngle"] = abs(roll);
  doc["hasGPS"] = gpsReady;
  if (gpsReady) {
    doc["lat"] = gpsLatitude;
    doc["lng"] = gpsLongitude;
  }
  
  String payload;
  serializeJson(doc, payload);
  
  HTTPClient http;
  http.begin(firebaseHost + crashPath);
  http.addHeader("Content-Type", "application/json");
  http.POST(payload);
  http.end();
}

void sendLiveToFirebase() {
  if (WiFi.status() != WL_CONNECTED) return;
  
  StaticJsonDocument<512> doc;
  doc["engineRunning"] = engineRunning;
  
  if (mpu6050Ready) {
    doc["mpu6050"]["accelX"] = accel.acceleration.x;
    doc["mpu6050"]["accelY"] = accel.acceleration.y;
    doc["mpu6050"]["accelZ"] = accel.acceleration.z;
    doc["mpu6050"]["totalAccel"] = currentTotalAccel;
    doc["mpu6050"]["roll"] = currentRoll;
  }
  
  if (gpsReady) {
    doc["gps"]["lat"] = gpsLatitude;
    doc["gps"]["lng"] = gpsLongitude;
    doc["gps"]["speed"] = gpsSpeed;
    doc["gps"]["satellites"] = gpsSatellites;
  }
  
  doc["crashDetected"] = crashDetected;
  doc["alcoholDetected"] = alcoholDetected;
  doc["autoEngineControl"] = autoEngineControl;
  doc["antiTheftArmed"] = antiTheftArmed;
  doc["vibrationSensor"] = digitalRead(vibrationSensorPin);
  doc["relayState"] = digitalRead(relayPin);
  doc["relayStatus"] = digitalRead(relayPin) ? "OFF" : "ON";
  doc["timestamp"] = millis();
  
  String payload;
  serializeJson(doc, payload);
  
  HTTPClient http;
  http.begin(firebaseHost + livePath);
  http.addHeader("Content-Type", "application/json");
  http.PUT(payload);
  http.end();
}

void checkAlcoholStatus() {
  if (WiFi.status() != WL_CONNECTED) return;
  
  for (int i = 0; i < numAlcoholPaths; i++) {
    HTTPClient http;
    http.begin(firebaseHost + alcoholPaths[i]);
    int httpCode = http.GET();
    
    if (httpCode == HTTP_CODE_OK) {
      String response = http.getString();
      
      if (response == "null" || response.length() <= 2) {
        http.end();
        continue;
      }
      
      bool currentAlcoholState = false;
      
      if (response.indexOf("Danger") != -1 || 
          response.indexOf("danger") != -1 ||
          response.indexOf("Detected") != -1) {
        currentAlcoholState = true;
      }
      
      if (response.indexOf("sensorValue") != -1) {
        int sensorStart = response.indexOf("sensorValue") + 13;
        int sensorEnd = response.indexOf(",", sensorStart);
        if (sensorEnd == -1) sensorEnd = response.indexOf("}", sensorStart);
        
        if (sensorEnd > sensorStart) {
          String sensorStr = response.substring(sensorStart, sensorEnd);
          int sensorValue = sensorStr.toInt();
          if (sensorValue > 2000) {
            currentAlcoholState = true;
          }
        }
      }
      
      if (currentAlcoholState != lastAlcoholState) {
        lastAlcoholState = currentAlcoholState;
        
        if (currentAlcoholState) {
          Serial.println("\n🚨 ALCOHOL DETECTED!");
          alcoholDetected = true;
          digitalWrite(relayPin, HIGH);
          if (engineRunning) {
            triggerAlcoholShutdown();
          }
        } else {
          Serial.println("\n✅ Alcohol cleared");
          alcoholDetected = false;
        }
      }
      
      http.end();
      return;
    }
    http.end();
  }
}

void triggerAlcoholShutdown() {
  Serial.println("\n🚨 ALCOHOL - EMERGENCY SHUTDOWN!");
  
  digitalWrite(relayPin, HIGH);
  engineRunning = false;
  
  for (int i = 0; i < 10; i++) {
    digitalWrite(buzzerPin, HIGH);
    digitalWrite(lightIndicatorPin, HIGH);
    delay(500);
    digitalWrite(buzzerPin, LOW);
    digitalWrite(lightIndicatorPin, LOW);
    delay(300);
  }
}

void handleDashboardButton() {
  if (millis() - lastButtonCheck > BUTTON_CHECK_INTERVAL) {
    checkDashboardButton();
    lastButtonCheck = millis();
  }
}

void checkDashboardButton() {
  if (WiFi.status() != WL_CONNECTED) return;
  
  HTTPClient http;
  http.begin(firebaseHost + buttonPath);
  int httpCode = http.GET();
  
  if (httpCode == HTTP_CODE_OK) {
    String response = http.getString();
    bool currentButtonState = (response.indexOf("true") != -1);
    
    if (currentButtonState != lastDashboardButtonState) {
      lastDashboardButtonState = currentButtonState;
      
      if (currentButtonState) {
        Serial.println("\n🖥️ DASHBOARD BUTTON PRESSED!");
        
        if (!engineRunning) {
          engineStartRequested = true;
          startEngine();
        } else {
          stopEngine();
          engineStartRequested = false;
        }
        
        clearDashboardButton();
      }
    }
  }
  http.end();
}

void clearDashboardButton() {
  HTTPClient http;
  http.begin(firebaseHost + buttonPath);
  http.addHeader("Content-Type", "application/json");
  http.PUT("false");
  http.end();
}
