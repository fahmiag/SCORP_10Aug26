#include "CycleStorage.h"

void CycleStorage::Begin()
{
    // Nothing required for UNO R3
}

uint32_t CycleStorage::LoadCount()
{
    uint32_t count;

    EEPROM.get(
        EEPROM_ADDRESS,
        count
    );

    // Fresh EEPROM commonly contains 0xFF
    if (count == UINT32_MAX)
    {
        count = 0;
    }

    return count;
}

void CycleStorage::SaveCount(uint32_t count)
{
    EEPROM.put(
        EEPROM_ADDRESS,
        count
    );
}