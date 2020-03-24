void _LED::gyroShow(unsigned long _color = 'hogehoge') {
  int deg2 = gyro.deg - 11;
  deg2 += 360;
  deg2 %= 360;
  if (deg2 > 180) {
    deg2 -= 360;
  }
  deg2 *= -1;
  deg2 += 360;
  deg2 %= 360;
  int light = 0;
  for (int i = 0; i <= 15; i++) {
    if (deg2 <= (i + 1) * 22.5) {
      light = i;
      break;
    }
  }

  if (LED.white) {
    RGBLED.setPixelColor(light, 255, 255, 255);
    RGBLED.setPixelColor((light + 1) % 16, 255, 255, 255);
    RGBLED.setPixelColor((light + 2) % 16, 255, 255, 255);
    RGBLED.setPixelColor((light + 15) % 16, 255, 255, 255);
    RGBLED.setPixelColor((light + 14) % 16, 255, 255, 255);
    RGBLED.setPixelColor((light + 13) % 16, 255, 255, 255);
  } else {
    if (_color == 'hogehoge') {
      RGBLED.setPixelColor(light, LED.defaltColor);
      RGBLED.setPixelColor((light + 1) % 16, LED.defaltColor);
      RGBLED.setPixelColor((light + 2) % 16, LED.defaltColor);
      RGBLED.setPixelColor((light + 15) % 16, LED.defaltColor);
      RGBLED.setPixelColor((light + 14) % 16, LED.defaltColor);
      RGBLED.setPixelColor((light + 13) % 16, LED.defaltColor);
    } else {
      RGBLED.setPixelColor(light, _color);
      RGBLED.setPixelColor((light + 1) % 16, _color);
      RGBLED.setPixelColor((light + 2) % 16, _color);
      RGBLED.setPixelColor((light + 15) % 16, _color);
      RGBLED.setPixelColor((light + 14) % 16, _color);
      RGBLED.setPixelColor((light + 13) % 16, _color);
    }
  }
}

void _LED::degShow(int d, unsigned long _color = 'hogehoge') {
  int deg2 = d + 11;

  deg2 += 360;
  deg2 %= 360;
  int light = 0;
  for (int i = 0; i <= 15; i++) {
    if (deg2 <= (i + 1) * 22.5) {
      light = i;
      break;
    }
  }

  if (_color == 'hogehoge') {
    _color = LED.subColor;
  }

  int _d;

  if (dist == true) {
    _d = ball.dist;
  } else {
    _d = 2;
  }

  if (LED.white) {
    RGBLED.setPixelColor(light, 255, 255, 255);
    RGBLED.setPixelColor((light + 1) % 16, 255, 255, 255);
    RGBLED.setPixelColor((light + 2) % 16, 255, 255, 255);
    RGBLED.setPixelColor((light + 15) % 16, 255, 255, 255);
    RGBLED.setPixelColor((light + 14) % 16, 255, 255, 255);
    RGBLED.setPixelColor((light + 13) % 16, 255, 255, 255);
  } else {
    if (d != 1000) {
      RGBLED.setPixelColor(light, _color);
      RGBLED.setPixelColor((light + 1) % 16, _color);
      RGBLED.setPixelColor((light + 15) % 16, _color);
      RGBLED.setPixelColor((light + 14) % 16, _color);

      for (int i = 1; i <= _d; i++) {
        RGBLED.setPixelColor((light + 1 + i) % 16, _color);
        RGBLED.setPixelColor((light + 14 - i) % 16, _color);
      }

    } else {
      changeAll(255, 255, 255);
    }
  }
}

void _LED::changeAll(int red, int green, int blue) {
  if (!LED.white) {
    for (int i = 0; i <= 15; i++) {
      RGBLED.setPixelColor(i, red, green, blue);
    }
  } else {
    for (int i = 0; i <= 15; i++) {
      RGBLED.setPixelColor(i, 0, 0, 0);
    }
  }
}

void _LED::changeAll(unsigned long _color) {
  if (!LED.white) {
    for (int i = 0; i <= 15; i++) {
      RGBLED.setPixelColor(i, _color);
    }
  } else {
    for (int i = 0; i <= 15; i++) {
      RGBLED.setPixelColor(i, 0, 0, 0);
    }
  }
}
