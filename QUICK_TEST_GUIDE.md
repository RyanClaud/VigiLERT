# Quick Test Guide 🧪

## Step-by-Step Testing Instructions

### 1️⃣ Upload Updated Helmet Code

1. Open Arduino IDE
2. Load `HelmetToWebApplication_HEARTBEAT.ino`
3. Select your ESP32 board
4. Upload the code
5. Open Serial Monitor (115200 baud)

**Expected Output:**
```
✅ Connected to WiFi

[ALCOHOL] Sensor Value: 977 | Threshold: 2000 | Status: SAFE ✓
[FIREBASE] Alcohol status updated
[FIREBASE] Helmet status set to: ON
[FIREBASE] Helmet heartbeat sent: On at 3488166772
```

### 2️⃣ Test Dashboard Updates

1. Open your web browser
2. Navigate to your dashboard
3. Press F12 to open Developer Console
4. Go to "Console" tab

**Expected Output (when safe):**
```
━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
[10:30:00 AM] 🍺 ALCOHOL LISTENER TRIGGERED
[DEBUG] Raw Firebase Data: {
  "sensorValue": 977,
  "status": "Safe",
  "timestamp": 829157
}
[ALCOHOL] Sensor Value: 977
[ALCOHOL] Threshold: 2000
[ALCOHOL] Status: Safe
✓ Alcohol status: SAFE
[UPDATE] alcoholStatus set to: Safe
[UPDATE] alcoholSubtitle set to: No alcohol detected (Value: 977)
[SUCCESS] Alcohol card should now update on dashboard
━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
```

### 3️⃣ Test Alcohol Detection

1. Expose MQ3 sensor to alcohol (hand sanitizer, rubbing alcohol, etc.)
2. Wait for sensor value to rise above 2000
3. Watch Serial Monitor and Dashboard Console

**Expected Serial Monitor:**
```
[ALCOHOL] Sensor Value: 3071 | Threshold: 2000 | Status: DANGER ⚠️
   ⚠️ ALCOHOL DETECTED! Value 3071 exceeds threshold 2000
[FIREBASE] Alcohol status updated
```

**Expected Dashboard Console:**
```
━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
[10:30:45 AM] 🍺 ALCOHOL LISTENER TRIGGERED
[DEBUG] Raw Firebase Data: {
  "sensorValue": 3071,
  "status": "Danger",
  "timestamp": 338375
}
[ALCOHOL] Sensor Value: 3071
[ALCOHOL] Threshold: 2000
[ALCOHOL] Status: Danger
[ALCOHOL] Is Danger: true
🚨🚨🚨 ALCOHOL DANGER DETECTED! 🚨🚨🚨
   Value 3071 exceeds threshold 2000
[UPDATE] alcoholStatus set to: Danger
[UPDATE] alcoholSubtitle set to: Alcohol Detected! Value: 3071
[ALERT] Alert added to dashboard
[SUCCESS] Alcohol card should now update on dashboard
━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
```

**Expected Dashboard Changes:**
- ✅ Alcohol Detection card turns RED
- ✅ Text changes to "Danger"
- ✅ Shows "Alcohol Detected! Value: 3071"
- ✅ Alert notification appears at top
- ✅ Sound plays (beep)

### 4️⃣ Verify Firebase Data

1. Open Firebase Console
2. Navigate to Realtime Database
3. Go to: `helmet_public/{yourUID}/alcohol/status`

**Expected Data:**
```json
{
  "sensorValue": 3071,
  "status": "Danger",
  "timestamp": 338375
}
```

---

## Troubleshooting Checklist

### ❌ If Serial Monitor shows no threshold:
- [ ] Did you upload the updated `.ino` file?
- [ ] Is Serial Monitor set to 115200 baud?
- [ ] Try pressing the ESP32 reset button

### ❌ If Dashboard doesn't update:
- [ ] Is browser console open (F12)?
- [ ] Do you see alcohol listener logs?
- [ ] Try hard refresh: Ctrl+Shift+R (Windows) or Cmd+Shift+R (Mac)
- [ ] Check if Firebase path is correct
- [ ] Verify userUID matches in both hardware and dashboard

### ❌ If no data in Firebase:
- [ ] Is ESP32 connected to WiFi?
- [ ] Check WiFi credentials in code
- [ ] Verify Firebase URL is correct
- [ ] Check Firebase security rules

### ❌ If card shows wrong status:
- [ ] Clear browser cache
- [ ] Check console for errors
- [ ] Verify both listeners are working (check logs)
- [ ] Make sure no browser extensions are blocking updates

---

## Quick Commands

### Arduino Serial Monitor
```
Baud Rate: 115200
Line Ending: Both NL & CR
```

### Browser Console Filters
```javascript
// Show only alcohol logs
console.log filter: "ALCOHOL"

// Show all Firebase logs
console.log filter: "FIREBASE"

// Show alerts
console.log filter: "ALERT"
```

### Firebase Console Path
```
https://console.firebase.google.com/project/vigilance-shield/database/vigilance-shield-default-rtdb/data/helmet_public/MnzBjTBslZNijOkq732PE91hHa23/alcohol/status
```

---

## Success Criteria ✅

Your system is working correctly if:

1. ✅ Serial Monitor shows threshold value (2000)
2. ✅ Serial Monitor shows "DANGER ⚠️" when value > 2000
3. ✅ Firebase updates with correct sensorValue and status
4. ✅ Dashboard console shows detailed alcohol listener logs
5. ✅ Dashboard card changes color (Blue → Red)
6. ✅ Dashboard card shows correct status and value
7. ✅ Alert notification appears
8. ✅ Sound plays on detection

---

## Test Scenarios

### Scenario 1: Normal Operation (No Alcohol)
```
Sensor Value: 500-1500
Expected: Card shows "Safe" (Blue)
```

### Scenario 2: Near Threshold
```
Sensor Value: 1800-1999
Expected: Card shows "Safe" (Blue)
```

### Scenario 3: Just Above Threshold
```
Sensor Value: 2001-2500
Expected: Card shows "Danger" (Red) + Alert
```

### Scenario 4: High Alcohol
```
Sensor Value: 3000+
Expected: Card shows "Danger" (Red) + Alert + Sound
```

---

## Performance Metrics

- **Update Latency:** < 1 second from sensor to dashboard
- **Firebase Write:** Every 1 second (helmet module)
- **Dashboard Refresh:** Real-time (Firebase onValue listener)
- **Alert Response:** Immediate (< 100ms)

---

## Next Steps

After confirming everything works:

1. ✅ Test with actual motorcycle ride
2. ✅ Verify all other sensors (GPS, MPU6050, etc.)
3. ✅ Test crash detection
4. ✅ Test helmet removal warning
5. ✅ Monitor system stability over time

---

## Support

If you encounter issues:

1. Check `REALTIME_DATA_FIX.md` for detailed explanation
2. Review `BEFORE_AFTER_COMPARISON.md` for code changes
3. Verify Firebase data structure
4. Check browser console for errors
5. Review Serial Monitor output

**All sensor data should now reflect in real-time on your dashboard!** 🎉
