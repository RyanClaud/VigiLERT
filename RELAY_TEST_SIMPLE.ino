/*
Simple Relay Test - Verify relay is working
Upload this to test if relay responds to GPIO 27
*/

const int relayPin = 27;
const int ledPin = 13;

void setup() {
  Serial.begin(115200);
  pinMode(relayPin, OUTPUT);
  pinMode(ledPin, OUTPUT);
  
  Serial.println("\n=== RELAY TEST STARTED ===");
  Serial.println("This will toggle relay every 2 seconds");
  Serial.println("Watch the relay LED (DS1) and listen for clicking sound");
  Serial.println("================================\n");
}

void loop() {
  // Turn relay ON
  Serial.println("🟢 Relay ON (HIGH) - DS1 should be ON");
  digitalWrite(relayPin, HIGH);
  digitalWrite(ledPin, HIGH);
  delay(2000);
  
  // Turn relay OFF
  Serial.println("🔴 Relay OFF (LOW) - DS1 should be OFF");
  digitalWrite(relayPin, LOW);
  digitalWrite(ledPin, LOW);
  delay(2000);
  
  Serial.println("---");
}
