//#pragma once

#include "App.h"
#include <Arduino.h>
#include <avr/wdt.h>



App::App()
    : brushMotor_(
          Pin::BrushPWM,
          Pin::BrushDir),

      verticalMotor_(
          Pin::VerticalMotorPWM,
          Pin::VerticalMotorDir,
          Pin::VerticalTopLimit,
          Pin::VerticalBottomLimit),



      stateMachine_(
          io_,
          brushMotor_,
          verticalMotor_)
{
}
void App::Begin()
{
    Serial.begin(115200); 
    wdt_enable(WDTO_8S);

    io_.Begin();

    verticalMotor_.Begin();
                     
    brushMotor_.Begin();

    Serial.println("\nStart state machine: Done INIT");
    Serial.println("Press START Button");
}

void App::Update()
{

    io_.Update();

    stateMachine_.Update();

    brushMotor_.Update();
    verticalMotor_.Update();


   
    wdt_reset();
 

    
}