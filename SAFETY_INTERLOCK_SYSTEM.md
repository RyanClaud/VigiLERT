# Safety Interlock System

## Overview
The VigiLERT system now includes a **Safety Interlock** that prevents the engine from starting unless BOTH devices are active and the helmet is worn.

## Safety Requirements

### Engine Can Start ONLY When:
1. ✅ **Motorcycle Module is ON** (this device is running)
2. ✅ **Helmet Module is ON** (sending heartbeats to Firebase)
3. ✅ **Helmet Switch is ON** (helmet is physically worn)

### If ANY requirement is missing:
- ❌ Engine start is **BLOCKED**
- 🔊 Warning beeps sound
- 💡 LED flashes
- 📱 SMS sent explaining why engine cannot start

## How It Works

### Step 1: Motorcycle Module Checks Helmet Module
Every 5 seconds, the motorcycle module checks Firebase:
```
/helmet_public/{userId}/devices/helmet
```

It looks for:
- `status: "On"`
- `lastHeartbeat: <recent timestamp>`

If heartbeat is older than 15 seconds → Helmet module is INACTIVE

### Step 2: Engine Start Request
When user tries to start engine (via SMS, call, or button):

```
[SAFETY CHECK] Engine Start Request:
  ✓ Motorcycle Module: ON
  ✓ Helmet Module: ON
  ✓ Helmet Switch: ON
  → ALL CHECKS PASSED! Engine can start.
```

OR

```
[SAFETY CHECK] Engine Start Request:
  ✓ Motorcycle Module: ON
  ✗ Helmet Module: OFF
  ✗ Helmet Switch: OFF
  → SAFETY INTERLOCK ACTIVE! Engine cannot start.
  → Helmet module is not active. Turn on helmet device!
  → Helmet switch is OFF. Put on helmet!
```

### Step 3: SMS Notification
If engine start is blocked, user receives SMS:
```
Engine start DENIED! Helmet module not active. Helmet not worn. Safety first!
```

If engine starts successfully:
```
Engine Started. All safety checks passed.
```

## Serial Monitor Output

### Normal Operation:
```
[SENSOR] Accel: 9.81 | Roll: 2.1° | Battery: 3.7V | Helmet: ON | GPS: OK | Speed: 0.0 km/h | Helmet Module: ACTIVE
[SAFETY] Helmet module is ACTIVE
```

### Engine Start Attempt (Success):
```
[SAFETY CHECK] Engine Start Request:
  ✓ Motorcycle Module: ON
  ✓ Helmet Module: ON
  ✓ Helmet Switch: ON
  → ALL CHECKS PASSED! Engine can start.
[ENGINE] Started successfully!
```

### Engine Start Attempt (Blocked):
```
[SAFETY CHECK] Engine Start Request:
  ✓ Motorcycle Module: ON
  ✗ Helmet Module: OFF
  ✓ Helmet Switch: ON
  → SAFETY INTERLOCK ACTIVE! Engine cannot start.
  → Helmet module is not active. Turn on helmet device!
[SAFETY] Engine start BLOCKED by safety interlock!
```

### Helmet Removed While Engine Running:
```
[HELMET] Helmet REMOVED
[SAFETY] Helmet removed - stopping engine!
[ENGINE] Stopped.
```

## SMS Commands

### START Command:
```
SMS: 1234 START
```

**If all checks pass:**
```
Reply: Engine Started. All safety checks passed.
```

**If checks fail:**
```
Reply: Engine start DENIED! Helmet module not active. Safety first!
```

### STATUS Command (NEW):
```
SMS: 1234 STATUS
```

**Reply:**
```
Helmet Module: ON | Helmet Switch: ON | Engine: STOPPED
```

OR

```
Helmet Module: OFF | Helmet Switch: OFF | Engine: STOPPED
```

### Other Commands:
- `1234 STOP` - Stop engine (no safety check needed)
- `1234 LOCATE` - Get GPS location

## Testing

### Test 1: Both Devices ON
1. Turn on helmet module (upload HelmetToWebApplication_HEARTBEAT.ino)
2. Turn on motorcycle module (upload MotorcycleToWebApplication_SAFETY.ino)
3. Put helmet on (close helmet switch)
4. Send SMS: `1234 START`
5. **Expected:** Engine starts, SMS: "Engine Started. All safety checks passed."

