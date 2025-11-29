# Anti-Theft Dashboard Alert Testing Guide

## 🎯 Quick Test Procedure

### Prerequisites
- ESP32 with WiFi connected
- Dashboard open in browser
- Anti-theft system armed

### Test 1: Basic Vibration Alert

**Steps**:
```
1. Open Serial Monitor (115200 baud)
2. Type: ARM THEFT
3. Wait 30 seconds (system arms)
4. Type: TEST VIBRATION
5. Watch Serial Monitor for alerts
6. Check Dashboard for alert banner
```

**Expected Results**:
```
✅ Serial Monitor shows:
   [ANTI-THEFT] ⚠️ VIBRATION #1! (Count: 1/1)
   [ANTI-THEFT] 🔔 Alert Level 1 - 5 beeps
   [ANTI-THEFT] ✅ Alert sent to dashboard!

✅ Dashboard shows:
   - Orange alert banner appears
   - "ANTI-THEFT ALERT" title
   - "Unauthorized movement detected!" message
   - Severity: LOW badge
   - Sound notification plays
   - Alert added to Recent Alerts list
```

### Test 2: Escalating Alert Levels

**Steps**:
```
1. Ensure system is armed
2. Trigger vibration 3 times (wait 1 second between each)
3. Watch alert level escalate
```

**Expected Results**:
```
Vibration 1:
✅ 5 beeps
✅ Dashboard: Severity LOW (yellow)

Vibration 2:
✅ 8 beeps
✅ Dashboard: Severity MEDIUM (orange)

Vibration 3:
✅ 15 beeps
✅ Dashboard: Severity HIGH (red)
```

### Test 3: Dashboard Alert Dismissal

**Steps**:
```
1. Trigger anti-theft alert
2. Wait for dashboard banner to appear
3. Click "Dismiss" button
4. Verify banner disappears
```

**Expected Results**:
```
✅ Alert banner fades out
✅ Alert remains in Recent Alerts list
✅ Can be triggered again
```

### Test 4: GSM Failure Scenario

**Purpose**: Verify dashboard alerts work even when GSM fails

**Steps**:
```
1. Disconnect GSM module (or ensure no SIM card)
2. Arm anti-theft system
3. Trigger vibration
4. Check Serial Monitor
5. Check Dashboard
```

**Expected Results**:
```
✅ Serial Monitor shows:
   [GSM] ❌ Not ready
   [ANTI-THEFT] ❌ SMS failed
   [ANTI-THEFT] ✅ Alert sent to dashboard!  ← KEY!

✅ Dashboard shows:
   - Alert banner appears (even without SMS)
   - Sound notification plays
   - Alert in Recent Alerts
```

**This proves the backup notification system works!**

### Test 5: Multiple Alerts

**Steps**:
```
1. Trigger alert
2. Dismiss alert
3. Wait 30 seconds
4. Trigger alert again
5. Verify new alert appears
```

**Expected Results**:
```
✅ Each alert has unique timestamp
✅ Multiple alerts in Recent Alerts list
✅ Latest alert at top
✅ Maximum 10 alerts stored
```

## 🔍 Debugging

### Dashboard Not Showing Alerts

**Check**:
```
1. Open browser console (F12)
2. Look for:
   [ANTI-THEFT] Setting up alert listener...
   [ANTI-THEFT] Alert received: {...}

3. Verify Firebase paths:
   /{userId}/antiTheft/status
   /helmet_public/{userId}/theft_alerts
```

**Fix**:
```
- Ensure WiFi connected
- Check Firebase authentication
- Verify userId matches
- Clear browser cache
```

### Arduino Not Sending Alerts

**Check Serial Monitor**:
```
[ANTI-THEFT] ✅ Alert sent to dashboard!  ← Should see this
[ANTI-THEFT] ❌ Dashboard alert failed: HTTP 401  ← Auth error
```

**Fix**:
```
- Check WiFi connection
- Verify firebaseHost URL
- Check firebaseAuth token
- Ensure userUID is correct
```

### Alerts Not Playing Sound

**Check**:
```
1. Browser sound permissions
2. Volume not muted
3. Console for errors
```

**Fix**:
```
- Allow sound in browser settings
- Check playSound() function
- Verify audio file exists
```

## 📊 Firebase Data Verification

### Check Anti-Theft Status

**Firebase Console**:
```
Navigate to:
/{userId}/antiTheft/status

Should see:
{
  "armed": true,
  "alertActive": true,
  "lastVibration": 1700000123456,
  "alertLevel": 3,
  "vibrationCount": 5
}
```

### Check Theft Alerts Log

**Firebase Console**:
```
Navigate to:
/helmet_public/{userId}/theft_alerts

Should see:
{
  "-NxYz123abc": {
    "timestamp": 1700000123456,
    "type": "theft_attempt",
    "message": "Unauthorized movement detected!",
    "severity": "high",
    "alertLevel": 3
  }
}
```

## ✅ Success Criteria

All tests pass when:

```
✅ Vibration triggers buzzer alerts
✅ Dashboard shows alert banner
✅ Sound notification plays
✅ Alert appears in Recent Alerts
✅ Severity levels escalate correctly
✅ Alerts work WITHOUT GSM
✅ Alerts can be dismissed
✅ Multiple alerts tracked
✅ Firebase data updated correctly
✅ No console errors
```

## 🎉 Expected Behavior Summary

### When Vibration Detected:

**Arduino**:
1. Buzzer sounds (5/8/15 beeps based on level)
2. LED flashes
3. Sends data to Firebase (2 paths)
4. Attempts SMS (if GSM available)
5. Logs to Serial Monitor

**Dashboard**:
1. Receives Firebase update (instant)
2. Shows alert banner (animated)
3. Plays sound notification
4. Adds to Recent Alerts list
5. Displays severity badge
6. Allows user dismissal

**Result**: User is notified via **both SMS and Dashboard**, with Dashboard working even if GSM fails!

## 🚀 Production Checklist

Before deploying:

```
□ Test all 5 scenarios above
□ Verify Firebase rules allow writes
□ Check WiFi stability
□ Test on mobile and desktop
□ Verify sound notifications work
□ Test alert dismissal
□ Check alert history limit (10 max)
□ Verify escalating severity levels
□ Test GSM failure scenario
□ Document any issues
```

## 📝 Notes

- **Alert Cooldown**: SMS has 5-minute cooldown, Dashboard has none
- **Alert History**: Maximum 10 alerts stored
- **Severity Reset**: After 30 seconds of no vibration
- **Arming Delay**: 30 seconds after engine stops
- **Debounce**: 200ms between vibration readings

---

**Ready to test!** Follow the procedures above to verify the anti-theft dashboard alert system is working correctly.
