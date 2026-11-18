// ═══════════════════════════════════════════════════════════════════════════
// MPU6050 LEAN ANGLE TEST - REAL SENSOR READINGS
// ═══════════════════════════════════════════════════════════════════════════
// 🎯 PURPOSE: Get actual lean angle readings from MPU6050 sensor
// 📐 MEASURE: Roll, Pitch, and calculated lean angles
// 🏍️ TEST: Different motorcycle positions to determine crash thresholds
// 📊 OUTPUT: Real-time angle data for threshold calibration
// ═══════════════════════════════════════════════════════════════════════════

#include <Wire.h>
#include <MPU6050.h>

// ═══════════════════════════════════════════════════════════════════════════
// MPU6050 CONFIGURATION
// ═══════════════════════════════════════════════════════════════════════════

MPU6050 mpu;

// MPU6050 I2C pins (default for ESP32)
const int SDA_PIN = 21;
const int SCL_PIN = 22;

// Raw sensor readings
int16_t ax, ay, az;
int16_t gx, gy, gz;

// Calculated angles
float roll = 0.0;
float pitch = 0.0;
float yaw = 0.0;

// Lean angle calculations
float leanAngle = 0.0;
float maxLeanAngle = 0.0;
float minLeanAngle = 0.0;

// Calibration values (will be calculated during setup)
float rollOffset = 0.0;
float pitchOffset = 0.0;

// Statistics tracking
unsigned long readingCount = 0;
float totalLeanAngle = 0.0;
float averageLeanAngle = 0.0;

// Timing
unsigned long lastReading = 0;
unsigned long lastStats = 0;
const unsigned long READING_INTERVAL = 100;  // 100ms = 10Hz
const unsigned long STATS_INTERVAL = 5000;   // 5 seconds

// Test modes
enum TestMode {
  CONTINUOUS_MONITORING,
  CALIBRATION_MODE,
  THRESHOLD_TESTING
};

TestMode currentMode = CONTINUOUS_MONITORING;

// ═══════════════════════════════════════════════════════════════════════════
// SETUP
// ═══════════════════════════════════════════════════════════════════════════

void setup() {
  Serial.begin(115200);
  delay(1000);
  
  Serial.println("\n╔════════════════════════════════════════════════════════════╗");
  Serial.println("║              MPU6050 LEAN ANGLE TEST                      ║");
  Serial.println("╠════════════════════════════════════════════════════════════╣");
  Serial.println("║ 🎯 Get actual lean angle readings from MPU6050            ║");
  Serial.println("║ 📐 Measure Roll, Pitch, and calculated lean angles        ║");
  Serial.println("║ 🏍️ Test different motorcycle positions                    ║");
  Serial.println("║ 📊 Determine proper crash detection thresholds            ║");
  Serial.println("╚════════════════════════════════════════════════════════════╝\n");

  // Initialize I2C
  Wire.begin(SDA_PIN, SCL_PIN);
  Serial.printf("[I2C] Initialized - SDA: GPIO%d, SCL: GPIO%d\n", SDA_PIN, SCL_PIN);

  // Initialize MPU6050
  Serial.println("[MPU6050] Initializing sensor...");
  mpu.initialize();
  
  // Test connection
  if (mpu.testConnection()) {
    Serial.println("[MPU6050] ✅ Connection successful!");
    
    // Get device ID
    uint8_t deviceId = mpu.getDeviceID();
    Serial.printf("[MPU6050] Device ID: 0x%02X\n", deviceId);
    
    // Configure sensor settings
    configureMPU6050();
    
    // Calibrate sensor
    calibrateSensor();
    
  } else {
    Serial.println("[MPU6050] ❌ Connection failed!");
    Serial.println("Check wiring:");
    Serial.println("  VCC → 3.3V");
    Serial.println("  GND → GND");
    Serial.printf("  SDA → GPIO%d\n", SDA_PIN);
    Serial.printf("  SCL → GPIO%d\n", SCL_PIN);
    while (1) {
      delay(1000);
      Serial.println("MPU6050 not connected. Check wiring and restart.");
    }
  }

  Serial.println("\n🚀 MPU6050 LEAN ANGLE TEST READY!");
  Serial.println("\n📋 COMMANDS:");
  Serial.println("  CALIBRATE  - Recalibrate sensor (place motorcycle upright)");
  Serial.println("  RESET      - Reset statistics");
  Serial.println("  STATS      - Show detailed statistics");
  Serial.println("  THRESHOLD  - Test specific lean angles");
  Serial.println("  HELP       - Show this help");
  Serial.println("\n🏍️ TESTING POSITIONS:");
  Serial.println("  1. Upright (0°) - Normal riding position");
  Serial.println("  2. Light lean (15-25°) - Normal cornering");
  Serial.println("  3. Moderate lean (25-35°) - Aggressive cornering");
  Serial.println("  4. Heavy lean (35-45°) - Racing lean");
  Serial.println("  5. Extreme lean (45°+) - Crash/fall simulation");
  Serial.println("\n📊 Real-time readings starting...\n");
}

