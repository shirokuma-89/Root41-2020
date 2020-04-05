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
  int valA = 0;
  int valA_pin = NULL;
  int valCount = 0;
  float valZ;
  bool valB[20];
  int valC = 255;
  for (int i = 0; i <= 19; i++) {
    valB[i] = false;
  }
  for (int i = 0; i <= 255; i++) {
    analogWrite(LINE_BRIGHT, i);
    Serial.println(valA);
    delay(10);
    for (int j = 0; j <= 19; j++) {
      valZ = 0;
      for (int k = 0; k < 10; k++) {
        valZ += (1 - 0.7) * (!digitalRead(LINE[j]) - valZ);
      }

      if (round(valZ) && !valA) {
        valA = i;
        valA_pin = j;
      }
      if (round(valZ) && !valB[j]) {
        valCount++;
        valB[j] = true;
        if (valCount >= 10) {
          valC = i;
        }
      }
    }
  }
  bright = (valC + valA * 16) / 2;
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
