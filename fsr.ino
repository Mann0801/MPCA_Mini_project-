// ============================================================
// FSR402 Force Sensitive Resistor Module
// ============================================================
// FSR402 measures weight/force applied to the bin floor.
// Returns analog value (0-1023) mapped to weight levels.
// ============================================================

// ---- CONSTANTS ----
const int FSR_THRESHOLD_LOW    = 300;   // Below 300 = Low weight
const int FSR_THRESHOLD_MEDIUM = 600;   // 300-600 = Medium weight
// Above 600 = High weight

const int FSRCALIBRATE_SAMPLES = 10;     // Samples for averaging

// ---- FUNCTIONS ----

// Reads raw analog value from FSR sensor
// fsrPin: Analog pin connected to FSR
// Returns: Averaged ADC value (0-1023)
int readFSRAnalog(int fsrPin) {
  long total = 0;

  // Take multiple samples and average for stability
  for (int i = 0; i < FSRCALIBRATE_SAMPLES; i++) {
    total += analogRead(fsrPin);
    delay(5);  // Small delay between samples
  }

  return total / FSRCALIBRATE_SAMPLES;
}

// Determines weight level from FSR reading
// fsrPin: Analog pin connected to FSR
// Returns: "Low", "Medium", or "High" based on thresholds
String getWeightLevel(int fsrPin) {
  int fsrValue = readFSRAnalog(fsrPin);

  if (fsrValue < FSR_THRESHOLD_LOW) {
    return "Low";
  } else if (fsrValue < FSR_THRESHOLD_MEDIUM) {
    return "Medium";
  } else {
    return "High";
  }
}

// Returns raw FSR value for debugging
// fsrPin: Analog pin connected to FSR
// Returns: Raw ADC value (0-1023)
int getFSRRawValue(int fsrPin) {
  return readFSRAnalog(fsrPin);
}
