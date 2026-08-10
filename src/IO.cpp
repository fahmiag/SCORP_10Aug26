//#pragma once

#include "IO.h"
#include "config/PinMap.h"

void IO::Begin()
{
    // Buttons
    pinMode(Pin::BUTTON_START, INPUT_PULLUP);
    pinMode(Pin::BUTTON_STOP,  INPUT_PULLUP);
   // pinMode(PIN_BUTTON_UP,    INPUT_PULLUP);
   // pinMode(PIN_BUTTON_DOWN,  INPUT_PULLUP);

    // Axis 1 limit switches
    pinMode(Pin::VerticalTopLimit, INPUT_PULLUP);
    pinMode(Pin::VerticalBottomLimit, INPUT_PULLUP);

    // Axis 2 limit switches
   // pinMode(PIN_AXIS2_UP_LIMIT, INPUT_PULLUP);
   // pinMode(PIN_AXIS2_DN_LIMIT, INPUT_PULLUP);

    // Read initial state
    Update();
}

void IO::Update()
{
    // Buttons
    startPressed_ = (digitalRead(Pin::BUTTON_START) == LOW);
    stopPressed_  = (digitalRead(Pin::BUTTON_STOP)  == LOW);
    //upPressed_    = (digitalRead(PIN_BUTTON_UP)    == LOW);
    //downPressed_  = (digitalRead(PIN_BUTTON_DOWN)  == LOW);

    // Vertical Axis limits
    verticalUpperLimit_ = (digitalRead(Pin::VerticalTopLimit) == LOW);
    verticalLowerLimit_ = (digitalRead(Pin::VerticalBottomLimit) == LOW);


}

// --------------------------------------------------
// Buttons
// --------------------------------------------------

bool IO::StartPressed() const
{
    return startPressed_;
}

bool IO::StopPressed() const
{
    return stopPressed_;
}

bool IO::UpPressed() const
{
    return upPressed_;
}

bool IO::DownPressed() const
{
    return downPressed_;
}

// --------------------------------------------------
// Vertical Axis limits
// --------------------------------------------------

bool IO::VerticalUpperLimit() const
{
    return verticalUpperLimit_;
}

bool IO::VerticalLowerLimit() const
{
    return verticalLowerLimit_;
}

