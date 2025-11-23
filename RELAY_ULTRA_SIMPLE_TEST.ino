/*
 * ULTRA SIMPLE RELAY TEST
 * Just toggles GPIO 25 HIGH/LOW every 2 seconds
 * No libraries, no complexity - just raw GPIO control
 */

const int relayPin = 5;   // GPIO 5 - PROVEN TO WORK!

void setup() {
  Serial.begin(115200);
  delay(100);
  
  Serial.println("\n╔════════════════════════════════════════╗");
  Serial.println("║   ULTRA SIMPLE RELAY TEST              ║");
  Serial.println("╠════════════════════════════════════════╣");
  Serial.println("║ This will toggle GPIO 25 every 2s     ║");
  Serial.println("║ Watch the relay LEDs (DS1/DS2)        ║");
  Serial.println("╚════════════════════════════════════════╝\n");
  
  pinMode(relayPin, OUTPUT);
  
  Serial.printf("GPIO Pin: %d\n", relayPin);
  Serial.println("Starting toggle test...\n");
  
  delay(2000);
}

void loop() {
  // Set HIGH
  digitalWrite(relayPin, HIGH);
  int state1 = digitalRead(relayPin);
  Serial.println("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━");
  Serial.printf("Set GPIO %d to HIGH\n", relayPin);
  Serial.printf("Read back: %d\n", state1);
  Serial.println("Check relay LEDs now!");
  Serial.println("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n");
  delay(2000);
  
  // Set LOW
  digitalWrite(relayPin, LOW);
  int state2 = digitalRead(relayPin);
  Serial.println("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━");
  Serial.printf("Set GPIO %d to LOW\n", relayPin);
  Serial.printf("Read back: %d\n", state2);
  Serial.println("Check relay LEDs now!");
  Serial.println("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n");
  delay(2000);
}
