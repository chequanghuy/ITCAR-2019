#include "FUNCTION.h"
#define bit_is_set(sfr,bit) \
  (_SFR_BYTE(sfr) & _BV(bit))




#define handleCase10 -2
#define handleCase11 -4
#define handleCase12 -6
#define handleCase13 -14
#define handleCase14 -20
#define handleCase15 -26
#define handleCase16 -33
#define handleCase17 -40
#define handleCase18 -47

#define handleCase20 2
#define handleCase21 4
#define handleCase22 6
#define handleCase23 14
#define handleCase24 20
#define handleCase25 26
#define handleCase26 33
#define handleCase27 40
#define handleCase28 47

int speed_car = 255;

void bamLine();

int pattern2 = 0; //day la bien cho phep phat hien cac truong hop dac biet hay khong

void print_sensor(int val);

/* ================================== KHAI BÁO ================================== */
void setup() {
  Serial.begin(9600);
  init_sys();
  speed_r(0, 0);
  handle(0);
  pattern = 0;
  pinMode(BTN2, INPUT_PULLUP);
  Serial.print("Start");
  hoc_mau();
  for(int i=0; i<8; i++) {
    Serial.print(sensor_black[i]);
    Serial.print(':');
  }
  Serial.println();
  for(int i=0; i<8; i++) {
    Serial.print(sensor_white[i]);
    Serial.print(':');
  }
  Serial.println();
  for(int i=0; i<8; i++) {
    Serial.print((sensor_black[i] - sensor_white[i])/2);
    Serial.print(':');
  }
  while(digitalRead(BTN2) == HIGH) {
    delay(50);
  }
  while(digitalRead(BTN2) == LOW) {};
}


