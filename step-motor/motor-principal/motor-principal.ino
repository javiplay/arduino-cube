#include <AccelStepper.h>

AccelStepper stepper(AccelStepper::HALF4WIRE, 8, 10, 9, 11);

void setup() {
  stepper.setMaxSpeed(1000);
  stepper.setAcceleration(200);
  Serial.begin(9600);
  
  Serial.println("Comandos:");
  Serial.println("'f' = adelante 1 vuelta");
  Serial.println("'b' = atrás 1 vuelta");
  Serial.println("'s' = detener");
}

void loop() {
  // SIEMPRE llamar a run()
  stepper.run();
  
  // Leer comandos seriales
  if (Serial.available() > 0) {
    char cmd = Serial.read();
    
    if (cmd == 'f') {
      // Mover relativo: 1 vuelta adelante
      stepper.move(2048);
      Serial.println("Girando adelante...");
    }
    else if (cmd == 'b') {
      // Mover relativo: 1 vuelta atrás
      stepper.move(-2048);
      Serial.println("Girando atrás...");
    }
    else if (cmd == 's') {
      // Detener con rampa
      stepper.stop(); // Desacelera hasta 0
      Serial.println("Deteniendo...");
    }
  }
  
  // Mostrar posición actual cada 500ms
  static unsigned long lastPrint = 0;
  if (millis() - lastPrint > 500) {
    Serial.print("Posición: ");
    Serial.print(stepper.currentPosition());
    Serial.print(" | Velocidad: ");
    Serial.println(stepper.speed());
    lastPrint = millis();
  }
}