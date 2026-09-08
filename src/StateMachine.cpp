#include "StateMachine.h"
#include "config/Debug.h"

StateMachine::StateMachine(
    IO& io,
    BrushMotor& brushMotor,
    AxisMotor& axisMotor1)
    : io_(io),
      brushMotor_(brushMotor),
      verticalMotor_(axisMotor1),
      state_(State::IDLE),
      verticalDirection_(AxisDirection::UP),
      reverseStartTime_(0),
      cycleCount_(0),
      cycleUpCompleted_(false),
      axisMoveStartTime_(0),
      FaultReason_(FaultReason::NONE)
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
    verticalMotor_.Stop();

    DEBUG_PRINTLN("[STATE] IDLE");
    DEBUG_PRINTLN("[CYCLE] Counter = 0");
}

void StateMachine::Update()
{
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
                cycleCount_++;

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

    if (elapsed >= REVERSE_DELAY_MS)
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
    cycleUpCompleted_ = false;

    DEBUG_PRINTLN("[CYCLE] Counter reset");
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

    if (elapsed >= AXIS_TIMEOUT_MS)
    {
        Serial.println("[FAULT] Axis movement timeout");
        EnterFault(FaultReason::AXIS_TIMEOUT);

        
    }
}

void StateMachine::EnterFault(FaultReason reason)
{
    verticalMotor_.Stop();
    brushMotor_.Stop();

    state_ = State::FAULT;
    //FaultReason_ = StateMachine::GetFault();
    FaultReason_ = reason;

    Serial.println("[STATE] FAULT");
}