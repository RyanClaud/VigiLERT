// ═══════════════════════════════════════════════════════════════════════════
// MPU6050 DIAGNOSTIC TOOL
// ═══════════════════════════════════════════════════════════════════════════
// 🔍 PURPOSE: Diagnose MPU6050 connection and configuration issues
// 🛠️ FEATURES: I2C scanner, register dump, connection test, calibration check
// 🚨 USE WHEN: MPU6050 not detected or giving incorrect readings
// 📋 HELPS WITH: Wiring issues, I2C problems, sensor configuration
// ═══════════════════════════════════════════════════════════════════════════

#include <Wire.h>

// I2C Configuration
const int SDA_PIN = 21;
const int SCL_PIN = 22;
const int MPU6050_ADDRESS = 0x68;

// MPU6050 Register Addresses
const int MPU6050_WHO_AM_I = 0x75;
const int MPU6050_PWR_MGMT_1 = 0x6B;
const int MPU6050_GYRO_CONFIG = 0x1B;
const int MPU6050_ACCEL_CONFIG = 0x1C;
const int MPU6050_CONFIG = 0x1A;
const int MPU6050_SMPRT_DIV = 0x19;
const int MPU6050_ACCEL_XOUT_H = 0x3B;

void setup() {
  Serial.begin(115200);
  delay(1000);
  
  Serial.println("\n╔════════════════════════════════════════════════════════════╗");
  Serial.println("║                MPU6050 DIAGNOSTIC TOOL                    ║");
  Serial.println("╠════════════════════════════════════════════════════════════╣");
  Serial.println("║ 🔍 Diagnose MPU6050 connection and configuration issues   ║");
  Serial.println("║ 🛠️ I2C scanner, register dump, connection test            ║");
  Serial.println("║ 🚨 Use when MPU6050 not detected or incorrect readings    ║");
  Serial.println("╚════════════════════════════════════════════════════════════╝\n");

  // Initialize I2C
  Wire.begin(SDA_PIN, SCL_PIN);
  Serial.printf("[I2C] Initialized - SDA: GPIO%d, SCL: GPIO%d\n", SDA_PIN, SCL_PIN);
  
  // Set I2C clock speed
  Wire.setClock(100000); // 100kHz (standard mode)
  Serial.println("[I2C] Clock speed: 100kHz (standard mode)");
  
  Serial.println("\n🚀 MPU6050 DIAGNOSTIC TOOL READY!");
  Serial.println("\n📋 COMMANDS:");
  Serial.println("  SCAN       - Scan I2C bus for devices");
  Serial.println("  TEST       - Test MPU6050 connection");
  Serial.println("  REGISTERS  - Dump MPU6050 registers");
  Serial.println("  CALIBRATE  - Test calibration procedure");
  Serial.println("  READINGS   - Show raw sensor readings");
  Serial.println("  WIRING     - Show wiring diagram");
  Serial.println("  HELP       - Show this help");
  Serial.println("\n🔧 Starting automatic diagnostics...\n");
  
  // Run automatic diagnostics
  runAutomaticDiagnostics();
}

void loop() {
  handleSerialCommands();
  delay(100);
}

void runAutomaticDiagnostics() {
  Serial.println("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━");
  Serial.println("🔍 AUTOMATIC MPU6050 DIAGNOSTICS");
  Serial.println("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━");
  
  // Step 1: I2C Bus Scan
  Serial.println("\n🔍 STEP 1: I2C BUS SCAN");
  scanI2CBus();
  
  // Step 2: MPU6050 Connection Test
  Serial.println("\n🔍 STEP 2: MPU6050 CONNECTION TEST");
  testMPU6050Connection();
  
  // Step 3: Register Analysis
  Serial.println("\n🔍 STEP 3: REGISTER ANALYSIS");
  analyzeRegisters();
  
  // Step 4: Sensor Reading Test
  Serial.println("\n🔍 STEP 4: SENSOR READING TEST");
  testSensorReadings();
  
  Serial.println("\n━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━");
  Serial.println("✅ AUTOMATIC DIAGNOSTICS COMPLETE");
  Serial.println("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n");
}

