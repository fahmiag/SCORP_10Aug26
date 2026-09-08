#pragma once

#define DEBUG_ENABLED 1
#define DEBUG_STATE   1
#define DEBUG_MOTOR   1
#define DEBUG_IO      1

#if DEBUG_ENABLED

    #define DEBUG_PRINT(x) Serial.print(x)
    #define DEBUG_PRINTLN(x) Serial.println(x)

#else

    #define DEBUG_PRINT(x)
    #define DEBUG_PRINTLN(x)

#endif

#if DEBUG_STATE
    #define DEBUG_STATE_PRINTLN(x) Serial.println(x)
#else
    #define DEBUG_STATE_PRINTLN(x)
#endif

#if DEBUG_MOTOR
    #define DEBUG_MOTOR_PRINTLN(x) Serial.println(x)
    #define DEBUG_MOTOR_PRINT(x) Serial.print(x)
#else
    #define DEBUG_MOTOR_PRINTLN(x)
    #define DEBUG_MOTOR_PRINT(x)
#endif

#if DEBUG_IO
    #define DEBUG_IO_PRINTLN(x) Serial.println(x)
#else
    #define DEBUG_IO_PRINTLN(x)
#endif