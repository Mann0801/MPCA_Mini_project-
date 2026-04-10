// ============================================================
// Smart Dustbin - Main Sketch
// ============================================================

// ---- INCLUDES ----
#include <Servo.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// ---- CONSTANTS ----
const int TRIG_PIN      = 9;
const int ECHO_PIN      = 10;
const int IR_PIN        = 7;
const int FSR_PIN       = A0;
const int SERVO_PIN     = 6;
const int LCD_ADDR      = 0x27;
const int LCD_COLS      = 16;
const int LCD_ROWS      = 2;

const float HAND_THRESHOLD_CM = 10.0;
const unsigned long CLOSE_DELAY_MS = 3000;
const unsigned long LID_ANIMATION_MS = 500;

// ---- STATES ----
enum LidState {
  STATE_IDLE,
  STATE_OPENING,
  STATE_OPEN,
  STATE_CLOSING
};

// ---- GLOBAL OBJECTS ----
LiquidCrystal_I2C lcd(LCD_ADDR, LCD_COLS, LCD_ROWS);

// ---- GLOBAL VARIABLES ----
LidState lidState = STATE_IDLE;
unsigned long lastHandTime = 0;
unsigned long lidAnimStart = 0;
float currentDistance = 0.0;
bool binFull = false;
String weightLevel = "Low";

bool handPresent = false;

// ============================================================
// FUNCTION DECLARATIONS (defined in other tabs)
// ============================================================
float getDistance(int trigPin, int echoPin);
bool isBinFull(int irPin);
String getWeightLevel(int fsrPin);

void initLCD(LiquidCrystal_I2C &lcd);
void showStatus(LiquidCrystal_I2C &lcd, String line1, String line2);

void openLid();
void closeLid();
bool isLidAnimating();
void updateLidAnimation();

// ============================================================
// SETUP
// ============================================================
void setup() {
  Serial.begin(9600);
  Serial.println("=== Smart Dustbin Starting ===");

  // Initialize components
  initLCD(lcd);

  // Initial status display
  showStatus(lcd, "Ready", "Low");
  Serial.println("LCD initialized");

  // Servo starts closed
  closeLid();
  Serial.println("Servo initialized to closed position");

  Serial.println("=== Setup Complete ===");
}

// ============================================================
// MAIN LOOP
// ============================================================
void loop() {
  // Read sensors
  currentDistance = getDistance(TRIG_PIN, ECHO_PIN);
  binFull = isBinFull(IR_PIN);
  weightLevel = getWeightLevel(FSR_PIN);

  // Debug output
  Serial.print("Distance: ");
  Serial.print(currentDistance);
  Serial.print(" cm | IR Full: ");
  Serial.print(binFull ? "YES" : "NO");
  Serial.print(" | Weight: ");
  Serial.print(weightLevel);
  Serial.print(" | State: ");
  Serial.println(lidState == STATE_IDLE ? "IDLE" :
                  lidState == STATE_OPENING ? "OPENING" :
                  lidState == STATE_OPEN ? "OPEN" : "CLOSING");

  // Detect hand presence
  handPresent = (currentDistance < HAND_THRESHOLD_CM && currentDistance > 0.0);

  // State machine
  switch (lidState) {
    case STATE_IDLE:
      if (handPresent) {
        Serial.println("Hand detected! Opening lid...");
        openLid();
        lidState = STATE_OPENING;
        lidAnimStart = millis();
        lastHandTime = millis();
      }
      break;

    case STATE_OPENING:
      if (millis() - lidAnimStart >= LID_ANIMATION_MS) {
        lidState = STATE_OPEN;
        Serial.println("Lid fully open");
      }
      lastHandTime = millis();
      break;

    case STATE_OPEN:
      if (handPresent) {
        lastHandTime = millis();
      }
      if (millis() - lastHandTime >= CLOSE_DELAY_MS) {
        Serial.println("No hand for 3s. Closing lid...");
        closeLid();
        lidState = STATE_CLOSING;
        lidAnimStart = millis();
      }
      break;

    case STATE_CLOSING:
      if (millis() - lidAnimStart >= LID_ANIMATION_MS) {
        lidState = STATE_IDLE;
        Serial.println("Lid fully closed");
      }
      break;
  }

  // Update LCD based on bin status
  if (binFull) {
    showStatus(lcd, "BIN FULL!", weightLevel);
  } else {
    showStatus(lcd, "Ready", weightLevel);
  }

  delay(100);  // Small delay to prevent LCD flickering
}
