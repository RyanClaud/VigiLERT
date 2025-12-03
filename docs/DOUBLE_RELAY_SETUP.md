# 🔧 Using 2-Channel Relay Module (Replacing Broken Single Relay)

## 📸 Your Broken Relay Analysis

From your photo, I can see:
- **Relay appears burnt/corroded** - definitely needs replacement
- **Blue relay housing shows damage** - likely from overcurrent or short circuit
- **This is why it's not working!**

---

## ✅ Solution: Use 2-Channel Relay Module

A 2-channel relay module has **TWO independent relays**. You only need to use **ONE** for the engine control.

### Advantages:
- ✅ Same wiring as single relay (just use Relay 1)
- ✅ Spare relay available (Relay 2) for future features
- ✅ Usually more robust than single relay modules
- ✅ Better heat dissipation

---

## 🔌 2-Channel Relay Module Pinout

```
┌─────────────────────────────────────────┐
│  2-Channel Relay Module                 │
├─────────────────────────────────────────┤
│                                         │
│  Control Pins (Left Side):             │
│  ┌────┐                                │
│  │VCC │ ← 5V Power                     │
│  │GND │ ← Ground                       │
│  │IN1 │ ← Control Relay 1 (GPIO 27)   │
│  │IN2 │ ← Control Relay 2 (not used)  │
│  └────┘                                │
│                                         │
│  Relay 1 Contacts (Top):               │
│  ┌──────────┐                          │
│  │ NC  COM  NO │                       │
│  └──────────┘                          │
│                                         │
│  Relay 2 Contacts (Bottom):            │
│  ┌──────────┐                          │
│  │ NC  COM  NO │ (not used)            │
│  └──────────┘                          │
│                                         │
└─────────────────────────────────────────┘
```

---

## 🔧 Wiring Instructions

### Step 1: Control Pins (Left Side of Module)

```
ESP32 Pin          Wire Color     Relay Module Pin
─────────────────────────────────────────────────
5V        ────→    Red/Yellow ────→    VCC
GND       ────→    Black/White ────→   GND
GPIO 27   ────→    Purple      ────→   IN1
(nothing) ────→                 ────→   IN2 (leave empty)
```

### Step 2: Relay 1 Contacts (Use Top Relay Only)

```
Relay 1 Contacts          Motorcycle
┌──────────────┐         ┌──────────────┐
│ NC           │         │              │ (not used)
│ COM          │────→    │ Ignition +12V│
│ NO           │────→    │ Engine Start │
└──────────────┘         └──────────────┘
```

### Step 3: Leave Relay 2 Empty (Bottom Relay)

```
Relay 2 Contacts: DO NOT CONNECT ANYTHING
- This relay is spare for future use
- Leave all 3 terminals (NC, COM, NO) empty
```

---

## 📝 Code Changes (NONE REQUIRED!)

**Good news:** The code doesn't need any changes! 

The 2-channel relay module works exactly like a single relay:
- GPIO 27 controls IN1 (Relay 1)
- IN2 is not connected, so Relay 2 stays OFF
- All existing code works as-is

```cpp
// Existing code - NO CHANGES NEEDED
const int relayPin = 27;  // Controls IN1 on 2-channel module

digitalWrite(relayPin, HIGH);  // Relay 1 ON
digitalWrite(relayPin, LOW);   // Relay 1 OFF
```

---

## 🧪 Testing the New Relay Module

### Test 1: Basic Relay Function

1. **Wire the control pins** (VCC, GND, IN1)
2. **Upload `RELAY_TEST_SIMPLE.ino`** (from earlier)
3. **Watch Relay 1** (top relay):
   - Should click ON/OFF every 2 seconds
   - Blue LED should blink
   - You should hear clicking sound
4. **Watch Relay 2** (bottom relay):
   - Should stay OFF (no clicking)
   - No LED activity
   - This is correct!

### Test 2: Verify Only Relay 1 Works

```cpp
// Test code to verify correct relay activates
void setup() {
  Serial.begin(115200);
  pinMode(27, OUTPUT);
  
  Serial.println("Testing Relay 1 (IN1)...");
  digitalWrite(27, HIGH);
  delay(2000);
  Serial.println("Relay 1 should be ON now");
  
  digitalWrite(27, LOW);
  delay(2000);
  Serial.println("Relay 1 should be OFF now");
}

void loop() {
  // Empty
}
```

**Expected Result:**
- ✅ Relay 1 (top) clicks ON/OFF
- ✅ Relay 2 (bottom) stays silent
- ✅ Only one relay is active

---

## 🔌 Complete Wiring Diagram

