#include <Servo.h>
#define handle_center 95
#define BTN1 10
#define BTN2 11
#define BTN3 12

bool enable_print = false;

int buzzer = 13;

//=================================================================================================
//                                               sensor value define
//=================================================================================================

unsigned int sensor_white[8]={0,0,0,0,0,0,0,0};
unsigned int sensor_black[8]={1024,1024,1024,1024,1024,1024,1024,1024};

void hoc_mau() {
  while (digitalRead(BTN2) == HIGH) {
    delay(50);
  }
  while (digitalRead(BTN2) == LOW) {}
  Serial.println("hoc mau line trang");
  digitalWrite(buzzer, LOW);
  delay(1000);
  digitalWrite(buzzer, HIGH);
  while (digitalRead(BTN2) == HIGH) {
    for(int i=0; i<8; i++) {
      int val = analogRead(i);
      if (val > sensor_white[i]) {
        sensor_white[i]=val;
      }
    }
  }
  while (digitalRead(BTN2) == LOW) {}; 
  digitalWrite(buzzer, LOW);
  delay(1000);
  digitalWrite(buzzer, HIGH);
  while (digitalRead(BTN2) == HIGH) {};
  while (digitalRead(BTN2) == LOW) {};
  Serial.println("hoc mau line den");
  digitalWrite(buzzer, LOW);
  delay(1000);
  digitalWrite(buzzer, HIGH);
  while (digitalRead(BTN2) == HIGH) {
    for(int i=0; i<8; i++) {
      int val = analogRead(i);
      if (val < sensor_black[i]) {
        sensor_black[i]=val;
      }
    }
  }
  digitalWrite(buzzer, LOW);
  delay(1000);
  digitalWrite(buzzer, HIGH);
}

//________________________________________________MOTOR DEFINE_____________________________________
//Motor 1

int pinAIN1 = 3; //Direction
int pinAIN2 = 4; //Direction
int pinPWMA = 5; //Speed

//Motor 2
int pinBIN1 = 7; //Direction
int pinBIN2 = 8; //Direction
int pinPWMB = 6; //Speed

//Standby
int pinSTBY = 2;
static boolean turnCW = 0;
static boolean turnCCW = 1;
static boolean motor1 = 0;
static boolean motor2 = 1;
//________________________________________________BUTTON____________________________________________
int BT1 = 10;
int BT2 = 11;
int BT3 = 12;
//________________________________________________DEBUG DEVICE______________________________________

//________________________________________________VARIABLES_________________________________________
bool start;
unsigned long lastTime;
unsigned char pattern;
unsigned char pattern1;
unsigned char sensor;
uint8_t rc_angle;
uint8_t speed_ru_forward;

bool flat = false;
bool temp = true;

