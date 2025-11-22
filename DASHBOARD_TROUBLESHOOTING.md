# Dashboard Troubleshooting - My Location & Recent Trips Not Working

## Symptoms
- Cannot click "My Location" tab
- Recent Trips section is gone/empty

## Possible Causes

### Cause 1: JavaScript Error
**Check:** Open browser console (F12) and look for red error messages

**Common errors:**
- `Uncaught ReferenceError: X is not defined`
- `Uncaught TypeError: Cannot read property 'X' of undefined`
- `Uncaught SyntaxError: Unexpected token`

**Solution:** Share the error message so I can fix it

### Cause 2: Vue Component Not Rendering
**Check:** Look for Vue warnings in console (yellow/orange messages)

**Common warnings:**
- `Component is missing template or render function`
- `Failed to resolve component`
- `Invalid prop type`

### Cause 3: Data Not Loading from Firebase
**Check Console for:**
```
[DEBUG] Loaded crash events: [...]
```

If you DON'T see Firebase data loading messages, the issue might be with Firebase connection.

### Cause 4: Tab Click Not Working
**Check:** Does clicking other tabs work? (Speed Data, etc.)

If NO tabs work → JavaScript error blocking all interactions
If only "My Location" doesn't work → Issue with that specific tab

## Quick Fixes to Try

### Fix 1: Hard Refresh
1. Press `Ctrl + Shift + R` (Windows) or `Cmd + Shift + R` (Mac)
2. This clears cache and reloads everything

### Fix 2: Clear Browser Cache
1. Open DevTools (F12)
2. Right-click the refresh button
3. Select "Empty Cache and Hard Reload"

### Fix 3: Check if Dashboard.vue Compiled
1. Look for build errors in terminal
2. If using `npm run dev`, check if it says "compiled successfully"

### Fix 4: Restart Dev Server
1. Stop the dev server (Ctrl+C)
2. Run `npm run dev` again
3. Wait for "compiled successfully"
4. Refresh browser

## Debug Steps

### Step 1: Check Console
Open browser console and look for:
```
✅ Good signs:
[DEBUG] Helmet status data from Firebase: {...}
[LocationSection] Crash events received: [...]
No red errors

❌ Bad signs:
Uncaught Error: ...
Failed to compile
Component not found
```

### Step 2: Check Network Tab
1. Open DevTools (F12)
2. Go to Network tab
3. Refresh page
4. Look for failed requests (red)

### Step 3: Check Vue DevTools
1. Install Vue DevTools extension
2. Open DevTools → Vue tab
3. Check if Dashboard component is mounted
4. Check component data (crashEvents, recentTrips, etc.)

### Step 4: Check Firebase Console
1. Go to Firebase Console
2. Check Realtime Database
3. Verify data exists at:
   - `/helmet_public/{userId}/trips`
   - `/helmet_public/{userId}/crashes`

## What I Changed

The only changes I made were to the crash event listener:

**Before:**
```javascript
if (!event || !event.timestamp || typeof event.roll !== 'number') {
  console.warn("Invalid crash event received", event);
  return; // Rejected crashes
}
```

**After:**
```javascript
if (!event || !event.timestamp) {
  console.warn("Invalid crash event received (missing timestamp)", event);
  return;
}
// Now accepts crashes and adds lat/lng for map markers
crashEvents.value.push({...});
```

This change should NOT affect:
- My Location tab functionality
- Recent Trips loading
- Any other Dashboard features

## If Still Broken

### Option 1: Revert My Changes
If you want to revert my changes to the crash listener:

1. Find this section in Dashboard.vue (around line 1067)
2. Replace with original code
3. Save and refresh

### Option 2: Check Git Diff
```bash
git diff src/views/Dashboard.vue
```

This shows exactly what changed. If you see unexpected changes, let me know.

### Option 3: Share Console Output
Copy and paste:
1. All red errors from console
2. Any Vue warnings
3. Network errors (if any)

Then I can provide a specific fix.

## Expected Console Output (Working)

When Dashboard loads correctly, you should see:
```
[DEBUG] Helmet device data from Firebase: {...}
[DEBUG] Motorcycle device data from Firebase: {...}
[DEBUG] Helmet status data from Firebase: {...}
[LocationSection] Crash events received: Array(X)
[LocationSection] Number of crashes: X
```

And NO red errors.

## Next Steps

1. **Open browser console** (F12)
2. **Refresh page** (Ctrl+R)
3. **Look for red errors**
4. **Try clicking "My Location" tab**
5. **Share what you see** in console

I'll help you fix it once I know what the specific error is!
