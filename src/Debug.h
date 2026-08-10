#pragma once

#define DEBUG_ENABLED 1

#if DEBUG_ENABLED

    #define DEBUG_PRINT(x) Serial.print(x)
    #define DEBUG_PRINTLN(x) Serial.println(x)

#else

    #define DEBUG_PRINT(x)
    #define DEBUG_PRINTLN(x)

#endif