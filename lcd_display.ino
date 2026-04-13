// ============================================================
// 16x2 I2C LCD Display Module
// ============================================================
// Displays bin status and weight information.
// Uses I2C communication (SDA: A4, SCL: A5).
// Address: 0x27 (default for most PCF8574 modules)
// ============================================================

// ---- CONSTANTS ----
const int LCD_BACKLIGHT_TIMEOUT_MS = 0;  // 0 = always on (disabled)

// ---- INTERNAL STATE ----
String lastLine1 = "";   // Last displayed line 1
String lastLine2 = "";  // Last displayed line 2

// ---- FUNCTIONS ----

// Initializes the LCD display
// Turns on backlight, clears screen, shows cursor off
void initLCD(LiquidCrystal_I2C &lcd) {
  lcd.init();
  lcd.backlight();
  lcd.clear();
  lcd.noCursor();

  // Welcome message
  lcd.setCursor(0, 0);
  lcd.print("Smart Dustbin");
  lcd.setCursor(0, 1);
  lcd.print("Initializing...");
  delay(1000);
}

// Clears the LCD screen
void clearLCD(LiquidCrystal_I2C &lcd) {
  lcd.clear();
}

// Sets cursor position
// col: Column (0-15)
// row: Row (0-1)
void setLCDCursor(LiquidCrystal_I2C &lcd, int col, int row) {
  lcd.setCursor(col, row);
}

// Prints text at specific position
void printLCDAt(LiquidCrystal_I2C &lcd, int col, int row, String text) {
  lcd.setCursor(col, row);
  lcd.print(text);
}

// Displays status on LCD (updates only changed lines)
// line1: Text for row 0 (16 characters max)
// line2: Text for row 1 (16 characters max)
void showStatus(LiquidCrystal_I2C &lcd, String line1, String line2) {
  // Only update if content changed (reduces flickering)

  if (line1 != lastLine1) {
    lcd.setCursor(0, 0);
    lcd.print("                ");  // Clear line (16 spaces)
    lcd.setCursor(0, 0);
    lcd.print(line1);
    lastLine1 = line1;
  }

  if (line2 != lastLine2) {
    lcd.setCursor(0, 1);
    lcd.print("                ");  // Clear line (16 spaces)
    lcd.setCursor(0, 1);
    lcd.print(line2);
    lastLine2 = line2;
  }
}

// Turns LCD backlight on
void lcdBacklightOn(LiquidCrystal_I2C &lcd) {
  lcd.backlight();
}

// Turns LCD backlight off
void lcdBacklightOff(LiquidCrystal_I2C &lcd) {
  lcd.noBacklight();
}

// Displays a countdown message
// seconds: Countdown value to display
void showCountdown(LiquidCrystal_I2C &lcd, int seconds) {
  lcd.setCursor(14, 0);
  lcd.print(seconds);
}

// Shows loading animation (moves cursor across screen)
void showLoading(LiquidCrystal_I2C &lcd) {
  static int loadingPos = 0;
  lcd.setCursor(loadingPos, 1);
  lcd.print("*");
  loadingPos = (loadingPos + 1) % 16;
}
