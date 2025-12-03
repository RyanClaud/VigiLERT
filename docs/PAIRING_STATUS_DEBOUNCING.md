# Pairing Status Debouncing System 🔄

## Problem

The pairing status was flickering on and off repeatedly, changing state with every heartbeat update. This created a poor user experience with:
- Status constantly changing
- Multiple alerts firing
- Confusing visual feedback
- Sound playing repeatedly

### Before (Flickering):
```
Heartbeat 1: Connected ✓
  ↓ 2 seconds
Heartbeat 2: Disconnected ✗ (brief network delay)
  ↓ Alert fires! 🔊
Heartbeat 3: Connected ✓
  ↓ 2 seconds
Heartbeat 4: Disconnected ✗
  ↓ Alert fires again! 🔊
```

## Solution

Implemented a **debouncing/stabilization system** that:
1. **Waits 10 seconds** before confirming disconnection
2. **Immediately updates** when device reconnects
3. **Only triggers alerts** after sustained disconnection
4. **Prevents duplicate alerts** with flag system

### After (Stable):
```
Heartbeat 1: Connected ✓
  ↓ 2 seconds
Heartbeat 2: Brief disconnect detected
  ↓ Start 10-second timer ⏱️
  ↓ No alert yet...
Heartbeat 3: Connected ✓
  ↓ Timer cancelled ✓
  ↓ Status remains: Connected
```

## How It Works

### State Variables

```javascript
// Debouncing state
const helmetDisconnectedSince = ref(null);
const motorcycleDisconnectedSince = ref(null);
const DISCONNECT_ALERT_DELAY = 10000; // 10 seconds
let disconnectAlertShown = false;
```

### Logic Flow

#### 1. Device Connected
```javascript
if (isConnected) {
  // Immediately update to connected
  if (!helmetPaired.value) {
    console.log('[PAIRING] Helmet reconnected!');
    helmetPaired.value = true;
  }
  // Clear disconnect timer
  helmetDisconnectedSince.value = null;
}
```

#### 2. Device Disconnected (First Detection)
```javascript
if (!isConnected && helmetPaired.value) {
  if (!helmetDisconnectedSince.value) {
    // Start timer
    helmetDisconnectedSince.value = Date.now();
    console.log('[PAIRING] Helmet disconnect detected, waiting 10s...');
  }
}
```

#### 3. Sustained Disconnection (After 10 Seconds)
```javascript
const disconnectedDuration = Date.now() - helmetDisconnectedSince.value;
if (disconnectedDuration >= DISCONNECT_ALERT_DELAY) {
  // Confirm disconnection
  helmetPaired.value = false;
  console.log('[PAIRING] Helmet sustained disconnection confirmed!');
  
  // Trigger alert (only once)
  if (!disconnectAlertShown) {
    alerts.value.unshift({
      type: 'danger',
      message: '⚠️ Helmet Device Disconnected!',
      details: 'Helmet module has been offline for 10 seconds.',
      time: new Date().toLocaleTimeString()
    });
    playSound();
    disconnectAlertShown = true;
  }
}
```

## Behavior Scenarios

### Scenario 1: Brief Network Glitch
```
Time 0s: Connected ✓
Time 2s: Heartbeat missed (network glitch)
  → Disconnect detected, timer starts
  → Status still shows: Connected ✓
  → No alert
Time 4s: Heartbeat received
  → Timer cancelled
  → Status remains: Connected ✓
  → No alert
```

### Scenario 2: Device Actually Turned Off
```
Time 0s: Connected ✓
Time 2s: Device turned off
  → Disconnect detected, timer starts
  → Status still shows: Connected ✓
  → No alert
Time 4s: Still disconnected
  → Timer continues...
Time 6s: Still disconnected
  → Timer continues...
Time 8s: Still disconnected
  → Timer continues...
Time 10s: Still disconnected
  → Timer expires!
  → Status changes: Disconnected ✗
  → Alert fires! 🔊
Time 12s: Still disconnected
  → Status remains: Disconnected ✗
  → No duplicate alert
```

### Scenario 3: Device Reconnects Before 10 Seconds
```
Time 0s: Connected ✓
Time 2s: Device disconnected
  → Timer starts (0s elapsed)
Time 4s: Still disconnected
  → Timer continues (2s elapsed)
Time 6s: Still disconnected
  → Timer continues (4s elapsed)
Time 8s: Device reconnects!
  → Timer cancelled
  → Status remains: Connected ✓
  → No alert ever fired
```

## Console Output

