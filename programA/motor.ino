_motor::_motor(void) {
  //ハードウェアが完成してから実装します
}

void _motor::drive(int _deg, int _power, bool _stop = false) {
  //ハードウェアが完成してから実装します
  for (int i = 0; i < 3; i++) {
    val[i] = 0;
  }

  directDrive(val);
}

void _motor::directDrive(int* p) {
  //ハードウェアが完成してから実装します
}
