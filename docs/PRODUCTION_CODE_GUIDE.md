# VigiLERT Motorcycle Module - Production Code Guide

## 🎉 Final Production Version

**File:** `MotorcycleToWebApplication_PRODUCTION.ino`

This is the complete, production-ready code with all features working properly, including the relay control fix.

---

## ✅ What's Working

### 1. **Relay Control** ✅
- **Hardware Fix Applied:** External 5V power supply required
- **Relay Type:** ACTIVE-LOW (HIGH = OFF, LOW = ON)
- **GPIO:** 13
- **Status:** Fully functional

### 2. **Crash Detection** ✅
- MPU6050 accelerometer and gyroscope
- Threshold: 15g or 40° roll angle
- Immediate engine shutdown on crash
- 30-second auto-clear or manual CLEAR command
- Firebase crash event logging with GPS coordinates

### 3. **Helmet Module Detection** ✅
- 64-bit timestamp synchronization
- 10-second heartbeat timeout
- Real-time status monitoring
- Proper pairing detection

### 4. **Alcohol Safety** ✅
- Firebase alcohol status integration
- Engine start blocked if alcohol detected
- Real-time safety checks

### 5. **GPS Tracking** ✅
- TinyGPS++ library
- Real-time location tracking
- Speed monitoring
- Trip start/end coordinates

### 6. **GSM Communication** ✅
- SMS commands (START, STOP, STATUS)
- Password protection
- Authorized number verification
- Alert notifications

### 7. **Firebase Integration** ✅
- Real-time live data updates
- Crash event logging
- Trip management
- Device heartbeat system
- Speed limit synchronization

### 8. **Safety Interlock System** ✅
- Multi-condition engine start:
  - Helmet module active
  - No alcohol detected
  - WiFi connected
  - No crash state
- Visual indicator (LED blinking when not ready)

---

## 🔧 Hardware Requirements

### **Critical: Relay Power Supply**
```
⚠️  ESP32 3.3V output is INSUFFICIENT for relay module!
✅ SOLUTION: Use external 5V power supply

Wiring:
- External 5V → Relay VCC
- ESP32 GND → Relay GND (common ground!)
- ESP32 GPIO 13 → Relay IN
```

### **Pin Assignments**
```cpp
Battery:          GPIO 34 (ADC)
Headlight:        GPIO 32 (Input)
Taillight:        GPIO 33 (Input)
Left Signal:      GPIO 32 (Input)
Right Signal:     GPIO 33 (Input)
Relay:            GPIO 13 (Output) ⚠️ REQUIRES EXTERNAL 5V!
Vibration:        GPIO 14 (Input)
Buzzer:           GPIO 12 (Output)
Light Indicator:  GPIO 2 (Output)

GSM:
- RX: GPIO 4
- TX: GPIO 5

GPS:
- RX: GPIO 16
- TX: GPIO 17

MPU6050:
- SDA: GPIO 21
- SCL: GPIO 22
```

---

## 📋 Serial Commands

### **Available Commands:**
```
CRASH      - Test crash detection
START      - Start engine (if safety allows)
STOP       - Stop engine
CLEAR      - Clear crash state manually
STATUS     - Show complete system status
MPU        - Test MPU6050 sensor
GPS        - Show GPS diagnostics
HELP       - Show all commands
```

### **Command Examples:**

#### Test Crash Detection
```
> CRASH
🧪 MANUAL CRASH TEST TRIGGERED!
Relay GPIO 13 = 1 (HIGH/OFF)
✓ Test crash complete! Type 'CLEAR' to reset.
```

#### Start Engine
```
> START
✅ Engine started!
Relay GPIO 13 = 0 (LOW/ON)
```

#### Clear Crash State
```
> CLEAR
✅ Crash state manually cleared
✅ System ready for normal operation
✅ Type 'START' to restart engine
```

#### Check Status
```
> STATUS
╔═══════════════════════════════════════════════════════════╗
║                     SYSTEM STATUS                         ║
╠═══════════════════════════════════════════════════════════╣
║ WiFi:    ✓ Connected                                      ║
║ Helmet:  ✓ Active                                         ║
║ Alcohol: ✓ Safe                                           ║
║ Engine:  RUNNING                                          ║
║ Crash:   ✓ None                                           ║
╠═══════════════════════════════════════════════════════════╣
║ Accel:   9.81 g                                           ║
║ Roll:    0.5°                                             ║
║ Relay:   ON (LOW)                                         ║
╠═══════════════════════════════════════════════════════════╣
║ GPS:     14.123456, 121.123456                            ║
║ Speed:   45.2 km/h                                        ║
║ Sats:    8                                                ║
╠═══════════════════════════════════════════════════════════╣
║ Can Start Engine: ✓ YES                                   ║
╚═══════════════════════════════════════════════════════════╝
```

