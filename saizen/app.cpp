#include "app.h"
#include "Tracer.h"
#include "Clock.h"
using namespace ev3api;

Tracer tracer;
Clock clock;

static const sensor_port_t
    touch_sensor    = EV3_PORT_1,
    color_sensor    = EV3_PORT_2,
    sonar_sensor    = EV3_PORT_3,
    gyro_sensor     = EV3_PORT_4;

void main_task(intptr_t unused) {
  tracer.init();
  ev3_sensor_config(touch_sensor, TOUCH_SENSOR);

  /* スタート待機 */
  while(1)
  {
      if (ev3_touch_sensor_is_pressed(touch_sensor) == 1)
      {
          break; /* タッチセンサが押された */
      }

      tslp_tsk(10 * 1000U); /* 10msecウェイト */
  }

  // main loop
  while(1) {
    if (ev3_button_is_pressed(BACK_BUTTON)) break;
    
    tracer.reset();

    tracer.manual_run(31, 100);
    clock.sleep(1440);
    tracer.manual_run(100, 100);
    clock.sleep(1000);
    tracer.manual_run(80, 80);
    clock.sleep(650);
    tracer.manual_run(42, 100);
    clock.sleep(2500);
    tracer.manual_run(50, 100);
    clock.sleep(700);
    tracer.manual_run(55, 55);
    clock.sleep(200);
    tracer.terminate();
    clock.sleep(5000);
    tracer.manual_run(-100, -100);
    clock.sleep(1000);
    tracer.terminate();
    clock.sleep(5000);

    tslp_tsk(4 * 1000U); /* 4msec周期起動 */
  }

  ext_tsk();
}