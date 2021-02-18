#include "Motor.h"
#include "util.h"

using namespace ev3api;

class Tracer {
public:
  Tracer();
  void init();
  void terminate();
  void manual_run(int leftPWM, int rightPWM);
  void reset();

private:
  Motor leftWheel;
  Motor rightWheel;
};
