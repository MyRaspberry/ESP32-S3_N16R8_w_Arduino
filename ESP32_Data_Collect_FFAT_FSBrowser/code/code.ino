/* this is /ESP32_Data_Collect_FFAT_FSBrowser/code/code.ino
*  ESP32-S3 N16R8 : my board : YD-ESP32-S3
*  settings:
*  tools : board : esp32s3 Dev Module
*  tools : Flash size : 16MB
*  tools : Partition Scheme : 16MB ( 3MB APP, 10MB FATFS )
*  tools : PSRAM : OPI PSRAM
*/

#include "board_RGB_LED.h"
Board_RGB_LED rainbow;

#include "esp32_IO.h"
Esp32_IO esp_io;

#include "esp32_array.h"
Esp32_Array adata;

#include "FFAT_tools.h"
FFAT_Tools esp_drive;

void setup(void){ 
  delay(500);
  Serial.begin(115200);
  delay(500);
  rainbow.setup();
  esp_io.setup();
  adata.setup(); // use PSRAM arrays 1 h of 1 sec readings
  Serial.println( esp_io.esp_info() ); // ___________________________ CHIP INFO
  esp_drive.setup(); // _____________________________________________ FFAT check file /data/readings.csv
  wifi_setup(); // __________________________________________________ wifi.no +++ web server with filesystem access
  Serial.println( "\n___ SETUP  complete" );
}

void Job_1sec(){ // _________________________________________________ 1 sec jobs
  //rainbow.run(); // _________________________________________________ board_RGB_LED change color // OFF as LED makes Ain BAD / board issue ?
  wifi_run();
  adata.Ains();
}

void Job_Nsec() { // ________________________________________________ and do some analog reading
  rainbow.turnOFF(); // _____________________________________________ board RGB LED OFF for sampling, 1 sec Job ON  rainbow.run() 
  esp_io.Ains(); // _________________________________________________ do the readings
}

char isFname[35]; // = "/data/readings.csv";
char newFname[35];

void Job_1min(){
  esp_io.nows();
  Serial.print("\n___ loop1min ");
  Serial.println(esp_io.get_tnows()); // set by nows();

  strncpy(newFname,esp_io.get_Fname(),35);

  if ( strncmp(isFname,newFname,35) == 0 ) { // same
    esp_drive.appendFile(FFat, isFname, esp_io.get_A_Reads() ); // _ last 15 sec reading stored as string
  } else {
    Serial.print("___ old name is ");
    Serial.print(isFname);
    Serial.print(" +++ want write to ");
    Serial.print(newFname);
    strncpy(isFname,newFname,35);
    Serial.print(" +++ now use ");
    Serial.println(isFname);
    // after boot that would erase a existing file, so check first with my new existFile function
    if ( !esp_drive.existFile(FFat, isFname) ) {
      esp_drive.writeFile(FFat, isFname, ",rec,datetimes,A0,A1,A2,\n"); // STEP 3 create file with CSV header, only needed one time per day
      delay(500);
    }
    esp_drive.appendFile(FFat, isFname, esp_io.get_A_Reads() ); // _ last 15 sec reading stored as CSV string
    //Serial.print("___ create and write and append to it ");
  }
  adata.job1min();  // print 1min AVG MIN MAX of 1sev values of PSRAM arrays of 1h long 

}

void loop(void) { // ________________________________________________ see code_loop.ino
  loopC();
  loopNsec();
  loop1min();
  loopM();
}