void scanI2CBus() {
  Serial.println("Scanning I2C bus for devices...");
  
  int deviceCount = 0;
  
  for (int address = 1; address < 127; address++) {
    Wire.beginTransmission(address);
    int error = Wire.endTransmission();
    
    if (error == 0) {
      deviceCount++;
      Serial.printf("✅ Device found at address 0x%02X", address);
      
      // Identify known devices
      if (address == 0x68) {
        Serial.print(" (MPU6050 - Primary Address)");
      } else if (address == 0x69) {
        Serial.print(" (MPU6050 - Secondary Address)");
      } else if (address == 0x77) {
        Serial.print(" (BMP180/BMP280)");
      } else if (address == 0x76) {
        Serial.print(" (BMP280)");
      }
      
      Serial.println();
    } else if (error == 4) {
      Serial.printf("❌ Unknown error at address 0x%02X\n", address);
    }
  }
  
  Serial.printf("\n📊 I2C Scan Results: %d device(s) found\n", deviceCount);
  
  if (deviceCount == 0) {
    Serial.println("⚠️ NO I2C DEVICES FOUND!");
    Serial.println("   Check wiring connections:");
    Serial.println("   - VCC → 3.3V (NOT 5V!)");
    Serial.println("   - GND → GND");
    Serial.println("   - SDA → GPIO 21");
    Serial.println("   - SCL → GPIO 22");
  }
}

void testMPU6050Connection() {
  Serial.println("Testing MPU6050 connection...");
  
  // Test basic I2C communication
  Wire.beginTransmission(MPU6050_ADDRESS);
  int error = Wire.endTransmission();
  
  if (error != 0) {
    Serial.printf("❌ MPU6050 not responding (Error: %d)\n", error);
    Serial.println("   Possible issues:");
    Serial.println("   - Incorrect wiring");
    Serial.println("   - Wrong I2C address (try 0x69 if AD0 is HIGH)");
    Serial.println("   - Power supply issues");
    Serial.println("   - Damaged sensor");
    return;
  }
  
  Serial.println("✅ MPU6050 responding to I2C communication");
  
  // Read WHO_AM_I register
  uint8_t whoAmI = readRegister(MPU6050_WHO_AM_I);
  Serial.printf("📋 WHO_AM_I Register: 0x%02X", whoAmI);
  
  if (whoAmI == 0x68) {
    Serial.println(" ✅ (Correct - MPU6050 detected)");
  } else if (whoAmI == 0x00 || whoAmI == 0xFF) {
    Serial.println(" ❌ (Communication error - check wiring)");
  } else {
    Serial.printf(" ⚠️ (Unexpected value - may be different sensor)\n");
  }
  
  // Test wake up
  Serial.println("🔄 Attempting to wake up MPU6050...");
  writeRegister(MPU6050_PWR_MGMT_1, 0x00); // Wake up
  delay(100);
  
  uint8_t powerMgmt = readRegister(MPU6050_PWR_MGMT_1);
  Serial.printf("📋 Power Management: 0x%02X", powerMgmt);
  
  if (powerMgmt == 0x00) {
    Serial.println(" ✅ (Awake and ready)");
  } else if (powerMgmt & 0x40) {
    Serial.println(" ⚠️ (Still in sleep mode)");
  } else {
    Serial.printf(" ⚠️ (Unexpected power state)\n");
  }
}

