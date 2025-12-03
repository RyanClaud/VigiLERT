# 🛡️ Complete Safety System - Relay Control

## ✅ Three Safety Conditions

The relay automatically turns OFF (cuts engine power) when ANY of these conditions fail:

### 1. **Helmet Connection** 
- **Check**: Helmet module sends heartbeat to Firebase every 2 seconds
- **Status**: `isHelmetModuleActive` (checked every 10 seconds)
- **Fail Condition**: No heartbeat for >15 seconds
- **Relay Action**: Turn OFF → Engine stops
- **LED Pattern**: Slow blink (500ms)

### 2. **Alcohol Detection**
- **Check**: Helmet module sends alcohol sensor data to Firebase
- **Status**: `isAlcoholSafe()` reads from Firebase `/alcohol/status`
- **Fail Condition**: Status = "Danger" (alcohol detected)
- **Relay Action**: Turn OFF → Engine stops
- **LED Pattern**: Medium blink (300ms)

### 3. **Crash Detection**
- **Check**: Motorcycle MPU6050 detects severe lean (>60°) or impact (>20g)
- **Status**: `crashDetected` (checked every 5ms in main loop)
- **Fail Condition**: Crash detected with 3 confirmations
- **Relay Action**: Turn OFF → Engine stops immediately
- **LED Pattern**: Fast blink (200ms)

---

## 🎯 Safety Logic Flow

```
Every 100ms:
  ├─ Check Helmet Connected? (from Firebase)
  ├─ Check Alcohol Safe? (from Firebase)  
  ├─ Check No Crash? (from MPU6050)
  │
  ├─ ALL 3 CONDITIONS MET?
  │  ├─ YES → Relay can be controlled normally
  │  │         (ON when engine running, OFF when stopped)
  │  │
  │  └─ NO → Force relay OFF immediately
  │           └─ Stop engine if running
  │           └─ Show LED pattern based on failure type
```

---

## 📊 Relay States

| Condition | Helmet | Alcohol | Crash | Relay | Engine | LED Pattern |
|-----------|--------|---------|-------|-------|--------|-------------|
| **All Safe** | ✅ Connected | ✅ Safe | ✅ No | ON/OFF* | Can run | Solid/Off |
| **Helmet Off** | ❌ Disconnected | ✅ Safe | ✅ No | OFF | Stopped | Slow blink (500ms) |
| **Alcohol Detected** | ✅ Connected | ❌ Danger | ✅ No | OFF | Stopped | Medium blink (300ms) |
| **Crash Detected** | ✅ Connected | ✅ Safe | ❌ Yes | OFF | Stopped | Fast blink (200ms) |
| **Multiple Failures** | ❌ | ❌ | ❌ | OFF | Stopped | Fastest blink shown |

*Relay ON when engine running, OFF when stopped

---

## 🔄 Data Flow

### Helmet Module → Firebase:
```
Every 2 seconds:
  /devices/helmet/
    ├─ status: "On"
    ├─ lastHeartbeat: timestamp
    └─ timestamp: timestamp

Every 5 seconds:
  /alcohol/status/
    ├─ status: "Safe" or "Danger"
    ├─ sensorValue: 0-4095
    └─ timestamp: timestamp
```

### Motorcycle Module → Firebase:
```
Every 10 seconds (checks):
  ← Read /devices/helmet/lastHeartbeat
  ← Read /alcohol/status/status

Immediately on crash:
  /crashes/
    ├─ timestamp: timestamp
    ├─ hasGPS: true/false
    ├─ lat: latitude (if GPS)
    ├─ lng: longitude (if GPS)
    ├─ impactStrength: g-force
    ├─ roll: lean angle
    └─ type: "crash"
```

---

## 🧪 Testing Each Condition

### Test 1: Helmet Disconnection
1. **Turn OFF helmet module** (or disconnect power)
2. **Wait 15 seconds** (heartbeat timeout)
3. **Expected**:
   - Serial: "⚠️⚠️⚠️ SAFETY SHUTDOWN"
   - Serial: "❌ Helmet disconnected"
   - Relay: OFF (LED off, no click)
   - LED: Slow blink (500ms)
   - Engine: Cannot start

