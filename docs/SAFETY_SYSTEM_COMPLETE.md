# 🛡️ VigiLERT Comprehensive Safety System
## Three Safety Features Using Single Relay

---

## 🎯 Overview

All three safety features are integrated and controlled by a **SINGLE RELAY** (Pin 27) on the motorcycle module. The relay controls the engine ignition circuit.

- **Relay HIGH** = Engine can run
- **Relay LOW** = Engine is OFF

---

## ✅ Safety Feature 1: Device Pairing Requirement

### Description
The motorcycle engine **WILL NOT START** unless both devices are turned ON and communicating.

### How It Works
1. **Helmet Module** sends heartbeat every 2 seconds to Firebase:
   - Path: `/helmet_public/{userUID}/devices/helmet`
   - Data: `{ status: "On", lastHeartbeat: timestamp }`

2. **Motorcycle Module** checks before engine start:
   - Reads helmet heartbeat from Firebase
   - Verifies heartbeat is less than 5 seconds old
   - Checks physical helmet switch is ON (Pin 39)
   - Checks motorcycle module is active

3. **Engine Start Blocked If:**
   - Helmet module is OFF or not responding
   - Helmet switch is not physically connected
   - Motorcycle module is OFF

### Code Location
- **Motorcycle**: `canStartEngine()` function checks `isHelmetModuleActive`
- **Helmet**: `sendHelmetHeartbeat()` sends status every 2 seconds

---

## ✅ Safety Feature 2: Alcohol Detection Prevention

### Description
The motorcycle engine **WILL NOT START** if alcohol is detected in the helmet.

### How It Works
1. **Helmet Module** (MQ3 Sensor):
   - Reads alcohol sensor value (Pin 34)
   - Compares to threshold (2000)
   - Sends status to Firebase every 1 second:
     - Path: `/helmet_public/{userUID}/alcohol/status`
     - Data: `{ status: "Safe" or "Danger", sensorValue: value }`

2. **Motorcycle Module** checks before engine start:
   - Calls `isAlcoholSafe()` function
   - Reads alcohol status from Firebase
   - Only allows engine start if status = "Safe"

3. **Engine Start Blocked If:**
   - Alcohol sensor value > 2000
   - Status = "Danger"
   - Cannot read Firebase (defaults to Safe for safety)

### Code Location
- **Helmet**: `sendAlcoholToFirebase()` sends alcohol data
- **Motorcycle**: `isAlcoholSafe()` checks alcohol status before engine start

---

## ✅ Safety Feature 3: Automatic Crash Shutdown

### Description
The motorcycle engine **AUTOMATICALLY SHUTS DOWN** 2 seconds after detecting a crash event.

### How It Works
1. **Crash Detection** (MPU6050 Sensor):
   - Monitors total acceleration (threshold: 15g)
   - Monitors lean angle (threshold: -47° to 40°)
   - Detects sudden impacts or severe tilts

2. **Automatic Shutdown Sequence**:
   ```
   Crash Detected → Wait 2 seconds → Set Relay LOW → Engine OFF
   ```

3. **Safety Benefits:**
   - Prevents fire hazards from fuel leaks
   - Stops engine damage from continued running
   - Reduces risk of further accidents

### Code Location
- **Motorcycle**: Crash detection in main `loop()` function
- After crash detected, calls `stopEngine()` after 2-second delay

---

## 🔧 Hardware Setup

### Motorcycle Module (ESP32)
```
Pin 27  → Relay Control (Engine Ignition)
Pin 39  → Helmet Switch (Physical connection check)
Pin 21  → MPU6050 SDA (Crash detection)
Pin 22  → MPU6050 SCL (Crash detection)
```

### Helmet Module (ESP32)
```
Pin 34  → MQ3 Alcohol Sensor (Analog input)
Pin 2   → LED Indicator (Alcohol warning)
Pin 5   → Buzzer (Alcohol alert)
```

