#pragma once

#include <Arduino.h>
#include <LiquidCrystal_I2C.h>
#include "../StateMachine.h"
#include "../Motor/AxisMotor.h"
#include <Wire.h>    // Required for I2C communication

class Display
{
public:
    Display(
        uint8_t address = 0x27,
        uint8_t columns = 20,
        uint8_t rows = 4
    );

    bool Begin();

    void Update(
        StateMachine::State state,
        StateMachine::AxisDirection direction,
        StateMachine::FaultReason fault,
        uint32_t cycleCount
    );

    void Clear();

    void Print(
        uint8_t column,
        uint8_t row,
        const char* text
    );

    void Print(
        uint8_t column,
        uint8_t row,
        const String& text
    );

    void PrintLine(
        uint8_t row,
        const char* text
    );

    void SetBacklight(bool enabled);

private:
    LiquidCrystal_I2C lcd_;

    uint8_t columns_;
    uint8_t rows_;
    bool initialized_;

    StateMachine::State previousState_;
    StateMachine::AxisDirection previousDirection_;
    StateMachine::FaultReason previousFault_;
    uint32_t previousCycleCount_;

    unsigned long lastUpdateTime_;

    
};