// ============================================================
// IR Sensor Module (Bin Full Detection)
// ============================================================
// IR sensor detects if bin is full.
// When bin is not full: IR sensor sees open space (digital LOW).
// When bin is full: IR beam is blocked (digital HIGH).
// ============================================================

// ---- CONSTANTS ----
const int IR_THRESHOLD_LOW = HIGH;   // LOW = nothing detected (bin not full)

// ---- FUNCTION ----

// Reads IR sensor to determine if bin is full
// irPin: Digital pin connected to IR sensor output
// Returns: true if bin is full (IR beam blocked), false otherwise
bool isBinFull(int irPin) {
  int irValue = digitalRead(irPin);

  // IR sensor outputs LOW when object detected (bin not full)
  // IR sensor outputs HIGH when no object (bin empty above sensor)
  // We want to detect when bin is FULL - sensor blocked = HIGH
  // Note: Adjust logic based on your IR sensor's behavior

  // Active LOW sensors (most common): returns true when pin is LOW
  // Active HIGH sensors: returns true when pin is HIGH
  // Below uses active LOW logic - modify if your sensor is active HIGH

  // For most IR modules: LOW = object detected, HIGH = no object
  // So bin full = sensor blocked = likely returns LOW
  // Adjust based on your specific module

  return (irValue == LOW);
}