void analyzeRegisters() {
  Serial.println("Analyzing MPU6050 register configuration...");
  
  struct RegisterInfo {
    int address;
    const char* name;
    const char* description;
  };
  
  RegisterInfo registers[] = {
    {MPU6050_PWR_MGMT_1, "PWR_MGMT_1", "Power Management 1"},
    {MPU6050_GYRO_CONFIG, "GYRO_CONFIG", "Gyroscope Configuration"},
    {MPU6050_ACCEL_CONFIG, "ACCEL_CONFIG", "Accelerometer Configuration"},
    {MPU6050_CONFIG, "CONFIG", "Configuration (DLPF)"},
    {MPU6050_SMPRT_DIV, "SMPRT_DIV", "Sample Rate Divider"}
  };
  
  for (int i = 0; i < 5; i++) {
    uint8_t value = readRegister(registers[i].address);
    Serial.printf("📋 %s (0x%02X): 0x%02X - %s\n", 
                  registers[i].name, 
                  registers[i].address, 
                  value, 
                  registers[i].description);
    
    // Analyze specific registers
    if (registers[i].address == MPU6050_GYRO_CONFIG) {
      int gyroRange = (value >> 3) & 0x03;
      const char* ranges[] = {"±250°/s", "±500°/s", "±1000°/s", "±2000°/s"};
      Serial.printf("   Gyro Range: %s\n", ranges[gyroRange]);
    }
    
    if (registers[i].address == MPU6050_ACCEL_CONFIG) {
      int accelRange = (value >> 3) & 0x03;
      const char* ranges[] = {"±2g", "±4g", "±8g", "±16g"};
      Serial.printf("   Accel Range: %s\n", ranges[accelRange]);
    }
    
    if (registers[i].address == MPU6050_CONFIG) {
      int dlpf = value & 0x07;
      Serial.printf("   DLPF Setting: %d\n", dlpf);
    }
  }
}

void testSensorReadings() {
  Serial.println("Testing sensor readings (10 samples)...");
  
  // Wake up MPU6050
  writeRegister(MPU6050_PWR_MGMT_1, 0x00);
  delay(100);
  
  Serial.println("Sample | AccelX | AccelY | AccelZ | GyroX | GyroY | GyroZ | Roll  | Pitch");
  Serial.println("-------|--------|--------|--------|-------|-------|-------|-------|-------");
  
  for (int i = 0; i < 10; i++) {
    // Read raw sensor data
    Wire.beginTransmission(MPU6050_ADDRESS);
    Wire.write(MPU6050_ACCEL_XOUT_H);
    Wire.endTransmission(false);
    Wire.requestFrom(MPU6050_ADDRESS, 14, true);
    
    int16_t ax = Wire.read() << 8 | Wire.read();
    int16_t ay = Wire.read() << 8 | Wire.read();
    int16_t az = Wire.read() << 8 | Wire.read();
    int16_t temp = Wire.read() << 8 | Wire.read(); // Temperature (not used)
    int16_t gx = Wire.read() << 8 | Wire.read();
    int16_t gy = Wire.read() << 8 | Wire.read();
    int16_t gz = Wire.read() << 8 | Wire.read();
    
    // Calculate angles
    float roll = atan2(ay, az) * 180.0 / PI;
    float pitch = atan2(-ax, sqrt(ay * ay + az * az)) * 180.0 / PI;
    
    Serial.printf("  %2d   | %6d | %6d | %6d | %5d | %5d | %5d | %5.1f | %5.1f\n",
                  i + 1, ax, ay, az, gx, gy, gz, roll, pitch);
    
    delay(100);
  }
  
  Serial.println("\n📊 Sensor Reading Analysis:");
  Serial.println("✅ If readings change when moving sensor: Sensor is working");
  Serial.println("❌ If all readings are 0 or -1: Communication problem");
  Serial.println("❌ If readings don't change: Sensor may be damaged");
  Serial.println("⚠️ If readings are noisy: Check power supply and connections");
}

uint8_t readRegister(int registerAddress) {
  Wire.beginTransmission(MPU6050_ADDRESS);
  Wire.write(registerAddress);
  Wire.endTransmission(false);
  Wire.requestFrom(MPU6050_ADDRESS, 1, true);
  return Wire.read();
}

void writeRegister(int registerAddress, uint8_t value) {
  Wire.beginTransmission(MPU6050_ADDRESS);
  Wire.write(registerAddress);
  Wire.write(value);
  Wire.endTransmission(true);
}

void handleSerialCommands() {
  if (Serial.available()) {
    String cmd = Serial.readStringUntil('\n');
    cmd.trim();
    cmd.toUpperCase();
    
    if (cmd == "SCAN") {
      Serial.println("\n🔍 I2C BUS SCAN");
      scanI2CBus();
    }
    else if (cmd == "TEST") {
      Serial.println("\n🔍 MPU6050 CONNECTION TEST");
      testMPU6050Connection();
    }
    else if (cmd == "REGISTERS") {
      Serial.println("\n🔍 MPU6050 REGISTER DUMP");
      dumpAllRegisters();
    }
    else if (cmd == "CALIBRATE") {
      Serial.println("\n🔍 CALIBRATION TEST");
      testCalibration();
    }
    else if (cmd == "READINGS") {
      Serial.println("\n🔍 CONTINUOUS SENSOR READINGS");
      continuousReadings();
    }
    else if (cmd == "WIRING") {
      showWiringDiagram();
    }
    else if (cmd == "HELP") {
      showHelp();
    }
    else if (cmd == "AUTO") {
      runAutomaticDiagnostics();
    }
  }
}

