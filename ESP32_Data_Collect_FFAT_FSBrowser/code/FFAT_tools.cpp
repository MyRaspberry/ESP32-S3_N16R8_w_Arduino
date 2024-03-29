// __________________________________________________________________ FFAT_tools.cpp
#include "FFAT_tools.h"
#include "Arduino.h"

#include "FS.h"
#include "FFat.h"

static bool DIAG = true; //false; //true;

FFAT_Tools::FFAT_Tools() { // init 
  ;
}

void FFAT_Tools::FFAT_info() {
  // char FFAT_info[100];
  snprintf(FFAT_infos,100,"ESP32 PSFLASH has FFAT : total %d , used %d , free %d \n", FFat.totalBytes(), FFat.usedBytes(), FFat.freeBytes()  );  // 
  //if ( DIAG ) { Serial.println(FFAT_infos); }
}

char * FFAT_Tools::get_FFAT_infos() {
  //if ( DIAG ) { Serial.println(" read FFAT drive again "); }
  FFAT_info();
  return FFAT_infos;
}

void FFAT_Tools::setup() {
  if ( DIAG ) { Serial.println("\n+++ FFAT_Tools.setup() ");}
  // ________________________________________________________________ FFAT
  // You only need to format FFat the first time

  #define FORMAT_FFAT true
  
  if(!FFat.begin()){
    Serial.println("\n!\n--- FFat Mount Failed\n!");
    if (FORMAT_FFAT) {
      Serial.println("+++ try to format FFat");
      FFat.format(); // ________________________________ STEP 1 needed only one time
    }
    FFat.begin(); // try again OR REBOOT NEEDE?
  }
  FFAT_info();
  Serial.println(FFAT_infos);
  

  listDir(FFat, "/", 1); // _________________________________________ level 1 shows also /data dir content
// OLD
  //writeFile(FFat, "/hello.txt", "Hello ");
  //appendFile(FFat, "/hello.txt", "World!\r\n");
  //readFile(FFat, "/hello.txt");
  //renameFile(FFat, "/hello.txt", "/foo.txt");
  //readFile(FFat, "/foo.txt");
  //deleteFile(FFat, "/foo.txt");
  
  //testFileIO(FFat, "/test.txt");
  //Serial.printf("Free space: %10u\n", FFat.freeBytes());
  //deleteFile(FFat, "/test.txt");
// end OLD

  //createDir(FFat, "/data"); // ______________________________________ STEP 2 only needed one time
  //removeDir(FFat, "/data");
  //deleteFile(FFat, "/data/readings.csv");
  //writeFile(FFat, "/data/readings.csv", ",rec,datetimes,A0,A1,A2,\n"); // STEP 3 create file with CSV header, only needed one time
  //readFile(FFat, "/data/readings.csv");
// end tests
  if ( existFile(FFat,"/data") ) {
    Serial.println("___ found /data dir");
  } else {
    createDir(FFat, "/data");
    Serial.println("___ made /data dir");
  }

}

// This file should be compiled with 'Partition Scheme' (in Tools menu)
// set to 'Default with ffat' if you have a 4MB ESP32 dev module or
// set to '16M Fat' if you have a 16MB ESP32 dev module.

void FFAT_Tools::listDir(fs::FS &fs, const char * dirname, uint8_t levels){
  Serial.printf("Listing directory: %s\r\n", dirname);

  File root = fs.open(dirname);
  if(!root){
    Serial.println("- failed to open directory");
    return;
  }
  if(!root.isDirectory()){
    Serial.println(" - not a directory");
    return;
  }

  File file = root.openNextFile();
  while(file){
    if(file.isDirectory()){
      Serial.print("  DIR : ");
        Serial.println(file.name());
        if(levels){
          listDir(fs, file.path(), levels -1);
        }
    } else {
      Serial.print(" name: ");
      Serial.print(file.name());
      Serial.print("\tsize: ");
      Serial.println(file.size());
    }
    file = root.openNextFile();
  }
}

