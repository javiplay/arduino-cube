// MASTER Arduino with MCUFRIEND touchscreen

#include <MCUFRIEND_kbv.h>
#include <TouchScreen.h>
#include <Adafruit_GFX.h>

MCUFRIEND_kbv tft;

// CORRECT PIN CONFIGURATION FOR YOUR SCREEN
#define YP A1  // Changed from A3 to A1
#define XM A2
#define YM 7   // Changed from 9 to 7
#define XP 6   // Changed from 8 to 6

// Touchscreen calibration - these values will be adjusted later
#define TS_MINX 100
#define TS_MAXX 920
#define TS_MINY 70
#define TS_MAXY 900

// Screen constants
#define BLACK   0x0000
#define BLUE    0x001F
#define RED     0xF800
#define GREEN   0x07E0
#define CYAN    0x07FF
#define MAGENTA 0xF81F
#define YELLOW  0xFFE0
#define WHITE   0xFFFF

// Pressure threshold adjusted according to your values (130-166)
TouchScreen ts = TouchScreen(XP, YP, XM, YM, 300);

// Variables to control the servo
int servoPos = 90; // Initial servo position (0-180)

void setup() {
  Serial.begin(9600);

  // Initialize the screen
  uint16_t ID = tft.readID();
  tft.begin(ID);
  tft.setRotation(1); // Adjust orientation if necessary

  // Draw the interface
  tft.fillScreen(BLACK);
  drawInterface();
}

void loop() {
  // Get touch input
  TSPoint p = ts.getPoint();

  // Need to restore pins as outputs after ts.getPoint()
  pinMode(YP, OUTPUT);
  pinMode(XM, OUTPUT);

  if (p.z > ts.pressureThreshhold) {
    // Convert touch coordinates to screen coordinates
    p.x = map(p.x, TS_MINX, TS_MAXX, 0, tft.width());
    p.y = map(p.y, TS_MINY, TS_MAXY, 0, tft.height());

    // Process the touch
    processTouch(p.x, p.y);

    // Debounce
    delay(200);
  }

  delay(50); // Small pause for stability
}

void drawInterface() {
  // Title
  tft.setTextColor(WHITE);
  tft.setTextSize(3);
  tft.setCursor(20, 10);
  tft.print("Servo Control");

  // Left button (-)
  tft.fillRect(20, 80, 80, 80, RED);
  tft.setTextColor(WHITE);
  tft.setTextSize(4);
  tft.setCursor(45, 105);
  tft.print("-");

  // Center button (90)
  tft.fillRect(120, 80, 80, 80, BLUE);
  tft.setTextSize(3);
  tft.setCursor(135, 105);
  tft.print("90");

  // Right button (+)
  tft.fillRect(220, 80, 80, 80, GREEN);
  tft.setTextSize(4);
  tft.setCursor(245, 105);
  tft.print("+");

  // Button labels
  tft.setTextSize(1);
  tft.setCursor(30, 170);
  tft.print("-10 degrees");
  tft.setCursor(135, 170);
  tft.print("Center");
  tft.setCursor(230, 170);
  tft.print("+10 degrees");

  // Show current position
  updatePositionDisplay();
}

void updatePositionDisplay() {
  // Clear position area
  tft.fillRect(80, 200, 160, 35, BLACK);

  // Show current position
  tft.setTextColor(YELLOW);
  tft.setTextSize(3);
  tft.setCursor(90, 205);
  tft.print("Pos: ");
  tft.print(servoPos);
  tft.print((char)167); // Degree symbol
}

void processTouch(int x, int y) {
  bool changed = false;

  // Check if left button (-) was touched
  if (x > 20 && x < 100 && y > 80 && y < 160) {
    servoPos = max(0, servoPos - 10);
    Serial.write('L'); // Send command to move left
    changed = true;
  }

  // Check if right button (+) was touched
  else if (x > 220 && x < 300 && y > 80 && y < 160) {
    servoPos = min(180, servoPos + 10);
    Serial.write('R'); // Send command to move right
    changed = true;
  }

  // Check if center button (90) was touched
  else if (x > 120 && x < 200 && y > 80 && y < 160) {
    servoPos = 90;
    Serial.write('C'); // Send command to center
    changed = true;
  }

  // Update the screen only if there was a change
  if (changed) {
    updatePositionDisplay();

    // Visual feedback - flash the touched area
    delay(100);
  }
}
