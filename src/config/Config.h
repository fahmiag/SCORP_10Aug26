#include <Arduino.h>

//application settings,
//one central configuration file.
//No magic numbers elsewhere.

namespace Config
{
    constexpr uint32_t BUTTON_SCAN_MS = 20;
    constexpr uint32_t BUTTON_DEBOUNCE_MS = 50;
    constexpr uint32_t BUTTON_LONG_PRESS_MS = 1000;

    constexpr uint32_t BRUSH_MOTOR_SPEED = 130; //150
    constexpr uint32_t BRUSH_RAMP_RATE = 25;
    constexpr uint32_t BRUSH_UPDATE_RATE = 20;

    //5inch Blue wheel
    //Speed 30 without load is ok.
    //Speed 50 is just nice
    //Speed 100 is too fast

    //Belt
    //Speed 50 is too slow
    //Speed 150 is just nice
    constexpr uint32_t VERTICAL_MOTOR_UP_SPEED = 55;
    constexpr uint32_t VERTICAL_MOTOR_DW_SPEED = 50;
    constexpr uint8_t VERTICAL_MOTOR_REDUCED_SPEED = 40;
    constexpr unsigned long VERTICAL_SPEED_REDUCE_AFTER_MS = 12000;
    constexpr uint32_t VERTICAL_RAMP_RATE = 10;
    constexpr uint32_t VERTICAL_UPDATE_RATE = 10;

    constexpr unsigned long REVERSE_DELAY_MS = 1000;
    constexpr unsigned long AXIS_TIMEOUT_MS = 30000;

    constexpr uint32_t SAVE_INTERVAL_CYCLES = 50;  


    constexpr uint8_t PUMP_MOTOR_SPEED = 100;
    constexpr uint8_t PUMP_RAMP_RATE = 10;
    constexpr unsigned long PUMP_UPDATE_RATE = 20;
}
