# 🔧 VigiLERT Hardware Installation Guide
## Complete Sensor & Relay Wiring for ESP32

---

## 📦 Required Components

### Motorcycle Module
- 1x ESP32 Development Board (30 pins)
- 1x MPU6050 Gyroscope/Accelerometer Module
- 1x NEO-6M GPS Module
- 1x SIM800L GSM Module
- 1x 5V Relay Module (Single Channel)
- 1x Vibration Sensor Module
- 1x Buzzer Module (Active or Passive)
- 1x LED Indicator
- Resistors: 220Ω (for LED), 10kΩ (pull-up for helmet switch)
- Jumper wires (Male-to-Male, Male-to-Female)
- Breadboard or PCB
- 12V to 5V Buck Converter (for powering ESP32 from motorcycle battery)
- Fuse (5A recommended for safety)

### Helmet Module
- 1x ESP32 Development Board (30 pins)
- 1x MQ3 Alcohol Sensor Module
- 1x NEO-6M GPS Module (optional, for location tracking)
- 1x LED Indicator
- 1x Buzzer Module
- Jumper wires
- Portable power bank or battery pack (5V, 2A minimum)

---

## 🏍️ MOTORCYCLE MODULE INSTALLATION

### 1️⃣ ESP32 Pinout Reference
```
                    ESP32 (30 PIN)
                 ┌─────────────────┐
            3V3  │1              30│ GND
            EN   │2              29│ GPIO23
            VP   │3 (GPIO36)     28│ GPIO22 ← MPU6050 SCL
            VN   │4 (GPIO39)     27│ TXD0
       Battery → │5 (GPIO34)     26│ RXD0
            GPIO35│6             25│ GPIO21 ← MPU6050 SDA
            GPIO32│7 (Headlight) 24│ GPIO19
            GPIO33│8 (Taillight) 23│ GPIO18
            GPIO25│9 (Left Sig)  22│ GPIO5  ← GSM TX
            GPIO26│10(Right Sig) 21│ GPIO4  ← GSM RX
            GPIO27│11← RELAY     20│ GPIO2
            GPIO14│12← Vibration 19│ GPIO15
            GPIO12│13← Buzzer    18│ TXD2
            GND   │14            17│ RXD2
            GPIO13│15← LED       16│ GPIO17 ← GPS TX
            VIN   │               │ GPIO16 ← GPS RX
                 └─────────────────┘
```

---

## 🔌 DETAILED WIRING CONNECTIONS

### 📍 1. MPU6050 (Crash Detection Sensor)

**Purpose:** Detects crashes via acceleration and tilt angle

```
MPU6050 Module          ESP32
┌──────────────┐       ┌──────┐
│ VCC          │────→  │ 3.3V │
│ GND          │────→  │ GND  │
│ SCL          │────→  │ 22   │
│ SDA          │────→  │ 21   │
│ XDA          │       │      │ (Not used)
│ XCL          │       │      │ (Not used)
│ AD0          │────→  │ GND  │ (I2C Address: 0x68)
│ INT          │       │      │ (Not used)
└──────────────┘       └──────┘
```

**Installation Location:** Mount securely on motorcycle frame near center of gravity
- Use double-sided tape or zip ties
- Ensure sensor is level when bike is upright
- Avoid mounting near engine vibrations

**⚠️ Important:**
- Use **3.3V** NOT 5V (can damage ESP32)
- Keep wires short (< 20cm) to reduce noise
- If AD0 is connected to VCC, I2C address becomes 0x69

---

### 📡 2. NEO-6M GPS Module

**Purpose:** Provides location, speed, and time data

```
GPS Module              ESP32
┌──────────────┐       ┌──────┐
│ VCC          │────→  │ 5V   │ (or 3.3V, check module)
│ GND          │────→  │ GND  │
│ TX           │────→  │ 16   │ (GPS TX → ESP32 RX)
│ RX           │────→  │ 17   │ (GPS RX → ESP32 TX)
└──────────────┘       └──────┘
```

**Installation Location:** 
- Mount on top of motorcycle (under seat or on dashboard)
- GPS antenna must have **clear view of sky**
- Keep away from metal objects that block signals
- Use external antenna if mounting inside metal compartment

