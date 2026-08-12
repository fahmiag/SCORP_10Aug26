#include "AxisMotor.h"
#include "Debug.h"

AxisMotor::AxisMotor(
    uint8_t pwmPin,
    uint8_t dirPin,
    uint8_t upperLimitPin,
    uint8_t lowerLimitPin)
    : pwmPin_(pwmPin),
      dirPin_(dirPin),
      upperLimitPin_(upperLimitPin),
      lowerLimitPin_(lowerLimitPin),
      direction_(Direction::STOP),
      currentSpeed_(0),
      targetSpeed_(0),
      rampRate_(20),
      lastUpdateTime_(0)
{
}

void AxisMotor::Begin()
{
    pinMode(pwmPin_, OUTPUT);
    pinMode(dirPin_, OUTPUT);

    pinMode(upperLimitPin_, INPUT_PULLUP);
    pinMode(lowerLimitPin_, INPUT_PULLUP);

    direction_ = Direction::STOP;
    currentSpeed_ = 0;
    targetSpeed_ = 0;

    ApplyOutput();

    lastUpdateTime_ = millis();
}

void AxisMotor::Update()
{
    unsigned long now = millis();

    if (now - lastUpdateTime_ < 10)
        return;

    lastUpdateTime_ = now;

    // --------------------------------------------------
    // Check limit switches
    // --------------------------------------------------

    if (direction_ == Direction::UP && IsAtUpperLimit())
    {
        Stop();
    }

    if (direction_ == Direction::DOWN && IsAtLowerLimit())
    {
        Stop();
    }

    // --------------------------------------------------
    // Speed ramp
    // --------------------------------------------------

    if (currentSpeed_ < targetSpeed_)
    {
        uint16_t newSpeed = currentSpeed_ + rampRate_;

        if (newSpeed > targetSpeed_)
            newSpeed = targetSpeed_;

        currentSpeed_ = newSpeed;
        
        DEBUG_MOTOR_PRINT("VMotor");
        DEBUG_MOTOR_PRINTLN (currentSpeed_);
    }
    else if (currentSpeed_ > targetSpeed_)
    {
        int16_t newSpeed = currentSpeed_ - rampRate_;

        if (newSpeed < targetSpeed_)
            newSpeed = targetSpeed_;

        currentSpeed_ = newSpeed;
        
        DEBUG_MOTOR_PRINT("VMotor");
        DEBUG_MOTOR_PRINTLN (currentSpeed_);
    }

    ApplyOutput();
}

void AxisMotor::MoveUp()
{
    // Do not allow movement into upper limit
    if (IsAtUpperLimit())
    {
        Stop();
        return;
    }

    direction_ = Direction::UP;
}

void AxisMotor::MoveDown()
{
    // Do not allow movement into lower limit
    if (IsAtLowerLimit())
    {
        Stop();
        return;
    }

    direction_ = Direction::DOWN;
}

void AxisMotor::Stop() //Controlled deceleration
{
    targetSpeed_ = 0;
}

void AxisMotor::EmergencyStop() //Immediate output shutdown
{
    analogWrite(pwmPin_, 0); 
    targetSpeed_ = 0;
    currentSpeed_ = 0;
}

void AxisMotor::SetSpeed(uint8_t speed)
{
    targetSpeed_ = speed;
}

void AxisMotor::SetRampRate(uint8_t rampRate)
{
    rampRate_ = rampRate;
}

bool AxisMotor::IsAtUpperLimit() const
{
    return digitalRead(upperLimitPin_) == LOW;
}

bool AxisMotor::IsAtLowerLimit() const
{
    return digitalRead(lowerLimitPin_) == LOW;
}

bool AxisMotor::IsMoving() const
{
    return currentSpeed_ > 0;
}

uint8_t AxisMotor::GetCurrentSpeed() const
{
    return currentSpeed_;
}

uint8_t AxisMotor::GetTargetSpeed() const
{
    return targetSpeed_;
}

void AxisMotor::ApplyOutput()
{
    // --------------------------------------------------
    // Safety: prevent movement into active limit
    // --------------------------------------------------

    if (direction_ == Direction::UP && IsAtUpperLimit())
    {
        analogWrite(pwmPin_, 0);
        return;
    }

    if (direction_ == Direction::DOWN && IsAtLowerLimit())
    {
        analogWrite(pwmPin_, 0);
        return;
    }

    switch (direction_)
    {
        case Direction::UP:
            digitalWrite(dirPin_, HIGH);
            break;

        case Direction::DOWN:
            digitalWrite(dirPin_, LOW);
            break;

        case Direction::STOP:
            analogWrite(pwmPin_, 0);  //TODO: Check this part, maybe need to remove
            return;
    }

    analogWrite(pwmPin_, currentSpeed_);
}

//TODO: add EmergencyStop()