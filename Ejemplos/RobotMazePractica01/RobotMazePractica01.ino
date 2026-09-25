//L298n
int ENA = 9;
int IN1 = 8;
int IN2 = 10;

int IN3 = 12;
int IN4 = 13;
int ENB = 11;

//ultras 
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

int distanciaF = 12;
int Dpasillo = 10;
int ladoL = 7;

void setup() {
  Serial.begin(19200);
  // L298n
  pinMode ( ENA, OUTPUT);
  pinMode ( IN1, OUTPUT);
  pinMode ( IN2, OUTPUT);
  pinMode ( IN3, OUTPUT);
  pinMode ( IN4, OUTPUT);
  pinMode ( ENB, OUTPUT);

  //Ultras
  pinMode ( trigg1, OUTPUT);
  pinMode (echo1, INPUT);

  pinMode ( trigg2, OUTPUT);
  pinMode (echo2, INPUT);

  pinMode ( trigg3, OUTPUT);
  pinMode (echo3, INPUT);
}

void loop() {
  lecturas(); // Llama a las funciones de lectura de los ultras

  if (distance1 < distanciaF) {
    // Hay un obstáculo al frente: ¡Detenerse y decidir un giro!
    detener();
    Serial.write('P'); // ENVIAR: 'P' para Parar
    delay(200); // Pequeña pausa para asegurar la detención

    bool derechaTransitable = (distance2 > Dpasillo);
    bool izquierdaTransitable = (distance3 > Dpasillo);

    // Caso 1: Derecha es transitable y la izquierda no (o está más cerrada)
    if (derechaTransitable && !izquierdaTransitable) {
      derecha();
      Serial.write('D'); // ENVIAR: 'D' para Derecha
      delay(550); // Ajusta para un giro de 90 grados
      detener();
      Serial.write('P'); // ENVIAR: 'P' para Parar
      delay(150);
    }
    // Caso 2: Izquierda es transitable y la derecha no (o está más cerrada)
    else if (izquierdaTransitable && !derechaTransitable) {
      izquierda();
      Serial.write('I'); // ENVIAR: 'I' para Izquierda
      delay(550); // Ajusta para un giro de 90 grados
      detener();
      Serial.write('P'); // ENVIAR: 'P' para Parar
      delay(150);
    }
    // Caso 3: Ambos lados son transitables: Priorizar el lado con más espacio
    else if (derechaTransitable && izquierdaTransitable) {
      if (distance2 >= distance3) { // Si derech a tiene más o igual espacio
        derecha();
        Serial.write('d'); // ENVIAR: 'd' para Derecha2 (minúscula)
      } else { // Si izquierda tiene más espacio
        izquierda();
        Serial.write('i'); // ENVIAR: 'i' para Izquierda2 (minúscula)
      }
      delay(550); // Ajusta para un giro de 90 grados
      detener();
      Serial.write('P'); // ENVIAR: 'P' para Parar
      delay(150);
    }
    // Caso 4: Frente bloqueado Y ambos lados no son transitables (callejón sin salida)
    else {
      atras(); // Retroceder un poco
      Serial.write('R'); // ENVIAR: 'R' para Retroceder
      delay(150); // Ajusta el tiempo para retroceder
      detener();
      Serial.write('P'); // ENVIAR: 'P' para Parar
      delay(150);
      derecha(); // Intentar un giro de 180 grados (ej. a la derecha)
      Serial.write('U'); // ENVIAR: 'U' para U-turn
      delay(900); // Ajusta para un giro de 180 grados
      detener();
      Serial.write('P'); // ENVIAR: 'P' para Parar
      delay(150);
    }
  } else {
    // El frente está libre, avanzar por defecto
    avanzar();
    Serial.write('A'); // ENVIAR: 'A' para Avanzar
  }

  // Pequeño delay para no saturar los sensores y el procesador
  delay(50);
}
void avanzar (){
  digitalWrite (IN1, HIGH);
  analogWrite (ENA, 150);
  digitalWrite (IN2, LOW);

  digitalWrite (IN3, HIGH);
  analogWrite (ENB, 150);
  digitalWrite (IN4, LOW); 
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

void lectura1() {
  digitalWrite (trigg1, LOW);
  delayMicroseconds (2);
  digitalWrite (trigg1, HIGH);
  delayMicroseconds (10);
  digitalWrite (trigg1, LOW);
  duration1 = pulseIn(echo1, HIGH);
  distance1 = duration1 * 0.034 / 2;
  // Manejo de errores: si la lectura es 0 o muy alta, asume muy lejos
  if (distance1 == 0 || distance1 > 400) distance1 = 999;
}

void lectura2() {
  digitalWrite (trigg2, LOW);
  delayMicroseconds (2);
  digitalWrite (trigg2, HIGH);
  delayMicroseconds (10);
  digitalWrite (trigg2, LOW);
  duration2 = pulseIn(echo2, HIGH);
  distance2 = duration2 * 0.034 / 2;
  // Manejo de errores: si la lectura es 0 o muy alta, asume muy lejos
  if (distance2 == 0 || distance2 > 400) distance2 = 999;
}

void lectura3() {
  digitalWrite (trigg3, LOW);
  delayMicroseconds (2);
  digitalWrite (trigg3, HIGH);
  delayMicroseconds (10);
  digitalWrite (trigg3, LOW);
  duration3 = pulseIn(echo3, HIGH);
  distance3 = duration3 * 0.034 / 2;
  // Manejo de errores: si la lectura es 0 o muy alta, asume muy lejos
  if (distance3 == 0 || distance3 > 400) distance3 = 999;
}

void lecturas() {
  lectura1();
  lectura2();
  lectura3();
  /*
  Serial.print("enfrente: ");
  Serial.print(distance1);
  Serial.print("cm || derecha: ");
  Serial.print(distance2);
  Serial.print("cm || izquierda: ");
  Serial.print(distance3);
  Serial.println("cm"); 
  delay(100);
  */
}