**⚠️ Important:**
- GPS needs 30-60 seconds for initial fix (cold start)
- Test in open area first
- Some modules need 5V, others work with 3.3V

---

### 📱 3. SIM800L GSM Module

**Purpose:** SMS control and alerts

```
SIM800L Module          ESP32
┌──────────────┐       ┌──────┐
│ VCC          │────→  │ 5V   │ (Needs stable 3.7-4.2V)
│ GND          │────→  │ GND  │
│ TX           │────→  │ 4    │ (GSM TX → ESP32 RX)
│ RX           │────→  │ 5    │ (GSM RX → ESP32 TX)
│ RST          │       │      │ (Not used)
└──────────────┘       └──────┘
```

**⚠️ CRITICAL - Power Requirements:**
- SIM800L draws **2A peak current** during transmission
- ESP32 USB cannot provide enough power
- **MUST use external power supply or large capacitor (1000µF)**

**Recommended Power Solution:**
```
Motorcycle 12V Battery
        ↓
    [5A Fuse]
        ↓
  [Buck Converter] → 5V 3A output
        ↓
    ┌───┴───┐
    ↓       ↓
  ESP32   SIM800L
          + 1000µF capacitor
```

**Installation Location:**
- Mount in weatherproof enclosure
- Keep antenna away from metal
- Insert activated SIM card with SMS plan

---

### ⚡ 4. Relay Module (Engine Control)

**Purpose:** Controls motorcycle ignition circuit

```
Relay Module            ESP32
┌──────────────┐       ┌──────┐
│ VCC          │────→  │ 5V   │
│ GND          │────→  │ GND  │
│ IN           │────→  │ 27   │ (Control signal)
└──────────────┘       └──────┘

Relay Contacts          Motorcycle
┌──────────────┐       ┌──────────────┐
│ COM (Common) │────→  │ Ignition +12V│
│ NO (Normally │────→  │ Engine Start │
│     Open)    │       │ Circuit      │
│ NC (Normally │       │              │
│     Closed)  │       │ (Not used)   │
└──────────────┘       └──────────────┘
```

**Wiring Diagram:**
```
Motorcycle Battery (+12V)
        │
        ├──→ [Fuse 5A] ──→ Buck Converter (5V) ──→ ESP32 VIN
        │
        └──→ Ignition Switch
                  │
                  ├──→ Relay COM
                  │
            Relay NO ──→ Engine Start Circuit
                         (Starter Motor/ECU)
```

**⚠️ CRITICAL Safety:**
- Use relay rated for **12V DC, 10A minimum**
- Add flyback diode across relay coil (prevents voltage spikes)
- **NEVER connect relay directly to starter motor** (too much current)
- Connect to ignition circuit or ECU enable line
- Test with multimeter before connecting to motorcycle

**Installation Steps:**
1. Locate motorcycle ignition wire (usually behind key switch)
2. Cut the wire that enables engine start
3. Connect one end to Relay COM
4. Connect other end to Relay NO
5. When relay activates (GPIO 27 HIGH), circuit closes, engine can start

---

### 🔋 5. Battery Voltage Monitor

**Purpose:** Monitor motorcycle battery health

```
Voltage Divider         ESP32
┌──────────────┐       ┌──────┐
│ 12V Battery  │       │      │
│      │       │       │      │
│   [10kΩ]     │       │      │
│      │       │       │      │
│      ├───────│────→  │ 34   │ (ADC1)
│      │       │       │      │
│   [2.2kΩ]    │       │      │
│      │       │       │      │
│     GND      │────→  │ GND  │
└──────────────┘       └──────┘
```

**Calculation:**
- 12V battery → Voltage divider → 3.3V max to ESP32
- Ratio: 10kΩ / (10kΩ + 2.2kΩ) = 0.18
- 12V × 0.18 = 2.16V (safe for ESP32)
- Code multiplies by 4.3 to get actual voltage

**⚠️ Important:**
- ESP32 ADC max input: **3.3V**
- Never connect 12V directly to GPIO
- Use 1% tolerance resistors for accuracy

---

### 🔔 6. Buzzer & LED Indicators

