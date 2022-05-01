#include "Arduino.h"
#include "BaySimulator.h"



//Constructor passing variables to class
CircuitBraker::CircuitBraker (uint8_t clCmd, uint8_t opCmd, uint8_t state, uint8_t ready,double charTime){

    _closeCmdPin = clCmd; //Pass close command pin
    _openCmdPin = opCmd; //Pass open comand pin
    _statePin = state; //Pass state pin
    _ReadyPin = ready; //Pass cb ready pin 
    _MotorTime = 0; //Clear motor timer 
    _MotorChargeTime = charTime; //Pass spring charge time

  }

//Method 
void CircuitBraker::Monitoring(){

    // Close command execute
    // Condition is charged cb spring "cb ready" and relased open comand "anti pumping algorithm" and set close comand
    if (!digitalRead(_closeCmdPin) & digitalRead(_ReadyPin) & digitalRead(_openCmdPin)){
      digitalWrite(_statePin, HIGH);
      _MotorTime = 0;    
    } 

    //Open command execute
    if (!digitalRead(_openCmdPin))
      digitalWrite(_statePin, LOW); 

    //Incrase motor counter if cb is discharged
    if (_MotorTime <= _MotorChargeTime) //aproxim 1000 = 1s
      _MotorTime ++;

    //If timers reach charge time set cb ready high
    if (_MotorTime > _MotorChargeTime)
      digitalWrite(_ReadyPin, HIGH);
    else
      digitalWrite(_ReadyPin, LOW);

  } 
