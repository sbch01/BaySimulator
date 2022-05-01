/*
  BaySimolator is open source device wich simulate 1 circuit braker and 4 switches.
  This tool is used for simulating in lab conditions behaivor of primariy equipment. During relay protection test.
*/


#include <Arduino.h>
#include <LiquidCrystal.h>
//#include <EEPROM.h>
#include <USBAPI.h>


/* Ports names
   Именуване на входовете и изходите
=======================================================*/

const uint8_t CB_closeCmd = 2; 
const uint8_t CB_openCmd = 3; 
const uint8_t CB_state = 14; 
const uint8_t CB_ready = 15; 


class CircuitBraker  {
 
  public:

  // Constructor
  CircuitBraker (uint8_t clCmd, uint8_t opCmd, uint8_t state, uint8_t ready){

    _closeCmdPin = clCmd; //Pass close command pin
    _openCmdPin = opCmd; //Pass open comand pin
    _statePin = state; //Pass state pin
    _ReadyPin = ready; //Pass cb ready pin 
    _MotorTime = 0; //Clear motor timer 

  }

  // Method thath cycling in loop and controling CB
  void Monitoring(){

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
    if (_MotorTime <= 10000) //aproxim 1000 = 1s
      _MotorTime ++;

    //If timers reach charge time set cb ready high
    if (_MotorTime > 10000)
      digitalWrite(_ReadyPin, HIGH);
    else
      digitalWrite(_ReadyPin, LOW);

  } 

  private:

    uint8_t _closeCmdPin, _openCmdPin, _statePin, _ReadyPin;
    double  _MotorTime; //

    
};

//Create object circuit baraker
CircuitBraker CB1(CB_closeCmd,CB_openCmd,CB_state,CB_ready);

// SETUP INICIALISATION
void setup() {

  /* Config direction of ports
     Конфигуриране на посоката портовете  */
  pinMode(LED_BUILTIN, OUTPUT);
  
  pinMode(CB_closeCmd,INPUT);
  pinMode(CB_openCmd,INPUT);

  pinMode(CB_state,OUTPUT);
  pinMode(CB_ready,OUTPUT);

  /* Off All switches and circuit braker */
  digitalWrite(CB_state, LOW);
  digitalWrite(CB_ready, LOW);

  
}

// LOOO...P.............................
//======================================
void loop() {

 CB1.Monitoring();

  delay(1);
}
