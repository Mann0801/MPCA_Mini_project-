#include <Servo.h>   // Include servo library to control motor
Servo lidServo;      // Create a servo object

#define IR_PIN 7     // Define IR sensor pin (connected to pin 7)
#define SERVO_PIN 6  // Define servo signal pin (connected to pin 6)

bool lidOpen = false;            // Variable to store lid state (open/closed)
unsigned long openTime = 0;      // Stores time when lid was opened
unsigned long openDuration = 3000; // Time (in ms) to keep lid open (3 sec)

void setup() {
    pinMode(IR_PIN, INPUT);        // Set IR sensor pin as input
    lidServo.attach(SERVO_PIN);    // Attach servo to pin 6
    lidServo.write(0);             // Set initial position (0° = lid closed)
    Serial.begin(9600);            // Start serial communication for debugging
    Serial.println("System Started by IR Sensor...");  // Print startup message
}

void loop() {
    int irState = digitalRead(IR_PIN);   // Read IR sensor value (HIGH or LOW)
    Serial.print("IR State: ");          // Print label
    Serial.println(irState);             // Print sensor value

    // 🔹 OPEN LID (only if object detected AND lid is currently closed)
    if (irState == LOW && !lidOpen) {    
        Serial.println("Object Detected in IR Sensor!");
        lidServo.write(90);      // Rotate servo to 90° (open lid)
        Serial.println("Lid Open granted by IR Sensor");
        lidOpen = true;          // Update state → lid is now open
        openTime = millis();     // Store current time when lid opened
    }

    // 🔹 CLOSE LID after 3 seconds (non-blocking, no delay used)
    if (lidOpen && (millis() - openTime >= openDuration)) {
        lidServo.write(0);       // Rotate servo back to 0° (close lid)
        Serial.println("Lid Closed by IR Sensor");
        lidOpen = false;         // Update state → lid is now closed
    }
}