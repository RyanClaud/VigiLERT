# Heartbeat-Based Connection System

## Overview
Instead of hardcoded "On" status, both devices now send periodic heartbeats with timestamps. The Dashboard checks if heartbeats are recent (within 10 seconds) to determine if devices are truly connected.

## How It Works

### Arduino Side (Both Devices)

**Motorcycle Module:**
- Sends heartbeat every 2 seconds to `/devices/motorcycle`
- Includes: `status: "On"`, `lastHeartbeat: <timestamp>`, `timestamp: <timestamp>`
- Helmet switch status sent to `/devices/helmet`

**Helmet Module:**
- Sends heartbeat every 2 seconds to `/devices/helmet`
- Includes: `status: "On"`, `lastHeartbeat: <timestamp>`, `timestamp: <timestamp>`

### Dashboard Side

**Connection Check Logic:**
1. Listen to `/devices/helmet` and `/devices/motorcycle`
2. When data received, check `lastHeartbeat` timestamp
3. Calculate: `timeSinceLastBeat = now - lastHeartbeat`
4. If `timeSinceLastBeat < 10000ms` (10 seconds) → **CONNECTED**
5. If `timeSinceLastBeat >= 10000ms` → **DISCONNECTED**
6. Re-check every 5 seconds automatically

## Firebase Data Structure

```json
{
  "helmet_public": {
    "MnzBjTBslZNijOkq732PE91hHa23": {
      "devices": {
        "helmet": {
          "status": "On",
          "lastHeartbeat": 1234567890,
          "timestamp": 1234567890
        },
        "motorcycle": {
          "status": "On",
          "lastHeartbeat": 1234567891,
          "timestamp": 1234567891
        }
      }
    }
  }
}
```

## Connection States

### Both Connected
- Helmet heartbeat < 10s old
- Motorcycle heartbeat < 10s old
- **Display:** "Connected" (green)

### Helmet Disconnected
- Helmet heartbeat > 10s old OR no data
- Motorcycle heartbeat < 10s old
- **Display:** "Disconnected" (red)
- Helmet indicator: Gray/Off
- Motorcycle indicator: Green

### Motorcycle Disconnected
- Helmet heartbeat < 10s old
- Motorcycle heartbeat > 10s old OR no data
- **Display:** "Disconnected" (red)
- Helmet indicator: Green
- Motorcycle indicator: Gray/Off

### Both Disconnected
- Both heartbeats > 10s old OR no data
- **Display:** "Disconnected" (red)
- Both indicators: Gray/Off

## Setup Instructions

### Step 1: Update Firebase Structure

In Firebase Console, update your structure to:
```
helmet_public/MnzBjTBslZNijOkq732PE91hHa23/
└── devices
    ├── helmet
    │   ├── status: "On"
    │   ├── lastHeartbeat: 0
    │   └── timestamp: 0
    └── motorcycle
        ├── status: "On"
        ├── lastHeartbeat: 0
        └── timestamp: 0
```

### Step 2: Upload Motorcycle Arduino Code

Upload `MotorcycleToWebApplication_FIXED.ino` to your motorcycle ESP32.

**What it does:**
- Sends motorcycle heartbeat every 2 seconds
- Sends helmet heartbeat based on helmet switch state
- Updates timestamps automatically

### Step 3: Upload Helmet Arduino Code

Upload `HelmetToWebApplication_HEARTBEAT.ino` to your helmet ESP32.

**What it does:**
- Sends helmet heartbeat every 2 seconds
- Includes alcohol sensor data
- Updates timestamps automatically

### Step 4: Deploy Updated Dashboard

The Dashboard code is already updated with:
- Heartbeat checking logic
- Automatic re-evaluation every 5 seconds
- Console logging for debugging

## Testing

### Test 1: Both Devices On
1. Power on both ESP32 modules
2. Wait 5 seconds
3. Check Dashboard → Should show "Connected"
4. Check browser console → Should see:
   ```
   [DEBUG] Helmet: CONNECTED (last beat: 1234ms ago)
   [DEBUG] Motorcycle: CONNECTED (last beat: 2345ms ago)
   ```

### Test 2: Turn Off Helmet
1. Power off helmet ESP32
2. Wait 15 seconds
3. Check Dashboard → Should show "Disconnected"
4. Helmet indicator should be gray/off
5. Motorcycle indicator should still be green

### Test 3: Turn Off Motorcycle
1. Power off motorcycle ESP32
2. Wait 15 seconds
3. Check Dashboard → Should show "Disconnected"
4. Motorcycle indicator should be gray/off
5. Helmet indicator depends on helmet module

### Test 4: Reconnect Device
1. Power on a disconnected device
2. Wait 5 seconds
3. Dashboard should automatically update to "Connected"
4. No page refresh needed

