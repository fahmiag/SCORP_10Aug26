#pragma once

#include <Arduino.h>

#include "IO.h"
#include "Motor/BrushMotor.h"
#include "Motor/AxisMotor.h"
#include "CycleStorage.h"

class StateMachine
{
public:
    StateMachine(
        IO& io,
        BrushMotor& brushMotor,
        AxisMotor& verticalMotor,
        CycleStorage& cycleStorage
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

    enum class FaultReason
    {
        NONE,
        AXIS_TIMEOUT,
        TOP_LIMIT_ERROR, //Temporary placeholder
        BOTTOM_LIMIT_ERROR
    };



    State GetState() const;
    AxisDirection GetAxisDirection() const;

    FaultReason GetFault() const;

    uint32_t GetCycleCount() const;
    void ResetCycleCount();

    void IncrementCycleCount();
    void SaveCycleCount();


private:



    IO& io_;
    BrushMotor& brushMotor_;
    AxisMotor& verticalMotor_;
    CycleStorage& cycleStorage_;

    State state_;
    AxisDirection verticalDirection_;
    FaultReason FaultReason_;

    unsigned long reverseStartTime_;
    static constexpr unsigned long REVERSE_DELAY_MS = 3000;

    // Used for button edge detection
    bool previousStart_;
    bool previousStop_;

    // Cycle counter
    uint32_t cycleCount_;
    bool cycleUpCompleted_;
    uint32_t lastSavedCycleCount_;

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

    static constexpr unsigned long AXIS_TIMEOUT_MS = 40000;

    unsigned long axisMoveStartTime_;

    void CheckAxisTimeout();
    void EnterFault(FaultReason reason);


    static constexpr uint32_t SAVE_INTERVAL_CYCLES = 10;


    
};