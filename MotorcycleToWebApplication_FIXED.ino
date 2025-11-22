/*
VigiLERT ESP32 Motorcycle/Helmet System - FIXED VERSION
- MPU6050 (I2C)
- GPS (UART1)
- GSM Module (UART2)
- Engine control via Relay
- Vibration/Buzzer/Light Alerts
- Helmet detection switch
- Firebase Realtime Database integration
- Crash detection
- FIXED: Now writes to correct Firebase paths that Dashboard reads from
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
const String firebaseAuth = ""; // leave blank if public write enabled

// ======= PIN ASSIGNMENTS =======
const int batteryPin = 34;        // ADC1
const int helmetSwitchPin = 39;   // Digital input
const int headlightPin = 32;
const int taillightPin = 33;
const int leftSignalPin = 25;
const int rightSignalPin = 26;
const int relayPin = 27;          // Engine control
const int vibrationPin = 14;      // Vibration sensor
const int buzzerPin = 12;         // Buzzer
const int lightIndicatorPin = 13; // Visual alert LED

// GSM module pins (UART2)
#define GSM_RX 4
#define GSM_TX 5
HardwareSerial gsmSerial(2); // UART2

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

// Crash detection
bool crashDetected = false;
const float ACCEL_THRESHOLD = 15.0; // m/s²
bool isHelmetOn = false;
unsigned long lastHelmetOffMillis = 0;

// Speed limit
float currentSpeedLimit = 80.0;
unsigned long lastSpeedCheck = 0;

// Firebase update intervals
unsigned long lastLiveUpdate = 0;
unsigned long lastStatusUpdate = 0;
unsigned long lastElectricalUpdate = 0;
const unsigned long LIVE_UPDATE_INTERVAL = 1000;      // 1 second
const unsigned long STATUS_UPDATE_INTERVAL = 2000;    // 2 seconds
const unsigned long ELECTRICAL_UPDATE_INTERVAL = 5000; // 5 seconds

// ======= SETUP =======
void setup() {
  Serial.begin(115200);
  delay(100);
  Serial.println("\n[SETUP] Starting VigiLERT System...");

  // Try different I2C pins if default doesn't work
  // Default: Wire.begin(21, 22); // SDA=21, SCL=22
  // Alternative 1: Wire.begin(23, 19); // SDA=23, SCL=19
  // Alternative 2: Wire.begin(25, 26); // SDA=25, SCL=26
  Wire.begin(21, 22); // I2C for MPU6050 (SDA=21, SCL=22)
  
  Serial.println("[SETUP] I2C initialized on SDA=GPIO21, SCL=GPIO22");
  Serial.println("[SETUP] If MPU6050 doesn't work, try different pins in code");
  
  // Scan I2C bus for devices
  Serial.println("[SETUP] Scanning I2C bus...");
  scanI2C();

  // MPU6050 initialization with retry
  int retryCount = 0;
  while (!mpu.begin()) {
    Serial.printf("[ERROR] Failed to find MPU6050 chip (attempt %d). Checking again in 2 seconds...\n", ++retryCount);
    if (retryCount >= 5) {
      Serial.println("\n⚠️⚠️⚠️ CRITICAL ERROR ⚠️⚠️⚠️");
      Serial.println("Cannot initialize MPU6050 after 5 attempts!");
      Serial.println("Please check:");
      Serial.println("  - I2C wiring (SDA=GPIO21, SCL=GPIO22)");
      Serial.println("  - MPU6050 power (VCC=3.3V, GND=GND)");
      Serial.println("  - I2C address (0x68 or 0x69)");
      Serial.println("System will continue but crash detection will NOT work!");
      Serial.println("⚠️⚠️⚠️⚠️⚠️⚠️⚠️⚠️⚠️⚠️⚠️⚠️⚠️⚠️⚠️\n");
      break;
    }
    delay(2000);
  }
  
  if (retryCount < 5) {
    Serial.println("[SUCCESS] MPU6050 initialized.");
  }

  mpu.setAccelerometerRange(MPU6050_RANGE_16_G);
  mpu.setGyroRange(MPU6050_RANGE_2000_DEG);
  mpu.setFilterBandwidth(MPU6050_BAND_21_HZ);
  
  // Test MPU6050 readings multiple times
  Serial.println("[SETUP] Testing MPU6050 readings...");
  delay(100);
  
  bool mpuWorking = false;
  for (int i = 0; i < 5; i++) {
    mpu.getEvent(&accel, &gyro, &temp);
    Serial.printf("[MPU6050 Test %d] X:%.2f Y:%.2f Z:%.2f | Temp:%.1f°C\n", 
      i+1, accel.acceleration.x, accel.acceleration.y, accel.acceleration.z, temp.temperature);
    
    // Check if we're getting real data (gravity should be ~9.8 m/s²)
    float totalAccel = sqrt(accel.acceleration.x * accel.acceleration.x +
                           accel.acceleration.y * accel.acceleration.y +
                           accel.acceleration.z * accel.acceleration.z);
    
    if (totalAccel > 8.0 && totalAccel < 12.0) {
      mpuWorking = true;
      Serial.printf("[SUCCESS] MPU6050 is working! Total acceleration: %.2f m/s²\n", totalAccel);
      break;
    }
    delay(200);
  }
  
  if (!mpuWorking) {
    Serial.println("\n⚠️⚠️⚠️ MPU6050 ERROR ⚠️⚠️⚠️");
    Serial.println("MPU6050 readings are invalid!");
    Serial.println("Possible issues:");
    Serial.println("  1. I2C wiring: SDA=GPIO21, SCL=GPIO22");
    Serial.println("  2. MPU6050 not powered (VCC=3.3V, GND=GND)");
    Serial.println("  3. Wrong I2C address (should be 0x68 or 0x69)");
    Serial.println("  4. Faulty MPU6050 module");
    Serial.println("Crash detection will NOT work until this is fixed!");
    Serial.println("⚠️⚠️⚠️⚠️⚠️⚠️⚠️⚠️⚠️⚠️⚠️⚠️⚠️⚠️⚠️\n");
  }

  // GPS UART
  gpsSerial.begin(9600, SERIAL_8N1, 16, 17);
  Serial.println("[SETUP] GPS Serial started on RX=16, TX=17");
  Serial.println("[SETUP] Waiting for GPS data... (this may take 30-60 seconds outdoors)");

  // GSM UART
  gsmSerial.begin(9600, SERIAL_8N1, GSM_RX, GSM_TX);
  Serial.println("[SETUP] GSM Serial started.");

  // Pin modes
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
  
  // Initialize Firebase with default values
  initializeFirebaseData();
  
  startNewTrip();
  currentSpeedLimit = getSpeedLimitFromFirebase();

  Serial.println("[SUCCESS] Setup complete. Entering main loop...");
}

// ======= MAIN LOOP =======
void loop() {
  ArduinoOTA.handle();

  // GPS data
  while (gpsSerial.available()) gps.encode(gpsSerial.read());

  // GSM handling
  handleGSM();

  // Sensor readings
  bool helmetSwitchState = (digitalRead(helmetSwitchPin) == LOW);
  bool headlightOn = digitalRead(headlightPin);
  bool taillightOn = digitalRead(taillightPin);
  bool leftSignalOn = digitalRead(leftSignalPin);
  bool rightSignalOn = digitalRead(rightSignalPin);
  float batteryVoltage = analogRead(batteryPin) * (3.3f / 4095.0f) * 4.3;

  // MPU6050 crash detection
  mpu.getEvent(&accel, &gyro, &temp);
  
  // Calculate acceleration and angles
  float totalAcceleration = sqrt(
    accel.acceleration.x * accel.acceleration.x +
    accel.acceleration.y * accel.acceleration.y +
    accel.acceleration.z * accel.acceleration.z
  );
  float roll = atan2(accel.acceleration.y, accel.acceleration.z) * 180.0 / PI;
  float pitch = atan2(-accel.acceleration.x, sqrt(accel.acceleration.y * accel.acceleration.y + 
                                                   accel.acceleration.z * accel.acceleration.z)) * 180.0 / PI;
  
  // Debug: Print MPU6050 data every 5 seconds (less verbose)
  static unsigned long lastMpuDebug = 0;
  if (millis() - lastMpuDebug > 5000) {
    Serial.printf("[MPU6050] Accel:%.2f | Pitch:%.1f° (Lean) | Roll:%.1f° | Temp:%.1f°C\n", 
      totalAcceleration, pitch, roll, temp.temperature);
    lastMpuDebug = millis();
  }

  // Enhanced Serial output for monitoring
  Serial.printf("Accel Total: %.2f | Roll: %.2f | Battery: %.2fV | Helmet: %s | ", 
    totalAcceleration, roll, batteryVoltage, helmetSwitchState ? "ON" : "OFF");
  
  // GPS Debug Info
  if (gps.location.isValid()) {
    Serial.printf("GPS: Lat=%.6f Lng=%.6f Speed=%.2f km/h Sats=%d\n",
      gps.location.lat(), gps.location.lng(), gps.speed.kmph(), gps.satellites.value());
  } else {
    Serial.printf("GPS: NO FIX (Chars=%d Sats=%d)\n", 
      gps.charsProcessed(), gps.satellites.value());
  }

  // Crash detection - Check for LEFT/RIGHT lean (pitch) or high impact
  // Pitch: Positive = lean right, Negative = lean left
  bool leftLean = pitch < -45;   // Leaning left more than 45°
  bool rightLean = pitch > 45;   // Leaning right more than 45°
  bool highImpact = totalAcceleration >= ACCEL_THRESHOLD;
  
  if ((leftLean || rightLean || highImpact) && !crashDetected) {
    Serial.println("\n⚠️⚠️⚠️ CRASH DETECTED! ⚠️⚠️⚠️");
    Serial.printf("Acceleration: %.2f m/s² | Pitch: %.2f° (Lean) | Roll: %.2f°\n", 
                  totalAcceleration, pitch, roll);
    
    if (leftLean) Serial.println("→ LEANING LEFT!");
    if (rightLean) Serial.println("→ LEANING RIGHT!");
    if (highImpact) Serial.println("→ HIGH IMPACT!");
    
    if (gps.location.isValid()) {
      Serial.printf("Location: %.6f, %.6f\n", gps.location.lat(), gps.location.lng());
      sendCrashEventToFirebase(gps.location.lat(), gps.location.lng(), totalAcceleration, pitch);
    } else {
      Serial.println("Location: GPS NOT AVAILABLE");
      sendCrashEventToFirebaseNoGPS(totalAcceleration, pitch);
    }
    crashDetected = true;
    triggerAlert();
  }

  // Clear crash detection when bike is upright again
  if (totalAcceleration < ACCEL_THRESHOLD && pitch > -30 && pitch < 30 && crashDetected) {
    crashDetected = false;
    Serial.println("[INFO] Crash cleared - bike upright");
  }

  // Engine security
  if (!engineRunning && digitalRead(vibrationPin) == HIGH) {
    Serial.println("Unauthorized movement detected!");
    triggerAlert();
    sendAlertSMS();
  }

  // Helmet state
  handleHelmetState(helmetSwitchState, batteryVoltage, headlightOn, taillightOn, leftSignalOn, rightSignalOn);

  unsigned long now = millis();

  // Firebase live updates (location, speed, battery)
  if (now - lastLiveUpdate >= LIVE_UPDATE_INTERVAL) {
    if (gps.location.isValid()) {
      float speedKmph = gps.speed.kmph();
      sendLiveToFirebase(gps.location.lat(), gps.location.lng(), speedKmph, batteryVoltage,
                        headlightOn, taillightOn, leftSignalOn, rightSignalOn);
      if (speedKmph > maxRecordedSpeed) maxRecordedSpeed = speedKmph;
    } else {
      sendLiveToFirebaseNoGPS(batteryVoltage, headlightOn, taillightOn, leftSignalOn, rightSignalOn);
    }
    lastLiveUpdate = now;
  }

  // Firebase status updates (helmet connection, pairing)
  if (now - lastStatusUpdate >= STATUS_UPDATE_INTERVAL) {
    updateHelmetStatus(helmetSwitchState);
    updateHelmetDeviceStatus(helmetSwitchState); // Send helmet heartbeat
    updateDeviceStatus();                         // Send motorcycle heartbeat
    updateDeviceHealth(batteryVoltage, gps.location.isValid());
    lastStatusUpdate = now;
  }

  // Firebase electrical diagnostics
  if (now - lastElectricalUpdate >= ELECTRICAL_UPDATE_INTERVAL) {
    updateElectricalDiagnostics(headlightOn, taillightOn, leftSignalOn, rightSignalOn);
    lastElectricalUpdate = now;
  }

  // Periodic speed limit update
  if (now - lastSpeedCheck > 60000) {
    currentSpeedLimit = getSpeedLimitFromFirebase();
    lastSpeedCheck = now;
  }

  // GPS Diagnostics every 10 seconds
  static unsigned long lastGpsDiag = 0;
  if (millis() - lastGpsDiag > 10000) {
    printGPSDiagnostics();
    lastGpsDiag = millis();
  }

  delay(50); // Faster response for crash detection (20 Hz loop)
}

// ======= I2C SCANNER =======
void scanI2C() {
  byte error, address;
  int nDevices = 0;

  Serial.println("Scanning I2C bus (addresses 0x01 to 0x7F)...");

  for (address = 1; address < 127; address++) {
    Wire.beginTransmission(address);
    error = Wire.endTransmission();

    if (error == 0) {
      Serial.printf("✓ I2C device found at address 0x%02X\n", address);
      if (address == 0x68 || address == 0x69) {
        Serial.println("  → This is likely the MPU6050!");
      }
      nDevices++;
    } else if (error == 4) {
      Serial.printf("✗ Unknown error at address 0x%02X\n", address);
    }
  }

  if (nDevices == 0) {
    Serial.println("✗ No I2C devices found!");
    Serial.println("  Check wiring: SDA=GPIO21, SCL=GPIO22");
  } else {
    Serial.printf("✓ Found %d I2C device(s)\n", nDevices);
  }
  Serial.println();
}

// ======= GPS DIAGNOSTICS =======
void printGPSDiagnostics() {
  Serial.println("\n========== GPS DIAGNOSTICS ==========");
  Serial.printf("Characters processed: %d\n", gps.charsProcessed());
  Serial.printf("Sentences with fix: %d\n", gps.sentencesWithFix());
  Serial.printf("Failed checksum: %d\n", gps.failedChecksum());
  Serial.printf("Passed checksum: %d\n", gps.passedChecksum());
  
  if (gps.location.isValid()) {
    Serial.printf("✓ Location: %.6f, %.6f\n", gps.location.lat(), gps.location.lng());
    Serial.printf("✓ Altitude: %.2f m\n", gps.altitude.meters());
    Serial.printf("✓ Speed: %.2f km/h\n", gps.speed.kmph());
    Serial.printf("✓ Satellites: %d\n", gps.satellites.value());
    Serial.printf("✓ HDOP: %d\n", gps.hdop.value());
  } else {
    Serial.println("✗ Location: INVALID");
    if (gps.charsProcessed() < 10) {
      Serial.println("  → No data from GPS module. Check wiring!");
      Serial.println("  → RX should be on GPIO 16");
      Serial.println("  → TX should be on GPIO 17");
      Serial.println("  → GPS TX connects to ESP32 RX (GPIO 16)");
      Serial.println("  → GPS RX connects to ESP32 TX (GPIO 17)");
    } else {
      Serial.println("  → GPS receiving data but no satellite fix yet");
      Serial.println("  → Move to open area with clear sky view");
      Serial.println("  → Wait 30-60 seconds for satellite lock");
    }
  }
  
  if (gps.date.isValid()) {
    Serial.printf("✓ Date: %02d/%02d/%04d\n", gps.date.day(), gps.date.month(), gps.date.year());
  }
  if (gps.time.isValid()) {
    Serial.printf("✓ Time: %02d:%02d:%02d\n", gps.time.hour(), gps.time.minute(), gps.time.second());
  }
  Serial.println("=====================================\n");
}

// ======= FIREBASE FUNCTIONS (FIXED PATHS) =======

void initializeFirebaseData() {
  Serial.println("[FIREBASE] Initializing default data...");
  
  // Initialize helmet status
  updateHelmetStatus(false);
  
  // Initialize device status (motorcycle is ON, helmet is OFF initially)
  updateDeviceStatus();              // Motorcycle = On
  updateHelmetDeviceStatus(false);   // Helmet = Off
  
  // Initialize device health
  updateDeviceHealth(0, false);
  
  // Initialize electrical diagnostics
  updateElectricalDiagnostics(false, false, false, false);
  
  Serial.println("[FIREBASE] Initialization complete.");
}

void updateHelmetStatus(bool connected) {
  // Path: /helmet_public/{userId}/helmetStatus/status
  StaticJsonDocument<128> doc;
  doc["helmetConnected"] = connected;
  doc["alertnessStatus"] = "Normal";
  doc["alcoholLevel"] = 0.0;
  
  String payload;
  serializeJson(doc, payload);
  
  HTTPClient http;
  String path = "/helmet_public/" + userUID + "/helmetStatus/status.json?auth=" + firebaseAuth;
  http.begin(firebaseHost + path);
  http.addHeader("Content-Type", "application/json");
  int code = http.PUT(payload);
  if (code > 0) {
    Serial.printf("[FIREBASE] Helmet status updated: %d\n", code);
  }
  http.end();
}

void updateDeviceStatus() {
  // Path: /helmet_public/{userId}/devices/motorcycle
  // Send heartbeat with Unix timestamp (milliseconds)
  StaticJsonDocument<128> doc;
  doc["status"] = "On";
  
  // Use GPS time if available, otherwise use millis as fallback
  unsigned long timestamp;
  if (gps.time.isValid() && gps.date.isValid()) {
    timestamp = gpsToUnixTime() * 1000UL; // Convert to milliseconds
  } else {
    // Fallback: Use epoch + millis (approximate)
    timestamp = 1700000000000UL + millis(); // Rough Unix timestamp
  }
  
  doc["lastHeartbeat"] = timestamp;
  doc["timestamp"] = timestamp;
  
  String payload;
  serializeJson(doc, payload);
  
  HTTPClient http;
  String path = "/helmet_public/" + userUID + "/devices/motorcycle.json?auth=" + firebaseAuth;
  http.begin(firebaseHost + path);
  http.addHeader("Content-Type", "application/json");
  int code = http.PUT(payload);
  Serial.printf("[FIREBASE] Motorcycle heartbeat sent: %lu (HTTP: %d)\n", timestamp, code);
  http.end();
}

void updateDeviceHealth(float voltage, bool gpsValid) {
  // Path: /helmet_public/{userId}/deviceHealth
  StaticJsonDocument<128> doc;
  doc["battery"] = (int)((voltage / 4.2) * 100); // Convert voltage to percentage
  doc["gsm"] = true; // Assume GSM is connected if we can send data
  doc["gps"] = gpsValid;
  
  String payload;
  serializeJson(doc, payload);
  
  HTTPClient http;
  String path = "/helmet_public/" + userUID + "/deviceHealth.json?auth=" + firebaseAuth;
  http.begin(firebaseHost + path);
  http.addHeader("Content-Type", "application/json");
  http.PUT(payload);
  http.end();
}

void updateElectricalDiagnostics(bool headlight, bool taillight, bool leftSignal, bool rightSignal) {
  // Path: /helmet_public/{userId}/electrical
  StaticJsonDocument<128> doc;
  doc["headlight"] = headlight;
  doc["taillight"] = taillight;
  doc["brakeLight"] = taillight; // Assuming brake light same as taillight
  doc["signalLights"] = (leftSignal || rightSignal);
  
  String payload;
  serializeJson(doc, payload);
  
  HTTPClient http;
  String path = "/helmet_public/" + userUID + "/electrical.json?auth=" + firebaseAuth;
  http.begin(firebaseHost + path);
  http.addHeader("Content-Type", "application/json");
  http.PUT(payload);
  http.end();
}

void sendLiveToFirebase(float lat, float lng, float speed, float v, bool h, bool t, bool l, bool r) {
  // Path: /helmet_public/{userId}/live
  StaticJsonDocument<512> doc;
  doc["locationLat"] = lat;
  doc["locationLng"] = lng;
  doc["speed"] = speed;
  doc["batteryVoltage"] = v;
  doc["headlight"] = h;
  doc["taillight"] = t;
  doc["leftSignal"] = l;
  doc["rightSignal"] = r;
  doc["timestamp"] = millis();
  doc["source"] = "gps_module";
  
  String payload;
  serializeJson(doc, payload);
  
  HTTPClient http;
  String path = "/helmet_public/" + userUID + "/live.json?auth=" + firebaseAuth;
  http.begin(firebaseHost + path);
  http.addHeader("Content-Type", "application/json");
  http.PUT(payload);
  http.end();
}

void sendLiveToFirebaseNoGPS(float v, bool h, bool t, bool l, bool r) {
  // Path: /helmet_public/{userId}/live
  StaticJsonDocument<256> doc;
  doc["batteryVoltage"] = v;
  doc["headlight"] = h;
  doc["taillight"] = t;
  doc["leftSignal"] = l;
  doc["rightSignal"] = r;
  doc["timestamp"] = millis();
  doc["source"] = "gps_module";
  
  String payload;
  serializeJson(doc, payload);
  
  HTTPClient http;
  String path = "/helmet_public/" + userUID + "/live.json?auth=" + firebaseAuth;
  http.begin(firebaseHost + path);
  http.addHeader("Content-Type", "application/json");
  http.PATCH(payload); // Use PATCH to update only these fields
  http.end();
}

void sendCrashEventToFirebase(float lat, float lng, float impact, float leanAngle) {
  // Path: /helmet_public/{userId}/crashes
  // Use GPS time if available
  unsigned long timestamp;
  if (gps.time.isValid() && gps.date.isValid()) {
    timestamp = gpsToUnixTime() * 1000UL;
  } else {
    timestamp = 1700000000000UL + millis();
  }
  
  StaticJsonDocument<256> doc;
  doc["timestamp"] = timestamp;
  doc["lat"] = lat;
  doc["lng"] = lng;
  doc["hasGPS"] = true;
  doc["impactStrength"] = impact;
  doc["roll"] = leanAngle; // Actually pitch (lean angle), but keeping field name for compatibility
  doc["leanAngle"] = leanAngle; // New field for clarity
  
  String payload;
  serializeJson(doc, payload);
  
  HTTPClient http;
  String path = "/helmet_public/" + userUID + "/crashes.json?auth=" + firebaseAuth;
  http.begin(firebaseHost + path);
  http.addHeader("Content-Type", "application/json");
  int code = http.POST(payload);
  Serial.printf("[FIREBASE] Crash event sent: %d\n", code);
  http.end();
}

void sendCrashEventToFirebaseNoGPS(float impact, float leanAngle) {
  // Path: /helmet_public/{userId}/crashes
  unsigned long timestamp;
  if (gps.time.isValid() && gps.date.isValid()) {
    timestamp = gpsToUnixTime() * 1000UL;
  } else {
    timestamp = 1700000000000UL + millis();
  }
  
  StaticJsonDocument<256> doc;
  doc["timestamp"] = timestamp;
  doc["hasGPS"] = false;
  doc["impactStrength"] = impact;
  doc["roll"] = leanAngle; // Actually pitch (lean angle), but keeping field name for compatibility
  doc["leanAngle"] = leanAngle; // New field for clarity
  
  String payload;
  serializeJson(doc, payload);
  
  HTTPClient http;
  String path = "/helmet_public/" + userUID + "/crashes.json?auth=" + firebaseAuth;
  http.begin(firebaseHost + path);
  http.addHeader("Content-Type", "application/json");
  int code = http.POST(payload);
  Serial.printf("[FIREBASE] Crash event sent (no GPS): %d\n", code);
  http.end();
}

float getSpeedLimitFromFirebase() {
  HTTPClient http;
  String path = "/helmet_public/" + userUID + "/settings/speedLimit.json?auth=" + firebaseAuth;
  http.begin(firebaseHost + path);
  int code = http.GET();
  float limit = 80.0;
  if (code == HTTP_CODE_OK) {
    String response = http.getString();
    limit = response.toFloat();
    if (limit == 0) limit = 80.0; // Default if null or 0
  }
  http.end();
  return limit;
}

// ======= TRIP MANAGEMENT =======

void startNewTrip() {
  tripStartTime = millis();
  currentStartLat = gps.location.isValid() ? gps.location.lat() : 0.0;
  currentStartLng = gps.location.isValid() ? gps.location.lng() : 0.0;
  maxRecordedSpeed = 0;
}

void endCurrentTrip(float endLat, float endLng, float speed, float voltage, bool h, bool t, bool l, bool r, unsigned long startT, unsigned long endT) {
  // Path: /helmet_public/{userId}/trips
  StaticJsonDocument<512> doc;
  doc["startLat"] = currentStartLat;
  doc["startLng"] = currentStartLng;
  doc["endLat"] = endLat;
  doc["endLng"] = endLng;
  doc["startTime"] = startT;
  doc["endTime"] = endT;
  doc["duration"] = (endT - startT) / 60000;
  doc["maxSpeed"] = maxRecordedSpeed;
  doc["distance"] = sqrt((endLat - currentStartLat) * (endLat - currentStartLat) + 
                        (endLng - currentStartLng) * (endLng - currentStartLng));
  doc["batteryVoltage"] = voltage;
  doc["headlight"] = h;
  doc["taillight"] = t;
  doc["leftSignal"] = l;
  doc["rightSignal"] = r;
  doc["status"] = "completed";
  
  String payload;
  serializeJson(doc, payload);
  
  HTTPClient http;
  String path = "/helmet_public/" + userUID + "/trips.json?auth=" + firebaseAuth;
  http.begin(firebaseHost + path);
  http.addHeader("Content-Type", "application/json");
  http.POST(payload);
  http.end();
}

void updateHelmetDeviceStatus(bool isOn) {
  // Path: /helmet_public/{userId}/devices/helmet
  // Send heartbeat with Unix timestamp (milliseconds)
  StaticJsonDocument<128> doc;
  doc["status"] = isOn ? "On" : "Off";
  
  // Use GPS time if available, otherwise use millis as fallback
  unsigned long timestamp;
  if (gps.time.isValid() && gps.date.isValid()) {
    timestamp = gpsToUnixTime() * 1000UL; // Convert to milliseconds
  } else {
    // Fallback: Use epoch + millis (approximate)
    timestamp = 1700000000000UL + millis(); // Rough Unix timestamp
  }
  
  doc["lastHeartbeat"] = timestamp;
  doc["timestamp"] = timestamp;
  
  String payload;
  serializeJson(doc, payload);
  
  HTTPClient http;
  String path = "/helmet_public/" + userUID + "/devices/helmet.json?auth=" + firebaseAuth;
  http.begin(firebaseHost + path);
  http.addHeader("Content-Type", "application/json");
  int code = http.PUT(payload);
  Serial.printf("[FIREBASE] Helmet heartbeat: %s at %lu (HTTP: %d)\n", isOn ? "On" : "Off", timestamp, code);
  http.end();
}

void handleHelmetState(bool currentHelmetState, float voltage, bool h, bool t, bool l, bool r) {
  if (currentHelmetState && !isHelmetOn) {
    Serial.println("[HELMET] Helmet PUT ON - Starting trip");
    startNewTrip();
    isHelmetOn = true;
    crashDetected = false;
    updateHelmetStatus(true);
    updateHelmetDeviceStatus(true);  // Update /devices/helmet/status
    updateDeviceStatus();             // Update /devices/motorcycle/status
  }
  
  if (!currentHelmetState && isHelmetOn) {
    Serial.println("[HELMET] Helmet REMOVED - Ending trip");
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
    updateHelmetStatus(false);
    updateHelmetDeviceStatus(false);  // Update /devices/helmet/status
    updateDeviceStatus();              // Update /devices/motorcycle/status
  }
}

// ======= GSM FUNCTIONS =======

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
  return 0;
}

void handleGSM() {
  while (gsmSerial.available()) {
    String response = gsmSerial.readString();
    Serial.println("GSM: " + response);
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
        String locMsg = "GPS Lat: " + String(gps.location.lat(), 6) + 
                       " Lng: " + String(gps.location.lng(), 6);
        sendSMS(locMsg);
      } else sendSMS("GPS location not available.");
    }
  }
}

void sendSMS(String msg) {
  gsmSerial.print("AT+CMGS=\"" + lastSender + "\"\r");
  delay(1000);
  if (gsmSerial.find(">")) {
    gsmSerial.print(msg);
    delay(100);
    gsmSerial.write(26); // CTRL+Z
  }
}

void startEngine() {
  digitalWrite(relayPin, HIGH);
  digitalWrite(lightIndicatorPin, LOW);
  engineRunning = true;
  updateDeviceStatus();
  sendSMS("Engine Started.");
}

void stopEngine() {
  digitalWrite(relayPin, LOW);
  digitalWrite(lightIndicatorPin, HIGH);
  engineRunning = false;
  updateDeviceStatus();
  sendSMS("Engine Stopped.");
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

// ======= HELPERS =======

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
