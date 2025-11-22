/*
VigiLERT ESP32 Motorcycle/Helmet System with Safety Interlock
- Both helmet and motorcycle modules must be ON to start engine
- MPU6050 (I2C)
- GPS (UART1)
- GSM Module (UART2)
- Engine control via Relay with safety check
- Vibration/Buzzer/Light Alerts
- Helmet detection switch
- Firebase Realtime Database integration
- Crash detection
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

// ✅ NEW: Safety interlock - Check if helmet module is active
bool isHelmetModuleActive = false;
unsigned long lastHelmetModuleCheck = 0;
const unsigned long HELMET_CHECK_INTERVAL = 5000; // Check every 5 seconds

// Speed limit
float currentSpeedLimit = 80.0;
unsigned long lastSpeedCheck = 0;

// Firebase paths
const String tripsPath = "/helmet_public/" + userUID + "/trips.json?auth=" + firebaseAuth;
const String livePath  = "/helmet_public/" + userUID + "/live.json?auth=" + firebaseAuth;
const String crashPath = "/helmet_public/" + userUID + "/crashes.json?auth=" + firebaseAuth;
const String helmetStatusPath = "/helmet_public/" + userUID + "/helmetStatus/status.json?auth=" + firebaseAuth;
const String devicesPath = "/helmet_public/" + userUID + "/devices.json?auth=" + firebaseAuth;

// ======= SETUP =======
void setup() {
  Serial.begin(115200);
  delay(100);
  Serial.println("\n[SETUP] Starting VigiLERT System with Safety Interlock...");

  Wire.begin(21, 22);

  while (!mpu.begin()) {
    Serial.println("[ERROR] Failed to find MPU6050 chip. Retrying in 2s...");
    delay(2000);
  }
  Serial.println("[SUCCESS] MPU6050 initialized.");

  mpu.setAccelerometerRange(MPU6050_RANGE_16_G);
  mpu.setGyroRange(MPU6050_RANGE_2000_DEG);
  mpu.setFilterBandwidth(MPU6050_BAND_21_HZ);

  gpsSerial.begin(9600, SERIAL_8N1, 16, 17);
  Serial.println("[SETUP] GPS Serial started.");

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
  
  // ✅ Update motorcycle device status on startup
  updateMotorcycleDeviceStatus(true);
  
  startNewTrip();
  currentSpeedLimit = getSpeedLimitFromFirebase();

  Serial.println("[SUCCESS] Setup complete. Safety interlock ACTIVE.");
  Serial.println("[SAFETY] Engine can only start when BOTH helmet and motorcycle modules are ON.");
}

// ======= MAIN LOOP =======
void loop() {
  ArduinoOTA.handle();

  while (gpsSerial.available()) gps.encode(gpsSerial.read());
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
  float totalAcceleration = sqrt(
    accel.acceleration.x * accel.acceleration.x +
    accel.acceleration.y * accel.acceleration.y +
    accel.acceleration.z * accel.acceleration.z
  );
  float roll = atan2(accel.acceleration.y, accel.acceleration.z) * 180.0 / PI;

  // ✅ Periodic check: Is helmet module active?
  if (millis() - lastHelmetModuleCheck > HELMET_CHECK_INTERVAL) {
    checkHelmetModuleStatus();
    lastHelmetModuleCheck = millis();
  }

  // Serial monitoring
  static unsigned long lastPrint = 0;
  if (millis() - lastPrint > 1000) {
    Serial.printf("\n[SENSOR] Accel: %.2f | Roll: %.1f° | Battery: %.2fV | Helmet: %s",
                  totalAcceleration, roll, batteryVoltage, helmetSwitchState ? "ON" : "OFF");
    Serial.printf(" | GPS: %s | Speed: %.1f km/h",
                  gps.location.isValid() ? "OK" : "N/A",
                  gps.location.isValid() ? gps.speed.kmph() : 0.0);
    Serial.printf(" | Helmet Module: %s", isHelmetModuleActive ? "ACTIVE" : "INACTIVE");
    lastPrint = millis();
  }

  // Crash detection
  if ((totalAcceleration >= ACCEL_THRESHOLD || roll < -47 || roll > 40) && !crashDetected) {
    Serial.println("\n⚠️ CRASH DETECTED!");
    if (gps.location.isValid()) {
      sendCrashEventToFirebase(gps.location.lat(), gps.location.lng(), totalAcceleration, roll);
    } else {
      sendCrashEventToFirebaseNoGPS(totalAcceleration, roll);
    }
    crashDetected = true;
    triggerAlert();
    updateHelmetStatusInFirebase(isHelmetOn, "Crash Alert", 0.0);
  }

  if (totalAcceleration < ACCEL_THRESHOLD && roll > -10 && roll < 10 && crashDetected) {
    crashDetected = false;
    Serial.println("[INFO] Crash state cleared.");
    updateHelmetStatusInFirebase(isHelmetOn, "Normal", 0.0);
  }

  // Unauthorized movement
  if (!engineRunning && digitalRead(vibrationPin) == HIGH) {
    Serial.println("[SECURITY] Unauthorized movement!");
    triggerAlert();
    sendAlertSMS();
  }

  // Helmet state & trip management
  handleHelmetState(helmetSwitchState, batteryVoltage, headlightOn, taillightOn, leftSignalOn, rightSignalOn);

  // Firebase live updates
  if (gps.location.isValid()) {
    float speedKmph = gps.speed.kmph();
    sendLiveToFirebase(gps.location.lat(), gps.location.lng(), speedKmph, batteryVoltage,
                      headlightOn, taillightOn, leftSignalOn, rightSignalOn);
    if (speedKmph > maxRecordedSpeed) maxRecordedSpeed = speedKmph;
  } else {
    sendLiveToFirebaseNoGPS(batteryVoltage, headlightOn, taillightOn, leftSignalOn, rightSignalOn);
  }

  // Periodic speed limit update
  if (millis() - lastSpeedCheck > 60000) {
    currentSpeedLimit = getSpeedLimitFromFirebase();
    lastSpeedCheck = millis();
  }

  delay(200);
}

// ======= SAFETY FUNCTIONS =======

// ✅ Check if helmet module is sending data to Firebase
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
      
      // Check if helmet module sent heartbeat recently (within last 15 seconds)
      unsigned long now;
      if (gps.time.isValid() && gps.date.isValid()) {
        now = gpsToUnixTime() * 1000UL;
      } else {
        now = 1700000000000UL + millis();
      }
      
      unsigned long timeSinceHeartbeat = now - lastHeartbeat;
      
      if (status == "On" && timeSinceHeartbeat < 15000) {
        isHelmetModuleActive = true;
        Serial.println("[SAFETY] Helmet module is ACTIVE");
      } else {
        isHelmetModuleActive = false;
        Serial.printf("[SAFETY] Helmet module is INACTIVE (last beat: %lu ms ago)\n", timeSinceHeartbeat);
      }
    } else {
      isHelmetModuleActive = false;
      Serial.println("[SAFETY] Helmet module data parse error");
    }
  } else {
    isHelmetModuleActive = false;
    Serial.printf("[SAFETY] Failed to check helmet module status: HTTP %d\n", code);
  }
  http.end();
}

// ✅ Update motorcycle device status in Firebase
void updateMotorcycleDeviceStatus(bool isOn) {
  unsigned long timestamp;
  if (gps.time.isValid() && gps.date.isValid()) {
    timestamp = gpsToUnixTime() * 1000UL;
  } else {
    timestamp = 1700000000000UL + millis();
  }
  
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
  Serial.printf("[FIREBASE] Motorcycle status: %s (HTTP: %d)\n", isOn ? "On" : "Off", code);
  http.end();
}

// ✅ Check if both devices are ready before starting engine
bool canStartEngine() {
  // Check 1: Is helmet switch ON?
  bool helmetSwitchOn = (digitalRead(helmetSwitchPin) == LOW);
  
  // Check 2: Is helmet module active (sending data)?
  bool helmetModuleOn = isHelmetModuleActive;
  
  // Check 3: Is motorcycle module ON (this device)?
  bool motorcycleOn = true; // This device is always ON if code is running
  
  Serial.println("\n[SAFETY CHECK] Engine Start Request:");
  Serial.printf("  ✓ Motorcycle Module: %s\n", motorcycleOn ? "ON" : "OFF");
  Serial.printf("  %s Helmet Module: %s\n", helmetModuleOn ? "✓" : "✗", helmetModuleOn ? "ON" : "OFF");
  Serial.printf("  %s Helmet Switch: %s\n", helmetSwitchOn ? "✓" : "✗", helmetSwitchOn ? "ON" : "OFF");
  
  if (motorcycleOn && helmetModuleOn && helmetSwitchOn) {
    Serial.println("  → ALL CHECKS PASSED! Engine can start.");
    return true;
  } else {
    Serial.println("  → SAFETY INTERLOCK ACTIVE! Engine cannot start.");
    if (!helmetModuleOn) {
      Serial.println("  → Helmet module is not active. Turn on helmet device!");
    }
    if (!helmetSwitchOn) {
      Serial.println("  → Helmet switch is OFF. Put on helmet!");
    }
    return false;
  }
}

// ======= ENGINE CONTROL (WITH SAFETY CHECK) =======

void startEngine() {
  // ✅ Safety check before starting engine
  if (!canStartEngine()) {
    String errorMsg = "Engine start DENIED! ";
    if (!isHelmetModuleActive) {
      errorMsg += "Helmet module not active. ";
    }
    if (digitalRead(helmetSwitchPin) == HIGH) {
      errorMsg += "Helmet not worn. ";
    }
    errorMsg += "Safety first!";
    
    sendSMS(errorMsg);
    
    // Flash warning
    for (int i = 0; i < 3; i++) {
      digitalWrite(lightIndicatorPin, HIGH);
      tone(buzzerPin, 2000, 200);
      delay(300);
      digitalWrite(lightIndicatorPin, LOW);
      delay(300);
    }
    
    Serial.println("[SAFETY] Engine start BLOCKED by safety interlock!");
    return;
  }
  
  // All checks passed - start engine
  digitalWrite(relayPin, HIGH);
  digitalWrite(lightIndicatorPin, LOW);
  engineRunning = true;
  sendSMS("Engine Started. All safety checks passed.");
  Serial.println("[ENGINE] Started successfully!");
}

void stopEngine() {
  digitalWrite(relayPin, LOW);
  digitalWrite(lightIndicatorPin, HIGH);
  engineRunning = false;
  sendSMS("Engine Stopped.");
  Serial.println("[ENGINE] Stopped.");
}

// ======= REST OF THE FUNCTIONS (unchanged) =======

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
  gsmSerial.println("AT"); delay(1000);
  gsmSerial.println("AT+CMGF=1"); delay(1000);
  gsmSerial.println("AT+CNMI=1,2,0,0,0"); delay(1000);
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
  else startEngine(); // Will check safety interlock
  
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
    
    if (command == "START") startEngine(); // Will check safety interlock
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

void sendLiveToFirebase(float lat, float lng, float speed, float v, bool h, bool t, bool l, bool r) {
  StaticJsonDocument<512> doc;
  doc["locationLat"] = lat;
  doc["locationLng"] = lng;
  doc["speed"] = speed;
  doc["batteryVoltage"] = v;
  doc["headlight"] = h;
  doc["taillight"] = t;
  doc["leftSignal"] = l;
  doc["rightSignal"] = r;
  
  String payload;
  serializeJson(doc, payload);
  
  HTTPClient http;
  http.begin(firebaseHost + livePath);
  http.addHeader("Content-Type", "application/json");
  http.PUT(payload);
  http.end();
}

void sendLiveToFirebaseNoGPS(float v, bool h, bool t, bool l, bool r) {
  StaticJsonDocument<256> doc;
  doc["batteryVoltage"] = v;
  doc["headlight"] = h;
  doc["taillight"] = t;
  doc["leftSignal"] = l;
  doc["rightSignal"] = r;
  
  String payload;
  serializeJson(doc, payload);
  
  HTTPClient http;
  http.begin(firebaseHost + livePath);
  http.addHeader("Content-Type", "application/json");
  http.PUT(payload);
  http.end();
}

void sendCrashEventToFirebase(float lat, float lng, float impact, float roll) {
  StaticJsonDocument<256> doc;
  doc["timestamp"] = gpsToUnixTime() * 1000UL;
  doc["lat"] = lat;
  doc["lng"] = lng;
  doc["hasGPS"] = true;
  doc["impactStrength"] = impact;
  doc["roll"] = roll;
  
  String payload;
  serializeJson(doc, payload);
  
  HTTPClient http;
  http.begin(firebaseHost + crashPath);
  http.addHeader("Content-Type", "application/json");
  http.POST(payload);
  http.end();
}

void sendCrashEventToFirebaseNoGPS(float impact, float roll) {
  StaticJsonDocument<256> doc;
  doc["timestamp"] = gpsToUnixTime() * 1000UL;
  doc["hasGPS"] = false;
  doc["impactStrength"] = impact;
  doc["roll"] = roll;
  
  String payload;
  serializeJson(doc, payload);
  
  HTTPClient http;
  http.begin(firebaseHost + crashPath);
  http.addHeader("Content-Type", "application/json");
  http.POST(payload);
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
    updateHelmetStatusInFirebase(true, "Normal", 0.0);
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
    updateHelmetStatusInFirebase(false, "Normal", 0.0);
    Serial.println("[HELMET] Helmet REMOVED");
    
    // ✅ Stop engine if helmet is removed while running
    if (engineRunning) {
      Serial.println("[SAFETY] Helmet removed - stopping engine!");
      stopEngine();
    }
  }
}
