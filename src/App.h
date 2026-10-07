#pragma once

#include "IO.h"
#include "Motor/AxisMotor.h"
#include "Motor/BrushMotor.h"
#include "Motor/PumpMotor.h"
#include "StateMachine.h"
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

    PumpMotor pumpMotor_;

    CycleStorage cycleStorage_;

    StateMachine stateMachine_;

    Display display_;
};