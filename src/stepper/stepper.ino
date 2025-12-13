#include <AccelStepper.h>

AccelStepper stepper(AccelStepper::HALF4WIRE, 8, 10, 9, 11);

void setup() {
  stepper.setMaxSpeed(1000);
  stepper.setAcceleration(200);
  Serial.begin(9600);

  Serial.println("Commands:");
  Serial.println("'f' = forward 1 revolution");
  Serial.println("'b' = backward 1 revolution");
  Serial.println("'s' = stop");
}

void loop() {
  // ALWAYS call run()
  stepper.run();

  // Read serial commands
  if (Serial.available() > 0) {
    char cmd = Serial.read();

    if (cmd == 'f') {
      // Move relative: 1 revolution forward
      stepper.move(2048);
      Serial.println("Rotating forward...");
    }
    else if (cmd == 'b') {
      // Move relative: 1 revolution backward
      stepper.move(-2048);
      Serial.println("Rotating backward...");
    }
    else if (cmd == 's') {
      // Stop with ramp
      stepper.stop(); // Decelerates to 0
      Serial.println("Stopping...");
    }
  }

  // Show current position every 500ms
  static unsigned long lastPrint = 0;
  if (millis() - lastPrint > 500) {
    Serial.print("Position: ");
    Serial.print(stepper.currentPosition());
    Serial.print(" | Speed: ");
    Serial.println(stepper.speed());
    lastPrint = millis();
  }
}
