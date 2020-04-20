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
  int valA = 255;
  int valA_pin = NULL;
  int valCount = 0;
  bool valZ[20];
  bool valB[20];
  int valC = 255;
  for (int i = 0; i <= 19; i++) {
    valB[i] = false;
    valZ[i] = true;
  }

  for (int i = 0; i <= 230; i += 10) {
  }

  bright = (valC + valA) / 2;
  bright = constrain(bright, 0, 255);
  EEPROM[13] = bright;

  analogWrite(LINE_BRIGHT, bright);
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
