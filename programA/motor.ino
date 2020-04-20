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

    int minimum = 0;

    //姿勢制御
    if (_deg == NULL && _power == NULL) {
      Kp = 3.9;   //比例定数
      Ki = 0.05;  //積分定数
      Kd = 0.17;

      minimum = 50;
    } else {
      Kp = 1.9;   //比例定数
      Ki = 0.08;  //積分定数
      Kd = 0.28;  //微分定数
    }

    direction = gyro.deg;
    direction = direction > 180 ? direction - 360 : direction;
    if (abs(direction) <= 50)
      integral += direction;
    direction *= Kp * -1;                                  //比例制御
    direction -= integral * Ki;                            //積分制御
    int _direction = 0;
    // (gyro.differentialRead() * Kd) * -1;  //微分制御
    if (direction >= 0) {
      direction += _direction;
      direction = constrain(direction, 10 + minimum, 355);
    } else {
      direction += _direction;
      direction = constrain(direction, -355, -10 - minimum);
    }

    //機体が前を向いたら積分していたものをクリアする
    if (gyro.deg <= 3 || gyro.deg >= 357) {
      integral = 0;
      direction = 0;
    }

    if (_deg == NULL && _power == NULL) {
      for (int i = 0; i < 4; i++) {
        val[i] = direction;
      }

      LED.changeAll(LED.BLUE);
    } else {
      for (int i = 0; i < 4; i++) {
        val[i] = calcVal[i][_deg];
      }

      for (int i = 0; i < 4; i++) {
        if (direction >= 0) {
          val[i] *= float(255 - direction) / 255.0;
          val[i] *= float(float(_power) / 100.0);
        } else {
          val[i] *= abs(float(-255 - direction)) / 255.0;
          val[i] *= float(float(_power) / 100.0);
        }
        val[i] += direction;
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
