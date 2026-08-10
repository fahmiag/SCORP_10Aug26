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
 * [ ] Motor class 
 * [ ] BrushMotor Class, child to Motor class
 * [ ] AxisMotor Class, child to Motor class
 * [ ] Button Class
*/

#include <Arduino.h>
#include "App.h"

App app;

void setup()
{
    app.Initialize();

}

void loop()
{
    app.Update();
}