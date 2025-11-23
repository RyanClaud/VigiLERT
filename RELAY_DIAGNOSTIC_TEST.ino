/*
  RELAY DIAGNOSTIC TEST
  
  This code will help you determine:
  1. Is your relay active-HIGH or active-LOW?
  2. Is GPIO 27 working correctly?
  3. Is the relay responding to commands?
  
  Upload this code and follow the instructions in Serial Monitor.
*/

const int relayPin = 27;
const int ledPin = 13; // Built-in LED for visual feedback

void setup() {
  Serial.begin(115200);
  delay(2000);
  
  pinMode(relayPin, OUTPUT);
  pinMode(ledPin, OUTPUT);
  
  // Start with relay OFF
  digitalWrite(relayPin, LOW);
  digitalWrite(ledPin, LOW);
  
  Serial.println("\n\n");
  Serial.println("╔════════════════════════════════════════════════════════╗");
  Serial.println("║                                                        ║");
  Serial.println("║        RELAY DIAGNOSTIC TEST - 2-CHANNEL RELAY         ║");
  Serial.println("║                                                        ║");
  Serial.println("╚════════════════════════════════════════════════════════╝");
  Serial.println();
  Serial.println("This test will help determine your relay type.");
  Serial.println("Watch the RED LED (DS1) on your relay module.");
  Serial.println();
  Serial.println("Starting test in 3 seconds...");
  delay(3000);
}