// ═══════════════════════════════════════════════════════════════════════════
// MAIN LOOP
// ═══════════════════════════════════════════════════════════════════════════

void loop() {
  unsigned long currentTime = millis();
  
  // Read sensor data
  if (currentTime - lastReading >= READING_INTERVAL) {
    readMPU6050Data();
    calculateAngles();
    updateStatistics();
    displayRealTimeData();
    lastReading = currentTime;
  }
  
  // Show periodic statistics
  if (currentTime - lastStats >= STATS_INTERVAL) {
    showPeriodicStats();
    lastStats = currentTime;
  }
  
  // Handle serial commands
  handleSerialCommands();
  
  delay(10);
}

// ═══════════════════════════════════════════════════════════════════════════
// MPU6050 FUNCTIONS
// ═══════════════════════════════════════════════════════════════════════════

void configureMPU6050() {
  Serial.println("[MPU6050] Configuring sensor settings...");
  
  // Set accelerometer range to ±2g (most sensitive)
  mpu.setFullScaleAccelRange(MPU6050_ACCEL_FS_2);
  Serial.println("[MPU6050] Accelerometer range: ±2g");
  
  // Set gyroscope range to ±250°/s (most sensitive)
  mpu.setFullScaleGyroRange(MPU6050_GYRO_FS_250);
  Serial.println("[MPU6050] Gyroscope range: ±250°/s");
  
  // Set digital low pass filter
  mpu.setDLPFMode(MPU6050_DLPF_BW_20);
  Serial.println("[MPU6050] Low pass filter: 20Hz");
  
  // Set sample rate to 100Hz
  mpu.setRate(9); // Sample Rate = Gyroscope Output Rate / (1 + SMPLRT_DIV)
  Serial.println("[MPU6050] Sample rate: 100Hz");
  
  Serial.println("[MPU6050] ✅ Configuration complete");
}

void calibrateSensor() {
  Serial.println("\n[CALIBRATION] Starting sensor calibration...");
  Serial.println("[CALIBRATION] ⚠️ IMPORTANT: Place motorcycle in UPRIGHT position!");
  Serial.println("[CALIBRATION] Calibrating in 5 seconds...");
  
  for (int i = 5; i > 0; i--) {
    Serial.printf("[CALIBRATION] %d...\n", i);
    delay(1000);
  }
  
  Serial.println("[CALIBRATION] Calibrating... (10 seconds)");
  
  float rollSum = 0.0;
  float pitchSum = 0.0;
  int samples = 0;
  
  unsigned long calibrationStart = millis();
  while (millis() - calibrationStart < 10000) { // 10 seconds
    readMPU6050Data();
    
    // Calculate raw angles
    float rawRoll = atan2(ay, az) * 180.0 / PI;
    float rawPitch = atan2(-ax, sqrt(ay * ay + az * az)) * 180.0 / PI;
    
    rollSum += rawRoll;
    pitchSum += rawPitch;
    samples++;
    
    delay(50);
  }
  
  // Calculate offsets
  rollOffset = rollSum / samples;
  pitchOffset = pitchSum / samples;
  
  Serial.printf("[CALIBRATION] ✅ Complete! Samples: %d\n", samples);
  Serial.printf("[CALIBRATION] Roll offset: %.2f°\n", rollOffset);
  Serial.printf("[CALIBRATION] Pitch offset: %.2f°\n", pitchOffset);
  Serial.println("[CALIBRATION] Sensor is now calibrated for upright position = 0°\n");
}

