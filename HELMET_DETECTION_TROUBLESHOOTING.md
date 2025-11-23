# 🔧 HELMET MODULE DETECTION TROUBLESHOOTING

## Problem: Motorcycle Shows "Helmet Module is OFF" Despite Heartbeat Sending

You're experiencing:
- ✅ Helmet module sends heartbeat to Firebase successfully
- ✅ Alcohol sensor sends data to Firebase successfully
- ❌ Motorcycle module reads helmet as "OFF"
- ❌ Cannot test relay because safety checks fail

---

## 🔍 Diagnostic Steps

### Step 1: Check Helmet Serial Monitor

Upload the updated `HelmetToWebApplication_HEARTBEAT.ino` and watch for:

```
[HEARTBEAT] ═══════════════════════════════
[HEARTBEAT] ✓ Helmet heartbeat sent successfully!
[HEARTBEAT] Status: On
[HEARTBEAT] Timestamp: 1700123456789
[HEARTBEAT] Payload: {"status":"On","lastHeartbeat":1700123456789,"timestamp":1700123456789}
[HEARTBEAT] Motorcycle module should detect this
[HEARTBEAT] ═══════════════════════════════
```

**What to check:**
- ✅ Status should be "On"
- ✅ Timestamp should be a large number (1700000000000+)
- ✅ HTTP code should be 200

### Step 2: Check Motorcycle Serial Monitor

Upload the updated `MotorcycleToWebApplication_COMPLETE.ino` and watch for:

```
[HELMET CHECK] ═══════════════════════════════
[HELMET CHECK] Fetching helmet status from Firebase...
[HELMET CHECK] HTTP Response Code: 200
[HELMET CHECK] Raw Firebase Response:
{"status":"On","lastHeartbeat":1700123456789,"timestamp":1700123456789}
[HELMET CHECK] ─────────────────────────────────
[HELMET CHECK] Status Field: 'On'
[HELMET CHECK] Last Heartbeat: 1700123456789
[HELMET CHECK] Current Time: 1700123458000
[HELMET CHECK] Time Difference: 1211 ms (1.2 seconds)
[HELMET CHECK] Threshold: 15000 ms (15 seconds)
[HELMET CHECK] Status Check: PASS
[HELMET CHECK] Heartbeat Check: PASS
[HELMET CHECK] Final Result: ✓ ACTIVE
[HELMET CHECK] ═══════════════════════════════
```

**What to check:**
- ✅ HTTP Response Code should be 200
- ✅ Status Field should be 'On'
- ✅ Time Difference should be < 15000 ms
- ✅ Both checks should PASS
- ✅ Final Result should be "✓ ACTIVE"

---

## 🐛 Common Issues & Fixes

### Issue 1: Time Difference is 999999 ms

**Symptom:**
```
[HELMET CHECK] Time Difference: 999999 ms (999.9 seconds)
[HELMET CHECK] Heartbeat Check: FAIL
```

**Cause:** Timestamp formats don't match

**Fix:**
1. Make sure BOTH modules use: `unsigned long timestamp = 1700000000000UL + millis();`
2. Re-upload both helmet and motorcycle code
3. Restart both ESP32 modules at the same time

---

### Issue 2: Status Field is 'Off'

**Symptom:**
```
[HELMET CHECK] Status Field: 'Off'
[HELMET CHECK] Status Check: FAIL
```

**Cause:** Helmet module not sending heartbeat or sending "Off"

**Fix:**
1. Check helmet Serial Monitor - should show "Status: On"
2. Make sure helmet code calls `sendHelmetHeartbeat(true)` not `sendHelmetHeartbeat(false)`
3. Check helmet WiFi connection

---

### Issue 3: HTTP Response Code is not 200

**Symptom:**
```
[HELMET CHECK] HTTP Response Code: -1
[HELMET CHECK] ✗ Firebase GET failed: -1
```

**Cause:** WiFi or Firebase connection issue

**Fix:**
1. Check motorcycle WiFi connection
2. Verify Firebase URL is correct
3. Check Firebase rules allow read access
4. Try pinging Firebase from browser

---

### Issue 4: Time Difference > 15000 ms

**Symptom:**
```
[HELMET CHECK] Time Difference: 25000 ms (25.0 seconds)
[HELMET CHECK] Heartbeat Check: FAIL
```

**Cause:** Helmet stopped sending heartbeats