### Test 2: Alcohol Detection
1. **Blow on alcohol sensor** (or set Firebase manually)
2. **Wait for next check** (up to 10 seconds)
3. **Expected**:
   - Serial: "⚠️⚠️⚠️ SAFETY SHUTDOWN"
   - Serial: "❌ Alcohol detected"
   - Relay: OFF
   - LED: Medium blink (300ms)
   - Engine: Cannot start

### Test 3: Crash Detection
1. **Tilt motorcycle >60°** (or shake hard >20g)
2. **Immediate response** (<150ms)
3. **Expected**:
   - Serial: "🚨 EMERGENCY SHUTDOWN"
   - Serial: "❌ Crash detected"
   - Relay: OFF
   - LED: Fast blink (200ms)
   - Engine: Stops immediately if running

### Test 4: All Conditions Met
1. **Helmet ON** (sending heartbeat)
2. **No alcohol** (status = "Safe")
3. **No crash** (upright, stable)
4. **Expected**:
   - Relay: Can be controlled (ON/OFF based on engine state)
   - LED: Solid ON when ready, OFF when running
   - Engine: Can start normally

---

## 🔍 Monitoring Safety Status

### Serial Monitor Output:
```
Every 100ms (safety check):
  - Checks all 3 conditions
  - Only prints when state changes

When safety violation occurs:
  ⚠️⚠️⚠️ SAFETY SHUTDOWN ⚠️⚠️⚠️
    ❌ Helmet disconnected
    ❌ Alcohol detected
    ❌ Crash detected
  ✓ Relay OFF - Engine stopped
```

### LED Patterns:
- **Fast blink (200ms)** = 🚨 CRASH - Most critical
- **Medium blink (300ms)** = ⚠️ ALCOHOL - Dangerous
- **Slow blink (500ms)** = ⚠️ HELMET OFF - Warning
- **Solid ON** = ✅ READY - All safe, can start
- **OFF** = 🏍️ RUNNING - Engine is on

---

## 🎯 Key Features

### 1. **Immediate Response**
- Crash detection: <150ms
- Safety check: Every 100ms
- Relay control: Instant

### 2. **Fail-Safe Design**
- ANY condition fails → Relay OFF
- Engine stops immediately
- Cannot restart until all conditions met

### 3. **Visual Feedback**
- Different LED patterns for each failure type
- Easy to diagnose which condition failed
- Always know system status

### 4. **Firebase Integration**
- Helmet status from Firebase
- Alcohol status from Firebase
- Crash events sent to Firebase
- Dashboard shows real-time status

### 5. **Non-Blocking**
- Firebase checks every 10 seconds (doesn't slow loop)
- Crash detection every 5ms (ultra-fast)
- Safety enforcement every 100ms (responsive)

---

## 🚨 Emergency Procedures

### If System Fails (Relay Stuck OFF):
1. **Check Serial Monitor** for error messages
2. **Verify Firebase connection** (WiFi status)
3. **Check all 3 conditions**:
   - Helmet heartbeat in Firebase
   - Alcohol status in Firebase
   - Crash state cleared
4. **Manual override**: Type `CLEAR` in Serial Monitor to clear crash
5. **Emergency bypass**: Disconnect relay and connect ignition wires directly

### If False Shutdowns Occur:
1. **Check helmet heartbeat** - Should update every 2 seconds
2. **Check alcohol sensor** - Should read <2000 (safe)
3. **Check crash thresholds** - May need adjustment
4. **Check Firebase connection** - Should be stable

---

## ✅ Success Criteria

Your safety system is working correctly if:

1. ✅ Engine stops when helmet disconnected
2. ✅ Engine stops when alcohol detected
3. ✅ Engine stops when crash detected
4. ✅ Engine can start when all conditions met
5. ✅ LED shows correct pattern for each failure
6. ✅ Relay responds within 100ms
7. ✅ Dashboard shows real-time status

---

## 🎉 Result

You now have a **comprehensive 3-layer safety system**:
- 🪖 Helmet connection monitoring
- 🍺 Alcohol detection
- 💥 Crash detection

All integrated with **automatic relay control** and **real-time Firebase monitoring**!

**Your motorcycle is now protected by a triple-redundant safety system!** 🛡️🏍️
