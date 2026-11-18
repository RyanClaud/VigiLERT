/*
 * VIBRATION SENSOR DIAGNOSTIC TEST
 * 
 * This code helps you:
 * 1. Check if sensor is working
 * 2. See real-time sensor readings
 * 3. Adjust sensitivity potentiometer
 * 4. Test detection
 * 
 * Upload this to your ESP32 and open Serial Monitor (115200 baud)
 */

const int vibrationSensorPin = 15;  // GPIO 15
const int buzzerPin = 12;           // GPIO 12

void setup() {
  Serial.begin(115200); 
  delay(1000);
  
  pinMode(vibrationSensorPin, INPUT);
  pinMode(buzzerPin, OUTPUT);
  digitalWrite(buzzerPin, LOW);
  
  Serial.println("\n╔════════════════════════════════════════╗");
  Serial.println("║  VIBRATION SENSOR DIAGNOSTIC TEST     ║");
  Serial.println("╚════════════════════════════════════════╝\n");
  
  Serial.println("📍 Sensor Pin: GPIO 15");
  Serial.println("🔊 Buzzer Pin: GPIO 12\n");
  
  // Initial reading
  delay(500);
  int reading = digitalRead(vibrationSensorPin);
  Serial.printf("🔍 Initial sensor reading: %s\n", reading ? "HIGH" : "LOW");
  
  if (reading == LOW) {
    Serial.println("✅ Sensor baseline: LOW (normal for active-high sensor)");
  } else {
    Serial.println("⚠️  Sensor baseline: HIGH (sensor may be triggered or wired incorrectly)");
  }
  
  Serial.println("\n📋 INSTRUCTIONS:");
  Serial.println("1. Watch the real-time readings below");
  Serial.println("2. Tap or move the sensor");
  Serial.println("3. Adjust the potentiometer on the sensor:");
  Serial.println("   - Turn CLOCKWISE = MORE sensitive");
  Serial.println("   - Turn COUNTER-CLOCKWISE = LESS sensitive");
  Serial.println("4. When sensor detects vibration, buzzer will beep\n");
  Serial.println("Starting continuous monitoring...\n");
}

void loop() {
  static int lastReading = -1;
  static unsigned long lastPrint = 0;
  static int transitionCount = 0;
  
  int currentReading = digitalRead(vibrationSensorPin);
  unsigned long currentTime = millis();
  
  // Print reading every 500ms
  if (currentTime - lastPrint >= 500) {
    Serial.printf("[%lu ms] Sensor: %s | Transitions: %d\n", 
                  currentTime, 
                  currentReading ? "HIGH" : "LOW",
                  transitionCount);
    lastPrint = currentTime;
  }
  
  // Detect state changes
  if (currentReading != lastReading && lastReading != -1) {
    transitionCount++;
    
    Serial.printf("\n🔔 TRANSITION #%d: %s → %s\n", 
                  transitionCount,
                  lastReading ? "HIGH" : "LOW",
                  currentReading ? "HIGH" : "LOW");
    
    // Beep on any transition
    digitalWrite(buzzerPin, HIGH);
    delay(100);
    digitalWrite(buzzerPin, LOW);
    
    Serial.println("✅ Buzzer beeped!\n");
  }
  
  lastReading = currentReading;
  
  // Small delay to prevent overwhelming serial output
  delay(10);
}
