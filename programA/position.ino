void _position::reflection(void) {
  if (rock) {
    if (side[0] == 1) {
      if (ball.top >= 13) {
        if (motor.referenceAngle == 0) {
          motor.referenceAngle = -(360 - ball.top * 22.5);
        }
      }
    } else if (side[0] == -1) {
      if (ball.top <= 3) {
        if (motor.referenceAngle == 0) {
          motor.referenceAngle = ball.top * 22.5;
        }
      }
    } else {
      motor.referenceAngle = 0;
    }
  } else {
    motor.referenceAngle = 0;
  }
}

void _position::get(void) {
  if (ball.deg >= 0 && ball.deg <= 165) {
    if (line.deg >= 210 && line.deg <= 330) {
      side[0] = 1;
      offTimer = device.getTime();
      rock = true;
    }
  }
  if (ball.deg >= 250 && ball.deg <= 360) {
    if (line.deg >= 30 && line.deg <= 150) {
      side[0] = -1;
      offTimer = device.getTime();
      rock = true;
    }
  }
  if (device.getTime() - offTimer >= 700) {
    rock = false;
    side[0] = 0;
    offTimer = 0;
  }
}