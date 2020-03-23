void _device::initialize(void) {
  LED.RED = RGBLED.Color(255, 0, 0);
  LED.BLUE = RGBLED.Color(0, 0, 255);
  LED.GREEN = RGBLED.Color(0, 255, 0);
  LED.YELLOW = RGBLED.Color(255, 255, 0);
  LED.WHITE = RGBLED.Color(255, 255, 255);
  LED.PURPLE = RGBLED.Color(255, 0, 200);
  LED.LIME = RGBLED.Color(120, 255, 14);
  LED.NONE = RGBLED.Color(0, 0, 0);

  Wire.begin();

  TOF.init();
  TOF.setTimeout(1);
  TOF.setMeasurementTimingBudget(10);
  TOF.startContinuous(0);

  RGBLED.begin();
  RGBLED.setBrightness(LED.bright);
  LED.changeAll(LED.GREEN);
  RGBLED.show();

  for (int i = 0; i <= 15; i++) {
    pinMode(BALL[i], INPUT);
  }
  pinMode(BALL_RESET, OUTPUT);
  pinMode(BALL_HOLD, INPUT);

  for (int i = 0; i <= 19; i++) {
    pinMode(LINE[i], INPUT);
  }

  pinMode(SW_1, INPUT_PULLUP);
  pinMode(SW_2, INPUT_PULLUP);
  pinMode(SW_RESET, INPUT_PULLUP);

  if (EEPROM[0] == 1) {
    //青号機
    device.robot = true;

    LED.defaltColor = RGBLED.Color(0, 150, 255);
    LED.subColor = RGBLED.Color(0, 0, 255);

    motor.cValue[0] = 0.98;
    motor.cValue[1] = 1.00;
    motor.cValue[2] = 0.99;
  } else {
    //赤号機
    device.robot = false;

    LED.defaltColor = RGBLED.Color(255, 100, 0);
    LED.subColor = RGBLED.Color(255, 0, 0);

    motor.cValue[0] = 1.00;
    motor.cValue[1] = 0.93;
    motor.cValue[2] = 0.84;
  }

  gyro.eeprom[0] = (EEPROM[1] * 256) + EEPROM[2];
  gyro.eeprom[1] = (EEPROM[3] * 256) + EEPROM[4];
  gyro.eeprom[2] = (EEPROM[5] * 256) + EEPROM[6];
  gyro.eeprom[3] = (EEPROM[7] * 256) + EEPROM[8];
  gyro.eeprom[4] = (EEPROM[9] * 256) + EEPROM[10];
  gyro.eeprom[5] = (EEPROM[11] * 256) + EEPROM[12];
}

void _device::check(void) {
  RGBLED.show();
  RGBLED.begin();
  RGBLED.clear();

  if (!digitalRead(SW_RESET)) {
    if ((!digitalRead(SW_1) || !digitalRead(SW_2)) && device.mode == 0) {
      RGBLED.begin();
      LED.changeAll(LED.LIME);
      RGBLED.show();
      gyro.setting();
      delay(200);
    } else {
      device.mode = 0;
    }
  } else if (!digitalRead(SW_1)) {
    device.mode = 1;
  } else if (!digitalRead(SW_2)) {
    device.mode = 2;
  }
}
