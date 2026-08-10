#pragma once

#include <Arduino.h>

class AxisMotor
{
public:

    AxisMotor(
        uint8_t pwmPin,
        uint8_t dirPin,
        uint8_t upperLimitPin,
        uint8_t lowerLimitPin
    );

    void Begin();
    void Update();

    void MoveUp();
    void MoveDown();
    void Stop();
    void EmergencyStop();

    void SetSpeed(uint8_t speed);
    void SetRampRate(uint8_t rampRate);

    bool IsAtUpperLimit() const;
    bool IsAtLowerLimit() const;

    bool IsMoving() const;

    uint8_t GetCurrentSpeed() const;
    uint8_t GetTargetSpeed() const;

private:
    enum class Direction
    {
        STOP,
        UP,
        DOWN
    };

    uint8_t pwmPin_;
    uint8_t dirPin_;

    uint8_t upperLimitPin_;
    uint8_t lowerLimitPin_;

    int targetSpeed_;
    int currentSpeed_;

    Direction direction_;

   // uint8_t currentSpeed_;
   // uint8_t targetSpeed_;

    uint8_t rampRate_;

    unsigned long lastUpdateTime_;

    void ApplyOutput();
};