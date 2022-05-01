/*
  BaySimulator.cpp - Library for Implement Primary eqipment
  Created by S. Banchev, May 1, 2022.
  Released into the public domain.
*/

#ifndef _BAYSIMULATOR_H_
#define _BAYSIMULATOR_H_



#include "Arduino.h"

class CircuitBraker  {
 
  public:

  // Constructor
  CircuitBraker (uint8_t clCmd, uint8_t opCmd, uint8_t state, uint8_t ready,double charTime);

  // Method thath cycling in loop and controling CB
  void Monitoring();

  private:

    uint8_t _closeCmdPin, _openCmdPin, _statePin, _ReadyPin;
    double  _MotorTime; //
    double  _MotorChargeTime;//
    
};

#endif
