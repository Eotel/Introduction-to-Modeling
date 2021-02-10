#include "Tracer.h"

Tracer::Tracer():
  leftWheel(PORT_C), rightWheel(PORT_B),
  colorSensor(PORT_3) {
}

void Tracer::init() {
  init_f("Tracer");
}

void Tracer::reset() {
  leftWheel.reset();
  rightWheel.reset();
}

void Tracer::terminate() {
  msg_f("Stopped.", 1);
  leftWheel.stop();
  rightWheel.stop();
}

float Tracer::calc_prop_value() {
  const float Kp = 0.83;
  const int target = 10;
  const int bias = 0;

  int diff = colorSensor.getBrightness() - target;
  return (Kp * diff + bias);
}

void Tracer::run() {
  msg_f("running...", 1);
  float turn = calc_prop_value();
  int pwm_l = pwm - turn;
  int pwm_r = pwm + turn;
  leftWheel.setPWM(pwm_l);
  rightWheel.setPWM(pwm_r);
}

void Tracer::manual_run(int leftPWM, int rightPWM) {
  msg_f("rounding...", 1);
  leftWheel.setPWM(leftPWM);
  rightWheel.setPWM(rightPWM);
}