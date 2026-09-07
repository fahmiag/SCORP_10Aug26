#include "Display.h"

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
      previousDirection_(AxisMotor::Direction::STOP),
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
    AxisMotor::Direction axisDirection
)
{
    if (!initialized_)
        return;

    const unsigned long now = millis();

    const bool stateChanged =
        state != previousState_;

    const bool directionChanged =
        axisDirection != previousDirection_;

    const bool periodicUpdate =
        (now - lastUpdateTime_) >= UPDATE_INTERVAL_MS;

        // No changes and 1 second hasn't passed

    // if (!stateChanged &&
    //     !directionChanged &&
    //     !periodicUpdate)

    if (!stateChanged &&
        !directionChanged)
    {
        return;
    }

    // Remember latest values
    previousState_ = state;
    previousDirection_ = axisDirection;
    lastUpdateTime_ = now;

    // -------------------------
    // State
    // -------------------------

    switch (state)
    {
        case StateMachine::State::IDLE:
            PrintLine(1, "State: IDLE");
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
            PrintLine(1, "State: FAULT");
            break;
    }

    switch (axisDirection)
    {
        case AxisMotor::Direction::UP:
            PrintLine(2, "Move : UP");
            break;

        case AxisMotor::Direction::DOWN:
            PrintLine(2, "Move : DOWN");
            break;

        case AxisMotor::Direction::STOP:
            PrintLine(2, "Move : STOP");
            break;
    }
}