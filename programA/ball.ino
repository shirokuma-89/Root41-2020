void _ball::read(int* b) {
  digitalWrite(BALL_RESET, HIGH);
  for (int i = 0; i <= 15; i++) {
    // *(b + i) += (1 - LPF) * (analogRead(BALL[i]) - *(b + i));
    *(b + i) = analogRead(BALL[i]);
    // if (i == 6) {
    //   *(b + i) = 900;
    // }
    // Serial.print(*(b + i));
    // Serial.print(" ");
  }
  // Serial.println(" ");
  digitalWrite(BALL_RESET, LOW);

  *b *= 0.98;
}

void _ball::calc(void) {
  int existCount = 0;
  top = 0;
  for (int i = 0; i <= 15; i++) {
    if (val[i] <= val[top]) {
      top = i;
    }
    if (val[i] >= 560) {
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

    offset += abs(deg) * 0.03 * dist + dist * 7;

    deg += 720;
    deg %= 360;

    const int error = 3;

    int topDiff = abs(top > 8 ? top - 16 : top);

    if (topDiff >= 1) {
      if (topDiff == 1)
        offset *= 0.95;

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
      if (!isAvoid) {
        avoidTimer = device.getTime();
      }
      kicker.val = false;
      if (dist >= 3 && top > 1 && top < 15) {
        speed = 100;
      } else {
        speed = 100;
      }
    }

    if (val[0] <= 250 && top == 0) {
      topTimer = device.getTime();
    }

    if (digitalRead(BALL_HOLD) && !(top > 3 && top < 13)) {
      if (device.getTime() - holdTimer >= 200) {
        kicker.val = true;
        if (device.getTime() - speedTimer >= 600 || speedTimer == 0) {
          speedTimer = device.getTime();
        }
      }
      speed = 100;
      deg = 0;

      if (device.getTime() - avoidTimer >= 500 && !isAvoid) {
        int angle = 1;
        // if (motor.direction >= 0) {
        //   angle = -1;
        // }
        isAvoid = true;
        // motor.referenceAngle = 45 * angle;
        // ball.deg = (80 * angle + 360) % 360;
        deg = 90;
        exist = true;
      }
    }

    if (device.getTime() - avoidTimer >= 1500) {
      motor.referenceAngle = 0;
      isAvoid = false;
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
    if (top != 4 && top != 12) {
      val[4] *= 0.98;
      val[12] *= 0.98;
      tempDist += (1 - 0.2) *
                  (min(val[(top + 2) % 16], val[(top + 14) % 16]) - tempDist);
    } else {
      tempDist += (1 - 0.2) *
                  (min(val[(top + 1) % 16], val[(top + 15) % 16]) - tempDist);
    }
  } else {
    if (top != 4 && top != 12) {
      tempDist = min(val[(top + 2) % 16], val[(top + 14) % 16]);
    } else {
      tempDist = min(val[(top + 1) % 16], val[(top + 15) % 16]);
    }
  }

  dist = constrain(myMap(tempDist, 360, 580, 5, 0), 1, 5);

  if (dist <= 2 && (top <= 6 || top >= 10)) {  //切り捨て
    dist = 1;
  }

  // Serial.println(dist);
}

float myMap(float x, float in_min, float in_max, float out_min, float out_max) {
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}
