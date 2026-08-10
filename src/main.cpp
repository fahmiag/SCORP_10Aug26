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
 * [ ] Check EmergencyStop() code
 * [ ] Write State Machine Code.
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