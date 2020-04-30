void _ball::read(int* b) {
  // digitalWrite(BALL_RESET, HIGH);
  for (int i = 0; i <= 15; i++) {
    // *(b + i) += (1 - LPF) * (analogRead(BALL[i]) - *(b + i));
    *(b + i) = analogRead(BALL[i]);
    // Serial.print(*(b + i));
    // Serial.print(" ");
  }
  // Serial.println(" ");
  digitalWrite(BALL_RESET, LOW);

  *b *= 0.95;
}

void _ball::calc(void) {
  int existCount = 0;
  top = 0;
  for (int i = 0; i <= 15; i++) {
    if (val[i] <= val[top]) {
      top = i;
    }
    if (val[i] >= 650) {
      existCount++;
    }
  }
  if (existCount <= 14) {  //ボールあります
    exist = true;
    deg = top * 22.5;

    if (deg >= 180) {
      deg -= 360;
    }

    int offset = 0;

    offset += dist * (abs(deg) * 0.02 + 12);

    deg += 720;
    deg %= 360;

    const int error = 3;

    int topDiff = abs(top > 8 ? top - 16 : top);

    if (topDiff >= 1) {
      if (topDiff == 1)
        offset *= 0.6;

      if (topDiff == 2)
        offset *= 0.8;

      if (min(min(val[7], val[8]), val[9]) <= 300) {
        offset *= 3;
        offset = constrain(offset, 0, 85);
      }

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
      if (dist >= 3 && top > 1 && top < 15) {
        speed = 90;
        // LED.changeAll(LED.WHITE);
      } else {
        speed = 90;
      }
    }

    if (val[0] <= 250 && top == 0) {
      topTimer = device.getTime();
    }

    if (digitalRead(BALL_HOLD) && !(top > 2 && top < 14)) {
      if (device.getTime() - holdTimer >= 300) {
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
    tempDist +=
        (1 - 0.1) * (min(val[(top + 2) % 16], val[(top + 14) % 16]) - tempDist);
  } else {
    if (top != 4 && top != 12) {
      tempDist = min(val[(top + 2) % 16], val[(top + 14) % 16]);
    } else {
      tempDist = min(val[(top + 1) % 16], val[(top + 15) % 16]);
    }
  }

  dist = constrain(myMap(tempDist, 360, 530, 5, 0), 0, 5);

  // Serial.println(dist);
}

float myMap(float x, float in_min, float in_max, float out_min, float out_max) {
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}
