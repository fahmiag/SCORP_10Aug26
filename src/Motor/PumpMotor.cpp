#include "PumpMotor.h"
#include "config/Config.h"

PumpMotor::PumpMotor(uint8_t pwmPin, uint8_t dirPin)
    : pwmPin_(pwmPin),
      initialized_(false),
      enabled_(false),
      selectedSpeed_(Config::PUMP_MOTOR_SPEED),
      currentSpeed_(0),
      targetSpeed_(0),
      rampRate_(Config::PUMP_RAMP_RATE),
      lastUpdateTime_(0)
{
    if (rampRate_ == 0)
    {
        rampRate_ = 1;
    }
}

void PumpMotor::Begin()
{
    // Set the output latch LOW before enabling the output.
    digitalWrite(pwmPin_, LOW);
    pinMode(pwmPin_, OUTPUT);

    enabled_ = false;
    currentSpeed_ = 0;
    targetSpeed_ = 0;

    initialized_ = true;

    ApplyOutput();

    lastUpdateTime_ = millis();
}

void PumpMotor::Update()
{
    if (!initialized_)
    {
        return;
    }

    const unsigned long now = millis();

    if ((now - lastUpdateTime_) < Config::PUMP_UPDATE_RATE)
    {
        return;
    }

    lastUpdateTime_ = now;

    if (currentSpeed_ < targetSpeed_)
    {
        uint16_t newSpeed =
            static_cast<uint16_t>(currentSpeed_) + rampRate_;

        if (newSpeed > targetSpeed_)
        {
            newSpeed = targetSpeed_;
        }

        currentSpeed_ = static_cast<uint8_t>(newSpeed);
    }
    else if (currentSpeed_ > targetSpeed_)
    {
        int16_t newSpeed =
            static_cast<int16_t>(currentSpeed_) - rampRate_;

        if (newSpeed < targetSpeed_)
        {
            newSpeed = targetSpeed_;
        }

        currentSpeed_ = static_cast<uint8_t>(newSpeed);
    }

    ApplyOutput();
}

void PumpMotor::Start()
{
    if (!initialized_)
    {
        return;
    }

    enabled_ = true;
    targetSpeed_ = selectedSpeed_;
}

void PumpMotor::Stop()
{
    // Normal stop: ramp down to zero.
    enabled_ = false;
    targetSpeed_ = 0;
}

void PumpMotor::EmergencyStop()
{
    // Immediate shutdown: bypass the ramp.
    enabled_ = false;
    targetSpeed_ = 0;
    currentSpeed_ = 0;

    if (initialized_)
    {
        ApplyOutput();
    }
}

void PumpMotor::SetSpeed(uint8_t speed)
{
    selectedSpeed_ = speed;

    if (enabled_)
    {
        targetSpeed_ = selectedSpeed_;
    }
}

void PumpMotor::SetRampRate(uint8_t rampRate)
{
    rampRate_ = (rampRate == 0) ? 1 : rampRate;
}

uint8_t PumpMotor::GetCurrentSpeed() const
{
    return currentSpeed_;
}

uint8_t PumpMotor::GetTargetSpeed() const
{
    return targetSpeed_;
}

uint8_t PumpMotor::GetSelectedSpeed() const
{
    return selectedSpeed_;
}

bool PumpMotor::IsRunning() const
{
    return currentSpeed_ > 0;
}

void PumpMotor::ApplyOutput()
{
    analogWrite(pwmPin_, currentSpeed_);
}