void FFAT_Tools::createDir(fs::FS &fs, const char * path){
    if ( DIAG ) Serial.printf("Creating Dir: %s\n", path);
    if(fs.mkdir(path)){
        if ( DIAG ) Serial.println("Dir created");
    } else {
        Serial.println("mkdir failed");
    }
}

void FFAT_Tools::removeDir(fs::FS &fs, const char * path){
    if ( DIAG ) Serial.printf("Removing Dir: %s\n", path);
    if(fs.rmdir(path)){
        if ( DIAG ) Serial.println("Dir removed");
    } else {
        Serial.println("rmdir failed");
    }
}

bool FFAT_Tools::existFile(fs::FS &fs, const char * path){
  if ( DIAG ) Serial.printf("checking file: %s\r\n", path);
  return fs.exists(path);
}

void FFAT_Tools::readFile(fs::FS &fs, const char * path){
  Serial.printf("Reading file: %s\r\n", path);

  File file = fs.open(path);
  if(!file || file.isDirectory()){
    Serial.println("- failed to open file for reading");
    return;
  }

  if ( DIAG ) Serial.println("- read from file:");
  while(file.available()){
    Serial.write(file.read());
  }
  file.close();
}

void FFAT_Tools::writeFile(fs::FS &fs, const char * path, const char * message){
  if ( DIAG ) Serial.printf("Writing file: %s\r\n", path);

  File file = fs.open(path, FILE_WRITE);
  if(!file){
    Serial.println("- failed to open file for writing");
    return;
  }
  if(file.print(message)){
    if ( DIAG ) Serial.println("- file written");
  } else {
    Serial.println("- write failed");
  }
  file.close();
}

void FFAT_Tools::appendFile(fs::FS &fs, const char * path, const char * message){
  if ( DIAG ) Serial.printf("Appending to file: %s\r\n", path);

  File file = fs.open(path, FILE_APPEND);
  if(!file){
    Serial.println("- failed to open file for appending");
    return;
  }
  if(file.print(message)){
    if ( DIAG ) Serial.println("- message appended");
  } else {
    Serial.println("- append failed");
  }
  file.close();
}

void FFAT_Tools::renameFile(fs::FS &fs, const char * path1, const char * path2){
  if ( DIAG ) Serial.printf("Renaming file %s to %s\r\n", path1, path2);
  if (fs.rename(path1, path2)) {
    if ( DIAG ) Serial.println("- file renamed");
  } else {
    Serial.println("- rename failed");
  }
}

void FFAT_Tools::deleteFile(fs::FS &fs, const char * path){
  if ( DIAG ) Serial.printf("Deleting file: %s\r\n", path);
  if(fs.remove(path)){
    if ( DIAG ) Serial.println("- file deleted");
  } else {
    Serial.println("- delete failed");
  }
}

void FFAT_Tools::testFileIO(fs::FS &fs, const char * path){
  Serial.printf("Testing file I/O with %s\r\n", path);

  static uint8_t buf[512];
  size_t len = 0;
  File file = fs.open(path, FILE_WRITE);
  if(!file){
    Serial.println("- failed to open file for writing");
    return;
  }

  size_t i;
  Serial.print("- writing" );
  uint32_t start = millis();
  for(i=0; i<2048; i++){
    if ((i & 0x001F) == 0x001F){
      Serial.print(".");
    }
    file.write(buf, 512);
  }
  Serial.println("");
  uint32_t end = millis() - start;
  Serial.printf(" - %u bytes written in %u ms\r\n", 2048 * 512, end);
  file.close();

  file = fs.open(path);
  start = millis();
  end = start;
  i = 0;
  if(file && !file.isDirectory()){
    len = file.size();
    size_t flen = len;
      start = millis();
      Serial.print("- reading" );
      while(len){
        size_t toRead = len;
        if(toRead > 512){
          toRead = 512;
        }
        file.read(buf, toRead);
        if ((i++ & 0x001F) == 0x001F){
          Serial.print(".");
        }
        len -= toRead;
      }
      Serial.println("");
      end = millis() - start;
      Serial.printf("- %u bytes read in %u ms\r\n", flen, end);
      file.close();
  } else {
    Serial.println("- failed to open file for reading");
  }
}

