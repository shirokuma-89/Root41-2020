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

    deg += 720;
    deg %= 360;

    //ホールド処理
    if (!digitalRead(BALL_HOLD)) {
      holdTimer = device.getTime();
      kicker.val = false;
    }

    if (digitalRead(BALL_HOLD) && !(top > 3 && top < 13)) {
      if (device.getTime() - holdTimer >= 200) {
        kicker.val = true;
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
