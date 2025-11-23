/*
VigiLERT ESP32 Motorcycle Module - NO GSM VERSION
✅ Crash Detection → Relay OFF
✅ Alcohol Detection → Relay OFF  
✅ Helmet Disconnected → Relay OFF
❌ GSM/SMS removed for testing
*/

#include <WiFi.h>
#include <HTTPClient.h>
#include <TinyGPSPlus.h>
#include <HardwareSerial.h>
#include <Wire.h>
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <ArduinoJson.h>

// WiFi & Firebase
const char* ssid = "DPWH";
const char* password = "12345678900";
const String firebaseHost = "https://vigilance-shield-default-rtdb.firebaseio.com";
const String userUID = "MnzBjTBslZNijOkq732PE91hHa23";
const String crashPath = "/helmet_public/" + userUID + "/crashes.json";

// Pins - GPIO 4 is safe (not used by WiFi/I2C/SPI)
const int relayPin = 4;  // Changed to GPIO 4
const int ledPin = 13;

// GPS
HardwareSerial gpsSerial(1);
TinyGPSPlus gps;

// MPU6050
Adafruit_MPU6050 mpu;
sensors_event_t accel, gyro, temp;

// Crash detection
bool crashDetected = false;
const float ACCEL_THRESHOLD = 20.0;
const float ROLL_THRESHOLD = 60.0;
unsigned long lastCrashTime = 0;
const unsigned long CRASH_COOLDOWN = 5000;

// Safety status
bool isHelmetModuleActive = false;
bool engineRunning = false;
unsigned long lastHelmetCheck = 0;

