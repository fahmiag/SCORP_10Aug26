#include <Arduino.h>

//application settings,
//one central configuration file.
//No magic numbers elsewhere.

namespace Config
{
    constexpr uint32_t BUTTON_SCAN_MS = 20;
    constexpr uint32_t BUTTON_DEBOUNCE_MS = 50;
    constexpr uint32_t BUTTON_LONG_PRESS_MS = 1000;

    constexpr uint32_t BRUSH_MOTOR_SPEED = 200;
    constexpr uint32_t BRUSH_RAMP_RATE = 50;
    constexpr uint32_t BRUSH_UPDATE_RATE = 20;

    constexpr uint32_t VERTICAL_MOTOR_MAX_SPEED = 200;
    constexpr uint32_t VERTICAL_RAMP_RATE = 50;
    constexpr uint32_t VERTICAL_UPDATE_RATE = 10;

    constexpr unsigned long REVERSE_DELAY_MS = 3000;
    constexpr unsigned long AXIS_TIMEOUT_MS = 75000;
    constexpr uint32_t SAVE_INTERVAL_CYCLES = 10;

    constexpr unsigned long UPDATE_INTERVAL_MS = 2000;
}