---

## 🚀 How to Use

### **1. Upload the Code**
1. Open `MotorcycleToWebApplication_PRODUCTION.ino` in Arduino IDE
2. Select board: **ESP32 Dev Module**
3. Select port: Your ESP32 COM port
4. Click **Upload**

### **2. Hardware Setup**
1. **Connect external 5V power to relay module**
2. Ensure common ground between ESP32 and relay
3. Connect all sensors (MPU6050, GPS, GSM)
4. Verify all pin connections

### **3. Initial Testing**
1. Open Serial Monitor (115200 baud)
2. Wait for WiFi connection
3. Type `STATUS` to check all systems
4. Type `MPU` to verify accelerometer
5. Type `GPS` to check GPS status

### **4. Relay Testing**
```
1. Type: START
   → LED should turn ON
   → Relay should click

2. Type: STOP
   → LED should turn OFF
   → Relay should click

3. Type: CRASH
   → LED should turn OFF immediately
   → Buzzer should sound

4. Type: CLEAR
   → Crash state cleared
   → Ready for normal operation
```

### **5. Safety System Testing**
1. **Helmet Detection:**
   - Turn on helmet module
   - Wait 3 seconds
   - Check STATUS - should show "Helmet: ✓ Active"

2. **Alcohol Detection:**
   - Simulate alcohol detection in Firebase
   - Try to START engine
   - Should be blocked with message

3. **Crash Detection:**
   - Tilt MPU6050 > 40°
   - Engine should shut down immediately
   - Crash event sent to Firebase

---

## 🔍 Troubleshooting

### **Relay Not Responding**
```
Problem: Relay doesn't turn ON/OFF
Solution: 
1. Check external 5V power supply
2. Verify common ground
3. Test with different GPIO pin
4. Check relay module type (active-low vs active-high)
```

### **Helmet Detection Not Working**
```
Problem: Helmet shows as inactive
Solution:
1. Check helmet module is powered on
2. Verify Firebase heartbeat timestamps
3. Check WiFi connection
4. Verify 64-bit timestamp synchronization
```

### **GPS No Fix**
```
Problem: GPS shows "No fix"
Solution:
1. Move to open area with clear sky view
2. Wait 30-60 seconds for satellite lock
3. Check GPS antenna connection
4. Verify RX=16, TX=17 wiring
```

### **Crash Detection Too Sensitive**
```
Problem: False crash detections
Solution:
1. Increase ACCEL_THRESHOLD (default: 15.0g)
2. Adjust roll angle threshold (default: 40°)
3. Check MPU6050 mounting (should be stable)
```

---

## 📊 System Architecture

### **Priority Loop Structure:**
```
1. WiFi Stability Check (every 5s)
2. MPU6050 Reading (continuous)
3. Motorcycle Heartbeat (every 2s)
4. Helmet Detection (every 3s)
5. Safety Interlock (continuous)
6. Crash Detection (continuous)
7. Serial Commands (continuous)
8. GPS Reading (continuous)
9. GSM Handling (continuous)
10. Sensor Readings (continuous)
11. Firebase Live Updates (every 2s)
12. Status Display (every 5s)
13. Periodic Tasks (various intervals)
```

### **Safety Interlock Logic:**
```
Can Start Engine = 
  Helmet Active AND
  Alcohol Safe AND
  WiFi Connected AND
  No Crash State
```

### **Crash Detection Logic:**
```
Crash Detected IF:
  (Total Acceleration >= 15g OR Roll Angle > 40°) AND
  Not Already in Crash State AND
  5 seconds since last crash

Actions:
1. Set relay HIGH (OFF)
2. Set engineRunning = false
3. Send crash event to Firebase
4. Trigger buzzer alert
5. Set crashDetected = true

Auto-Clear:
  After 30 seconds IF bike is stable
  (Roll < 20° AND Accel < 11g)
```

---

## 🔐 Security Features

### **GSM SMS Commands**
- Password protected: `1234`
- Authorized numbers only
- Commands: START, STOP, STATUS

