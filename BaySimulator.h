/*
  BaySimulator.cpp - Library for Implement Primary eqipment
  Created by S. Banchev, May 1, 2022.
  Released into the public domain.
*/

#ifndef _BAYSIMULATOR_H_
#define _BAYSIMULATOR_H_



#include "Arduino.h"

//============================================================================================
//
//============================================================================================

class CircuitBraker  {
 
  public:

  // Constructor
  CircuitBraker (uint8_t clCmd, uint8_t opCmd, uint8_t state, uint8_t ready,double charTime,uint32_t OpenTime,uint32_t CloseTime);

  // Method thath cycling in loop and controling CB
  void Monitoring();

  private:

    uint8_t _closeCmdPin, _openCmdPin, _statePin, _ReadyPin; //Used for pin pass
    double  _MotorTime; //Used for counting motor time
    double  _MotorChargeTime;//Used to pass motor time to object
    uint32_t _OpenDelay,_CloseDelay;//Used for delay CB Close and Open
    uint8_t _SetClose, _SetOpen;//Used for close and open  trigerring
    uint32_t _DelayCounter; //Used to delay CB Close and Open time
    
};
//============================================================================================
//
//============================================================================================
class Disconnector  {
 
  public:

  // Constructor
  Disconnector (uint8_t clCmd, uint8_t opCmd, uint8_t state, uint8_t motion,double motionTime);

  // Method thath cycling in loop and controling CB
  void Monitoring();

  private:

    uint8_t _closeCmdPin, _openCmdPin, _statePin, _InMotion; //Used for pin pass
    double  _motionCounter; //Used for counting motor time
    double  _transitTime;//Used to pass motor time to object
    uint32_t _BlinkCounter;//Used for blinking diode during motion
    uint8_t _SetClose, _SetOpen;//Used for close and open motion trigerring

};

#endif
