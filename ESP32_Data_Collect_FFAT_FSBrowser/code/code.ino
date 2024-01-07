// this is /ESP32_Data_Collect_FFAT_FSBrowser/code/code.ino
// ESP32-S3 N16R8 : my board : YD-ESP32-S3
// settings:
// tools : board : esp32s3 Dev Module
// tools : Flash size : 16MB
// tools : Partition Scheme : 16MB ( 3MB APP, 10MB FATFS )
// tools : PSRAM : OPI PSRAM
#include "board_RGB_LED.h" // my Class / lib
Board_RGB_LED rainbow;
#include "esp32_IO.h" // my Class / lib
Esp32_IO esp_io;
#include "FFAT_tools.h" // my Class / lib
FFAT_Tools esp_drive;

void setup(void){ 
  delay(500);
  Serial.begin(115200);
  delay(500);
  rainbow.setup();
  esp_io.setup();
  esp_drive.setup(); // _____________________________________________ FFAT check file /data/readings.csv
  esp_io.esp_info(); // _____________________________________________ CHIP INFO
  wifi_setup(); // __________________________________________________ wifi web server with filesystem access
  Serial.println( "\nSETUP  complete" );
}

void Job_1sec(){ // _________________________________________________ 1 sec jobs
  rainbow.run(); // _________________________________________________ board_RGB_LED change color
  wifi_run();
}

void Job_Nsec() { // ________________________________________________ and do some analog reading
  rainbow.turnOFF(); // _____________________________________________ board RGB LED OFF for sampling, 1 sec Job ON  rainbow.run() 
  esp_io.Ains(); // _________________________________________________ do the readings
}

void Job_1min(){
  esp_io.nows();
  Serial.print("\n___ loop1min ");
  Serial.println(esp_io.get_tnows()); // set by nows();
  esp_drive.appendFile(FFat, "/data/readings.csv", esp_io.get_A_Reads() ); // _ last 15 sec reading stored as string
}

void loop(void) { // ________________________________________________ see code_loop.ino
  loopC();
  loopNsec();
  loop1min();
  loopM();
}
