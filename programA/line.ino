ISR(timer5Event) {
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
    for (int i = 0; i <= 19 i++) {
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
  for (int i = 0; i <= 255; i++;) {
    bright = i;
    if (abs(!digitalRead(LINE[14]) - !digitalRead(LINE[4])) >= dif) {
      dif = abs(!digitalRead(LINE[14]) - !digitalRead(LINE[4]));
      best = i;
    }
  }
  bright = best;
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
      }
    } else {
      val[i] = false;
    }
  }
  //順番記憶
  order[whited] = now;

  //足跡残す
  whited++;
}
