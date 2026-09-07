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
          verticalMotor_),

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

    display_.Begin();

    display_.PrintLine(0, "SCORP Robot");
    display_.PrintLine(1, "State: IDLE");



    Serial.println("[APP] System ready");
}

void App::Update()
{

    io_.Update();

    stateMachine_.Update();

    brushMotor_.Update();
    verticalMotor_.Update();

    display_.Update(stateMachine_.GetState());


   
   
   
   
    wdt_reset();
 
    
}