/* ================================== CHẠY CHƯƠNG TRÌNH ================================== */
void loop() {
      sensor = sensor_mask(0b11111111);
      if (enable_print) print_sensor(sensor);
      if ((sensor == 0b11110000 || sensor == 0b11111000 || sensor == 0b11111100) && pattern2 ==0) //phat hien tin hieu chuyen lan trai
      {
        uint8_t temp_sensor = sensor_mask(0b11111111);
        unsigned long timeCheck = millis();
        while (millis() - timeCheck < 50) { //kiem tra tin hieu xem co bi nhieu hay khong
          temp_sensor = sensor_mask(0b11111111);
          if(temp_sensor == 0xFF) goto runCross; //day la phat hien cua vuong hoac no line
          if(temp_sensor != 0b11110000 && temp_sensor != 0b11111000 && temp_sensor != 0b11111100 && temp_sensor != 0b11111110) {
            Serial.println("tin hieu gia");
            goto runNormal; //cho chay lai binh thuong
          }
        }
        Serial.print("left");
        speed_r(60,60);
        handle(-12); //-8
        while(temp_sensor == 0b11110000 || temp_sensor == 0b11111000 || temp_sensor == 0b11111100)  {temp_sensor = sensor_mask(0b11111111);}
        delay(300);
        handle(-14); //-10
        while(temp_sensor != 0x00) { temp_sensor = sensor_mask(0b11111111);}
        Serial.print("da xuong");
        while(temp_sensor == 0x00) { temp_sensor = sensor_mask(0b11111111);}
        while(temp_sensor != 0b00011000 && temp_sensor != 0b00111000 && temp_sensor != 0b00011100) { temp_sensor = sensor_mask(0b11111111);}
        Serial.print("da thoat");
        handle(25);
        speed_r(100, 80);
        digitalWrite(buzzer, LOW);
        delay(500);
        digitalWrite(buzzer, HIGH);
        pattern = 0;
        goto runNormal;
      }
      if ((sensor == 0b00001111 || sensor == 0b00011111 || sensor == 0b00111111) && pattern2 !=1)//phat hien tin hieu chuyen lan phai
      {
        uint8_t temp_sensor = sensor_mask(0b11111111);
        unsigned long timeCheck = millis();
        while (millis() - timeCheck < 50) { //kiem tra tin hieu xem co bi nhieu hay khong
          temp_sensor = sensor_mask(0b11111111);
          if(temp_sensor == 0xFF) goto runCross; //day la phat hien cua vuong hoac no line
          if(temp_sensor != 0b00001111 && temp_sensor != 0b00011111 && temp_sensor != 0b00111111 && temp_sensor != 0b01111111) {
            Serial.println("tin hieu gia");
            goto runNormal; //cho chay lai binh thuong
          }
        }
        Serial.print("right");
        speed_r(60,60);
        handle(10); //8
        while(temp_sensor == 0b00001111 || temp_sensor == 0b00011111 || temp_sensor == 0b00111111) {temp_sensor = sensor_mask(0b11111111);}
        delay(300);
        handle(11); //10
        Serial.println("da xuong day");
        while(temp_sensor != 0x00) { temp_sensor = sensor_mask(0b11111111);}
        while(temp_sensor == 0x00) { temp_sensor = sensor_mask(0b11111111);}
        
        while(temp_sensor != 0b00011000 && temp_sensor != 0b00011100 && temp_sensor != 0b00111000 && temp_sensor != 0b00000111 && temp_sensor != 0b00001111) { temp_sensor = sensor_mask(0b11111111);}
        Serial.print("da thoat");
        pattern = 0;
        handle(-25);
        speed_r(80, 100);
        digitalWrite(buzzer, LOW);
        delay(500);
        digitalWrite(buzzer, HIGH);
        pattern = 0;
        goto runNormal;
      }
      if(sensor == 0xFF) //neu phat hien gap cua vuong trai(phai), noline
      {
        runCross:
        unsigned long timeCheck = millis();
        while (timeCheck - millis() < 5) { //kiem tra tin hieu xem co bi nhieu hay khong
          sensor = sensor_mask(0b11111111);
          if(sensor != 0xFF) {
            Serial.println("tin hieu gia");
            goto runNormal; //cho chay lai binh thuong
          }
        }
        unsigned long timeDis = millis();
        Serial.print("line cua vuong hoac no line");
        digitalWrite(buzzer, LOW);
        handle(0);
        speed_r(50,50); //giam toc do dong co xuong tu tu
        delay(200);
        while (sensor == 0xFF) {sensor = sensor_mask(0b11111111);} //doi den khi nao qua line dac biet do
        digitalWrite(buzzer, HIGH);
        Serial.println("da het line");
        speed_r(40,40); //giam toc do dong co xuong tu tu
        delay(500);
        sensor = sensor_mask(0b11111111);
        pattern = 0;
                         //be trai                                                                                       be phai                                                                                         no line             
        while (1) { //chay bam duong den khi nao gap cac tin hieu re trai (phai), noline
          bamLine(0.5,8);
          sensor = sensor_mask(0xFF);
          if (!(sensor != 0b11110000 && sensor != 0b11111000 && sensor != 0b11111100 && sensor != 0b11111110 && sensor != 0b01111111 && sensor != 0b00111111  && sensor != 0b00011111 && sensor != 0b00001111 && sensor != 0b00000000 ) && ((millis() - timeDis) > 500)) {
            //Serial.println("be");
            break;
          }
          //print_sensor(sensor);
          
        }
       
        if (sensor == 0b11110000 || sensor == 0b11111000 || sensor == 0b11111100) {//|| sensor == 0b11111110
            Serial.println("cua trai");
            speed_r (-100,240);
            handle(-35);
            sensor=sensor_mask(0xFF);
            //delay(1500); //hoac while den khi gap cac tin hieu chay thuong
            while(sensor!=0b00011000&&sensor!=0b00111000&&sensor!=0b00011100&&sensor!=0b00010000&&sensor!=0b00001000){sensor=sensor_mask(0xFF);}
            
            pattern = 0;
            goto runNormal;
        }
        if (sensor == 0b00001111 || sensor == 0b00011111 || sensor == 0b00111111) {//|| sensor == 0b01111111
          Serial.println("cua phai");
          speed_r (240,-100);
          handle(35);
          //delay(1500);
          while(sensor!=0b00011000&&sensor!=0b00111000&&sensor!=0b00011100&&sensor!=0b00010000&&sensor!=0b00001000){sensor=sensor_mask(0xFF);}
          pattern = 0;
          goto runNormal;
        }
        if (sensor == 0x00) {
          Serial.println("no line");
          //tu xu ly
        }
        Serial.println("qua day roi");
      }
      runNormal:
      bamLine(0.8,3);
}
/* ================================== PRINT_SENSOR ================================== */
void print_sensor(int val) {
  for (int i=7; i>=0; i--) {
    if (bit_is_set(val, i)) {
      Serial.print(1);
    }
    else
      Serial.print(0);
  }
  Serial.println();
}
void bamLine(float ratioSpeed,float ratioHandle) {
  float tempSpeedCar = speed_car;
  speed_car *= ratioSpeed;
  //lay lai line giua
  if ( (sensor == 0b00011100 || sensor == 0b00011000 || sensor == 00111000||sensor==00111100||sensor==0b00010000||sensor==0b00001000)) {            
    pattern = 0;
    speed_r (speed_car, speed_car);
    handle(0);
    pattern2 = 0;
  }
  //xac dinh lech trai
  if (pattern == 0 && (sensor == 0b00110000 || sensor == 0b01110000 || sensor == 0b01100000 || sensor == 0b11100000 || sensor == 0b11000000)) {
    pattern = 10;
    
  }
  //xac dinh lech phai
  if (pattern == 0 && (sensor == 0b00001100 || sensor == 0b00001110 || sensor == 0b00000110 || sensor == 0b00000111 || sensor == 0b00000011)) {
    pattern = 20;
  }
 
  //lech trai
  if (pattern == 10) {
    switch (sensor) {
      case 0b00110000:
        handle(-3-ratioHandle);
        speed_r(0.9*speed_car,0.92*speed_car);
        pattern2 = 0;
        break;
      case 0b01110000:
        handle(handleCase11-ratioHandle);
        speed_r(0.745*speed_car,0.82*speed_car);
        pattern2 = 0;
        break;
      case 0b01100000:
        handle(handleCase12-ratioHandle);
        speed_r(0.7*speed_car,0.78*speed_car);
        pattern2 = 0;
        break;
      case 0b11100000:
        handle(handleCase13-ratioHandle);
        speed_r(0.63*speed_car,0.784*speed_car);
        pattern2 = 0;
        break;
      case 0b11000000:
        handle(handleCase14-ratioHandle);
        speed_r(0.55*speed_car,0.745*speed_car);
        pattern2 = 0;
        break;
      case 0b10000000:
        handle(handleCase15-ratioHandle);
        speed_r(0.43*speed_car,0.7*speed_car);
        pattern2 = 0;
        break;
      case 0b00000000:
        handle(handleCase16-ratioHandle);
        speed_r(0.27*speed_car,0.7*speed_car);
        pattern2 = 1;
        break;
      case 0b00000001:
        handle(handleCase17-ratioHandle);
        speed_r(0.24*speed_car,0.67*speed_car);
        pattern2 = 1;
        break;
      case 0b00000011:
        handle(handleCase18-ratioHandle);
        speed_r(0.2*speed_car,0.67*speed_car);
        pattern2 = 1;
        break;
      case 0b00000111:
        handle(handleCase18-ratioHandle);
        speed_r(0.17*speed_car,0.59*speed_car);
        pattern2 = 1;
        break;
    }
  }
  
  //lech phai
  if (pattern == 20) {
    switch (sensor) {
      case 0b00001100:
        handle(3+ratioHandle);
        speed_r(0.92*speed_car,0.9*speed_car); 
        pattern2 = 0;
        break;
      case 0b00001110:
        handle(handleCase21+ratioHandle);
        speed_r(0.82*speed_car,0.745*speed_car);
        pattern2 = 0;
        break;
      case 0b00000110:
        handle(handleCase22+ratioHandle);
        speed_r(0.78*speed_car,0.7*speed_car);
        pattern2 = 0;
        break;
      case 0b00000111:
        handle(handleCase23+ratioHandle);
        speed_r(0.784*speed_car,0.63*speed_car);
        pattern2 = 0;
        break;
      case 0b00000011:
        handle(handleCase24+ratioHandle);
        speed_r(0.745*speed_car,0.55*speed_car);
        pattern2 = 0;
        break;
      case 0b00000001:
        handle(handleCase25+ratioHandle);
        speed_r(0.7*speed_car,0.43*speed_car);
        pattern2 = 0;
        break;
      case 0b00000000:
        handle(handleCase26+ratioHandle);
        speed_r(0.7*speed_car,0.27*speed_car);
        pattern2 = 1;
        break;
      case 0b10000000:
        handle(handleCase27+ratioHandle);
        speed_r(0.67*speed_car,0.24*speed_car);
        pattern2 = 1;
        break;
      case 0b11000000:
        handle(handleCase28+ratioHandle);
        speed_r(0.67*speed_car,0.2*speed_car);
        pattern2 = 1;
        break;
      case 0b11100000:
        handle(handleCase28+ratioHandle);
        speed_r(0.59*speed_car,0.17*speed_car);
        pattern2 = 1;
        break;
    }
  }
  //khoi phuc lai gia tri truoc do cua speed_car
  speed_car = tempSpeedCar;
}
