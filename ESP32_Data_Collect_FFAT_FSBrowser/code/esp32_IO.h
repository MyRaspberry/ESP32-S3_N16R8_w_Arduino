#ifndef ESP32_IO_h
#define ESP32_IO_h
#include "Arduino.h" // _____________________________________________ You need to import Arduino.h to write a custom library

#define A0pin 1 // __________ GPIO1 ADC1_0 ( here : pin 4 right )
#define A1pin 2 // __________ GPIO2 ADC1_1 ( here : pin 5 right )
#define A2pin 3 // __________ GPIO3 ADC1_2 ( here : pin 13 left )

class Esp32_IO {
  private:
    int rec = 0;
    int thisTZs = 0; // see setup
    char tnows[40]; // __________________________________________________ use : strcpy(Tnows,Esp32_IO.nows() );
    String espInfo = "";
    // ______________________________________________________________ get analog in data
    int sensorA0 = A0pin;   // select the input pin for the potentiometer
    int sensorA0val = 0;  // variable to store the value coming from the sensor
    int sensorA1 = A1pin;   // select the input pin for the potentiometer
    int sensorA1val = 0;  // variable to store the value coming from the sensor
    int sensorA2 = A2pin;   // select the input pin for the potentiometer
    int sensorA2val = 0;  // variable to store the value coming from the sensor
    char A_Reads[100]; // ___________________________________________ filled by call Esp32_IO.Ains()
    char WantedFileName[35]; // _____________________________________ filled by call Esp32_IO.Ains()

  public:
    Esp32_IO();
    void setup();
    String esp_info();

    void nows();
    char * get_tnows();
    char * get_Fname();

    void Ains();
    char * get_A_Reads();

    float in_volt(int ain);
    float in_pct(int ain);

};

#endif