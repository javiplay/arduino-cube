// SLAVE Arduino with servo

#include <Servo.h>

Servo myServo;
int servoPos = 90; // Initial position (0-180)
unsigned long lastMoveTime = 0; // For smooth movement control

void setup() {
  Serial.begin(9600);
  myServo.attach(9); // Connect servo to pin 9
  myServo.write(servoPos); // Initial position
  delay(1000); // Give servo time to reach initial position
}

void loop() {
  // Check if commands are available
  if (Serial.available() > 0) {
    char command = Serial.read();

    // Process commands
    switch(command) {
      case 'L': // Left
        servoPos = max(0, servoPos - 10);
        break;
      case 'R': // Right
        servoPos = min(180, servoPos + 10);
        break;
      case 'C': // Center
        servoPos = 90;
        break;
    }

    // Record the time of the last movement
    lastMoveTime = millis();
  }

  // Move the servo smoothly to avoid power issues
  static int currentPos = servoPos;

  // If target position is different from current, move gradually
  if (currentPos != servoPos) {
    // Determine movement direction
    if (currentPos < servoPos) {
      currentPos++;
    } else {
      currentPos--;
    }

    // Move servo to new position
    myServo.write(currentPos);

    // Small pause for smooth movement
    delay(15);
  }
}