**Fix:**
1. Check helmet Serial Monitor for heartbeat messages
2. Verify helmet WiFi is still connected
3. Check helmet code has heartbeat in loop (every 2 seconds)
4. Restart helmet module

---

## 🧪 Testing Relay Without Safety Checks

If you want to test the relay hardware independently:

### Option 1: Use Bypass Test Code

Upload `RELAY_BYPASS_TEST.ino` which:
- ✅ Bypasses all safety checks
- ✅ Allows manual relay control
- ✅ Still has crash detection
- ⚠️ For testing only!

**Commands:**
- `ON` - Turn relay ON
- `OFF` - Turn relay OFF
- `CRASH` - Simulate crash (relay OFF)
- `STATUS` - Show relay state

### Option 2: Temporarily Disable Safety Check

In `MotorcycleToWebApplication_COMPLETE.ino`, temporarily modify:

```cpp
bool canStartEngine() {
  // ⚠️ TEMPORARY: Bypass safety checks for testing
  return true;  // Always allow engine start
  
  // Original code (comment out for testing):
  // bool helmetModuleOn = isHelmetModuleActive;
  // bool motorcycleOn = true;
  // bool alcoholSafe = isAlcoholSafe();
  // return (motorcycleOn && helmetModuleOn && alcoholSafe);
}
```

**Then test:**
1. Type `START` in Serial Monitor or call via GSM
2. Relay should turn ON
3. Tilt MPU6050 > 40° to trigger crash
4. Relay should turn OFF immediately

**⚠️ IMPORTANT:** Restore original safety check code after testing!

---

## 📊 Expected Timeline

### Normal Operation:
```
0s:  Helmet sends heartbeat (timestamp: 1700000000000)
2s:  Helmet sends heartbeat (timestamp: 1700000002000)
4s:  Helmet sends heartbeat (timestamp: 1700000004000)
5s:  Motorcycle checks helmet (finds heartbeat at 1700000004000)
     Time diff: 1000ms → ACTIVE ✓
```

### Problem Scenario:
```
0s:  Helmet sends heartbeat (timestamp: 1732320000000) ← GPS time!
2s:  Helmet sends heartbeat (timestamp: 1732320002000)
5s:  Motorcycle checks helmet (current: 1700000005000)
     Time diff: 999999ms → INACTIVE ✗
```

---

## 🔧 Quick Fix Checklist

1. ✅ Upload updated `HelmetToWebApplication_HEARTBEAT.ino`
2. ✅ Upload updated `MotorcycleToWebApplication_COMPLETE.ino`
3. ✅ Restart BOTH modules at the same time
4. ✅ Check helmet Serial Monitor for successful heartbeat
5. ✅ Check motorcycle Serial Monitor for "✓ ACTIVE"
6. ✅ Verify time difference < 15000 ms
7. ✅ Test relay with bypass code if needed

---

## 📝 Debug Checklist

Copy this and fill in your values:

```
HELMET MODULE:
[ ] WiFi connected: ___________
[ ] Heartbeat sending: YES / NO
[ ] HTTP code: ___________
[ ] Status sent: ___________
[ ] Timestamp sent: ___________

MOTORCYCLE MODULE:
[ ] WiFi connected: ___________
[ ] HTTP code: ___________
[ ] Status received: ___________
[ ] Timestamp received: ___________
[ ] Current time: ___________
[ ] Time difference: ___________ ms
[ ] Status check: PASS / FAIL
[ ] Heartbeat check: PASS / FAIL
[ ] Final result: ACTIVE / INACTIVE

FIREBASE:
[ ] /devices/helmet exists: YES / NO
[ ] status field: ___________
[ ] lastHeartbeat field: ___________
[ ] timestamp field: ___________
```

---

## 🎯 Success Criteria

You'll know it's working when:

1. ✅ Helmet Serial Monitor shows heartbeat sent every 2 seconds
2. ✅ Motorcycle Serial Monitor shows "✓ ACTIVE"
3. ✅ Time difference is < 15 seconds
4. ✅ Safety status shows "Helmet Module Active: ✓ YES"
5. ✅ Can start engine (relay turns ON)
6. ✅ Crash detection turns relay OFF immediately

---

## 🚀 Next Steps After Fix

Once helmet detection works:

1. Test engine start via GSM call
2. Test crash detection (tilt MPU6050)
3. Verify relay turns OFF on crash
4. Check dashboard shows crash marker
5. Test alcohol detection blocking engine start

---

Need more help? Check the enhanced debug output in Serial Monitor!
