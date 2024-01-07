// esp32_IO.cpp
#include "esp32_IO.h"
#include "Arduino.h"
#include <time.h>

#include <Esp.h>
#include "esp_heap_caps.h" // not sure about this option:

static bool DIAG = true; //false; //true;

Esp32_IO::Esp32_IO() { // init for later use
 ;
}

void Esp32_IO::setup() {
  if ( DIAG ) { Serial.println("\n+++ Esp32_IO.setup()");}
}


void Esp32_IO::esp_info() {
  Serial.println("\n\n================================");
  Serial.printf("Chip Model: %s\n", ESP.getChipModel());
  Serial.printf("Chip Revision: %d\n", ESP.getChipRevision());
  Serial.printf("with %d core\n", ESP.getChipCores());
  Serial.printf("Flash Chip Size : %d \n", ESP.getFlashChipSize());
  Serial.printf("Flash Chip Speed : %d \n", ESP.getFlashChipSpeed());

  Serial.printf("Total Heap : %d \n",ESP.getHeapSize());
  Serial.printf("Free Heap  : %d \n",ESP.getFreeHeap());

  Serial.printf("Total PSRAM : %d \n",ESP.getPsramSize());
  Serial.printf("Free PSRAM  : %d \n",ESP.getFreePsram());

  Serial.printf("Used PSRAM : %d\n", ESP.getPsramSize() - ESP.getFreePsram());

  esp_chip_info_t chip_info;
  esp_chip_info(&chip_info);
  Serial.printf("\nFeatures included:\n %s\n %s\n %s\n %s\n %s\n",
      (chip_info.features & CHIP_FEATURE_EMB_FLASH) ? "embedded flash" : "",
      (chip_info.features & CHIP_FEATURE_WIFI_BGN) ? "2.4GHz WiFi" : "",
      (chip_info.features & CHIP_FEATURE_BLE) ? "Bluetooth LE" : "",
      (chip_info.features & CHIP_FEATURE_BT) ? "Bluetooth Classic" : "",
      (chip_info.features & CHIP_FEATURE_IEEE802154) ? "IEEE 802.15.4" : "");
  
  Serial.println();
}

void Esp32_IO::nows() { // ____________________________________________________ make / find timestring tnows
  time_t now = time(nullptr);
  now += TZs; // ____________________________________________________ add TZ, Time zone in Thailand (GMT+7) 
  struct tm ti;
  gmtime_r(&now, &ti);
  sprintf(tnows, "%d-%02d-%02d %02d:%02d:%02d",ti.tm_year+1900,ti.tm_mon+1, ti.tm_mday, ti.tm_hour, ti.tm_min, ti.tm_sec );
  //if ( DIAG ) { Serial.printf("\n+++ Esp32_IO.nows() : %s\n",tnows);}
  if ( DIAG ) { Serial.println(); }
}

char * Esp32_IO::get_tnows() {
  return tnows;
}

float Esp32_IO::in_pct(int ain) {
  float ainr = 100.0 * ain / 4095.0;
  return ainr;
}

float Esp32_IO::in_volt(int ain) {
  float ainr = 3.3 * ain / 4095.0;
  return ainr;
}

void Esp32_IO::Ains() {
  rec += 1;
  nows(); // ________________________________________________________ get timestamp, see tnows
  sensorA0val = analogRead(sensorA0);
  sensorA1val = analogRead(sensorA1);
  sensorA2val = analogRead(sensorA2);

  snprintf(A_Reads,100, ",%d,%s,%.2f,%.2f,%.2f,\n\r", rec,tnows,in_pct(sensorA0val), in_pct(sensorA1val), in_pct(sensorA2val) );

  if ( DIAG ) { Serial.print(A_Reads); }
}

char * Esp32_IO::get_A_Reads() {
  return A_Reads;
}
