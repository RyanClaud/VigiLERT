/*
 * RELAY BYPASS TEST - Test relay without safety checks
 * This bypasses helmet/alcohol checks to test relay hardware directly
 * 
 * WARNING: This is for testing only! Do not use in production!
 */

#include <WiFi.h>
#include <Wire.h>
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>

// WiFi Config
const char* ssid = "DPWH";
const char* password = "12345678900";

// Pin Config
const int relayPin = 25;  // GPIO 25
const int ledPin = 13;    // LED indicator

// MPU6050
Adafruit_MPU6050 mpu;
sensors_event_t accel, gyro, temp;

// Crash detection
bool crashDetected = false;
const float ACCEL_THRESHOLD = 15.0;
float currentTotalAccel = 0.0;
float currentRoll = 0.0;

void setup() {
  Serial.begin(115200);
  delay(100);
  
  Serial.println("\n╔════════════════════════════════════════╗");
  Serial.println("║   RELAY BYPASS TEST                    ║");
  Serial.println("║   (No Safety Checks)                   ║");
  Serial.println("╠════════════════════════════════════════╣");
  Serial.println("║ ⚠️  WARNING: Testing Mode Only!        ║");
  Serial.println("║ This bypasses all safety checks        ║");
  Serial.println("╚════════════════════════════════════════╝\n");
  
  // Setup pins
  pinMode(relayPin, OUTPUT);
  pinMode(ledPin, OUTPUT);
  
  // Start with relay OFF
  digitalWrite(relayPin, LOW);
  digitalWrite(ledPin, LOW);
  
  Serial.printf("Relay Pin: GPIO %d\n", relayPin);
  Serial.printf("Initial State: %d (LOW/OFF)\n\n", digitalRead(relayPin));
  
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
  
  // Connect WiFi (optional, for Firebase logging)
  WiFi.begin(ssid, password);
  Serial.print("Connecting WiFi");
  int attempts = 0;
  while (WiFi.status() != WL_CONNECTED && attempts < 20) {
    Serial.print(".");
    delay(500);
    attempts++;
  }
  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("\n✓ WiFi Connected: " + WiFi.localIP().toString());
  } else {
    Serial.println("\n⚠️ WiFi not connected (optional for this test)");
  }
  
  Serial.println("\n📋 COMMANDS:");
  Serial.println("  ON     - Turn relay ON (engine allowed)");
  Serial.println("  OFF    - Turn relay OFF (engine blocked)");
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
  if (autoDetect && !crashDetected) {
    if (currentTotalAccel >= ACCEL_THRESHOLD || abs(currentRoll) > 40) {
      Serial.println("\n🚨🚨🚨 CRASH DETECTED! 🚨🚨🚨");
      Serial.printf("Impact: %.2f g | Roll: %.1f°\n", currentTotalAccel, currentRoll);
      
      // IMMEDIATE RELAY SHUTDOWN
      Serial.println("🚨 EMERGENCY SHUTDOWN - Cutting relay NOW!");
      digitalWrite(relayPin, LOW);
      digitalWrite(ledPin, LOW);
      delay(100);
      
      Serial.printf("🚨 Relay GPIO %d = %d (should be 0)\n", relayPin, digitalRead(relayPin));
      Serial.println("🚨 Relay should be OFF!\n");
      
      crashDetected = true;
      
      // Alert
      for (int i = 0; i < 5; i++) {
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
    
    if (cmd == "ON") {
      Serial.println("\n[COMMAND] Turning relay ON...");
      digitalWrite(relayPin, HIGH);
      digitalWrite(ledPin, HIGH);
      delay(100);
      showStatus();
    }
    else if (cmd == "OFF") {
      Serial.println("\n[COMMAND] Turning relay OFF...");
      digitalWrite(relayPin, LOW);
      digitalWrite(ledPin, LOW);
      delay(100);
      showStatus();
    }
    else if (cmd == "CRASH") {
      Serial.println("\n🚨 MANUAL CRASH TEST");
      Serial.println("🚨 EMERGENCY SHUTDOWN - Cutting relay NOW!");
      digitalWrite(relayPin, LOW);
      digitalWrite(ledPin, LOW);
      delay(100);
      Serial.printf("🚨 Relay GPIO %d = %d (should be 0)\n", relayPin, digitalRead(relayPin));
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
      Serial.println("\nUnknown command. Type: ON, OFF, CRASH, STATUS, AUTO, MANUAL");
    }
  }
  
  // Status display every 2 seconds
  static unsigned long lastPrint = 0;
  if (millis() - lastPrint > 2000) {
    Serial.printf("[SENSOR] Accel: %.2f g | Roll: %.1f° | Relay: %s | Crash: %s\n",
                  currentTotalAccel, currentRoll,
                  digitalRead(relayPin) ? "ON" : "OFF",
                  crashDetected ? "YES" : "NO");
    lastPrint = millis();
  }
  
  delay(50);
}

void showStatus() {
  int relayState = digitalRead(relayPin);
  int ledState = digitalRead(ledPin);
  
  Serial.println("\n╔════════════════════════════════════════╗");
  Serial.println("║   RELAY STATUS                         ║");
  Serial.println("╠════════════════════════════════════════╣");
  Serial.printf("║ GPIO Pin: %d                           ║\n", relayPin);
  Serial.printf("║ Digital State: %s                       ║\n", 
                relayState ? "HIGH (1)" : "LOW (0) ");
  Serial.printf("║ Relay Status: %s                        ║\n", 
                relayState ? "ON      " : "OFF     ");
  Serial.printf("║ LED Status: %s                          ║\n", 
                ledState ? "ON      " : "OFF     ");
  Serial.printf("║ MPU6050 Accel: %.2f g                   ║\n", currentTotalAccel);
  Serial.printf("║ MPU6050 Roll: %.1f°                     ║\n", currentRoll);
  Serial.println("╠════════════════════════════════════════╣");
  Serial.println("║ ⚠️  Safety checks BYPASSED             ║");
  Serial.println("║ This is for testing only!              ║");
  Serial.println("╚════════════════════════════════════════╝\n");
}
