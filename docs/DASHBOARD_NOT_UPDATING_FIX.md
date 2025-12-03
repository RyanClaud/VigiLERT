# 🔍 Dashboard Not Updating - Troubleshooting Guide

## Problem
Crash detection works on ESP32 (Serial Monitor shows crash), but dashboard doesn't update in real-time.

---

## ✅ Quick Checks

### 1. Check Browser Console
1. Open dashboard in browser
2. Press **F12** to open Developer Tools
3. Click **Console** tab
4. Look for these messages:
   ```
   [INIT] Setting up crash listener on path: /helmet_public/MnzBjTBslZNijOkq732PE91hHa23/crashes
   [CRASH] New crash event received from Firebase: {...}
   [CRASH] ✓ Valid crash event: {...}
   ```

**If you see these messages:** Dashboard is receiving data ✅
**If you DON'T see these messages:** Firebase connection issue ❌

---

### 2. Check Firebase Database

1. Go to Firebase Console: https://console.firebase.google.com
2. Select your project: "vigilance-shield"
3. Click **Realtime Database** in left menu
4. Navigate to: `/helmet_public/MnzBjTBslZNijOkq732PE91hHa23/crashes`
5. Check if crash data is there

**Expected data structure:**
```json
{
  "crashes": {
    "-NxAbCdEfGh": {
      "timestamp": 1700123456789,
      "hasGPS": false,
      "impactStrength": 18.5,
      "roll": 45.2,
      "type": "crash"
    }
  }
}
```

**If data is there:** ESP32 is sending correctly ✅
**If data is NOT there:** ESP32 not sending to Firebase ❌

---

### 3. Check Network Tab

1. Open Developer Tools (F12)
2. Click **Network** tab
3. Filter by "firebase"
4. Trigger a crash
5. Look for POST request to Firebase

**If you see POST with status 200:** Data is being sent ✅
**If you see errors (401, 403, 404):** Firebase permissions issue ❌

---

## 🔧 Common Issues & Fixes

### Issue 1: Firebase Rules Blocking Writes

**Symptom:** ESP32 shows "Firebase response: 401" or "403"

**Cause:** Firebase security rules blocking writes

**Fix:** Update Firebase Rules
```json
{
  "rules": {
    "helmet_public": {
      "$uid": {
        ".read": true,
        ".write": true
      }
    }
  }
}
```

**How to update:**
1. Go to Firebase Console
2. Realtime Database → Rules tab
3. Paste the rules above
4. Click **Publish**

---

### Issue 2: Wrong Firebase Path

**Symptom:** Data goes to wrong location in Firebase

**Cause:** Path mismatch between ESP32 and Dashboard

**Check ESP32 path:**
```cpp
const String crashPath = "/helmet_public/" + userUID + "/crashes.json?auth=" + firebaseAuth;
```

**Check Dashboard path:**
```javascript
const crashRef = dbRef(database, `helmet_public/${userId}/crashes`);
```

**Both should use same userUID:** `MnzBjTBslZNijOkq732PE91hHa23`

---

### Issue 3: Dashboard Not Listening

**Symptom:** Data in Firebase but dashboard doesn't update

**Cause:** Real-time listener not initialized

**Fix:** Check browser console for:
```
[INIT] Setting up crash listener on path: ...
```

If missing, refresh the page or check for JavaScript errors.

---

### Issue 4: Timestamp Format Issue

**Symptom:** Crash appears in Firebase but not on dashboard

**Cause:** Timestamp format mismatch

**ESP32 sends:**
```cpp
unsigned long timestamp = 1700000000000UL + millis();
doc["timestamp"] = timestamp;
```

**Dashboard expects:** Number (milliseconds since epoch)

**Fix:** Already correct in optimized code ✅

---

### Issue 5: Browser Cache

**Symptom:** Old dashboard code running

**Cause:** Browser cached old JavaScript

**Fix:**
1. Press **Ctrl + Shift + R** (hard refresh)
2. Or clear browser cache
3. Or open in Incognito/Private window

---

## 🧪 Testing Steps

### Test 1: Verify ESP32 is Sending

1. Open Serial Monitor
2. Tilt MPU6050 to trigger crash
3. Look for:
   ```
   ⚠️⚠️⚠️ CRASH DETECTED! ⚠️⚠️⚠️
   Crash payload (no GPS):
   {"timestamp":1700123456789,"hasGPS":false,"impactStrength":18.5,"roll":45.2,"type":"crash"}
   Firebase response: 200
   ✓ Crash saved to Firebase!
   ```

**If you see "Firebase response: 200":** ESP32 sending correctly ✅

**If you see other codes:**
- 401/403: Permission denied (fix Firebase rules)
- 404: Wrong URL
- -1: WiFi not connected

---

### Test 2: Verify Firebase Receives Data

