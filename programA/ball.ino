void _ball::read(int* b) {
  for (int i = 0; i <= 15; i++) {
    *(b + i) += (1 - LPF) * (analogRead(BALL[i]) - *(b + i));
    if (*(b + i) <= 120) {
      *(b + i) = 1000;
    }
  }
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
}
