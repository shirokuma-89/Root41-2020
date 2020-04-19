void _position::reflection(int _type) {
  if (_type == 0) {
    reliability = 0;
  } else if (_type == 1) {
    // tof
    if (tof._dist >= 300) {
      reliability += 3;
      vertical[1] = 1;
    }
  } else if (_type == 2) {
    // line
  } else if (_type == 3) {
    // ball
  } else if (_type == 4) {
    // kick
  }
}

void _position::get(void) {
  if (reliability >= 3) {
  } else {
    rock = false;
    vertical[0] = 0;
    side[0] = 0;
  }
}