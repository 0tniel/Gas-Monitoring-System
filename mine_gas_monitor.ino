#include <DHT.h>

// --- DHT Sensor ---
#define DHTPIN A3
#define DHTTYPE DHT11   // Change to DHT22 if needed
DHT dht(DHTPIN, DHTTYPE);

// --- Pins for Buzzer & LED ---
#define BUZZER_PIN 8
#define LED_PIN 9

// --- MQ-7 (CO) Sensor ---
const int MQ7_PIN = A0;
const float RL_MQ7 = 2.0;     // kΩ
const float RO_MQ7 = 1.7;     // Calibrated baseline
const float MQ7_m = -1.183;
const float MQ7_c = 1.3;

// --- MQ-135 Sensor ---
const int MQ135_PIN = A1;
const float RL_MQ135 = 1.5;     // kΩ
const float RO_MQ135 = 2.4;     // Calibrated baseline
const float ratioBaseline = 2.3; // Clean air Rs/Ro baseline

// --- Moving Average (MQ-135) ---
const int MA_WINDOW_SIZE = 10;
float rsRoHistory[MA_WINDOW_SIZE];
int currentIndex = 0;
float sum = 0;

// --- Thresholds ---
#define CO_THRESHOLD 30.0       // ppm
#define TGI_THRESHOLD 60.0      // Toxic Gas Index

void setup() {
  Serial.begin(9600);

  // Init moving average history
  for (int i = 0; i < MA_WINDOW_SIZE; i++) {
    rsRoHistory[i] = 0;
  }

  // Sensor warm-up
  Serial.println("Warming up sensors...");
  delay(10000);

  dht.begin();
  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(LED_PIN, OUTPUT);
}

void loop() {
  // --- MQ-7 CO ---
  int analogMQ7 = analogRead(MQ7_PIN);
  float voltageMQ7 = (analogMQ7 / 1023.0) * 5.0;
  float rsMQ7 = (5.0 - voltageMQ7) * RL_MQ7 / voltageMQ7;
  float ratioMQ7 = rsMQ7 / RO_MQ7;
  float ppmCO = pow(10, MQ7_m * log10(ratioMQ7) + MQ7_c);

  // --- MQ-135 Toxic Gas ---
  int analogMQ135 = analogRead(MQ135_PIN);
  float voltageMQ135 = (analogMQ135 / 1023.0) * 5.0;
  float rsMQ135 = (5.0 - voltageMQ135) * RL_MQ135 / voltageMQ135;
  float ratioMQ135 = rsMQ135 / RO_MQ135;

  // --- Moving Average for Rs/Ro ---
  sum -= rsRoHistory[currentIndex];
  rsRoHistory[currentIndex] = ratioMQ135;
  sum += ratioMQ135;
  currentIndex = (currentIndex + 1) % MA_WINDOW_SIZE;
  float movingAvg = sum / MA_WINDOW_SIZE;

  // --- TGI Calculation ---
  float delta = ratioBaseline - movingAvg;
  if (delta < 0) delta = 0;
  if (delta > ratioBaseline) delta = ratioBaseline;
  float TGI = (delta / ratioBaseline) * 100.0;

  // --- DHT Temp & Humidity ---
  float temp = dht.readTemperature();
  float hum = dht.readHumidity();

  // --- Alerts ---
  bool coDanger = ppmCO > CO_THRESHOLD;
  bool gasDanger = TGI > TGI_THRESHOLD;

  if (coDanger || gasDanger) {
    digitalWrite(BUZZER_PIN, HIGH);
    digitalWrite(LED_PIN, HIGH);
  } else {
    digitalWrite(BUZZER_PIN, LOW);
    digitalWrite(LED_PIN, LOW);
  }

  // --- Output ---
  Serial.println("------ Gas Monitor ------");
  Serial.print("MQ-7 CO (A0): Analog = "); Serial.print(analogMQ7);
  Serial.print(" | CO ppm = "); Serial.println(ppmCO, 2);

  Serial.print("MQ-135 (A1): Analog = "); Serial.print(analogMQ135);
  Serial.print(" | Rs/Ro = "); Serial.println(ratioMQ135, 2);
  Serial.print("Moving Avg Rs/Ro: "); Serial.println(movingAvg, 2);

  Serial.print("TGI: "); Serial.print(TGI, 2);
  Serial.print(" | delta: "); Serial.println(delta, 2);

  Serial.print("Temp (°C): "); Serial.print(temp);
  Serial.print(" | Humidity (%): "); Serial.println(hum);

  Serial.print("ALERT: ");
  if (coDanger) Serial.print("⚠️ CO ");
  if (gasDanger) Serial.print("⚠️ Toxic Gas ");
  if (!coDanger && !gasDanger) Serial.print("✅ Air seems safe");
  Serial.println("\n--------------------------\n");

  delay(2000);
}
