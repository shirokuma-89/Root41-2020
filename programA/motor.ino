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

  if (!_stop) {
    gyro.deg = gyro.read();

    //姿勢制御
    Kp = 3.1;    //比例定数
    Ki = 0.65;   //積分定数
    Kd = -0.45;  //微分定数

    front = gyro.deg;
    front = front > 180 ? front - 360 : front;
    if (abs(front) <= 50)
      integral += front;
    front *= Kp * -1;                             //比例制御
    front -= integral * Ki;                       //積分制御
    int _front = (gyro.differentialRead() * Kd);  //微分制御
    if (front >= 0) {
      front += _front;
      front = constrain(front, 10, 255);
    } else {
      front += _front;
      front = constrain(front, -255, -10);
    }

    //機体が前を向いたら積分していたものをクリアする
    if (gyro.deg <= 5 || gyro.deg >= 355) {
      integral = 0;
      front = 0;
    }

    if (_deg == NULL && _power == NULL) {
      for (int i = 0; i < 4; i++) {
        val[i] = front;
      }
    } else {
      val[0] = 255;
      val[1] = -255;
      val[2] = 255;
      val[3] = -255;

      int valTemp[4];
      memcpy(valTemp, val, sizeof(val));
      for (i = 0; i < total; ++i) {
        for (j = i + 1; j < total; ++j) {
          if (valTemp[i] > valTemp[j]) {
            int temp = valTemp[i];
            valTemp[i] = valTemp[j];
            valTemp[j] = temp;
          }
        }
      }

      for (int i = 0; i < 4; i++) {
        val[i] *= (valTemp[3] - front) / 255;
        val[i] += front;
      }
    }
  }

  for (int i = 0; i <= 3; i++) {
    val[i] = constrain(val[i], -255, 255);
  }

  directDrive(val);
}

void _motor::directDrive(int* p) {
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
