#include "app.h"
#include "Tracer.h"
#include "Clock.h"
using namespace ev3api;

#if defined(MAKE_BT_DISABLE)
    static const int _bt_enabled = 0;
#else
    static const int _bt_enabled = 1;
#endif

/**
 * シミュレータかどうかの定数を定義します
 */
#if defined(MAKE_SIM)
    static const int _SIM = 1;
#elif defined(MAKE_EV3)
    static const int _SIM = 0;
#else
    static const int _SIM = 0;
#endif

Tracer tracer;
Clock clock;

bool isDoneSection = false;
bool isInitializedClock = false;

uint32_t currentTime = 0;
uint32_t lastTime = 0;
int stateFlag = 0;
/*
stateFlag :
0 : initial state 起動待ち状態
1 : 左旋回
2 : 前進
3 : 左旋回
4 : 前進
5 : 停止
6 : 
*/

static const sensor_port_t
    touch_sensor    = EV3_PORT_1,
    color_sensor    = EV3_PORT_2,
    sonar_sensor    = EV3_PORT_3,
    gyro_sensor     = EV3_PORT_4;

static int      bt_cmd = 0;     /* Bluetoothコマンド 1:リモートスタート */
static FILE     *bt = NULL;     /* Bluetoothファイルハンドル */

#define CMD_START         '1'    /* リモートスタートコマンド */

static int DULATION_TIMES[] = {
  20000,  // 起動待ち時間
  3000,
  1500,
  1500,
  1500
};

bool CheckTime(uint32_t now, uint32_t last, uint32_t dulation);
void InitializeClock();

void tracer_task(intptr_t exinf) {
  tracer.run();
  ext_tsk();
}

void main_task(intptr_t unused) {
  int dulation = 0;
  printf("main task is running.");
  tracer.init();

  ev3_sensor_config(touch_sensor, TOUCH_SENSOR);

  if (_bt_enabled)
  {
      /* Open Bluetooth file */
      bt = ev3_serial_open_file(EV3_SERIAL_BT);
      assert(bt != NULL);

      /* Bluetooth通信タスクの起動 */
      act_tsk(BT_TASK);
  }

  if (_bt_enabled)
  {
      fprintf(bt, "Bluetooth Remote Start: Ready.\n", EV3_SERIAL_BT);
      fprintf(bt, "send '1' to start\n", EV3_SERIAL_BT);
  }

  /* スタート待機 */
  while(1)
  {
      //tail_control(TAIL_ANGLE_STAND_UP); /* 完全停止用角度に制御 */

      if (bt_cmd == 1)
      {
          break; /* リモートスタート */
      }

      if (ev3_touch_sensor_is_pressed(touch_sensor) == 1)
      {
          break; /* タッチセンサが押された */
      }

      tslp_tsk(10 * 1000U); /* 10msecウェイト */
  }

  tracer.reset();
  // main loop
  while(1) {
    if (ev3_button_is_pressed(BACK_BUTTON)) break;

    // currentTime = clock.now();
    // dulation = DULATION_TIMES[stateFlag];

    // printf("currentTime : %d\n", (int)currentTime);
    // printf("lastTime    : %d\n", (int)lastTime);
    // printf("stateFlag   : %d\n", stateFlag);
    
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

    // switch (stateFlag){
    //   case 0:
    //     clock.wait(dulation);
    //     stateFlag++;
    //     break;

    //   case 1:
    //     tracer.manual_run(30, 100);
    //     clock.wait(dulation);
    //     stateFlag++;
    //     break;

    //   case 2:
    //     tracer.manual_run(100, 100);
    //     clock.wait(dulation);
    //     stateFlag++;
    //     break;

    //   case 3:
    //     tracer.terminate();
    //     break;

    //   case 4:
    //     break;

    //   case 5:
    //     break;
    // }

    tslp_tsk(4 * 1000U); /* 4msec周期起動 */
  }
  
  if (_bt_enabled)
  {
      ter_tsk(BT_TASK);
      fclose(bt);
  }

  isDoneSection = false;
  ext_tsk();
}

bool CheckTime(uint32_t now, uint32_t last, uint32_t dulation) {
  if((now - last) >= dulation) {
    return true;
  } else {
    return false;
  }
}

void InitializeClock() {
  clock.reset();
  int now;
  now = clock.now();
  currentTime = now;
  lastTime = now;
  isInitializedClock = true;
  printf("Clock has initialized.");
}

void bt_task(intptr_t unused)
{
    while(1)
    {
        if (_bt_enabled)
        {
            uint8_t c = fgetc(bt); /* 受信 */
            switch(c)
            {
            case '1':
                bt_cmd = 1;
                break;
            default:
                break;
            }
            fputc(c, bt); /* エコーバック */
        }
    }
}