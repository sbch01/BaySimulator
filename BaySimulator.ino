/*
  BaySimolator is open source device wich simulate 1 circuit braker and 3 switches.
  This tool is used for simulating in lab conditions behaivor of primariy equipment. During relay protection test.
*/

#include "BaySimulator.h"
#include <Arduino.h>
#include <EEPROM.h>
#define CR 13
#define ESC 27

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

//variables for emulated equipment
double CB_chargeTime; //Time for charging the spring of braker -> aproxim 1000 = 1s
double Disc_motionTime; //Time for motion disconnector -> aprox 4 = 1s
uint32_t CB_CloseDelay; //aprox 1 = 1 ms
uint32_t CB_OpenDelay;

//variables for communication
char recived_byte; //used for serial communication
char inputMsg [40]; //used for buffer for input from user keybord
unsigned int RcvCnt = 0; //used for couting input msg
uint8_t MsgCnt = 0; //Flag for that is chose from menu option
uint8_t MenuDept = 0;//For menu selection
uint8_t ParameterSelect = 0;
uint32_t ParameterTemp;//temporary 
//======================================================



//Create object circuit baraker
CircuitBraker CB1(CB_closeCmd,CB_openCmd,CB_state,CB_ready,CB_chargeTime,CB_OpenDelay,CB_CloseDelay);

//Create object disconnector 1
Disconnector Disc1(Disc1_closeCmd, Disc1_openCmd, Disc1_state, Disc1_motion, Disc_motionTime);

//Create object disconnector 1
Disconnector Disc2(Disc2_closeCmd, Disc2_openCmd, Disc2_state, Disc2_motion, Disc_motionTime);



