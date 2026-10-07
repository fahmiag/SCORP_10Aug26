//#pragma once

#include "App.h"
#include <Arduino.h>
#include <avr/wdt.h>

#include "config/PinMap.h"



App::App()
    : 
        io_(),
    
        brushMotor_(
          Pin::BrushPWM,
          Pin::BrushDir),

      verticalMotor_(
          Pin::VerticalMotorPWM,
          Pin::VerticalMotorDir,
          Pin::VerticalTopLimit,
          Pin::VerticalBottomLimit),

      pumpMotor_(
            Pin::PumpPWM, 
            Pin::PumpDir),

      cycleStorage_(),

      stateMachine_(
          io_,
          brushMotor_,
          verticalMotor_,
          pumpMotor_,
          cycleStorage_),

      display_(0x27, 20, 4)

{
}


void App::Begin()
{
    Serial.begin(115200); 

    Serial.println("==============================");
    Serial.println(" SCORP Machine Starting      ");
    Serial.println("==============================");


    wdt_enable(WDTO_8S);

    io_.Begin();

    verticalMotor_.Begin();
    brushMotor_.Begin();
    pumpMotor_.Begin();   

    cycleStorage_.Begin();

    // TEMPORARY - reset EEPROM counter
   // stateMachine_.ResetCycleCount();

    stateMachine_.Begin();

    // display_.Begin();

    // display_.PrintLine(0, "SCORP Robot");
    // display_.PrintLine(1, "State: IDLE");

    // Serial.println("[APP] System ready");

    display_.Begin();

    // Check again in case E-stop was pressed during LCD startup.
    io_.Update();
    stateMachine_.Update();

    // Display the actual startup state instead of forcing IDLE.
    display_.Update(
        stateMachine_.GetState(),
        stateMachine_.GetAxisDirection(),
        stateMachine_.GetFault(),
        stateMachine_.GetCycleCount()
    );

    if (stateMachine_.GetState() == StateMachine::State::ESTOP)
    {
        Serial.println("[APP] Startup blocked by E-stop");
    }
    else
    {
        Serial.println("[APP] System ready");
    }

    wdt_reset();
}

void App::Update()
{

    io_.Update();
    stateMachine_.Update();

    // Skip all normal motor updates while E-stop is latched.
    if (stateMachine_.GetState() != StateMachine::State::ESTOP)
    {
        brushMotor_.Update();
        verticalMotor_.Update();   
        pumpMotor_.Update(); 
    }

    display_.Update(
        stateMachine_.GetState(),
        stateMachine_.GetAxisDirection(),
        stateMachine_.GetFault(),
        stateMachine_.GetCycleCount()
    );

    wdt_reset();
    // Keep servicing the watchdog, including during E-stop.
    // Otherwise a watchdog reset could clear the latch
    // after the physical button has been released.
 
    
}