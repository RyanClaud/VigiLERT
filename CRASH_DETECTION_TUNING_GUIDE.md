# 🎯 Crash Detection Tuning Guide

## 📊 Current Issue Analysis:

From your Serial output:
```
Roll: 100.0° → Sensor miscalibrated (bike not upside down!)
Sudden change: 0.68 g → Too low for crash
Confirmation: 0.40 g → Correctly rejected as false alarm
```

**Problem:** Roll sensor is giving bad readings (100° when bike is upright), triggering false detections.

---

## ✅ FIXES APPLIED:

### 1. Increased Roll Threshold
```cpp
// Before: 45° threshold (too sensitive)
bool severeRoll = (currentRoll < -45 || currentRoll > 45);

// After: 60° threshold (less sensitive to sensor noise)
bool severeRoll = (currentRoll < -60 || currentRoll > 60);
```

### 2. Combined Conditions (AND logic)
```cpp
// Before: Impact OR Roll (too easy to trigger)
if (accelChange > 5.0 || severeRoll) {
  // Crash!
}

// After: High impact OR (Moderate impact AND Severe roll)
bool suddenImpact = (accelChange > 5.0);      // High impact alone
bool moderateImpact = (accelChange > 3.0);    // Moderate impact
bool severeRoll = (abs(currentRoll) > 60);    // Severe roll

if (suddenImpact || (moderateImpact && severeRoll)) {
  // Crash!
}
```

**Benefits:**
- High impact (>5g) alone triggers crash
- Moderate impact (3-5g) requires severe roll (>60°) to trigger
- Reduces false alarms from sensor noise

---

## 🧪 TESTING CRASH DETECTION:

### Method 1: Serial Command (Recommended)
```
1. Open Serial Monitor (115200 baud)
2. Type: CRASH
3. Press Enter
4. Should see:
   🧪 MANUAL CRASH TEST TRIGGERED!
   📍 Sending test crash WITH GPS: 12.747565, 121.482181
   ✓ Test crash sent to Firebase!
   ✓ Check dashboard for crash marker and alert!
```

### Method 2: Physical Test (Careful!)
```
1. Hold motorcycle steady
2. Tilt quickly to >60° angle
3. Should trigger if acceleration change >3g
4. Or shake violently to create >5g impact
```

### Method 3: Simulate in Code
```cpp
// Add to loop() for testing:
if (Serial.available()) {
  String cmd = Serial.readStringUntil('\n');
  if (cmd == "SIMULATE") {
    // Force crash detection
    previousTotalAccel = 5.0;
    currentTotalAccel = 15.0; // 10g change!
    // Will trigger on next check
  }
}
```

---

## 🔧 TUNING PARAMETERS:

### If Too Sensitive (False Alarms):

**Option 1: Increase Impact Threshold**
```cpp
bool suddenImpact = (accelChange > 7.0);      // Was 5.0
bool moderateImpact = (accelChange > 5.0);    // Was 3.0
```

**Option 2: Increase Roll Threshold**
```cpp
bool severeRoll = (abs(currentRoll) > 75);    // Was 60
```

**Option 3: Increase Confirmation Threshold**
```cpp
if (confirmChange > 5.0) {  // Was 3.0
  // Confirmed crash
}
```

**Option 4: Add Speed Check**
```cpp
bool isMoving = (gps.speed.kmph() > 5.0);
if ((suddenImpact || (moderateImpact && severeRoll)) && isMoving) {
  // Only detect crashes when moving
}
```

### If Not Sensitive Enough (Misses Crashes):

**Option 1: Decrease Impact Threshold**
```cpp
bool suddenImpact = (accelChange > 3.0);      // Was 5.0
bool moderateImpact = (accelChange > 2.0);    // Was 3.0
```

**Option 2: Decrease Roll Threshold**
```cpp
bool severeRoll = (abs(currentRoll) > 45);    // Was 60
```

**Option 3: Decrease Confirmation Threshold**
```cpp
if (confirmChange > 2.0) {  // Was 3.0
  // Confirmed crash
}
```

---

## 📊 DETECTION SCENARIOS:

### Scenario 1: High-Speed Impact
```
Speed: 50 km/h
Acceleration change: 12.0 g
Roll: 15°
Result: CRASH DETECTED ✓ (suddenImpact = true)
```

