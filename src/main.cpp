/*
 * Project: Abrasion test, State Machine, Class
 * Date: 10 Aug 2026
 * Programmer: Fahmi Ghani
 * 
 * Formating:
 * - Curly braces must be in same indent.
 * - Function name -TBA
 * - Variable name - TBA
 * 
 * TODO: 
 * [x] BrushMotor Class
 * [x] AxisMotor Class
 * [x] Button, in IO class
 * [x] Write State Machine Code.
 * [x] Button and limit Serial print
 * [x] Serial Print Motor Speed
 * [x] Check brush Motor Speed
 * [x] Check Motor Slowdown during direction change
 * [x] Stop for a while during direction change
 * [ ] Check EmergencyStop() code
 * [ ] Emergency Stop, Stop all motor immediately
 * [ ] Add timeout, fault
 * [x] Add count
 * [ ] Test the code with actual motor driver and DC motor
 * [ ] Test Watch Dog Timer
 * [ ] Add LCD Display
 * [ ] Save count data in EEPROM
 * [ ] Send count data to RasPI
 * [ ] Write Debug function
 * [ ] Remove Magic number
 */

#include <Arduino.h>
#include "App.h"

LiquidCrystal_I2C lcd(0x27,20,4);  

App app;

void setup()
{
    app.Begin();

}

void loop()
{
    app.Update();
}