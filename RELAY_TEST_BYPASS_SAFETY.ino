/*
 * RELAY TEST WITH SAFETY BYPASS
 * This temporarily disables safety checks so you can test relay hardware
 * 
 * ⚠️ WARNING: For testing only! Re-enable safety checks after testing!
 */

#include <WiFi.h>
#include <Wire.h>
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>

// WiFi Config
const char* ssid = "DPWH";
const char* password = "12345678900";

// Pin Config - FIXED CONFLICTS
const int relayPin = 13;  // GPIO 13 - PROVEN TO WORK!
const int ledPin = 2;     // GPIO 2 (not GPIO 5 - conflicts with GSM)
const int buzzerPin = 12; // GPIO 12

// MPU6050
Adafruit_MPU6050 mpu;
sensors_event_t accel, gyro, temp;

// Crash detection
bool crashDetected = false;
const float ACCEL_THRESHOLD = 15.0;
float currentTotalAccel = 0.0;
float currentRoll = 0.0;

// Engine control
bool engineRunning = false;

void setup() {
  Serial.begin(115200);
  delay(100);
  
  Serial.println("\n╔════════════════════════════════════════╗");
  Serial.println("║   RELAY TEST - SAFETY BYPASS          ║");
  Serial.println("║   ⚠️  ALL SAFETY CHECKS DISABLED!      ║");
  Serial.println("╠════════════════════════════════════════╣");
  Serial.println("║ This is for testing relay hardware    ║");
  Serial.println("║ Re-enable safety checks after test!   ║");
  Serial.println("╚════════════════════════════════════════╝\n");
  
  // Setup pins
  pinMode(relayPin, OUTPUT);
  pinMode(ledPin, OUTPUT);
  pinMode(buzzerPin, OUTPUT);
  
  // ✅ ACTIVE-LOW RELAY: Start with HIGH (OFF)
  digitalWrite(relayPin, HIGH);
  digitalWrite(ledPin, LOW);
  
  Serial.printf("Relay Pin: GPIO %d\n", relayPin);
  Serial.printf("LED Pin: GPIO %d\n", ledPin);
  Serial.println("Relay Type: ACTIVE-LOW (HIGH=OFF, LOW=ON)");
  Serial.printf("Initial Relay State: %d (HIGH/OFF)\n\n", digitalRead(relayPin));
  
  // Setup MPU6050
  Wire.begin(21, 22);
  if (!mpu.begin()) {
    Serial.println("⚠️ MPU6050 not found! Crash detection disabled.");
  } else {
    Serial.println("✓ MPU6050 initialized");
    mpu.setAccelerometerRange(MPU6050_RANGE_16_G);
    mpu.setGyroRange(MPU6050_RANGE_2000_DEG);
    mpu.setFilterBandwidth(MPU6050_BAND_21_HZ);
  }
  
  // Connect WiFi (optional)
  WiFi.begin(ssid, password);
  Serial.print("Connecting WiFi");
  int attempts = 0;
  while (WiFi.status() != WL_CONNECTED && attempts < 10) {
    Serial.print(".");
    delay(500);
    attempts++;
  }
  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("\n✓ WiFi Connected");
  } else {
    Serial.println("\n⚠️ WiFi not connected (optional for this test)");
  }
  
  Serial.println("\n📋 COMMANDS:");
  Serial.println("  START  - Start engine (relay ON)");
  Serial.println("  STOP   - Stop engine (relay OFF)");
  Serial.println("  CRASH  - Simulate crash (relay OFF)");
  Serial.println("  STATUS - Show current state");
  Serial.println("  AUTO   - Enable auto crash detection");
  Serial.println("  MANUAL - Disable auto crash detection\n");
  
  delay(2000);
  showStatus();
}

