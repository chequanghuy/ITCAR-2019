#include "function.h"
#define max_speed 255
unsigned char result;
void setup() {
  Serial.begin(9600);
  init_sys();
  handle(0);
}

void loop() {
  state = Serial.read();
  
  Serial.println("wait");
  switch (state) {
  case 'F':// đi thẳng
    speed_r(sp,sp);
    handle(0);
    break;
  case 'B'://lùi
    speed_r(-sp,-sp);
    handle(0);
    break;
  case 'L'://turn left
    speed_r(0,0);
    handle(-45);
    break;
  case 'R'://turn right
    speed_r(0,0);
    handle(45);
    break;
  case 'G':// Forward left
    speed_r(0.5*sp,sp);
    handle(-25);
    break;
  case 'I'://Forward right
    speed_r(sp,0.5*sp);
    handle(25);
    break;
  case 'H'://back left
    speed_r(-0.5*sp,-sp);
    handle(-25);
    break;
  case 'J'://back right
    speed_r(-sp,-0.5*sp);
    handle(25);
    break;
  case 'S'://back right
    speed_r(0,0);
    handle(0);
    break; 
  case '0':
    sp=0*max_speed;
    break; 
  case '1':
    sp=0.1*max_speed;
    break;
  case '2':
    sp=0.2*max_speed;
    break;
  case '3':
    sp=0.3*max_speed;
    break;
  case '4':
    sp=0.4*max_speed;
    break;
  case '5':
    sp=0.5*max_speed;
    break;
  case '6':
    sp=0.6*max_speed;
    break;
  case '7':
    sp=0.7*max_speed;
    break;
  case '8':
    sp=0.8*max_speed;
    break;
  case '9':
    sp=0.9*max_speed;
    break;
  case 'q':
    sp=max_speed;
    break;
  case 'V':
    digitalWrite(buzzer, LOW);
    tone(buzzer, 100, 100);
    break;
  case 'v':
    digitalWrite(buzzer, HIGH);
    break;
  default: 
   break;
  }
}
