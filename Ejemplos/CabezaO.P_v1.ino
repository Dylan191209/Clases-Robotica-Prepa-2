#include <Servo.h>
Servo izq;
Servo der;
Servo x;

int xpin = 4;
int derP = 3;
int izqP = 2;


void setup() {
  // put your setup code here, to run once:
izq.attach(izqP);
x.attach(xpin);
der.attach(derP);
Serial.begin(9600);
}

void loop() {
x.write(90);
abajo();
delay(1000);
}
void abajo (){
izq.write(1);
der.write(100);
}
void arriba (){
izq.write(100);
der.write(1);
}