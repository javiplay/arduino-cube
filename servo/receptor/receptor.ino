// Arduino ESCLAVO con servo

#include <Servo.h>

Servo myServo;
int servoPos = 90; // Posición inicial (0-180)
unsigned long lastMoveTime = 0; // Para controlar movimientos suaves

void setup() {
  Serial.begin(9600);
  myServo.attach(9); // Conectar el servo al pin 9
  myServo.write(servoPos); // Posición inicial
  delay(1000); // Dar tiempo al servo para alcanzar la posición inicial
}

void loop() {
  // Revisar si hay comandos disponibles
  if (Serial.available() > 0) {
    char command = Serial.read();
    
    // Procesar comandos
    switch(command) {
      case 'L': // Izquierda
        servoPos = max(0, servoPos - 10);
        break;
      case 'R': // Derecha
        servoPos = min(180, servoPos + 10);
        break;
      case 'C': // Centro
        servoPos = 90;
        break;
    }
    
    // Registrar el tiempo del último movimiento
    lastMoveTime = millis();
  }
  
  // Mover el servo de forma suave para evitar problemas de alimentación
  static int currentPos = servoPos;
  
  // Si la posición objetivo es diferente de la actual, mover gradualmente
  if (currentPos != servoPos) {
    // Determinar dirección del movimiento
    if (currentPos < servoPos) {
      currentPos++;
    } else {
      currentPos--;
    }
    
    // Mover el servo a la nueva posición
    myServo.write(currentPos);
    
    // Pequeña pausa para movimiento suave
    delay(15);
  }
}