/*
  CB Simulator
*/
#include <Arduino.h>
#include <LiquidCrystal.h>
//#include <EEPROM.h>
#include <USBAPI.h>

class CircuitBraker {

char State;
int  MotorTime;


};

// the setup function runs once when you press reset or power the board
void setup() {
  // initialize digital pin LED_BUILTIN as an output.
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(PIN_A0,OUTPUT);
  pinMode(PIN_A1,OUTPUT);
}

// the loop function runs over and over again forever
void loop() {
  digitalWrite(LED_BUILTIN, HIGH);  // turn the LED on (HIGH is the voltage level)
  digitalWrite(PIN_A0,HIGH);
  delay(500);                       // wait for a second
  digitalWrite(LED_BUILTIN, LOW);    // turn the LED off by making the voltage LOW
  digitalWrite(PIN_A0,LOW);
  delay(500);                       // wait for a second
}
