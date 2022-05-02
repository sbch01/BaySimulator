/*
  BaySimolator is open source device wich simulate 1 circuit braker and 3 switches.
  This tool is used for simulating in lab conditions behaivor of primariy equipment. During relay protection test.
*/

#include "BaySimulator.h"
#include <Arduino.h>

//#include <EEPROM.h>



/* Ports names
   Именуване на входовете и изходите
=======================================================*/

const uint8_t CB_closeCmd = 2; 
const uint8_t CB_openCmd = 3; 
const uint8_t CB_state = 14; 
const uint8_t CB_ready = 15;

/* Variables
====================================================== */
double CB_chargeTime = 10000; //Time for charging the spring of braker -> aproxim 1000 = 1s

//======================================================

//Create object circuit baraker
//======================================================
CircuitBraker CB1(CB_closeCmd,CB_openCmd,CB_state,CB_ready,CB_chargeTime);

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