void dumpAllRegisters() {
  Serial.println("Complete MPU6050 register dump:");
  Serial.println("Addr | Value | Binary   | Description");
  Serial.println("-----|-------|----------|---------------------------");
  
  for (int addr = 0x00; addr <= 0x75; addr++) {
    uint8_t value = readRegister(addr);
    Serial.printf("0x%02X | 0x%02X  | ", addr, value);
    
    // Print binary
    for (int bit = 7; bit >= 0; bit--) {
      Serial.print((value >> bit) & 1);
    }
    
    // Add description for important registers
    Serial.print(" | ");
    switch (addr) {
      case 0x19: Serial.print("Sample Rate Divider"); break;
      case 0x1A: Serial.print("Configuration (DLPF)"); break;
      case 0x1B: Serial.print("Gyroscope Configuration"); break;
      case 0x1C: Serial.print("Accelerometer Configuration"); break;
      case 0x3B: Serial.print("Accel X High Byte"); break;
      case 0x6B: Serial.print("Power Management 1"); break;
      case 0x75: Serial.print("WHO_AM_I"); break;
      default: Serial.print(""); break;
    }
    Serial.println();
  }
}

void testCalibration() {
  Serial.println("Testing calibration procedure...");
  Serial.println("⚠️ Ensure sensor is level and stationary!");
  Serial.println("Starting calibration in 3 seconds...");
  
  for (int i = 3; i > 0; i--) {
    Serial.printf("%d...\n", i);
    delay(1000);
  }
  
  Serial.println("Calibrating... (5 seconds)");
  
  // Wake up MPU6050
  writeRegister(MPU6050_PWR_MGMT_1, 0x00);
  delay(100);
  
  float rollSum = 0.0;
  float pitchSum = 0.0;
  int samples = 0;
  
  unsigned long startTime = millis();
  while (millis() - startTime < 5000) {
    // Read accelerometer data
    Wire.beginTransmission(MPU6050_ADDRESS);
    Wire.write(MPU6050_ACCEL_XOUT_H);
    Wire.endTransmission(false);
    Wire.requestFrom(MPU6050_ADDRESS, 6, true);
    
    int16_t ax = Wire.read() << 8 | Wire.read();
    int16_t ay = Wire.read() << 8 | Wire.read();
    int16_t az = Wire.read() << 8 | Wire.read();
    
    // Calculate angles
    float roll = atan2(ay, az) * 180.0 / PI;
    float pitch = atan2(-ax, sqrt(ay * ay + az * az)) * 180.0 / PI;
    
    rollSum += roll;
    pitchSum += pitch;
    samples++;
    
    delay(50);
  }
  
  float rollOffset = rollSum / samples;
  float pitchOffset = pitchSum / samples;
  
  Serial.printf("✅ Calibration complete! Samples: %d\n", samples);
  Serial.printf("📐 Roll offset: %.2f°\n", rollOffset);
  Serial.printf("📐 Pitch offset: %.2f°\n", pitchOffset);
  
  if (abs(rollOffset) > 10 || abs(pitchOffset) > 10) {
    Serial.println("⚠️ Large offsets detected - check sensor mounting!");
  } else {
    Serial.println("✅ Offsets look reasonable");
  }
}

