#include <math.h>

#define motor_num 8
#define str_ind 4
#define str_pwm 100
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
bool ls_stat = 0;
int str_dir = 0;
int ls_str = 36;
long int pwm[motor_num] = {0};

void init(){
  ls_happening = false;
  hom = true;
  isr_ls = false;

  hom_dir = 0;
  ls_stat = false;
  str_dir = 0;
  ls_str = 36;
  pwm[motor_num] = {0};
}

void callback(){
  isr_ls = true;
}

void callback1(){
  int val = dipVal(dipPins);
  if(val == 1){
    //indicates that the homing process is started
    if(digitalRead(ls_str) == 1){ls_happening = true;}
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
  pinMode(dipPins[0], INPUT_PULLDOWN);
  attachInterrupt(digitalPinToInterrupt(ls_str), callback, CHANGE);
  attachInterrupt(digitalPinToInterrupt(dipPins[0]), callback1, CHANGE);
  Serial.begin(115200);
  init();
}

int val, a, b;

void loop() {
  // put your main code here, to run repeatedly:

  //check the input and see if the control is to move the motor
  
  a = digitalRead(ma);
  b = digitalRead(mb);
  // if(a == 1 && b == 0){
  //   digitalWrite(ina, 1);
  //   digitalWrite(inb, 0);
  //   analogWrite(pwmpin, 50);
  //   str_dir = 1;
  // }
  // else if(a == 0 && b == 1){
  //   digitalWrite(ina, 0);
  //   digitalWrite(inb, 1);
  //   analogWrite(pwmpin, 0);
  //   str_dir = -1;
  // }
  // else{
  //   str_dir = 0;
  // }
  if(a == 1 && b == 0){
    pwm[str_ind] = 50;
    str_dir = 1;
  }
  else if(a == 0 && b == 1){
    pwm[str_ind] = -50;
    str_dir = -1;
  }
  else{
    pwm[str_ind] = 0;
  }
  
  if(isr_ls == true){
    delay(50); //delay to sync values on the pins
    ls_stat = digitalRead(ls_str); //1 means non home, 0 is home
    ls_stat = ~ls_stat;  //1 is home, 0 is non home
    isr_ls = false;
    // Serial.println("ISR_LS is true");
    // Serial.println(ls_stat);
  }

  if(ls_stat == 1){
    hom = true;
    ls_happening = false;
    pwm[str_ind] = 0;
    init();
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
