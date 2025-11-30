# Helmet Connection Requirement for Engine Start

## New Safety Feature

Added helmet connection detection as a requirement before allowing engine to start. Both helmet and motorcycle must be ON and functional before engine can start.

## How It Works

### Helmet Connection Check
The motorcycle now checks if the helmet is sending heartbeats to Firebase:

```cpp
// Check every 1 second
if (millis() - lastHelmetCheck > HELMET_CHECK_INTERVAL) {
  checkHelmetConnection();
  lastHelmetCheck = millis();
}
```

### Connection Criteria
Helmet is considered connected if:
1. ✅ Status is "On" in Firebase
2. ✅ Heartbeat timestamp is within last 5 seconds
3. ✅ Firebase read is successful

### Engine Start Validation
```cpp
void startEngine() {
  // Check 1: Helmet must be connected
  if (!helmetConnected) {
    ❌ ENGINE START BLOCKED - HELMET NOT CONNECTED!
    return;
  }
  
  // Check 2: No alcohol detected
  if (alcoholDetected) {
    ❌ ENGINE START BLOCKED - ALCOHOL DETECTED!
    return;
  }
  
  // All checks passed
  ✅ Starting engine...
}
```

## Safety Checks Order

| Priority | Check | Block Reason | Beep Pattern |
|----------|-------|--------------|--------------|
| 1 | Helmet Connected | Helmet not ON | 2 short beeps (100ms) |
| 2 | Alcohol Safe | Alcohol detected | 3 long beeps (200ms) |
| 3 | Engine Start | All clear | Engine starts |

## Serial Monitor Output

### Helmet Not Connected
```
[ENGINE] startEngine() called
[ENGINE] helmetConnected = FALSE
[ENGINE] alcoholDetected = FALSE
[ENGINE] engineRunning = FALSE

❌ ENGINE START BLOCKED - HELMET NOT CONNECTED!
💡 TIP: Turn on helmet module first
💡 TIP: Wait for helmet to connect to WiFi and send heartbeat
*beep beep* (2 short beeps)
```

### Helmet Connected, Alcohol Detected
```
[ENGINE] startEngine() called
[ENGINE] helmetConnected = TRUE
[ENGINE] alcoholDetected = TRUE
[ENGINE] engineRunning = FALSE

❌ ENGINE START BLOCKED - ALCOHOL DETECTED!
💡 TIP: Check helmet alcohol sensor readings
💡 TIP: Helmet should send 'Safe' status if value < 600
*beep... beep... beep...* (3 long beeps)
```

### All Checks Passed
```
[ENGINE] startEngine() called
[ENGINE] helmetConnected = TRUE
[ENGINE] alcoholDetected = FALSE
[ENGINE] engineRunning = FALSE

✅ Starting engine...
✅ Relay GPIO 13 = 1 (ON)
```

### Helmet Connection Status
```
[HELMET] ✅ Connected!
[HELMET] ⚠️ Disconnected (heartbeat too old)
[HELMET] ⚠️ Disconnected (status not On)
[HELMET] ⚠️ Disconnected (Firebase read failed)
```

## System Status Display

Type `STATUS` in Serial Monitor:

```
━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
🔍 SYSTEM STATUS
━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
Helmet: CONNECTED ✅
Relay: GPIO 13 = 0 (OFF)
Engine: STOPPED
Crash: NO
Alcohol: NO
Auto Control: OFF
Anti-Theft: DISARMED
WiFi: CONNECTED
━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
```

## Testing Procedure

### Test 1: Helmet OFF, Try to Start Engine
1. Turn OFF helmet module
2. Wait 5 seconds
3. Click "Turn On Engine" on dashboard
4. **Expected:** Engine does NOT start
5. **Expected:** 2 short beeps
6. **Serial Monitor:** "❌ ENGINE START BLOCKED - HELMET NOT CONNECTED!"

