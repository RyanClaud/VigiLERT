/*
VigiLERT ESP32 Motorcycle Module - COMPLETE VERSION
✅ All sensor data sent to Firebase for real-time dashboard monitoring
✅ MPU6050 data, GPS quality, Engine status, Helmet switch, GSM signal
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
const char* ssid = "Kupal";
const char* password = "DEEABE7H406";
const String firebaseHost = "https://vigilance-shield-default-rtdb.firebaseio.com";
const String userUID = "MnzBjTBslZNijOkq732PE91hHa23";
const String firebaseAuth = "";

// ======= PIN ASSIGNMENTS =======
const int batteryPin = 34;
const int helmetSwitchPin = 39;
const int headlightPin = 32;
const int taillightPin = 33;
const int leftSignalPin = 25;
const int rightSignalPin = 26;
const int relayPin = 27;
const int vibrationPin = 14;
const int buzzerPin = 12;
const int lightIndicatorPin = 13;

// GSM module pins (UART2)
#define GSM_RX 4
#define GSM_TX 5
HardwareSerial gsmSerial(2);

// GPS module pins (UART1)
HardwareSerial gpsSerial(1);
TinyGPSPlus gps;

// MPU6050
Adafruit_MPU6050 mpu;
sensors_event_t accel, gyro, temp;

// Engine control
bool engineRunning = false;
String lastSender = "";
const String gsmPassword = "1234";
const String authorizedNumber1 = "+639675715673";
const String authorizedNumber2 = "+639542583234";

// Trip state
float currentStartLat = 0.0;
float currentStartLng = 0.0;
unsigned long tripStartTime = 0;
float maxRecordedSpeed = 0.0;

// Crash & helmet state
bool crashDetected = false;
const float ACCEL_THRESHOLD = 15.0;
bool isHelmetOn = false;
unsigned long lastHelmetOffMillis = 0;

// Safety interlock
bool isHelmetModuleActive = false;
unsigned long lastHelmetModuleCheck = 0;
const unsigned long HELMET_CHECK_INTERVAL = 5000;

// Speed limit
float currentSpeedLimit = 80.0;
unsigned long lastSpeedCheck = 0;

// ✅ NEW: GSM signal tracking
int gsmSignalStrength = 0;
unsigned long lastGSMCheck = 0;
const unsigned long GSM_CHECK_INTERVAL = 10000; // Check every 10 seconds

// ✅ NEW: Store current sensor readings for live updates
float currentRoll = 0.0;
float currentTotalAccel = 0.0;

// ✅ FIX: Crash detection based on CHANGE in acceleration (not absolute value)
float previousTotalAccel = 9.81; // Initialize with gravity
unsigned long lastCrashCheck = 0;
const unsigned long CRASH_CHECK_INTERVAL = 100; // Check every 100ms

// Firebase paths
const String tripsPath = "/helmet_public/" + userUID + "/trips.json?auth=" + firebaseAuth;
const String livePath = "/helmet_public/" + userUID + "/live.json?auth=" + firebaseAuth;
const String crashPath = "/helmet_public/" + userUID + "/crashes.json?auth=" + firebaseAuth;
const String helmetStatusPath = "/helmet_public/" + userUID + "/helmetStatus/status.json?auth=" + firebaseAuth;
const String devicesPath = "/helmet_public/" + userUID + "/devices.json?auth=" + firebaseAuth;

// ======= SETUP =======
void setup() {
  Serial.begin(115200);
  delay(100);
  Serial.println("\n[SETUP] Starting VigiLERT System - COMPLETE VERSION...");

  Wire.begin(21, 22);
  while (!mpu.begin()) {
    Serial.println("[ERROR] Failed to find MPU6050 chip. Retrying in 2s...");
    delay(2000);
  }
  Serial.println("[SUCCESS] MPU6050 initialized.");
  
  mpu.setAccelerometerRange(MPU6050_RANGE_16_G);
  mpu.setGyroRange(MPU6050_RANGE_2000_DEG);
  mpu.setFilterBandwidth(MPU6050_BAND_21_HZ);

  // ✅ Enhanced GPS initialization
  gpsSerial.begin(9600, SERIAL_8N1, 16, 17);
  Serial.println("[SETUP] GPS Serial started on RX:16, TX:17");
  
  // Wait for GPS to initialize
  delay(1000);
  
  // Clear any garbage data
  while (gpsSerial.available()) {
    gpsSerial.read();
  }
  
  Serial.println("[GPS] Waiting for GPS fix... This may take 30-60 seconds in open area.");
  Serial.println("[GPS] Make sure GPS antenna has clear view of sky.");

  gsmSerial.begin(9600, SERIAL_8N1, GSM_RX, GSM_TX);
  Serial.println("[SETUP] GSM Serial started.");

  pinMode(helmetSwitchPin, INPUT_PULLUP);
  pinMode(headlightPin, INPUT);
  pinMode(taillightPin, INPUT);
  pinMode(leftSignalPin, INPUT);
  pinMode(rightSignalPin, INPUT);
  pinMode(relayPin, OUTPUT);
  pinMode(vibrationPin, INPUT);
  pinMode(buzzerPin, OUTPUT);
  pinMode(lightIndicatorPin, OUTPUT);

  digitalWrite(relayPin, LOW);
  digitalWrite(buzzerPin, LOW);
  digitalWrite(lightIndicatorPin, LOW);

  connectToWiFi();
  setupOTA();
  initializeGSM();

  updateMotorcycleDeviceStatus(true);
  startNewTrip();
  currentSpeedLimit = getSpeedLimitFromFirebase();

  Serial.println("[SUCCESS] Setup complete. All sensors active.");
  Serial.println("\n📋 SERIAL COMMANDS AVAILABLE:");
  Serial.println("   Type 'CRASH' to test crash detection");
  Serial.println("   Type 'MPU' to test MPU6050 sensor");
  Serial.println("   Type 'GPS' for GPS diagnostics");
  Serial.println("   Type 'HELP' for all commands\n");
  
  // ✅ Print initial GPS diagnostics after 5 seconds
  delay(5000);
  printGPSDiagnostics();
}

// ======= MAIN LOOP =======
void loop() {
  ArduinoOTA.handle();
  
  // ✅ Serial command handler for testing
  if (Serial.available()) {
    String cmd = Serial.readStringUntil('\n');
    cmd.trim();
    cmd.toUpperCase();
    
    if (cmd == "TEST CRASH" || cmd == "CRASH") {
      Serial.println("\n🧪 MANUAL CRASH TEST TRIGGERED!");
      Serial.println("Simulating crash event...");
      
      // Simulate crash with high acceleration
      float testAccel = 20.0; // Above threshold
      float testRoll = currentRoll;
      
      if (gps.location.isValid()) {
        Serial.printf("📍 Sending test crash WITH GPS: %.6f, %.6f\n", 
                      gps.location.lat(), gps.location.lng());
        Serial.printf("   Impact: %.2f g | Roll: %.1f°\n", testAccel, testRoll);
        sendCrashEventToFirebase(gps.location.lat(), gps.location.lng(), testAccel, testRoll);
      } else {
        Serial.println("⚠️ Sending test crash WITHOUT GPS");
        Serial.printf("   Impact: %.2f g | Roll: %.1f°\n", testAccel, testRoll);
        sendCrashEventToFirebaseNoGPS(testAccel, testRoll);
      }
      
      triggerAlert();
      Serial.println("✓ Test crash sent to Firebase!");
      Serial.println("✓ Check dashboard for crash marker and alert!");
      
      // Mark as crash detected to prevent auto-detection during test
      crashDetected = true;
    }
    else if (cmd == "MPU TEST" || cmd == "MPU") {
      Serial.println("\n🧪 MPU6050 TEST:");
      Serial.printf("Accel X: %.2f | Y: %.2f | Z: %.2f\n", 
                    accel.acceleration.x, accel.acceleration.y, accel.acceleration.z);
      Serial.printf("Total Accel: %.2f g\n", currentTotalAccel);
      Serial.printf("Roll: %.1f°\n", currentRoll);
      Serial.printf("Crash Threshold: %.2f g\n", ACCEL_THRESHOLD);
      
      if (currentTotalAccel < 0.1) {
        Serial.println("⚠️ MPU6050 NOT WORKING! Check wiring!");
      } else {
        Serial.println("✓ MPU6050 is working!");
      }
    }
    else if (cmd == "HELP") {
      Serial.println("\n📋 AVAILABLE COMMANDS:");
      Serial.println("  CRASH or TEST CRASH - Trigger test crash event");
      Serial.println("  MPU or MPU TEST     - Test MPU6050 sensor");
      Serial.println("  GPS                 - Show GPS diagnostics");
      Serial.println("  HELP                - Show this help");
    }
    else if (cmd == "GPS") {
      printGPSDiagnostics();
    }
  }
  
  // ✅ Enhanced GPS reading with debugging
  while (gpsSerial.available() > 0) {
    char c = gpsSerial.read();
    gps.encode(c);
    
    // Debug: Print raw GPS data every 5 seconds
    static unsigned long lastGPSDebug = 0;
    if (millis() - lastGPSDebug > 5000) {
      Serial.print("[GPS RAW] ");
      Serial.print(c);
      if (c == '\n') {
        lastGPSDebug = millis();
      }
    }
  }
  
  handleGSM();

  // Sensor readings
  bool helmetSwitchState = (digitalRead(helmetSwitchPin) == LOW);
  bool headlightOn = digitalRead(headlightPin);
  bool taillightOn = digitalRead(taillightPin);
  bool leftSignalOn = digitalRead(leftSignalPin);
  bool rightSignalOn = digitalRead(rightSignalPin);
  float batteryVoltage = analogRead(batteryPin) * (3.3f / 4095.0f) * 4.3;

  // MPU6050 data
  mpu.getEvent(&accel, &gyro, &temp);
  currentTotalAccel = sqrt(accel.acceleration.x * accel.acceleration.x +
                           accel.acceleration.y * accel.acceleration.y +
                           accel.acceleration.z * accel.acceleration.z);
  currentRoll = atan2(accel.acceleration.y, accel.acceleration.z) * 180.0 / PI;

  // ✅ Periodic GSM signal check
  if (millis() - lastGSMCheck > GSM_CHECK_INTERVAL) {
    gsmSignalStrength = getGSMSignalStrength();
    lastGSMCheck = millis();
  }

  // Periodic helmet module check
  if (millis() - lastHelmetModuleCheck > HELMET_CHECK_INTERVAL) {
    checkHelmetModuleStatus();
    lastHelmetModuleCheck = millis();
  }

  // ✅ FIX: Standardized heartbeat timing (reduced to 5 seconds to prevent flickering)
  static unsigned long lastMotorcycleHeartbeat = 0;
  const unsigned long MOTORCYCLE_HEARTBEAT_INTERVAL = 5000; // 5 seconds
  if (millis() - lastMotorcycleHeartbeat > MOTORCYCLE_HEARTBEAT_INTERVAL) {
    Serial.println("\n[HEARTBEAT] Sending motorcycle heartbeat to Firebase...");
    updateMotorcycleDeviceStatus(true);
    lastMotorcycleHeartbeat = millis();
  }

  // ✅ Periodic GPS diagnostics (every 30 seconds)
  static unsigned long lastGPSDiag = 0;
  if (millis() - lastGPSDiag > 30000) {
    printGPSDiagnostics();
    lastGPSDiag = millis();
  }

  // ✅ Enhanced Serial monitoring with detailed GPS info
  static unsigned long lastPrint = 0;
  if (millis() - lastPrint > 1000) {
    Serial.printf("\n[SENSOR] Accel: %.2f | Roll: %.1f° | Battery: %.2fV | Helmet: %s",
                  currentTotalAccel, currentRoll, batteryVoltage, helmetSwitchState ? "ON" : "OFF");
    
    // Detailed GPS information
    if (gps.location.isValid()) {
      Serial.printf(" | GPS: ✓ VALID");
      Serial.printf("\n[GPS] Lat: %.6f | Lng: %.6f | Speed: %.1f km/h",
                    gps.location.lat(), gps.location.lng(), gps.speed.kmph());
      Serial.printf(" | Sats: %d | HDOP: %.1f",
                    gps.satellites.value(), gps.hdop.hdop());
      if (gps.date.isValid() && gps.time.isValid()) {
        Serial.printf(" | Time: %02d:%02d:%02d",
                      gps.time.hour(), gps.time.minute(), gps.time.second());
      }
    } else {
      Serial.printf(" | GPS: ✗ NO FIX");
      Serial.printf("\n[GPS] Chars: %d | Sentences: %d | Failed: %d",
                    gps.charsProcessed(), gps.sentencesWithFix(), gps.failedChecksum());
      if (gps.charsProcessed() < 10) {
        Serial.print(" | ⚠️ NO DATA FROM GPS MODULE!");
      }
    }
    
    Serial.printf(" | GSM: %d%% | Engine: %s", 
                  gsmSignalStrength, engineRunning ? "RUNNING" : "STOPPED");
    lastPrint = millis();
  }

  // ✅ MPU6050 Diagnostics - Check if sensor is working
  static unsigned long lastMPUCheck = 0;
  if (millis() - lastMPUCheck > 10000) { // Every 10 seconds
    if (currentTotalAccel < 0.1) {
      Serial.println("\n⚠️ WARNING: MPU6050 reading 0.00! Sensor may not be working!");
      Serial.println("   Check: 1) MPU6050 wiring (SDA:21, SCL:22)");
      Serial.println("          2) MPU6050 power (3.3V)");
      Serial.println("          3) I2C address (0x68 or 0x69)");
    }
    lastMPUCheck = millis();
  }

  // ✅ FIX: Crash detection based on SUDDEN CHANGE in acceleration (not absolute value)
  if (millis() - lastCrashCheck > CRASH_CHECK_INTERVAL) {
    lastCrashCheck = millis();
    
    // Calculate CHANGE in acceleration (jerk detection)
    float accelChange = abs(currentTotalAccel - previousTotalAccel);
    
    // ✅ Detect crash based on SUDDEN CHANGE AND/OR severe roll (combined conditions)
    bool suddenImpact = (accelChange > 5.0);
    bool severeRoll = (currentRoll < -60 || currentRoll > 60); // Increased threshold to 60°
    bool moderateImpact = (accelChange > 3.0);
    
    // ✅ Crash if: (High impact) OR (Moderate impact + Severe roll)
    if ((suddenImpact || (moderateImpact && severeRoll)) && !crashDetected) {
      Serial.println("\n⚠️⚠️⚠️ CRASH DETECTED! ⚠️⚠️⚠️");
      Serial.printf("Sudden change: %.2f g | Roll: %.1f° | Speed: %.1f km/h\n", 
                    accelChange, currentRoll, gps.speed.kmph());
      Serial.printf("Current accel: %.2f g | Previous: %.2f g\n", 
                    currentTotalAccel, previousTotalAccel);
      Serial.printf("Conditions: Impact=%.2fg (>5.0?) | Roll=%.1f° (>60?) | Moderate=%.2fg (>3.0?)\n",
                    accelChange, abs(currentRoll), accelChange);
      
      // ✅ Wait 500ms and re-check to confirm it's not just a bump
      delay(500);
      
      // ✅ Re-check acceleration to confirm crash
      mpu.getEvent(&accel, &gyro, &temp);
      float confirmAccel = sqrt(accel.acceleration.x * accel.acceleration.x +
                               accel.acceleration.y * accel.acceleration.y +
                               accel.acceleration.z * accel.acceleration.z);
      float confirmChange = abs(confirmAccel - previousTotalAccel);
      
      Serial.printf("Confirmation check: Change = %.2f g\n", confirmChange);
      
      // ✅ Only send if still showing significant change (real crash, not vibration)
      if (confirmChange > 3.0) {
        // ✅ ONLY send crash event to /crashes path (creates map marker)
        if (gps.location.isValid()) {
          Serial.printf("📍 Sending CONFIRMED crash to Firebase WITH GPS: %.6f, %.6f\n", 
                        gps.location.lat(), gps.location.lng());
          Serial.println("⚠️ This will create a marker on the dashboard map!");
          sendCrashEventToFirebase(gps.location.lat(), gps.location.lng(), accelChange, currentRoll);
        } else {
          Serial.println("⚠️ Sending CONFIRMED crash to Firebase WITHOUT GPS (no map marker)");
          sendCrashEventToFirebaseNoGPS(accelChange, currentRoll);
        }
        
        crashDetected = true;
        triggerAlert();
        
        Serial.println("✓ Crash event sent to Firebase!");
      } else {
        Serial.println("⚠️ False alarm - just a bump/vibration, not a crash");
        Serial.printf("   (Confirmation change %.2f g < 3.0 g threshold)\n", confirmChange);
      }
    }
    
    // Update previous acceleration for next comparison
    previousTotalAccel = currentTotalAccel;
  }

  // Clear crash state when acceleration is stable
  if (abs(currentTotalAccel - previousTotalAccel) < 1.0 && currentRoll > -10 && currentRoll < 10 && crashDetected) {
    crashDetected = false;
    Serial.println("[INFO] Crash state cleared - acceleration stable.");
  }

  // Unauthorized movement
  if (!engineRunning && digitalRead(vibrationPin) == HIGH) {
    Serial.println("[SECURITY] Unauthorized movement!");
    triggerAlert();
    sendAlertSMS();
  }

  // Helmet state & trip management
  handleHelmetState(helmetSwitchState, batteryVoltage, headlightOn, taillightOn, leftSignalOn, rightSignalOn);

  // ✅ Firebase live updates with ALL sensor data
  if (gps.location.isValid()) {
    float speedKmph = gps.speed.kmph();
    sendLiveToFirebase(gps.location.lat(), gps.location.lng(), speedKmph, batteryVoltage,
                       headlightOn, taillightOn, leftSignalOn, rightSignalOn, helmetSwitchState);
    if (speedKmph > maxRecordedSpeed) maxRecordedSpeed = speedKmph;
  } else {
    sendLiveToFirebaseNoGPS(batteryVoltage, headlightOn, taillightOn, leftSignalOn, rightSignalOn, helmetSwitchState);
  }

  // Periodic speed limit update
  if (millis() - lastSpeedCheck > 60000) {
    currentSpeedLimit = getSpeedLimitFromFirebase();
    lastSpeedCheck = millis();
  }

  delay(200);
}

// ======= ✅ NEW: GPS DIAGNOSTICS =======
void printGPSDiagnostics() {
  Serial.println("\n========== GPS DIAGNOSTICS ==========");
  Serial.printf("GPS Module Status: %s\n", gpsSerial.available() > 0 ? "RECEIVING DATA" : "NO DATA");
  Serial.printf("Characters Processed: %d\n", gps.charsProcessed());
  Serial.printf("Sentences with Fix: %d\n", gps.sentencesWithFix());
  Serial.printf("Failed Checksums: %d\n", gps.failedChecksum());
  
  Serial.println("\n--- Location ---");
  Serial.printf("Valid: %s\n", gps.location.isValid() ? "YES" : "NO");
  if (gps.location.isValid()) {
    Serial.printf("Latitude: %.6f\n", gps.location.lat());
    Serial.printf("Longitude: %.6f\n", gps.location.lng());
    Serial.printf("Age: %d ms\n", gps.location.age());
  }
  
  Serial.println("\n--- Satellites ---");
  Serial.printf("Valid: %s\n", gps.satellites.isValid() ? "YES" : "NO");
  if (gps.satellites.isValid()) {
    Serial.printf("Count: %d\n", gps.satellites.value());
  }
  
  Serial.println("\n--- HDOP (Accuracy) ---");
  Serial.printf("Valid: %s\n", gps.hdop.isValid() ? "YES" : "NO");
  if (gps.hdop.isValid()) {
    Serial.printf("HDOP: %.1f (Lower is better, <5 is good)\n", gps.hdop.hdop());
  }
  
  Serial.println("\n--- Speed ---");
  Serial.printf("Valid: %s\n", gps.speed.isValid() ? "YES" : "NO");
  if (gps.speed.isValid()) {
    Serial.printf("Speed: %.1f km/h\n", gps.speed.kmph());
  }
  
  Serial.println("\n--- Date/Time ---");
  if (gps.date.isValid() && gps.time.isValid()) {
    Serial.printf("Date: %04d-%02d-%02d\n", gps.date.year(), gps.date.month(), gps.date.day());
    Serial.printf("Time: %02d:%02d:%02d UTC\n", gps.time.hour(), gps.time.minute(), gps.time.second());
  } else {
    Serial.println("Date/Time: INVALID");
  }
  
  Serial.println("\n--- Troubleshooting ---");
  if (gps.charsProcessed() < 10) {
    Serial.println("⚠️ PROBLEM: GPS module not sending data!");
    Serial.println("   Check: 1) GPS RX/TX wiring (RX:16, TX:17)");
    Serial.println("          2) GPS power supply (3.3V or 5V)");
    Serial.println("          3) GPS antenna connection");
  } else if (!gps.location.isValid()) {
    Serial.println("⚠️ PROBLEM: GPS receiving data but no fix!");
    Serial.println("   Check: 1) Move to open area with clear sky view");
    Serial.println("          2) Wait 30-60 seconds for initial fix");
    Serial.println("          3) Check GPS antenna is connected");
  } else {
    Serial.println("✓ GPS is working correctly!");
  }
  Serial.println("=====================================\n");
}

// ======= ✅ NEW: GET GSM SIGNAL STRENGTH =======
int getGSMSignalStrength() {
  gsmSerial.println("AT+CSQ");
  delay(500);
  
  String response = "";
  unsigned long timeout = millis() + 1000;
  while (millis() < timeout && gsmSerial.available()) {
    response += (char)gsmSerial.read();
  }
  
  // Parse response: +CSQ: <rssi>,<ber>
  int csqIndex = response.indexOf("+CSQ:");
  if (csqIndex != -1) {
    int commaIndex = response.indexOf(",", csqIndex);
    if (commaIndex != -1) {
      String rssiStr = response.substring(csqIndex + 6, commaIndex);
      rssiStr.trim();
      int rssi = rssiStr.toInt();
      
      // Convert RSSI to percentage (0-31 range, 99 = no signal)
      if (rssi == 99 || rssi == 0) return 0;
      return (rssi * 100) / 31;
    }
  }
  
  return 0; // No signal or parse error
}

// ======= ✅ ENHANCED: SEND LIVE DATA WITH ALL SENSORS =======
void sendLiveToFirebase(float lat, float lng, float speed, float v, bool h, bool t, bool l, bool r, bool helmetSwitch) {
  StaticJsonDocument<1024> doc; // Increased size for more data
  
  // GPS data
  doc["locationLat"] = lat;
  doc["locationLng"] = lng;
  doc["speed"] = speed;
  
  // Battery
  doc["batteryVoltage"] = v;
  
  // Lights
  doc["headlight"] = h;
  doc["taillight"] = t;
  doc["leftSignal"] = l;
  doc["rightSignal"] = r;
  doc["brakeLight"] = t; // Use taillight as brake light (or add separate sensor)
  
  // ✅ NEW: MPU6050 data
  doc["mpu6050"]["accelX"] = accel.acceleration.x;
  doc["mpu6050"]["accelY"] = accel.acceleration.y;
  doc["mpu6050"]["accelZ"] = accel.acceleration.z;
  doc["mpu6050"]["gyro"] = currentRoll;
  doc["mpu6050"]["totalAccel"] = currentTotalAccel;
  
  // ✅ NEW: GPS quality
  if (gps.hdop.isValid()) {
    doc["gps"]["accuracy"] = String(gps.hdop.hdop()) + "m";
  } else {
    doc["gps"]["accuracy"] = "N/A";
  }
  doc["gps"]["satellites"] = gps.satellites.isValid() ? gps.satellites.value() : 0;
  
  // ✅ NEW: GSM data
  doc["gsm"]["signal"] = String(gsmSignalStrength) + "%";
  doc["gsm"]["network"] = gsmSignalStrength > 0 ? "4G" : "Offline";
  
  // ✅ NEW: Engine status
  doc["engineRunning"] = engineRunning;
  
  // ✅ NEW: Helmet switch state
  doc["helmetSwitch"] = helmetSwitch;
  
  // Timestamp
  doc["timestamp"] = millis();
  doc["source"] = "gps_module";
  
  String payload;
  serializeJson(doc, payload);
  
  HTTPClient http;
  http.begin(firebaseHost + livePath);
  http.addHeader("Content-Type", "application/json");
  int code = http.PUT(payload);
  
  if (code != HTTP_CODE_OK) {
    Serial.printf("[FIREBASE] Live update failed: %d\n", code);
  }
  
  http.end();
}

// ✅ ENHANCED: SEND LIVE DATA WITHOUT GPS
void sendLiveToFirebaseNoGPS(float v, bool h, bool t, bool l, bool r, bool helmetSwitch) {
  StaticJsonDocument<1024> doc;
  
  // Battery
  doc["batteryVoltage"] = v;
  
  // Lights
  doc["headlight"] = h;
  doc["taillight"] = t;
  doc["leftSignal"] = l;
  doc["rightSignal"] = r;
  doc["brakeLight"] = t;
  
  // ✅ NEW: MPU6050 data
  doc["mpu6050"]["accelX"] = accel.acceleration.x;
  doc["mpu6050"]["accelY"] = accel.acceleration.y;
  doc["mpu6050"]["accelZ"] = accel.acceleration.z;
  doc["mpu6050"]["gyro"] = currentRoll;
  doc["mpu6050"]["totalAccel"] = currentTotalAccel;
  
  // ✅ NEW: GPS quality (no GPS)
  doc["gps"]["accuracy"] = "No GPS";
  doc["gps"]["satellites"] = 0;
  
  // ✅ NEW: GSM data
  doc["gsm"]["signal"] = String(gsmSignalStrength) + "%";
  doc["gsm"]["network"] = gsmSignalStrength > 0 ? "4G" : "Offline";
  
  // ✅ NEW: Engine status
  doc["engineRunning"] = engineRunning;
  
  // ✅ NEW: Helmet switch state
  doc["helmetSwitch"] = helmetSwitch;
  
  // Timestamp
  doc["timestamp"] = millis();
  doc["source"] = "no_gps";
  
  String payload;
  serializeJson(doc, payload);
  
  HTTPClient http;
  http.begin(firebaseHost + livePath);
  http.addHeader("Content-Type", "application/json");
  http.PUT(payload);
  http.end();
}

// ======= REST OF THE FUNCTIONS (unchanged from original) =======
// ... (keep all other functions as they were)

unsigned long gpsToUnixTime() {
  if (gps.time.isValid() && gps.date.isValid()) {
    tmElements_t tm;
    tm.Year = gps.date.year() - 1970;
    tm.Month = gps.date.month();
    tm.Day = gps.date.day();
    tm.Hour = gps.time.hour();
    tm.Minute = gps.time.minute();
    tm.Second = gps.time.second();
    return makeTime(tm);
  }
  return millis() / 1000UL;
}

void checkHelmetModuleStatus() {
  HTTPClient http;
  String url = firebaseHost + "/helmet_public/" + userUID + "/devices/helmet.json?auth=" + firebaseAuth;
  http.begin(url);
  int code = http.GET();
  
  if (code == HTTP_CODE_OK) {
    String response = http.getString();
    StaticJsonDocument<256> doc;
    DeserializationError error = deserializeJson(doc, response);
    
    if (!error) {
      String status = doc["status"] | "Off";
      unsigned long lastHeartbeat = doc["lastHeartbeat"] | 0;
      unsigned long now = gpsToUnixTime() * 1000UL;
      unsigned long timeSinceHeartbeat = now - lastHeartbeat;
      
      isHelmetModuleActive = (status == "On" && timeSinceHeartbeat < 15000);
    }
  }
  
  http.end();
}

void updateMotorcycleDeviceStatus(bool isOn) {
  // ✅ FIX: Use a base Unix timestamp + millis for consistent heartbeat
  // This ensures the timestamp is always increasing and comparable
  unsigned long timestamp = 1700000000000UL + millis();
  
  StaticJsonDocument<128> doc;
  doc["status"] = isOn ? "On" : "Off";
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
    Serial.printf("[FIREBASE] Motorcycle heartbeat sent: %lu\n", timestamp);
  } else {
    Serial.printf("[FIREBASE] Motorcycle heartbeat failed: %d\n", code);
  }
  
  http.end();
}

bool canStartEngine() {
  bool helmetSwitchOn = (digitalRead(helmetSwitchPin) == LOW);
  bool helmetModuleOn = isHelmetModuleActive;
  bool motorcycleOn = true;
  
  Serial.println("\n[SAFETY CHECK] Engine Start Request:");
  Serial.printf("  ✓ Motorcycle Module: %s\n", motorcycleOn ? "ON" : "OFF");
  Serial.printf("  %s Helmet Module: %s\n", helmetModuleOn ? "✓" : "✗", helmetModuleOn ? "ON" : "OFF");
  Serial.printf("  %s Helmet Switch: %s\n", helmetSwitchOn ? "✓" : "✗", helmetSwitchOn ? "ON" : "OFF");
  
  return (motorcycleOn && helmetModuleOn && helmetSwitchOn);
}

void startEngine() {
  if (!canStartEngine()) {
    Serial.println("[SAFETY] Engine start BLOCKED!");
    return;
  }
  
  digitalWrite(relayPin, HIGH);
  engineRunning = true;
  sendSMS("Engine Started");
  Serial.println("[ENGINE] Started!");
}

void stopEngine() {
  digitalWrite(relayPin, LOW);
  engineRunning = false;
  sendSMS("Engine Stopped");
  Serial.println("[ENGINE] Stopped.");
}

// ======= HELPER FUNCTIONS =======
void updateHelmetStatusInFirebase(bool helmetConnected, const String& alertness, float alcoholLevel) {
  StaticJsonDocument<256> doc;
  doc["helmetConnected"] = helmetConnected;
  doc["alertnessStatus"] = alertness;
  doc["alcoholLevel"] = alcoholLevel;
  
  String payload;
  serializeJson(doc, payload);
  
  HTTPClient http;
  http.begin(firebaseHost + helmetStatusPath);
  http.addHeader("Content-Type", "application/json");
  int code = http.PUT(payload);
  
  if (code == 200) {
    Serial.printf("\n[FIREBASE] Helmet status updated: %s\n", helmetConnected ? "Connected" : "Disconnected");
  }
  http.end();
}

void handleGSM() {
  while (gsmSerial.available()) {
    String response = gsmSerial.readString();
    if (response.length() > 0) {
      Serial.println("[GSM] " + response);
    }
    if (response.indexOf("RING") != -1) handleIncomingCall(response);
    if (response.indexOf("+CMT:") != -1) handleIncomingSMS(response);
  }
}

bool initializeGSM() {
  gsmSerial.println("AT"); 
  delay(1000);
  gsmSerial.println("AT+CMGF=1"); 
  delay(1000);
  gsmSerial.println("AT+CNMI=1,2,0,0,0"); 
  delay(1000);
  return true;
}

void handleIncomingCall(String response) {
  int start = response.indexOf("\"") + 1;
  int end = response.indexOf("\"", start);
  String caller = response.substring(start, end);
  
  if (caller != authorizedNumber1 && caller != authorizedNumber2) {
    gsmSerial.println("ATH");
    return;
  }
  
  if (engineRunning) stopEngine();
  else startEngine();
  
  gsmSerial.println("ATH");
}

void handleIncomingSMS(String response) {
  int start = response.indexOf("\"") + 1;
  int end = response.indexOf("\"", start);
  String sender = response.substring(start, end);
  sender.trim();
  
  if (sender != authorizedNumber1 && sender != authorizedNumber2) return;
  
  lastSender = sender;
  
  int bodyStart = response.indexOf("\r\n", end) + 2;
  String smsBody = response.substring(bodyStart);
  smsBody.trim();
  
  if (smsBody.startsWith(gsmPassword + " ")) {
    String command = smsBody.substring(gsmPassword.length() + 1);
    command.trim();
    command.toUpperCase();
    
    if (command == "START") startEngine();
    else if (command == "STOP") stopEngine();
    else if (command == "LOCATE") {
      if (gps.location.isValid()) {
        String locMsg = "GPS Lat: " + String(gps.location.lat(), 6) + " Lng: " + String(gps.location.lng(), 6);
        sendSMS(locMsg);
      } else sendSMS("GPS location not available.");
    }
    else if (command == "STATUS") {
      String statusMsg = "Helmet Module: ";
      statusMsg += isHelmetModuleActive ? "ON" : "OFF";
      statusMsg += " | Helmet Switch: ";
      statusMsg += (digitalRead(helmetSwitchPin) == LOW) ? "ON" : "OFF";
      statusMsg += " | Engine: ";
      statusMsg += engineRunning ? "RUNNING" : "STOPPED";
      sendSMS(statusMsg);
    }
  }
}

void sendSMS(String msg) {
  gsmSerial.print("AT+CMGS=\"" + lastSender + "\"\r");
  delay(1000);
  if (gsmSerial.find(">")) {
    gsmSerial.print(msg);
    delay(100);
    gsmSerial.write(26);
  }
}

void triggerAlert() {
  int count = 0;
  bool ledState = false;
  unsigned long lastBlink = millis(), lastTone = millis();
  
  while (count < 15) {
    unsigned long now = millis();
    
    if (now - lastBlink >= 300) {
      ledState = !ledState;
      digitalWrite(lightIndicatorPin, ledState);
      lastBlink = now;
    }
    
    if (now - lastTone < 200) tone(buzzerPin, 1500);
    else if (now - lastTone < 400) tone(buzzerPin, 1000);
    else {
      lastTone = now;
      count++;
    }
  }
  
  noTone(buzzerPin);
  digitalWrite(lightIndicatorPin, LOW);
}

void sendAlertSMS() {
  sendSMS("ALERT! Unauthorized movement detected!");
}

void sendCrashEventToFirebase(float lat, float lng, float impact, float roll) {
  Serial.println("\n[FIREBASE] Sending crash event WITH GPS...");
  Serial.println("📍 This will create a CRASH MARKER on the dashboard map!");
  
  StaticJsonDocument<256> doc;
  doc["timestamp"] = gpsToUnixTime() * 1000UL;
  doc["lat"] = lat;
  doc["lng"] = lng;
  doc["hasGPS"] = true;
  doc["impactStrength"] = impact;
  doc["roll"] = roll;
  doc["type"] = "crash"; // ✅ Mark as crash event for map filtering
  
  String payload;
  serializeJson(doc, payload);
  
  Serial.println("[FIREBASE] Crash payload: " + payload);
  
  HTTPClient http;
  http.begin(firebaseHost + crashPath);
  http.addHeader("Content-Type", "application/json");
  int code = http.POST(payload);
  
  if (code == HTTP_CODE_OK || code == HTTP_CODE_CREATED) {
    Serial.printf("[FIREBASE] ✓ Crash sent successfully! HTTP: %d\n", code);
    Serial.println("✓ Crash location will appear on dashboard map!");
  } else {
    Serial.printf("[FIREBASE] ✗ Crash send failed! HTTP: %d\n", code);
  }
  
  http.end();
}

void sendCrashEventToFirebaseNoGPS(float impact, float roll) {
  Serial.println("\n[FIREBASE] Sending crash event WITHOUT GPS...");
  Serial.println("⚠️ No GPS fix - crash will be recorded but won't show on map");
  
  StaticJsonDocument<256> doc;
  doc["timestamp"] = gpsToUnixTime() * 1000UL;
  doc["hasGPS"] = false;
  doc["impactStrength"] = impact;
  doc["roll"] = roll;
  doc["type"] = "crash"; // ✅ Mark as crash event
  
  String payload;
  serializeJson(doc, payload);
  
  Serial.println("[FIREBASE] Crash payload: " + payload);
  
  HTTPClient http;
  http.begin(firebaseHost + crashPath);
  http.addHeader("Content-Type", "application/json");
  int code = http.POST(payload);
  
  if (code == HTTP_CODE_OK || code == HTTP_CODE_CREATED) {
    Serial.printf("[FIREBASE] ✓ Crash sent successfully! HTTP: %d\n", code);
    Serial.println("⚠️ Crash recorded but won't show on map (no GPS coordinates)");
  } else {
    Serial.printf("[FIREBASE] ✗ Crash send failed! HTTP: %d\n", code);
  }
  
  http.end();
}

float getSpeedLimitFromFirebase() {
  HTTPClient http;
  http.begin(firebaseHost + "/helmet_public/" + userUID + "/settings/speedLimit.json?auth=" + firebaseAuth);
  int code = http.GET();
  
  float limit = 80.0;
  if (code == HTTP_CODE_OK) {
    String res = http.getString();
    limit = res.toFloat();
  }
  
  http.end();
  return limit;
}

void connectToWiFi() {
  WiFi.begin(ssid, password);
  Serial.print("Connecting WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println("\nConnected IP: " + String(WiFi.localIP().toString()));
}

void setupOTA() {
  ArduinoOTA.setHostname("HelmetSafetySystem");
  ArduinoOTA.setPassword("password");
  ArduinoOTA.begin();
}

void startNewTrip() {
  tripStartTime = millis();
  currentStartLat = gps.location.isValid() ? gps.location.lat() : 0.0;
  currentStartLng = gps.location.isValid() ? gps.location.lng() : 0.0;
  maxRecordedSpeed = 0;
}

void endCurrentTrip(float endLat, float endLng, float speed, float voltage, bool h, bool t, bool l, bool r, unsigned long startT, unsigned long endT) {
  StaticJsonDocument<512> doc;
  doc["startLat"] = currentStartLat;
  doc["startLng"] = currentStartLng;
  doc["endLat"] = endLat;
  doc["endLng"] = endLng;
  doc["startTime"] = startT;
  doc["endTime"] = endT;
  doc["duration"] = (endT - startT) / 60000.0;
  doc["maxSpeed"] = speed;
  doc["distance"] = sqrt(pow(endLat - currentStartLat, 2) + pow(endLng - currentStartLng, 2)) * 111.0 * 1000;
  doc["batteryVoltage"] = voltage;
  doc["headlight"] = h;
  doc["taillight"] = t;
  doc["leftSignal"] = l;
  doc["rightSignal"] = r;
  doc["status"] = "completed";
  
  String payload;
  serializeJson(doc, payload);
  
  HTTPClient http;
  http.begin(firebaseHost + tripsPath);
  http.addHeader("Content-Type", "application/json");
  http.POST(payload);
  http.end();
}

void handleHelmetState(bool currentHelmetState, float voltage, bool h, bool t, bool l, bool r) {
  if (currentHelmetState && !isHelmetOn) {
    startNewTrip();
    isHelmetOn = true;
    crashDetected = false;
    // ✅ Removed: Only helmet module should update helmetStatus to avoid conflicts
    // updateHelmetStatusInFirebase(true, "Normal", 0.0);
    Serial.println("[HELMET] Helmet PUT ON");
  }
  
  if (!currentHelmetState && isHelmetOn) {
    unsigned long endTime = millis();
    endCurrentTrip(
      gps.location.isValid() ? gps.location.lat() : currentStartLat,
      gps.location.isValid() ? gps.location.lng() : currentStartLng,
      gps.speed.kmph(), voltage, h, t, l, r,
      tripStartTime, endTime
    );
    
    isHelmetOn = false;
    lastHelmetOffMillis = millis();
    crashDetected = false;
    // ✅ Removed: Only helmet module should update helmetStatus to avoid conflicts
    // updateHelmetStatusInFirebase(false, "Normal", 0.0);
    Serial.println("[HELMET] Helmet REMOVED");
    
    // Stop engine if helmet is removed while running
    if (engineRunning) {
      Serial.println("[SAFETY] Helmet removed - stopping engine!");
      stopEngine();
    }
  }
}
