#include <Servo.h>
Servo lidServo;

#define FSR_PIN A0
#define SERVO_PIN 6
#define FSR_THRESHOLD 300   // tune this based on your setup

void setup() {
    lidServo.attach(SERVO_PIN);
    lidServo.write(0);
    Serial.begin(9600);
}

void loop() {
    int fsrValue = analogRead(FSR_PIN);
    Serial.print("FSR Value: ");
    Serial.println(fsrValue);

    if (fsrValue > FSR_THRESHOLD) {   // bin is full / weight detected
        lidServo.write(90);            // open lid (or use as alert)
        delay(3000);
        lidServo.write(0);
    }
    delay(200);
}