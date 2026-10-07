#include "Display.h"
#include "config/Config.h"

//constructor updated with wrong previous value
Display::Display(
    uint8_t address,
    uint8_t columns,
    uint8_t rows
)
    : lcd_(address, columns, rows),
      columns_(columns),
      rows_(rows),
      initialized_(false),
      previousState_(StateMachine::State::FAULT),
      previousDirection_(StateMachine::AxisDirection::UP),
      previousFault_(StateMachine::FaultReason::NONE),
      previousCycleCount_(UINT32_MAX),
      lastUpdateTime_(0)
{
}

bool Display::Begin()
{
    lcd_.init();
    lcd_.backlight();
    lcd_.clear();

    initialized_ = true;

    PrintLine(0, "SCORP Robot");

    return true;
}

void Display::Clear()
{
    if (!initialized_)
        return;

    lcd_.clear();
}

void Display::Print(
    uint8_t column,
    uint8_t row,
    const char* text
)
{
    if (!initialized_)
        return;

    if (column >= columns_ || row >= rows_)
        return;

    lcd_.setCursor(column, row);
    lcd_.print(text);
}

void Display::Print(
    uint8_t column,
    uint8_t row,
    const String& text
)
{
    Print(column, row, text.c_str());
}

void Display::PrintLine(
    uint8_t row,
    const char* text
)
{
    if (!initialized_)
        return;

    if (row >= rows_)
        return;

    lcd_.setCursor(0, row);

    // Clear existing content on this row
    for (uint8_t i = 0; i < columns_; i++)
    {
        lcd_.print(' ');
    }

    lcd_.setCursor(0, row);
    lcd_.print(text);
}

void Display::SetBacklight(bool enabled)
{
    if (!initialized_)
        return;

    if (enabled)
    {
        lcd_.backlight();
    }
    else
    {
        lcd_.noBacklight();
    }
}

void Display::Update(
    StateMachine::State state,
    StateMachine::AxisDirection direction,
    StateMachine::FaultReason fault,
    uint32_t cycleCount
)
{
    if (!initialized_)
        return;

    // E-stop owns the entire display.
    // Return before direction, cycle or fault updates can overwrite it.
    if (state == StateMachine::State::ESTOP)
    {
        if (previousState_ != StateMachine::State::ESTOP)
        {
            PrintLine(0, "SCORP Robot");
            PrintLine(1, "***  E-STOP  ***");
            PrintLine(2, "Contact person ");
            PrintLine(3, " in charge");

            previousState_ = StateMachine::State::ESTOP;
        }

        return;
    }

    const unsigned long now = millis();

    const bool stateChanged =
        state != previousState_;

    const bool directionChanged =
        direction != previousDirection_;
    
    const bool cycleChanged =
        cycleCount != previousCycleCount_;

    // const bool periodicUpdate =
    //     (now - lastUpdateTime_) >= UPDATE_INTERVAL_MS;
    const bool faultChanged =
        fault != previousFault_;
    

    // No changes

    if (!stateChanged &&
        !directionChanged &&
        !cycleChanged &&
        !faultChanged
    )
    {
        return;
    }

     
    // ---------------------------------------
    // Update state row only when necessary
    // ---------------------------------------
    if (stateChanged) //(stateChanged || periodicUpdate)
    {
        switch (state)
        {
            case StateMachine::State::IDLE:
                PrintLine(1, "State: IDLE   ");
                break;

            case StateMachine::State::RUNNING:
                PrintLine(1, "State: RUNNING");
                break;
            
            case StateMachine::State::WAITING_TO_REVERSE:
                PrintLine(1, "State: REVERSE");
                break;
            
            case StateMachine::State::STOPPING:
                PrintLine(1, "State: STOPPING");
                break;

            case StateMachine::State::FAULT:
                PrintLine(1, "State: FAULT   ");
                break;

            case StateMachine::State::ESTOP:
                // Handled above.
                break;
        }
        previousState_ = state;
    }
    // ---------------------------------------
    // Update direction row only when necessary
    // ---------------------------------------

    if (directionChanged) //(directionChanged || periodicUpdate)
    {
        switch (direction)
        {
            case StateMachine::AxisDirection::UP:
                PrintLine(2, "Dir  : UP");
                break;

            case StateMachine::AxisDirection::DOWN:
                PrintLine(2, "Dir  : DOWN");
                break;
        }

        previousDirection_ = direction;
    }

        // ---------------------------------------
    // Update cycle row only when necessary
    // ---------------------------------------

    if (cycleChanged) //(cycleChanged || periodicUpdate)
    {
        char buffer[21];

        snprintf(
            buffer,
            sizeof(buffer),
            "Cycle: %lu",
            static_cast<unsigned long>(cycleCount)
        );

        PrintLine(3, buffer);

        previousCycleCount_ = cycleCount;
    }

    if (faultChanged)
    {
        switch (fault)
        {
            case StateMachine::FaultReason::NONE:
                PrintLine(2, "                "); //delete all fault
                break;

            case StateMachine::FaultReason::AXIS_TIMEOUT:
                PrintLine(2, "FAULT: TIMEOUT");
                break;

        }
        previousFault_ = fault;
    }

   
    
    lastUpdateTime_ = now;

}