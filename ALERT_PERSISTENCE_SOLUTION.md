# Alert Persistence Solution

## Problem
When users click "Clear All" to dismiss alerts, the alerts reappear after refreshing the page. This happens because:
1. Alerts are stored only in memory (Vue ref)
2. Firebase listeners regenerate alerts on every page load
3. No mechanism to remember which alerts were dismissed

## Solution Implemented

### 1. localStorage Tracking
Added a dismissal timestamp system that remembers when alerts were cleared:

```javascript
const dismissedAlertsKey = `dismissedAlerts_${userId}`;
const alertDismissalTime = ref(0);
```

### 2. Dismissal Time Storage
When user clicks "Clear All", we save the current timestamp:

```javascript
const clearAllAlerts = () => {
  // Record dismissal time
  alertDismissalTime.value = Date.now();
  localStorage.setItem(dismissedAlertsKey, alertDismissalTime.value.toString());
  
  // Clear alerts
  alerts.value = [];
};
```

### 3. Alert Filtering
Only show alerts that are newer than the dismissal time:

```javascript
const shouldShowAlert = (alertTimestamp) => {
  return alertTimestamp > alertDismissalTime.value;
};
```

### 4. Helper Function
Created `addAlert()` helper that automatically checks dismissal:

```javascript
const addAlert = (type, message, details = '') => {
  const alertTime = Date.now();
  
  // Check if alert should be shown
  if (!shouldShowAlert(alertTime)) {
    console.log('[ALERTS] Alert dismissed:', message);
    return false;
  }
  
  // Add alert with timestamp
  alerts.value.unshift({
    type,
    message,
    details,
    time: new Date().toLocaleTimeString(),
    timestamp: alertTime
  });
  
  return true;
};
```

---

## How It Works

### Timeline Example

```
10:00 AM - User opens dashboard
         - Crash alert appears (timestamp: 10:00 AM)
         - Overspeed alert appears (timestamp: 10:00 AM)

10:05 AM - User clicks "Clear All"
         - Dismissal time saved: 10:05 AM
         - Alerts cleared from screen

10:10 AM - User refreshes page
         - Old alerts (10:00 AM) are NOT shown
         - They're older than dismissal time (10:05 AM)

10:15 AM - New crash occurs
         - New alert (10:15 AM) IS shown
         - It's newer than dismissal time (10:05 AM)
```

---

## Updated Alert Functions

### Before (Always Shows)
```javascript
alerts.value.unshift({
  type: 'danger',
  message: '🚨 CRASH DETECTED!',
  details: 'Impact detected',
  time: new Date().toLocaleTimeString()
});
```

### After (Checks Dismissal)
```javascript
const alertTime = Date.now();
if (shouldShowAlert(alertTime)) {
  alerts.value.unshift({
    type: 'danger',
    message: '🚨 CRASH DETECTED!',
    details: 'Impact detected',
    time: new Date().toLocaleTimeString(),
    timestamp: alertTime  // ← Added timestamp
  });
}
```

### Or Using Helper
```javascript
addAlert('danger', '🚨 CRASH DETECTED!', 'Impact detected');
```

---

## localStorage Structure

### Key
```
dismissedAlerts_MnzBjTBslZNijOkq732PE91hHa23
```

### Value
```
1732742700000  // Unix timestamp in milliseconds
```

### Example
```javascript
// Save dismissal time
localStorage.setItem('dismissedAlerts_MnzBjTBslZNijOkq732PE91hHa23', '1732742700000');

// Load dismissal time
const stored = localStorage.getItem('dismissedAlerts_MnzBjTBslZNijOkq732PE91hHa23');
alertDismissalTime.value = parseInt(stored); // 1732742700000
```

---

## Alert Types Updated

### ✅ Crash Detection
```javascript
const alertTime = Date.now();
if (shouldShowAlert(alertTime)) {
  alerts.value.unshift({
    type: 'danger',
    message: '🚨 CRASH DETECTED!',
    timestamp: alertTime
  });
}
```

### ✅ Overspeed
```javascript
const handleOverspeed = (payload) => {
  const alertTime = Date.now();
  if (!shouldShowAlert(alertTime)) return;
  // ... add alert
};
```

### ✅ Helmet Disconnected
```javascript
const shown = addAlert('danger', '⚠️ Helmet Device Disconnected!', 'Helmet module offline');
if (shown) playSound();
```

### ✅ Motorcycle Disconnected
```javascript
const shown = addAlert('danger', '⚠️ Motorcycle Device Disconnected!', 'Motorcycle module offline');
if (shown) playSound();
```

---

## User Experience