```
┌─────────────┐
│   ESP32     │
│             │
│  5V    ─────┼────→ VCC  ┐
│  GND   ─────┼────→ GND  │  2-Channel
│  GPIO27 ────┼────→ IN1  │  Relay Module
│             │      IN2  │  (IN2 not connected)
└─────────────┘            │
                           │
                    Relay 1 (Top)
                    ┌──────────┐
Motorcycle 12V ────→│ COM      │
Engine Circuit ←────│ NO       │
                    │ NC       │ (not used)
                    └──────────┘
                    
                    Relay 2 (Bottom)
                    ┌──────────┐
                    │ NC       │ (all empty)
                    │ COM      │ (not used)
                    │ NO       │ (not used)
                    └──────────┘
```

---

## ⚠️ Important Notes

### 1. Use Only Relay 1 (Top Relay)
- IN1 controls Relay 1 (top)
- IN2 controls Relay 2 (bottom)
- Only connect to IN1

### 2. Relay 2 is Spare
- You can use it later for additional features:
  - Headlight control
  - Horn control
  - Fuel pump control
  - Emergency flashers
- For now, leave it disconnected

### 3. Power Supply
- 2-channel modules draw slightly more power
- Make sure your 5V supply can provide at least **200mA**
- ESP32 USB power should be sufficient

### 4. Jumper Settings
Some 2-channel relay modules have jumpers:
- **JD-VCC jumper:** Keep it ON (connected)
- **High/Low trigger:** Set to HIGH trigger (default)
- If unsure, leave jumpers as they came from factory

---

## 🎯 Advantages of 2-Channel Module

### Current Use:
- ✅ Engine control (Relay 1)
- ✅ Spare relay available (Relay 2)

### Future Expansion Ideas:

**Option 1: Dual Safety System**
- Relay 1: Main engine control
- Relay 2: Backup safety cutoff

**Option 2: Additional Features**
- Relay 1: Engine control
- Relay 2: Headlight auto-on when engine starts

**Option 3: Emergency Features**
- Relay 1: Engine control
- Relay 2: Emergency flashers activation

---

## 🔍 Troubleshooting

### Issue: Both Relays Click Together

**Cause:** IN1 and IN2 are connected together

**Fix:** 
- Disconnect wire from IN2
- Only connect to IN1

---

### Issue: Wrong Relay Activates

**Cause:** Connected to IN2 instead of IN1

**Fix:**
- Move control wire from IN2 to IN1
- IN1 is usually the top relay

---

### Issue: No Relay Clicks

**Cause:** Power or control signal issue

**Fix:**
1. Check VCC has 5V
2. Check GND is connected
3. Check IN1 receives 3.3V when HIGH
4. Try manual test: touch IN1 to VCC

---

### Issue: Relay Clicks But Wrong Logic

**Cause:** Active-LOW relay module

**Fix:** Invert code logic:
```cpp
digitalWrite(relayPin, LOW);  // Engine ON
digitalWrite(relayPin, HIGH); // Engine OFF
```

---

## 📊 Pin Summary

| ESP32 Pin | 2-Channel Module Pin | Function |
|-----------|---------------------|----------|
| 5V | VCC | Power supply |
| GND | GND | Ground |
| GPIO 27 | IN1 | Control Relay 1 |
| (none) | IN2 | Not connected |

| Relay 1 Contact | Connection | Function |
|----------------|------------|----------|
| COM | Ignition +12V | Common |
| NO | Engine Circuit | Normally Open |
| NC | (empty) | Not used |

| Relay 2 Contact | Connection | Function |
|----------------|------------|----------|
| All | (empty) | Spare for future |

---

## ✅ Installation Checklist

- [ ] Remove old broken relay module
- [ ] Connect VCC to ESP32 5V
- [ ] Connect GND to ESP32 GND
- [ ] Connect IN1 to ESP32 GPIO 27
- [ ] Leave IN2 disconnected
- [ ] Connect Relay 1 COM to ignition +12V
- [ ] Connect Relay 1 NO to engine circuit
- [ ] Leave Relay 1 NC empty
- [ ] Leave all Relay 2 contacts empty
- [ ] Upload test code
- [ ] Verify only Relay 1 clicks
- [ ] Test with multimeter (COM to NO)
- [ ] Upload main code
- [ ] Test engine start/stop

---

## 🎉 Result

Your new 2-channel relay module will work perfectly with the existing code. Just use Relay 1 (top relay) and leave Relay 2 for future expansion!

---

## 💡 Pro Tip: Future Use of Relay 2

If you want to add a second feature later, you can use GPIO 26 (currently used for right signal) or any other free GPIO:

```cpp
const int relay1Pin = 27;  // Engine control
const int relay2Pin = 26;  // Future feature

pinMode(relay1Pin, OUTPUT);
pinMode(relay2Pin, OUTPUT);

digitalWrite(relay1Pin, HIGH);  // Engine ON
digitalWrite(relay2Pin, HIGH);  // Feature ON
```

But for now, just use Relay 1 with GPIO 27!
