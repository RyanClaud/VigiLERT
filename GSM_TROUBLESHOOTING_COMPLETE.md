# GSM Module Troubleshooting Guide

## Your Issue: Blinking Twice Every 30 Seconds

### What It Means

| Blink Pattern | Status | Meaning |
|---------------|--------|---------|
| **2 blinks every 30s** | ❌ **NOT REGISTERED** | Searching for network |
| 1 blink every 3s | ✅ Registered | Connected to network |
| Constant on | ⚠️ Error | Module stuck |
| No blink | ❌ No power | Not powered |

**Your module is SEARCHING for network but cannot register.**

## Common Causes & Solutions

### 1. ⚡ Power Supply Issue (MOST COMMON)

**Problem**: GSM modules need **2A peak current**, ESP32 USB can only provide ~500mA

**Symptoms**:
- Blinks twice then resets
- Works briefly then fails
- "Not ready" in Serial Monitor

**Solutions**:

#### Option A: External Power Supply (Recommended)
```
GSM Module Power:
├── VCC → External 5V/2A power supply
├── GND → Common ground with ESP32
└── TX/RX → ESP32 GPIO 4/5
```

#### Option B: Large Capacitor
```
Add 1000µF capacitor:
├── Positive → GSM VCC
└── Negative → GND

This provides burst current for GSM transmission
```

#### Option C: Separate Battery
```
Use 18650 Li-ion battery:
├── Battery → Step-up converter (5V)
├── 5V → GSM VCC
└── GND → Common ground
```

### 2. 📡 SIM Card Issues

**Check:**
- [ ] SIM card inserted correctly (gold contacts facing down)
- [ ] SIM card activated (call from another phone)
- [ ] SIM card has load/credit
- [ ] SIM card not PIN locked
- [ ] SIM card size correct (use adapter if needed)

**Test SIM Card:**
```
1. Remove SIM from GSM module
2. Insert into phone
3. Make a call
4. Send SMS
5. If works → SIM is good
6. If fails → Contact carrier
```

### 3. 📶 Network Coverage

**Check:**
- [ ] Carrier has coverage in your area
- [ ] Not in basement/underground
- [ ] Antenna connected to GSM module
- [ ] Try different location

**Test Coverage:**
```
1. Put SIM in phone
2. Check signal bars
3. If weak → Move to better location
4. If strong → Problem is GSM module power
```

### 4. 🔌 Wiring Issues

**Verify Connections:**

```
ESP32 → GSM Module
├── GPIO 4 (RX) → GSM TX
├── GPIO 5 (TX) → GSM RX
├── GND → GND
└── 5V → VCC (needs 2A!)

⚠️ CRITICAL: TX connects to RX, RX connects to TX (crossed)
```

**Common Mistakes:**
- ❌ TX to TX, RX to RX (wrong!)
- ❌ No common ground
- ❌ Insufficient power
- ❌ Loose connections

### 5. 🔧 Module Configuration

**AT Commands to Try:**

```cpp
// In Serial Monitor, send these manually:

AT                    // Test communication
AT+CPIN?             // Check SIM status
AT+CREG?             // Check network registration
AT+CSQ               // Check signal strength
AT+COPS?             // Check operator
AT+CFUN=1            // Full functionality
AT+CMGF=1            // SMS text mode
```

## Enhanced GSM Initialization

Let me create an improved initialization function:

