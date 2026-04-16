# Smart Dustbin - Arduino Project

Automated waste bin with hand proximity detection, lid control, fill-level monitoring, and weight sensing.

## Components

| Component | Model | Pins |
|-----------|-------|------|
| Microcontroller | Arduino Uno | - |
| Ultrasonic Sensor | HC-SR04 | Trig: 9, Echo: 10 |
| IR Sensor | Generic IR Module | Digital: 7 |
| Force Sensor | FSR402 | Analog: A0 |
| Servo Motor | SG90 | Digital: 6 |
| LCD Display | 16x2 I2C | SDA: A4, SCL: A5 |

## Wiring Table

### HC-SR04 Ultrasonic Sensor
| Pin | Arduino Pin |
|-----|-------------|
| VCC | 5V |
| GND | GND |
| Trig | Pin 9 |
| Echo | Pin 10 |

### IR Sensor Module
| Pin | Arduino Pin |
|-----|-------------|
| VCC | 5V |
| GND | GND |
| OUT | Pin 7 |

### FSR402 Force Sensor
| Wire | Arduino Pin |
|------|-------------|
| Red (+) |
| Black (-) | GND |


### SG90 Servo Motor
| Wire | Arduino Pin |
|------|-------------|
| Red (VCC) | 5V |
| Brown (GND) | GND |
| Orange (Signal) | Pin 6 |

**Note:** If powered separately, connect GND to Arduino GND.

### 16x2 I2C LCD
| Pin | Arduino Pin |
|-----|-------------|
| VCC | 5V |
| GND | GND |
| SDA | A4 |
| SCL | A5 |

## Libraries Required

Install via Arduino Library Manager (`Sketch > Include Library > Manage Libraries`):

| Library | Author | Purpose |
|---------|--------|---------|
| LiquidCrystal_I2C | Frank de Brabander | I2C LCD control |

Built-in libraries (no installation needed):
- Servo.h
- Wire.h

## Behavior

### Proximity Detection
- Ultrasonic sensor continuously measures distance
- Hand detected within 10 cm triggers lid opening
- 3-second no-hand timer starts after hand removal

### Lid Control
- Lid opens to 90° when hand detected
- Lid closes to 0° after 3 seconds without detection
- Smooth animation using servo stepping
- Non-blocking (uses millis, not delay)

### Fill Level Detection
- IR sensor detects if bin is full (blocked = full)
- LCD line 1 shows "BIN FULL!" when full

### Weight Monitoring
- FSR sensor measures weight on bin floor
- Three levels: Low (<300), Medium (300-600), High (>600)
- Displayed on LCD line 2

### LCD Display States
| State | Line 1 | Line 2 |
|-------|--------|--------|
| Ready | Ready | Weight |
| Full | BIN FULL! | Weight |

## File Structure

```
smart_dustbin/
├── smart_dustbin.ino    # Main sketch (includes, setup, loop)
├── ultrasonic.ino      # HC-SR04 distance function
├── ir_sensor.ino        # IR bin-full detection
├── fsr.ino              # FSR weight level function
├── servo_control.ino    # Servo lid control
├── lcd_display.ino      # LCD initialization and display
└── README.md            # This file
```

## Installation

1. **Install Library**
   - Open Arduino IDE
   - Go to `Sketch > Include Library > Manage Libraries`
   - Search for "LiquidCrystal_I2C"
   - Install "LiquidCrystal I2C" by Frank de Brabander

2. **Upload Sketch**
   - Open `smart_dustbin.ino` in Arduino IDE
   - Select `Tools > Board > Arduino Uno`
   - Select `Tools > Port > COMX` (or /dev/ttyACM0 on Linux)
   - Click Upload (Ctrl+U)

3. **Wire Components**
   - Follow wiring table above
   - Ensure all GND pins are common
   - Double-check I2C address (0x27 default)

## Troubleshooting

### LCD not displaying
- Check I2C address with I2C Scanner sketch
- Verify SDA/A4 and SCL/A5 connections
- Ensure library is installed correctly

### Servo not moving
- Verify 5V power supply can deliver enough current
- Check signal pin connection (Pin 6)
- Ensure servo.write() values are within range (0-180)

### Ultrasonic reading 0
- Check Trig/Echo pin connections
- Ensure pins are set as OUTPUT/INPUT correctly
- Verify power supply to sensor

### IR sensor always shows full/empty
- Adjust sensor position/distance from bin contents
- Check digital pin connection (Pin 7)
- Invert logic in `isBinFull()` if needed

### Serial Monitor shows nothing
- Set baud rate to 9600 in Serial Monitor
- Check USB cable supports data (not power-only)
