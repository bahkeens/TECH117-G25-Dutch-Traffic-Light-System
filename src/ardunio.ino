// --- Configuration ---
#define SIGNAL_PIN 5               // Must be pin 5 (T1 input) for external counting
#define CALIBRATION_MS 2000
#define DETECTION_THRESHOLD 0.03f  // ±1%
#define SAMPLE_INTERVAL_MS 2       // high-resolution sampling
#define EMA_ALPHA 0.2f             // smoothing factor
#define PRINT_INTERVAL_MS 500      // update print every 500ms

// --- Calibration and EMA ---
float calibratedHighFreq = 0;
float calibratedLowFreq  = 0;
float freqHighEMA = 0;
float freqLowEMA  = 0;

// --- Setup Timer1 as external counter ---
void setupCounter() {
  pinMode(SIGNAL_PIN, INPUT);
  // Stop timer
  TCCR1A = 0;
  TCCR1B = 0;
  TCNT1 = 0;
  // External clock on T1 pin (rising edge)
  TCCR1B |= (1 << CS12) | (1 << CS11) | (1 << CS10);
}

// --- Read and clear Timer1 counter ---
uint16_t readAndResetCounter() {
  uint16_t count;
  noInterrupts();
  count = TCNT1;
  TCNT1 = 0;
  interrupts();
  return count;
}

// --- Calibration ---
void calibrateFrequency() {
  Serial.println(F("Calibrating HIGH/LOW frequencies..."));
  unsigned long start = millis();
  uint32_t sumHigh = 0, sumLow = 0;
  int cntHigh = 0, cntLow = 0;
  bool isHigh = false;

  while (millis() - start < CALIBRATION_MS) {
    delay(50);
    uint16_t count = readAndResetCounter();
    float freq = count * (1000.0f / 50.0f);

    if (isHigh) { sumHigh += freq; cntHigh++; }
    else        { sumLow  += freq; cntLow++; }

    isHigh = !isHigh;
  }

  calibratedHighFreq = cntHigh ? (float)sumHigh / cntHigh : 0;
  calibratedLowFreq  = cntLow  ? (float)sumLow  / cntLow  : 0;

  freqHighEMA = calibratedHighFreq;
  freqLowEMA  = calibratedLowFreq;

  Serial.print(F("Calibration complete. HIGH="));
  Serial.print(calibratedHighFreq, 2);
  Serial.print(F(" Hz, LOW="));
  Serial.println(calibratedLowFreq, 2);
}

// --- Setup ---
void setup() {
  Serial.begin(115200);
  setupCounter();
  calibrateFrequency();
}

// --- Loop ---
void loop() {
  static unsigned long lastSample = 0;
  static unsigned long lastPrint = 0;
  static bool isHigh = false;
  static bool deviationHigh = false;
  static bool deviationLow  = false;

  unsigned long now = millis();

  if (now - lastSample >= SAMPLE_INTERVAL_MS) {
    lastSample = now;
    uint16_t count = readAndResetCounter();
    float freq = count * (1000.0f / SAMPLE_INTERVAL_MS);

    // --- Update EMA per phase ---
    if (isHigh) {
      freqHighEMA = EMA_ALPHA * freq + (1.0f - EMA_ALPHA) * freqHighEMA;
      deviationHigh = fabs(freqHighEMA - calibratedHighFreq) > calibratedHighFreq * DETECTION_THRESHOLD;
    } else {
      freqLowEMA = EMA_ALPHA * freq + (1.0f - EMA_ALPHA) * freqLowEMA;
      deviationLow = fabs(freqLowEMA - calibratedLowFreq) > calibratedLowFreq * DETECTION_THRESHOLD;
    }

    isHigh = !isHigh;
  }

  if (now - lastPrint >= PRINT_INTERVAL_MS) {
    lastPrint = now;
    Serial.print(F("HIGH EMA: "));
    Serial.print(freqHighEMA, 2);
    if (deviationHigh) Serial.print(F(" [Δ>1%]"));
    Serial.print(F(" | LOW EMA: "));
    Serial.print(freqLowEMA, 2);
    if (deviationLow) Serial.print(F(" [Δ>1%]"));
    Serial.println();
  }
}