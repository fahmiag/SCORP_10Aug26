#include "StateMachine.h"
#include "config/Debug.h"
#include "config/Config.h"


StateMachine::StateMachine(
    IO& io,
    BrushMotor& brushMotor,
    AxisMotor& axisMotor1,
    PumpMotor& pumpMotor,
    CycleStorage& cycleStorage
    )
    : io_(io),
      brushMotor_(brushMotor),
      verticalMotor_(axisMotor1),
      pumpMotor_(pumpMotor),
      cycleStorage_(cycleStorage),
      state_(State::IDLE),
      verticalDirection_(AxisDirection::UP),
      FaultReason_(FaultReason::NONE),
      reverseStartTime_(0),
      cycleCount_(0),
      cycleUpCompleted_(false),
      lastSavedCycleCount_(0),
      axisMoveStartTime_(0)
{
}

void StateMachine::Begin()
{
    state_ = State::IDLE;

    // First movement direction
    verticalDirection_ = AxisDirection::UP;
    // TODO: Add homing function. or manual control

    reverseStartTime_ = 0;

    cycleCount_ = 0;
    cycleUpCompleted_ = false;

    brushMotor_.Stop();
    pumpMotor_.Stop();
    verticalMotor_.Stop();

    cycleCount_ = cycleStorage_.LoadCount();
    lastSavedCycleCount_ = cycleCount_;

    DEBUG_PRINT("[CYCLE] Loaded counter = ");
    DEBUG_PRINTLN(cycleCount_);

    DEBUG_PRINTLN("[STATE] IDLE");
    DEBUG_PRINTLN("[CYCLE] Loaded Counter = 0");

    // E-stop already pressed at startup:
    // enter the latched state before normal operation begins.
    if (io_.EmergencyStopPressed())
    {
        EnterEmergencyStop();
    }

    
}

void StateMachine::Update()
{
    // E-stop takes priority over buttons, limits and timeouts.
    if (io_.EmergencyStopPressed() &&
        state_ != State::ESTOP)
    {
        EnterEmergencyStop();
    }

    // No normal state processing while latched.
    if (state_ == State::ESTOP)
    {
        return;
    }

    CheckAxisTimeout();

    switch (state_)
    {
        case State::IDLE:
            HandleIdle();
            break;

        case State::RUNNING:
            HandleRunning();
            break;

        case State::WAITING_TO_REVERSE:
            HandleWaitingToReverse();
            break;

        case State::STOPPING:
            HandleStopping();
            break;

        case State::FAULT:
            // Motors remain stopped
            // For Now only Reboot can remove state FAULT
            break;

        case State::ESTOP:
            break;
    }
}


// ======================================================
// IDLE
// ======================================================

void StateMachine::HandleIdle()
{
    if (io_.StartEvent())
    {
        StartMachine();

  
    }
}


// ======================================================
// RUNNING
// ======================================================

void StateMachine::HandleRunning()
{
    // ----------------------------------------------
    // STOP button
    // ----------------------------------------------

    if (io_.StopEvent())
    {
        StopMachine();
        return;
    }

    // ----------------------------------------------
    // Check axis limits
    // ----------------------------------------------

    if (verticalDirection_ == AxisDirection::UP)
    {
        if (verticalMotor_.IsAtUpperLimit())
        {
            cycleUpCompleted_ = true;
            // Ramp motor down to zero first
            DEBUG_PRINTLN("[AXIS1] TOP LIMIT");
            //verticalMotor_.Stop();
            StartReverseDelay();

            // We will reverse once the motor has stopped
        }
    }
    else
    {
        if (verticalMotor_.IsAtLowerLimit())
        {       
            if (cycleUpCompleted_)
            {
                IncrementCycleCount();

                cycleUpCompleted_ = false;

                DEBUG_PRINT("[CYCLE] Count = ");
                DEBUG_PRINTLN(cycleCount_);
            }
            // Ramp motor down to zero first
            //verticalMotor_.Stop();
            DEBUG_PRINTLN("[AXIS1] TOP LIMIT");
            StartReverseDelay();

            // We will reverse once the motor has stopped
        }
    }

    // ----------------------------------------------
    // Once the motor has slowed to zero,
    // reverse direction
    // ----------------------------------------------

    if (!verticalMotor_.IsMoving())
    {
        if (verticalDirection_ == AxisDirection::UP)
        {
            // We have reached the top
            if (verticalMotor_.IsAtUpperLimit())
            {
                ReverseAxis();
            }
        }
        else
        {
            // We have reached the bottom
            if (verticalMotor_.IsAtLowerLimit())
            {
                ReverseAxis();
            }
        }
    }
}

void StateMachine::StartReverseDelay()
{
    // Ramp axis down to zero
    verticalMotor_.Stop();

    // Record when the delay started
    reverseStartTime_ = millis();

    state_ = State::WAITING_TO_REVERSE;

    DEBUG_PRINTLN("[AXIS1] Stopping before reverse");
    DEBUG_PRINTLN("[STATE] WAITING_TO_REVERSE");
}

void StateMachine::HandleWaitingToReverse()
{
    // STOP should still work during the delay
    if (io_.StopEvent())
    {
        StopMachine();
        return;
    }

    unsigned long elapsed =
        millis() - reverseStartTime_;

    if (elapsed >= Config::REVERSE_DELAY_MS)
    {
        ReverseAxis();
    }


}

