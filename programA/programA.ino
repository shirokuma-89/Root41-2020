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

#define LINE_BRIGHT 12

#define SW_1 22
#define SW_2 25
#define SW_RESET 23

#define SOLENOID 27

//インスタンス作成
Adafruit_NeoPixel RGBLED = Adafruit_NeoPixel(16, 28, NEO_GRB + NEO_KHZ800);
VL53L0X TOF;

class _ball {
 public:
  void read(int* b);
  void calc(void);

  bool exist;

  int val[16];
  int top;
  int deg;
  int dist;

 private:
  float LPF = 0.24;

} ball;

class _line {
 public:
  _line(void);
  void read(void);
  void autoadjustment(void);
  void process(void);

  //配列系
  bool val[20];
  int order[20];
  int check[20];

  int now;
  int first;
  int whited;

  bool flag;
  bool touch;
  int mode;
  int error;

  int bright;
  int dif;

  unsigned long stopTimer;

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
  void calibrationEEPROM(void);
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
  int _dist;

 private:
} tof;

class _position {
 public:
  void reflection(int _type);
  void get(void);

  bool rock;
  int side[4];
  int vertical[4];
  int reliability;  //信頼度

 private:
} position;

class _device {
 public:
  void initialize(void);
  void check(void);
  void UI(void);

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
  void animation1(void);
  void animation2(void);

  bool white = false;
  bool dist = true;

  int bright = 50;

  unsigned long defaultColor;
  unsigned long RED;
  unsigned long BLUE;
  unsigned long GREEN;
  unsigned long YELLOW;
  unsigned long WHITE;
  unsigned long PURPLE;
  unsigned long MINT;
  unsigned long LIME;
  unsigned long ORANGE;
  unsigned long NONE;

 private:
  // none
} LED;

class _kicker {
 public:
  // none
 private:
  // none
} kicker;

void setup(void) {
  RGBLED.begin();
  RGBLED.show();

  device.initialize();
  device.mode = 0;

  Serial.begin(115200);

  gyro.setting();
  gyro.read();

  //起動イルミネーション
  LED.animation1();
  LED.animation2();

  gyro.read();
}

void loop(void) {
  device.check();

  if (device.mode == 0) {  //待機中
    gyro.deg = gyro.read();
    LED.gyroShow();
    motor.drive(NULL, NULL, true);
    analogWrite(LINE_BRIGHT, 10);

    //ボタンによるUI処理
    device.UI();
  } else if (device.mode == 1) {  //駆動中
    //処理
    line.read();
    line.process();

    //駆動
  }
}
