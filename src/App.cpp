
#include "App.h"
#include <Arduino.h>
#include <avr/wdt.h>



void App::Initialize()
{
    Serial.begin(115200); 
    wdt_enable(WDTO_8S);


    Serial.println("\nStart state machine: Done INIT");
    Serial.println("Press START");
}

void App::Update()
{

    


   
    wdt_reset();
 

    
}