### Normal Operation (No Flickering):
```
[DEBUG] Helmet device data from Firebase: {status: "On", lastHeartbeat: 1700000006000}
[DEBUG] Helmet: CONNECTED (using status-based check)

[DEBUG] Helmet device data from Firebase: {status: "On", lastHeartbeat: 1700000008000}
[DEBUG] Helmet: CONNECTED (using status-based check)
```

### Brief Disconnect (Ignored):
```
[DEBUG] Helmet device data from Firebase: {status: "Off"}
[DEBUG] Helmet: DISCONNECTED (no data or status Off)
[PAIRING] Helmet disconnect detected, waiting 10s before alert...

[DEBUG] Helmet device data from Firebase: {status: "On", lastHeartbeat: 1700000010000}
[DEBUG] Helmet: CONNECTED (using status-based check)
[PAIRING] Helmet reconnected!
```

### Sustained Disconnect (Alert Triggered):
```
[DEBUG] Helmet device data from Firebase: {status: "Off"}
[DEBUG] Helmet: DISCONNECTED (no data or status Off)
[PAIRING] Helmet disconnect detected, waiting 10s before alert...

... 10 seconds pass ...

[PAIRING] Helmet sustained disconnection confirmed!
[ALERT] Helmet Device Disconnected!
```

## Benefits

### 1. Stable Status Display
- No flickering between Connected/Disconnected
- Status only changes after confirmed disconnection
- Better user experience

### 2. Reduced Alert Spam
- Only one alert per disconnection event
- No duplicate alerts
- Alert only after sustained disconnection

### 3. Network Resilience
- Tolerates brief network glitches
- Doesn't trigger false alarms
- 10-second buffer handles temporary issues

### 4. Immediate Reconnection
- Device reconnection is instant
- No delay when coming back online
- Timer cancelled immediately

## Configuration

### Adjust Disconnect Delay
```javascript
const DISCONNECT_ALERT_DELAY = 10000; // 10 seconds (default)

// For faster alerts (not recommended):
const DISCONNECT_ALERT_DELAY = 5000; // 5 seconds

// For more tolerance:
const DISCONNECT_ALERT_DELAY = 15000; // 15 seconds
```

### Alert Customization
```javascript
alerts.value.unshift({
  type: 'danger',
  message: '⚠️ Helmet Device Disconnected!',
  details: 'Helmet module has been offline for 10 seconds.',
  time: new Date().toLocaleTimeString()
});
```

## Visual Comparison

### Before (Flickering):
```
Pairing Status: Connected ✓
  ↓ 2 seconds
Pairing Status: Disconnected ✗ 🔊
  ↓ 2 seconds
Pairing Status: Connected ✓
  ↓ 2 seconds
Pairing Status: Disconnected ✗ 🔊
  ↓ Confusing and annoying!
```

### After (Stable):
```
Pairing Status: Connected ✓
  ↓ Brief disconnect (< 10s)
Pairing Status: Connected ✓ (no change)
  ↓ Sustained disconnect (> 10s)
Pairing Status: Disconnected ✗ 🔊 (one alert)
  ↓ Remains disconnected
Pairing Status: Disconnected ✗ (no more alerts)
```

## Testing

### Test 1: Brief Disconnect
```
1. Both devices connected
2. Turn off helmet for 5 seconds
3. Turn helmet back on

Expected:
  - Status remains: Connected ✓
  - No alert fires
  - No flickering
```

### Test 2: Sustained Disconnect
```
1. Both devices connected
2. Turn off helmet
3. Wait 15 seconds

Expected:
  - Status shows: Connected for first 10s
  - Status changes: Disconnected after 10s
  - Alert fires once at 10s mark
  - No duplicate alerts
```

### Test 3: Multiple Brief Disconnects
```
1. Both devices connected
2. Turn off helmet for 3 seconds
3. Turn on helmet
4. Wait 5 seconds
5. Turn off helmet for 3 seconds
6. Turn on helmet

Expected:
  - Status remains: Connected throughout
  - No alerts fire
  - Timer resets each time
```

## Summary

The pairing status is now **stable and reliable**:

### Key Features:
- ✅ 10-second debouncing prevents flickering
- ✅ Immediate reconnection response
- ✅ One alert per disconnection event
- ✅ Network glitch tolerance
- ✅ Clear console logging for debugging

### User Experience:
- 🎯 Stable status display
- 🔇 No alert spam
- ⚡ Fast reconnection
- 🛡️ Reliable disconnection detection

The system now provides accurate, stable pairing status without annoying flickering or duplicate alerts! 🎉
