#define ina 5
#define inb 4
#define pwmpin 17
#define ma 14
#define mb 27

void setup() {
  pinMode(ma, INPUT_PULLDOWN);
  pinMode(mb, INPUT_PULLDOWN);
  pinMode(pwmpin, OUTPUT);
  pinMode(ina, OUTPUT);
  pinMode(inb, OUTPUT);
  Serial.begin(115200);
}

int a, b;

void loop() {
  a = digitalRead(ma);
  b = digitalRead(mb);
  if(a == 1 && b == 0){
    digitalWrite(ina, 1);
    digitalWrite(inb, 0);
  }
  else{
    digitalWrite(ina, 0);
    digitalWrite(inb, 1);
  }
  if(a || b){
    analogWrite(pwmpin, 50);
  }
  else{
    analogWrite(pwmpin, 0);
  }
  delay(50);
}
