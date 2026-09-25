/*
EJEMPLO DE ULTRASONICO V2
OJO: esta es con una librería así que asegurense de descargarla e incluirla para que funcione

*/
#include <NewPing.h>

// Definición de las constantes para los pines del sensor
int trigPin = 9;  // Pin trig del sensor
int echoPin = 10;  // Pin echo del sensor
int maxDistance = 200;  // Distancia máxima en cm

NewPing sonar(trigPin, echoPin, maxDistance);  //esta es la función dentro de la librería, si quieren el porque, abran la librería

void setup() {
  Serial.begin(9600);// Inicialización de la comunicación serial
}

void loop() {
  // Lectura de la distancia con NewPing
  int distance = sonar.ping_cm();
  
  // Mostrar la distancia en el monitor serial
  Serial.print("Distancia: "); //acuerdense de dejar 1 espacio para que se vea mas estético y no así "Distancia:15cm"
  Serial.print(distance);
  Serial.println(" cm");//igual, para que se termine viendo así "Distancia: 15 cm"
  
  // Espera de 1 segundo antes de la próxima medición
  delay(100); //aquí le cambié ya que 1seg es demasiado tiempo, de preferencia 100 como estandar, y si no les afecta, pues 10
}
