#include "BrushMotor.h"
#include "config/Debug.h"

BrushMotor::BrushMotor(uint8_t pwmPin, uint8_t dirPin)
    : pwmPin_(pwmPin),
      dirPin_(dirPin),
      direction_(Direction::STOP),
      currentSpeed_(0),
      targetSpeed_(0),
      rampRate_(20), //5
      lastUpdateTime_(0)
{
}

void BrushMotor::Begin()
{
    pinMode(pwmPin_, OUTPUT);
    pinMode(dirPin_, OUTPUT);

    direction_ = Direction::STOP;
    currentSpeed_ = 0;
    targetSpeed_ = 0;

    ApplyOutput();

    lastUpdateTime_ = millis();
}

void BrushMotor::Update()
{
    unsigned long now = millis();

    if (now - lastUpdateTime_ < 10)
        return;

    lastUpdateTime_ = now;

    // Accelerate
    if (currentSpeed_ < targetSpeed_)
    {
        uint16_t newSpeed = currentSpeed_ + rampRate_;

        if (newSpeed > targetSpeed_)
            newSpeed = targetSpeed_;

        currentSpeed_ = newSpeed;
        
        // DEBUG_PRINT("Brush");
        // DEBUG_PRINTLN (currentSpeed_);
    }

    // Decelerate
    else if (currentSpeed_ > targetSpeed_)
    {
        int16_t newSpeed = currentSpeed_ - rampRate_;

        if (newSpeed < targetSpeed_)
            newSpeed = targetSpeed_;

        currentSpeed_ = newSpeed;
        // DEBUG_PRINT("Brush");
        // DEBUG_PRINTLN (currentSpeed_);
    }

    ApplyOutput();
}

void BrushMotor::Forward()
{
    direction_ = Direction::FORWARD;
}

void BrushMotor::Reverse()
{
    direction_ = Direction::REVERSE;
}

void BrushMotor::Stop()
{
    targetSpeed_ = 0;
}

void BrushMotor::EmergencyStop()
{
    analogWrite(pwmPin_, 0); 
    targetSpeed_ = 0;
    currentSpeed_ = 0;
}

void BrushMotor::SetSpeed(uint8_t speed)
{
    targetSpeed_ = speed;
}

void BrushMotor::SetRampRate(uint8_t rampRate)
{
    rampRate_ = rampRate;
}

uint8_t BrushMotor::GetCurrentSpeed() const
{
    return currentSpeed_;
}

uint8_t BrushMotor::GetTargetSpeed() const
{
    return targetSpeed_;
}

bool BrushMotor::IsRunning() const
{
    return currentSpeed_ > 0;
}

void BrushMotor::ApplyOutput()
{
    switch (direction_)
    {
        case Direction::FORWARD:
            digitalWrite(dirPin_, HIGH);
            break;

        case Direction::REVERSE:
            digitalWrite(dirPin_, LOW);
            break;

        case Direction::STOP:  
            //analogWrite(pwmPin_, 0);
            return;
    }

    analogWrite(pwmPin_, currentSpeed_);
}