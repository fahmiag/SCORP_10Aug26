#pragma once

#include <Arduino.h>
#include <LiquidCrystal_I2C.h>
#include "../StateMachine.h"

class Display
{
public:
    Display(
        uint8_t address = 0x27,
        uint8_t columns = 20,
        uint8_t rows = 4
    );

    bool Begin();

    void Update(StateMachine::State state);

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
};