void _position::reflection(void) {
  if (rock) {
    if (side[0] == 1) {
      motor.referenceAngle = -20;
    } else if (side[0] == -1) {
      motor.referenceAngle = 20;
    } else {
      motor.referenceAngle = 0;
    }
  } else {
    motor.referenceAngle = 0;
  }
}

void _position::get(void) {
  if (ball.deg >= 30 && ball.deg <= 150) {
    if (line.deg >= 210 && line.deg <= 330) {
      side[0] = 1;
      offTimer = device.getTime();
      rock = true;
    }
  }
  if (ball.deg >= 210 && ball.deg <= 330) {
    if (line.deg >= 30 && line.deg <= 150) {
      side[0] = -1;
      offTimer = device.getTime();
      rock = true;
    }
  }
  if (device.getTime() - offTimer >= 1000) {
    rock = false;
    side[0] = 0;
    offTimer = 0;
  }
}