void continuousReadings() {
  Serial.println("Continuous sensor readings (press any key to stop)...");
  Serial.println("Time | Roll  | Pitch | Lean  | AccelX | AccelY | AccelZ");
  Serial.println("-----|-------|-------|-------|--------|--------|--------");
  
  // Wake up MPU6050
  writeRegister(MPU6050_PWR_MGMT_1, 0x00);
  delay(100);
  
  unsigned long startTime = millis();
  
  while (!Serial.available()) {
    // Read accelerometer data
    Wire.beginTransmission(MPU6050_ADDRESS);
    Wire.write(MPU6050_ACCEL_XOUT_H);
    Wire.endTransmission(false);
    Wire.requestFrom(MPU6050_ADDRESS, 6, true);
    
    int16_t ax = Wire.read() << 8 | Wire.read();
    int16_t ay = Wire.read() << 8 | Wire.read();
    int16_t az = Wire.read() << 8 | Wire.read();
    
    // Calculate angles
    float roll = atan2(ay, az) * 180.0 / PI;
    float pitch = atan2(-ax, sqrt(ay * ay + az * az)) * 180.0 / PI;
    float lean = abs(roll);
    
    unsigned long elapsed = (millis() - startTime) / 1000;
    
    Serial.printf("%4lu | %5.1f | %5.1f | %5.1f | %6d | %6d | %6d\n",
                  elapsed, roll, pitch, lean, ax, ay, az);
    
    delay(200);
  }
  
  // Clear serial buffer
  while (Serial.available()) {
    Serial.read();
  }
  
  Serial.println("Continuous readings stopped.");
}

void showWiringDiagram() {
  Serial.println("\n🔌 MPU6050 WIRING DIAGRAM");
  Serial.println("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━");
  Serial.println("");
  Serial.println("   MPU6050 Module      ESP32 Pin       Description");
  Serial.println("   ──────────────      ─────────       ───────────");
  Serial.println("   VCC                 3.3V            Power Supply (NOT 5V!)");
  Serial.println("   GND                 GND             Ground");
  Serial.println("   SDA                 GPIO 21         I2C Data Line");
  Serial.println("   SCL                 GPIO 22         I2C Clock Line");
  Serial.println("   XDA                 Not Connected   External I2C (optional)");
  Serial.println("   XCL                 Not Connected   External I2C (optional)");
  Serial.println("   AD0                 GND             I2C Address (0x68)");
  Serial.println("   INT                 Not Connected   Interrupt (optional)");
  Serial.println("");
  Serial.println("⚠️ CRITICAL NOTES:");
  Serial.println("   • Use 3.3V power supply, NOT 5V (can damage sensor)");
  Serial.println("   • Secure all connections (loose wires cause issues)");
  Serial.println("   • Keep wires short (long wires cause I2C problems)");
  Serial.println("   • AD0 to GND sets I2C address to 0x68");
  Serial.println("   • If AD0 is HIGH, I2C address becomes 0x69");
  Serial.println("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━");
}

void showHelp() {
  Serial.println("\n📋 MPU6050 DIAGNOSTIC TOOL - COMMAND HELP");
  Serial.println("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━");
  Serial.println("🔧 DIAGNOSTIC COMMANDS:");
  Serial.println("   SCAN       - Scan I2C bus for all connected devices");
  Serial.println("   TEST       - Test MPU6050 connection and basic communication");
  Serial.println("   REGISTERS  - Dump all MPU6050 registers with descriptions");
  Serial.println("   CALIBRATE  - Test calibration procedure (sensor must be level)");
  Serial.println("   READINGS   - Show continuous sensor readings (press key to stop)");
  Serial.println("   WIRING     - Display wiring diagram and connection info");
  Serial.println("   AUTO       - Run complete automatic diagnostics");
  Serial.println("   HELP       - Show this help information");
  Serial.println("");
  Serial.println("🔍 TROUBLESHOOTING WORKFLOW:");
  Serial.println("   1. Run AUTO for complete diagnostics");
  Serial.println("   2. If no devices found, check wiring with WIRING command");
  Serial.println("   3. If MPU6050 detected but not working, use REGISTERS");
  Serial.println("   4. Test real-time data with READINGS command");
  Serial.println("   5. Verify calibration accuracy with CALIBRATE");
  Serial.println("");
  Serial.println("⚠️ COMMON ISSUES:");
  Serial.println("   • No devices found: Check wiring, especially power (3.3V not 5V)");
  Serial.println("   • Wrong WHO_AM_I: May be different sensor or communication error");
  Serial.println("   • All readings zero: Power or communication problem");
  Serial.println("   • Noisy readings: Loose connections or power supply issues");
  Serial.println("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━");
}