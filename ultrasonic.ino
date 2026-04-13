// ============================================================
// HC-SR04 Ultrasonic Sensor Module
// ============================================================
// Measures distance using sound waves.
// Trig pin sends ultrasound pulse.
// Echo pin receives the reflected pulse.
// Distance = (travel time / 2) * speed of sound (~0.034 cm/us)
// ============================================================

// ---- CONSTANTS ----
const float SOUND_SPEED_CM_PER_US = 0.034;

// ---- FUNCTION ----

// Reads distance from HC-SR04 sensor
// trigPin: Pin connected to Trig (output)
// echoPin: Pin connected to Echo (input)
// Returns: Distance in centimeters as float (0.0 if no echo)
float getDistance(int trigPin, int echoPin) {
  // Set Trig LOW for 2us (clean pulse start)
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);

  // Send 10us HIGH pulse to trigger sensor
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  // Read Echo pin - pulseIn returns pulse duration in microseconds
  // timeout of 30000us (~5m max distance) prevents blocking
  long duration = pulseIn(echoPin, HIGH, 30000);

  // No echo detected
  if (duration == 0) {
    return 0.0;
  }

  // Calculate distance: (duration / 2) * speed of sound
  // Divide by 2 because pulse travels to object AND back
  float distance = (duration * SOUND_SPEED_CM_PER_US) / 2.0;

  return distance;
}
