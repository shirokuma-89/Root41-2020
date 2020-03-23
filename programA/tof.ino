int _tof::read(void) {
  int data = TOF.readRangeContinuousMillimeters();
  dist = data > 1 ? data : dist;
  return dist;
}
