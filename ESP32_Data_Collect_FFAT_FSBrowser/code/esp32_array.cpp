// esp32_array.cpp
#include "esp32_array.h"
#include "Arduino.h" // _____________________________________________ You need to import Arduino.h to write a custom library

// https://www.upesy.com/blogs/tutorials/get-more-ram-on-esp32-with-psram#

#include <Esp.h>
#include "esp_heap_caps.h"
const int limit = 10000; // heap_caps_malloc_extmem_enable(limit);   // use in Esp32_Array::setup() // seems only related to libraries??

static bool DIAG = false; // false; // true;

Esp32_Array::Esp32_Array() { // init
 ;
}

void Esp32_Array::logMemory(String txt) {
  Serial.println(txt);
  Serial.printf("___ PSRam : %d, Free : %d, Used : %d\n", ESP.getPsramSize(), ESP.getFreePsram(), ESP.getPsramSize() - ESP.getFreePsram());
}

void Esp32_Array::setup() {
  #include "settings.h"
  thisTZs = LOCALTZs;

  if ( DIAG ) { Serial.println("\n+++ Esp32_Array.setup()");}
  if(psramInit()){
    Serial.println("\n+++ PSRAM is correctly initialized");
  }else{
    Serial.println("\n! PSRAM does not work !");
  }
  heap_caps_malloc_extmem_enable(limit);
  logMemory("after init"); 
  // change malloc to calloc ( aka init with 0 )
  //count_array = (int *) ps_malloc(recMax * sizeof(int));
  count_array = (int *) ps_calloc(recMax, sizeof(int));
  datetime_array = (time_t *) ps_calloc(recMax, sizeof(time_t));
  A0pct_array = (float *) ps_calloc(recMax, sizeof(float));
  A1pct_array = (float *) ps_calloc(recMax, sizeof(float));
  A2pct_array = (float *) ps_calloc(recMax, sizeof(float));

  logMemory("after setup() arrays ");

}

void Esp32_Array::nows() { // ____________________________________________________ make / find timestring tnows
  time_t now = time(nullptr);
  now += thisTZs;
  struct tm ti;
  gmtime_r(&now, &ti);
  sprintf(tnows, "%d-%02d-%02d %02d:%02d:%02d",ti.tm_year+1900,ti.tm_mon+1, ti.tm_mday, ti.tm_hour, ti.tm_min, ti.tm_sec );
  //if ( DIAG ) { Serial.printf("\n+++ Esp32_IO.nows() : %s\n",tnows);}
}

char * Esp32_Array::get_tnows() {
  return tnows;
}


float Esp32_Array::in_pct(int ain) {
  float ainr = 100.0 * ain / 4095.0;
  return ainr;
}

float Esp32_Array::in_volt(int ain) {
  float ainr = 3.3 * ain / 4095.0;
  return ainr;
}

void Esp32_Array::Ains() {

  nows(); // makes tnows for print
  
  sensorA0val = analogRead(sensorA0);
  sensorA1val = analogRead(sensorA1);
  sensorA2val = analogRead(sensorA2);

  count = rec;
  datetime = time(nullptr);
  A0pct = in_pct(sensorA0val);
  A1pct = in_pct(sensorA1val);
  A2pct = in_pct(sensorA2val);
  if ( DIAG ) { 
    snprintf(A_Reads,100, ",%d,%s,%.2f,%.2f,%.2f,\n", count,tnows,A0pct,A1pct,A2pct );
    Serial.println(A_Reads); 
  } //else {
  //  Serial.print("r"); // to check if there is a problem
  //}

  // save to PSRAM arrays
  count_array[rec]=rec;
  datetime_array[rec]=datetime;
  A0pct_array[rec]=A0pct;
  A1pct_array[rec]=A1pct;
  A2pct_array[rec]=A2pct;

  rec += 1;
  if ( rec >= recMax ) { rec = 0; } // next hour start again.. actually it should start with begin of hour !!

}

void Esp32_Array::job1min() {
  Serial.println("___ start adata.job1min() ");
  float A0_AVG =0.0;
  float A1_AVG =0.0;
  float A2_AVG =0.0;
  float A0_MIN =101.0;
  float A1_MIN =101.0;
  float A2_MIN =101.0;
  float A0_MAX =-1.0;
  float A1_MAX =-1.0;
  float A2_MAX =-1.0;

  int rpoint = rec; // last written to array is -1 of it
  float val=0;

  for (int r=0; r < 60;r++) {
    rpoint -= 1;
    if ( rpoint >= recMax ) { rpoint -= recMax; }
    if ( rpoint < 0 ) { rpoint += recMax; }
    if ( ( r == 0 ) or ( r == 59 ) ) { 
      Serial.printf("r : %d, rpoint : %d \n",r,rpoint);
    }

    val = A0pct_array[rpoint];
    A0_AVG += val;
    if ( A0_MIN > val ) { A0_MIN = val; }
    if ( A0_MAX < val ) { A0_MAX = val; }

    val = A1pct_array[rpoint];
    A1_AVG += val;
    if ( A1_MIN > val ) { A1_MIN = val; }
    if ( A1_MAX < val ) { A1_MAX = val; }

    val = A2pct_array[rpoint];
    A2_AVG += val;
    if ( A2_MIN > val ) { A2_MIN = val; }
    if ( A2_MAX < val ) { A2_MAX = val; }
  }
  A0_AVG /= 60.0;
  A1_AVG /= 60.0;
  A2_AVG /= 60.0;
  char minline[100];
  DYN_PSRAM1min = "";
  snprintf(minline,100,"___ 1min A0 avg : %.2f, min : %.2f, max : %.2f \n",A0_AVG,A0_MIN, A0_MAX);
  DYN_PSRAM1min += String(minline);
  snprintf(minline,100,"___ 1min A1 avg : %.2f, min : %.2f, max : %.2f \n",A1_AVG,A1_MIN, A1_MAX);
  DYN_PSRAM1min += String(minline);
  snprintf(minline,100,"___ 1min A2 avg : %.2f, min : %.2f, max : %.2f \n",A2_AVG,A2_MIN, A2_MAX);
  DYN_PSRAM1min += String(minline);
  Serial.println( DYN_PSRAM1min );
}

String Esp32_Array::get_PSRAM1min() {
  return DYN_PSRAM1min;
}
