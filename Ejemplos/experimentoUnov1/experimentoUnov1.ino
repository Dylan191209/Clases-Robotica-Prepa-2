#include "DHT.h"      // Librería para el sensor de temperatura y humedad 
#include <Servo.h>    // Librería para el servo

//PINES DE CONEXIÓN 
#define SERVO_PIN 11      
#define JOYSTICK_X_PIN A0 // El eje X del joystick va a este pin analógico (para controlar el servo)
#define DHTPIN 2          
#define LDR_PIN A2        
#define TRIGG_PIN 9       
#define ECHO_PIN 10       

//CONFIGURACIÓN DE SENSORES 
#define DHTTYPE DHT11 // es mi modelo de dht, ya que hay otro que no es "11", sino que 22
DHT dht(DHTPIN, DHTTYPE); // Creamos un "objeto" para poder usar el sensor DHT con sus funciones.

//VARIABLES GLOBALES 
Servo servoMotor; // le ponemos nombre al servo
const long ULTRASONIC_TIMEOUT = 25000; // Es el tiempo máximo que esperamos la respuesta del sensor ultrasónico, para que no se quede "pensando" por siempre.
long duration;    // para el cálculo del ultra
long distanceCm = 0; //este también
float humedad = 0.0;    // ocupamos que sea float para ponerle decimal
float temperaturaC = 0.0; // ocupamos que sea float para grados con decimal
int LDRVal = 0;   // Aquí guardamos el valor de la luz, que va de 0 a 1023
int servoAngle = 0; // Aquí guardamos el ángulo en el que está el servo

unsigned long ultimaLectura = 0; // Guardamos el momento en el que enviamos los últimos datos
const int intervaloLectura = 100; // Cada cuánto tiempo queremos enviar los datos. En este caso, cada 100 ms.

void setup() {
  Serial.begin(115200); // ponemos el monitor serial a 115200, que es una velocidad bastante rápida
  dht.begin();          // Encendemos el sensor de temperatura y humedad para que empiece a funcionar
  servoMotor.attach(SERVO_PIN); 
  pinMode(TRIGG_PIN, OUTPUT); //trigg = disparador = OUT
  pinMode(ECHO_PIN, INPUT);   //echo = recibe = IN
}

void loop() { 
  int joystickXval = analogRead(JOYSTICK_X_PIN); // Leemos el valor del joystick, que es un número entre 0 y 1023.
  servoAngle = map(joystickXval, 0, 1023, 0, 180);//mapeamos el joystick con el servo
  servoMotor.write(servoAngle); // Mueve el servo al ángulo que acabamos de calcular.

  unsigned long tiempoActual = millis(); // Preguntamos cuánto tiempo ha pasado desde que el Arduino se encendió.

  // Si ya pasaron 100 milisegundos desde la última vez...
  if (tiempoActual - ultimaLectura >= intervaloLectura) {
    ultimaLectura = tiempoActual; // ...entonces actualizamos el tiempo.

    digitalWrite(TRIGG_PIN, LOW); // Limpiamos el pin para que no haya problemas por interferencia al momento de encenderlo
    delayMicroseconds(2);
    digitalWrite(TRIGG_PIN, HIGH); // Emitimos una sonda
    delayMicroseconds(10);
    digitalWrite(TRIGG_PIN, LOW);

    duration = pulseIn(ECHO_PIN, HIGH, ULTRASONIC_TIMEOUT); // Medimos cuánto tarda en regresar el sonido

    if (duration > 0) {
      distanceCm = (duration * 0.034) / 2; // Hacemos la magia de la física para convertir el tiempo en distancia
    } else {
      distanceCm = 0; // Si no regresa el sonido, decimos que la distancia es 0 para evitar errores
    }

   
    humedad = dht.readHumidity();     // Guardamos la humedad
    temperaturaC = dht.readTemperature(); // Guardamos la temperatura

    
    LDRVal = analogRead(LDR_PIN); // Leemos el valor de la luz (un número)

    //mostramos todos los datos en el monitor srial
    Serial.print(distanceCm);
    Serial.print(",");
    Serial.print(temperaturaC);
    Serial.print(",");
    Serial.print(humedad);
    Serial.print(",");
    Serial.println(LDRVal); // El "println" es para que se mande todo lo anterior, y cuando llege a ese, pase un espacio, como el enter
  }
}
