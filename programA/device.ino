void _device::initialize(void) {
  LED.RED = RGBLED.Color(255, 0, 0);
  LED.BLUE = RGBLED.Color(0, 0, 255);
  LED.GREEN = RGBLED.Color(0, 255, 0);
  LED.YELLOW = RGBLED.Color(255, 255, 0);
  LED.WHITE = RGBLED.Color(255, 255, 255);
  LED.PURPLE = RGBLED.Color(255, 0, 200);
  LED.ORANGE = RGBLED.Color(255, 100, 0);
  LED.MINT = RGBLED.Color(100, 255, 50);
  LED.LIME = RGBLED.Color(190, 255, 0);
  LED.NONE = RGBLED.Color(0, 0, 0);

  Wire.begin();

  TOF.init();
  TOF.setTimeout(1);
  TOF.setMeasurementTimingBudget(10);
  TOF.startContinuous(0);

  RGBLED.begin();
  RGBLED.setBrightness(LED.bright);
  LED.changeAll(LED.BLUE);
  RGBLED.show();

  for (int i = 0; i <= 15; i++) {
    pinMode(BALL[i], INPUT);
  }
  pinMode(BALL_RESET, OUTPUT);
  pinMode(BALL_HOLD, INPUT);

  for (int i = 0; i <= 19; i++) {
    pinMode(LINE[i], INPUT_PULLUP);
  }

  pinMode(SW_1, INPUT_PULLUP);
  pinMode(SW_2, INPUT_PULLUP);
  pinMode(SW_RESET, INPUT_PULLUP);

  pinMode(SOLENOID, 27);

  if (EEPROM[0] == 1) {
    //緑号機
    device.robot = true;

    LED.defaultColor = LED.GREEN;
  } else {
    //赤号機
    device.robot = false;

    LED.defaultColor = LED.RED;
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
    device.mode = 0;
  } else if (!digitalRead(SW_1)) {
    device.mode = 1;
  } else if (!digitalRead(SW_2)) {
    device.mode = 2;
  }
}

void _device::UI(void) {
  if (!digitalRead(SW_RESET)) {
    if (!digitalRead(SW_1)) {
      LED.changeAll(LED.BLUE);
      RGBLED.show();

      delay(20);  //チャッタリング防止
      while (!digitalRead(SW_RESET)) {
      }
      delay(200);

      while (true) {
        if (!digitalRead(SW_RESET)) {  //戻るボタン
          break;
        }

        if (!digitalRead(SW_1)) {
          LED.changeAll(LED.ORANGE);
          RGBLED.show();
          line.autoadjustment();
          LED.animation1();
          delay(500);
          break;
        }

        if (!digitalRead(SW_2)) {
          LED.changeAll(LED.MINT);
          RGBLED.show();

          while (true) {
            LED.changeAll(LED.MINT);
            RGBLED.show();
            if (!digitalRead(SW_2)) {
              LED.changeAll(LED.LIME);
              RGBLED.show();
              digitalWrite(SOLENOID, HIGH);
              delay(200);
              digitalWrite(SOLENOID, LOW);
              delay(1200);
            }

            if (!digitalRead(SW_RESET)) {
              break;
            }
          }
          break;
        }
      }

    } else if (!digitalRead(SW_2)) {
      LED.changeAll(LED.YELLOW);
      RGBLED.show();

      delay(20);  //チャッタリング防止
      while (!digitalRead(SW_RESET)) {
      }
      delay(200);

      while (true) {
        if (!digitalRead(SW_RESET)) {  //戻るボタン
          break;
        }

        if (!digitalRead(SW_1)) {
          LED.changeAll(LED.PURPLE);
          RGBLED.show();
          gyro.calibrationEEPROM();
          break;
        }

        if (!digitalRead(SW_2)) {
          LED.changeAll(LED.GREEN);
          RGBLED.show();
          gyro.setting();
          gyro.read();
          LED.animation1();
          delay(500);
          break;
        }
      }
    }
  }
}
