void _ball::read(int* b) {
  for (int i = 0; i <= 15; i++) {
    *(b + i) += (1 - LPF) * (analogRead(BALL[i]) - *(b + i));
    if (*(b + i) <= 100) {
      *(b + i) = 1000;
    }
  }

  digitalWrite(BALL_RESET, LOW);
}

void _ball::calc(void) {
  int existCount = 0;
  top = 0;
  for (int i = 0; i <= 15; i++) {
    if (val[i] <= val[top]) {
      top = i;
    }
    if (val[i] >= 490) {
      existCount++;
    }
  }
  if (existCount <= 14) {  //ボールあります
    exist = true;
    deg = top * 22.5;

    dist = constrain(map(existCount, 2, 9, 5, 0), 0, 5);
    if (dist <= 2)
      dist = 0;

    int offset = 0;
    offset = dist * 15;

    if (val[top] <= 220 && dist <= 3) {
      offset += 40;
      offset = constrain(offset, -60, 60);
    }

    offset = constrain(offset, -95, 95);
    if (top != 0) {
      if (top < 3 || top > 13) {
        offset *= 0.4;
      }
      if (deg >= 180) {
        deg -= offset;
      } else {
        deg += offset;
      }
    }

    //ホールド処理
    if (!digitalRead(BALL_HOLD)) {
      holdTimer = device.getTime();
      kicker.val = false;
      if (val[top] <= 300 && top > 1 && top < 15) {
        speed = 80;
      } else {
        speed = 100;
      }
    }

    if (digitalRead(BALL_HOLD)) {
      if (device.getTime() - holdTimer >= 100) {
        kicker.val = true;
      }
      speed = 100;
    }

    Serial.println(existCount);
  } else {  //ボールなくて草wwwww
    exist = false;
    speed = NULL;
    deg = NULL;
  }
}
