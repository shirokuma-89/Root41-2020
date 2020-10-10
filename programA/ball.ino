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
  dist = 0;
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
      dist += 690;
    } else {
      dist += val[i];
    }
    // dist += val[i];
  }

  dist /= 16;  // - existCount;

  // Serial.println(dist);
  if (existCount <= 14) {  //ボールあります
    exist = true;
    deg = top * 22.5;
    offset = 180 - abs(180 - deg);

    offset = offset * 0.2;  // + dist * 3;
    if (dist <= 550) {
      offset *= 1.7;
      offset += 40;
    } else if (dist <= 590) {
      // offset *= 1.2;
      offset += 25;
    }
    offset = constrain(offset, 0, 100);
    Serial.print(offset);
    Serial.print("\t");
    Serial.println(dist);

    if (top >= 2 && top <= 14) {
      if (top <= 8) {
        deg += offset;
      } else {
        deg -= offset;
      }
    }

    //ホールド処理
    if (!digitalRead(BALL_HOLD)) {
      holdTimer = device.getTime();
      kicker.val = false;
    }

    if (digitalRead(BALL_HOLD) && !(top > 3 && top < 13)) {
      if (device.getTime() - holdTimer >= 200) {
        kicker.val = true;
        if (motor.referenceAngle != 0) {
          if (device.getTime() - holdTimer < 300) {
            kicker.val = false;
          }
        }
        if (device.getTime() - speedTimer >= 600 || speedTimer == 0) {
          speedTimer = device.getTime();
        }
      }
    }

    LED.dist = true;

    speed = 100;

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
