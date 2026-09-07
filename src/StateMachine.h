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

    enum class State
    {
        IDLE,
        RUNNING,
        WAITING_TO_REVERSE,
        STOPPING,
        FAULT
    };

    enum class AxisDirection
    {
        UP,
        DOWN
    };



    State GetState() const;
    AxisDirection GetAxisDirection() const;

    uint32_t GetCycleCount() const;
    void ResetCycleCount();


private:



    IO& io_;
    BrushMotor& brushMotor_;
    AxisMotor& verticalMotor_;

    State state_;
    AxisDirection verticalDirection_;

    unsigned long reverseStartTime_;
    static constexpr unsigned long REVERSE_DELAY_MS = 2000;

    // Used for button edge detection
    bool previousStart_;
    bool previousStop_;

    // Cycle counter
    uint32_t cycleCount_;
    bool cycleUpCompleted_;

    void HandleIdle();
    void HandleRunning();
    void HandleWaitingToReverse();
    void HandleStopping();

    void StartMachine();
    void StopMachine();

    void ReverseAxis();

    void StartReverseDelay();

    //bool StartPressedEvent();
    //bool StopPressedEvent();
};