void readMPU6050Data() {
  // Read raw accelerometer and gyroscope data
  mpu.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);
}

void calculateAngles() {
  // Convert raw accelerometer data to angles
  // Roll (rotation around X-axis) - this is the main lean angle for motorcycles
  roll = atan2(ay, az) * 180.0 / PI - rollOffset;
  
  // Pitch (rotation around Y-axis) - forward/backward tilt
  pitch = atan2(-ax, sqrt(ay * ay + az * az)) * 180.0 / PI - pitchOffset;
  
  // Calculate lean angle (absolute value of roll for motorcycle lean)
  leanAngle = abs(roll);
  
  // Track maximum and minimum lean angles
  if (leanAngle > maxLeanAngle) {
    maxLeanAngle = leanAngle;
  }
  
  if (readingCount == 0 || leanAngle < minLeanAngle) {
    minLeanAngle = leanAngle;
  }
}

void updateStatistics() {
  readingCount++;
  totalLeanAngle += leanAngle;
  averageLeanAngle = totalLeanAngle / readingCount;
}

// ═══════════════════════════════════════════════════════════════════════════
// DISPLAY FUNCTIONS
// ═══════════════════════════════════════════════════════════════════════════

void displayRealTimeData() {
  // Clear previous line and show current readings
  Serial.printf("\r[LIVE] Roll: %6.1f° | Pitch: %6.1f° | Lean: %6.1f° | Max: %6.1f° | Avg: %5.1f° | Count: %lu", 
                roll, pitch, leanAngle, maxLeanAngle, averageLeanAngle, readingCount);
  
  // Add lean angle classification
  String classification = classifyLeanAngle(leanAngle);
  Serial.print(" | " + classification);
}

String classifyLeanAngle(float angle) {
  if (angle < 5.0) {
    return "UPRIGHT ✅";
  } else if (angle < 15.0) {
    return "SLIGHT 📐";
  } else if (angle < 25.0) {
    return "NORMAL 🏍️";
  } else if (angle < 35.0) {
    return "AGGRESSIVE ⚠️";
  } else if (angle < 45.0) {
    return "EXTREME 🚨";
  } else {
    return "CRASH! 💥";
  }
}

void showPeriodicStats() {
  Serial.println("\n\n━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━");
  Serial.println("📊 LEAN ANGLE STATISTICS (Last 5 seconds)");
  Serial.println("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━");
  Serial.printf("📐 Current Lean Angle: %.1f° (%s)\n", leanAngle, classifyLeanAngle(leanAngle).c_str());
  Serial.printf("📈 Maximum Lean Angle: %.1f°\n", maxLeanAngle);
  Serial.printf("📉 Minimum Lean Angle: %.1f°\n", minLeanAngle);
  Serial.printf("📊 Average Lean Angle: %.1f°\n", averageLeanAngle);
  Serial.printf("🔢 Total Readings: %lu\n", readingCount);
  Serial.printf("⏱️ Uptime: %.1f minutes\n", millis() / 60000.0);
  
  // Crash detection analysis
  Serial.println("\n🚨 CRASH DETECTION ANALYSIS:");
  if (maxLeanAngle < 25.0) {
    Serial.println("✅ SAFE: Normal riding angles detected");
    Serial.println("💡 Suggested threshold: 35-40° for crash detection");
  } else if (maxLeanAngle < 35.0) {
    Serial.println("⚠️ MODERATE: Aggressive cornering detected");
    Serial.println("💡 Suggested threshold: 40-45° for crash detection");
  } else if (maxLeanAngle < 45.0) {
    Serial.println("🚨 HIGH: Extreme lean angles detected");
    Serial.println("💡 Suggested threshold: 45-50° for crash detection");
  } else {
    Serial.println("💥 CRITICAL: Crash-level angles detected!");
    Serial.println("💡 Current angle exceeds safe riding limits");
  }
  
  Serial.println("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n");
}

