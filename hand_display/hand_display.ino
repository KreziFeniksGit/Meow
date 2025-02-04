#include <TroykaIMU.h>
//#define sw 
unsigned long rpstimer;
float sampleRate = 100;
float gx, gy, gz, ax, ay, az;
float yaw, pitch, roll, accy;
Accelerometer accelerometer;
Madgwick filter;
Gyroscope gyroscope;
uint8_t strk = 1;
String pg1[] = {"Игры", "Значения", "Настройки", "ИГРЫ", "ЗНАЧЕНИЯ", "НАСТРОЙКИ"};
String pggames[] = {"Реакция", "Повторяшка", "к/н/б", "РЕАКЦИЯ", "ПОВТОРЯШКА", "К/Н/Б"};
String rpslist[] = {"Три...", "Два...", "Один..."};
String rpsvars[] = {"Бумага!", "Ножницы!", "Камень!"};
bool joyt, joyb, joyl, joyr, change, rpsflag;
uint8_t rpsmas[] = {1,1,1,1,1,2,2,2,3,3,3,3}; 
//это ответы робота, 3 - камень, 2 - ножницы, 1 - бумага
uint8_t rpscount = 0;
uint8_t rpsplayer = 0;
uint8_t rpsrobot = 0;
uint8_t rpsvalue=0;
#include <GyverOLED.h>
GyverOLED<SSD1306_128x64, OLED_NO_BUFFER> oled;
unsigned long timer2;
void setup() {
  // put your setup code here, to run once:
  pinMode(A3, INPUT);
  pinMode(A2, INPUT);
  pinMode(4,INPUT_PULLUP);
  pinMode(9,INPUT_PULLUP);
  pinMode(10,INPUT_PULLUP);
  oled.init();
  oled.clear();
  oled.home();
  randomSeed(analogRead(A0));
  Serial.begin(9600);
  gyroscope.begin();
  // Инициализируем акселерометр
  accelerometer.begin();
  // Инициализируем фильтр
  filter.begin();
}
int y() {
  return analogRead(A2);
}
int x() {
  return analogRead(A3);
}