void StateMachine::ReverseAxis()
{
    if (verticalDirection_ == AxisDirection::UP)
    {
        verticalDirection_ = AxisDirection::DOWN;

        DEBUG_PRINTLN("[AXIS1] Reversing DOWN");

        verticalMotor_.MoveDown();
    }
    else
    {
        verticalDirection_ = AxisDirection::UP;

        DEBUG_PRINTLN("[AXIS1] Reversing UP");

        verticalMotor_.MoveUp();
    }

    verticalMotor_.SetSpeed(180);

    state_ = State::RUNNING;

    DEBUG_PRINTLN("[STATE] RUNNING");

    // New axis movement starts here.
    // Restart movement timeout.
    axisMoveStartTime_ = millis();
}

// ======================================================
// STOPPING
// ======================================================

void StateMachine::HandleStopping()
{
    // Motors are already commanded to stop.
    // Wait until they have completely ramped down.

    if (!verticalMotor_.IsMoving() &&
        !brushMotor_.IsRunning())
    {
        DEBUG_PRINTLN("[STATE] IDLE");
        state_ = State::IDLE;

        SaveCycleCount();

        
    }
}


// ======================================================
// START MACHINE
// ======================================================

void StateMachine::StartMachine()
{
    state_ = State::RUNNING;

    Serial.println("[STATE] START");

    // --------------------------------------------------
    // Start brush
    // --------------------------------------------------

    brushMotor_.Forward();
    brushMotor_.SetSpeed(200);

    // --------------------------------------------------
    // Start Pump
    // --------------------------------------------------
    pumpMotor_.SetSpeed(200);    // Remember selected speed
    pumpMotor_.Start();          // Ramp up to selected speed

    // --------------------------------------------------
    // Start axis in remembered direction
    // --------------------------------------------------

    if (verticalDirection_ == AxisDirection::UP)
    {
        verticalMotor_.MoveUp();
        Serial.println("Move UP");
    }
    else
    {
        verticalMotor_.MoveDown();
        Serial.println("Move Down");
    }

    verticalMotor_.SetSpeed(180);

    axisMoveStartTime_ = millis();
}


// ======================================================
// STOP MACHINE
// ======================================================

void StateMachine::StopMachine()
{
    state_ = State::STOPPING;
    
    Serial.println("[STATE] STOP");

    // Controlled ramp down
    verticalMotor_.Stop();
    brushMotor_.Stop();
    pumpMotor_.Stop();

    axisMoveStartTime_ = 0;

    // IMPORTANT:
    // axisDirection_ is NOT changed.
    //
    // Therefore if the machine was moving UP,
    // pressing START again will resume UP.
}

uint32_t StateMachine::GetCycleCount() const
{
    return cycleCount_;
}

void StateMachine::ResetCycleCount()
{
    cycleCount_ = 0;
    lastSavedCycleCount_ = 0;

    cycleStorage_.SaveCount(0);

    DEBUG_PRINTLN("[CYCLE] Counter reset to 0");
}

StateMachine::State StateMachine::GetState() const
{
    return state_;
}

StateMachine::AxisDirection StateMachine::GetAxisDirection() const
{
    return verticalDirection_;
}

StateMachine::FaultReason  StateMachine::GetFault() const
{
    return FaultReason_;
;
}

void StateMachine::CheckAxisTimeout()
{
    if (state_ != State::RUNNING)
        return;

    if (axisMoveStartTime_ == 0)
        return;

    const unsigned long elapsed =
        millis() - axisMoveStartTime_;

    if (elapsed >= Config::AXIS_TIMEOUT_MS)
    {
        Serial.println("[FAULT] Axis movement timeout");
        cycleStorage_.SaveCount(cycleCount_);
        EnterFault(FaultReason::AXIS_TIMEOUT);

        
    }
}

void StateMachine::EnterFault(FaultReason reason)
{
    verticalMotor_.Stop();
    brushMotor_.Stop();
    pumpMotor_.Stop();

    state_ = State::FAULT;
    //FaultReason_ = StateMachine::GetFault();
    FaultReason_ = reason;

    Serial.println("[STATE] FAULT");
}

void StateMachine::IncrementCycleCount()
{
    cycleCount_++;

    if ((cycleCount_ - lastSavedCycleCount_) >=
        Config::SAVE_INTERVAL_CYCLES)
    {
        cycleStorage_.SaveCount(cycleCount_);

        lastSavedCycleCount_ = cycleCount_;

        Serial.print("[EEPROM] Saved cycle count: ");
        Serial.println(cycleCount_);
    }
}

void StateMachine::SaveCycleCount()
{
    if (cycleCount_ == lastSavedCycleCount_)
        return;

    cycleStorage_.SaveCount(cycleCount_);

    lastSavedCycleCount_ = cycleCount_;

    Serial.print("[EEPROM] Saved cycle count: ");
    Serial.println(cycleCount_);
}

void StateMachine::EnterEmergencyStop()
{
    state_ = State::ESTOP;

    // Immediate shutdown; do not use ramp-down Stop().
    verticalMotor_.EmergencyStop();
    brushMotor_.EmergencyStop();
    pumpMotor_.EmergencyStop();

    cycleUpCompleted_ = false;
    reverseStartTime_ = 0;
    axisMoveStartTime_ = 0;

    // Preserve completed cycles after outputs are stopped.
    SaveCycleCount();

    Serial.println("[STATE] E-STOP LATCHED");
}