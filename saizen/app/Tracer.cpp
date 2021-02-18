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

void Tracer::manual_run(int leftPWM, int rightPWM) {
  msg_f("rounding...", 1);
  leftWheel.setPWM(leftPWM);
  rightWheel.setPWM(rightPWM);
}