### Test 2: Helmet Module OFF
1. Turn OFF helmet module (unplug or stop code)
2. Put helmet on
3. Send SMS: `1234 START`
4. **Expected:** Engine does NOT start, SMS: "Engine start DENIED! Helmet module not active..."

### Test 3: Helmet Switch OFF
1. Turn on helmet module
2. Remove helmet (open helmet switch)
3. Send SMS: `1234 START`
4. **Expected:** Engine does NOT start, SMS: "Engine start DENIED! Helmet not worn..."

### Test 4: Helmet Removed While Running
1. Start engine with all checks passing
2. Remove helmet while engine is running
3. **Expected:** Engine automatically stops, Serial: "[SAFETY] Helmet removed - stopping engine!"

## Configuration

### Adjust Helmet Module Check Interval:
```cpp
const unsigned long HELMET_CHECK_INTERVAL = 5000; // Check every 5 seconds
```

Change to:
- `3000` - Check every 3 seconds (more responsive)
- `10000` - Check every 10 seconds (less Firebase reads)

### Adjust Heartbeat Timeout:
```cpp
if (status == "On" && timeSinceHeartbeat < 15000) {
  // Helmet module is active
}
```

Change `15000` to:
- `10000` - Stricter (10 second timeout)
- `20000` - More forgiving (20 second timeout)

## Safety Features

### Feature 1: Automatic Engine Stop
If helmet is removed while engine is running, engine automatically stops.

### Feature 2: Visual/Audio Warning
If engine start is blocked:
- LED flashes 3 times
- Buzzer beeps 3 times at 2000 Hz
- Clear indication that safety interlock is active

### Feature 3: SMS Notifications
User receives detailed SMS explaining why engine cannot start:
- "Helmet module not active" - Turn on helmet device
- "Helmet not worn" - Put on helmet
- "Safety first!" - Reminder of safety importance

### Feature 4: Status Check
New SMS command `STATUS` allows user to check:
- Is helmet module active?
- Is helmet switch on?
- Is engine running?

## Firebase Integration

### Motorcycle Module Writes:
```json
{
  "devices": {
    "motorcycle": {
      "status": "On",
      "lastHeartbeat": 1732248246000,
      "timestamp": 1732248246000
    }
  }
}
```

### Motorcycle Module Reads:
```json
{
  "devices": {
    "helmet": {
      "status": "On",
      "lastHeartbeat": 1732248247000,
      "timestamp": 1732248247000
    }
  }
}
```

### Check Logic:
```cpp
if (helmet.status == "On" && (now - helmet.lastHeartbeat) < 15000) {
  // Helmet module is ACTIVE
} else {
  // Helmet module is INACTIVE
}
```

## Troubleshooting

### Issue: Engine Won't Start Even With Helmet On

**Check 1: Is helmet module running?**
- Check if helmet ESP32 is powered on
- Check Serial Monitor of helmet module for heartbeat messages
- Check Firebase Console for `/devices/helmet/lastHeartbeat`

**Check 2: Is helmet switch working?**
- Check Serial Monitor: Should show "Helmet: ON"
- Test helmet switch with multimeter
- Check wiring to GPIO 39

**Check 3: Is motorcycle module checking Firebase?**
- Look for "[SAFETY] Helmet module is ACTIVE" in Serial Monitor
- If not appearing, check WiFi connection
- Check Firebase URL and auth token

### Issue: Engine Starts Without Helmet

This should NOT happen! If it does:
- Check `canStartEngine()` function
- Verify helmet switch is connected to GPIO 39
- Check if INPUT_PULLUP is set correctly

### Issue: Engine Stops Randomly

**Possible causes:**
1. Helmet switch is loose - Check wiring
2. Helmet module loses connection - Check WiFi
3. Firebase heartbeat timeout - Increase timeout value

## Summary

✅ **Safety First:** Engine cannot start without helmet
✅ **Dual Check:** Both devices must be active
✅ **Automatic Stop:** Engine stops if helmet removed
✅ **Clear Feedback:** SMS and Serial messages explain status
✅ **Easy Testing:** Use STATUS command to check system
✅ **Configurable:** Adjust timeouts and intervals as needed

This safety system ensures riders ALWAYS wear their helmet before starting the motorcycle!
