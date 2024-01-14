// __________________________________________________________________ FFAT_tools.h
#ifndef FFAT_TOOLS_h
#define FFAT_TOOLS_h
#include "Arduino.h" // _____________________________________________ You need to import Arduino.h to write a custom library
#include "FS.h"
#include "FFat.h"

class FFAT_Tools {
  private:
    char FFAT_infos[100];
  public:
    FFAT_Tools();
    void FFAT_info();
    char * get_FFAT_infos();
    void setup();
    void listDir(fs::FS &fs, const char * dirname, uint8_t levels);
    void createDir(fs::FS &fs, const char * path);
    void removeDir(fs::FS &fs, const char * path);
    bool existFile(fs::FS &fs, const char * path);
    void readFile(fs::FS &fs, const char * path);
    void writeFile(fs::FS &fs, const char * path, const char * message);
    void appendFile(fs::FS &fs, const char * path, const char * message);
    void renameFile(fs::FS &fs, const char * path1, const char * path2);
    void deleteFile(fs::FS &fs, const char * path);
    void testFileIO(fs::FS &fs, const char * path);
};

#endif