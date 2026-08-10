#pragma once

#include <Arduino.h>

class BrushMotor
{
public:

    BrushMotor(
        uint8_t pwmPin,
        uint8_t dirPin
    );

    void Begin();
    void Update();

    void Forward();
    void Reverse();
    void Stop();
    void EmergencyStop();

    void SetSpeed(uint8_t speed);

    void SetRampRate(uint8_t rampRate);

    uint8_t GetCurrentSpeed() const;
    uint8_t GetTargetSpeed() const;

    bool IsRunning() const;

private:

    enum class Direction
    {
        STOP,
        FORWARD,
        REVERSE
    };

    uint8_t pwmPin_;
    uint8_t dirPin_;

    Direction direction_;

    uint8_t currentSpeed_;
    uint8_t targetSpeed_;

    uint8_t rampRate_;

    unsigned long lastUpdateTime_;

    void ApplyOutput();
};