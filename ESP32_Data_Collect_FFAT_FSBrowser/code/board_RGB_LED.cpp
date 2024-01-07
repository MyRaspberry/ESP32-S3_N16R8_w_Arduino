// board_RGB_LED.cpp
#include "board_RGB_LED.h"
#include "Arduino.h"

#include <Esp.h>
#define RGBLED_PIN 48
#define MAX_BRIGHT 50  // 0 .. 255

static bool DIAG = false; //true;

Board_RGB_LED::Board_RGB_LED() { // init for later use : Board_RGB_LED.run();
  cR=MAX_BRIGHT;
  dcR = 5;
  cG=MAX_BRIGHT;
  dcG = 6;
  cB=MAX_BRIGHT;
  dcB = -7;
}

void Board_RGB_LED::setup() {
  ledState = true;
  neopixelWrite(RGBLED_PIN,0,0,MAX_BRIGHT); // color blue
  if ( DIAG ) { Serial.println("\n+++ Board_RGB_LED.setup() : RGB blue ");}
}

void Board_RGB_LED::turnON() {
  ledState = true;
  neopixelWrite(RGBLED_PIN,MAX_BRIGHT,MAX_BRIGHT,MAX_BRIGHT); // color white
  if ( DIAG ) { Serial.println("+++ RGB white ");}
}

void Board_RGB_LED::turnOFF() {
  ledState = false;
  neopixelWrite(RGBLED_PIN,0,0,0); // color black
  if ( DIAG ) { Serial.println("+++ RGB OFF ");}
}

void Board_RGB_LED::toggle() {
  if ( ledState ) {
    neopixelWrite(RGBLED_PIN,0,0,0); // color black
    ledState = false;
    if ( DIAG ) { Serial.println("+++ RGB OFF ");}
  } else {
    neopixelWrite(RGBLED_PIN,MAX_BRIGHT,MAX_BRIGHT,MAX_BRIGHT); // color white
    ledState = true;
    if ( DIAG ) { Serial.println("+++ RGB ON ");}
  }
}

void Board_RGB_LED::red() {
  ledState = true;
  neopixelWrite(RGBLED_PIN,MAX_BRIGHT,0,0); // color red
  if ( DIAG ) { Serial.println("+++ RGB red ");}
}

void Board_RGB_LED::green() {
  ledState = true;
  neopixelWrite(RGBLED_PIN,0,MAX_BRIGHT,0); // color green
  if ( DIAG ) { Serial.println("+++ RGB green ");}
}

void Board_RGB_LED::blue() {
  ledState = true;
  neopixelWrite(RGBLED_PIN,0,0,MAX_BRIGHT); // color blue
  if ( DIAG ) { Serial.println("+++ RGB blue ");}
}

 void Board_RGB_LED::blink() {
  blue();
  delay(300);
  green();
  delay(300);
  red();  
  delay(300);
 }

 void Board_RGB_LED::run() {
  cR += dcR;
  if ( cR > MAX_BRIGHT ) {
    cR = MAX_BRIGHT;
    dcR = -dcR;
  } 
  if ( cR < 0 ) {
    cR = 0;
    dcR = -dcR;
  }

  cG += dcG;
  if ( cG > MAX_BRIGHT ) {
    cG = MAX_BRIGHT;
    dcG = -dcG;
  } 
  if ( cG < 0 ) {
    cG = 0;
    dcG = -dcG;
  }

  cB += dcB;
  if ( cB > MAX_BRIGHT ) {
    cB = MAX_BRIGHT;
    dcB = -dcB;
  } 
  if ( cB < 0 ) {
    cB = 0;
    dcB = -dcB;
  }
  ledState = true;
  neopixelWrite(RGBLED_PIN,cR,cG,cB);
  if ( DIAG ) { Serial.printf("+++ RGB %d : %d : %d \n",cR,cG,cB);}
 }

bool Board_RGB_LED::getState() {
  return ledState;
}