void loop() {
  // Read MPU6050
  mpu.getEvent(&accel, &gyro, &temp);
  currentTotalAccel = sqrt(accel.acceleration.x * accel.acceleration.x +
                           accel.acceleration.y * accel.acceleration.y +
                           accel.acceleration.z * accel.acceleration.z);
  currentRoll = atan2(accel.acceleration.y, accel.acceleration.z) * 180.0 / PI;
  
  // Auto crash detection (if enabled)
  static bool autoDetect = true;
  if (autoDetect && !crashDetected && engineRunning) {
    if (currentTotalAccel >= ACCEL_THRESHOLD || abs(currentRoll) > 40) {
      Serial.println("\n🚨🚨🚨 CRASH DETECTED! 🚨🚨🚨");
      Serial.printf("Impact: %.2f g | Roll: %.1f°\n", currentTotalAccel, currentRoll);
      
      // IMMEDIATE RELAY SHUTDOWN
      Serial.println("🚨 EMERGENCY SHUTDOWN - Cutting relay NOW!");
      digitalWrite(relayPin, HIGH);  // ACTIVE-LOW: HIGH = OFF
      digitalWrite(ledPin, LOW);
      engineRunning = false;
      delay(100);
      
      Serial.printf("🚨 Relay GPIO %d = %d (should be 1 = HIGH/OFF)\n", relayPin, digitalRead(relayPin));
      Serial.println("🚨 Relay LEDs should turn OFF!\n");
      
      crashDetected = true;
      
      // Alert
      for (int i = 0; i < 5; i++) {
        tone(buzzerPin, 1500, 200);
        digitalWrite(ledPin, HIGH);
        delay(200);
        digitalWrite(ledPin, LOW);
        delay(200);
      }
    }
  }
  
  // Clear crash state when upright
  if (crashDetected && abs(currentRoll) < 30 && currentTotalAccel < 12.0) {
    crashDetected = false;
    Serial.println("[INFO] Crash state cleared - ready for new detection\n");
  }
  
  // Serial commands
  if (Serial.available()) {
    String cmd = Serial.readStringUntil('\n');
    cmd.trim();
    cmd.toUpperCase();
    
    if (cmd == "START") {
      Serial.println("\n[COMMAND] Starting engine...");
      digitalWrite(relayPin, LOW);   // ACTIVE-LOW: LOW = ON
      digitalWrite(ledPin, HIGH);
      engineRunning = true;
      delay(100);
      Serial.printf("✓ Relay GPIO %d = %d (should be 0 = LOW/ON)\n", relayPin, digitalRead(relayPin));
      Serial.println("✓ Engine started - relay LEDs should turn ON!");
      showStatus();
    }
    else if (cmd == "STOP") {
      Serial.println("\n[COMMAND] Stopping engine...");
      digitalWrite(relayPin, HIGH);  // ACTIVE-LOW: HIGH = OFF
      digitalWrite(ledPin, LOW);
      engineRunning = false;
      delay(100);
      Serial.printf("✓ Relay GPIO %d = %d (should be 1 = HIGH/OFF)\n", relayPin, digitalRead(relayPin));
      Serial.println("✓ Engine stopped - relay LEDs should turn OFF!");
      showStatus();
    }
    else if (cmd == "CRASH") {
      Serial.println("\n🚨 MANUAL CRASH TEST");
      Serial.println("🚨 EMERGENCY SHUTDOWN - Cutting relay NOW!");
      digitalWrite(relayPin, HIGH);  // ACTIVE-LOW: HIGH = OFF
      digitalWrite(ledPin, LOW);
      engineRunning = false;
      delay(100);
      Serial.printf("🚨 Relay GPIO %d = %d (should be 1 = HIGH/OFF)\n", relayPin, digitalRead(relayPin));
      Serial.println("🚨 Relay LEDs should turn OFF!");
      showStatus();
    }
    else if (cmd == "STATUS") {
      showStatus();
    }
    else if (cmd == "AUTO") {
      autoDetect = true;
      Serial.println("\n[MODE] Auto crash detection ENABLED");
    }
    else if (cmd == "MANUAL") {
      autoDetect = false;
      Serial.println("\n[MODE] Auto crash detection DISABLED");
    }
    else {
      Serial.println("\nUnknown command. Type: START, STOP, CRASH, STATUS, AUTO, MANUAL");
    }
  }
  
  // Status display every 3 seconds
  static unsigned long lastPrint = 0;
  if (millis() - lastPrint > 3000) {
    Serial.printf("[SENSOR] Accel: %.2f g | Roll: %.1f° | Engine: %s | Relay: %s | Crash: %s\n",
                  currentTotalAccel, currentRoll,
                  engineRunning ? "RUNNING" : "STOPPED",
                  digitalRead(relayPin) ? "OFF" : "ON",
                  crashDetected ? "YES" : "NO");
    lastPrint = millis();
  }
  
  delay(50);
}

void showStatus() {
  int relayState = digitalRead(relayPin);
  int ledState = digitalRead(ledPin);
  
  Serial.println("\n╔════════════════════════════════════════╗");
  Serial.println("║   CURRENT STATUS                       ║");
  Serial.println("╠════════════════════════════════════════╣");
  Serial.printf("║ Relay GPIO: %d                         ║\n", relayPin);
  Serial.printf("║ Relay Digital: %s                       ║\n", 
                relayState ? "HIGH (1)" : "LOW (0) ");
  Serial.printf("║ Relay Status: %s (ACTIVE-LOW)          ║\n", 
                relayState ? "OFF     " : "ON      ");
  Serial.printf("║ Engine Status: %s                      ║\n", 
                engineRunning ? "RUNNING " : "STOPPED ");
  Serial.printf("║ LED Status: %s                          ║\n", 
                ledState ? "ON      " : "OFF     ");
  Serial.printf("║ MPU6050 Accel: %.2f g                   ║\n", currentTotalAccel);
  Serial.printf("║ MPU6050 Roll: %.1f°                     ║\n", currentRoll);
  Serial.println("╠════════════════════════════════════════╣");
  Serial.println("║ ⚠️  SAFETY CHECKS BYPASSED!            ║");
  Serial.println("║ For testing only - re-enable safety!  ║");
  Serial.println("╚════════════════════════════════════════╝\n");
}