# 🎯 Crash Detection Thresholds - Optimized for Real Riding

## ✅ New Thresholds (Adjusted for Accuracy)

### 1. **Acceleration Threshold**
- **Previous**: 15.0g (too sensitive)
- **New**: 20.0g (only severe impacts)
- **Reason**: Normal bumps and potholes can reach 10-15g, but crashes typically exceed 20g

### 2. **Roll Angle Threshold**
- **Previous**: -47° to +40° (too sensitive)
- **New**: -60° to +60° (only severe lean/fall)
- **Reason**: Normal motorcycle lean in turns is 20-30°, crashes/falls exceed 60°

### 3. **Confirmation Count**
- **Previous**: 2 consecutive readings
- **New**: 3 consecutive readings
- **Reason**: Reduces false positives from momentary spikes

---

## 📊 Motorcycle Lean Angles Reference

### Normal Riding:
| Situation | Lean Angle | Status |
|-----------|------------|--------|
| Straight riding | 0-5° | ✅ Normal |
| Gentle turn | 10-20° | ✅ Normal |
| Moderate turn | 20-30° | ✅ Normal |
| Aggressive turn | 30-45° | ⚠️ Sporty (safe for experienced riders) |

### Dangerous/Crash:
| Situation | Lean Angle | Status |
|-----------|------------|--------|
| Extreme lean | 45-60° | ⚠️ Dangerous (near limit) |
| **Falling/Crash** | **>60°** | 🚨 **CRASH DETECTED** |
| Bike on side | 80-90° | 🚨 Crashed/Fallen |

---

## 🎯 Detection Logic

### Crash is Detected When:
```
(Acceleration ≥ 20.0g  OR  Roll < -60°  OR  Roll > 60°)
AND
3 consecutive readings within 150ms confirm the condition
AND
5 seconds have passed since last crash (cooldown)
```

### Examples:

#### ✅ Normal Riding (No False Alarm):
```
Scenario: Aggressive cornering
Roll: 35° (within safe range)
Accel: 12g (normal for turns)
Result: NO CRASH DETECTED ✅
```

#### ✅ Pothole/Bump (No False Alarm):
```
Scenario: Hit a pothole
Roll: 5° (upright)
Accel: 18g (momentary spike)
Result: NO CRASH DETECTED ✅ (below 20g threshold)
```

#### 🚨 Actual Crash (Correctly Detected):
```
Scenario: Motorcycle falls over
Roll: 75° (severe lean)
Accel: 25g (impact)
Result: CRASH DETECTED! 🚨
```

#### 🚨 High-Speed Impact (Correctly Detected):
```
Scenario: Collision with object
Roll: 15° (upright)
Accel: 28g (severe impact)
Result: CRASH DETECTED! 🚨
```

---

## 🔧 Fine-Tuning Guide

### If You Get False Positives (Too Sensitive):

**Option 1: Increase Acceleration Threshold**
```cpp
const float ACCEL_THRESHOLD = 25.0;  // Change from 20.0 to 25.0
```

**Option 2: Increase Roll Angle Threshold**
```cpp
bool crashCondition = (currentTotalAccel >= ACCEL_THRESHOLD || currentRoll < -70 || currentRoll > 70);
// Change from 60° to 70°
```

**Option 3: Increase Confirmation Count**
```cpp
if (crashConfirmCount >= 4) {  // Change from 3 to 4
```

**Option 4: Increase Confirmation Window**
```cpp
} else if (millis() - firstCrashDetection < 200) {  // Change from 100ms to 200ms
```

---

### If You Miss Real Crashes (Not Sensitive Enough):

**Option 1: Decrease Acceleration Threshold**
```cpp
const float ACCEL_THRESHOLD = 18.0;  // Change from 20.0 to 18.0
```

**Option 2: Decrease Roll Angle Threshold**
```cpp
bool crashCondition = (currentTotalAccel >= ACCEL_THRESHOLD || currentRoll < -55 || currentRoll > 55);
// Change from 60° to 55°
```

**Option 3: Decrease Confirmation Count**
```cpp
if (crashConfirmCount >= 2) {  // Change from 3 to 2
```

---

## 🧪 Testing Procedure

### Step 1: Test Normal Riding
1. **Ride normally** on straight road
2. **Make gentle turns** (20-30° lean)
3. **Hit small bumps** and potholes
4. **Expected**: NO crash detection ✅

