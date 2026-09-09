#pragma once

#include <Arduino.h>
#include <EEPROM.h>

class CycleStorage
{
public:
    void Begin();

    uint32_t LoadCount();

    void SaveCount(uint32_t count);

private:
    static constexpr int EEPROM_ADDRESS = 0;
};