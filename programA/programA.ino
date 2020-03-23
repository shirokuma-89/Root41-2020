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

#define BUZ_1 14
#define BUZ_2 15
#define BUZ_3 16

//インスタンス作成
Adafruit_NeoPixel RGBLED = Adafruit_NeoPixel(16, 28, NEO_GRB + NEO_KHZ800);
FaBoLCDmini_AQM0802A lcd;
VL53L0X TOF;

class _ball {
 public:
  void read(int* b);
  void calc(void);
  void keeper(void);

  unsigned long lineOut;
  unsigned long lineTimer;

  int val[16];
  int _val[16];
  int speed = 80;
  int top;
  int deg;
  int Ldeg;
  int dist;
  int cm;
  int left = 0;
  int tdeg;

  bool exist;
  bool hold = false;
  bool turn;
  bool emg;

  int right;
  int _right;

  float degLPF = 0.2;

  unsigned long keeperOut;
  unsigned long keeperTime;

 private:
  int _top;
  int _deg;
  int exCount = true;

  float x;
  float y;

  float LPF = 0.24;

  unsigned long holdTimer;

} ball;

class _line {
 public:
  _line(void);
  void read(void);
  void process(void);

  bool flag;
  bool val[20];
  bool touch;
  bool firstTime = false;

  float deg = 1000;
  float Sdeg = 1000;
  float Ldeg = 1000;

  int col;
  int whited;
  int logs[20];
  int root1[3] = {100, 100, 100};
  int root2[3] = {100, 100, 100};
  bool rootsave;
  int direction;
  int height1;
  int height2;
  int first = 100;
  int second = 100;
  int last;
  int now;
  int right;
  int left;
  int way;
  int autodeg;
  int lock;
  int mode;
  int weight = 10;
  int sigdeg;
  int _deg;
  float rDeg;
  float nDeg;
  int rError;
  int nError;
  float firstX;
  float firstY;
  float firstCalc;

  float vector[20][2];
  float firstDeg;
  float x;
  float y;
  float Nx;
  float Ny;
  float gap;
  float _x;
  float _y;
  float backupDeg;
  float offsetX = 1.3;
  float offsetY = 1;

  unsigned long stopTimer;
  unsigned long overTimer;
  unsigned long lockTimer;
  unsigned long _millis;
  unsigned long Gmillis;

 private:
  bool _flag;
} line;

class _motor {
 public:
  _motor(void);

  void directDrive(int* p);
  void drive(int _deg, int _power, bool _stop = false);
  void speed(void);

  int val[3];
  float cValue[3];

  const int move = 60;

  unsigned long moveTimer = 0;
  int integral = 0;

  int calcVal[3][360];

 private:
  int front = 0;
  int _front;
  float Kp;
  float Ki;
  float Kd;
  float Km;

  int correctionVal = 0;
  int error;

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
  int dist;

 private:
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
  void lineShow(void);
  void topShow(void);

  bool white = false;
  bool dist = false;

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

  unsigned long timer;

 private:
  // none
} LED;

class _LCD {
 public:
  int output = 0;

  int data = NULL;
  String unit = "NULL";

  unsigned long timer;

 private:
} LCD;

class _carpet {
 public:
  int tile = 1;  // 0がパンチカーペット　　1がタイルカーペット

  int _lineWhited[2] = {15, 17};
  int _lineWhitedT[2] = {19, 10};  //タイマーのやつ

  float _motorPower[2] = {1.0, 1.0};

 private:
} carpet;

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
