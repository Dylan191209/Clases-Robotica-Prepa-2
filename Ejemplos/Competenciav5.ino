#include <Servo.h>
Servo servo1;
int echo = 11;
int trig = 10;
long duration;
int distance;
int delayval;
int servoPos = 0;
int servoreadl = 0;
int servoreadr = 0;
int IN1 = 1;
int IN2 = 2;
int IN3 = 8;
int IN4 = 7;
int sensor_d = 2;
int sensor_i = 4;
int val = 0;
int val2 = 0;
int medirDistancia(){
  digitalWrite(trig, LOW);
  delayMicroseconds(2);
  digitalWrite(trig, HIGH);
  delayMicroseconds(10);
  digitalWrite(trig, LOW);
  duration = pulseIn(echo, HIGH);
  distance = duration * 0.034 / 2;
  return distance;    
}
void setup() {
  // put your setup code here, to run once:
servo1.attach(9);
pinMode(trig, OUTPUT);
pinMode(echo, INPUT);
servo1.write(90);
Serial.begin(9600);
pinMode(sensor_d, INPUT);
pinMode(sensor_i, INPUT);
pinMode(IN1, OUTPUT);
pinMode(IN2, OUTPUT);
pinMode(IN3, OUTPUT);
pinMode(IN4, OUTPUT);

digitalWrite(IN1, LOW);
digitalWrite(IN2, LOW);
digitalWrite(IN3, LOW);
digitalWrite(IN4, LOW);
}

void loop() {
  // put your main code here, to run repeatedly:
distance = medirDistancia();
  Serial.println(distance);

  if(distance < 2)
  { stopCar();
    servo1.write(10);  
    delay(600); 
    servoReadr = medirDistancia();
                            
    servo1.write(170);  
    delay(600); 
    servoReadl = medirDistancia();

    servo1.write(90);  
    delay(600); 
if(servoreadl > servoreadr)
{
 Serial.println("giro izquierda");
 turnLeftCar();
}
if(servoreadr > servoreadl)
{
 Serial.println("giro derecha");
 turnRightCar();
}
if(Distance > 2)
{
 Serial.println("recto");
 moveFordwarCar();
}
val = digitalRead(sensor_d);
val2 = digitalRead(sensor_i);

if (val ==0 && val2 ==0)//recto
{
digitalWrite(IN1, LOW);
digitalWrite(IN2, HIGH);
digitalWrite(IN3, LOW);
digitalWrite(IN4, HIGH);
}
if (val ==0 && val2==1)// derecha
{
digitalWrite(IN1, LOW);
digitalWrite(IN2, HIGH);
digitalWrite(IN3, LOW);
digitalWrite(IN4, LOW);
}
if(val ==1 && val2==0)// izquierda
{
digitalWrite(IN1, LOW);
digitalWrite(IN2, LOW);
digitalWrite(IN3, LOW);
digitalWrite(IN4, HIGH);
}
if(val ==1 && val2==1)//linea negra
{
digitalWrite(IN1, LOW);
digitalWrite(IN2, LOW);
digitalWrite(IN3, LOW);
digitalWrite(IN4, LOW);
}
}
void stopCar(){
digitalWrite(IN1, LOW);
digitalWrite(IN2, LOW);
digitalWrite(IN3, LOW);
digitalWrite(IN4, LOW);
}
void turnRightCar()
{
digitalWrite(IN1, LOW);
digitalWrite(IN2, HIGH);
digitalWrite(IN3, LOW);
digitalWrite(IN4, LOW);
}
void turnLeftCar()
{
  digitalWrite(IN1, LOW);
digitalWrite(IN2, LOW);
digitalWrite(IN3, LOW);
digitalWrite(IN4, HIGH);
}
void moveFordwarCar()
{
  digitalWrite(IN1, LOW);
digitalWrite(IN2, HIGH);
digitalWrite(IN3, LOW);
digitalWrite(IN4, HIGH);
}

