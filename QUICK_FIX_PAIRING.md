# Quick Fix for Pairing Status

## Problem
Your Firebase shows `helmetConnected: false` but the Dashboard needs data at:
- `/helmet_public/{userId}/devices/helmet/status` = "On" or "Off"
- `/helmet_public/{userId}/devices/motorcycle/status` = "On" or "Off"

## IMMEDIATE FIX - Manual Firebase Update

### Option 1: Firebase Console (Fastest)
1. Go to Firebase Console: https://console.firebase.google.com
2. Open Realtime Database
3. Navigate to: `helmet_public/MnzBjTBslZNijOkq732PE91hHa23`
4. Click the `+` button next to your user ID
5. Add new child: `devices`
6. Under `devices`, add:
   - Child: `helmet` → Child: `status` → Value: `"On"` (with quotes)
   - Child: `motorcycle` → Child: `status` → Value: `"On"` (with quotes)

Your structure should look like:
```
helmet_public
└── MnzBjTBslZNijOkq732PE91hHa23
    ├── devices
    │   ├── helmet
    │   │   └── status: "On"
    │   └── motorcycle
    │       └── status: "On"
    ├── helmetStatus
    │   └── status
    │       └── helmetConnected: true
    └── live
        └── ...
```

7. Refresh your dashboard → Should show "Connected"!

### Option 2: REST API (Quick Test)
Open a new browser tab and paste these URLs (they will create the data):

**Set Helmet to On:**
```
https://vigilance-shield-default-rtdb.firebaseio.com/helmet_public/MnzBjTBslZNijOkq732PE91hHa23/devices/helmet/status.json?auth=
```
Body: `"On"`

**Set Motorcycle to On:**
```
https://vigilance-shield-default-rtdb.firebaseio.com/helmet_public/MnzBjTBslZNijOkq732PE91hHa23/devices/motorcycle/status.json?auth=
```
Body: `"On"`

## PERMANENT FIX - Arduino Code

### For Motorcycle Arduino

Add this function BEFORE `handleHelmetState()`:

```cpp
void updateHelmetDeviceStatus(bool isOn) {
  HTTPClient http;
  String path = "/helmet_public/" + userUID + "/devices/helmet/status.json?auth=" + firebaseAuth;
  http.begin(firebaseHost + path);
  http.addHeader("Content-Type", "application/json");
  String status = isOn ? "\"On\"" : "\"Off\"";
  int code = http.PUT(status);
  Serial.printf("[FIREBASE] Helmet device status: %s (HTTP: %d)\n", isOn ? "On" : "Off", code);
  http.end();
}

void updateMotorcycleDeviceStatus() {
  HTTPClient http;
  String path = "/helmet_public/" + userUID + "/devices/motorcycle/status.json?auth=" + firebaseAuth;
  http.begin(firebaseHost + path);
  http.addHeader("Content-Type", "application/json");
  int code = http.PUT("\"On\""); // Motorcycle is always ON when code is running
  Serial.printf("[FIREBASE] Motorcycle device status: On (HTTP: %d)\n", code);
  http.end();
}
```

Then in `setup()`, add after `connectToWiFi()`:

```cpp
// Initialize device pairing status
updateMotorcycleDeviceStatus();
updateHelmetDeviceStatus(false); // Helmet starts as OFF
```

Then in `handleHelmetState()`, add these calls:

```cpp
void handleHelmetState(bool currentHelmetState, float voltage, bool h, bool t, bool l, bool r) {
  if (currentHelmetState && !isHelmetOn) {
    startNewTrip();
    isHelmetOn = true;
    crashDetected = false;
    updateHelmetStatusInFirebase(true, "Normal", 0.0);
    updateHelmetDeviceStatus(true);      // ← ADD THIS
    updateMotorcycleDeviceStatus();      // ← ADD THIS
  }
  
  if (!currentHelmetState && isHelmetOn) {
    unsigned long endTime = millis();
    endCurrentTrip(...);
    isHelmetOn = false;
    lastHelmetOffMillis = millis();
    crashDetected = false;
    updateHelmetStatusInFirebase(false, "Normal", 0.0);
    updateHelmetDeviceStatus(false);     // ← ADD THIS
    updateMotorcycleDeviceStatus();      // ← ADD THIS
  }
}
```

### For Helmet Arduino (Optional - if you want helmet module to also update)

Add this function:

