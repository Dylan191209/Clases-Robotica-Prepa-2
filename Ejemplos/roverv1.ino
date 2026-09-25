//Este prototipo de rover...
//#include <SoftwareSerial.h>
#include <Servo.h> 

//SoftwareSerial espSerial(0, 1); // RX, TX
//Servo servo; 
Servo ejex; // el eje x se usará para la garra
int x = 5;
Servo ejey; // el eje y se usará para la garra
int y = 4;
int taladro = 3;

//int pin = 2; //definimos que responde al pin
//int potenciometro = A5; // es analógico, comentar el potenciómetro

//declaramos pines del L298N
int ENA = 9; // tiene que ser PWM
int IN1 = 8;
int IN2 = 10;
int IN3 = 12;
int IN4 = 13;
int ENB = 11;// tiene que ser PWM

//declaramos los pines del ultrasónico 
int trig = 6; // azul
int echo = 7; // cable morado
long duration;
long distance;

unsigned long ultimaLectura = 0;
const int intervaloLectura = 10; // 10 milisegundos


void setup() {
  Serial.begin(9600);
  //espSerial.begin(9600); 
  //servo.attach(pin); 
  
  // l298n
  pinMode (ENA, OUTPUT);
  pinMode (IN1, OUTPUT);
  pinMode (IN2, OUTPUT);
  pinMode (IN3, OUTPUT);
  pinMode (IN4, OUTPUT);
  pinMode (ENB, OUTPUT);

  //ultrasónico 
  pinMode (trig,OUTPUT);
  pinMode (echo, INPUT);

  //servo
  ejex.attach(x);
  ejey.attach(y);
  pinMode (taladro, OUTPUT);
  
}

void loop() { 
  //int val = analogRead(potenciometro); 
  //int grados = map(val, 0, 1023, 0, 180); 
  //servo.write(grados);
  //servo.write(90); // Posición fija para el servo
  //Serial.println("Servo en posición fija");

  unsigned long tiempoActual = millis();
  if (tiempoActual - ultimaLectura >= intervaloLectura) {
    lecturas();
    ultimaLectura = tiempoActual;
  }
  avanzar();
  if (distance <=  10){
    detener();
    activado();
    delay(1500);
    guardado();
    delay(500);
    esquivar();
  }
  else{
    avanzar();
  }
}

void reinicio(){
  digitalWrite(trig, LOW);
  delayMicroseconds(2);
  digitalWrite(trig, HIGH); 
  delayMicroseconds(10);
  digitalWrite(trig, LOW);
}

void lecturas (){
  reinicio();
  duration = pulseIn(echo, HIGH);
  distance = duration * 0.034 / 2;
  Serial.print("Distancia: ");
  Serial.print(distance);
  Serial.println(" cm");
  delay(10);
}

void avanzar(){
  digitalWrite(IN1, HIGH);
  analogWrite(ENA, 255);
  digitalWrite(IN2, LOW);

  digitalWrite(IN3, LOW);
  analogWrite(ENB, 255);
  digitalWrite(IN4, HIGH);
}

void detener(){
  digitalWrite(IN1, LOW);
  analogWrite(ENA, 0);
  digitalWrite(IN2, LOW);

  digitalWrite(IN3, LOW);
  analogWrite(ENB, 0);
  digitalWrite(IN4, LOW);
}

void derecha(){
  digitalWrite(IN1, HIGH);
  analogWrite(ENA, 255);
  digitalWrite(IN2, LOW);

  digitalWrite(IN3, HIGH);
  analogWrite(ENB, 255);
  digitalWrite(IN4, LOW);
}

void izquierda(){
  digitalWrite(IN1, LOW);
  analogWrite(ENA, 255);
  digitalWrite(IN2, HIGH);

  digitalWrite(IN3, LOW);
  analogWrite(ENB, 255);
  digitalWrite(IN4, HIGH);
}

void activado (){
  ejey.write(180);
  delay(500);
  ejex.write(90);
  delay(500);  
  digitalWrite(taladro, HIGH);
  //servo.write(45); // La cámara gira a 45 grados
  //delay(1000);
}

void guardado (){
  digitalWrite(taladro, LOW);
  ejey.write(0);
  delay(500);
  ejex.write(150);
  //servo.write(90); // La cámara regresa a su posición origina
  delay(500);
  
}

void esquivar(){
  derecha();
  delay(500); 
  detener();
  
  avanzar();
  delay(500); 
  detener();
  
  izquierda();
  delay(500); 
  detener();
}

/*
void tomarFoto() {
  enviarComandoFoto();
  mostrarMensajeFotoTomada();
  
  Serial.println("foto");
}

void enviarComandoFoto() {
  espSerial.println("foto");
}

void mostrarMensajeInicio() {
  Serial.println("Sistema iniciado. Esperando...");
}

void mostrarMensajeFotoTomada() {
  Serial.println("Foto tomada");
}
*/