**Buzzer (Crash Alert):**
```
Buzzer Module           ESP32
┌──────────────┐       ┌──────┐
│ VCC          │────→  │ 5V   │
│ GND          │────→  │ GND  │
│ I/O          │────→  │ 12   │
└──────────────┘       └──────┘
```

**LED Indicator (Status Light):**
```
LED Circuit             ESP32
┌──────────────┐       ┌──────┐
│ GPIO 13      │────→  [220Ω] ──→ LED (+) ──→ GND
└──────────────┘       └──────┘
```

---

### 📳 7. Vibration Sensor (Theft Detection)

**Purpose:** Detects unauthorized movement when engine is off

```
Vibration Sensor        ESP32
┌──────────────┐       ┌──────┐
│ VCC          │────→  │ 3.3V │
│ GND          │────→  │ GND  │
│ DO (Digital) │────→  │ 14   │
└──────────────┘       └──────┘
```

**Installation:** Mount on motorcycle frame, adjust sensitivity potentiometer

---

### 🔌 8. Helmet Switch (Physical Connection Check)

**Purpose:** Detects if helmet is physically connected

```
Helmet Switch           ESP32
┌──────────────┐       ┌──────┐
│ Switch       │────→  │ 39   │ (INPUT_PULLUP)
│ (Normally    │       │      │
│  Open)       │       │      │
│              │────→  │ GND  │
└──────────────┘       └──────┘
```

**Installation:**
- Use magnetic reed switch or physical button
- Mount on helmet connector port
- When helmet plugs in, switch closes (connects to GND)
- ESP32 reads LOW when helmet connected

---

### 🚦 9. Light Sensors (Optional - Dashboard Monitoring)

**Purpose:** Monitor headlight, taillight, turn signals

```
Light Sensor            ESP32
┌──────────────┐       ┌──────┐
│ 12V Light    │       │      │
│      │       │       │      │
│   [10kΩ]     │       │      │
│      │       │       │      │
│      ├───────│────→  │ 32   │ (Headlight)
│      │       │       │ 33   │ (Taillight)
│   [2.2kΩ]    │       │ 25   │ (Left Signal)
│      │       │       │ 26   │ (Right Signal)
│     GND      │────→  │ GND  │
└──────────────┘       └──────┘
```

**Alternative:** Use optocouplers for isolation

---

## 🪖 HELMET MODULE INSTALLATION

### 📍 1. MQ3 Alcohol Sensor

**Purpose:** Detects alcohol in rider's breath

```
MQ3 Module              ESP32
┌──────────────┐       ┌──────┐
│ VCC          │────→  │ 5V   │
│ GND          │────→  │ GND  │
│ AO (Analog)  │────→  │ 34   │ (ADC1)
│ DO (Digital) │       │      │ (Not used)
└──────────────┘       └──────┘
```

**Installation Location:**
- Mount inside helmet near mouth area
- Use velcro or double-sided tape
- Position sensor 5-10cm from rider's mouth
- Ensure good airflow

**⚠️ Important:**
- MQ3 needs **24-48 hours preheat** for accurate readings
- Calibrate threshold based on your sensor
- Default threshold: 2000 (adjust in code)

---

### 🔔 2. Helmet Buzzer & LED

**Buzzer (Alcohol Alert):**
```
Buzzer Module           ESP32
┌──────────────┐       ┌──────┐
│ VCC          │────→  │ 5V   │
│ GND          │────→  │ GND  │
│ I/O          │────→  │ 5    │
└──────────────┘       └──────┘
```

**LED Indicator:**
```
LED Circuit             ESP32
┌──────────────┐       ┌──────┐
│ GPIO 2       │────→  [220Ω] ──→ LED (+) ──→ GND
└──────────────┘       └──────┘
```

---

### 📡 3. Helmet GPS (Optional)

Same wiring as motorcycle GPS module (see above)

---

## ⚡ POWER SUPPLY SETUP

### Motorcycle Module Power

