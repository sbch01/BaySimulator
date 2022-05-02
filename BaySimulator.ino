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

//Circuiut braker pins
const uint8_t CB_closeCmd = 2; 
const uint8_t CB_openCmd = 3; 
const uint8_t CB_state = 14; 
const uint8_t CB_ready = 15;

//Disconnector-1 pins
const uint8_t Disc1_closeCmd = 4;
const uint8_t Disc1_openCmd = 5;
const uint8_t Disc1_state = 16;
const uint8_t Disc1_motion = 17;


/* Variables
====================================================== */
double CB_chargeTime = 10000; //Time for charging the spring of braker -> aproxim 1000 = 1s
double Disc_motionTime = 60; //Time for motion disconnector -> aprox 1= 0.25s
//======================================================

//Create object circuit baraker
CircuitBraker CB1(CB_closeCmd,CB_openCmd,CB_state,CB_ready,CB_chargeTime);

//Create object disconnector 1
Disconnector Disc1(Disc1_closeCmd, Disc1_openCmd, Disc1_state, Disc1_motion, Disc_motionTime);

// SETUP INICIALISATION
void setup() {

  /* Config direction of ports
     Конфигуриране на посоката портовете  */
  pinMode(LED_BUILTIN, OUTPUT);
  
  //Inputs
  pinMode(CB_closeCmd,INPUT);
  pinMode(CB_openCmd,INPUT);

  pinMode(Disc1_closeCmd,INPUT);
  pinMode(Disc1_openCmd,INPUT);

  //Outputs
  pinMode(CB_state,OUTPUT);
  pinMode(CB_ready,OUTPUT);
  pinMode(Disc1_state,OUTPUT);
  pinMode(Disc1_motion,OUTPUT);

  /* Off All switches and circuit braker */
  digitalWrite(CB_state, LOW);
  digitalWrite(CB_ready, LOW);
  digitalWrite(Disc1_state,LOW);
  digitalWrite(Disc1_motion,LOW);

  
}

// LOOO...P.............................
//======================================
void loop() {

 CB1.Monitoring();
 Disc1.Monitoring();

  delay(1);
}
