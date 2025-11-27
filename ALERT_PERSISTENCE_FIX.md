# Alert Persistence Fix - Crash Detection

## Problem
When restarting the application, old crash alerts would trigger again, causing:
- Continuous alarm sounds
- Red "Crash Detected" card showing even when no new crash occurred
- Alert notifications appearing for old crashes
- User confusion about whether a new crash happened

## Root Cause
The Firebase `onChildAdded` listener fires for **ALL existing crash events** when the app starts, not just new ones. This means:

1. User experiences a crash → Crash saved to Firebase
2. User restarts the app
3. Firebase listener loads ALL crashes (including old ones)
4. Old crash triggers alert/sound again as if it just happened

## Solution Implemented

### 1. App Start Time Tracking
Added `appStartTime` ref that records when the app component mounts:

```javascript
const appStartTime = ref(Date.now());

onMounted(() => {
  appStartTime.value = Date.now();
  console.log('[INIT] App started at:', new Date(appStartTime.value).toLocaleString());
  // ...
});
```

### 2. Filter Old Crashes
Modified `initializeCrashListener()` to ignore crashes that occurred BEFORE the app started:

```javascript
// ✅ FIX: Ignore crashes that happened BEFORE the app started
if (eventTime < appStartTime.value) {
  console.log('[CRASH] ⏭️ Skipping old crash from before app start');
  console.log('[CRASH] Crash time:', new Date(eventTime).toLocaleString());
  console.log('[CRASH] App start:', new Date(appStartTime.value).toLocaleString());
  
  // Still add to crash events array for map display (but no alert/sound)
  const crashEvent = { /* ... */ };
  crashEvents.value = [crashEvent];
  return; // Don't trigger alert/sound for old crashes
}
```

### 3. Enhanced Alert Clearing
Improved the `clearCrashAlert()` function to properly stop animations:

```javascript
const clearCrashAlert = (index) => {
  const event = crashEvents.value[index];
  if (event) {
    // Stop any ongoing crash animation
    if (crashInterval) {
      clearInterval(crashInterval);
      crashInterval = null;
    }
    
    // Reset crash display to stable
    crashDisplayStatus.value = 'Stable';
    crashDisplayMessage.value = 'Vehicle Stable';
  }
};
```

### 4. Clear All Alerts Button
Added a new function and UI button to clear all alerts at once:

```javascript
const clearAllAlerts = () => {
  alerts.value = [];
  
  // Stop crash animation if running
  if (crashInterval) {
    clearInterval(crashInterval);
    crashInterval = null;
  }
  
  // Reset crash display
  crashDisplayStatus.value = 'Stable';
  crashDisplayMessage.value = 'Vehicle Stable';
};
```

## User Experience Improvements

### Before Fix
❌ Restart app → Old crash alerts trigger again
❌ Alarm sounds for old crashes
❌ Red crash card shows even when no new crash
❌ No way to dismiss persistent alerts

### After Fix
✅ Restart app → Only shows crash history (no alerts/sounds)
✅ Alarms only for NEW crashes after app start
✅ Crash card only turns red for NEW crashes
✅ "Clear All" button to dismiss all alerts
✅ Individual "Clear Alert" buttons for each crash event

## Technical Details

### Timeline Comparison
```
Scenario: Crash at 10:00 AM, App restart at 10:30 AM

OLD BEHAVIOR:
10:00 AM - Crash occurs → Alert + Sound ✓
10:30 AM - App restarts → Alert + Sound ✗ (WRONG!)

NEW BEHAVIOR:
10:00 AM - Crash occurs → Alert + Sound ✓
10:30 AM - App restarts → Shows in history only (no alert/sound) ✓
10:35 AM - New crash → Alert + Sound ✓
```

### Data Flow
1. **App Starts** → Record `appStartTime = Date.now()`
2. **Firebase Listener** → Receives all crash events
3. **Filter Logic** → Compare `event.timestamp` vs `appStartTime`
4. **Old Crash** → Add to map display only (silent)
5. **New Crash** → Full alert + sound + animation

## Files Modified
- `src/views/Dashboard.vue`
  - Added `appStartTime` ref
  - Modified `initializeCrashListener()` to filter old crashes
  - Enhanced `clearCrashAlert()` function
  - Added `clearAllAlerts()` function
  - Added "Clear All" button to alerts section UI

## Testing Checklist
- [x] Restart app with existing crash → No alert/sound
- [x] New crash after app start → Alert + sound triggers
- [x] "Clear All" button removes all alerts
- [x] Individual "Clear Alert" button works per crash
- [x] Crash history still visible on map
- [x] Console logs show proper filtering

## Benefits
1. **No False Alarms** - Users won't be startled by old crashes
2. **Better UX** - Clear distinction between old and new crashes
3. **User Control** - Can dismiss alerts when acknowledged
4. **Data Preservation** - Old crashes still visible in history
5. **Performance** - Prevents unnecessary sound/animation playback

## Future Enhancements
- Add auto-dismiss timer for alerts (e.g., 30 seconds)
- Add "Acknowledge" button that marks crash as reviewed
- Add crash severity levels (minor/moderate/severe)
- Add crash report export functionality
