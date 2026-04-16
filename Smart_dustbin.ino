#include <Servo.h>              // Include library to control servo motor
#include <Wire.h>               // Include I2C communication library
#include <LiquidCrystal_I2C.h>  // Include LCD I2C library

Servo lidServo;                 // Create servo object to control lid
LiquidCrystal_I2C lcd(0x27, 16, 2); // Create LCD object (address 0x27, 16x2 display)

// Pins
#define TRIG_PIN 9              // Ultrasonic trigger pin
#define ECHO_PIN 10             // Ultrasonic echo pin
#define SERVO_PIN 6             // Servo signal pin
#define IR_PIN 7                // IR sensor pin (detects full bin)
#define FSR_PIN A0              // Force sensor pin (weight detection)

#define DIST_THRESHOLD 20       // Distance threshold (cm) to open lid

bool lidOpen = false;           // Stores whether lid is open
bool waitingToClose = false;    // Stores whether closing delay has started

unsigned long closeDelayTimer = 0; // Stores time when delay to close starts
unsigned long screenTimer = 0;     // Stores last time LCD screen switched

int screen = 0;                 // 0 = screen 1, 1 = screen 2

// -------- Distance Function --------
long getDistance() {            
  digitalWrite(TRIG_PIN, LOW);  // Ensure trigger pin is LOW
  delayMicroseconds(2);         // Small delay for stability

  digitalWrite(TRIG_PIN, HIGH); // Send HIGH pulse to ultrasonic sensor
  delayMicroseconds(10);        // Keep pulse HIGH for 10 microseconds
  digitalWrite(TRIG_PIN, LOW);  // Turn off pulse

  long duration = pulseIn(ECHO_PIN, HIGH, 30000); // Measure echo time (max 30ms)

  if (duration == 0) return 999; // If no echo, return large value (no object)

  return duration * 0.034 / 2;  // Convert time to distance in cm
}

// -------- Setup --------
void setup() {
  pinMode(TRIG_PIN, OUTPUT);   // Set trigger pin as output
  pinMode(ECHO_PIN, INPUT);    // Set echo pin as input
  pinMode(IR_PIN, INPUT);      // Set IR sensor pin as input

  lidServo.attach(SERVO_PIN);  // Attach servo to defined pin
  delay(500);                  // Wait for servo to stabilize
  lidServo.write(0);           // Set lid to closed position (0°)

  lcd.init();                  // Initialize LCD
  lcd.backlight();             // Turn on LCD backlight

  Serial.begin(9600);          // Start serial communication (debugging)
}

// -------- Main Loop --------
void loop() {

  // -------- SENSOR READINGS --------
  long d1 = getDistance();     // First distance reading
  delay(30);                   // Small delay for accuracy
  long d2 = getDistance();     // Second distance reading
  long distance = (d1 + d2) / 2; // Average both readings

  int fsrValue = analogRead(FSR_PIN); // Read weight sensor value (0–1023)
  int irStatus = digitalRead(IR_PIN); // Read IR sensor (LOW = full, HIGH = not full)

  unsigned long currentMillis = millis(); // Get current time in milliseconds

  // -------- SERVO CONTROL --------

  if (irStatus == LOW) {       // If bin is full (IR blocked)
    lidServo.write(0);         // Keep lid closed
    lidOpen = false;           // Update state (lid is closed)
    waitingToClose = false;    // Cancel any closing delay
  }

  else if (distance < DIST_THRESHOLD && distance != 999) { // If hand/object is near
    lidServo.write(90);        // Open lid (90°)
    lidOpen = true;            // Mark lid as open
    waitingToClose = false;    // Cancel any closing delay
  }

  else {                       // If no object detected
    if (lidOpen && !waitingToClose) { // If lid was open and delay not started
      closeDelayTimer = currentMillis; // Start closing timer
      waitingToClose = true;           // Mark delay started
    }

    if (waitingToClose && currentMillis - closeDelayTimer > 3000) { // After 3 sec
      lidServo.write(0);        // Close lid
      lidOpen = false;          // Update state
      waitingToClose = false;   // Reset delay state
    }
  }

  // -------- SCREEN SWITCH (7 SECONDS) --------
  if (currentMillis - screenTimer > 7000) { // If 7 seconds passed
    screen = !screen;          // Toggle between screen 0 and 1
    screenTimer = currentMillis; // Reset screen timer
  }

  // -------- LCD DISPLAY --------
  lcd.setCursor(0, 0);         // Set cursor to first row

  if (screen == 0) {           // If screen 1 is active
    if (irStatus == LOW || fsrValue > 600) { // If bin is full
      lcd.print("Status: FULL "); // Display FULL
    } else {
      lcd.print("Status: OK   "); // Display OK
    }

    lcd.setCursor(0, 1);       // Move to second row
    lcd.print("Weight: ");     
    lcd.print(fsrValue);       // Display weight value
    lcd.print("   ");          // Clear leftover characters
  } 
  else {                       // If screen 2 is active
    if (distance < DIST_THRESHOLD) { // If object detected
      lcd.print("Object: YES  "); // Show YES
    } else {
      lcd.print("Object: NO   "); // Show NO
    }

    lcd.setCursor(0, 1);       // Move to second row
    lcd.print("Dist: ");
    lcd.print(distance);       // Show distance
    lcd.print(" cm   ");       // Add unit and clear space
  }

  delay(100);                  // Small delay for stable operation
}