### Scenario 2: Low-Speed Tip Over
```
Speed: 5 km/h
Acceleration change: 4.0 g
Roll: 75°
Result: CRASH DETECTED ✓ (moderateImpact + severeRoll = true)
```

### Scenario 3: Bump in Road
```
Speed: 30 km/h
Acceleration change: 2.5 g
Roll: 10°
Result: NO CRASH ✓ (below thresholds)
```

### Scenario 4: Sensor Noise
```
Speed: 0 km/h
Acceleration change: 0.68 g
Roll: 100° (bad sensor reading)
Result: NO CRASH ✓ (accelChange too low)
```

---

## 🐛 DEBUGGING TIPS:

### Check Serial Output:
```
[SENSOR] Accel: 10.00 | Roll: 100.0° | Battery: 0.00V
⚠️⚠️⚠️ CRASH DETECTED! ⚠️⚠️⚠️
Sudden change: 0.68 g | Roll: 100.0° | Speed: 0.5 km/h
Conditions: Impact=0.68g (>5.0?) | Roll=100.0° (>60?) | Moderate=0.68g (>3.0?)
Confirmation check: Change = 0.40 g
⚠️ False alarm - just a bump/vibration, not a crash
```

### What to Look For:
1. **Acceleration change** - Should be >5g for real crash
2. **Roll angle** - Should be >60° for tip-over
3. **Confirmation change** - Should be >3g to confirm
4. **Speed** - Real crashes usually happen when moving

### Common Issues:

**Issue 1: Roll sensor gives bad readings (100° when upright)**
- **Cause:** MPU6050 not calibrated or mounted incorrectly
- **Fix:** Increase roll threshold to 60° or 75°
- **Alternative:** Disable roll detection, use only acceleration

**Issue 2: Constant false alarms**
- **Cause:** Thresholds too low or sensor too sensitive
- **Fix:** Increase thresholds (5g → 7g, 60° → 75°)
- **Alternative:** Add speed check (only detect when moving)

**Issue 3: Misses real crashes**
- **Cause:** Thresholds too high
- **Fix:** Decrease thresholds (5g → 3g, 60° → 45°)
- **Alternative:** Remove confirmation delay

---

## 🎯 RECOMMENDED SETTINGS:

### For Stable Detection (Current):
```cpp
bool suddenImpact = (accelChange > 5.0);      // High impact
bool moderateImpact = (accelChange > 3.0);    // Moderate impact
bool severeRoll = (abs(currentRoll) > 60);    // Severe roll
float confirmThreshold = 3.0;                  // Confirmation
```

### For Sensitive Detection (Catches more):
```cpp
bool suddenImpact = (accelChange > 3.0);      // Lower threshold
bool moderateImpact = (accelChange > 2.0);    // Lower threshold
bool severeRoll = (abs(currentRoll) > 45);    // Lower threshold
float confirmThreshold = 2.0;                  // Lower threshold
```

### For Conservative Detection (Fewer false alarms):
```cpp
bool suddenImpact = (accelChange > 7.0);      // Higher threshold
bool moderateImpact = (accelChange > 5.0);    // Higher threshold
bool severeRoll = (abs(currentRoll) > 75);    // Higher threshold
float confirmThreshold = 5.0;                  // Higher threshold
bool isMoving = (gps.speed.kmph() > 10.0);    // Only when moving
```

---

## 🚀 TESTING CHECKLIST:

- [ ] Upload updated Arduino code
- [ ] Open Serial Monitor (115200 baud)
- [ ] Type "CRASH" to test manual crash
- [ ] Check dashboard for crash marker
- [ ] Check dashboard for crash alert
- [ ] Verify GPS coordinates are correct
- [ ] Test with physical tilt (>60°)
- [ ] Verify no false alarms when stationary
- [ ] Verify no false alarms from vibrations

---

## 📈 EXPECTED RESULTS:

✅ **Manual test ("CRASH" command)** → Creates crash marker on dashboard  
✅ **High impact (>5g)** → Triggers crash detection  
✅ **Moderate impact + severe roll** → Triggers crash detection  
✅ **Small bumps (<3g)** → No false alarm  
✅ **Sensor noise (bad roll reading)** → No false alarm  
✅ **Stationary vibrations** → No false alarm  

The system should now accurately detect real crashes while ignoring false alarms!
