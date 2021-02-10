#include "Motor.h"
#include "ColorSensor.h"
#include "util.h"

using namespace ev3api;

class Tracer {
public:
  Tracer();
  void run();
  void init();
  void terminate();
  void manual_run(int leftPWM, int rightPWM);
  void reset();

private:
  Motor leftWheel;
  Motor rightWheel;
  ColorSensor colorSensor;
  const int8_t mThreshold = 20;
  const int8_t pwm = (Motor::PWM_MAX) / 6;

  float calc_prop_value();
};
