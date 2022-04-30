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


class CircuitBraker {

char State;
int  MotorTime;


};

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

   if (!digitalRead(CB_closeCmd))
      digitalWrite(CB_state, HIGH);
      
   if (!digitalRead(CB_openCmd))
      digitalWrite(CB_state, LOW);  


/* 
  delay(500);                       // wait for a second */
}
