#define RL_VALUE 1500     // The fixed resistor you used with MQ-7 (10k ohms)
#define ANALOG_PIN A1        // The analog pin the sensor is connected to
#define CLEAN_AIR_RSRO 3.5 // Datasheet average Rs/Ro in clean air

float Ro = 10.0;  // Will be calculated after warmup 16k

void setup() {
  Serial.begin(9600);
  Serial.println("Warming up sensor for 10 seconds...");
  delay(10000);  // Just warm it up for 10 seconds

  // Get baseline resistance (Ro) in clean air
  Ro = getRo();
  Serial.print("Baseline Ro: ");
  Serial.println(Ro);
}

void loop() {
  float Rs = getRs();         // Read current sensor resistance
  float ratio = Rs / Ro;      // Compare to baseline

  Serial.print("Rs/Ro: ");
  Serial.println(ratio);

  if (ratio < 0.3) {
    Serial.println("⚠️  CO DETECTED! DANGER!");
    // You could turn on a buzzer or LED here
  } else {
    Serial.println("✅ Air seems clean.");
  }

  delay(2000);  // Check every 2 seconds
}

// Read current resistance from sensor
float getRs() {
  int adcValue = analogRead(ANALOG_PIN);
  float voltage = (adcValue * 5.0) / 1023.0;

  // Formula from datasheet: Rs/RL = (5 - Vout) / Vout
  float Rs = ((5.0 - voltage) / voltage) * RL_VALUE;
  return Rs;
}

// Take 50 readings, average them, and calculate Ro
float getRo() {
  float sum = 0;
  for (int i = 0; i < 50; i++) {
    sum += getRs();
    delay(100);
  }
  float avgRs = sum / 50.0;
  return avgRs / CLEAN_AIR_RSRO;  // From datasheet graph
}




/// code for baseline
// --- Configuration ---
const int MQ135_PIN = A1;
const float RL_MQ135 = 1.5;       // kΩ
const float RO_MQ135 = 2.4;       // Your sensor's Ro (or a close guess)

void setup() {
  Serial.begin(9600);
  Serial.println("Calibrating MQ-135 in current air environment...");
  delay(1000);
  
  float baseline = calculateBaseline(200);  // 200 samples = ~20 seconds
  Serial.print("🔥 Suggested ratioBaseline = ");
  Serial.println(baseline, 2);
}

void loop() {
  // Nothing here for now
}

// --- Averaging function ---
float calculateBaseline(int samples) {
  float sum = 0;
  for (int i = 0; i < samples; i++) {
    int analogVal = analogRead(MQ135_PIN);
    float voltage = analogVal * 5.0 / 1023.0;
    float rs = (5.0 - voltage) * RL_MQ135 / voltage;
    float ratio = rs / RO_MQ135;

    sum += ratio;
    delay(100);  // 100ms delay = 10 readings per second
  }
  return sum / samples;
}
///
