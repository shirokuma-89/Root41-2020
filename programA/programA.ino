//ライブラリのインクルード
#include <Adafruit_NeoPixel.h>
#include <EEPROM.h>
#include <MPU6050_6Axis_MotionApps20.h>
#include <Timer5.h>
#include <VL53L0X.h>
#include <Wire.h>

//ピン番号定義
int BALL[16] = {A0, A1, A2,  A3,  A4,  A5,  A6,  A7,
                A8, A9, A10, A11, A12, A13, A14, A15};
#define BALL_RESET 26
#define BALL_HOLD 29

int LINE[20] = {30, 31, 32, 33, 34, 35, 36, 37, 38, 39,
                40, 41, 42, 43, 44, 45, 46, 47, 48, 49};

#define SW_2 22
#define SW_1 23
#define SW_RESET 25

//インスタンス作成
Adafruit_NeoPixel RGBLED = Adafruit_NeoPixel(16, 28, NEO_GRB + NEO_KHZ800);
FaBoLCDmini_AQM0802A lcd;
VL53L0X TOF;

class _ball {
 public:
  void read(int* b);
  void calc(void);
  
  int val[16];
  int top;
  int deg;

  bool exist;

 private:
  float LPF = 0.24;

} ball;

class _line {
 public:
  _line(void);
  void read(void);

  bool val[20];

 private:
  // none
} line;

class _motor {
 public:
  _motor(void);
  void directDrive(int* p);
  void drive(int _deg, int _power, bool _stop = false);
  void speed(void);

  int val[4];

 private:
  float Kp;
  float Ki;
  float Kd;

} motor;

class _gyro {
 public:
  void setting(void);
  int read(void);
  int differentialRead(void);

  int deg;
  int differentialDeg = 0;
  int eeprom[6];
  int offset;

 private:
  // none
} gyro;

class _tof {
 public:
  int read(void);

 private:
  int _dist;
} tof;

class _device {
 public:
  void initialize(void);
  void check(void);

  bool robot;

  int mode = 0;

 private:
  // none
} device;

class _LED {
 public:
  void gyroShow(unsigned long _color = 'hogehoge');
  void changeAll(int red, int green, int blue);
  void changeAll(unsigned long _color);
  void degShow(int d, unsigned long _color = 'hogehoge');

  int bright = 150;

  unsigned long defaltColor;
  unsigned long subColor;
  unsigned long RED;
  unsigned long BLUE;
  unsigned long GREEN;
  unsigned long YELLOW;
  unsigned long WHITE;
  unsigned long PURPLE;
  unsigned long LIME;
  unsigned long NONE;

 private:
  // none
} LED;

void setup(void) {
  device.initialize();
  device.mode = 0;

  Serial.begin(115200);

  gyro.setting();
  gyro.read();

  //起動イルミネーション
  for (int i = 0; i <= 15; i++) {
    RGBLED.begin();
    RGBLED.setBrightness(LED.bright);
    LED.changeAll(0, 0, 0);
    for (int k = 0; k <= i; k++) {
      RGBLED.setPixelColor(k, LED.WHITE);
    }
    RGBLED.show();
    delay(15);
  }

  delay(500);

  gyro.read();
}

void loop(void) {
  device.check();

  if (device.mode == 0) {  //待機中
    gyro.deg = gyro.read();
    LED.gyroShow();
    motor.drive(NULL, NULL, true);
  } else if (device.mode == 1) {  //駆動中
    //処理

    //駆動
    
  }
}
