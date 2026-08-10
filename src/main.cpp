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
 * [ ] Button and limit Serial print
 * [ ] Serial Print Motor Speed
 * [ ] Check Motor Slowdown during direction change
 * [ ] Check EmergencyStop() code
 * [ ] Emergency Stop, Stop all motor immediately
 * [ ] Add timeout, fault
 * [ ] Add count
 * [ ] Test the code with actual motor driver and DC motor
 * [ ] Test Watch Dog Timer
 * [ ] Add LCD Display
 * [ ] Save count data in EEPROM
 * [ ] Send count data to RasPI
 * [ ] Write Debug function
 * 
 */

#include <Arduino.h>
#include "App.h"

App app;

void setup()
{
    app.Begin();

}

void loop()
{
    app.Update();
}