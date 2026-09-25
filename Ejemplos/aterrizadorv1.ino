//añadir la librerpia del servo
#include <Servo.h>
#include <LiquidCrystal_I2C.h> 
//ponerle nombre a cada uno
Servo servo1;
Servo servo2;


#include "DHT.h"
#define DHTPIN 10     //DATA del DHT11
#define DHTTYPE DHT11 // modelo dht11
DHT dht(DHTPIN, DHTTYPE);


int pinAudio = 7; // Pin conectado a PLAYE del ISD1820

bool audioJugado = false; // Para que el pal panadero solo pase una vez

//servos (pines)
int s1pin= 2;
int s2pin= 3; //rojo

// La dirección es 0x27   amarillo A5
LiquidCrystal_I2C lcd(0x27, 16, 2);


unsigned long ultimaLectura = 0;
const int intervaloLectura = 10; // 10 milisegundos

// Deben ser pines PWM para permitir la mezcla de colores (analogWrite)
int r = A0; // Pin para el Rojo
int g = A1; // Pin para el Verde
int b = A2; // Pin para el Azul

//ultra
int trigg = 8;
int echo = 9 ; //amarillo
/*se usa long ya q es una capacidad más grande que un int normal, pero tmb c puede usar un float, y como son variables no se declaran
con ningun =  */
long distancia;
long tiempo;

void setup() {
//es la función principal de los servos, es como:"vas a hacer lo que (pin) te diga"
servo1.attach(s1pin);
servo2.attach(s2pin);


//pal panadero con el pan
pinMode(pinAudio, OUTPUT);
digitalWrite(pinAudio, LOW); // Aseguramos que empiece apagado

// LED RGB como OUTPUT
  pinMode(r, OUTPUT);
  pinMode(g, OUTPUT);
  pinMode(b, OUTPUT);

/*lo mismo, pero con pinMode y para el ultra, trigg es el que manda la onda, por tanto OUTPUT, mientras que echo es la q la recibe,
por tanto INPUT*/
pinMode(trigg, OUTPUT);
pinMode(echo, INPUT);


dht.begin();

/*iniciamos el monitor serial (lo puse en 115200 pq quiero que recuerden que no solo se pone el 9600, sino que se pueden usar las demás,
entre más alto el número, mayor la velocidad de la comunicación serial)
*/
Serial.begin(9600);
  lcd.init();
  lcd.backlight(); // Enciende la retroiluminación
  lcd.clear();     // Limpia la pantalla
// Muestra un mensaje de inicio una sola vez.
 lcd.setCursor(0, 0);
 lcd.print("Iniciando");
  lcd.setCursor(0, 1);
 lcd.print("Aterrizador");
 for (int i =0; i <=6; i++){
  lcd.print(".");
  delay(100);
 }
 lcd.clear();

}

void loop() {
/*
  unsigned long tiempoActual = millis();
  if (tiempoActual - ultimaLectura >= intervaloLectura) {
    lectura();
    ultimaLectura = tiempoActual;
  }
  // put your main code here, to run repeatedly:
lectura();
//esta parte es lógica de programación, "si la distancia es igual o menor a 15, entonces..."
if (distancia <= 30) {
    medio(); // Servos a 90 grados 
    rojo();  // Advertencia crítica
  }
   else if (distancia <= 50) {
    casiMedio(); // Servos a 45 grados
    amarillo();  // Advertencia de Aproximación (LED Amarillo)
  } 
  else {// de lo contrario...
    nada(); // Servos a 0 grados (Posición inicial)
    verde(); // Distancia segura (LED Verde)
  }

//esto lo comenté ya q era solo para probar los servos y que funcionaran bien
/*/
/**
delay(1000);
casiMedio();
delay(1000);
*/
//medio();
/*
delay(1000);
todo();
delay(1000);

mostrarDistanciaLCD(distancia);
delay(100);

*/
  unsigned long tiempoActual = millis();
  if (tiempoActual - ultimaLectura >= intervaloLectura) {
    lectura();
    ultimaLectura = tiempoActual;
  }

  lectura();

  // Lógica de aterrizaje con candado para el audio
  if (distancia <= 15) { // Cambié a 15 para estar seguros de que ya tocó suelo
    medio(); 
    rojo();
    
    // Si la distancia es baja y NO ha sonado el audio, lo activamos
    if (audioJugado == false) {
      audio();
      audioJugado = true; // Cerramos el candado
    }
  }
  else if (distancia <= 30) {
     medio();
     rojo();
  }
  else if (distancia <= 50) {
    casiMedio(); 
    amarillo();
    // Si vuelve a subir, podrías resetearlo si quieres que vuelva a sonar al bajar
    // audioJugado = false; 
  } 
  else {
    nada(); 
    verde();
    // Opcional: si quieres que el audio se "recargue" al estar lejos del suelo
    audioJugado = false; 
  }

  mostrarDistanciaLCD(distancia);
  delay(100);

}
//a partir de aquí son puras funciones para facilitar el código, igual, se forman con "void" + (nombre) + "()" + "{}"
void nada(){
  servo1.write(0);
  servo2.write(0);
  /*
  servo3.write(0);
  servo4.write(0);
  servo5.write(0);
  servo6.write(0);
  */
}