void showDetailedStats() {
  Serial.println("\n━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━");
  Serial.println("📊 DETAILED MPU6050 LEAN ANGLE STATISTICS");
  Serial.println("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━");
  
  // Current readings
  Serial.println("🔬 CURRENT SENSOR READINGS:");
  Serial.printf("   Raw Accelerometer: X=%d, Y=%d, Z=%d\n", ax, ay, az);
  Serial.printf("   Raw Gyroscope: X=%d, Y=%d, Z=%d\n", gx, gy, gz);
  Serial.printf("   Roll (lean): %.2f°\n", roll);
  Serial.printf("   Pitch (forward/back): %.2f°\n", pitch);
  Serial.printf("   Lean Angle (abs): %.2f°\n", leanAngle);
  
  // Calibration info
  Serial.println("\n⚖️ CALIBRATION DATA:");
  Serial.printf("   Roll Offset: %.2f°\n", rollOffset);
  Serial.printf("   Pitch Offset: %.2f°\n", pitchOffset);
  
  // Statistics
  Serial.println("\n📈 STATISTICS:");
  Serial.printf("   Total Readings: %lu\n", readingCount);
  Serial.printf("   Average Lean: %.2f°\n", averageLeanAngle);
  Serial.printf("   Maximum Lean: %.2f°\n", maxLeanAngle);
  Serial.printf("   Minimum Lean: %.2f°\n", minLeanAngle);
  Serial.printf("   Reading Rate: %.1f Hz\n", 1000.0 / READING_INTERVAL);
  
  // Threshold recommendations
  Serial.println("\n💡 CRASH DETECTION THRESHOLD RECOMMENDATIONS:");
  Serial.println("   🏍️ Conservative (street riding): 35°");
  Serial.println("   🏁 Moderate (sport riding): 40°");
  Serial.println("   🏆 Aggressive (track/racing): 45°");
  Serial.println("   🚨 Emergency (crash detection): 50°");
  
  // Current classification
  Serial.printf("\n🎯 CURRENT CLASSIFICATION: %s\n", classifyLeanAngle(leanAngle).c_str());
  
  Serial.println("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n");
}

// ═══════════════════════════════════════════════════════════════════════════
// SERIAL COMMANDS
// ═══════════════════════════════════════════════════════════════════════════

void handleSerialCommands() {
  if (Serial.available()) {
    String cmd = Serial.readStringUntil('\n');
    cmd.trim();
    cmd.toUpperCase();
    
    if (cmd == "CALIBRATE") {
      calibrateSensor();
    }
    else if (cmd == "RESET") {
      resetStatistics();
    }
    else if (cmd == "STATS") {
      showDetailedStats();
    }
    else if (cmd == "THRESHOLD") {
      testThresholds();
    }
    else if (cmd == "HELP") {
      showHelp();
    }
    else if (cmd.startsWith("SET ")) {
      // Allow setting custom thresholds for testing
      float threshold = cmd.substring(4).toFloat();
      if (threshold > 0 && threshold < 90) {
        testCustomThreshold(threshold);
      } else {
        Serial.println("❌ Invalid threshold. Use: SET 40 (for 40° threshold)");
      }
    }
  }
}

void resetStatistics() {
  readingCount = 0;
  totalLeanAngle = 0.0;
  averageLeanAngle = 0.0;
  maxLeanAngle = 0.0;
  minLeanAngle = 0.0;
  
  Serial.println("\n🔄 Statistics reset!");
  Serial.println("📊 Starting fresh data collection...\n");
}

