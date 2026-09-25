#include <Wire.h> 
#include <LiquidCrystal_I2C.h>

// Configuración de los pines del controlador L298N
int ENA = 9;
int IN1 = 8;
int IN2 = 10;
int IN3 = 12;
int IN4 = 13;
int ENB = 11;

// Configuración de los pines para los sensores ultrasónicos
int trigg1 = 4;
int echo1 = 5;
long duration1;
long distance1;

int trigg2 = 6;
int echo2 = 7;
long duration2;
long distance2;

int trigg3 = 3;
int echo3 = 2;
long duration3;
long distance3;

// Parámetros de navegación del robot
int distanciaF = 12;
int Dpasillo = 10;
int ladoL = 7;

// Configuración del LCD I2C
LiquidCrystal_I2C lcd(0x27, 16, 2); 

// Variable para almacenar el estado actual del robot
char estadoActual = ' ';

void setup() {
  Serial.begin(19200);

  // Inicialización del LCD
  Wire.begin();
  lcd.begin(16, 2); 
  lcd.backlight(); 

  // Configuración de pines para el L298N
  pinMode(ENA, OUTPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  pinMode(ENB, OUTPUT);

  // Configuración de pines para los sensores ultrasónicos
  pinMode(trigg1, OUTPUT);
  pinMode(echo1, INPUT);
  pinMode(trigg2, OUTPUT);
  pinMode(echo2, INPUT);
  pinMode(trigg3, OUTPUT);
  pinMode(echo3, INPUT);

  lcd.setCursor(0, 0);
  lcd.print("Robot Iniciado");
  lcd.setCursor(0, 1);
  lcd.print("Buscando...");
  delay(1000);
  lcd.clear();
}

void loop() {
  lecturas();

  if (distance1 < distanciaF) {
    // Si hay un obstáculo al frente, se detiene y evalúa
    detener();
    actualizarLCD('P'); 
    delay(200);

    bool derechaTransitable = (distance2 > Dpasillo);
    bool izquierdaTransitable = (distance3 > Dpasillo);

    if (derechaTransitable && !izquierdaTransitable) {
      derecha();
      actualizarLCD('D'); 
      delay(550);
      detener();
      actualizarLCD('P');
      delay(150);
    } else if (izquierdaTransitable && !derechaTransitable) {
      izquierda();
      actualizarLCD('I');
      delay(550);
      detener();
      actualizarLCD('P');
      delay(150);
    } else if (derechaTransitable && izquierdaTransitable) {
      if (distance2 >= distance3) {
        derecha();
        actualizarLCD('d');
      } else {
        izquierda();
        actualizarLCD('i');
      }
      delay(550);
      detener();
      actualizarLCD('P');
      delay(150);
    } else {
      // **¡BLOQUE DE ESCAPE!** Esto se activa cuando todos los sensores 
      // detectan un obstáculo muy cerca (distancia < Dpasillo).
      // El robot está en un callejón sin salida y necesita girar.
      actualizarLCD('S'); // S para "sin salida"
      delay(1000); // Muestra el mensaje por 1 segundo

      atras();
      actualizarLCD('R');
      delay(150);
      detener();
      actualizarLCD('P');
      delay(150);

      // Ahora intenta el giro de escape (180 grados)
      derecha();
      actualizarLCD('U');
      delay(900);
      detener();
      actualizarLCD('P');
      delay(150);
    }
  } else {
    avanzar();
    actualizarLCD('A');
  }
  delay(50);
}

// Función para actualizar el LCD
void actualizarLCD(char nuevoEstado) {
  if (nuevoEstado != estadoActual) {
    lcd.clear();
    lcd.setCursor(0, 0);

    switch (nuevoEstado) {
      case 'A':
        lcd.print("Polaris v6");
        lcd.setCursor(0, 1);
        lcd.print("Prepa 2");
        break;
      case 'R':
        lcd.print("Retrocediendo");
        break;
      case 'D':
        lcd.print("Giro Derecha");
        break;
      case 'I':
        lcd.print("Giro Izquierda");
        break;
      case 'P':
        lcd.print("Detenido");
        break;
      case 'U':
        lcd.print("Giro de 180");
        break;
      case 'd':
        lcd.print("Avanzando...");
        lcd.setCursor(0, 1);
        lcd.print("Giro Derecha");
        break;
      case 'i':
        lcd.print("Avanzando...");
        lcd.setCursor(0, 1);
        lcd.print("Giro Izquierda");
        break;
      case 'S':
        lcd.print("Callejon sin");
        lcd.setCursor(0, 1);
        lcd.print("salida!");
        break;
      default:
        lcd.print("Comando Desconocido");
        break;
    }
    estadoActual = nuevoEstado;
  }
}


// Funciones de movimiento
void avanzar() {
  digitalWrite(IN1, HIGH);
  analogWrite(ENA, 150);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, HIGH);
  analogWrite(ENB, 150);
  digitalWrite(IN4, LOW);
}

void detener() {
  digitalWrite(IN1, LOW);
  analogWrite(ENA, 0);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  analogWrite(ENB, 0);
  digitalWrite(IN4, LOW);
}

void derecha() {
  digitalWrite(IN1, HIGH);
  analogWrite(ENA, 150);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  analogWrite(ENB, 150);
  digitalWrite(IN4, HIGH);
}

void izquierda() {
  digitalWrite(IN1, LOW);
  analogWrite(ENA, 150);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, HIGH);
  analogWrite(ENB, 150);
  digitalWrite(IN4, LOW);
}

void atras() {
  digitalWrite(IN1, LOW);
  analogWrite(ENA, 150);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, LOW);
  analogWrite(ENB, 150);
  digitalWrite(IN4, HIGH);
}

// Funciones de lectura de sensores ultrasónicos
void lectura1() {
  digitalWrite(trigg1, LOW);
  delayMicroseconds(2);
  digitalWrite(trigg1, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigg1, LOW);
  duration1 = pulseIn(echo1, HIGH);
  distance1 = duration1 * 0.034 / 2;
  if (distance1 == 0 || distance1 > 400) distance1 = 999;
}

void lectura2() {
  digitalWrite(trigg2, LOW);
  delayMicroseconds(2);
  digitalWrite(trigg2, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigg2, LOW);
  duration2 = pulseIn(echo2, HIGH);
  distance2 = duration2 * 0.034 / 2;
  if (distance2 == 0 || distance2 > 400) distance2 = 999;
}

void lectura3() {
  digitalWrite(trigg3, LOW);
  delayMicroseconds(2);
  digitalWrite(trigg3, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigg3, LOW);
  duration3 = pulseIn(echo3, HIGH);
  distance3 = duration3 * 0.034 / 2;
  if (distance3 == 0 || distance3 > 400) distance3 = 999;
}

void lecturas() {
  lectura1();
  lectura2();
  lectura3();
  Serial.print("Distancias: F=");
  Serial.print(distance1);
  Serial.print("cm, D=");
  Serial.print(distance2);
  Serial.print("cm, I=");
  Serial.print(distance3);
  Serial.println("cm");
}