1. Open Firebase Console
2. Go to Realtime Database
3. Watch the `/crashes` node
4. Trigger crash on ESP32
5. Data should appear immediately

**If data appears:** Firebase receiving correctly ✅

---

### Test 3: Verify Dashboard Receives Data

1. Open dashboard in browser
2. Open Console (F12)
3. Trigger crash on ESP32
4. Look for console logs:
   ```
   [CRASH] New crash event received from Firebase: {...}
   ```

**If you see logs:** Dashboard receiving correctly ✅

---

### Test 4: Manual Firebase Test

**Add crash manually in Firebase Console:**

1. Go to Firebase Console → Realtime Database
2. Navigate to `/helmet_public/MnzBjTBslZNijOkq732PE91hHa23/crashes`
3. Click **+** to add child
4. Add this data:
   ```json
   {
     "timestamp": 1700123456789,
     "hasGPS": false,
     "impactStrength": 20.0,
     "roll": 45.0,
     "type": "crash"
   }
   ```
5. Check if dashboard updates

**If dashboard updates:** ESP32 → Firebase connection is the issue
**If dashboard doesn't update:** Dashboard → Firebase connection is the issue

---

## 🎯 Most Likely Issues

### 1. WiFi Not Connected (Most Common)

**Check Serial Monitor for:**
```
✅ WiFi OK
```

**If you see:**
```
❌ WiFi FAILED
```

**Fix:** Check WiFi credentials in code:
```cpp
const char* ssid = "DPWH";
const char* password = "12345678900";
```

---

### 2. Firebase Rules Blocking (Second Most Common)

**Check Firebase Console → Database → Rules**

**Should be:**
```json
{
  "rules": {
    "helmet_public": {
      "$uid": {
        ".read": true,
        ".write": true
      }
    }
  }
}
```

---

### 3. Browser Not Refreshing

**Fix:** Hard refresh with **Ctrl + Shift + R**

---

## 📊 Diagnostic Checklist

Run through this checklist:

- [ ] ESP32 connected to WiFi (Serial shows "✅ WiFi OK")
- [ ] Crash detected (Serial shows "⚠️ CRASH DETECTED!")
- [ ] Firebase response 200 (Serial shows "Firebase response: 200")
- [ ] Data appears in Firebase Console
- [ ] Dashboard open in browser
- [ ] Browser console open (F12)
- [ ] Console shows "[INIT] Setting up crash listener"
- [ ] Console shows "[CRASH] New crash event received"
- [ ] Dashboard displays crash marker/alert

**Find where the chain breaks and fix that step!**

---

## 🔧 Quick Fixes

### Fix 1: Restart Everything
```
1. Close dashboard browser tab
2. Reset ESP32 (press reset button)
3. Wait for "✅ WiFi OK"
4. Open dashboard in new tab
5. Open console (F12)
6. Trigger crash
7. Watch console for logs
```

### Fix 2: Check Firebase Rules
```
1. Firebase Console → Realtime Database → Rules
2. Set to:
   {
     "rules": {
       "helmet_public": {
         "$uid": {
           ".read": true,
           ".write": true
         }
       }
     }
   }
3. Publish
4. Test again
```

### Fix 3: Hard Refresh Dashboard
```
1. Press Ctrl + Shift + R
2. Or Ctrl + F5
3. Or clear cache and reload
```

---

## 💡 Pro Tips

### Tip 1: Keep Console Open
Always keep browser console open when testing:
- Press F12
- Click Console tab
- Watch for real-time logs

### Tip 2: Use Two Monitors
- Monitor 1: Serial Monitor (ESP32)
- Monitor 2: Dashboard + Console (Browser)
- See both sides of the communication

### Tip 3: Check Timestamps
If crash appears but seems old:
- Check timestamp in Firebase
- Compare with current time
- Dashboard might be filtering old crashes

### Tip 4: Test with Manual Data
Add crash manually in Firebase Console to isolate:
- If manual works → ESP32 issue
- If manual doesn't work → Dashboard issue

---

## 🎯 Expected Flow

**Normal operation:**
```
1. ESP32: Tilt MPU6050
   ↓
2. ESP32: Detect crash (accel > 15g or roll > 40°)
   ↓
3. ESP32: Send to Firebase (POST request)
   ↓
4. Firebase: Store crash data
   ↓
5. Firebase: Notify dashboard (real-time listener)
   ↓
6. Dashboard: Receive crash event
   ↓
7. Dashboard: Display crash marker + alert
```

**Find where this breaks!**

---

## 📞 Next Steps

1. **Open browser console** (F12)
2. **Trigger crash** on ESP32
3. **Watch Serial Monitor** - Should show "Firebase response: 200"
4. **Watch Browser Console** - Should show "[CRASH] New crash event received"
5. **Report back** which step fails

This will tell us exactly where the problem is!