void casiMedio(){
  servo1.write(45);
  servo2.write(45);
  /*
  servo3.write(45);
  servo4.write(45);
  servo5.write(45);
  servo6.write(45);
  */
}
void medio(){
  servo1.write(180);
  servo2.write(180);
  /*
  servo3.write(90);
  servo4.write(90);
  servo5.write(90);
  servo6.write(90);
  */
}
void todo(){
  servo1.write(180);
  servo2.write(180);
/*
  servo3.write(180);
  servo4.write(180);
  servo5.write(180);
  servo6.write(180);
  */
}

void lectura(){
  /*lecturas para el ultrasónico*/
  digitalWrite(trigg, LOW);//cortamos la onda de sonido durante 2 microsegundos para evitar interferencias de sonido
  delayMicroseconds(2);
  digitalWrite(trigg, HIGH);//la lanzamos durante 10 microsegundos
  delayMicroseconds(10);
  digitalWrite(trigg, LOW);//la volvemos a cortar, para formar el "disparo"

  tiempo = pulseIn(echo, HIGH); //el dato que nos dé el echo (el tiempo q tardó en ir y regresar la onda), es el nuevo valor de la variable "tiempo"
  distancia = (tiempo*0.034)/2;//usamos los cálculos y fórmulas físicas y matemáticas para calcular la distancia con el tiempo y la velocidad del sonido
  /*esto es más q nada estética, simplemente podemos poner la 2da línea y ya queda*/
  Serial.print("distancia: ");
  Serial.print(distancia);
  Serial.println("cm");/*el "ln" q c le agrega es para hacer un salto de línea, que salga así:

  distancia: 19cm
  distancia: 10cm
  distancia: 5cm

  y no así:
  distancia 19cmdistancia: 10cmdistancia:5cm 
  */
  
}

void mostrarDistanciaLCD(long d) {
  float t = dht.readTemperature(); // Lectura para mostrar
  lcd.setCursor(0, 0); 
  lcd.print(d); 
  lcd.print("cm ");
  lcd.setCursor(10, 0);
  lcd.print("T:"); lcd.print((int)t); lcd.print("C"); // El toque de la Mega
  
  lcd.setCursor(0, 1);
  lcd.print("                "); 
  lcd.setCursor(0, 1); 

  // Añade un mensaje de estado
  if (d <= 15) {
      // Mensaje ajustado a la longitud de 16 caracteres de la LCD
      lcd.setCursor(6, 0);
      lcd.print(" Me ");
      lcd.setCursor(0, 1);
      lcd.print("caigoooooooooooo"); 
  } else if (d <= 50) {
      lcd.setCursor(6, 0);
      lcd.print(" ya mero      ");
      lcd.setCursor(0, 1);
      lcd.print("llego :P      ");
  } else {
      lcd.setCursor(6, 0);
      lcd.print(" que      ");
      lcd.setCursor(0, 1);
      lcd.print("agusticidad va? ");
  }
}

void RGB_Color(int valr, int valg, int valb) {
  // analogWrite usa valores de 0 (apagado) a 255 (brillo máximo)
  analogWrite(r, valr);
  analogWrite(g, valg);
  analogWrite(b, valb);
}

// Funciones para los colores de estado

void rojo() {
  // ROJO (Advertencia Crítica)
  RGB_Color(255, 0, 0); 
}

void verde() {
  // VERDE (OK)
  RGB_Color(0, 255, 0); 
}

void azul() {
  // AZUL (Modo de nada)
  RGB_Color(0, 0, 255); 
}

void amarillo() {
  // AMARILLO (Advertencia de Aproximación: Rojo y Verde)
  RGB_Color(255, 255, 0);
}

void audio(){
   // ACTIVAR AUDIO
  digitalWrite(pinAudio, HIGH); // Dispara el audio
  delay(100);                   // Pulso breve
  digitalWrite(pinAudio, LOW);
}