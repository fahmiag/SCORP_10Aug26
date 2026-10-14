#include <Arduino.h>

//application settings,
//one central configuration file.
//No magic numbers elsewhere.

namespace Config
{
    constexpr uint32_t BUTTON_SCAN_MS = 20;
    constexpr uint32_t BUTTON_DEBOUNCE_MS = 50;
    constexpr uint32_t BUTTON_LONG_PRESS_MS = 1000;

    constexpr uint32_t BRUSH_MOTOR_SPEED = 255;
    constexpr uint32_t BRUSH_RAMP_RATE = 25;
    constexpr uint32_t BRUSH_UPDATE_RATE = 20;

    //5inch Blue wheel
    //Speed 30 without load is ok.
    //Speed 50 is just nice
    //Speed 100 is too fast

    //Belt
    //Speed 50 is too slow
    //Speed 100
    constexpr uint32_t VERTICAL_MOTOR_MAX_SPEED = 255;
    constexpr uint32_t VERTICAL_MOTOR_UP_SPEED = 255;
    constexpr uint32_t VERTICAL_MOTOR_DW_SPEED = 255;
    constexpr uint32_t VERTICAL_RAMP_RATE = 25;
    constexpr uint32_t VERTICAL_UPDATE_RATE = 10;


    constexpr unsigned long REVERSE_DELAY_MS = 3000;
    constexpr unsigned long AXIS_TIMEOUT_MS = 120000;


    constexpr uint32_t SAVE_INTERVAL_CYCLES = 10;  //TODO: increase this.

    constexpr unsigned long UPDATE_INTERVAL_MS = 2000;  //WHERE do this used?

    constexpr uint8_t PUMP_MOTOR_SPEED = 100;
    constexpr uint8_t PUMP_RAMP_RATE = 10;
    constexpr unsigned long PUMP_UPDATE_RATE = 20;
}
