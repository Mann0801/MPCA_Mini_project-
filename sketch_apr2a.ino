#include <Servo.h>

Servo lidServo;

#define TRIG_PIN 9
#define ECHO_PIN 10
#define SERVO_PIN 6

void setup() {
    pinMode(TRIG_PIN, OUTPUT);
    pinMode(ECHO_PIN, INPUT);
    lidServo.attach(SERVO_PIN);
    lidServo.write(0);   // start with lid closed
    Serial.begin(9600);
}

long getDistance() {
    digitalWrite(TRIG_PIN, LOW);
    delayMicroseconds(2);
    digitalWrite(TRIG_PIN, HIGH);
    delayMicroseconds(10);
    digitalWrite(TRIG_PIN, LOW);

    long duration = pulseIn(ECHO_PIN, HIGH);
    long distance = duration * 0.034 / 2;
    return distance;
}

void loop() {
    long dist = getDistance();
    Serial.print("Distance: ");
    Serial.println(dist);

    if (dist < 20) {         // hand within 20cm
        lidServo.write(90);  // open lid
        delay(3000);         // keep open 3 seconds
        lidServo.write(0);   // close lid
    }

    delay(200);
}#include <Servo.h>

Servo lidServo;

#define TRIG_PIN 9
#define ECHO_PIN 10
#define SERVO_PIN 6

void setup() {
    pinMode(TRIG_PIN, OUTPUT);
    pinMode(ECHO_PIN, INPUT);
    lidServo.attach(SERVO_PIN);
    lidServo.write(0);   // start with lid closed
    Serial.begin(9600);
}

long getDistance() {
    digitalWrite(TRIG_PIN, LOW);
    delayMicroseconds(2);
    digitalWrite(TRIG_PIN, HIGH);
    delayMicroseconds(10);
    digitalWrite(TRIG_PIN, LOW);

    long duration = pulseIn(ECHO_PIN, HIGH);
    long distance = duration * 0.034 / 2;
    return distance;
}

void loop() {
    long dist = getDistance();
    Serial.print("Distance: ");
    Serial.println(dist);

    if (dist < 20) {         // hand within 20cm
        lidServo.write(90);  // open lid
        delay(3000);         // keep open 3 seconds
        lidServo.write(0);   // close lid
    }

    delay(200);
}