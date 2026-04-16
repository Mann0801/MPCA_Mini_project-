#include <Servo.h>              
#include <Wire.h>               
#include <LiquidCrystal_I2C.h>  

Servo lidServo;                 
LiquidCrystal_I2C lcd(0x27, 16, 2);

// Pins
#define TRIG_PIN 9              
#define ECHO_PIN 10             
#define SERVO_PIN 6             
#define IR_PIN 7                
#define FSR_PIN A0              

#define DIST_THRESHOLD 20       

bool lidOpen = false;           
bool waitingToClose = false;    

unsigned long closeDelayTimer = 0; 
unsigned long screenTimer = 0;     

int screen = 0;                 

// -------- Distance Function --------
long getDistance() {            
  digitalWrite(TRIG_PIN, LOW);  
  delayMicroseconds(2);         

  digitalWrite(TRIG_PIN, HIGH); 
  delayMicroseconds(10);        
  digitalWrite(TRIG_PIN, LOW);  

  long duration = pulseIn(ECHO_PIN, HIGH, 30000); 

  if (duration == 0) return 999; 

  return duration * 0.034 / 2;  
}

// -------- Setup --------
void setup() {
  pinMode(TRIG_PIN, OUTPUT);   
  pinMode(ECHO_PIN, INPUT);    
  pinMode(IR_PIN, INPUT);      

  lidServo.attach(SERVO_PIN);  
  delay(500);                  
  lidServo.write(0);           

  lcd.init();                  
  lcd.backlight();             

  Serial.begin(9600);          
}

// -------- Main Loop --------
void loop() {

  // -------- SENSOR READINGS --------
  long d1 = getDistance();     
  delay(30);                   
  long d2 = getDistance();     
  long distance = (d1 + d2) / 2; 

  int fsrValue = analogRead(FSR_PIN); 
  int irStatus = digitalRead(IR_PIN); 

  unsigned long currentMillis = millis(); 

  // -------- SERVO CONTROL --------

  // Case 1: Bin FULL → always closed
  if (irStatus == LOW) {       
    lidServo.write(0);         
    lidOpen = false;           
    waitingToClose = false;    
  }

  // Case 2: Object detected → open
  else if (distance < DIST_THRESHOLD && distance != 999) {
    lidServo.write(90);        
    lidOpen = true;            
    waitingToClose = false;    
  }

  // Case 3: Object removed → delayed close
  else {
    if (lidOpen && !waitingToClose) { 
      closeDelayTimer = currentMillis; 
      waitingToClose = true;           
    }

    if (waitingToClose && currentMillis - closeDelayTimer > 3000) { 
      lidServo.write(0);        
      lidOpen = false;          
      waitingToClose = false;   
    }
  }

  // -------- SCREEN SWITCH (UPDATED TO 7 SECONDS) --------
  if (currentMillis - screenTimer > 7000) { // 7 seconds
    screen = !screen;          
    screenTimer = currentMillis; 
  }

  // -------- LCD DISPLAY --------
  lcd.setCursor(0, 0);         

  if (screen == 0) {
    // Screen 1: Status + Weight
    if (irStatus == LOW || fsrValue > 600) {
      lcd.print("Status: FULL ");
    } else {
      lcd.print("Status: OK   ");
    }

    lcd.setCursor(0, 1);       
    lcd.print("Weight: ");
    lcd.print(fsrValue);
    lcd.print("   ");
  } 
  else {
    // Screen 2: Ultrasonic Info
    if (distance < DIST_THRESHOLD) {
      lcd.print("Object: YES  ");
    } else {
      lcd.print("Object: NO   ");
    }

    lcd.setCursor(0, 1);       
    lcd.print("Dist: ");
    lcd.print(distance);
    lcd.print(" cm   ");
  }

  delay(100);                  
}
