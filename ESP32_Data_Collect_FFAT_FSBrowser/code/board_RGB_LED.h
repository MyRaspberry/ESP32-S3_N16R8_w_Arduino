// __________________________________________________________________ board_RGB_LED.h
#ifndef BOARD_RGB_LED_h
#define BOARD_RGB_LED_h
#include "Arduino.h" // _____________________________________________ You need to import Arduino.h to write a custom library

class Board_RGB_LED {
  private:
    bool ledState=false;
    int cR=0;
    int cG=0;
    int cB=0;
    int dcR=0;
    int dcG=0;
    int dcB=0;

  public:
    Board_RGB_LED();
    void setup();
    void turnON();
    void turnOFF();
    void toggle();
    void red();
    void green();
    void blue();
    void blink();
    void run();
    bool getState();
};

#endif