<function_calls>
<invoke name="fsWrite">
<parameter name="path">GSM_ENHANCED_INIT.inoNHANCED GSM Initialization with better diagnostics
void initializeGSM() {
  Serial.println("\n╔═══════════════════════════════════════════════════════════╗");
  Serial.println("║              GSM MODULE INITIALIZATION                    ║");
  Serial.println("╚═══════════════════════════════════════════════════════════╝");
  
  Serial.println("\n[GSM] Step 1: Waiting for module boot...");
  delay(5000);  // Longer delay for module to fully boot
  
  // Clear serial buffer
  while (gsmSerial.available()) {
    gsmSerial.read();
  }
  
  // Test 1: Basic AT command
  Serial.println("\n[GSM] Step 2: Testing AT command...");
  gsmSerial.println("AT");
  delay(1000);
  
  String response = "";
  if (gsmSerial.available()) {
    response = gsmSerial.readString();
    Serial.println("[GSM] Response: " + response);
    
    if (response.indexOf("OK") != -1) {
      Serial.println("[GSM] ✅ Module responding!");
    } else {
      Serial.println("[GSM] ❌ Unexpected response");
      Serial.println("[GSM] Check: Wiring, baud rate, power");
      return;
    }
  } else {
    Serial.println("[GSM] ❌ No response!");
    Serial.println("[GSM] TROUBLESHOOTING:");
    Serial.println("[GSM]   • Check power supply (needs 2A)");
    Serial.println("[GSM]   • Verify wiring: RX:4, TX:5");
    Serial.println("[GSM]   • Check baud rate (9600)");
    Serial.println("[GSM]   • Try external power supply");
    return;
  }
  
  // Test 2: Check SIM card
  Serial.println("\n[GSM] Step 3: Checking SIM card...");
  gsmSerial.println("AT+CPIN?");
  delay(2000);
  
  response = "";
  if (gsmSerial.available()) {
    response = gsmSerial.readString();
    Serial.println("[GSM] SIM Status: " + response);
    
    if (response.indexOf("READY") != -1) {
      Serial.println("[GSM] ✅ SIM card ready!");
    } else if (response.indexOf("SIM PIN") != -1) {
      Serial.println("[GSM] ❌ SIM card PIN locked!");
      Serial.println("[GSM] Remove PIN using phone settings");
      return;
    } else {
      Serial.println("[GSM] ❌ SIM card not detected!");
      Serial.println("[GSM] Check: SIM inserted correctly");
      return;
    }
  }
  
  // Test 3: Check network registration
  Serial.println("\n[GSM] Step 4: Checking network registration...");
  
  for (int attempt = 0; attempt < 10; attempt++) {
    gsmSerial.println("AT+CREG?");
    delay(1000);
    
    response = "";
    if (gsmSerial.available()) {
      response = gsmSerial.readString();
      Serial.printf("[GSM] Registration (attempt %d/10): %s\n", attempt + 1, response.c_str());
      
      // +CREG: 0,1 or +CREG: 0,5 means registered
      if (response.indexOf(",1") != -1 || response.indexOf(",5") != -1) {
        Serial.println("[GSM] ✅ Registered on network!");
        break;
      } else if (response.indexOf(",2") != -1) {
        Serial.println("[GSM] ⏳ Searching for network...");
      } else if (response.indexOf(",3") != -1) {
        Serial.println("[GSM] ❌ Registration denied!");
        Serial.println("[GSM] Check: SIM activated, carrier coverage");
        return;
      }
    }
    
    if (attempt == 9) {
      Serial.println("[GSM] ❌ Failed to register after 10 attempts");
      Serial.println("[GSM] TROUBLESHOOTING:");
      Serial.println("[GSM]   • Check carrier coverage in your area");
      Serial.println("[GSM]   • Verify SIM card is activated");
      Serial.println("[GSM]   • Try SIM in phone first");
      Serial.println("[GSM]   • Check antenna connection");
      return;
    }
  }
  
  // Test 4: Check signal strength
  Serial.println("\n[GSM] Step 5: Checking signal strength...");
  gsmSerial.println("AT+CSQ");
  delay(1000);
  
  response = "";
  if (gsmSerial.available()) {
    response = gsmSerial.readString();
    Serial.println("[GSM] Signal: " + response);
    
    // Extract signal value (format: +CSQ: XX,YY)
    int signalStart = response.indexOf(":") + 2;
    int signalEnd = response.indexOf(",", signalStart);
    if (signalEnd > signalStart) {
      String signalStr = response.substring(signalStart, signalEnd);
      int signalValue = signalStr.toInt();
      
      Serial.printf("[GSM] Signal Strength: %d/31\n", signalValue);
      
      if (signalValue >= 10) {
        Serial.println("[GSM] ✅ Good signal!");
      } else if (signalValue >= 5) {
        Serial.println("[GSM] ⚠️ Weak signal - may have issues");
      } else {
        Serial.println("[GSM] ❌ Very weak signal!");
        Serial.println("[GSM] Move to area with better coverage");
      }
    }
  }
  
  // Test 5: Set SMS mode
  Serial.println("\n[GSM] Step 6: Configuring SMS mode...");
  gsmSerial.println("AT+CMGF=1");
  delay(1000);
  
  response = "";
  if (gsmSerial.available()) {
    response = gsmSerial.readString();
    if (response.indexOf("OK") != -1) {
      Serial.println("[GSM] ✅ SMS mode set to text");
    }
  }
  
  // Test 6: Check operator
  Serial.println("\n[GSM] Step 7: Checking operator...");
  gsmSerial.println("AT+COPS?");
  delay(2000);
  
  response = "";
  if (gsmSerial.available()) {
    response = gsmSerial.readString();
    Serial.println("[GSM] Operator: " + response);
  }
  
  // All tests passed
  gsmReady = true;
  
  Serial.println("\n╔═══════════════════════════════════════════════════════════╗");
  Serial.println("║           GSM MODULE READY FOR SMS ALERTS!                ║");
  Serial.println("╚═══════════════════════════════════════════════════════════╝\n");
}
