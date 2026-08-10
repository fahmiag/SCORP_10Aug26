#pragma once

#include <Arduino.h>

class IO
{
public:

    void Begin();
    void Update();

    //Button
    bool StartPressed() const;
    bool StopPressed() const;
    bool UpPressed() const;
    bool DownPressed() const;

    // --------------------------------------------------
    // Button events
    // Returns true once when button is pressed
    // --------------------------------------------------

    bool StartEvent();
    bool StopEvent();
    bool UpEvent();
    bool DownEvent();

    //Vertical Axis Limit
    bool VerticalUpperLimit() const;
    bool VerticalLowerLimit() const;

    //Horizontal Axis Limit
    // bool HorizontalUpperLimit() const;
    // bool HorizontalLowerLimit() const;

private:

    // Button states
    bool startPressed_ = false;
    bool stopPressed_  = false;
    bool upPressed_    = false;
    bool downPressed_  = false;

    // Button events
    bool startEvent_ = false;
    bool stopEvent_  = false;
    bool upEvent_    = false;
    bool downEvent_  = false;

    // Previous stable states
    bool previousStart_ = false;
    bool previousStop_  = false;
    bool previousUp_    = false;
    bool previousDown_  = false;

    bool verticalUpperLimit_ = false;
    bool verticalLowerLimit_ = false;

    bool vTopEvent_ = false;
    bool vBottomEvent_  = false;
    
    bool previousVTop_    = false;
    bool previousVBottom_  = false;

    // bool HorizontalUpperLimit_ = false;
    // bool HorizontalLowerLimit_ = false;

        // Button debounce
    static constexpr unsigned long DEBOUNCE_TIME_MS = 30; //TODO: Move to config.h

    bool rawStart_ = false;
    bool rawStop_  = false;
    bool rawUp_    = false;
    bool rawDown_  = false;

    
    bool rawVTop_    = false;
    bool rawVBottom_  = false;

    unsigned long startChangeTime_ = 0;
    unsigned long stopChangeTime_  = 0;
    unsigned long upChangeTime_    = 0;
    unsigned long downChangeTime_  = 0;

    
    unsigned long vTopChangeTime_    = 0;
    unsigned long vBottomChangeTime_  = 0;

    void UpdateButton(
        bool rawState,
        bool& rawStateStorage,
        bool& stableState,
        unsigned long& changeTime
    );
};