void setup() {
  Serial.begin(115200);
  delay(100);
  
  Serial.println("\n=== VigiLERT - NO GSM VERSION ===");
  Serial.println("Testing: Crash + Alcohol + Helmet → Relay");
  
  // Initialize MPU6050 FIRST
  Wire.begin(21, 22);
  if (!mpu.begin()) {
    Serial.println("MPU6050 failed!");
    while (1) delay(10);
  }
  mpu.setAccelerometerRange(MPU6050_RANGE_16_G);
  Serial.println("MPU6050 OK");
  
  // GPS
  gpsSerial.begin(9600, SERIAL_8N1, 16, 17);
  Serial.println("GPS started");
  
  // WiFi
  WiFi.begin(ssid, password);
  Serial.print("WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println(" OK");
  
  // Initialize relay pin LAST (after WiFi and I2C)
  pinMode(relayPin, OUTPUT);
  pinMode(ledPin, OUTPUT);
  
  // Force relay OFF multiple times
  for (int i = 0; i < 10; i++) {
    digitalWrite(relayPin, LOW);
    delay(100);
  }
  
  Serial.printf("Relay GPIO %d initialized: %d (should be 0=OFF)\n", 
    relayPin, digitalRead(relayPin));
  
  Serial.println("\n=== SYSTEM READY ===");
  Serial.println("Commands: RELAY, CRASH, CLEAR, STATUS");
  Serial.println("Tilt >60° to trigger crash\n");
}

void loop() {
  // Commands
  if (Serial.available()) {
    String cmd = Serial.readStringUntil('\n');
    cmd.trim();
    cmd.toUpperCase();
    
    if (cmd == "RELAY") {
      Serial.println("\n=== RELAY TEST ===");
      Serial.println("HIGH (ON)");
      digitalWrite(relayPin, HIGH);
      delay(2000);
      Serial.println("LOW (OFF)");
      digitalWrite(relayPin, LOW);
      delay(2000);
      Serial.println("Done\n");
    }
    else if (cmd == "CRASH") {
      Serial.println("\n=== MANUAL CRASH ===");
      triggerCrash(25.0, 70.0);
    }
    else if (cmd == "CLEAR") {
      Serial.println("\n=== CLEAR CRASH ===");
      crashDetected = false;
      Serial.println("Cleared\n");
    }
    else if (cmd == "STATUS") {
      Serial.printf("\nHelmet: %s | Alcohol: %s | Crash: %s | Relay: %s\n\n",
        isHelmetModuleActive ? "ON" : "OFF",
        isAlcoholSafe() ? "SAFE" : "DANGER",
        crashDetected ? "YES" : "NO",
        digitalRead(relayPin) ? "ON" : "OFF");
    }
  }
  
  // GPS
  while (gpsSerial.available() > 0) {
    gps.encode(gpsSerial.read());
  }
  
  // MPU6050
  mpu.getEvent(&accel, &gyro, &temp);
  float totalAccel = sqrt(
    accel.acceleration.x * accel.acceleration.x +
    accel.acceleration.y * accel.acceleration.y +
    accel.acceleration.z * accel.acceleration.z
  );
  float roll = atan2(accel.acceleration.y, accel.acceleration.z) * 180.0 / PI;
  
  // Display
  static unsigned long lastDisplay = 0;
  if (millis() - lastDisplay > 200) {
    Serial.printf("Accel: %.2fg | Roll: %.1f° | Crash: %s\n", 
      totalAccel, roll, crashDetected ? "YES" : "NO");
    lastDisplay = millis();
  }
  
  // Check helmet status every 10 seconds
  if (millis() - lastHelmetCheck > 10000) {
    checkHelmetStatus();
    lastHelmetCheck = millis();
  }
  
  // SAFETY CHECK - Control relay based on 3 conditions
  bool helmetOK = isHelmetModuleActive;
  bool alcoholOK = isAlcoholSafe();
  bool noCrash = !crashDetected;
  bool safeToRun = helmetOK && alcoholOK && noCrash;
  
  if (!safeToRun) {
    // Turn relay OFF
    digitalWrite(relayPin, LOW);
    digitalWrite(ledPin, HIGH);  // LED on when blocked
    
    if (engineRunning) {
      engineRunning = false;
      Serial.println("\n⚠️ SAFETY SHUTDOWN");
      if (!helmetOK) Serial.println("  ❌ Helmet OFF");
      if (!alcoholOK) Serial.println("  ❌ Alcohol detected");
      if (!noCrash) Serial.println("  ❌ Crash detected");
      Serial.println("✓ Relay OFF\n");
    }
  } else {
    // Safe - relay can be controlled
    if (engineRunning) {
      digitalWrite(relayPin, HIGH);  // Relay ON
      digitalWrite(ledPin, LOW);
    } else {
      digitalWrite(relayPin, LOW);  // Relay OFF
      digitalWrite(ledPin, LOW);
    }
  }
  
  // Crash detection
  bool crashCondition = (totalAccel >= ACCEL_THRESHOLD || 
                         roll < -ROLL_THRESHOLD || 
                         roll > ROLL_THRESHOLD);
  
  if (crashCondition && !crashDetected) {
    unsigned long timeSince = millis() - lastCrashTime;
    if (timeSince >= CRASH_COOLDOWN) {
      Serial.println("\n⚠️⚠️⚠️ CRASH DETECTED! ⚠️⚠️⚠️");
      Serial.printf("Impact: %.2fg | Roll: %.1f°\n", totalAccel, roll);
      triggerCrash(totalAccel, roll);
    }
  }
  
  // Clear crash when stable
  if (crashDetected) {
    bool stable = (roll > -30 && roll < 30) && (totalAccel < 15.0);
    if (stable) {
      crashDetected = false;
      Serial.println("✓ Crash cleared\n");
    }
  }
  
  delay(10);
}

void triggerCrash(float impact, float roll) {
  crashDetected = true;
  lastCrashTime = millis();
  
  // FORCE RELAY OFF - Multiple times to ensure it works
  Serial.println("🚨 RELAY OFF!");
  for (int i = 0; i < 20; i++) {
    digitalWrite(relayPin, LOW);
    delayMicroseconds(100);
  }
  Serial.printf("GPIO %d = %d (forced 20x)\n", relayPin, digitalRead(relayPin));
  
  // Verify it's actually LOW
  if (digitalRead(relayPin) == LOW) {
    Serial.println("✓ GPIO is LOW");
  } else {
    Serial.println("✗ GPIO still HIGH!");
  }
  
  // Send to Firebase
  sendCrashToFirebase(impact, roll);
  
  Serial.println("✓ Check relay LED - should be OFF!\n");
}

void sendCrashToFirebase(float impact, float roll) {
  if (WiFi.status() != WL_CONNECTED) return;
  
  StaticJsonDocument<256> doc;
  doc["timestamp"] = millis();
  doc["hasGPS"] = gps.location.isValid();
  if (gps.location.isValid()) {
    doc["lat"] = gps.location.lat();
    doc["lng"] = gps.location.lng();
  }
  doc["impactStrength"] = impact;
  doc["roll"] = roll;
  doc["type"] = "crash";
  
  String payload;
  serializeJson(doc, payload);
  
  HTTPClient http;
  http.begin(firebaseHost + crashPath);
  http.addHeader("Content-Type", "application/json");
  http.setTimeout(3000);
  
  int code = http.POST(payload);
  Serial.printf("Firebase: %d\n", code);
  
  http.end();
}

void checkHelmetStatus() {
  HTTPClient http;
  String url = firebaseHost + "/helmet_public/" + userUID + "/devices/helmet.json";
  http.begin(url);
  http.setTimeout(2000);
  
  int code = http.GET();
  if (code == 200) {
    String response = http.getString();
    StaticJsonDocument<256> doc;
    deserializeJson(doc, response);
    
    String status = doc["status"] | "Off";
    unsigned long lastHB = doc["lastHeartbeat"] | 0;
    unsigned long now = 1700000000000UL + millis();
    unsigned long diff = (now > lastHB) ? (now - lastHB) : 999999;
    
    isHelmetModuleActive = (status == "On" && diff < 15000);
  } else {
    isHelmetModuleActive = false;
  }
  
  http.end();
}

bool isAlcoholSafe() {
  HTTPClient http;
  String url = firebaseHost + "/helmet_public/" + userUID + "/alcohol/status.json";
  http.begin(url);
  http.setTimeout(2000);
  
  int code = http.GET();
  if (code == 200) {
    String response = http.getString();
    StaticJsonDocument<128> doc;
    deserializeJson(doc, response);
    
    String status = doc["status"] | "Safe";
    http.end();
    return (status == "Safe");
  }
  
  http.end();
  return true;  // Default safe
}