```cpp
void updateHelmetDeviceStatusFromHelmet(bool isOn) {
  HTTPClient http;
  String url = firebaseHost + "/helmet_public/" + userUID + "/devices/helmet/status.json?auth=" + firebaseAuth;
  http.begin(url);
  http.addHeader("Content-Type", "application/json");
  String status = isOn ? "\"On\"" : "\"Off\"";
  int code = http.PUT(status);
  Serial.printf("[FIREBASE] Helmet device status: %s (HTTP: %d)\n", isOn ? "On" : "Off", code);
  http.end();
}
```

Then in `setup()`, add:

```cpp
updateHelmetDeviceStatusFromHelmet(true); // Helmet module is ON
```

And in `loop()`, replace:

```cpp
sendHelmetStatusToFirebase(true); // Always ON while running
```

With:

```cpp
sendHelmetStatusToFirebase(true);
updateHelmetDeviceStatusFromHelmet(true); // Update device pairing status
```

## Testing

### Step 1: Upload Arduino Code
Upload the updated code to your ESP32

### Step 2: Check Serial Monitor
You should see:
```
[FIREBASE] Motorcycle device status: On (HTTP: 200)
[FIREBASE] Helmet device status: Off (HTTP: 200)
```

When you put helmet on:
```
[HELMET] Helmet PUT ON - Starting trip
[FIREBASE] Helmet device status: On (HTTP: 200)
```

### Step 3: Check Firebase
Open Firebase Console and verify:
```
devices
├── helmet
│   └── status: "On"
└── motorcycle
    └── status: "On"
```

### Step 4: Check Dashboard
- Refresh dashboard
- Pairing Status should show "Connected" (green)
- Both helmet and motorcycle indicators should be green

## Troubleshooting

### Still Shows Disconnected?

1. **Check Firebase Console**
   - Does `/devices/helmet/status` exist?
   - Does `/devices/motorcycle/status` exist?
   - Are values exactly `"On"` (with quotes)?

2. **Check Serial Monitor**
   - Look for HTTP 200 responses
   - If HTTP 401 → Auth error
   - If HTTP 404 → Path error

3. **Check Browser Console**
   - Press F12
   - Look for: `[DEBUG] Helmet status from Firebase: "On"`
   - Look for: `[DEBUG] Motorcycle status from Firebase: "On"`

4. **Hard Refresh Dashboard**
   - Press Ctrl+Shift+R (Windows)
   - Or Cmd+Shift+R (Mac)

5. **Clear Browser Cache**
   - Settings → Privacy → Clear browsing data
   - Check "Cached images and files"
   - Clear data

### Values Are "On" But Still Disconnected?

Check Dashboard code is looking at the right path:
```javascript
const helmetStatusRef = dbRef(database, `helmet_public/${userId}/devices/helmet/status`);
const motorcycleStatusRef = dbRef(database, `helmet_public/${userId}/devices/motorcycle/status`);
```

## Expected Result

After fix:
- ✅ Pairing Status: **Connected** (green background)
- ✅ Helmet indicator: Green dot + "Helmet"
- ✅ Motorcycle indicator: Green dot + "Motorcycle"
- ✅ Firebase has `/devices/helmet/status` = "On"
- ✅ Firebase has `/devices/motorcycle/status` = "On"

## Quick Test Command

If you want to test immediately without uploading code, run this in browser console on your dashboard:

```javascript
// Import Firebase functions
import { ref as dbRef, set } from 'firebase/database';
import { database } from './src/firebase/config';

const userId = 'MnzBjTBslZNijOkq732PE91hHa23';

// Set both devices to On
await set(dbRef(database, `helmet_public/${userId}/devices/helmet/status`), 'On');
await set(dbRef(database, `helmet_public/${userId}/devices/motorcycle/status`), 'On');

console.log('✅ Both devices set to On');
```

Or use this simpler version (paste in console):

```javascript
fetch('https://vigilance-shield-default-rtdb.firebaseio.com/helmet_public/MnzBjTBslZNijOkq732PE91hHa23/devices/helmet/status.json', {
  method: 'PUT',
  body: JSON.stringify('On')
});

fetch('https://vigilance-shield-default-rtdb.firebaseio.com/helmet_public/MnzBjTBslZNijOkq732PE91hHa23/devices/motorcycle/status.json', {
  method: 'PUT',
  body: JSON.stringify('On')
});

console.log('✅ Sent both updates');
```

Then refresh the page!
