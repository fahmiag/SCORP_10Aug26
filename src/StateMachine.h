#pragma once

#include <Arduino.h>

#include "IO.h"
#include "Motor/BrushMotor.h"
#include "Motor/AxisMotor.h"

class StateMachine
{
public:
    StateMachine(
        IO& io,
        BrushMotor& brushMotor,
        AxisMotor& verticalMotor
    );

    void Begin();
    void Update();

private:

    enum class State
    {
        IDLE,
        RUNNING,
        STOPPING
    };

    enum class AxisDirection
    {
        UP,
        DOWN
    };

    IO& io_;
    BrushMotor& brushMotor_;
    AxisMotor& verticalMotor_;

    State state_;
    AxisDirection verticalDirection_;

    // Used for button edge detection
    bool previousStart_;
    bool previousStop_;

    void HandleIdle();
    void HandleRunning();
    void HandleStopping();

    void StartMachine();
    void StopMachine();

    void ReverseAxis();

    bool StartPressedEvent();
    bool StopPressedEvent();
};