### Step 2: Test Aggressive Riding
1. **Make sharp turns** (30-45° lean)
2. **Accelerate hard** (10-15g)
3. **Brake hard** (10-15g)
4. **Expected**: NO crash detection ✅

### Step 3: Test Crash Detection (Safely!)
1. **Tilt motorcycle** beyond 60° (while stationary)
2. **Expected**: CRASH DETECTED! 🚨
3. **Return to upright** position
4. **Expected**: Crash cleared after 5 seconds ✅

### Step 4: Test Impact Detection
1. **Hit a hard bump** at speed (15-18g)
2. **Expected**: NO crash detection ✅
3. **Simulate hard impact** (shake violently to exceed 20g)
4. **Expected**: CRASH DETECTED! 🚨

---

## 📈 Recommended Settings by Riding Style

### Conservative (Fewer False Positives):
```cpp
const float ACCEL_THRESHOLD = 25.0;  // Very high
bool crashCondition = (currentTotalAccel >= ACCEL_THRESHOLD || currentRoll < -70 || currentRoll > 70);
if (crashConfirmCount >= 4) {  // 4 confirmations
```
**Best for**: City riding, commuting, beginners

### Balanced (Current Settings):
```cpp
const float ACCEL_THRESHOLD = 20.0;  // Moderate
bool crashCondition = (currentTotalAccel >= ACCEL_THRESHOLD || currentRoll < -60 || currentRoll > 60);
if (crashConfirmCount >= 3) {  // 3 confirmations
```
**Best for**: Mixed riding, general use

### Aggressive (More Sensitive):
```cpp
const float ACCEL_THRESHOLD = 18.0;  // Lower
bool crashCondition = (currentTotalAccel >= ACCEL_THRESHOLD || currentRoll < -55 || currentRoll > 55);
if (crashConfirmCount >= 2) {  // 2 confirmations
```
**Best for**: Off-road, racing, high-risk riding

---

## 🎯 Current Configuration Summary

```cpp
// Acceleration threshold
const float ACCEL_THRESHOLD = 20.0g;  // Severe impact only

// Roll angle threshold
Roll < -60° OR Roll > 60°  // Severe lean/fall only

// Confirmation
3 consecutive readings within 150ms  // Reduces false positives

// Cooldown
5 seconds between crashes  // Prevents duplicate alerts
```

---

## ✅ Expected Behavior

### During Normal Riding:
- **Lean up to 45°**: ✅ No alert
- **Bumps up to 18g**: ✅ No alert
- **Sharp turns**: ✅ No alert
- **Hard braking**: ✅ No alert

### During Crash:
- **Fall over (>60° lean)**: 🚨 Alert within 150ms
- **High impact (>20g)**: 🚨 Alert within 150ms
- **Combination**: 🚨 Alert within 150ms

### After Crash:
- **Return to upright**: ✅ Clears after stable for 2 seconds
- **New crash**: 🚨 Can detect after 5-second cooldown

---

## 🚨 Safety Notes

1. **Test in safe environment** before real riding
2. **Adjust thresholds** based on your riding style
3. **Monitor for false positives** during first few rides
4. **Don't rely solely** on automatic detection - always wear helmet!
5. **Emergency contacts** should be configured in case of real crash

---

## 📞 Quick Reference

**Too many false alarms?**
→ Increase ACCEL_THRESHOLD to 25.0
→ Increase roll angles to ±70°
→ Increase confirmations to 4

**Missing real crashes?**
→ Decrease ACCEL_THRESHOLD to 18.0
→ Decrease roll angles to ±55°
→ Decrease confirmations to 2

**Perfect balance?**
→ Keep current settings (20.0g, ±60°, 3 confirmations)

---

## 🎉 Result

With these optimized thresholds, your crash detection system will:
- ✅ **Ignore normal riding** (no false alarms during turns)
- ✅ **Ignore bumps and potholes** (no false alarms on rough roads)
- ✅ **Detect real crashes** (falls, collisions, severe impacts)
- ✅ **Respond quickly** (<150ms detection time)
- ✅ **Stay reliable** (3-reading confirmation reduces false positives)

**Your system is now tuned for real-world motorcycle riding!** 🏍️🛡️