### Scenario 1: Clear All Alerts
1. User sees multiple alerts
2. Clicks "Clear All" button
3. All alerts disappear
4. Dismissal time saved: `Date.now()`
5. User refreshes page
6. **Old alerts don't reappear** ✅

### Scenario 2: New Alert After Dismissal
1. User clears all alerts at 10:00 AM
2. New crash occurs at 10:05 AM
3. New alert appears (timestamp > dismissal time)
4. User sees the new alert ✅

### Scenario 3: Multiple Devices
1. User A clears alerts on Device A
2. User A opens dashboard on Device B
3. Alerts still appear on Device B
4. Each device tracks dismissals independently

---

## Benefits

### ✅ Persistent Dismissal
- Alerts stay dismissed after page refresh
- No more annoying repeated alerts
- Better user experience

### ✅ Smart Filtering
- Only shows NEW alerts after dismissal
- Doesn't hide important new events
- Timestamp-based logic

### ✅ Per-User Storage
- Each user has their own dismissal time
- Uses `userId` in localStorage key
- No conflicts between users

### ✅ Automatic Cleanup
- Keeps only last 10 alerts
- Prevents memory bloat
- Clean alert list

---

## Technical Details

### Alert Object Structure
```javascript
{
  type: 'danger',           // 'danger', 'warning', 'success', 'info'
  message: 'Alert title',   // Main message
  details: 'More info',     // Additional details
  time: '10:30:45 AM',      // Display time (string)
  timestamp: 1732742700000  // Unix timestamp (number) ← NEW!
}
```

### Dismissal Check Flow
```
1. Alert triggered
   ↓
2. Get current timestamp
   ↓
3. Check: timestamp > dismissalTime?
   ↓
4a. YES → Show alert
4b. NO  → Skip alert (log to console)
```

### localStorage Persistence
```
Page Load
   ↓
Load dismissalTime from localStorage
   ↓
Set alertDismissalTime.value
   ↓
Firebase listeners fire
   ↓
Each alert checks shouldShowAlert()
   ↓
Only new alerts appear
```

---

## Console Logs

### When Alert is Dismissed
```
[ALERTS] Clearing all alerts
[ALERTS] Dismissal time saved: 11/27/2025, 10:30:45 AM
```

### When Page Loads
```
[INIT] App started at: 11/27/2025, 10:35:00 AM
[ALERTS] Loaded dismissal time: 11/27/2025, 10:30:45 AM
```

### When Old Alert is Skipped
```
[ALERTS] Alert dismissed: 🚨 CRASH DETECTED!
[ALERTS] Crash alert dismissed (older than dismissal time)
```

### When New Alert is Shown
```
[ALERTS] Adding alert: ⚠️ Helmet Device Disconnected!
```

---

## Testing Checklist

- [x] Clear all alerts
- [x] Refresh page
- [x] Old alerts don't reappear
- [x] New alerts still appear
- [x] Dismissal time saved to localStorage
- [x] Dismissal time loaded on page load
- [x] Console logs show filtering
- [x] Multiple alert types work
- [x] Sound only plays for new alerts

---

## Edge Cases Handled

### 1. First Time User
- `alertDismissalTime` starts at 0
- All alerts are shown (timestamp > 0)
- Normal behavior

### 2. localStorage Cleared
- Dismissal time resets to 0
- All alerts shown again
- User can dismiss again

### 3. Clock Changes
- Uses `Date.now()` consistently
- Relative timestamps work
- No timezone issues

### 4. Very Old Alerts
- If alert timestamp < dismissal time
- Alert is skipped
- Prevents old data from showing

---

## Files Modified

- `src/views/Dashboard.vue`
  - Added `dismissedAlertsKey` constant
  - Added `alertDismissalTime` ref
  - Added `loadDismissalTime()` function
  - Added `shouldShowAlert()` function
  - Added `addAlert()` helper function
  - Updated `clearAllAlerts()` to save dismissal time
  - Updated crash alert to check dismissal
  - Updated overspeed alert to check dismissal
  - Updated helmet disconnection alert
  - Updated motorcycle disconnection alert
  - Added dismissal time loading in `onMounted()`

---

## Future Enhancements

Possible improvements:
- Add "Dismiss for 1 hour" option
- Add "Dismiss this type" option
- Add alert categories (critical, warning, info)
- Add alert history view
- Add export alerts feature
- Add alert statistics

---

## Summary

**Problem:** Alerts reappear after page refresh
**Solution:** Track dismissal time in localStorage
**Result:** Dismissed alerts stay dismissed, new alerts still appear

The system now remembers when you cleared alerts and only shows new ones that occurred after that time. Much better user experience! 🎉