### Relay Wiring
```
Relay Module:
- VCC → 5V
- GND → GND
- IN  → GPIO 27 (Motorcycle ESP32)
- COM → Motorcycle Ignition Circuit
- NO  → Engine Power (Normally Open)
```

---

## 📊 Firebase Data Structure

```json
{
  "helmet_public": {
    "{userUID}": {
      "devices": {
        "helmet": {
          "status": "On",
          "lastHeartbeat": 1700000000000,
          "timestamp": 1700000000000
        },
        "motorcycle": {
          "status": "On",
          "lastHeartbeat": 1700000000000,
          "timestamp": 1700000000000
        }
      },
      "alcohol": {
        "status": {
          "status": "Safe",
          "sensorValue": 1500,
          "timestamp": 123456
        }
      }
    }
  }
}
```

---

## 🧪 Testing the Safety System

### Test 1: Device Pairing
1. Turn ON helmet module → Wait for heartbeat
2. Turn ON motorcycle module
3. Try to start engine via SMS: `1234 START`
4. ✅ Should work if both devices ON
5. Turn OFF helmet module
6. Try to start engine again
7. ❌ Should be BLOCKED

### Test 2: Alcohol Detection
1. Ensure both devices are ON
2. Blow alcohol vapor near MQ3 sensor
3. Watch Serial Monitor: Should show "DANGER ⚠️"
4. Try to start engine via SMS: `1234 START`
5. ❌ Should be BLOCKED with message "Alcohol detected"
6. Wait for sensor to clear (fresh air)
7. Try to start engine again
8. ✅ Should work when status = "Safe"

### Test 3: Crash Shutdown
1. Start the engine (ensure it's running)
2. Trigger crash via Serial: Type `CRASH` in Serial Monitor
3. Watch for crash detection message
4. Engine should shut down after 2 seconds
5. ✅ Relay should go LOW, engine OFF

---

## 🔍 Serial Monitor Output

### Successful Engine Start
```
╔════════════════════════════════════════╗
║   SAFETY CHECK - Engine Start Request ║
╠════════════════════════════════════════╣
║ ✓ Motorcycle Module: ON               ║
║ ✓ Helmet Module: ON                   ║
║ ✓ Helmet Switch: ON                   ║
║ ✓ Alcohol Test: SAFE                  ║
╚════════════════════════════════════════╝

✅ [ENGINE] Started successfully!
✅ All safety checks passed
```

### Blocked Engine Start (Alcohol Detected)
```
╔════════════════════════════════════════╗
║   SAFETY CHECK - Engine Start Request ║
╠════════════════════════════════════════╣
║ ✓ Motorcycle Module: ON               ║
║ ✓ Helmet Module: ON                   ║
║ ✓ Helmet Switch: ON                   ║
║ ✗ Alcohol Test: DANGER                ║
╚════════════════════════════════════════╝
  ❌ BLOCKED: Alcohol detected - unsafe to ride!

❌❌❌ [SAFETY] ENGINE START BLOCKED! ❌❌❌
All safety requirements must be met:
  1. Both devices must be ON
  2. No alcohol detected
  3. Helmet switch must be ON
```

### Crash Shutdown
```
⚠️⚠️⚠️ CRASH DETECTED! ⚠️⚠️⚠️
Impact: 18.50 g | Roll: 45.2° | Threshold: 15.00 g

⚠️ SAFETY PROTOCOL ACTIVATED!
⚠️ Engine will shut down in 2 seconds...
✓ Engine automatically shut down for safety
✓ This prevents fire hazards and further damage
```

---

## 📝 Summary

✅ **All 3 safety features are now integrated**
✅ **Single relay controls all safety functions**
✅ **Helmet module sends alcohol data to Firebase**
✅ **Motorcycle module checks all conditions before engine start**
✅ **Automatic crash shutdown after 2 seconds**

The system provides comprehensive protection for the rider through device pairing, alcohol detection, and automatic crash response.
