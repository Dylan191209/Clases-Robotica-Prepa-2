/* 
Esta es para el sensor de color, y unos cuantos leds, fue usado para el primer robot de maze, los otros ejemplos en la carpeta son para calibrarlo y extras
Conexiones:
 TCS3200 (VCC) -> Arduino (5V)
 TCS3200 (GND) -> Arduino (GND)
 TCS3200 (S0)  -> Arduino (Pin 4)
 TCS3200 (S1)  -> Arduino (Pin 5)
 TCS3200 (S2)  -> Arduino (Pin 6)
 TCS3200 (S3)  -> Arduino (Pin 7)
 TCS3200 (OUT) -> Arduino (Pin 8)
 LED Rojo (lr) -> Arduino (Pin 3) (con resistencia de 220)
 LED Azul (la) -> Arduino (Pin 9) (igual con resistencia)

 Los pines varían según tu caso, tu los puedes elegir y no es necesario que sean PWM, y no pasa nada 
 si no le pones resistencias, si no tienes o no sabes de cuánto tienen que ser, pero es recomendable
 para evitar cortos y que pueda quemarse
*/
#define S0 4		// S0 a pin 4
#define S1 5		// S1 a pin 5
#define S2 6		// S2 a pin 6
#define S3 7		// S3 a pin 7
#define salidaTCS 8	// salidaTCS a pin 8
int lr =3;
int la =9;

void setup() {
  pinMode(lr, OUTPUT);	
  pinMode(la, OUTPUT);

  // Los "S" son para mandar órdenes (Salidas del Arduino)
  pinMode(S0, OUTPUT); //pin 4
  pinMode(S1, OUTPUT); //pin 5
  pinMode(S2, OUTPUT); //pin 6
  pinMode(S3, OUTPUT); //pin 7

  // El OUT es para escuchar al sensor (Entrada al Arduino)
  pinMode(salidaTCS, INPUT); //pin 8
  
  // Ponemos la velocidad al 20% de una vez
  digitalWrite(S0, HIGH);
  digitalWrite(S1, LOW);
  
  Serial.begin(9600);
}

void loop() {
  digitalWrite(S2,LOW);			// establece fotodiodos
  digitalWrite(S3,LOW);			// con filtro rojo
  int rojo = pulseIn(salidaTCS, LOW);	// obtiene duracion de pulso de salida del sensor
  delay(200);				// demora de 200 mseg
  
  digitalWrite(S2,HIGH);		// establece fotodiodos
  digitalWrite(S3,HIGH);		// con filtro verde
  int verde = pulseIn(salidaTCS, LOW);	// obtiene duracion de pulso de salida del sensor
  delay(200);				// demora de 200 mseg
  
  digitalWrite(S2,LOW);			// establece fotodiodos
  digitalWrite(S3,HIGH);		// con filtro azul
  int azul = pulseIn(salidaTCS, LOW);	// obtiene duracion de pulso de salida del sensor
  delay(200);				// demora de 200 mseg
  
  Serial.print("R:");			// muestra texto
  Serial.print(rojo);			// muestra valor de variable rojo

  Serial.print("\t");			// espacio de tabulacion

  Serial.print("V:");			// muestra texto
  Serial.print(verde);			// muestra valor de variable verde

  Serial.print("\t");			// espacio de tabulacion

  Serial.print("A:");			// muestra texto
  Serial.println(azul);			// muestra valor de variable azul
  					// y salto de linea


  /*A partir de aquí, usas if/else para los colores, es en formato RBG, no hexadecimal o parecidos,
  tienes que "calibrar" el sensor con esos 3 filtros R,G y B, en el monitor serial arrojan un número,
  sácale un aproximado de su rango a cada uno, dejando un margen de error para que pueda detectar otros
  tonos del color.

  Luego, podrás usar  'if/else if' para definir rangos, esto es un ejemplo por si lo de arriba no se 
  explica bien:

  if (rojo < 500 && verde > 1000 && azul > 1000) {
    // Es muy probable que sea Rojo
    Serial.println("¡Color detectado: ROJO!");
    digitalWrite(lr, HIGH); // Encender LED rojo (si está conectado)
  } else if (// condicion para VERDE) {
    // ...
  }
  // Y así sucesivamente para otros colores.
  // Recuerda considerar un margen de error en tus rangos.
  */
}