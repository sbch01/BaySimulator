/*
  BaySimolator is open source device wich simulate 1 circuit braker and 3 switches.
  This tool is used for simulating in lab conditions behaivor of primariy equipment. During relay protection test.
*/

#include "BaySimulator.h"
#include <Arduino.h>
#define CR 13
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

//Disconnector-2 pins
const uint8_t Disc2_closeCmd = 6;
const uint8_t Disc2_openCmd = 7;
const uint8_t Disc2_state = 18;
const uint8_t Disc2_motion = 19;


/* Variables
====================================================== */
double CB_chargeTime = 10000; //Time for charging the spring of braker -> aproxim 1000 = 1s
double Disc_motionTime = 20; //Time for motion disconnector -> aprox 4= 1s
uint32_t CB_CloseDelay = 35; //aprox 1 = 1 ms
uint32_t CB_OpenDelay = 20;

//variables for communication
uint8_t recived_byte; //used for serial communication
String inputMsg;
uint8_t MsgCnt = 0; //Flag for that is chose from menu option
//======================================================



//Create object circuit baraker
CircuitBraker CB1(CB_closeCmd,CB_openCmd,CB_state,CB_ready,CB_chargeTime,CB_OpenDelay,CB_CloseDelay);

//Create object disconnector 1
Disconnector Disc1(Disc1_closeCmd, Disc1_openCmd, Disc1_state, Disc1_motion, Disc_motionTime);

//Create object disconnector 1
Disconnector Disc2(Disc2_closeCmd, Disc2_openCmd, Disc2_state, Disc2_motion, Disc_motionTime);

// SETUP INICIALISATION
void setup() {

//Printing srttings on serial port
 Serial.begin(9600); 
 StartMenu();

  /* Config direction of ports
     Конфигуриране на посоката портовете  */
  pinMode(LED_BUILTIN, OUTPUT);
  
  //Inputs
  pinMode(CB_closeCmd,INPUT);
  pinMode(CB_openCmd,INPUT);

  pinMode(Disc1_closeCmd,INPUT);
  pinMode(Disc1_openCmd,INPUT);

  pinMode(Disc2_closeCmd,INPUT);
  pinMode(Disc2_openCmd,INPUT);

  //Outputs
  pinMode(CB_state,OUTPUT);
  pinMode(CB_ready,OUTPUT);
  pinMode(Disc1_state,OUTPUT);
  pinMode(Disc1_motion,OUTPUT);
  pinMode(Disc2_state,OUTPUT);
  pinMode(Disc2_motion,OUTPUT);

  /* Off All switches and circuit braker */
  digitalWrite(CB_state, LOW);
  digitalWrite(CB_ready, LOW);
  digitalWrite(Disc1_state,LOW);
  digitalWrite(Disc1_motion,LOW);
  digitalWrite(Disc2_state,LOW);
  digitalWrite(Disc2_motion,LOW);

  
}

// LOOO...P.............................
//======================================
void loop() {

 //Control cycle of braker and disconnector
 CB1.Monitoring();
 Disc1.Monitoring();
 Disc2.Monitoring();
 delayMicroseconds(850);


//Control cycle
if (Serial.available()){

    recived_byte = Serial.read();
    Serial.write(recived_byte);
    
    if(recived_byte == CR){
      
      if (inputMsg.equals("1")) {
      
      //Serial.print("\33\143"); //clear putty screen
      Serial.println();
      Serial.print("Enter CB charge time in secons:");
      //recived_byte = Serial.read();
      //Serial.write(recived_byte);
      }
      else{
        Serial.println();
        Serial.println(inputMsg);
        inputMsg = "";
        StartMenu();
      }   
    }
    else{
      inputMsg = inputMsg + recived_byte;
    }
  
  }

}

//Custom function for communication

void StartMenu(){

  //Serial.print("\33\143"); //clear putty screen
  Serial.println("PSS Bay Emulator 1CB-2DC  ver.0.1");
  Serial.println("==============================");
  Serial.println("Settings:");
  Serial.println("------------------------------");
  Serial.print("1.CB spring charge time: ");  Serial.print(CB_chargeTime/1000,1);Serial.println("s");
  Serial.print("2.CB close delay: ");  Serial.print(CB_CloseDelay);Serial.println("ms");
  Serial.print("3.CB open delay: ");  Serial.print(CB_OpenDelay);Serial.println("ms");
  Serial.print("4.Disconnectors open/close time: ");  Serial.print(Disc_motionTime/4, 1);Serial.println("s");
  Serial.println("------------------------------");
  Serial.print("Enter number of parameter to change: ");

}
