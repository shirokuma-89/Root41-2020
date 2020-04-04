_motor::_motor(void) {
  pinMode(4, OUTPUT);
  pinMode(5, OUTPUT);

  pinMode(6, OUTPUT);
  pinMode(7, OUTPUT);

  pinMode(8, OUTPUT);
  pinMode(9, OUTPUT);

  pinMode(10, OUTPUT);
  pinMode(11, OUTPUT);
}

void _motor::drive(int _deg, int _power, bool _stop = false) {
  //ハードウェアが完成してから実装します
  for (int i = 0; i <= 3; i++) {
    val[i] = 0;
  }

  directDrive(val);
}

void _motor::directDrive(int* p) {
  //ハードウェアが完成してから実装します
  for (int i = 0; i <= 3; i++) {
    if (*(p + i) == 0) {
      digitalWrite(4 + i * 2, HIGH);
      digitalWrite(5 + i * 2, HIGH);
    } else if (*(p + i) > 0) {
      analogWrite(4 + i * 2, *(p + i));
      digitalWrite(5 + i * 2, LOW);
    } else {
      digitalWrite(4 + i * 2, LOW);
      analogWrite(5 + i * 2, abs(*(p + i)));
    }
  }
}
