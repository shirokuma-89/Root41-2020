// ISR(timer5Event) {
//   //ラインの記述はここに
//   line.read();
// }

_line::_line(void) {
  for (int i = 0; i <= 19; i++) {
    val[i] = false;
  }
}

int _line::calc(void) {
  float _deg;
  float _x = 0;
  float _y = 0;
  if (flag) {
    for (int i = 0; i <= 10; i++) {  //可変
      if (order[i] != 100) {
        _x += vector[order[i]][0];
        _y += vector[order[i]][1];
      }
    }
    _deg = atan2(_x, _y);
    _deg = degrees(_deg);
    if (_deg < 180) {
      _deg += 180;
    } else {
      _deg -= 180;
    }
  } else {
    _deg = 1000;
  }
  return _deg;
}

void _line::process(void) {
  if (flag) {
    if (!touch) {
      flag = false;
    }
    // if (mode == 1 && touch = true) {
    //   line.deg = atan2(line.x, line.y);
    //   line.deg = degrees(line.deg);
    //   if (line.deg < 180) {
    //     line.deg += 180;
    //   } else {
    //     line.deg -= 180;
    //   }
    // }
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
    delay(0);
  }

  bright = (lowestBright + highestBright * 2) / 3;
  bright = constrain(bright, 0, 255);
  EEPROM[13] = bright;

  analogWrite(LINE_BRIGHT, bright);

  Serial.println(bright);
}

void _line::read(void) {
  //読み込み
  touch = false;
  for (int i = 0; i <= 19; i++) {
    if (!digitalRead(LINE[i])) {
      if (whited == 0) {
        first = i;
      }
      if (check[i] == 0) {
        now = i;
        order[whited] = now;
        whited++;
        check[i] = 1;
      }
      flag = true;
      val[i] = true;
      touch = true;
      mode = 1;
    } else {
      val[i] = false;
    }
  }
}
