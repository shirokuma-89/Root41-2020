void _ball::read(int* b) {
  for (int i = 0; i <= 15; i++) {
    *(b + i) += (1 - LPF) * (analogRead(BALL[i]) - *(b + i));
    if (*(b + i) <= 100) {
      *(b + i) = 1000;
    }
  }
  *(b + 3) = 1000;
  *(b + 1) = 1000;
  digitalWrite(BALL_RESET, LOW);
}

void _ball::calc(void) {
  top = 0;
  for (int i = 0; i <= 15; i++) {
    if (val[i] <= val[top]) {
      top = i;
    }
  }

  deg = top * 22.5;

  Serial.println(top);

  //ホールド処理
  if (!digitalRead(BALL_HOLD)) {
    holdTimer = device.getTime();
    kicker.val = false;
    ball.speed = 85;
  }

  if (digitalRead(BALL_HOLD)) {
    if (device.getTime() - holdTimer >= 50) {
      kicker.val = true;
    }
    ball.speed = 100;
  }
}
