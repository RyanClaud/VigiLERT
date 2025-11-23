/*
MINIMAL VERSION - Crash Detection + Relay Control ONLY
This removes all blocking operations to ensure relay responds
*/

#include <WiFi.h>
#include <HTTPClient.h>
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

// Pins - GPIO 23 is safer (no WiFi/I2C conflicts)
const int relayPin = 23;  // Changed from 25 to 23
const int ledPin = 13;

// MPU6050
Adafruit_MPU6050 mpu;
sensors_event_t accel, gyro, temp;

// Crash detection
bool crashDetected = false;
const float ACCEL_THRESHOLD = 20.0;
const float ROLL_THRESHOLD = 60.0;
unsigned long lastCrashTime = 0;
const unsigned long CRASH_COOLDOWN = 5000;

void setup() {
  Serial.begin(115200);
  delay(100);
  
  Serial.println("\n=== MINIMAL CRASH DETECTION + RELAY ===");
  
  // Initialize pins
  pinMode(relayPin, OUTPUT);
  pinMode(ledPin, OUTPUT);
  digitalWrite(relayPin, LOW);  // Relay OFF (LOW = OFF, HIGH = ON)
  digitalWrite(ledPin, LOW);
  
  Serial.printf("Relay pin %d initialized to LOW (OFF)\n", relayPin);
  Serial.println("NOTE: This relay is ACTIVE-HIGH (HIGH=ON, LOW=OFF)");
  
  // Initialize MPU6050
  Wire.begin(21, 22);
  if (!mpu.begin()) {
    Serial.println("Failed to find MPU6050!");
    while (1) delay(10);
  }
  Serial.println("MPU6050 initialized");
  
  mpu.setAccelerometerRange(MPU6050_RANGE_16_G);
  mpu.setGyroRange(MPU6050_RANGE_2000_DEG);
  mpu.setFilterBandwidth(MPU6050_BAND_21_HZ);
  
  // Connect WiFi
  WiFi.begin(ssid, password);
  Serial.print("Connecting WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println("\nWiFi connected!");
  
  Serial.println("\n=== SYSTEM READY ===");
  Serial.println("Commands: RELAY, CRASH, CLEAR");
  Serial.println("Tilt >60° to trigger crash\n");
}

void loop() {
  // Serial commands
  if (Serial.available()) {
    String cmd = Serial.readStringUntil('\n');
    cmd.trim();
    cmd.toUpperCase();
    
    if (cmd == "RELAY") {
      Serial.println("\n=== RELAY TEST ===");
      Serial.println("🟢 Relay ON (HIGH) - DS1 should be ON");
      digitalWrite(relayPin, HIGH);
      digitalWrite(ledPin, HIGH);
      delay(2000);
      
      Serial.println("🔴 Relay OFF (LOW) - DS1 should be OFF");
      digitalWrite(relayPin, LOW);
      digitalWrite(ledPin, LOW);
      delay(2000);
      
      Serial.println("=== TEST COMPLETE ===\n");
    }
    else if (cmd == "CRASH") {
      Serial.println("\n=== MANUAL CRASH TRIGGER ===");
      triggerCrash(25.0, 70.0);
    }
    else if (cmd == "CLEAR") {
      Serial.println("\n=== CLEARING CRASH ===");
      crashDetected = false;
      digitalWrite(relayPin, LOW);  // Relay OFF (active-HIGH)
      digitalWrite(ledPin, LOW);
      Serial.println("Crash cleared - relay OFF\n");
    }
  }
  
  // Read MPU6050
  mpu.getEvent(&accel, &gyro, &temp);
  
  float totalAccel = sqrt(
    accel.acceleration.x * accel.acceleration.x +
    accel.acceleration.y * accel.acceleration.y +
    accel.acceleration.z * accel.acceleration.z
  );
  
  float roll = atan2(accel.acceleration.y, accel.acceleration.z) * 180.0 / PI;
  
  // Display readings
  static unsigned long lastDisplay = 0;
  if (millis() - lastDisplay > 200) {
    Serial.printf("Accel: %.2fg | Roll: %.1f° | Crash: %s\n", 
      totalAccel, roll, crashDetected ? "YES" : "NO");
    lastDisplay = millis();
  }
  
  // Crash detection
  bool crashCondition = (totalAccel >= ACCEL_THRESHOLD || 
                         roll < -ROLL_THRESHOLD || 
                         roll > ROLL_THRESHOLD);
  
  if (crashCondition && !crashDetected) {
    unsigned long timeSinceLastCrash = millis() - lastCrashTime;
    
    if (timeSinceLastCrash >= CRASH_COOLDOWN) {
      Serial.println("\n⚠️⚠️⚠️ CRASH DETECTED! ⚠️⚠️⚠️");
      Serial.printf("Impact: %.2fg | Roll: %.1f°\n", totalAccel, roll);
      
      triggerCrash(totalAccel, roll);
    }
  }
  
  // FORCE relay OFF continuously during crash
  if (crashDetected) {
    digitalWrite(relayPin, LOW);  // Keep forcing LOW
    
    // Clear crash when stable
    bool isUpright = (roll > -30 && roll < 30);
    bool isStable = (totalAccel < 15.0);
    
    if (isUpright && isStable) {
      crashDetected = false;
      digitalWrite(relayPin, LOW);  // Relay OFF
      digitalWrite(ledPin, LOW);
      Serial.println("✓ Crash cleared - stable\n");
    }
  }
  
  delay(10);  // Small delay
}

void triggerCrash(float impact, float roll) {
  crashDetected = true;
  lastCrashTime = millis();
  
  // IMMEDIATE RELAY CONTROL - Force it multiple times!
  Serial.println("\n🚨🚨🚨 CRASH DETECTED! 🚨🚨🚨");
  Serial.println("🔴 FORCING RELAY OFF NOW!");
  
  // Force relay OFF 10 times to ensure it works
  for (int i = 0; i < 10; i++) {
    digitalWrite(relayPin, LOW);
    delay(10);
  }
  
  Serial.printf("GPIO %d forced to LOW 10 times\n", relayPin);
  Serial.printf("GPIO %d current state: %d (should be 0)\n", relayPin, digitalRead(relayPin));
  
  // Verify relay is actually LOW
  if (digitalRead(relayPin) == LOW) {
    Serial.println("✓ GPIO is LOW - relay should be OFF");
  } else {
    Serial.println("✗ ERROR: GPIO is still HIGH!");
  }
  
  // LED fast blink
  digitalWrite(ledPin, HIGH);
  
  // Send to Firebase (non-blocking)
  sendCrashToFirebase(impact, roll);
  
  Serial.println("✓ Check relay - DS1 and DS2 should be OFF!\n");
}

void sendCrashToFirebase(float impact, float roll) {
  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("WiFi not connected!");
    return;
  }
  
  StaticJsonDocument<256> doc;
  doc["timestamp"] = millis();
  doc["hasGPS"] = false;
  doc["impactStrength"] = impact;
  doc["roll"] = roll;
  doc["type"] = "crash";
  
  String payload;
  serializeJson(doc, payload);
  
  Serial.println("Sending to Firebase...");
  
  HTTPClient http;
  http.begin(firebaseHost + crashPath);
  http.addHeader("Content-Type", "application/json");
  http.setTimeout(3000);
  
  int code = http.POST(payload);
  Serial.printf("Firebase response: %d\n", code);
  
  if (code == 200 || code == 201) {
    Serial.println("✓ Crash saved to Firebase!");
  } else {
    Serial.printf("✗ Firebase error: %d\n", code);
  }
  
  http.end();
}
