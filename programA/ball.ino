void _ball::read(int* b) {
  for (int i = 0; i <= 15; i++) {
    // *(b + i) += (1 - LPF) * (analogRead(BALL[i]) - *(b + i));
    *(b + i) = analogRead(BALL[i]);

    // *b *= 1.07;
    // if (*(b + i) <= 100) {
    //   *(b + i) = 1000;
    // }

    // *(b + 2) *= 0.9;
    // *(b + 6) *= 0.9;
    // *(b + 10) *= 0.9;
    // *(b + 14) *= 0.9;
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
    if (val[i] >= 500) {
      existCount++;
    }
  }
  if (existCount <= 14) {  //ボールあります
    exist = true;
    deg = top * 22.5;

    int offset = 0;
    offset = dist * 18;

    offset = constrain(offset, -95, 95);

    const int error = 3;

    if (top != 0) {
      if (top <= error || top >= 16 - error)
        offset *= 0.9;

      if (top <= 1 || top >= 16 - 1)
        offset *= 0.5;

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
      if (val[top] <= 300 && top > 4 && top < 12) {
        speed = 90;
      } else if (val[top] <= 300 && top > 1 && top < 15) {
        speed = 90;
      } else {
        speed = 100;
      }
    }

    if (val[0] <= 300 && deg == 0) {
      if (val[15] > val[1] + 100) {
        deg = 15;
      } else if (val[15] < val[1] - 100) {
        deg = 345;
      }
    }

    if (digitalRead(BALL_HOLD) && !(top > 0 && top < 16)) {
      if (device.getTime() - holdTimer >= 100) {
        kicker.val = true;
      }
      speed = 100;

      deg = 0;
    }

    LED.dist = true;

  } else {  //ボールなし
    exist = false;
    speed = NULL;
    deg = NULL;
  }
}

void _ball::readDistance(void) {
  static float tempDist;
  if (true) {  // trueでローパス
    tempDist += (1 - 0.35) *
                (min(val[(top + 3) % 16], val[(top + 13) % 16]) - tempDist);
  } else {
    tempDist = min(val[(top + 3) % 16], val[(top + 13) % 16]);
  }

  dist = constrain(map(tempDist, 400, 500, 5, 0), 0, 5);
}
