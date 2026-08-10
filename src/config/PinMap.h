/*
This file contains only pin definitions.
*/

#pragma once
#include <Arduino.h>


namespace Pin
{
    //Vertical Motor
    constexpr uint8_t VerticalMotorPWM {9};
    constexpr uint8_t VerticalMotorDir {8};

    constexpr uint8_t VerticalTopLimit {4};
    constexpr uint8_t VerticalBottomLimit {5}; 

    //Brush motor
    constexpr uint8_t BrushPWM {10};
    constexpr uint8_t BrushDir {11};

    //Button
    constexpr uint8_t BUTTON_START {2};
    constexpr uint8_t BUTTON_STOP {3};

    //Horizontal Motor
    // constexpr uint8_t HorizontalMotorPWM {8};
    // constexpr uint8_t HorizontalMotorDir {9};

    // constexpr uint8_t HorizontalTopLimit {4};
    // constexpr uint8_t HorizontalBottomLimit {5}; 


}