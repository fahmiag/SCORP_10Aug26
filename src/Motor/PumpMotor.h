#pragma once

#include <Arduino.h>

class PumpMotor
{
public:
    explicit PumpMotor(uint8_t pwmPin, uint8_t dirPin);

    void Begin();
    void Update();

    void Start();
    void Stop();
    void EmergencyStop();

    // Selected running speed: 0–255.
    // Changing this while stopped does not start the pump.
    void SetSpeed(uint8_t speed);

    // PWM steps per update. Minimum value is 1.
    void SetRampRate(uint8_t rampRate);

    uint8_t GetCurrentSpeed() const;
    uint8_t GetTargetSpeed() const;
    uint8_t GetSelectedSpeed() const;

    // True while PWM is nonzero, including ramp-down.
    bool IsRunning() const;

private:
    uint8_t pwmPin_;

    bool initialized_;
    bool enabled_;

    uint8_t selectedSpeed_;
    uint8_t currentSpeed_;
    uint8_t targetSpeed_;
    uint8_t rampRate_;

    unsigned long lastUpdateTime_;

    void ApplyOutput();
};