**Option 1: Direct from Battery (Recommended)**
```
Motorcycle Battery (12V)
        │
    [5A Fuse]
        │
  [Buck Converter]
   (12V → 5V, 3A)
        │
        ├──→ ESP32 VIN (5V)
        ├──→ GPS VCC (5V)
        ├──→ GSM VCC (5V) + [1000µF capacitor]
        └──→ Relay VCC (5V)
```

**Option 2: USB Power Bank (Testing Only)**
- Use 10,000mAh power bank
- Connect to ESP32 USB port
- Not recommended for permanent installation

---

### Helmet Module Power

**Option 1: Portable Power Bank**
- 5,000-10,000mAh power bank
- Connect to ESP32 USB port
- Lasts 8-12 hours

**Option 2: Rechargeable Battery Pack**
- 3.7V LiPo battery + TP4056 charging module
- Connect to ESP32 3.3V pin
- Add power switch

---

## 🛠️ ASSEMBLY STEPS

### Motorcycle Module Assembly

1. **Prepare Enclosure**
   - Use weatherproof plastic box (IP65 rated)
   - Drill holes for wires and antenna
   - Add ventilation holes (with mesh to prevent water)

2. **Mount Components**
   - Secure ESP32 to breadboard or PCB
   - Mount relay module separately (generates heat)
   - Position GPS antenna on top of box
   - Keep GSM antenna away from GPS

3. **Wire Connections**
   - Follow wiring diagram above
   - Use heat shrink tubing on all connections
   - Label all wires with tape
   - Test continuity with multimeter

4. **Power Supply**
   - Install buck converter near battery
   - Add inline fuse (5A)
   - Connect to motorcycle battery terminals
   - Test voltage output (should be 5V ±0.2V)

5. **Relay Connection**
   - Locate ignition wire (use motorcycle wiring diagram)
   - Cut wire and connect to relay COM and NO
   - Insulate all connections
   - Test with multimeter before powering on

6. **Mount on Motorcycle**
   - Under seat (most common)
   - Behind dashboard
   - In side panel compartment
   - Use zip ties or mounting brackets

---

### Helmet Module Assembly

1. **Prepare Helmet**
   - Choose mounting location for sensors
   - Ensure MQ3 sensor is near mouth
   - LED should be visible to rider

2. **Mount Components**
   - Use velcro for easy removal
   - Route wires along helmet padding
   - Keep connections accessible for charging

3. **Power Supply**
   - Mount power bank in helmet pocket
   - Or use external battery pack in jacket

4. **Testing**
   - Power on and check Serial Monitor
   - Verify WiFi connection
   - Test alcohol sensor with alcohol vapor
   - Check heartbeat in Firebase

---

## 🧪 TESTING PROCEDURE

### 1. Bench Test (Before Installation)

**Motorcycle Module:**
```
1. Connect ESP32 to computer via USB
2. Open Serial Monitor (115200 baud)
3. Check WiFi connection
4. Verify GPS receives data (may take 60 seconds)
5. Test MPU6050: Type "MPU" in Serial Monitor
6. Test crash detection: Type "CRASH"
7. Check Firebase updates
```

**Helmet Module:**
```
1. Connect ESP32 to computer via USB
2. Open Serial Monitor (115200 baud)
3. Check WiFi connection
4. Test MQ3 sensor with alcohol vapor
5. Verify LED and buzzer activate
6. Check Firebase alcohol status updates
```

---

### 2. Integration Test

```
1. Power both modules
2. Wait for both to connect to WiFi
3. Check Firebase /devices/ for both heartbeats
4. Try engine start via SMS: "1234 START"
5. Should work if all conditions met
6. Test alcohol blocking: Blow alcohol near MQ3
7. Try engine start again - should be blocked
8. Test crash shutdown: Type "CRASH" in Serial Monitor
9. Engine should shut down after 2 seconds
```

---

### 3. On-Motorcycle Test

```
1. Install all components
2. Connect power supply
3. Test relay operation (engine start/stop)
4. Take short test ride
5. Monitor GPS tracking on dashboard
6. Test crash detection (tilt bike carefully)
7. Verify all sensors working
```

---

## ⚠️ SAFETY WARNINGS

