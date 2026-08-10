#include "StateMachine.h"
#include "Debug.h"

StateMachine::StateMachine(
    IO& io,
    BrushMotor& brushMotor,
    AxisMotor& axisMotor1)
    : io_(io),
      brushMotor_(brushMotor),
      verticalMotor_(axisMotor1),
      state_(State::IDLE),
      verticalDirection_(AxisDirection::UP),
      previousStart_(false),
      previousStop_(false)
{
}

void StateMachine::Begin()
{
    state_ = State::IDLE;

    // First movement direction
    verticalDirection_ = AxisDirection::UP;

    previousStart_ = false;
    previousStop_ = false;

    brushMotor_.Stop();
    verticalMotor_.Stop();
}

void StateMachine::Update()
{
    switch (state_)
    {
        case State::IDLE:
            HandleIdle();
            break;

        case State::RUNNING:
            HandleRunning();
            break;

        case State::STOPPING:
            HandleStopping();
            break;
    }
}


// ======================================================
// IDLE
// ======================================================

void StateMachine::HandleIdle()
{
    if (StartPressedEvent())
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

    if (StopPressedEvent())
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
            // Ramp motor down to zero first
            verticalMotor_.Stop();

            // We will reverse once the motor has stopped
        }
    }
    else
    {
        if (verticalMotor_.IsAtLowerLimit())
        {
            // Ramp motor down to zero first
            verticalMotor_.Stop();

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


// ======================================================
// REVERSE AXIS
// ======================================================

void StateMachine::ReverseAxis()
{
    if (verticalDirection_ == AxisDirection::UP)
    {
        verticalDirection_ = AxisDirection::DOWN;
        Serial.println("[Motor] Down");

        verticalMotor_.MoveDown();
        verticalMotor_.SetSpeed(180);
    }
    else
    {
        verticalDirection_ = AxisDirection::UP;
        
        Serial.println("[Motor] Up");
        verticalMotor_.MoveUp();
        verticalMotor_.SetSpeed(180);
    }
}


// ======================================================
// START BUTTON EDGE DETECTION
// ======================================================

bool StateMachine::StartPressedEvent()
{
    bool current = io_.StartPressed();

    bool event = current && !previousStart_;

    previousStart_ = current;

    return event;
}


// ======================================================
// STOP BUTTON EDGE DETECTION
// ======================================================

bool StateMachine::StopPressedEvent()
{
    bool current = io_.StopPressed();

    bool event = current && !previousStop_;

    previousStop_ = current;

    return event;
}