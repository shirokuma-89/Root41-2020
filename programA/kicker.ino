void _kicker::kick(bool status) {
  static bool mode = false;
  if (status != _val && status && device.getTime() - protectionTimer >= 1000) {
    mode = true;
    kickTimer = device.getTime();
    digitalWrite(SOLENOID, HIGH);
    LED.changeAll(LED.WHITE);
  } else if (device.getTime() - kickTimer <= 200) {
    mode = true;
    digitalWrite(SOLENOID, HIGH);
    LED.changeAll(LED.WHITE);
    protectionTimer = device.getTime();
  } else {
    digitalWrite(SOLENOID, LOW);
  }
  _val = status;
}
