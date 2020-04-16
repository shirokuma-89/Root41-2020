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

    if (deg >= 180) {
      deg -= 360;
    }
    deg = round((float)deg * (float)sqrt(abs(deg)) * (float)0.092);
    deg += 720;
    deg %= 360;

    int offset = 0;
    offset = dist * 17;

    const int error = 3;

    int topDiff = abs(top > 8 ? top - 16 : top);

    if (topDiff >= 1) {
      if (topDiff == 1)
        offset *= 0.6;

      if (topDiff == 2)
        offset *= 0.7;

      if (topDiff > 2 && topDiff < 6) {
        offset *= 0.93;
      }

      if (topDiff >= 6)
        offset *= 0.7;

      if (val[8] <= 270) {
        offset += 30;
        LED.changeAll(LED.WHITE);
      }
      offset = constrain(offset, -95, 95);

      if (top >= 8) {
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
        speed = 100;
      } else if (val[top] <= 300 && top > 1 && top < 15) {
        speed = 100;
      } else {
        speed = 100;
      }
    }

    // if (val[0] <= 220 && deg == 0) {
    //   if (val[15] > val[1] + 50) {
    //     deg = 20;
    //     LED.changeAll(LED.WHITE);
    //   } else if (val[15] < val[1] - 50) {
    //     deg = 340;
    //     LED.changeAll(LED.WHITE);
    //   }
    // }

    if (digitalRead(BALL_HOLD) && !(top > 2 && top < 14)) {
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
  if (false) {  // trueでローパス
    tempDist += (1 - 0.15) *
                (min(val[(top + 2) % 16], val[(top + 14) % 16]) - tempDist);
  } else {
    tempDist = min(val[(top + 2) % 16], val[(top + 14) % 16]);
  }

  dist = constrain(myMap(tempDist, 300, 450, 5, 0), 0, 5);

  Serial.println(dist);
}

float myMap(float x, float in_min, float in_max, float out_min, float out_max) {
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}
