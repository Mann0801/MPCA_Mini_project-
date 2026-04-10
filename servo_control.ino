// ============================================================
// SG90 Servo Motor Module
// ============================================================
// Controls lid opening and closing.
// Uses millis() for non-blocking animation.
// Servo positions: Closed = 0°, Open = 90°
// ============================================================

// ---- INCLUDES ----
#include <Servo.h>

// ---- CONSTANTS ----
const int SERVO_CLOSED_ANGLE = 0;     // Lid closed position (degrees)
const int SERVO_OPEN_ANGLE   = 90;    // Lid open position (degrees)
const unsigned long SERVO_STEP_DELAY_MS = 15;  // Delay between servo steps

// ---- GLOBAL OBJECTS ----
Servo lidServo;

// ---- INTERNAL STATE ----
int targetAngle = SERVO_CLOSED_ANGLE;      // Target angle for lid
int currentAngle = SERVO_CLOSED_ANGLE;     // Current angle (starts closed)
unsigned long lastServoStep = 0;           // Timestamp of last servo step
bool servoAttached = false;                 // Tracks servo attachment state

// ---- FUNCTIONS ----

// Initializes the servo motor
// Attaches servo to pin and sets initial position
void initServo() {
  lidServo.attach(SERVO_PIN);
  lidServo.write(SERVO_CLOSED_ANGLE);
  currentAngle = SERVO_CLOSED_ANGLE;
  targetAngle = SERVO_CLOSED_ANGLE;
  servoAttached = true;
  lastServoStep = millis();
}

// Called once in setup() - same as initServo but clearer naming
void initServoMotor() {
  initServo();
}

// Starts opening the lid (sets target to open position)
void openLid() {
  if (!servoAttached) {
    lidServo.attach(SERVO_PIN);
    servoAttached = true;
  }
  targetAngle = SERVO_OPEN_ANGLE;
}

// Starts closing the lid (sets target to closed position)
void closeLid() {
  if (!servoAttached) {
    lidServo.attach(SERVO_PIN);
    servoAttached = true;
  }
  targetAngle = SERVO_CLOSED_ANGLE;
}

// Updates servo position incrementally toward target
// Call this frequently in loop() for smooth animation
void updateServo() {
  if (!servoAttached) return;

  unsigned long now = millis();

  // Only move if enough time has passed since last step
  if (now - lastServoStep >= SERVO_STEP_DELAY_MS) {
    lastServoStep = now;

    if (currentAngle < targetAngle) {
      currentAngle++;
      lidServo.write(currentAngle);
    } else if (currentAngle > targetAngle) {
      currentAngle--;
      lidServo.write(currentAngle);
    }
  }
}

// Checks if servo is still moving to target position
// Returns: true if animation in progress, false if at target
bool isLidAnimating() {
  return (currentAngle != targetAngle);
}

// Returns current lid position
// Returns: 0 = closed, 90 = open, values between during animation
int getLidPosition() {
  return currentAngle;
}

// Emergency stop - detaches servo to prevent jitter
void detachServo() {
  if (servoAttached) {
    lidServo.detach();
    servoAttached = false;
  }
}
