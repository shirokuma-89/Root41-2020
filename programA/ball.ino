void _ball::read(int* b) {
  digitalWrite(BALL_RESET, HIGH);
  for (int i = 0; i <= 15; i++) {
    *(b + i) += (1 - LPF) * (analogRead(BALL[i]) - *(b + i));
    if (*(b + i) <= 120) {
      *(b + i) = 1000;
    }
  }
  
  digitalWrite(BALL_RESET, LOW);
}

void _ball::calc(void) {
}