void imu() {
  accy = accelerometer.readAccelerationAY();
  unsigned long startMillis = millis();
  accelerometer.readAccelerationGXYZ(ax, ay, az);
  gyroscope.readRotationRadXYZ(gx, gy, gz);
  filter.setFrequency(sampleRate);
  filter.update(gx, gy, gz, ax, ay, az);
  yaw = filter.getYawDeg();
  pitch = filter.getPitchDeg();
  roll = filter.getRollDeg();;
  unsigned long deltaMillis = millis() - startMillis;
  sampleRate = 1000 / deltaMillis;
Serial.print(yaw);
Serial.print(" ");
Serial.print(pitch);
Serial.print(" ");
Serial.println(roll);
}
bool butL(){
  return !digitalRead(9);
}
bool butR(){
  return !digitalRead(10);
}
void joystick(uint8_t maxstrk) {
  if (y() < 5 and joyb == 0 and abs(x() - 500) < 100) {
    joyb = 1;
  }
  else if (y() > 5 and joyb == 1 and abs(x() - 500) < 100) {
    strk++;
    joyb = 0;
  }
  else if (y() > 1000 and joyt == 0 and abs(x() - 500) < 100) {
    joyt = 1;
  }
  else if (y() < 1000 and joyt == 1 and abs(x() - 500) < 100) {
    strk--;
    joyt = 0;
  }
  if (x() > 1000 and abs(y() - 500) < 100) {
    joyr = 1;
  }
  else {
    joyr = 0;
  }
  if (x() < 5 and abs(y() - 500) < 100) {
    joyl = 1;
  }
  else {
    joyl = 0;
  }
  if (joyl==0 and joyr==0){
      change=0;
    }
  if (strk < 1)strk = 1;
  if (strk > maxstrk)strk = maxstrk;
}
void pages() {
  strk = 1;
  uint8_t last;
  oled.setScale(2);
  while (1) {
    joystick(3);
    if (last != strk) {
      oled.setScale(2); oled.clear(); oled.home(); oled.print(pg1[0 + (strk == 1) * 3]); oled.setCursor(0, 2); oled.print(pg1[1 + (strk == 2) * 3]); oled.setCursor(0, 4); oled.print(pg1[2 + (strk == 3) * 3]);
      last = strk;
    }
    if (joyr == 1) {
      change = 1;
      if (strk == 1) {
        games();
      }
      if (strk == 2) {
        values();
      }
      if (strk==3){
        rpswon();
      }
      last = 0;

    

}

  }
}
void xyz() {
  if (millis() > 500 + timer2) {
    oled.clear(); oled.setScale(2); oled.setCursor(0, 1); oled.print("x = "); oled.print(52); oled.setCursor(0, 3); oled.print("y = "); oled.print(42); oled.setCursor(0, 5); oled.print("z = "); oled.print(666);

    timer2 = millis();
  }
}
void values() {
  strk = 1;
  while (joyl == 0) {
    xyz();
    joystick(1);
  }

}
void games() {
  strk = 1;
  uint8_t lastgames = 0;
  oled.setScale(2);
  while (change==1){//если пользователь все еще зажимает джойстик
    joystick(3);
     if (lastgames != strk) {//обновлять экран
      oled.setScale(2); oled.clear(); oled.home(); oled.print(pggames[0 + (strk == 1) * 3]); 
      oled.setCursor(0, 2); oled.print(pggames[1 + (strk == 2) * 3]);
      oled.setCursor(0, 4); oled.print(pggames[2 + (strk == 3) * 3]);
      lastgames = strk;
    }
  }
  while (joyl == 0) {//позволить пользоваться джойстиком
    joystick(3);
    if (lastgames != strk) {
      oled.setScale(2); oled.clear(); oled.home(); oled.print(pggames[0 + (strk == 1) * 3]); oled.setCursor(0, 2); oled.print(pggames[1 + (strk == 2) * 3]); oled.setCursor(0, 4); oled.print(pggames[2 + (strk == 3) * 3]);
      lastgames = strk;
    }
   
        if (joyr==1){
          change=1;
          if (strk==1){
            games();
          }
          if (strk==2){
            values();
          }
          if (strk==3){
            rpswon();
          }
          lastgames=0;
    
        
    }

  }
  change=1;
  pages();
}
uint8_t rps() {
  
  rpsflag = 0;
  rpscount = 0;
  if (rpsvalue==0){//проверяется, проводилась ли до этого калибровка
   rpsvalue= rpscal();//калибровка
  }
  oled.home(); oled.clear(); oled.print("Играйте!");
  while (1) {
    imu();
    if (accy > rpsvalue - 5 and rpsflag == 0) {//если человек взмахнул с калибровочной силой взмаха
      rpsflag = 1;

    }
    else if (accy < rpsvalue - 5 and rpsflag == 1) {//если человек опустил руку
      rpsflag = 0;
      if (millis() > rpstimer + 300) {//если было 300 милисекунд между взмахом
        rpscount++;//зафиксировать номер взмаха
        oled.setCursor(0, rpscount * 2);//вывести индикацию о взмахе
        oled.print(rpslist[rpscount - 1]);
      }

      rpstimer = millis();
    }

    if (rpscount == 3) {//если человек взмахнул 3 раза
       unsigned long pp = millis();
      while (millis() < pp + 1000) {
        
        imu();
      }
      oled.home();
      oled.clear();
     
      oled.print("Гироскоп считывает...");
      while (millis() < pp + 5000) {//считывать значения imu-сенсора
        
        imu();
      }
      oled.setCursor(0,2);
      if (abs(pitch) < 30) {//определение по углу наклона, что выкинул пользователь

        oled.print("Бумагу...");
        return 1;

      }
      else {
        if (abs(roll)<35) {
          oled.print("Ножницы...");
          return 2;

        }
        else {
          oled.print("Камень...");
          filter.reset();
          return 3;

        }
      }
    }
  }
}
uint8_t rpscal() {
  oled.clear(); oled.home(); oled.print("Сделайте кулак и ");
  oled.setCursor(0, 2); oled.print("нажмите на джойстик...");//пользователь делают кулак для калибровки
  while(!digitalRead(4)==0){}
  while (!digitalRead(4)==1){
   filter.reset();//калибруется imu-сенсор
delay(500);

  }
   oled.setCursor(0, 4); oled.print("Успешно");
  delay(1000);
  if(rpsvalue==0){//если калибровка на силу взмаха не проходила
    delay(1000);
  oled.clear(); oled.home(); oled.print("Проводится калибровка...");//просит пользователя взмахнуть рукой
   oled.setCursor(0, 2); oled.print("Встряхните рукой...");
  imu();
  while (accy < 10) {
    imu();
  }
  float accmax = 0;
  while (accy > 10 and accy < 20) {//считывание максимального вертикального ускорения в пространстве
    imu();
    if (abs(accy) > abs(accmax)) {
      accmax = accy;
    }
  }
  oled.setCursor(0, 4); oled.print("Ваша сила взмаха - "); 
  oled.print(abs(int(accmax))); delay(2000); oled.clear();// вывод силы взмаха
  return abs(accmax);
  }
  else{
    return rpsvalue;
  }
}
void rpswon() {
  oled.setScale(1);
  uint8_t rpsresults = rps();//считывание что выкинул пользователь
  uint8_t rpsbot = 0;
  rpsbot = rpsmas [random(11)];//генерация что выкидывает робот
  delay(1000); oled.setCursor(0, 2); oled.print("А робот выбрал...");
   delay(1000); 
   oled.setCursor(0, 4); oled.print(rpsvars[rpsbot - 1]); delay(1000); oled.setCursor(0, 6);//вывод ответ бота
  if (rpsresults == rpsbot) {//определение кто победил
    oled.print("Ничья!");
  }
  else if (rpsresults > rpsbot) {
       rpsplayer++;
    oled.print("Победа!");
 
  }
  else if (rpsresults == 1 and rpsbot == 3) {
     rpsplayer++;
    oled.print("Победа!");
   
  }
  else {
    rpsrobot++;
    oled.print("Проигрыш");
    
  }
  delay(2000);
  oled.clear(); oled.home(); oled.print("Счёт:"); //вывод счета
  oled.setCursor(0, 2); oled.print("Игрок - "); oled.print(rpsplayer);//предложение играть снова или выйти в меню
   oled.print(" Робот - "); oled.print(rpsrobot);
  oled.setCursor(0,6);oled.print("Вернуться?");
   oled.setCursor(62,6); oled.print("Продолжить?");
  while(joyl==0){
    joystick(1);
    if(joyr==1){//играть снова?
      rpswon();
    }
  }
  change=1;
  rpsvalue=0;
  games();//вернуться в меню?
}
void loop() {

  // put your main code here, to run repeatedly:
  pages();

//  rpswon();
  //к кбн б ннкк н кк
}
