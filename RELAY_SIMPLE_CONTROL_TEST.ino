/*
RELAY SIMPLE CONTROL TEST
✅ No safety interlocks - direct relay control only
✅ Test if relay responds to commands without interference
*/

const int relayPin = 13;
const int ledPin = 2;

void setup() {
  Serial.begin(115200);
  pinMode(relayPin, OUTPUT);
  pinMode(ledPin, OUTPUT);
  
  // Start with relay OFF
  digitalWrite(relayPin, LOW);
  digitalWrite(ledPin, LOW);
  
  Serial.println("\n=== RELAY SIMPLE CONTROL TEST ===");
  Serial.println("RELAY TYPE: ACTIVE-HIGH");
  Serial.println("LOW = OFF | HIGH = ON");
  Serial.println("================================");
  Serial.println("\nCOMMANDS:");
  Serial.println("  ON    - Turn relay ON");
  Serial.println("  OFF   - Turn relay OFF");
  Serial.println("  CRASH - Test crash shutdown");
  Serial.println("  AUTO  - Auto toggle every 2 seconds");
  Serial.println("  STOP  - Stop auto toggle\n");
  
  Serial.printf("Initial state: GPIO %d = %d (LOW/OFF)\n", relayPin, digitalRead(relayPin));
}

bool autoMode = false;

void loop() {
  // Handle serial commands
  if (Serial.available()) {
    String cmd = Serial.readStringUntil('\n');
    cmd.trim();
    cmd.toUpperCase();
    
    if (cmd == "ON") {
      autoMode = false;
      Serial.println("\n[COMMAND] Turning relay ON...");
      digitalWrite(relayPin, HIGH);
      digitalWrite(ledPin, HIGH);
      Serial.printf("✅ Relay GPIO %d = %d (HIGH/ON)\n", relayPin, digitalRead(relayPin));
      Serial.println("✅ Check: DS1 LED should be ON, relay should click");
    }
    else if (cmd == "OFF") {
      autoMode = false;
      Serial.println("\n[COMMAND] Turning relay OFF...");
      digitalWrite(relayPin, LOW);
      digitalWrite(ledPin, LOW);
      Serial.printf("🛑 Relay GPIO %d = %d (LOW/OFF)\n", relayPin, digitalRead(relayPin));
      Serial.println("🛑 Check: DS1 LED should be OFF, relay should click");
    }
    else if (cmd == "CRASH") {
      autoMode = false;
      Serial.println("\n🚨 CRASH TEST - EMERGENCY SHUTDOWN!");
      digitalWrite(relayPin, LOW);  // Force OFF
      digitalWrite(ledPin, LOW);
      Serial.printf("🚨 Relay GPIO %d = %d (LOW/OFF)\n", relayPin, digitalRead(relayPin));
      Serial.println("🚨 Engine power should be CUT!");
      
      // Alert pattern
      for (int i = 0; i < 3; i++) {
        digitalWrite(ledPin, HIGH);
        delay(200);
        digitalWrite(ledPin, LOW);
        delay(200);
      }
    }
    else if (cmd == "AUTO") {
      Serial.println("\n[AUTO] Starting auto toggle mode...");
      autoMode = true;
    }
    else if (cmd == "STOP") {
      Serial.println("\n[STOP] Stopping auto mode...");
      autoMode = false;
    }
    else {
      Serial.println("\nUnknown command. Available: ON, OFF, CRASH, AUTO, STOP");
    }
  }
  
  // Auto toggle mode
  if (autoMode) {
    static unsigned long lastToggle = 0;
    static bool relayState = false;
    
    if (millis() - lastToggle > 2000) {
      relayState = !relayState;
      digitalWrite(relayPin, relayState ? HIGH : LOW);
      digitalWrite(ledPin, relayState ? HIGH : LOW);
      
      Serial.printf("[AUTO] Relay %s - GPIO %d = %d\n", 
                    relayState ? "ON " : "OFF", 
                    relayPin, digitalRead(relayPin));
      
      lastToggle = millis();
    }
  }
  
  delay(50);
}