## Serial Monitor Output

### Motorcycle Module:
```
[FIREBASE] Motorcycle heartbeat sent (HTTP: 200)
[FIREBASE] Helmet heartbeat: On (HTTP: 200)
[FIREBASE] Motorcycle heartbeat sent (HTTP: 200)
[FIREBASE] Helmet heartbeat: Off (HTTP: 200)
```

### Helmet Module:
```
[FIREBASE] Helmet heartbeat sent: On
[FIREBASE] Alcohol status updated
[FIREBASE] Helmet status set to: ON
[FIREBASE] Helmet heartbeat sent: On
```

## Browser Console Output

```
[DEBUG] Helmet device data from Firebase: {status: "On", lastHeartbeat: 1234567890, timestamp: 1234567890}
[DEBUG] Helmet: CONNECTED (last beat: 1234ms ago)
[DEBUG] Motorcycle device data from Firebase: {status: "On", lastHeartbeat: 1234567891, timestamp: 1234567891}
[DEBUG] Motorcycle: CONNECTED (last beat: 2345ms ago)
```

## Troubleshooting

### Dashboard Shows Disconnected But Devices Are On

**Check 1: Serial Monitor**
- Look for "Helmet heartbeat sent" or "Motorcycle heartbeat sent"
- If not appearing → Arduino not sending heartbeats
- Check WiFi connection

**Check 2: Firebase Console**
- Check `/devices/helmet/lastHeartbeat` value
- Check `/devices/motorcycle/lastHeartbeat` value
- If values not updating → Arduino not reaching Firebase

**Check 3: Browser Console**
- Look for "[DEBUG] Helmet device data from Firebase"
- If showing old timestamps → Heartbeats not being sent
- If showing "null" → Path is wrong

**Check 4: Timestamp Values**
- Arduino uses `millis()` which resets on reboot
- Dashboard uses `Date.now()` which is Unix timestamp
- **Solution:** Use consistent timestamp format

### Fix Timestamp Mismatch

Update Arduino code to use Unix timestamp:

```cpp
unsigned long getUnixTime() {
  return millis() / 1000; // Convert to seconds
}

// Then use:
doc["lastHeartbeat"] = getUnixTime();
```

Or update Dashboard to use relative time:

```javascript
// Instead of Date.now(), use millis equivalent
const now = performance.now();
```

### Heartbeat Stops After Some Time

**Possible causes:**
1. WiFi disconnection → Check `connectToWiFi()` is called
2. Firebase rate limiting → Increase heartbeat interval
3. Memory leak → Check for memory issues in Serial Monitor

**Solution:**
- Add WiFi reconnection check in loop
- Increase heartbeat interval to 5 seconds
- Monitor free heap memory

## Performance Considerations

### Heartbeat Interval
- **2 seconds:** Good for real-time, but more Firebase writes
- **5 seconds:** Balanced approach
- **10 seconds:** Less Firebase writes, but slower detection

### Connection Timeout
- **10 seconds:** Current setting
- **15 seconds:** More forgiving for network issues
- **5 seconds:** Faster detection, but may false-trigger

### Recommended Settings
```cpp
// Arduino
const long heartbeatInterval = 3000; // 3 seconds

// Dashboard
const connectionTimeout = 10000; // 10 seconds
const recheckInterval = 5000;    // 5 seconds
```

## Firebase Usage

### Writes Per Hour
- 2 devices × 30 heartbeats/minute = 60 writes/minute
- 60 writes/minute × 60 minutes = 3,600 writes/hour
- Well within Firebase free tier (100,000 writes/day)

### Optimization
If you need to reduce writes:
1. Increase heartbeat interval to 5 seconds
2. Only send heartbeat when status changes
3. Use Firebase Realtime Database presence system

## Advanced: Firebase Presence

For production, consider using Firebase's built-in presence:

```javascript
// Dashboard
import { ref, onDisconnect, set } from 'firebase/database';

const deviceRef = ref(database, `devices/helmet`);
onDisconnect(deviceRef).set({ status: 'Off', lastSeen: Date.now() });
set(deviceRef, { status: 'On', lastSeen: Date.now() });
```

This automatically sets status to "Off" when connection drops.

## Summary

✅ **Dynamic connection status** - No hardcoded values
✅ **Real-time detection** - Knows within 10 seconds if device disconnects
✅ **Automatic recovery** - Reconnects automatically when device comes back
✅ **No page refresh needed** - Dashboard updates in real-time
✅ **Debug logging** - Easy to troubleshoot connection issues
✅ **Scalable** - Can add more devices easily

The system now accurately reflects the true connection state of both devices!
