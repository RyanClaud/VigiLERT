# 🚨 Relay LED Indicator Guide

## LED Patterns Explained

### 🔴 Fast Blinking (200ms) - CRASH DETECTED
```
ON-OFF-ON-OFF-ON-OFF (rapid)
```
**Meaning**: Crash detected! Relay is locked OFF
**Action**: 
- Engine CANNOT start
- Motorcycle is in emergency shutdown mode
- Wait for crash state to clear (return to upright position)
- Or manually clear with CLEAR command in Serial Monitor

---

### ⚠️ Slow Blinking (500ms) - SAFETY BLOCK
```
ON----OFF----ON----OFF (slow)
```
**Meaning**: Safety conditions not met
**Possible Reasons**:
- Helmet module not connected
- Alcohol detected
- System initializing

**Action**: 
- Check helmet module is ON
- Check alcohol sensor status
- Wait for all safety checks to pass

---

### ✅ Solid ON - READY TO START
```
ON (constant)
```
**Meaning**: All systems ready, engine can start
**Conditions Met**:
- No crash detected
- Helmet module connected
- No alcohol detected
- Relay is ready

**Action**: 
- Engine can be started normally
- All safety systems operational

---

### ⚫ OFF - Engine Running
```
OFF (constant)
```
**Meaning**: Engine is currently running
**Note**: 
- LED turns off when engine starts
- This is normal operation
- Relay is in ON state (allowing ignition)

---

## 🎯 What Happens During Crash

### Timeline:
```
0ms    - Crash detected (>60° lean or >20g impact)
0ms    - Relay immediately forced OFF
0ms    - LED starts fast blinking (200ms)
0ms    - Engine power cut (if running)
100ms  - Firebase notification sent
200ms  - Dashboard updated
```

### Relay Behavior:
1. **Immediately**: `digitalWrite(relayPin, LOW)` - Relay OFF
2. **LED**: Fast blinking (200ms intervals)
3. **Engine**: Cannot start (ignition circuit open)
4. **State**: Locked until crash cleared

### How to Clear Crash State:
1. **Automatic**: Return motorcycle to upright position (<30° lean) and stable (<18g)
2. **Manual**: Type `CLEAR` in Serial Monitor
3. **Timeout**: Automatically clears after conditions met

---

## 🔧 Relay Wiring Verification

### Check Your Connections:

**Relay Module:**
```
VCC  → ESP32 5V
GND  → ESP32 GND
IN1  → ESP32 GPIO 27
```

**Relay Contacts:**
```
COM  → From key switch (12V when key ON)
NO   → To ignition system (ECU/CDI)
NC   → Leave empty (not used)
```

### LED on Relay Module:
- **DS1 (Relay 1)**: Should match the behavior described above
- **DS2 (Relay 2)**: Not used (stays off)

---

## 🧪 Testing Relay Response

### Test 1: Normal Operation
1. **Power on system**
2. **LED should**: Slow blink (safety block - helmet off)
3. **Relay state**: OFF (engine cannot start)
4. **Expected**: ✅ Correct

### Test 2: Crash Detection
1. **Tilt motorcycle >60°**
2. **LED should**: Fast blink (200ms)
3. **Relay state**: OFF (locked)
4. **Serial Monitor**: "🚨 EMERGENCY SHUTDOWN"
5. **Expected**: ✅ Correct

### Test 3: Crash Clear
1. **Return to upright** (<30° lean)
2. **Wait 2-3 seconds**
3. **LED should**: Return to slow blink or solid ON
4. **Serial Monitor**: "✓ Crash cleared"
5. **Expected**: ✅ Correct

### Test 4: Manual Clear
1. **While in crash state**
2. **Type `CLEAR`** in Serial Monitor
3. **LED should**: Stop fast blinking
4. **Relay state**: Returns to normal
5. **Expected**: ✅ Correct

---

## 🚨 Troubleshooting

### Issue 1: LED Not Blinking During Crash
**Symptoms**: Crash detected but LED stays solid or off

**Possible Causes**:
1. LED pin not connected (GPIO 13)
2. LED burned out
3. Code not uploaded correctly

**Solutions**:
1. Check wiring: GPIO 13 → LED (with resistor)
2. Test LED with simple blink sketch
3. Re-upload code

---

### Issue 2: Relay Not Turning OFF During Crash
**Symptoms**: Crash detected but relay stays ON, engine still starts

**Possible Causes**:
1. Relay wiring incorrect
2. Relay module faulty
3. Using NC instead of NO contact

**Solutions**:
1. Verify wiring: GPIO 27 → IN1
2. Test relay with multimeter (COM-NO should be OPEN when crash)
3. Check relay module LED (DS1 should be OFF during crash)
4. Swap relay module if faulty

---

### Issue 3: Relay Stuck OFF After Crash Clear
**Symptoms**: Crash cleared but relay won't turn back ON

**Possible Causes**:
1. Crash state not actually cleared
2. Safety conditions not met
3. Code issue

**Solutions**:
1. Check Serial Monitor for "✓ Crash cleared" message
2. Verify motorcycle is upright (<30° lean)
3. Type `CLEAR` to manually clear
4. Check safety conditions (helmet, alcohol)

---

## 📊 Expected Behavior Summary

| State | LED Pattern | Relay State | Engine Can Start? |
|-------|-------------|-------------|-------------------|
| **Crash Detected** | Fast blink (200ms) | OFF | ❌ NO |
| **Safety Block** | Slow blink (500ms) | OFF | ❌ NO |
| **Ready** | Solid ON | OFF | ✅ YES (when key turned) |
| **Engine Running** | OFF | ON | ✅ Already running |

---

## 🎯 Key Points

1. **Crash = Immediate Relay OFF**: No delay, instant response
2. **Fast Blink = Emergency**: System is in crash lockout mode
3. **Slow Blink = Warning**: Safety conditions not met
4. **Solid ON = Ready**: All systems go, engine can start
5. **Manual Override**: Type `CLEAR` to reset crash state

---

## ✅ Success Criteria

Your relay system is working correctly if:

1. ✅ LED fast blinks (200ms) when crash detected
2. ✅ Relay turns OFF immediately during crash
3. ✅ Engine cannot start during crash state
4. ✅ LED returns to normal after crash cleared
5. ✅ Relay allows engine start after crash cleared
6. ✅ Manual CLEAR command works

---

## 🚀 Quick Reference

**Fast Blink (200ms)** = 🚨 CRASH - Relay locked OFF
**Slow Blink (500ms)** = ⚠️ WARNING - Safety block
**Solid ON** = ✅ READY - Can start engine
**OFF** = 🏍️ RUNNING - Engine is on

**To clear crash**: Return to upright position or type `CLEAR`

---

Your relay safety system is now fully functional! 🛡️