### Test 2: Helmet ON, Engine Starts
1. Turn ON helmet module
2. Wait for helmet to connect to WiFi
3. Wait 2 seconds for heartbeat
4. Click "Turn On Engine" on dashboard
5. **Expected:** Engine STARTS
6. **Serial Monitor:** "[HELMET] ✅ Connected!" then "✅ Starting engine..."

### Test 3: Helmet Disconnects While Engine Running
1. Engine is running
2. Turn OFF helmet module
3. **Expected:** Engine continues running (doesn't auto-stop)
4. **Note:** Helmet check only applies to starting, not while running

### Test 4: Serial Command (Bypasses Check)
1. Helmet is OFF
2. Type `START ENGINE` in Serial Monitor
3. **Expected:** Still blocked! (check applies to all start methods)

### Test 5: Physical Key (Bypasses Check)
1. Helmet is OFF
2. Turn physical key ON
3. **Expected:** Still blocked! (check applies to all start methods)

## How Helmet Connection is Detected

### Firebase Path
```
/helmet_public/{userId}/devices/helmet
{
  "status": "On",
  "lastHeartbeat": 1700000123456,
  "timestamp": 1700000123456
}
```

### Detection Logic
```cpp
1. Read helmet heartbeat from Firebase
2. Check if status is "On"
3. Parse timestamp
4. Calculate time difference
5. If difference < 5 seconds → Connected ✅
6. If difference >= 5 seconds → Disconnected ❌
```

### Timing
- **Check interval:** Every 1 second
- **Heartbeat timeout:** 5 seconds
- **Response time:** 1-2 seconds to detect connection/disconnection

## Benefits

### Safety
✅ Ensures rider has helmet ON before starting
✅ Prevents starting without safety equipment
✅ Both devices must be functional

### User Experience
✅ Clear error messages
✅ Different beep patterns for different errors
✅ Real-time connection status
✅ Automatic detection (no manual pairing)

### System Integration
✅ Works with dashboard button
✅ Works with Serial commands
✅ Works with physical key
✅ Works with all start methods

## Troubleshooting

### Helmet Shows Connected but Engine Won't Start
**Check:**
1. Alcohol status (might be blocking)
2. Serial Monitor for exact error message
3. Type `STATUS` to see all system states

### Helmet Won't Connect
**Check:**
1. Helmet WiFi connection
2. Helmet heartbeat sending (check helmet Serial Monitor)
3. Firebase path correct
4. Motorcycle WiFi connection

### False Disconnections
**If helmet disconnects randomly:**
1. Check WiFi signal strength
2. Increase timeout from 5 to 10 seconds:
   ```cpp
   if (timeDiff < 10000) {  // 10 seconds instead of 5
   ```

### Want to Disable This Feature
**To allow engine start without helmet:**
```cpp
// Comment out the helmet check in startEngine()
// if (!helmetConnected) {
//   return;
// }
```

## Configuration

### Adjust Connection Timeout
```cpp
// Current: 5 seconds
if (timeDiff < 5000) {

// More lenient: 10 seconds
if (timeDiff < 10000) {

// Stricter: 3 seconds
if (timeDiff < 3000) {
```

### Adjust Check Interval
```cpp
// Current: Check every 1 second
const unsigned long HELMET_CHECK_INTERVAL = 1000;

// Faster: Check every 500ms
const unsigned long HELMET_CHECK_INTERVAL = 500;

// Slower: Check every 2 seconds
const unsigned long HELMET_CHECK_INTERVAL = 2000;
```

## Summary

**What was added:**
- ✅ Helmet connection detection
- ✅ Engine start blocked if helmet not connected
- ✅ Real-time connection monitoring
- ✅ Clear error messages and beep patterns
- ✅ Status display shows helmet connection

**What still works:**
- ✅ All existing features unchanged
- ✅ Alcohol detection
- ✅ Crash detection
- ✅ Anti-theft system
- ✅ Dashboard button
- ✅ Physical key
- ✅ Serial commands

**Result:**
Both helmet and motorcycle must be ON and functional before engine can start. This ensures rider safety and proper system operation.

Upload the code and test - engine will only start when helmet is connected!
