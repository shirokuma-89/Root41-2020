ISR(timer5Event) {
  //ラインの記述はここに
  line.read();
}

_line::_line(void) {
  for (int i = 0; i <= 19; i++) {
    val[i] = false;
  }
}

void _line::process(void) {
  if (flag) {
    if (mode == 0) {
    }
  } else {
    //リセット
    for (int i = 0; i <= 19; i++) {
      val[i] = false;
      order[i] = 100;
      check[i] = 0;
    }
    now = 100;
    first = 100;
    whited = 0;
    mode = 0;
    error = 0;
  }
}

void _line::brightnessAdjust(void) {
  int lowestBright = 255;  //最低値
  int highestBright = 255;
  int accumulation = 0;
  bool reacted[20] = {};

  for (int i = 0; i <= 255; i++) {
    analogWrite(46, i);
    for (int j = 0; j <= 19; j++) {
      pinMode(LINE[j], INPUT);
      bool val = !digitalRead(LINE[j]);
      if (val) {
        if (lowestBright == 255) {
          lowestBright = i;
        }
        if (!reacted[j]) {
          accumulation++;
        }
        reacted[j] = true;
      }
      Serial.print(val);
    }
    Serial.println("");
    if (accumulation >= 8) {
      highestBright = i;
      // break;
    }
    //    delay(0);
  }

  bright = (lowestBright + highestBright * 2) / 3;
  bright = constrain(bright, 0, 255);
  EEPROM[13] = bright;

  analogWrite(LINE_BRIGHT, bright);

  Serial.println(bright);
}

void _line::read(void) {
  //読み込み
  for (int i = 0; i <= 19; i++) {
    if (!digitalRead(LINE[i])) {
      val[i] = true;
      if (whited == 0) {
        first = i;
      }
      if (check[i] == 0) {
        now = i;
        order[whited] = now;
        whited++;
        check[i] = 1;
      }
    } else {
      val[i] = false;
    }
  }
}
