# Real-Time GPS Speed Monitoring

## Overview
The VigiLERT system now supports real-time speed monitoring using the mobile phone's GPS sensor. This provides accurate speed tracking directly from the device, complementing the GPS module data.

## Features

### 1. Continuous GPS Speed Tracking
- **Real-time Updates**: Uses `navigator.geolocation.watchPosition()` for continuous location and speed updates
- **Automatic Speed Calculation**: Speed is provided directly by the GPS sensor in meters per second and converted to km/h
- **High Accuracy Mode**: Enabled for precise GPS readings
- **Smart Filtering**: Filters out very small speed values (< 0.5 km/h) to show 0 when stationary

### 2. Dual Source Support
The system can receive speed data from two sources:
- **GPS Module**: Hardware GPS module on the motorcycle (default)
- **Phone GPS**: Mobile device's built-in GPS sensor (when monitoring is active)

### 3. Speed Monitoring Control Panel
A dedicated control panel allows users to:
- **Start/Stop Monitoring**: Toggle GPS speed monitoring with a single button
- **View Status**: See which source is currently active
- **Real-time Indicator**: Visual indicator shows monitoring status with animated pulse

### 4. Automatic Overspeed Detection
- Continuously checks speed against the set speed limit
- Triggers alerts when speed exceeds the limit
- Plays alert sound for immediate notification
- Logs overspeed events in the alerts panel

### 5. Firebase Integration
All GPS data is automatically synced to Firebase:
- Location coordinates (latitude/longitude)
- Current speed
- Timestamp
- Data source identifier (phone_gps or module)

## How It Works

### Starting GPS Monitoring
1. User clicks "Start Monitoring" button
2. Browser requests location permission (if not already granted)
3. System begins continuous GPS tracking
4. Speed and location update automatically every few seconds
5. Data is displayed on dashboard and synced to Firebase

### Speed Calculation
```javascript
// GPS provides speed in meters per second
const speedKmh = speed * 3.6; // Convert to km/h
const finalSpeed = speedKmh < 0.5 ? 0 : speedKmh; // Filter small values
```

### Data Flow
```
Phone GPS Sensor
    ↓
Browser Geolocation API
    ↓
Dashboard Component
    ↓
Firebase Database
    ↓
Real-time Updates to All Connected Devices
```

## UI Components

### GPS Speed Monitoring Control Panel
- **Location**: Above the tabs section on Dashboard
- **Color**: Blue gradient background
- **Status Indicator**: Shows active/inactive state with animated pulse
- **Toggle Button**: Large, easy-to-tap button for mobile use

### Speed Card Enhancement
- **Source Indicator**: Shows icon (smartphone or router) based on data source
- **Dynamic Text**: Updates to show "Phone GPS" or "GPS Module"
- **Overspeed Alert**: Red background with pulse animation when over limit

## Technical Details

### GPS Configuration
```javascript
{
  enableHighAccuracy: true,  // Use GPS instead of WiFi/cell tower
  timeout: 10000,            // 10 second timeout
  maximumAge: 0              // Always get fresh data
}
```

### Error Handling
The system handles various GPS errors:
- **PERMISSION_DENIED**: User denied location access
- **POSITION_UNAVAILABLE**: GPS signal not available
- **TIMEOUT**: Location request took too long
- **Generic Errors**: Any other unexpected errors

All errors are logged and displayed to the user with helpful messages.

### Performance Optimization
- **Automatic Cleanup**: GPS monitoring stops when component unmounts
- **Smart Updates**: Only updates when speed changes significantly
- **Efficient Firebase Writes**: Batches updates to reduce database load

## Mobile Compatibility

### Supported Browsers
- ✅ Chrome (Android/iOS)
- ✅ Safari (iOS)
- ✅ Firefox (Android)
- ✅ Edge (Android)

### Requirements
- HTTPS connection (required for Geolocation API)
- Location permission granted
- GPS sensor available on device
- Active internet connection for Firebase sync

### Battery Considerations
- GPS monitoring uses device battery
- High accuracy mode increases battery consumption
- Recommended to stop monitoring when not riding
- System automatically stops on page close

## Usage Scenarios

### Scenario 1: Motorcycle Ride with Phone
1. Mount phone on motorcycle
2. Open VigiLERT dashboard
3. Click "Start Monitoring"
4. Ride with real-time speed tracking
5. System alerts on overspeed
6. Click "Stop Monitoring" when done

### Scenario 2: Backup GPS Source
1. GPS module loses signal
2. Switch to phone GPS monitoring
3. Continue tracking with phone sensor
4. Switch back when module reconnects

### Scenario 3: Speed Comparison
1. Monitor speed from both sources
2. Compare accuracy between module and phone
3. Use most reliable source for your setup

## Safety Features

### Overspeed Protection
- Automatic detection when speed exceeds limit
- Visual alert (red card with pulse)
- Audio alert (sound notification)
- Alert logged with timestamp and details

### Emergency Integration
- GPS location available for SOS alerts
- Accurate position for emergency contacts
- Real-time tracking during emergencies

### Data Logging
- All speed data logged to Firebase
- Historical speed tracking
- Trip recording with max speed
- Crash detection with location

## Future Enhancements

### Planned Features
- [ ] Speed averaging over time
- [ ] Route optimization based on speed patterns
- [ ] Speed limit suggestions based on location
- [ ] Automatic GPS source switching
- [ ] Offline speed tracking with sync
- [ ] Speed statistics and analytics
- [ ] Comparison charts (module vs phone)

## Troubleshooting

### GPS Not Working
1. Check location permissions in browser settings
2. Ensure HTTPS connection
3. Verify GPS is enabled on device
4. Try refreshing the page
5. Check for browser compatibility

### Inaccurate Speed Readings
1. Ensure clear view of sky for GPS signal
2. Wait for GPS to acquire satellites
3. Check if high accuracy mode is enabled
4. Compare with GPS module readings
5. Consider environmental factors (buildings, tunnels)

### Battery Drain
1. Stop monitoring when not needed
2. Use GPS module as primary source
3. Reduce update frequency if possible
4. Close other GPS-using apps

## API Reference

### Functions

#### `startGPSSpeedMonitoring()`
Starts continuous GPS speed monitoring from phone.

#### `stopGPSSpeedMonitoring()`
Stops GPS monitoring and cleans up resources.

#### `toggleGPSSpeedMonitoring()`
Toggles GPS monitoring on/off.

#### `handleLocationUpdate(newLocation)`
Handles one-time location updates from GPS.

### State Variables

- `isGPSSpeedActive`: Boolean indicating if monitoring is active
- `gpsSpeedSource`: String ('phone' or 'module') indicating data source
- `currentSpeed`: Number representing current speed in km/h
- `speedHistory`: Array of recent speed readings
- `isOverSpeed`: Boolean indicating if speed exceeds limit

## Conclusion

The real-time GPS speed monitoring feature provides accurate, reliable speed tracking using the mobile device's built-in GPS sensor. It seamlessly integrates with the existing VigiLERT system, offering users flexibility in choosing their speed data source while maintaining all safety features and Firebase synchronization.
