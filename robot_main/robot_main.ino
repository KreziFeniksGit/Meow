#include <Servo.h>
#include <SoftwareSerial.h>
SoftwareSerial ss(5, 4);
#define rfbp 11
#define rffp 10  //shim
#define lffp 13  //shim
#define lfbp 12
#define lbbp 9//shim
#define lbfp 8
#define rbfp 7
#define rbbp 6  //shim
bool forward_flag = 0;
int stat = 0;
int ch;
int serv = 100;
int serv1 = 0;
bool bool1 = 1;
float filter = 0;
int count;
unsigned long timer, timer1, timer2, timer3, timer4;
Servo s;
Servo s1;
void setup() {
  pinMode(rffp, OUTPUT);
  pinMode(lffp, OUTPUT);
  pinMode(lfbp, OUTPUT);
  pinMode(rfbp, OUTPUT);
  pinMode(lbbp, OUTPUT);
  pinMode(lbfp, OUTPUT);
  pinMode(rbfp, OUTPUT);
  pinMode(rbbp, OUTPUT);
  Serial.begin(9600);
  ss.begin(115200);
  //s.attach(12);//180-120
  //  s1.attach(13);//180-120
  s.write(serv);
  s1.write(serv1);
  //  start();
}
void start() {
  while (1) {
    ss.write(1);
    if (ss.available()) {
      int aaa = ss.read();
      if (aaa == 1) {
        break;
      }
    }
    while (ss.available()) {
      ss.read();
    }
  }

}
void brake() {
  digitalWrite(rffp, 0);
  digitalWrite(rbfp, 0);
  digitalWrite(lffp, 0);
  digitalWrite(lbfp, 0);
  digitalWrite(rfbp, 0);
  digitalWrite(lbbp, 0);
  digitalWrite(lfbp, 0);
  digitalWrite(rbbp, 0);
}
void forward_slow() {
  int l;
  if (millis() > timer1 + 15) {
    l = 1;
    timer1 = millis();
    digitalWrite(rffp, 1);
    digitalWrite(rbfp, 1);
    digitalWrite(lffp, 1);
    digitalWrite(lbfp, 1);
    digitalWrite(rfbp, 0);
    digitalWrite(lbbp, 0);
    digitalWrite(lfbp, 0);
    digitalWrite(rbbp, 0);

  }
  else {
    l = 0;
    brake();
  }
  Serial.println(l);
}
void forward_fast() {
  int l;
  if (millis() > timer1 + 10) {
    l = 1;
    timer1 = millis();
    digitalWrite(rffp, 1);
    digitalWrite(rbfp, 1);
    digitalWrite(lffp, 1);
    digitalWrite(lbfp, 1);
    digitalWrite(rfbp, 0);
    digitalWrite(lbbp, 0);
    digitalWrite(lfbp, 0);
    digitalWrite(rbbp, 0);
  }
  else {
    l = 0;
    brake();
  }
  Serial.println(l);
}
void back_fast() {
  int l;
  if (millis() > timer1 + 10) {
    l = 1;
    timer1 = millis();
    digitalWrite(rffp, 0);
    digitalWrite(rbfp, 0);
    digitalWrite(lffp, 0);
    digitalWrite(lbfp, 0);
    digitalWrite(lfbp, 1);
    digitalWrite(rfbp, 1);
    digitalWrite(lbbp, 1);
    digitalWrite(rbbp, 1);
  }
  else {
    l = 0;
    brake();
  }
  Serial.println(l);
}
void back_slow() {
  int l;
  if (millis() > timer1 + 15) {
    l = 1;
    timer1 = millis();
    digitalWrite(rffp, 0);
    digitalWrite(rbfp, 0);
    digitalWrite(lffp, 0);
    digitalWrite(lbfp, 0);
    digitalWrite(lfbp, 1);
    digitalWrite(rfbp, 1);
    digitalWrite(lbbp, 1);
    digitalWrite(rbbp, 1);
    
  }
  else {
    l = 0;
    brake();
  }
  Serial.println(l);
}
void right_fast() {
  int l;
  if (millis() > timer1 + 10) {
    l = 1;
    timer1 = millis();
    digitalWrite(rffp, 0);
    digitalWrite(rbfp, 0);
    digitalWrite(lffp, 1);
    digitalWrite(lbfp, 1);
    digitalWrite(rbbp, 1);
    digitalWrite(rfbp, 1);
    digitalWrite(lfbp, 0);
    digitalWrite(lbbp, 0);
  }
  else {
    l = 0;
    brake();
  }
  Serial.println(l);
}
void left_fast() {
  int l;
  if (millis() > timer1 + 10) {
    l = 1;
    timer1 = millis();
    digitalWrite(rffp, 1);
    digitalWrite(rbfp, 1);
    digitalWrite(lffp, 0);
    digitalWrite(lbfp, 0);
    digitalWrite(rbbp, 0);
    digitalWrite(rfbp, 0);
    digitalWrite(lfbp, 1);
    digitalWrite(lbbp, 1);
  }
  else {
    l = 0;
    brake();
  }
  Serial.println(l);
}
void right_slow() {
  int l;
  if (millis() > timer1 + 15) {
    l = 1;
    timer1 = millis();
    digitalWrite(rffp, 0);
    digitalWrite(rbfp, 0);
    digitalWrite(lffp, 1);
    digitalWrite(lbfp, 1);
    digitalWrite(rbbp, 1);
    digitalWrite(rfbp, 1);
    digitalWrite(lfbp, 0);
    digitalWrite(lbbp, 0);
  } else {
    l = 0;
    brake();
  }
   Serial.println(l);
}
void left_slow() {
  int l;
  if (millis() > timer1 + 15) {
    l = 1;
    timer1 = millis();
    digitalWrite(rffp, 1);
    digitalWrite(rbfp, 1);
    digitalWrite(lffp, 0);
    digitalWrite(lbfp, 0);
    digitalWrite(rbbp, 0);
    digitalWrite(rfbp, 0);
    digitalWrite(lfbp, 1);
    digitalWrite(lbbp, 1);
  } else {
    l = 0;
    brake();
  }
  Serial.println(l);
}
void drive() {
  if (stat == 27) {
    right_fast();
  } else if (stat == 30) {
    right_slow();
  } else if (stat == 33) {
    left_fast();
  } else if (stat == 36) {
    left_slow();
  } else if (stat == 15) {
    forward_fast();
  } else if (stat == 18) {
    forward_slow();
  } else if (stat == 21) {
    back_fast();
  } else if (stat == 24) {
    back_slow();
  } else {
    brake();
  }
}
void manip() {
  if ( 1) {
    if (millis() > 5 + timer2) {
      timer2 = millis();
      if (bool1) {
        serv1++;
        if (serv1 > 160) {
          bool1 = !bool1;
        }
      } else {
        serv1--;
        if (serv1 < 20) {
          bool1 = !bool1;
        }
      }
      //      s1.attach(13);//180-120
      s1.write(serv1);
      delay(1);
    } else {
      //        s.detach();//180-120
      //  s1.detach();//180-120
    }
  }
}
void bluetooth() {
  if (ss.available()) {
    timer3 = millis();
    timer4 = millis();
    int aaa = ss.read();

    if (aaa < 40 and aaa >= 0) {
      filter += float(aaa);
    }
   
      stat = round(filter);
      filter = 0;
    }
  
  else {
//    if (millis() > timer3 + 3000) {
//      asm volatile("jmp 0x00");
//      timer3 = millis();
//    }
//    if (millis() > timer4 + 1000) {
//      stat = 3;
//      timer4 = millis();
//    }
  }
  while (ss.available()) {
    ss.read();
  }
  Serial.println(stat);
  //Serial.println(filter);
}
void loop() {
  //manip();
    bluetooth();
  //  drive();
 
}
