#pragma once

#include "Motor/AxisMotor.h"
#include "Motor/BrushMotor.h"
#include "IO.h"
#include "StateMachine.h"
#include "config/PinMap.h"
#include <Wire.h>               // Required for I2C communication
#include "Display/Display.h"

class App
{
public:
    App();

    void Begin();
    void Update();

private:
    IO io_;

    BrushMotor brushMotor_;

    AxisMotor verticalMotor_;

    StateMachine stateMachine_;

    Display display_;
};