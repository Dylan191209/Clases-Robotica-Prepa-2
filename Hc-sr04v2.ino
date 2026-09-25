/*
ULTRASONICO 
Esta si es la buena, es el camino que "nunca falla" pq usamos una constante (OJO, a veces puede llegar a cambiar ya que la velocidad del sonido puede llegar a 
variar dependiendo el contexto), de igual forma, no es la única y ya existen librerías que nos ayudan, pero algunas veces nos piden 0 librerías así que prefiero
enseñarles a usar este método.
*/

int trigPin = 3; // Pin trig del sensor
int echoPin = 5; // Pin echo del sensor
long duration; //puede ser long o float; ya que a fin de cuentas es un número y puede tener decimales, a como se acomoden.
long distance; //puede ser long o float; ya que a fin de cuentas es un número y puede tener decimales, a como se acomoden.

void setup() {
  pinMode(trigPin, OUTPUT);//trigg, dispara, por tanto OUT
  pinMode(echoPin, INPUT);//echo, recibe, por tanto IN
  Serial.begin(9600);//inicia monitor serial
}

void loop() {


 reinicio(); // llamamos a la función para inicializarlo


  // mide cuanto tardó la onda sonora
  duration = pulseIn(echoPin, HIGH);

  // Calcular la distancia en cm
  //velocidad de la velocidad del sonido= duración
  //(de ida y vuelta) *0.034, todo se divide entre 2 para 
  //saber cuánto tardó de ida
  distance = duration * 0.034 / 2;

  // escribe distancia en el monitor serial
  Serial.print("Distancia: ");
  Serial.print(distance);
  Serial.println(" cm");

  // Espera 100 milisegundos antes de la próxima medición
  delay(100);
}
void reinicio(){
  digitalWrite(trigPin, LOW);//corta la onda sonora
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH); //la vuelve a lanzar
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);//la vuelve a cortar
}