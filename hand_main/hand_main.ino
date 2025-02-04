#include <SoftwareSerial.h>
#include <TroykaIMU.h>
#include <GyverOLED.h>
GyverOLED<SSD1306_128x64, OLED_NO_BUFFER> oled;
SoftwareSerial s(5, 6);  // RX, TX
Madgwick filter;
float sampleRate = 100;
float gx, gy, gz, ax, ay, az;
int yaw, roll, acc, pitch,ya,rol,pitc;
int but = 0;
unsigned long long timer1, timer2, timer3;
Gyroscope gyroscope;
Accelerometer accelerometer;
void setup() {
  oled.init();   // инициализация
  oled.clear();  // очистка
  // масштаб текста (1..4)
  oled.home();
  Serial.begin(9600);  // Инициализируем последовательное соединение для монитора порта.
  pinMode(5, INPUT);
  pinMode(9, INPUT_PULLUP);
  pinMode(10, INPUT_PULLUP);
  pinMode(6, OUTPUT);
  s.begin(115200);  // Инициализируем программное последовательное соединение для Bluetooth модуля на скорости 38400.
  gyroscope.begin();
  filter.begin();
  accelerometer.begin();
  start();
}

void start() {
  oled.setScale(2);
  oled.setCursor(0, 3);
  oled.print("Привет!");
  delay(1000);
  oled.clear();
  oled.setCursor(0, 2);
  oled.print("Ждём");
  oled.setCursor(0, 4);
  oled.print("Подключения");
  while (1) {
    if (s.available()) {
      int aaa = s.read();
      Serial.println(aaa);
      if (aaa == 1) {
        break;
      }
    }
        while (s.available()) {
          s.read();
        }
  }
  oled.clear();
  for (int i = 0; i < 30; i++) {
    s.write(1);
  }
}
void restart() {
  if (acc > 10 and (abs(roll) + abs(pitch) < 30)) {
    delay(3000);
    filter.reset();
  }
}
void gyro() {
  filter.reset();
  timer3 = millis();
}
bool but1() {
  return !digitalRead(9);
}
bool but2() {
  return !digitalRead(10);
}
void drive() {

  if (but1() and but2()) {
    gyro();
  } else if (but1()) {
    s.write(9);
  } else if (but2()) {
    s.write(6);

  } else if (roll < -60) {
    s.write(15);

  } else if (roll > -60 and roll < -20) {
    s.write(18);

  } else if (roll > 60) {
    s.write(21);

  } else if (roll < 60 and roll > 20) {
    s.write(24);

  } else if (yaw > 40) {
    s.write(27);

  } else if (yaw < 40 and yaw > 20) {
    s.write(30);

  } else if (yaw < -40) {
    s.write(33);

  } else if (yaw > -40 and yaw < -20) {
    s.write(36);

  } else {

    s.write(3);
  }
}
void imu() {
  unsigned long startMillis = millis();
  accelerometer.readAccelerationGXYZ(ax, ay, az);
  gyroscope.readRotationRadXYZ(gx, gy, gz);
  filter.setFrequency(sampleRate);
  filter.update(gx, gy, gz, ax, ay, az);
   ya = filter.getYawDeg();
   rol = filter.getRollDeg();
   pitc = filter.getPitchDeg();
  yaw = ya;
  roll = rol;
  pitch = pitc;
  if (abs(roll)>35){
    yaw=0;
    
  }
  acc = accelerometer.readAccelerationAY();
  unsigned long deltaMillis = millis() - startMillis;
  sampleRate = 1000 / deltaMillis;
  Serial.print(yaw);
  Serial.print(" ");
  Serial.print(roll);
  Serial.print(" ");
  Serial.print(pitch);
  Serial.println(" ");
}
void xyz() {
  if (millis() > 500 + timer2) {
    oled.clear();
    imu();
    oled.setScale(2);
    oled.setCursor(0, 1);
    imu();
    oled.print("x = ");
    imu();
    oled.print(ya);
    imu();
    oled.setCursor(0, 3);
    imu();
    oled.print("y = ");
    imu();
    oled.print(rol);
    imu();
    oled.setCursor(0, 5);
    imu();
    oled.print("z = ");
    imu();
    oled.print(pitc);
    imu();
    timer2 = millis();
  }
}
void loop() {
  imu();
  xyz();
  drive();
  // Ожидаем, пока данные появятся на мониторе порта.
  // bluetooth();
  // imu();

  // drive();

}