// SETUP INICIALISATION
void setup() {

//****************Uncoment for first load******************
//                EEPROM whrite
//*********************************************************
/* 
  CB_chargeTime = 10000;
  Disc_motionTime = 20;
  CB_CloseDelay = 35;
  CB_OpenDelay = 20;

  EEPROM.put(0,CB_chargeTime);
  EEPROM.put(10,Disc_motionTime);
  EEPROM.put(20,CB_CloseDelay);
  EEPROM.put(30,CB_OpenDelay); */
 //*********************************************************
 
  //Read parameters from EEPROM
  EEPROM.get(0, CB_chargeTime);
  EEPROM.get(10,Disc_motionTime);
  EEPROM.get(20,CB_CloseDelay);
  EEPROM.get(30,CB_OpenDelay);

  //Pass parameters to objects
  CB1.getParameters(CB_chargeTime,CB_OpenDelay,CB_CloseDelay);
  Disc1.getParameters(Disc_motionTime);
  Disc2.getParameters(Disc_motionTime);
  
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


//Control cycle of reciving input from UART
if (Serial.available()){

    //recive buffer is full read it
    recived_byte = Serial.read();
    
    //if press escape cancel evry dept of menu
    if (recived_byte == ESC){
        RcvCnt=0;
        MenuDept = 0;
        inputMsg [0] = '0';
        Serial.println();
        StartMenu();
    }

    //control input from user recive only digit 
    if (recived_byte >= '0' and recived_byte <= '9'){
      
      //interlock to iput more then one digit when you are on the main menu
      if (MenuDept ==0 && RcvCnt == 0){
          inputMsg[RcvCnt]=recived_byte;
          Serial.write(recived_byte);
          RcvCnt++;
      }

      //relase interlock above
      else if (MenuDept == 1){
          inputMsg[RcvCnt]=recived_byte;
          Serial.write(recived_byte);
          RcvCnt++;
      }
      
    }
    
      
    //Pressing Enter key   
    if(recived_byte == CR){
      
    //MenuDept 0 is a start screen
        if(MenuDept == 0){ 

            if (inputMsg [0] == '1') {
            
              PromptParamScreen("[From 1 to 30s]\nEnter CB charge time in secons:",1);
            }

            else if (inputMsg [0] == '2') {

              PromptParamScreen("[From 5 to 150ms]\nEnter CB close delay in miliseconds:",2);
            }

            else if (inputMsg [0] == '3') {

              PromptParamScreen("[From 5 to 150ms]\nEnter CB open delay in miliseconds:",3); 
            }

            else if (inputMsg [0] == '4') {

              PromptParamScreen("[From 5 to 120 s]\nEnter disconnector open/close time in seconds:",4);
            }

            else{

              Serial.println();
              StartMenu();
              RcvCnt=0;
            } 

          } 

          else if (MenuDept ==1){

            switch (ParameterSelect)
            {
            case 1://CB Charge time
            //===========================================================
              
              inputMsg[RcvCnt]='\0';
              ParameterTemp = atoi(inputMsg); //Convert striong to digit
              
              if (ParameterValidation(ParameterTemp,30,1)){

                CB_chargeTime = 1000 * ParameterTemp;
                EEPROM.put(0, CB_chargeTime);
                ParamSuccess();

              }
              else{
                ParamError();
              }
              break;

            case 2://CB Close delay
            //===========================================================

              inputMsg[RcvCnt]='\0';
              ParameterTemp = atoi(inputMsg); //Convert striong to digit
              
              if (ParameterValidation(ParameterTemp,150,5)){

                CB_CloseDelay = ParameterTemp;
                EEPROM.put(20, CB_CloseDelay);
                ParamSuccess();
              }
              else{
                ParamError();
              }

              break;
              

            case 3://CB Open delay
            //============================================================
              
              inputMsg[RcvCnt]='\0';
              ParameterTemp = atoi(inputMsg); //Convert striong to digit
              
              if (ParameterValidation(ParameterTemp,150,5)){

                CB_OpenDelay = ParameterTemp;
                EEPROM.put(30, CB_OpenDelay);
                ParamSuccess();
              }
              else{
                ParamError();
              }
              break;

            case 4://Disconnector open/close time
            //============================================================
              inputMsg[RcvCnt]='\0';
              ParameterTemp = atoi(inputMsg); //Convert striong to digit
              
              if (ParameterValidation(ParameterTemp,120,5)){

                Disc_motionTime = 4 * ParameterTemp;
                EEPROM.put(10, Disc_motionTime);
                ParamSuccess();

              }
              else{
                ParamError();
              }
              
              break;

            default:
              break;

            }


          }

    }
  
  
  }//end of recive control

}//end of loop


//Custom function for communication
//---------------------------------------------------------


//Display main menu in putty
void StartMenu(){

  Serial.print("\33\143"); //clear putty screen
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

//Sub routine for prompt parameter
void PromptParamScreen(char* promptText, unsigned int parmNum){

   Serial.print("\33\143"); //clear putty screen
          Serial.println();
          Serial.print(promptText);   //Print on screen parameter prompt text
          RcvCnt=0;                   //prepare recive buffer for new string 
          MenuDept=1;                 //Choise second menu dept parameters
          ParameterSelect = parmNum; 

}

//Sub routine for validaiting the input
boolean ParameterValidation (uint32_t ParamValue,uint32_t ParamMax, uint32_t ParamMin){

  if (ParamValue < ParamMin || ParamValue > ParamMax){
      return false;
  }

  else{
      return true;
  }  
} 

//Sub routine for success parameter change
void ParamSuccess(){

  Serial.print("\33\143"); //clear putty screen
  Serial.println();
  Serial.print("Your parameter is changed press Enter to reboot");
  RcvCnt=0;
  MenuDept = 0;
  inputMsg [0] = '0';

}

void ParamError(){

  Serial.print("\33\143"); //clear putty screen
  Serial.println();
  Serial.print("Your input is out of range, Press Enter for main menu.");
  RcvCnt=0;
  MenuDept = 0;
  inputMsg [0] = '0';
}


