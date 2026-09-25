/*Usamos 6 pines, ENA, IN1, IN2, IN3, IN4 y ENB,
los IN son para cada motor (el L298n puede controlar
2), IN1 e IN2 para un motor, y el IN3 e IN4 para el otro;
los pines ENA y ENB son para decirle qué tan rápido girar, 
mientras que los IN son para decirle hacia donde debe girar, ya sea
enfrente o atrás. Debemos considerar que los pines IN pueden ser 
digitales o PWM, no importa mucho, pero los EN (enable A y enable B), 
sí o sí deben ser PWM, ya que ellos son los que mandan la velocidad, usando 
el PWM (desde 0-255). Primero vamos a declararlos
*/
int ENA = ;// PWM obligatorio
int IN1 = ;
int IN2 = ;
int IN3 = ;
int IN4 = ;
int ENB = ;// PWM obligatorio

/*Otra cosa que debemos recalcar, es que el driver tiene 3 pines de alimentación,
5v, GND, 12v (aveces VIN), pero no debemos conectar el pin 5v del arduino, nunca, ya que eso,
a palabras no tan técnicas, no le va a dar lo que ocupa, y podemos quemar la placa completa. 
Entonces, usamos una pila externa (las pilas que usan son las 18650, y con ese portapilas de 2, suman 7.4v-9v), el positivo de eso
SIEMPRE va a 12v o VIN, nunca a 5v, es algo más técnico, pero quédense con que siempre a 12v, luego, el lado negativo va a GND del driver, y aparte
conectamos un cable a gnd, hacia la arduino, de forma que queden todos con la tierra común
*/

/*Hay que resaltar el concepto de "tierra general o tierra común", es casi casi obligatorio que tengan la misma tierra todo el circuito, tipo
que todos estén conectados a GND, independientemente de si su pin positivo sea diferente*/


void setup() {
/*ahora, como todos esos pines mandan info 
hacia afuera, son OUT*/
pinMode(ENA, OUTPUT);
pinMode(IN1, OUTPUT);
pinMode(IN2, OUTPUT);
pinMode(IN3, OUTPUT);
pinMode(IN4, OUTPUT);
pinMode(ENB, OUTPUT);

}

void loop(){
/*Para que movamos el motor, es necesario usar los primeros pines IN y el ENA, pero recordando que el ENA es analógico y los IN son digitales
nos queda algo así un ejemplo:

digitalWrite(IN1, HIGH);
analogWrite(ENA, 250);
digitalWrite(IN2, LOW);

Esto vamos a desglosarlo por partes, en si, tenemos varias formas para decirle a donde girar con los IN, primero un HIGH/LOW, como arriba, eso de
alguna forma va a girar, peor hay que ver hacia donde, y eso solo lo vemos en físico, y ese ENA en medio, el número que le pusimos es la velocidad a la que
gira, recordando que es de 0-255, en este caso, iría casí a máxima velocidad.

Pero algo que debemos marcar también, es que hay 2 casos particulares que debmos ver, cuando es LOW/LOW, no girará a ningún lado, pero si es HIGH/HIGH, hace un tipo corto el cerebro, y eso puede hacer que se queme la placa arduino, el driver, y los motores, y viendolo desde otra perspectiva, es lo mismo que en rogic 
con los FF, FB, BF y BB, solo que ahora en vez de F y B, usamos HIGH y LOW, y en vez de 0-15 en rogic, usamos 0-255 

*/



/* aquí viene lo más confuso, pero ya es lo final, yo les recomiendo primero probar y comprobar las funciones y luego pasarlas a funciones para 
que sea más práctico*/

}

void avanzar(){
  /*Aquí vas a pegar la función cuando ya confirmes que avanza el robot con esa configuración HIGH/LOW*/
}

void derecha(){
  /*Aquí vas a pegar la función cuando ya confirmes que gira a la derecha el robot con esa configuración HIGH/LOW*/
}

void izquierda(){
  /*Aquí vas a pegar la función cuando ya confirmes que gira a la izquierda el robot con esa configuración HIGH/LOW*/
}

void atras(){
  /*Aquí vas a pegar la función cuando ya confirmes que retrocede el robot con esa configuración HIGH/LOW*/
}

void detener(){
  /*En este si lo puedo hacer, ya que no vamos a tener movimiento, y la velocidad no debe tener valor, y si la pones, como quiera lo va a ignorar*/
  digitalWrite(IN1, LOW);
  analogWrite(ENA, 0);
  digitalWrite(IN2, LOW);

  digitalWrite(IN3, LOW);
  analogWrite(ENB, 0);
  digitalWrite(IN4, LOW);
}


/*Al final, por si no se acuerdan como usar las funciones en el loop, les quedaría algo así:

void loop(){
avanzar();
delay(1000);
atrás();
delay(2000);
}

Pero eso es solo cuando ya confirmes que las funciones están correctas
*/



/*EJEMPLOS:

void detener(){
  digitalWrite(IN1, LOW);
  analogWrite(ENA, 0);
  digitalWrite(IN2, LOW):

  digitalWrite(IN3, LOW);
  analogWrite(ENB, 0);
  digitalWrite(IN4, LOW):
}

void avanzar(){
  digitalWrite(IN1, HIGH);
  analogWrite(ENA, 250);
  digitalWrite(IN2, LOW):

  digitalWrite(IN3, HIGH);
  analogWrite(ENB, 250);
  digitalWrite(IN4, LOW):
}

void derecha(){
  digitalWrite(IN1, HIGH);
  analogWrite(ENA, 250);
  digitalWrite(IN2, LOW):

  digitalWrite(IN3, LOW);
  analogWrite(ENB, 250);
  digitalWrite(IN4, HIGH):
}

void izquierda(){
  digitalWrite(IN1, LOW);
  analogWrite(ENA, 250);
  digitalWrite(IN2, HIGH):

  digitalWrite(IN3, HIGH);
  analogWrite(ENB, 250);
  digitalWrite(IN4, LOW):
}

*/