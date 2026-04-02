# MPCA_Mini_project-
Smart Dustbin — Automated Waste Management System
Overview
An IoT-based smart dustbin built on Arduino Uno that automates lid control using proximity detection, monitors waste fill level using infrared sensing, measures garbage weight using a Force Sensitive Resistor, and displays real-time status on an LCD screen. Designed to minimize human contact with waste bins, improving hygiene in public and household environments.

Tech Stack

Microcontroller: Arduino Uno (ATmega328P, 16MHz, 32KB Flash)
IDE: Arduino IDE 2.3.8
Language: C++ (Arduino framework)
Communication Protocol: I2C (LCD), Digital I/O (sensors), PWM (servo)


Hardware Components
ComponentModelPurposeMicrocontrollerArduino UnoCentral processing unitProximity SensorHC-SR04Hand detection via ultrasonic wavesFill SensorIR SensorDetects if bin is fullWeight SensorFSR (Force Sensitive Resistor)Measures garbage weight via analog voltageDisplay16x2 LCD (I2C)Real-time status outputActuatorSG90 Servo MotorMechanical lid control

System Architecture
HC-SR04 ──────────────────────────────────┐
IR Sensor ────────────────────────────────┤
FSR Sensor (A0) ──────────────────────────┤──→ Arduino Uno ──→ Servo Motor (lid)
LCD (I2C - SDA/SCL) ←─────────────────────┘

Working Principle
Proximity Detection (HC-SR04)
The sensor emits a 10 microsecond ultrasonic pulse at 40kHz via the TRIG pin. The pulse travels through air, bounces off the nearest object, and returns to the ECHO pin. The time-of-flight is captured using pulseIn() and converted to distance using:
distance (cm) = duration × 0.034 / 2
If distance < 20cm, lid open sequence is triggered.
Lid Control (Servo Motor)
The SG90 servo operates on PWM signal. lidServo.write(90) rotates the horn 90 degrees, physically lifting the lid. After a 3 second delay, lidServo.write(0) returns it to closed position.
Fill Level Detection (IR Sensor)
The IR sensor emits infrared light inside the bin. If the bin is full, the garbage reflects the IR signal back to the receiver. The digital output goes LOW when the bin is full, HIGH when empty.
Weight Measurement (FSR)
The FSR is connected in a voltage divider circuit on pin A0. As weight increases, resistance decreases, increasing the analog voltage reading (0-1023). This value is mapped to a weight range and displayed on the LCD.
Display (16x2 LCD via I2C)
Uses the LiquidCrystal_I2C library. Displays two lines — fill status (Full/Not Full) and weight reading — updating every loop cycle.

Pin Configuration
PinComponentModeD9HC-SR04 TRIGOUTPUTD10HC-SR04 ECHOINPUTD6Servo SignalPWM OUTPUTD7IR Sensor OUTINPUTA0FSRANALOG INPUTSDA/SCLLCD I2CI2C

Flow

Arduino powers on → servo initializes to 0° (closed)
Loop starts → ultrasonic reads distance every 200ms
Distance < 20cm → servo opens lid to 90°
FSR reads analog weight → mapped to grams
IR checks fill level → digital HIGH/LOW
LCD updates with weight and fill status
3 second delay → servo closes lid
Loop repeats


Libraries Used

Servo.h — built-in Arduino servo control
Wire.h — I2C communication
LiquidCrystal_I2C.h — LCD control over I2C


Future Improvements

GSM module to send SMS alert when bin is full
Data logging to SD card
Solar powered operation
Multiple bin monitoring via central dashboard
