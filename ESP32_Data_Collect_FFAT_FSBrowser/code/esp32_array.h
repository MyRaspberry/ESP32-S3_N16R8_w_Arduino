#ifndef ESP32_ARRAY_h
#define ESP32_ARRAY_h
#include "Arduino.h" // _____ You need to import Arduino.h to write a custom library

#define A0pin 1 // __________ GPIO1 ADC1_0 ( here : pin 4 right )
#define A1pin 2 // __________ GPIO2 ADC1_1 ( here : pin 5 right )
#define A2pin 3 // __________ GPIO3 ADC1_2 ( here : pin 13 left )
#define RECMAX 3600

class Esp32_Array {
  private:
    int rec = 0;
    int sensorA0 = A0pin;   // select the input pin for the potentiometer
    int sensorA0val = 0;  // variable to store the value coming from the sensor
    int sensorA1 = A1pin;   // select the input pin for the potentiometer
    int sensorA1val = 0;  // variable to store the value coming from the sensor
    int sensorA2 = A2pin;   // select the input pin for the potentiometer
    int sensorA2val = 0;  // variable to store the value coming from the sensor
    int thisTZs = 0; // see setup
    char tnows[40]; // ______________________________________________ use : strcpy(Tnows,Esp32_Array.nows() );
    char A_Reads[100]; // ___________________________________________ filled by call Esp32_Array.Ains() for print only
    String DYN_PSRAM1min = "";
    int count;
    time_t datetime; // arduino that is a unsigned long
    float A0pct;
    float A1pct;
    float A2pct;

    const int recMax = RECMAX; // 3600;
    int *count_array; // = (int *) ps_malloc(recMax * sizeof(int)); // must do int setup!!!
    time_t *datetime_array;
    float *A0pct_array;
    float *A1pct_array;
    float *A2pct_array;

  public:
    Esp32_Array();
    void logMemory(String txt);
    void setup();
    void nows();
    char * get_tnows();
    void Ains();
    float in_volt(int ain);
    float in_pct(int ain);
    void job1min();
    String get_PSRAM1min();


};


#endif