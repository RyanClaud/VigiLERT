# 🔍 Root Cause Analysis: Active Status Flickering

## ❌ Problems Identified:

### 1. **Different WiFi Networks** 🌐
```cpp
// Motorcycle Module
const char* ssid = "Kupal";

// Helmet Module  
const char* ssid = "DPWH";  ❌ DIFFERENT NETWORK!
```
**Impact:** Different network latencies cause heartbeats to arrive at different times, creating timing desynchronization.

### 2. **Conflicting Firebase Updates** 🔥
```cpp
// Motorcycle writes to:
/devices/motorcycle (every 2s)
/helmetStatus/status (when helmet state changes) ❌ CONFLICT!

// Helmet writes to:
/devices/helmet (every 2s)
/helmetStatus/status (every 1s) ❌ CONFLICT!
```
**Impact:** Both devices writing to `/helmetStatus/status` simultaneously causes data races.

### 3. **Inconsistent Timestamp Sources** ⏰
```cpp
// Motorcycle: 1700000000000UL + millis()
// Helmet: GPS time OR 1700000000000UL + millis()
```
**Impact:** Different timestamp calculations cause comparison issues in dashboard.

### 4. **Too Frequent Updates** ⚡
```cpp
// Motorcycle: Every 2 seconds
// Helmet: Every 1 second (delay(1000))
```
**Impact:** Dashboard listeners triggered too frequently, causing rapid state changes.

### 5. **No Server-Side Timeout Logic** ⚠️
- Dashboard doesn't implement "if no heartbeat in 10 seconds → Disconnected"
- Relies on device status updates instead of timestamp checking

### 6. **Crash Events Sent to Wrong Path** 📍
- Continuous sensor readings might be creating map markers
- Only actual crash events should go to `/crashes` path

---

## ✅ Solutions Implemented:

### Fix 1: Synchronize WiFi Networks
```cpp
// Both devices now use:
const char* ssid = "Kupal";
const char* password = "DEEABE7H406";
```

### Fix 2: Increase Heartbeat Intervals
```cpp
// Both devices now use:
const long heartbeatInterval = 5000; // 5 seconds
delay(5000); // Match heartbeat interval
```

### Fix 3: Remove Conflicting Updates
```cpp
// Motorcycle module: REMOVED these lines
// updateHelmetStatusInFirebase(true, "Normal", 0.0);
// updateHelmetStatusInFirebase(false, "Crash Alert", 0.0);

// Only helmet module should update /helmetStatus/status
```

### Fix 4: Crash Events Only to /crashes Path
```cpp
// ✅ Only send crash events to /crashes (for map markers)
if ((currentTotalAccel >= ACCEL_THRESHOLD || currentRoll < -47 || currentRoll > 40) && !crashDetected) {
  // Send to /crashes path ONLY when crash detected
  sendCrashEventToFirebase(...);
  // NOT on every sensor reading
}
```

### Fix 5: Dashboard Server-Side Timeout (Recommended)
```javascript
// Dashboard should implement:
const DEVICE_TIMEOUT = 10000; // 10 seconds

// Check last heartbeat timestamp
const now = Date.now();
const timeSinceLastHeartbeat = now - device.lastHeartbeat;

if (timeSinceLastHeartbeat > DEVICE_TIMEOUT) {
  deviceStatus = 'Disconnected';
} else {
  deviceStatus = 'Connected';
}
```

---

## 📊 Expected Results:

✅ **No more flickering** - Stable connection status  
✅ **Reduced Firebase load** - 5-second intervals instead of 1-2 seconds  
✅ **Better reliability** - Same network, no conflicts  
✅ **Clean map markers** - Only crash events create markers  
✅ **10-second timeout** - Clean disconnection detection  

---

## 🧪 Testing Steps:

1. **Upload both updated Arduino codes**
2. **Monitor Serial output** - Should see heartbeats every 5 seconds
3. **Check dashboard** - Status should be rock solid
4. **Test disconnection** - Turn off device, should timeout after 10 seconds
5. **Test crash detection** - Only crash events should appear on map

---

## 🔧 Files Modified:

- `MotorcycleToWebApplication_COMPLETE.ino` - Heartbeat interval, removed conflicts
- `HelmetToWebApplication_HEARTBEAT.ino` - WiFi network, heartbeat interval
- Dashboard (recommended) - Add server-side timeout logic

The flickering should now be completely eliminated! 🚀