### Electrical Safety
- ❌ Never connect 12V directly to ESP32 GPIO pins
- ❌ Never connect relay directly to starter motor (too much current)
- ✅ Always use fuses on battery connections
- ✅ Insulate all exposed connections
- ✅ Use proper wire gauge (18 AWG minimum for power)

### Motorcycle Safety
- ⚠️ Test relay thoroughly before riding
- ⚠️ Have backup method to start motorcycle
- ⚠️ Don't rely solely on electronic system
- ⚠️ Keep manual key override functional

### Installation Safety
- 🔧 Disconnect battery before working on electrical system
- 🔧 Use proper tools and connectors
- 🔧 Follow motorcycle manufacturer's wiring diagram
- 🔧 Consider professional installation for relay connection

---

## 🔍 TROUBLESHOOTING

### GPS Not Working
- Check antenna has clear sky view
- Verify TX/RX pins not swapped
- Wait 60 seconds for initial fix
- Test in open area away from buildings

### GSM Not Connecting
- Check SIM card is activated
- Verify sufficient power supply (2A peak)
- Add 1000µF capacitor near module
- Check antenna connection

### MPU6050 Reading Zero
- Verify I2C wiring (SDA/SCL)
- Check I2C address (0x68 or 0x69)
- Test with I2C scanner sketch
- Ensure 3.3V power supply

### Relay Not Switching
- Check GPIO 27 output with LED
- Verify relay module power (5V)
- Test relay manually (connect IN to VCC)
- Check relay contacts with multimeter

### Alcohol Sensor Always High
- Sensor needs 24-48 hour preheat
- Adjust threshold in code
- Check sensor power supply
- Calibrate in fresh air

---

## 📊 Pin Summary Table

### Motorcycle Module
| Component | Pin | Type | Notes |
|-----------|-----|------|-------|
| MPU6050 SDA | 21 | I2C | 3.3V |
| MPU6050 SCL | 22 | I2C | 3.3V |
| GPS RX | 16 | UART | 5V tolerant |
| GPS TX | 17 | UART | 5V tolerant |
| GSM RX | 4 | UART | 5V tolerant |
| GSM TX | 5 | UART | 5V tolerant |
| Relay Control | 27 | Output | 5V relay |
| Battery Monitor | 34 | ADC | Voltage divider |
| Helmet Switch | 39 | Input | Pull-up |
| Vibration Sensor | 14 | Input | Digital |
| Buzzer | 12 | Output | PWM |
| LED Indicator | 13 | Output | 220Ω resistor |
| Headlight | 32 | Input | Voltage divider |
| Taillight | 33 | Input | Voltage divider |
| Left Signal | 25 | Input | Voltage divider |
| Right Signal | 26 | Input | Voltage divider |

### Helmet Module
| Component | Pin | Type | Notes |
|-----------|-----|------|-------|
| MQ3 Alcohol | 34 | ADC | Analog input |
| LED Indicator | 2 | Output | 220Ω resistor |
| Buzzer | 5 | Output | PWM |
| GPS RX | 16 | UART | Optional |
| GPS TX | 17 | UART | Optional |

---

## 📸 Recommended Tools

- Soldering iron and solder
- Wire strippers
- Multimeter
- Heat shrink tubing
- Zip ties
- Electrical tape
- Crimping tool
- Screwdrivers
- Drill (for enclosure)

---

## ✅ Installation Checklist

- [ ] All components purchased
- [ ] Wiring diagram printed
- [ ] Enclosures prepared
- [ ] Power supply tested (5V, 3A)
- [ ] Bench test completed
- [ ] Code uploaded to both ESP32s
- [ ] WiFi credentials configured
- [ ] Firebase connection verified
- [ ] Relay tested with multimeter
- [ ] All connections insulated
- [ ] Fuses installed
- [ ] Components mounted securely
- [ ] Integration test passed
- [ ] On-motorcycle test completed
- [ ] Emergency shutdown method available

---

**🎉 Installation Complete!**

Your VigiLERT safety system is now ready to protect you on every ride.

For support, refer to:
- `SAFETY_SYSTEM_COMPLETE.md` - Safety features overview
- `MPU6050_TROUBLESHOOTING.md` - Crash detection issues
- `CRASH_DETECTION_FIX.md` - Crash detection calibration