void testThresholds() {
  Serial.println("\n🧪 THRESHOLD TESTING MODE");
  Serial.println("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━");
  Serial.println("🎯 Test different lean angles and see crash detection triggers:");
  Serial.println("");
  
  float testThresholds[] = {30.0, 35.0, 40.0, 45.0, 50.0};
  int numThresholds = sizeof(testThresholds) / sizeof(testThresholds[0]);
  
  for (int i = 0; i < numThresholds; i++) {
    float threshold = testThresholds[i];
    String status = (leanAngle >= threshold) ? "🚨 TRIGGERED" : "✅ OK";
    Serial.printf("   %.0f° threshold: %s (current: %.1f°)\n", threshold, status.c_str(), leanAngle);
  }
  
  Serial.println("");
  Serial.println("💡 Use 'SET 40' to test a custom 40° threshold");
  Serial.println("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n");
}

void testCustomThreshold(float threshold) {
  Serial.printf("\n🎯 TESTING CUSTOM THRESHOLD: %.1f°\n", threshold);
  Serial.println("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━");
  
  // Monitor for 10 seconds
  unsigned long testStart = millis();
  int triggerCount = 0;
  
  Serial.printf("⏱️ Monitoring for 10 seconds with %.1f° threshold...\n", threshold);
  
  while (millis() - testStart < 10000) {
    readMPU6050Data();
    calculateAngles();
    
    if (leanAngle >= threshold) {
      triggerCount++;
      Serial.printf("🚨 CRASH TRIGGER! Lean angle: %.1f° (>= %.1f°) - Count: %d\n", 
                    leanAngle, threshold, triggerCount);
      delay(500); // Prevent spam
    }
    
    delay(100);
  }
  
  Serial.printf("📊 Test complete! Triggers in 10 seconds: %d\n", triggerCount);
  
  if (triggerCount == 0) {
    Serial.printf("✅ Threshold %.1f° is SAFE for current riding style\n", threshold);
  } else if (triggerCount < 5) {
    Serial.printf("⚠️ Threshold %.1f° triggered %d times - may be too sensitive\n", threshold, triggerCount);
  } else {
    Serial.printf("🚨 Threshold %.1f° triggered %d times - too sensitive for normal riding\n", threshold, triggerCount);
  }
  
  Serial.println("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n");
}

void showHelp() {
  Serial.println("\n📋 MPU6050 LEAN ANGLE TEST - COMMAND HELP");
  Serial.println("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━");
  Serial.println("🔧 COMMANDS:");
  Serial.println("   CALIBRATE  - Recalibrate sensor (place motorcycle upright first!)");
  Serial.println("   RESET      - Reset all statistics and start fresh");
  Serial.println("   STATS      - Show detailed sensor and statistics information");
  Serial.println("   THRESHOLD  - Test common crash detection thresholds");
  Serial.println("   SET 40     - Test custom threshold (example: 40 degrees)");
  Serial.println("   HELP       - Show this help information");
  Serial.println("");
  Serial.println("🏍️ TESTING PROCEDURE:");
  Serial.println("   1. Start with motorcycle upright and run CALIBRATE");
  Serial.println("   2. Gradually lean motorcycle to different angles");
  Serial.println("   3. Note the lean angle readings for each position");
  Serial.println("   4. Use THRESHOLD to test different crash detection limits");
  Serial.println("   5. Choose appropriate threshold based on your riding style");
  Serial.println("");
  Serial.println("📐 ANGLE CLASSIFICATIONS:");
  Serial.println("   0-5°:   UPRIGHT ✅ (normal riding)");
  Serial.println("   5-15°:  SLIGHT 📐 (gentle turns)");
  Serial.println("   15-25°: NORMAL 🏍️ (normal cornering)");
  Serial.println("   25-35°: AGGRESSIVE ⚠️ (sport riding)");
  Serial.println("   35-45°: EXTREME 🚨 (racing lean)");
  Serial.println("   45°+:   CRASH! 💥 (crash/fall detection)");
  Serial.println("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n");
}