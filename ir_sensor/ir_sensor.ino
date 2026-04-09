#define IR_PIN 7   // IR sensor is connected to digital pin 7

// Variables for FULL detection logic
unsigned long detectStartTime = 0;   // stores time when detection started
bool detecting = false;              // tracks if detection is ongoing
bool isFull = false;                 // stores final dustbin status (full or not)

// Variables for periodic printing
unsigned long lastPrintTime = 0;     
unsigned long printInterval = 5000;  // print every 5 seconds

void setup() {
    pinMode(IR_PIN, INPUT);          // set IR sensor pin as input
    Serial.begin(9600);              // start serial communication
    Serial.println("Dustbin Monitoring Started..."); // initial message
}

void loop() {
    int irState = digitalRead(IR_PIN);   // read IR sensor value (HIGH or LOW)

    // 🔹 FULL detection logic (object must be detected continuously for 5 sec)
    if (irState == LOW) {   // LOW means object detected
        if (!detecting) {   // if detection just started
            detectStartTime = millis();  // store start time
            detecting = true;            // mark detection active
        }
        // if detection continues for 5 seconds → mark dustbin as FULL
        if (millis() - detectStartTime >= 5000) {
            isFull = true;
        }
    } 

    else {
        detecting = false;   // reset detection if object is removed
        isFull = false;      // dustbin is not full anymore
    }

    // 🔹 Print status every 5 seconds
    if (millis() - lastPrintTime >= printInterval) {
        // Print object detection status
        if (irState == LOW) {
            Serial.print("Object: DETECTED | ");
        } else {
            Serial.print("Object: NOT DETECTED | ");
        }

        // Print dustbin status
        if (isFull) {
            Serial.println("Dustbin: FULL");
        } else {
            Serial.println("Dustbin: NOT FULL");
        }
        lastPrintTime = millis();   // update last print time
    }
}