Servo myServo;
//__________________________________________________________________________________________________
void init_sys() {
  myServo.attach(9);
  pinMode(BT1, INPUT_PULLUP);
  pinMode(BT2, INPUT_PULLUP);
  pinMode(BT3, INPUT_PULLUP);

  pinMode(pinPWMA, OUTPUT);
  pinMode(pinAIN1, OUTPUT);
  pinMode(pinAIN2, OUTPUT);

  pinMode(pinPWMB, OUTPUT);
  pinMode(pinBIN1, OUTPUT);
  pinMode(pinBIN2, OUTPUT);
 
  pinMode(pinSTBY, OUTPUT);
  pinMode(buzzer, OUTPUT);
  digitalWrite(buzzer, HIGH);
}
//__________________________________________________FUNCTIONS ABOUT SENSOR_______________________________
// Đọc sensor
unsigned char read_sensor() {
  uint8_t ret=0x00;
  for (uint8_t i=0; i<8; i++) {
    int valRead = analogRead(i);
    if (enable_print)
    {
      Serial.print (valRead);
      Serial.print (" : ");
    }
    if (valRead < (int)((sensor_black[i] - sensor_white[i])/2)) {
      ret |= (1<<i); // ret = ret | (1<<i)
    }
  }
  if (enable_print)
    Serial.println();
  return ret;
}
unsigned char sensor_mask(unsigned char mask) {
  unsigned char result = read_sensor();
  return (result & mask);
}
bool check_crossLine() {
  if (sensor_mask(0b00011011) == 0b00011011) return true;                                               // CROSS LINE
  else return false;                                                                                    // SOMETHING ELSE
}
char check_crossHalfLine() {
  if (sensor_mask(0b00011100) == 0b00011100) return 1;                                                  // LEFT HALF LINE
  else if (sensor_mask(0b00000111) == 0b00000111) return 2;                                             // RIGHT HALF LINE
  else return 0;                                                                                        // SOMETHING ELSE
}
//___________________________________________________FUNCTION OF HARDWARE CONTROL______________________________
void handle(uint8_t angle) {
  rc_angle = handle_center + angle;
  myServo.write(rc_angle);
}
void motorDrive(boolean motorNumber, boolean motorDirection, int motorSpeed)
{
  /*
    This Drives a specified motor, in a specific direction, at a specified speed:
    - motorNumber: motor1 or motor2 ---> Motor 1 or Motor 2
    - motorDirection: turnCW or turnCCW ---> clockwise or counter-clockwise
    - motorSpeed: 0 to 255 ---> 0 = stop / 255 = fast
  */
  boolean pinIn1;  //Relates to AIN1 or BIN1 (depending on the motor number specified)
  //Specify the Direction to turn the motor
  //Clockwise: AIN1/BIN1 = HIGH and AIN2/BIN2 = LOW
  //Counter-Clockwise: AIN1/BIN1 = LOW and AIN2/BIN2 = HIGH
  if (motorDirection == turnCW) pinIn1 = HIGH;
  else pinIn1 = LOW;
  //Select the motor to turn, and set the direction and the speed
  if (motorNumber == motor1) {
    digitalWrite(pinAIN1, !pinIn1);
    digitalWrite(pinAIN2, pinIn1);
    analogWrite(pinPWMA, motorSpeed);
  }
  else {
    digitalWrite(pinBIN1, pinIn1);
    digitalWrite(pinBIN2, !pinIn1);
    analogWrite(pinPWMB, motorSpeed);
  }
  //Finally , make sure STBY is disabled - pull it HIGH
  digitalWrite(pinSTBY, HIGH);
}
void speed_r(int left, int right) {
  if (left >= 0) {
    motorDrive(motor1, turnCW, left);
  }
  else {
    motorDrive(motor1, turnCCW, -left);
  }
  if (right >= 0) {
    motorDrive(motor2, turnCW, right);
  }
  else {
    motorDrive(motor2, turnCCW, -right);
  }
}
bool get_button(uint8_t btn) {
  switch (btn) {
    case 1:
      if (digitalRead(BT1) == LOW) 
      {
        for (int i = 3000; i < 4500; i += 500) {
          tone(buzzer, i, 100);
          delay(100);
        }
        digitalWrite(buzzer, HIGH);       // TURN BUZZER OFF
        return true;
      }
      else return false;
    case 2:
      if (digitalRead(BT2) == LOW) {
        for (int i = 4000; i < 5500; i += 500) {
          tone(buzzer, i, 50);
          delay(100);
        }
        digitalWrite(buzzer, HIGH);    // TURN BUZZER OFF
        return true;
      }
      else return false;
    case 3:
      if (digitalRead(BT3) == LOW) {
        for (int i = 4000; i > 2500; i -= 500) {
          tone(buzzer, i, 100);
          delay(100);
        }
        digitalWrite(buzzer, HIGH);    // TURN BUZZER OFF
        return true;
      }
      else return false;
    default: return false;
  }
}
//______________________________________________________OTHER FUNCTION______________________________
void resetTimer() {
  lastTime = millis();
}
unsigned long getTimer() {
  return (millis() - lastTime);
}
