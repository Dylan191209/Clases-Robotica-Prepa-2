int lr = 3;
int la = 9;
int bz = 12;

void setup() {
  // put your setup code here, to run once:
pinMode(lr, OUTPUT);
pinMode(la, OUTPUT);
pinMode(bz, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
digitalWrite(lr, HIGH);
delay(1000);
digitalWrite(lr, LOW);
delay(1000);
digitalWrite(la, HIGH);
delay(1000);
digitalWrite(la, LOW);
delay(1000);
digitalWrite(bz, HIGH);
delay(1000);
digitalWrite(bz, LOW);
delay(1000);
}
