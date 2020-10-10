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
    for (int i = 0; i <= 11; i++) {  //可変
      if (order[i] != 100) {
        _x += vector[order[i]][0];
        _y += vector[order[i]][1];
      }
    }
    if (_x == 0 || _y == 0) {
      if (_x == 0) {
        if (_y > 0) {
          deg = 0;
        } else {
          deg = 180;
        }
      } else if (_y == 0) {
        if (_x > 0) {
          deg = 90;
        } else {
          deg = 270;
        }
      }
    } else {
      _deg = atan2(_x, _y);
      _deg = degrees(_deg);
      if (_deg < 180) {
        _deg += 180;
      } else {
        _deg -= 180;
      }
    }
  } else {
    _deg = 1000;
  }
  return _deg;
}

void _line::process(void) {
  speed = 100;
  if (flag) {
    if (deg != 1000) {
      //ラインタッチ方向判定（四方向）
      if (deg <= 35 || deg >= 325) {
        side = 0;  //↑
      } else if (deg >= 145 && deg <= 215) {
        side = 2;  //↓
      } else if (deg < 180) {
        side = -1;  //→
      } else if (deg > 180) {
        side = 1;  //←
      }
    }
    if (_mode == 0) {
      // none
      flag = false;
    } else if (_mode == 1) {
      // stop
      deg = 1000;
      if (!touch) {
        //ストップ中にオーバーしたら緊急でもどる
        overTimer = device.getTime();
        _mode = 3;
      }
      if (device.getTime() - stopTimer >= 400) {
        //通常の動きに移行
        _mode = 2;
      }
      //センサーのタイマーでアプローチするかを判断
      approach = false;
      for (int i = 0; i <= 19; i++) {
        if (stopTime[i] >= 200) {
          if (abs(side) == 1) {
            if (ball.top >= 3 && ball.top <= 5) {
              _mode = 2;
              approachdeg = ball.top * 22.5;
              approach = true;
            } else if (ball.top >= 11 && ball.top <= 13) {
              _mode = 2;
              approachdeg = ball.top * 22.5;
              approach = true;
            }
          }
        }
      }
    } else if (_mode == 2) {
      // move
      if (approach) {
        _mode = 4;
      }
      if (!touch) {
        overTimer = device.getTime();
        _mode = 0;
      }
    } else if (_mode == 3) {
      // over
      if (touch) {
        _mode = 2;
      }
    } else if (_mode == 4) {
      // approach
      if (ball.deg != 1000) {
        if (touch) {
          deg = approachdeg;
          speed = 80;
        } else {
          approach = false;
          _mode = 3;
        }
      } else {
        _mode = 2;
      }
    }
  } else {
    //リセット
    for (int i = 0; i <= 19; i++) {
      val[i] = false;
      order[i] = 100;
      check[i] = 0;
      stopTime[i] = 0;
    }
    s = false;
    approach = false;
    now = 100;
    first = 100;
    whited = 0;
    side = 0;
    _mode = 0;
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
      if (!flag) {
        stopTimer = device.getTime();
        first = now;
        _mode = 1;
      }
      if (!val[i]) {
        stopingTimer[i] = device.getTime();
      }
      stopTime[i] = device.getTime() - stopingTimer[i];
      flag = true;
      val[i] = true;
      touch = true;
    } else {
      val[i] = false;
    }
  }
}