### **Unauthorized Movement Detection**
- Vibration sensor monitoring
- Alert SMS sent to authorized numbers
- Buzzer alarm triggered

---

## 📈 Firebase Data Structure

### **Live Data** (`/helmet_public/{userUID}/live`)
```json
{
  "locationLat": 14.123456,
  "locationLng": 121.123456,
  "speed": 45.2,
  "batteryVoltage": 12.6,
  "headlight": true,
  "taillight": true,
  "leftSignal": false,
  "rightSignal": false,
  "mpu6050": {
    "accelX": 0.1,
    "accelY": 0.2,
    "accelZ": 9.8,
    "gyro": 0.5,
    "totalAccel": 9.81
  },
  "gps": {
    "accuracy": "2.5m",
    "satellites": 8
  },
  "gsm": {
    "signal": "75%",
    "network": "4G"
  },
  "engineRunning": true,
  "helmetModuleActive": true,
  "timestamp": 1234567890,
  "source": "gps_module"
}
```

### **Crash Events** (`/helmet_public/{userUID}/crashes`)
```json
{
  "timestamp": 1234567890,
  "locationLat": 14.123456,
  "locationLng": 121.123456,
  "impactStrength": 18.5,
  "roll": 45.2,
  "type": "crash",
  "hasGPS": true,
  "severity": "moderate"
}
```

### **Device Status** (`/helmet_public/{userUID}/devices/motorcycle`)
```json
{
  "status": "On",
  "lastHeartbeat": 1700000000000,
  "timestamp": 1700000000000
}
```

---

## 🎯 Key Improvements from Previous Versions

### **1. Relay Control**
- ✅ External 5V power requirement documented
- ✅ ACTIVE-LOW logic properly implemented
- ✅ No LED conflicts on relay pin
- ✅ Immediate crash shutdown working

### **2. Crash Detection**
- ✅ 30-second auto-clear with manual override
- ✅ Proper cooldown between crashes
- ✅ Firebase crash event logging
- ✅ GPS coordinates included when available

### **3. Helmet Detection**
- ✅ 64-bit timestamp synchronization
- ✅ 10-second heartbeat timeout
- ✅ Detailed debug logging
- ✅ Proper active/inactive detection

### **4. Safety System**
- ✅ Multi-condition engine start
- ✅ Visual indicator (LED blinking)
- ✅ Detailed blocking reason messages
- ✅ Crash state integration

### **5. Code Organization**
- ✅ Clear section headers
- ✅ Priority-based loop structure
- ✅ Comprehensive error handling
- ✅ Detailed serial output

---

## 📝 Configuration

### **WiFi Settings**
```cpp
const char* ssid = "DPWH";
const char* password = "12345678900";
```

### **Firebase Settings**
```cpp
const String firebaseHost = "https://vigilance-shield-default-rtdb.firebaseio.com";
const String userUID = "MnzBjTBslZNijOkq732PE91hHa23";
const String firebaseAuth = "";  // Optional
```

### **GSM Settings**
```cpp
const String gsmPassword = "1234";
const String authorizedNumber1 = "+639675715673";
const String authorizedNumber2 = "+639542583234";
```

### **Crash Detection Thresholds**
```cpp
const float ACCEL_THRESHOLD = 15.0;  // g-force
const float ROLL_THRESHOLD = 40.0;   // degrees
const unsigned long CRASH_COOLDOWN = 5000;  // milliseconds
```

---

## 🎉 Success Criteria

### **All Systems Working:**
- ✅ Relay responds to commands
- ✅ Crash detection triggers relay
- ✅ Helmet module detected properly
- ✅ Alcohol safety integrated
- ✅ GPS tracking functional
- ✅ GSM commands working
- ✅ Firebase sync active
- ✅ Safety interlock enforced

### **Ready for Production:**
- ✅ All features tested
- ✅ Hardware requirements met
- ✅ Documentation complete
- ✅ Error handling robust
- ✅ Serial debugging available

---

## 📞 Support

If you encounter any issues:

1. Check Serial Monitor output (115200 baud)
2. Type `STATUS` to see system state
3. Type `HELP` to see available commands
4. Verify hardware connections
5. Check Firebase console for data

---

## 🔄 Version History

### **v1.0 - Production Release**
- All features working
- Relay control fixed with external 5V
- Complete safety system
- Comprehensive documentation

---

**Last Updated:** 2024
**Status:** ✅ Production Ready
**File:** `MotorcycleToWebApplication_PRODUCTION.ino`
