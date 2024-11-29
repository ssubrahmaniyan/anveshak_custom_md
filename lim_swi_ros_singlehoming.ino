//1 ls, 1 flag

#include<ros.h>
#include<std_msgs/Int32MultiArray.h>
#include<std_msgs/Bool.h>
#include<sensor_msgs/Joy.h>
#include<math.h>

#define ls_btn_1 6
#define ls_btn_2 7
#define motor_num 8
#define str_ind 4
#define str_pwm 255
#define ina 5
#define inb 4
#define pwmpin 17
#define ma 14
#define mb 27

std_msgs::Bool ls_msg;

bool ls_happening = false;
bool hom = true;
bool isr_ls = true;

int hom_dir = 0;
int ls_stat = 0;
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

void joyCallback(const sensor_msgs::Joy & msg)
{
  if (msg.axes[ls_btn_1] + msg.axes[ls_btn_2] == 2)
  {
    if (digitalRead(ls_str) == 1) ls_happening = true;
  }
}

// void messageCb(const std_msgs::Int32MultiArray & drive_inp)
// {
//   if (drive_inp.data[str_ind] > 0) str_dir = 1;
//   else if (drive_inp.data[str_ind] == 0) str_dir = 0;
//   else str_dir = -1;
// }

void callback()
{
  isr_ls = true;
}

ros::NodeHandle nh;
std_msgs::Int32MultiArray pwm_msg;
std_msgs::MultiArrayLayout pwm_layout;

ros::Publisher ls_pub("ls_happening", &ls_msg);
ros::Publisher pwm_pub("motor_pwm", &pwm_msg);

ros::Subscriber<sensor_msgs::Joy> sub("joy", joyCallback);
//ros::Subscriber<std_msgs::Int32MultiArray> sub_2("motor_pwm", messageCb);

void setup()
{
  pinMode(ls_str,INPUT_PULLDOWN);

  attachInterrupt(digitalPinToInterrupt(ls_str),callback,CHANGE);
  pinMode(ma, INPUT_PULLDOWN);
  pinMode(mb, INPUT_PULLDOWN);
  pinMode(ina, OUTPUT);
  pinMode(inb, OUTPUT);
  pinMode(pwmpin, OUTPUT);
  init();
  nh.getHardware()-> setBaud(115200);
  nh.initNode();
  nh.advertise(pwm_pub);
  nh.advertise(ls_pub);
  nh.subscribe(sub);
  //nh.subscribe(sub_2);
  pwm_msg.data_length = motor_num;
  pwm_msg.layout = pwm_layout;
}
int a, b;
void loop()
{
  nh.spinOnce();
  a = digitalRead(ma);
  b = digitalRead(mb);
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
  if (isr_ls == true)
  {
    delay(50);
    ls_stat = digitalRead(ls_str);
    ls_stat = !ls_stat;
    isr_ls = false;
  }
  if (ls_stat == 1)
  {
    hom = true;
    ls_happening = false;
    pwm[str_ind] = 0;
    init();
  }
  else
  {
    if (str_dir == 1) hom_dir = -1;
    else if (str_dir == -1) hom_dir = 1;
    hom = false;
  }
  

  if (ls_happening == true)
  {
    pwm[str_ind] = str_pwm*hom_dir;
  }

  pwm_msg.data = pwm;
  pwm_pub.publish(&pwm_msg);

  ls_msg.data = ls_happening;
  
  ls_pub.publish(&ls_msg);
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