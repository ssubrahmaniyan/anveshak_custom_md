#include <math.h>

#define motor_num 8
#define str_ind 4
#define str_pwm 255
#define ina 5
#define inb 4
#define pwmpin 17
#define ma 14
#define mb 27
#define ls1 36
#define ls2 39

int dipPins[] = {19, 21, 22, 23};

int dipVal(int* dipPins){
  int sum = 0;
  for(int i = 0; i < 4; i++){
    sum += digitalRead(dipPins[i]) * pow(2, i); 
  }
  return sum;
}

bool ls_happening = false;
bool hom = true;
bool isr_ls = false;

int hom_dir = 0;
int ls_stat = 0;
int str_dir = 0;
int ls_str = 36;
long int pwm[motor_num] = {0};

void callback(){
  isr_ls = true;
}

void callback1(){
  int val = dipVal(dipPins);
  if(val == 1){
    //indicates that the homing process is started
    if(digitalRead(isr_ls) == 0){ls_happening = true;}
  }
}
void setup() {
  // put your setup code here, to run once:
  pinMode(ls_str,INPUT_PULLDOWN);
  pinMode(ma, INPUT_PULLDOWN);
  pinMode(mb, INPUT_PULLDOWN);
  pinMode(ina, OUTPUT);
  pinMode(inb, OUTPUT);
  pinMode(pwmpin, OUTPUT);
  attachInterrupt(digitalPinToInterrupt(ls_str), callback, CHANGE);
  attachInterrupt(digitalPinToInterrupt(dipPins[0]), callback1, CHANGE);
  Serial.begin(115200);
}

int val, a, b;

void loop() {
  // put your main code here, to run repeatedly:

  //check the input and see if the control is to move the moto
  
  a = digitalRead(ma);
  b = digitalRead(mb);
  if(a == 1 && b == 0){
    digitalWrite(ina, 1);
    digitalWrite(inb, 0);
    str_dir = 1;
  }
  else if(a == 0 && b == 1){
    digitalWrite(ina, 0);
    digitalWrite(inb, 1);
    str_dir = -1;
  }
  else{
    str_dir = 0;
  }
  if(a || b){
    analogWrite(pwmpin, 50);
  }
  else{
    analogWrite(pwmpin, 0);
  }

  
  if(isr_ls == true){
    delay(50); //delay to sync values on the pins
    ls_stat = digitalRead(ls_str); //1 means non home, 0 is home
    ls_stat = ~ls_stat;  //1 is home, 0 is non home
  }

  if(ls_stat == 1){
    hom = true;
    ls_happening = false;
  }
  else{
    if(str_dir == 1) hom_dir = -1;
    else if(str_dir == -1) hom_dir = 1;
    hom = false;
  }

  if(ls_happening == true){
    pwm[str_ind] = str_pwm * hom_dir;
  }
  
  if(pwm[str_ind] > 0){
    digitalWrite(ina, 1);
    digitalWrite(inb, 0);
    analogWrite(pwmpin, pwm[str_ind]);
  }
  else if(pwm[str_ind] < 0){
    digitalWrite(ina, 0);
    digitalWrite(inb, 1);
    analogWrite(pwmpin, -1 * pwm[str_ind]);
  }
  else{
    digitalWrite(ina, 0);
    digitalWrite(inb, 0);
    analogWrite(pwmpin, 0);
  }
  
  delay(100);
}