void loop() {
  // ═══════════════════════════════════════════════════════════
  // TEST 1: GPIO LOW
  // ═══════════════════════════════════════════════════════════
  Serial.println("\n━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━");
  Serial.println("TEST 1: Setting GPIO 27 = LOW (0 Volts)");
  Serial.println("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━");
  
  digitalWrite(relayPin, LOW);
  digitalWrite(ledPin, LOW);
  delay(500);
  
  Serial.println();
  Serial.println("🔍 CHECK YOUR RELAY NOW:");
  Serial.println("   1. Is the RED LED (DS1) ON or OFF?");
  Serial.println("   2. Did you hear a click sound?");
  Serial.println("   3. Try to start your engine - does it start?");
  Serial.println();
  Serial.println("📝 RECORD YOUR OBSERVATIONS:");
  Serial.println("   GPIO LOW → LED is: _______  (ON or OFF?)");
  Serial.println("   GPIO LOW → Engine: _______ (Starts or Blocked?)");
  Serial.println();
  
  // Countdown
  for (int i = 10; i > 0; i--) {
    Serial.printf("   Waiting %d seconds...\n", i);
    delay(1000);
  }
  
  // ═══════════════════════════════════════════════════════════
  // TEST 2: GPIO HIGH
  // ═══════════════════════════════════════════════════════════
  Serial.println("\n━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━");
  Serial.println("TEST 2: Setting GPIO 27 = HIGH (3.3 Volts)");
  Serial.println("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━");
  
  digitalWrite(relayPin, HIGH);
  digitalWrite(ledPin, HIGH);
  delay(500);
  
  Serial.println();
  Serial.println("🔍 CHECK YOUR RELAY NOW:");
  Serial.println("   1. Is the RED LED (DS1) ON or OFF?");
  Serial.println("   2. Did you hear a click sound?");
  Serial.println("   3. Try to start your engine - does it start?");
  Serial.println();
  Serial.println("📝 RECORD YOUR OBSERVATIONS:");
  Serial.println("   GPIO HIGH → LED is: _______  (ON or OFF?)");
  Serial.println("   GPIO HIGH → Engine: _______ (Starts or Blocked?)");
  Serial.println();
  
  // Countdown
  for (int i = 10; i > 0; i--) {
    Serial.printf("   Waiting %d seconds...\n", i);
    delay(1000);
  }
  
  // ═══════════════════════════════════════════════════════════
  // RAPID SWITCHING TEST
  // ═══════════════════════════════════════════════════════════
  Serial.println("\n━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━");
  Serial.println("TEST 3: Rapid ON/OFF Switching (5 times)");
  Serial.println("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━");
  Serial.println();
  Serial.println("🔍 LISTEN FOR CLICKING SOUNDS");
  Serial.println("   You should hear 5 clicks if relay is working");
  Serial.println();
  
  for (int i = 1; i <= 5; i++) {
    Serial.printf("   Click %d: HIGH\n", i);
    digitalWrite(relayPin, HIGH);
    digitalWrite(ledPin, HIGH);
    delay(500);
    
    Serial.printf("   Click %d: LOW\n", i);
    digitalWrite(relayPin, LOW);
    digitalWrite(ledPin, LOW);
    delay(500);
  }
  
  Serial.println();
  Serial.println("📝 RECORD YOUR OBSERVATIONS:");
  Serial.println("   Did you hear clicking? _______  (YES or NO?)");
  Serial.println("   Did LED blink? _______  (YES or NO?)");
  Serial.println();
  
  // ═══════════════════════════════════════════════════════════
  // RESULTS INTERPRETATION
  // ═══════════════════════════════════════════════════════════
  Serial.println("\n╔════════════════════════════════════════════════════════╗");
  Serial.println("║              INTERPRET YOUR RESULTS                    ║");
  Serial.println("╠════════════════════════════════════════════════════════╣");
  Serial.println("║                                                        ║");
  Serial.println("║  SCENARIO 1: LED ON when GPIO = LOW                    ║");
  Serial.println("║  ─────────────────────────────────────────────         ║");
  Serial.println("║  → Your relay is ACTIVE-LOW (inverted logic)           ║");
  Serial.println("║  → You MUST invert the code logic                      ║");
  Serial.println("║  → Add: #define RELAY_ACTIVE_LOW true                  ║");
  Serial.println("║                                                        ║");
  Serial.println("║  SCENARIO 2: LED ON when GPIO = HIGH                   ║");
  Serial.println("║  ─────────────────────────────────────────────         ║");
  Serial.println("║  → Your relay is ACTIVE-HIGH (normal)                  ║");
  Serial.println("║  → Code logic is correct                               ║");
  Serial.println("║  → Problem might be wiring or wrong circuit            ║");
  Serial.println("║                                                        ║");
  Serial.println("║  SCENARIO 3: LED NEVER changes                         ║");
  Serial.println("║  ─────────────────────────────────────────────         ║");
  Serial.println("║  → Relay is stuck or not connected                     ║");
  Serial.println("║  → Check: IN1 wire connection to GPIO 27               ║");
  Serial.println("║  → Check: VCC and GND connections                      ║");
  Serial.println("║  → Try: Different GPIO pin                             ║");
  Serial.println("║                                                        ║");
  Serial.println("║  SCENARIO 4: LED changes but engine always starts      ║");
  Serial.println("║  ─────────────────────────────────────────────         ║");
  Serial.println("║  → Relay is working but not connected to engine        ║");
  Serial.println("║  → Check: COM and NO wiring to motorcycle              ║");
  Serial.println("║  → Verify: Correct ignition wire is cut                ║");
  Serial.println("║                                                        ║");
  Serial.println("╚════════════════════════════════════════════════════════╝");
  Serial.println();
  
  // ═══════════════════════════════════════════════════════════
  // ADDITIONAL TESTS
  // ═══════════════════════════════════════════════════════════
  Serial.println("\n╔════════════════════════════════════════════════════════╗");
  Serial.println("║              ADDITIONAL MANUAL TESTS                   ║");
  Serial.println("╠════════════════════════════════════════════════════════╣");
  Serial.println("║                                                        ║");
  Serial.println("║  TEST A: Bypass ESP32                                  ║");
  Serial.println("║  ────────────────────────────────────────              ║");
  Serial.println("║  1. Disconnect IN1 wire from GPIO 27                   ║");
  Serial.println("║  2. Touch IN1 wire to GND (ground)                     ║");
  Serial.println("║     → Does LED turn ON? Does relay click?              ║");
  Serial.println("║  3. Touch IN1 wire to VCC (5V)                         ║");
  Serial.println("║     → Does LED turn OFF? Does relay click?             ║");
  Serial.println("║                                                        ║");
  Serial.println("║  TEST B: Measure GPIO 27 Voltage                       ║");
  Serial.println("║  ────────────────────────────────────────              ║");
  Serial.println("║  1. Use multimeter on GPIO 27 pin                      ║");
  Serial.println("║  2. Should alternate between 0V and 3.3V               ║");
  Serial.println("║  3. If no change → GPIO 27 might be damaged            ║");
  Serial.println("║                                                        ║");
  Serial.println("║  TEST C: Check Relay Contacts                          ║");
  Serial.println("║  ────────────────────────────────────────              ║");
  Serial.println("║  1. Measure resistance between COM and NO              ║");
  Serial.println("║  2. When LED OFF: Should be ∞ Ω (open)                ║");
  Serial.println("║  3. When LED ON: Should be 0 Ω (closed)                ║");
  Serial.println("║                                                        ║");
  Serial.println("╚════════════════════════════════════════════════════════╝");
  Serial.println();
  
  Serial.println("\n⏸️  Test will repeat in 30 seconds...");
  Serial.println("    Press RESET button to restart test immediately");
  Serial.println();
  
  delay(30000);
  
  Serial.println("\n\n🔄 RESTARTING TEST SEQUENCE...\n\n");
  delay(2000);
}
