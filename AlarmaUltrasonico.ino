/*
EJEMPLO DE ALARMA INTELIGENTE
Este es para que aprendan a definir los rangos con los ultrasonicos y a cambiarlo (usando funciones) para que haga algo similar a lo de una alarma de estacionamiento para carro
*/

//declaramos los leds
int r = 3;
int a = 4;
int v = 5;

//declaramos el ultrasónico 
int trigg = 7;
int echo = 6;

//declaramos el buzzer
int bz = 8;

//declaramos duración y distancia para las ondas (son variables)
float duration;
float distance;

//le pongo const para que por ninguna razón se nos cambie el valor de esas variables
const float lejos = 50.0;
const float medio = 30.0;
const float cerca = 10.0;

void setup()
{
  //leds son de salida
  pinMode(r, OUTPUT);
  pinMode(a, OUTPUT);
  pinMode(v, OUTPUT);
  
  //buzzer también
  pinMode(bz, OUTPUT);
  
  pinMode(trigg, OUTPUT);// trigg, dispara, OUT
  pinMode(echo, INPUT);// echo, recibe, IN
  
  Serial.begin(9600); // iniciamos monitor serial
}

void loop(){
  reinicio();
  
  // mide cuanto tardó la onda sonora
  duration = pulseIn(echo, HIGH);

  /* Calcular la distancia en cm
    velocidad de la velocidad del sonido= duración
    (de ida y vuelta) *0.034, todo se divide entre 2 para 
    saber cuánto tardó de ida
  
  */
  distance = (duration * 0.034 )/ 2;

  // escribe distancia en el monitor serial
  Serial.print("Distancia: ");
  Serial.print(distance);
  Serial.println(" cm");
  
   if (distance >= lejos) {
    Serial.println("Lejos");
    digitalWrite(r, HIGH); // Enciende led rojo
    digitalWrite(a, LOW);
    digitalWrite(v, LOW);
    alarma1();

  }  if (distance >= medio ) {
    Serial.println("Medio");
    digitalWrite(r, LOW);
    digitalWrite(a, HIGH); // Enciende led amarillo
    digitalWrite(v, LOW);
    alarma2();
    
  } if (distance <= cerca) {
    Serial.println("Cerca");
    digitalWrite(r, LOW);
    digitalWrite(a, LOW);
    digitalWrite(v, HIGH); // Enciende led verde
    alarma3();
  }
  delay(100); //las lecturas se hacen cada 100 milisegundos
  
}

void reinicio(){
  digitalWrite(trigg, LOW);//corta la onda sonora
  delayMicroseconds(2);
  digitalWrite(trigg, HIGH); //la vuelve a lanzar
  delayMicroseconds(10);
  digitalWrite(trigg, LOW);//la vuelve a cortar
}
void alarma1 (){ //bz para cuando esté lejos
  digitalWrite(bz, HIGH);
  delay (1000);
  digitalWrite(bz, LOW);
  delay (1000);
}
void alarma2 (){ //bz para cuando este medio
  digitalWrite(bz, HIGH);
  delay (500);
  digitalWrite(bz, LOW);
  delay (500);
}
void alarma3 (){ //bz para cuando este cerca
  digitalWrite(bz, HIGH);
  delay (100);
  digitalWrite(bz, LOW);
  delay (100);
}