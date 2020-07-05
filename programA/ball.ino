void _ball::read(int* b) {
  digitalWrite(BALL_RESET, HIGH);
  for (int i = 0; i <= 15; i++) {
    // *(b + i) += (1 - LPF) * (analogRead(BALL[i]) - *(b + i));
    *(b + i) = analogRead(BALL[i]);
    if (i == 6) {
      *(b + i) = 900;
    }
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
      third = second;
      second = top;
      top = i;
    } else if (val[i] <= val[second]) {
      third = second;
      second = i;
    } else if (val[i] <= val[third]) {
      third = i;
    }

    if (val[i] >= 650) {
      existCount++;
    }
  }
  if (existCount <= 14) {  //ボールあります
    exist = true;
    deg = top * 22.5;

    float x = 0;
    float y = 0;
    x += sin(radians(top * 22.5)) * (724 - val[top]) * 2;
    x += sin(radians(second * 22.5)) * (724 - val[second]);
    x += sin(radians(third * 22.5)) * (724 - val[third]);
    y += cos(radians(top * 22.5)) * (724 - val[top]) * 2;
    y += cos(radians(second * 22.5)) * (724 - val[second]);
    y += cos(radians(third * 22.5)) * (724 - val[third]);

    deg = round(degrees(atan2(x, y)) + 360) % 360;

    top = round((float)deg / 22.5) % 16;
    readDistance();

    if (top == 8)
      deg = 180;

    if (deg >= 180) {
      deg -= 360;
    }

    int offset = 0;

    if (dist <= 2) {
      dist = 0;
    }
    offset += max(dist, 0) * abs(deg) * 0.02 + dist * 8;

    deg += 720;
    deg %= 360;

    const int error = 3;

    int topDiff = abs(top > 8 ? top - 16 : top);

    if (abs(deg) >= 0) {
      deg += 720;
      deg %= 360;
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

    deg += 720;
    deg %= 360;

    //ホールド処理
    if (!digitalRead(BALL_HOLD)) {
      // motor.referenceAngle = 0;
      // isAvoid = false;
      holdTimer = device.getTime();
      if (!isAvoid) {
        avoidTimer = device.getTime();
      }
      kicker.val = false;
      if (dist >= 3 && top > 1 && top < 15) {
        speed = 100;
        // LED.changeAll(LED.WHITE);
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
      }
      speed = 100;
      deg = 0;

      if (device.getTime() - avoidTimer >= 500 && !isAvoid) {
        int angle = 1;
        // if (motor.direction >= 0) {
        //   angle = -1;
        // }
        isAvoid = true;
        motor.referenceAngle = 35 * angle;
        // ball.deg = (80 * angle + 360) % 360;
        deg = 90;
        exist = true;
      }
    }

    if (device.getTime() - avoidTimer >= 1700) {
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
  if (false) {  // trueでローパス
    tempDist +=
        (1 - 0.1) * (min(val[(top + 2) % 16], val[(top + 14) % 16]) - tempDist);
  } else {
    if (top != 4 && top != 12 && top != 8 && top != 0) {
      tempDist = min(val[(top + 2) % 16], val[(top + 14) % 16]);
    } else {
      tempDist = min(val[(top + 1) % 16], val[(top + 15) % 16]);
    }
  }

  dist = constrain(myMap(tempDist, 400, 530, 5, 0), 0, 6);
  if (abs(8 - top) >= 6) {
    dist *= 0.9;
  }

  // Serial.println(dist);
}

float myMap(float x, float in_min, float in_max, float out_min, float out_max) {
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}
