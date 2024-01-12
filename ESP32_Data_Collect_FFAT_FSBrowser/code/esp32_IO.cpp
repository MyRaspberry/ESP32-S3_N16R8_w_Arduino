// esp32_IO.cpp
#include "esp32_IO.h"
#include "Arduino.h"
#include <time.h>

#include <Esp.h>
#include "esp_heap_caps.h"
// also check on FFAT installed / usage (10MB of PSFLASH 16MB)
#include "FS.h"
#include "FFat.h"

static bool DIAG = true; //false; //true;

Esp32_IO::Esp32_IO() { // init for later use
 ;
}

void Esp32_IO::setup() {
  if ( DIAG ) { Serial.println("\n+++ Esp32_IO.setup()");}
  #include "settings.h"
  thisTZs = LOCALTZs;
}


String Esp32_IO::esp_info() {
  static int BIGline = 200; 
  char infoline[BIGline];
  espInfo  = "\n================================\n";
  snprintf(infoline,BIGline,"Chip Model: %s\n", ESP.getChipModel() );
  espInfo += String( infoline );
  snprintf(infoline,BIGline,"Chip Revision: %d\n", ESP.getChipRevision());
  espInfo += String( infoline );

  snprintf(infoline,BIGline,"with %d core\n", ESP.getChipCores());
  espInfo += String( infoline );
  snprintf(infoline,BIGline,"Flash Chip Size : %d \n", ESP.getFlashChipSize());
  espInfo += String( infoline );
  snprintf(infoline,BIGline,"Flash Chip Speed : %d \n", ESP.getFlashChipSpeed());
  espInfo += String( infoline );


  snprintf(infoline,BIGline,"RAM :");
  espInfo += String( infoline );
  snprintf(infoline,BIGline,"Total Heap : %d \n",ESP.getHeapSize());
  espInfo += String( infoline );
  snprintf(infoline,BIGline,"Free Heap  : %d \n",ESP.getFreeHeap());
  espInfo += String( infoline );

  snprintf(infoline,BIGline,"Total PSRAM : %d \n",ESP.getPsramSize());
  espInfo += String( infoline );
  snprintf(infoline,BIGline,"Free PSRAM  : %d \n",ESP.getFreePsram());
  espInfo += String( infoline );
  snprintf(infoline,BIGline,"Used PSRAM : %d\n", ESP.getPsramSize() - ESP.getFreePsram());
  espInfo += String( infoline );

  esp_chip_info_t chip_info;
  esp_chip_info(&chip_info);
  snprintf(infoline,BIGline,"\nFeatures included:\n %s\n %s\n %s\n %s\n %s\n",
      (chip_info.features & CHIP_FEATURE_EMB_FLASH) ? "embedded flash" : "",
      (chip_info.features & CHIP_FEATURE_WIFI_BGN) ? "2.4GHz WiFi" : "",
      (chip_info.features & CHIP_FEATURE_BLE) ? "Bluetooth LE" : "",
      (chip_info.features & CHIP_FEATURE_BT) ? "Bluetooth Classic" : "",
      (chip_info.features & CHIP_FEATURE_IEEE802154) ? "IEEE 802.15.4" : "") ;
  espInfo += String( infoline );
  espInfo += "\n================================\n";
 
  if ( DIAG ) { Serial.println("___ esp_info :"); }
  return espInfo;

}

void Esp32_IO::nows() { // ____________________________________________________ make / find timestring tnows
  time_t now = time(nullptr);
  now += thisTZs;
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
  time_t now = time(nullptr);
  now += thisTZs;
  struct tm ti;
  gmtime_r(&now, &ti);
  sprintf(tnows, "%d-%02d-%02d %02d:%02d:%02d",ti.tm_year+1900,ti.tm_mon+1, ti.tm_mday, ti.tm_hour, ti.tm_min, ti.tm_sec );
  snprintf(WantedFileName,35, "/data/readings_%d-%02d-%02d.csv",ti.tm_year+1900,ti.tm_mon+1, ti.tm_mday);

  sensorA0val = analogRead(sensorA0);
  sensorA1val = analogRead(sensorA1);
  sensorA2val = analogRead(sensorA2);

  snprintf(A_Reads,100, ",%d,%s,%.2f,%.2f,%.2f,\n", rec,tnows,in_pct(sensorA0val), in_pct(sensorA1val), in_pct(sensorA2val) );
  if ( DIAG ) { Serial.println(); Serial.print(A_Reads); }
}

char * Esp32_IO::get_A_Reads() {
  return A_Reads;
}

char * Esp32_IO::get_Fname() {
  return WantedFileName;
}
