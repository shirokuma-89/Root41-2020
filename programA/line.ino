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

void _line::autoadjustment(void) {
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

  for (int i = 200; i <= 235; i += 10) {
    analogWrite(LINE_BRIGHT, i);
    Serial.println(valA);
    for (int j = 0; j <= 19; j++) {
      valZ[j] = true;
    }
    for (int j = 0; j < 30; j++) {
      for (int k = 0; k < 20; k++) {
        valZ[k] = valZ[k] & !digitalRead(LINE[k]);
      }
      delay(10);
    }
    for (int j = 0; j < 20; j++) {
      if (valZ[j] && valA == 255) {
        valA = i;
      }
    }
  }
  for (int i = 235; i <= 255; i += 1) {
    analogWrite(LINE_BRIGHT, i);
    Serial.println(valA);
    for (int j = 0; j <= 19; j++) {
      valZ[j] = true;
    }
    for (int j = 0; j < 30; j++) {
      for (int k = 0; k < 20; k++) {
        valZ[k] = valZ[k] & !digitalRead(LINE[k]);
      }
      delay(10);
    }
    for (int j = 0; j < 20; j++) {
      if (valZ[j] && valA == 255) {
        valA = i;
      }
    }
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
