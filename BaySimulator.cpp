#include "Arduino.h"
#include "BaySimulator.h"

/* =============================================================
CIRCUIT BARAKER
=============================================================== */

//Constructor passing variables to class
CircuitBraker::CircuitBraker (uint8_t clCmd, uint8_t opCmd, uint8_t state, uint8_t ready,double charTime,uint32_t OpenTime,uint32_t CloseTime){

    _closeCmdPin = clCmd; //Pass close command pin
    _openCmdPin = opCmd; //Pass open comand pin
    _statePin = state; //Pass state pin
    _ReadyPin = ready; //Pass cb ready pin 
    _MotorTime = 0; //Clear motor timer 
    _MotorChargeTime = charTime; //Pass spring charge time
    _OpenDelay = OpenTime;//Pass open delay 
    _CloseDelay = CloseTime;//Pass close dealay
    _SetClose = 0;//Zero flags for Close triggering
    _SetOpen = 0;//Zero flags for Open triggering
 
    _DelayCounter = 0;//Zero delay counter

  }

//Method 
void CircuitBraker:: getParameters (double charTime,uint32_t OpenTime,uint32_t CloseTime){

    _MotorChargeTime = charTime; //Pass spring charge time
    _OpenDelay = OpenTime;//Pass open delay 
    _CloseDelay = CloseTime;//Pass close dealay

}

void CircuitBraker::Monitoring(){

    // Close command execute
    // Condition is charged cb spring "cb ready" and relased open comand "anti pumping algorithm" and set close comand
    if (!digitalRead(_closeCmdPin) & digitalRead(_ReadyPin) & digitalRead(_openCmdPin) & !digitalRead(_statePin)){
      //digitalWrite(_statePin, HIGH);
      _SetClose = 1;
      _MotorTime = 0;    
    } 

    //Open command execute
    if (!digitalRead(_openCmdPin) & digitalRead(_closeCmdPin))
      //digitalWrite(_statePin, LOW);
      _SetOpen = 1; 

    //Generate open and close delay
    //==============================================

    if(_SetClose){
          
        if(_DelayCounter<=_CloseDelay){
          _DelayCounter++;
        }
        else{
          digitalWrite(_statePin, HIGH);
          _SetClose = 0;
          _DelayCounter = 0;
        }
    }

    if(_SetOpen){

        if(_DelayCounter<=_OpenDelay){
          _DelayCounter++;
        }
        else{
          digitalWrite(_statePin, LOW);
          _SetOpen = 0;
          _DelayCounter = 0;
        }
    }



    //Incrase motor counter if cb is discharged
    if (_MotorTime <= _MotorChargeTime) //aproxim 1000 = 1s
      _MotorTime ++;

    //If timers reach charge time set cb ready high
    if (_MotorTime > _MotorChargeTime)
      digitalWrite(_ReadyPin, HIGH);
    else
      digitalWrite(_ReadyPin, LOW);

  } 


/* =============================================================
DISCONNECTOR
=============================================================== */

//Constructor passing variables to class
Disconnector::Disconnector (uint8_t clCmd, uint8_t opCmd, uint8_t state, uint8_t motion,double motionTime){

    _closeCmdPin = clCmd; //Pass close command pin
    _openCmdPin = opCmd; //Pass open comand pin
    _statePin = state; //Pass state pin
    _InMotion = motion; //Pass in motion pin
    _motionCounter = 0; //Clear motor timer 
    _transitTime = motionTime; //Pass spring charge time
    _SetClose = 0;
    _SetOpen = 0;
    _BlinkCounter = 0;

  }

//Method
void Disconnector::getParameters(double motionTime){
  _transitTime = motionTime; //Pass spring charge time
}

void Disconnector::Monitoring(){

    if(!digitalRead(_closeCmdPin) & !digitalRead(_statePin) & digitalRead(_openCmdPin)){
        _SetClose =1; //Start close motion
        digitalWrite(_InMotion,HIGH);
    }
        

    if(!digitalRead(_openCmdPin) & digitalRead(_statePin) & digitalRead(_closeCmdPin)){
        _SetOpen = 1; //Start close motion
        digitalWrite(_InMotion,HIGH);
    }

        
    if (_SetClose){

      _BlinkCounter++;
      
        if(_BlinkCounter == 250){
          
          if(digitalRead(_InMotion))
          digitalWrite(_InMotion,LOW);
          else
          digitalWrite(_InMotion,HIGH);
        _BlinkCounter = 0;
        _motionCounter ++;
        }

        if(_motionCounter == _transitTime){
          digitalWrite(_InMotion,LOW);
          digitalWrite(_statePin,HIGH); //Here is actualy open
          _motionCounter = 0;
          _SetClose = 0;
        }
    }

    if (_SetOpen){

      _BlinkCounter++;
      
        if(_BlinkCounter == 250){
          
          if(digitalRead(_InMotion))
          digitalWrite(_InMotion,LOW);
          else
          digitalWrite(_InMotion,HIGH);
        _BlinkCounter = 0;
        _motionCounter ++;
        }

        if(_motionCounter == _transitTime){
          digitalWrite(_InMotion,LOW);
          digitalWrite(_statePin,LOW); //Here is actualy open
          _motionCounter = 0;
          _SetOpen = 0;
        }
    }


  } 