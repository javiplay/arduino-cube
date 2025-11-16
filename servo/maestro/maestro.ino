// Arduino MAESTRO con pantalla táctil MCUFRIEND

#include <MCUFRIEND_kbv.h>
#include <TouchScreen.h>
#include <Adafruit_GFX.h>

MCUFRIEND_kbv tft;

// CONFIGURACIÓN CORRECTA DE PINES PARA TU PANTALLA
#define YP A1  // Cambiado de A3 a A1
#define XM A2
#define YM 7   // Cambiado de 9 a 7
#define XP 6   // Cambiado de 8 a 6

// Calibración de la pantalla táctil - estos valores los ajustaremos después
#define TS_MINX 100
#define TS_MAXX 920
#define TS_MINY 70
#define TS_MAXY 900

// Constantes para la pantalla
#define BLACK   0x0000
#define BLUE    0x001F
#define RED     0xF800
#define GREEN   0x07E0
#define CYAN    0x07FF
#define MAGENTA 0xF81F
#define YELLOW  0xFFE0
#define WHITE   0xFFFF

// Umbral de presión ajustado según tus valores (130-166)
TouchScreen ts = TouchScreen(XP, YP, XM, YM, 300);

// Variables para controlar el servo
int servoPos = 90; // Posición inicial del servo (0-180)

void setup() {
  Serial.begin(9600);
  
  // Inicializar la pantalla
  uint16_t ID = tft.readID();
  tft.begin(ID);
  tft.setRotation(1); // Ajustar orientación si es necesario
  
  // Dibujar la interfaz
  tft.fillScreen(BLACK);
  drawInterface();
}

void loop() {
  // Obtener input táctil
  TSPoint p = ts.getPoint();
  
  // Necesario restaurar los pines como salidas después de ts.getPoint()
  pinMode(YP, OUTPUT);
  pinMode(XM, OUTPUT);
  
  if (p.z > ts.pressureThreshhold) {
    // Convertir coordenadas táctiles a coordenadas de la pantalla
    p.x = map(p.x, TS_MINX, TS_MAXX, 0, tft.width());
    p.y = map(p.y, TS_MINY, TS_MAXY, 0, tft.height());
    
    // Procesar el toque
    processTouch(p.x, p.y);
    
    // Anti-rebote
    delay(200);
  }
  
  delay(50); // Pequeña pausa para estabilidad
}

void drawInterface() {
  // Título
  tft.setTextColor(WHITE);
  tft.setTextSize(3);
  tft.setCursor(20, 10);
  tft.print("Control Servo");
  
  // Botón izquierdo (-)
  tft.fillRect(20, 80, 80, 80, RED);
  tft.setTextColor(WHITE);
  tft.setTextSize(4);
  tft.setCursor(45, 105);
  tft.print("-");
  
  // Botón central (90°)
  tft.fillRect(120, 80, 80, 80, BLUE);
  tft.setTextSize(3);
  tft.setCursor(135, 105);
  tft.print("90");
  
  // Botón derecho (+)
  tft.fillRect(220, 80, 80, 80, GREEN);
  tft.setTextSize(4);
  tft.setCursor(245, 105);
  tft.print("+");
  
  // Etiquetas de los botones
  tft.setTextSize(1);
  tft.setCursor(30, 170);
  tft.print("-10 grados");
  tft.setCursor(135, 170);
  tft.print("Centro");
  tft.setCursor(230, 170);
  tft.print("+10 grados");
  
  // Mostrar posición actual
  updatePositionDisplay();
}

void updatePositionDisplay() {
  // Limpiar área de posición
  tft.fillRect(80, 200, 160, 35, BLACK);
  
  // Mostrar posición actual
  tft.setTextColor(YELLOW);
  tft.setTextSize(3);
  tft.setCursor(90, 205);
  tft.print("Pos: ");
  tft.print(servoPos);
  tft.print((char)167); // Símbolo de grado °
}

void processTouch(int x, int y) {
  bool changed = false;
  
  // Comprobar si se tocó el botón izquierdo (-)
  if (x > 20 && x < 100 && y > 80 && y < 160) {
    servoPos = max(0, servoPos - 10);
    Serial.write('L'); // Enviar comando para mover a la izquierda
    changed = true;
  }
  
  // Comprobar si se tocó el botón derecho (+)
  else if (x > 220 && x < 300 && y > 80 && y < 160) {
    servoPos = min(180, servoPos + 10);
    Serial.write('R'); // Enviar comando para mover a la derecha
    changed = true;
  }
  
  // Comprobar si se tocó el botón central (90°)
  else if (x > 120 && x < 200 && y > 80 && y < 160) {
    servoPos = 90;
    Serial.write('C'); // Enviar comando para centrar
    changed = true;
  }
  
  // Actualizar la pantalla solo si hubo un cambio
  if (changed) {
    updatePositionDisplay();
    
    // Feedback visual - parpadear el área tocada
    delay(100);
  }
}