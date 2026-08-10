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

    //Vertical Axis Limit
    bool VerticalUpperLimit() const;
    bool VerticalLowerLimit() const;

    //Horizontal Axis Limit
    // bool HorizontalUpperLimit() const;
    // bool HorizontalLowerLimit() const;

private:

    bool startPressed_ = false;
    bool stopPressed_  = false;
    bool upPressed_    = false;
    bool downPressed_  = false;

    bool verticalUpperLimit_ = false;
    bool verticalLowerLimit_ = false;

    // bool HorizontalUpperLimit_ = false;
    // bool HorizontalLowerLimit_ = false;
};