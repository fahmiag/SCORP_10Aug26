/*
This file contains only pin definitions.
*/

#pragma once
#include <Arduino.h>


namespace Pin
{
    //Vertical Motor
    constexpr uint8_t VerticalMotorPWM {10};
    constexpr uint8_t VerticalMotorDir {8};
    
    constexpr uint8_t VerticalMotorPWM2 {11};
    constexpr uint8_t VerticalMotorDir2 {9};

    constexpr uint8_t VerticalTopLimit {5};
    constexpr uint8_t VerticalBottomLimit {4}; 

    //Brush motor
    constexpr uint8_t BrushPWM {3};
    constexpr uint8_t BrushDir {2};

    //Button
    constexpr uint8_t BUTTON_START {A0};
    constexpr uint8_t BUTTON_STOP {A1};

    //Horizontal Motor
    // constexpr uint8_t HorizontalMotorPWM {8};
    // constexpr uint8_t HorizontalMotorDir {9};

    // constexpr uint8_t HorizontalTopLimit {4};
    // constexpr uint8_t HorizontalBottomLimit {5}; 


}