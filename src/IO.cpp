//#pragma once

#include "IO.h"
#include "config/PinMap.h"
#include "config/Debug.h"

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

        // Initialize timing
    unsigned long now = millis();

    startChangeTime_ = now;
    stopChangeTime_  = now;
    upChangeTime_    = now;
    downChangeTime_  = now;

    vTopChangeTime_    = now;
    vBottomChangeTime_  = now;

    // Initialize button states
    rawStart_ = (digitalRead(Pin::BUTTON_START) == LOW);
    rawStop_  = (digitalRead(Pin::BUTTON_STOP)  == LOW);
   // rawUp_    = (digitalRead(Pin::BUTTON_UP)    == LOW);
   // rawDown_  = (digitalRead(Pin::BUTTON_DOWN)  == LOW);
   
    rawVTop_ = (digitalRead(Pin::VerticalTopLimit) == LOW);
    rawVBottom_  = (digitalRead(Pin::VerticalBottomLimit)  == LOW);

    startPressed_ = rawStart_;
    stopPressed_  = rawStop_;
    //upPressed_    = rawUp_;
   // downPressed_  = rawDown_;

    previousStart_ = startPressed_;
    previousStop_  = stopPressed_;
   // previousUp_    = upPressed_;
   // previousDown_  = downPressed_;
   previousVTop_  = verticalUpperLimit_;
   previousVBottom_  = verticalLowerLimit_;

    // No events generated during startup
    startEvent_ = false;
    stopEvent_  = false;
   // upEvent_    = false;
   // downEvent_  = false;
    vTopEvent_ = false;
    vBottomEvent_  = false;


    Update();
}

void IO::Update()
{
    unsigned long now = millis();

    // Events are generated during this Update()
    // and consumed by StartEvent(), StopEvent(), etc.
    startEvent_ = false;
    stopEvent_  = false;
    upEvent_    = false;
    downEvent_  = false;
    vTopEvent_ = false;
    vBottomEvent_  = false;
    

      // --------------------------------------------------
    // Read raw button states
    // INPUT_PULLUP:
    // LOW  = pressed
    // HIGH = released
    // --------------------------------------------------

    bool rawStart= (digitalRead(Pin::BUTTON_START) == LOW);
    bool rawStop  = (digitalRead(Pin::BUTTON_STOP)  == LOW);
    //upPressed_    = (digitalRead(PIN_BUTTON_UP)    == LOW);
    //downPressed_  = (digitalRead(PIN_BUTTON_DOWN)  == LOW);

    // Vertical Axis limits
    bool rawVTop    = (digitalRead(Pin::VerticalTopLimit) == LOW);
    bool rawVBottom  = (digitalRead(Pin::VerticalBottomLimit) == LOW);


    // if (startPressed_)
    //     Serial.println("START pressed");
    // if (stopPressed_)
    //     Serial.println("STOP pressed");
    // if (verticalUpperLimit_)
    //     Serial.println("UPPER Limit pressed");
    //  if (verticalLowerLimit_)
    //     Serial.println("BOTTOM Limit pressed");


    // --------------------------------------------------
    // Debounce buttons
    // --------------------------------------------------

    UpdateButton(
        rawStart,
        rawStart_,
        startPressed_,
        startChangeTime_
    );

    UpdateButton(
        rawStop,
        rawStop_,
        stopPressed_,
        stopChangeTime_
    );

    UpdateButton(
        rawVTop,
        rawVTop_,
        verticalUpperLimit_,
        stopChangeTime_
    );

    UpdateButton(
        rawVBottom,
        rawVBottom_,
        verticalLowerLimit_,
        stopChangeTime_
    );

        // --------------------------------------------------
    // Generate press events
    //
    // Event occurs only on:
    //
    // false -> true
    // --------------------------------------------------

    if (startPressed_ && !previousStart_)
    {
        startEvent_ = true;
        DEBUG_PRINTLN("[BUTTON] START");
    }

    if (stopPressed_ && !previousStop_)
    {
        stopEvent_ = true;
        DEBUG_PRINTLN("[BUTTON] STOP");
    }

    if (verticalUpperLimit_ && !previousVTop_)
    {
        vTopEvent_ = true;
        DEBUG_PRINTLN("[SW] Top Limit");
    }

    if (verticalLowerLimit_ && !previousVBottom_)
    {
        vBottomEvent_ = true;
        DEBUG_PRINTLN("[SW] Bottom Limit");
    }

        // --------------------------------------------------
    // Save current state for next update
    // --------------------------------------------------

    previousStart_ = startPressed_;
    previousStop_  = stopPressed_;

    previousVTop_ = verticalUpperLimit_;
    previousVBottom_ = verticalLowerLimit_;


}

// ======================================================
// Button debounce
// ======================================================

void IO::UpdateButton(
    bool rawState,
    bool& rawStateStorage,
    bool& stableState,
    unsigned long& changeTime)
{
    unsigned long now = millis();

    // Raw input changed
    if (rawState != rawStateStorage)
    {
        rawStateStorage = rawState;
        changeTime = now;
    }

    // Raw input has remained unchanged long enough
    if ((now - changeTime) >= DEBOUNCE_TIME_MS)
    {
        stableState = rawStateStorage;
    }
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
// ======================================================
// Button events
// ======================================================

bool IO::StartEvent()
{
    bool event = startEvent_;
    startEvent_ = false;

    return event;
}

bool IO::StopEvent()
{
    bool event = stopEvent_;
    stopEvent_ = false;

    return event;
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

