// wifi.ino use example FSBrowser
#include "FS.h"
#include "FFat.h"
#define FILESYSTEM FFat

#include <time.h> 
#include <WiFi.h>
#include <WiFiClient.h>
#include <WebServer.h>
#include <ESPmDNS.h>

#define BIGbuf 5000
char DYN_HTML[BIGbuf];
String DYN_HTMLs = " DYN_HTMLs "; // try for creation of HTML a string type, only before send convert back to char
String DYN_DIRS = " ";
// as can not append arrays in Cpp set
#define maxDirsFiles 100
String Flist[maxDirsFiles][4] = {{"type","name","size","date"}};
int Flistcount = 0;

#include "settings.h" // ______________________________________ here find the router login SSID PASSWORD ...
const char *ssid = STASSID;
const char *password = STAPSK;
const char *rev = REV;
const int port = PORT;
const char* host = HOST;

IPAddress thisip(FIXIP); // ___________________________________ fix IP for webserver

// Set your Gateway IP address
IPAddress gateway(192, 168, 1, 1);
IPAddress subnet(255, 255, 0, 0);
IPAddress primaryDNS(8, 8, 8, 8);   //optional
IPAddress secondaryDNS(8, 8, 4, 4); //optional

WebServer server(port);

File fsUploadFile; //holds the current upload

//format bytes
String formatBytes(size_t bytes) {
  if (bytes < 1024) {
    return String(bytes) + "B";
  } else if (bytes < (1024 * 1024)) {
    return String(bytes / 1024.0) + "kB";
  } else if (bytes < (1024 * 1024 * 1024)) {
    return String(bytes / 1024.0 / 1024.0) + "MB";
  } else {
    return String(bytes / 1024.0 / 1024.0 / 1024.0) + "GB";
  }
}

String getContentType(String filename) {
  if (server.hasArg("download")) {
    return "application/octet-stream";
  } else if (filename.endsWith(".htm")) {
    return "text/html";
  } else if (filename.endsWith(".html")) {
    return "text/html";
  } else if (filename.endsWith(".css")) {
    return "text/css";
  } else if (filename.endsWith(".js")) {
    return "application/javascript";
  } else if (filename.endsWith(".png")) {
    return "image/png";
  } else if (filename.endsWith(".gif")) {
    return "image/gif";
  } else if (filename.endsWith(".jpg")) {
    return "image/jpeg";
  } else if (filename.endsWith(".ico")) {
    return "image/x-icon";
  } else if (filename.endsWith(".xml")) {
    return "text/xml";
  } else if (filename.endsWith(".pdf")) {
    return "application/x-pdf";
  } else if (filename.endsWith(".zip")) {
    return "application/x-zip";
  } else if (filename.endsWith(".gz")) {
    return "application/x-gzip";
  }
  return "text/plain";
}

bool exists(String path){
  bool yes = false;
  File file = FILESYSTEM.open(path, "r");
  if(!file.isDirectory()){
    yes = true;
  }
  file.close();
  return yes;
}

bool handleFileRead(String path) {
  Serial.println("handleFileRead: " + path);
  if (path.endsWith("/")) {
    path += "index.htm";
  }
  String contentType = getContentType(path);
  String pathWithGz = path + ".gz";
  if (exists(pathWithGz) || exists(path)) {
    if (exists(pathWithGz)) {
      path += ".gz";
    }
    File file = FILESYSTEM.open(path, "r");
    server.streamFile(file, contentType);
    file.close();
    return true;
  }
  return false;
}

void handleFileUpload() {
  if (server.uri() != "/edit") {
    return;
  }
  HTTPUpload& upload = server.upload();
  if (upload.status == UPLOAD_FILE_START) {
    String filename = upload.filename;
    if (!filename.startsWith("/")) {
      filename = "/" + filename;
    }
    Serial.print("handleFileUpload Name: "); Serial.println(filename);
    fsUploadFile = FILESYSTEM.open(filename, "w");
    filename = String();
  } else if (upload.status == UPLOAD_FILE_WRITE) {
    //Serial.print("handleFileUpload Data: "); Serial.println(upload.currentSize);
    if (fsUploadFile) {
      fsUploadFile.write(upload.buf, upload.currentSize);
    }
  } else if (upload.status == UPLOAD_FILE_END) {
    if (fsUploadFile) {
      fsUploadFile.close();
    }
    Serial.print("handleFileUpload Size: "); Serial.println(upload.totalSize);
  }
}

void handleFileDelete() {
  if (server.args() == 0) {
    return server.send(500, "text/plain", "BAD ARGS");
  }
  String path = server.arg(0);
  Serial.println("handleFileDelete: " + path);
  if (path == "/") {
    return server.send(500, "text/plain", "BAD PATH");
  }
  if (!exists(path)) {
    return server.send(404, "text/plain", "FileNotFound");
  }
  FILESYSTEM.remove(path);
  server.send(200, "text/plain", "");
  path = String();
}

void handleFileCreate() {
  if (server.args() == 0) {
    return server.send(500, "text/plain", "BAD ARGS");
  }
  String path = server.arg(0);
  Serial.println("handleFileCreate: " + path);
  if (path == "/") {
    return server.send(500, "text/plain", "BAD PATH");
  }
  if (exists(path)) {
    return server.send(500, "text/plain", "FILE EXISTS");
  }
  File file = FILESYSTEM.open(path, "w");
  if (file) {
    file.close();
  } else {
    return server.send(500, "text/plain", "CREATE FAILED");
  }
  server.send(200, "text/plain", "");
  path = String();
}

void handleFileList() {
  if (!server.hasArg("dir")) {
    server.send(500, "text/plain", "BAD ARGS");
    return;
  }
  DYN_DIRS = "";
  String path = server.arg("dir");
  Serial.println("handleFileList: " + path);
  Flistcount = 0;
  File root = FILESYSTEM.open(path);
  path = String();

  DYN_DIRS = "[";
  if(root.isDirectory()){
    File file = root.openNextFile();
    while(file){
      if (DYN_DIRS != "[") {
        DYN_DIRS += ",";
      }
      DYN_DIRS += "\n{\"type\":\"";
      DYN_DIRS += (file.isDirectory()) ? "dir" : "file";
      Flistcount +=1;
      Flist[Flistcount][0] = (file.isDirectory()) ? "dir" : "file";
      DYN_DIRS += "\",\"name\":\"";
      DYN_DIRS += String(file.path()).substring(1);
      Flist[Flistcount][1] = String(file.path()).substring(1);
      if ( !file.isDirectory() ) {
        DYN_DIRS += "\", \"size\": "; // KLL add 
        DYN_DIRS += formatBytes(file.size());    // String(file.size());
        Flist[Flistcount][2] = formatBytes(file.size()); 
        //DYN_DIRS += " }"; // ______________________________________ new try include filedate
        DYN_DIRS += " , \"date\": ";
        time_t filedate = file.getLastWrite();
        filedate += TZs;
        struct tm ti;
        gmtime_r(&filedate, &ti);
        char tlast[40];
        sprintf(tlast, "%d-%02d-%02d %02d:%02d:%02d",ti.tm_year+1900,ti.tm_mon+1, ti.tm_mday, ti.tm_hour, ti.tm_min, ti.tm_sec );
        DYN_DIRS += String(tlast);
        Flist[Flistcount][3] = String(tlast);
        DYN_DIRS += " }";
      } else {
        DYN_DIRS += "\" }";
      }
      file = root.openNextFile();
    }
  }
  DYN_DIRS += "\n]";
  server.send(200, "text/json", DYN_DIRS );
  for ( int r = 0; r <= Flistcount; r++) {
    for ( int c = 0; c < 4; c++ ) {
      Serial.print(Flist[r][c]);
      Serial.print(",");
    }
    Serial.println();
  }
}

void handleFilePrint() { // on /print?file=/data/readings.csv
  if (!server.hasArg("file")) {
    server.send(500, "text/plain", "BAD ARGS");
    return;
  }
  String path = server.arg("file");
  Serial.println("\nhandleFile: " + path);

  String contentType = getContentType(path);
  File file = FILESYSTEM.open(path, "r");
  server.streamFile(file, contentType);
  file.close();

}

void handleFileDownload() { // on /download?file=/data/readings.csv
  if (!server.hasArg("file")) {
    server.send(500, "text/plain", "BAD ARGS");
    return;
  }
  String path = server.arg("file");
  Serial.println("\nhandleFileDownload: " + path);

  String contentType = getContentType(path);
  File file = FILESYSTEM.open(path, "r");
  server.streamFile(file, contentType);
  file.close();

}

void handleFavicon() {
  String icoPath = "/favicon.ico";
  Serial.println("try serving uploaded favicon.ico");
  String contentType = getContentType(icoPath);
  File file = FILESYSTEM.open(icoPath, "r");
  server.streamFile(file, contentType);
  file.close();
}

void handleRoot() {
  Serial.println();
  Serial.print("www index.html served");
  // uptime char
  int sec = millis() / 1000;
  int min = sec / 60;
  int hr = min / 60;
  char uptimec[100];
  snprintf(uptimec,100,"%02d:%02d:%02d",hr, min % 60, sec % 60);

  esp_io.nows(); // and use global Tnows STRING

  DYN_HTMLs  = "<!DOCTYPE html><html><head>\
  <meta http-equiv='refresh' content='15'/>\
  <title>KLL engineering</title>\
  <link rel=\"icon\" type=\"image/x-icon\" href=\"favicon.ico\">\
  <style>\
  body {font-family: \"Times New Roman\", Times, serif; background-color: lightgreen;\
  display:inline-block; margin: 0px auto; text-align: center;}\
  h1 {color: deeppink; word-wrap: break-word; padding: 1vh; font-size: 30px;}\
  p {font-size: 1.5rem; word-wrap: break-word;}\
  p.dotted {margin: auto; width: 75%; font-size: 25px; text-align: center;}\
  form {font-size: 1.5rem; }\
  input[type=number] {font-size: 2rem;}\
  table { width: 100%; }\
  </style>\
  </head>\
  <body>\
  <h1>ESP32-S3 Webserver</h1> <h3>via Arduino IDE 2.2.1</h3>\
  <img src='https://docs.espressif.com/projects/esp-idf/en/latest/esp32s3/_images/ESP32-S3_DevKitC-1_pinlayout_v1.1.jpg' style='margin:15px' alt='esp32-s3' width='500' >\
  <hr>\
  <p>This is a File System Web Browser on the FFAT<br/> in PSFLASH 10MB partition of 16MB ( N16 )</p>\
  <p><a href='/list?dir=/' target='_blank' ><b> root dir :</b></a></p>\
  <p><a href='/list?dir=/data' target='_blank' ><b> /data/ dir :Ain readings to FILE : SIZE</b></a></p><CODE>";
  //DYN_HTMLs  += DYN_DIRS;
  if ( Flistcount > 0 ) { // have dir file info to show in table
    DYN_HTMLs  += "<div><table>";
    DYN_HTMLs  += "<tr>";
    for ( int c = 0; c< 4;c++){
      DYN_HTMLs  += "<th>";
      DYN_HTMLs  += Flist[0][c];
      DYN_HTMLs  += "</th>";
    }
    DYN_HTMLs  += "</tr>";
    for ( int r =1; r <= Flistcount; r++) {
      DYN_HTMLs  += "<tr>";
      for ( int c = 0; c < 4;c++){
        DYN_HTMLs  += "<td>";
        DYN_HTMLs  += Flist[r][c];
        DYN_HTMLs  += "</td>";
      }
      DYN_HTMLs  += "</tr>";
    }

    DYN_HTMLs  += "</table></div>";
  }
  DYN_HTMLs  += "</CODE><hr><p><a href='/print?file=/data/readings.csv' target='_blank' ><b>File print</b></a></p>\
  <p><a href=\"/download?file=/data/readings.csv\" download=\"readings.csv\"><b>File download</b></a></p>\
  <hr><p><form action=/edit method=\"post\" enctype=\"multipart/form-data\">\
  <p><b>File upload :</b></p>\
  <input type=\"file\" name=\"name\">\
  <input class=\"button\" type=\"submit\" value=\"Upload\"></form></p>\
  <hr><table><tr><th>\
  <p><a href='http://kll.byethost7.com/kllfusion01/infusions/articles/articles.php?article_id=230'\
  target='_blank' ><b>kll engineering blog</b></a></p></th><th><p>rev: ";
  DYN_HTMLs  += String(rev);
  DYN_HTMLs  += "</p></th></tr><tr>  <td><p>";
  DYN_HTMLs  += esp_io.get_tnows() ;
  DYN_HTMLs  += "</p></td><td><p>Uptime: ";
  DYN_HTMLs  += String(uptimec);
  DYN_HTMLs  += "</p></td>  </tr>  </table>  <hr>  </body></html>";

  DYN_HTMLs.toCharArray(DYN_HTML,BIGbuf); // for HTML back to CHAR, still hope the creation of the string is more save as snprintf ?( about PANIC and Stack smashing protect failure!)
  //Serial.println(DYN_HTML);
  Serial.printf(" len INDEX : %d \n",strlen(DYN_HTML));
  server.send(200, "text/html", DYN_HTML);
  DYN_HTML[0]= '\0'; // kind of clear

}

long timezone = 1; 
byte daysavetime = 1;

void wifi_setup() {
  // ________________________________________________________________ WIFI NTP
  Serial.print("www Connecting to ");
  Serial.println(ssid);

  WiFi.config(thisip, gateway, subnet, primaryDNS, secondaryDNS);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.print("www WiFi connected to ");
  Serial.println(ssid);
  Serial.print("www IP address: ");
  Serial.println(WiFi.localIP());
  Serial.println("www Contacting Time Server");
	configTime(3600*timezone, daysavetime*3600, "time.nist.gov", "0.pool.ntp.org", "1.pool.ntp.org");
	struct tm tmstruct ;
  delay(2000);
  tmstruct.tm_year = 0;
  getLocalTime(&tmstruct, 5000);
	Serial.printf("\nwww Now is : %d-%02d-%02d %02d:%02d:%02d\n",(tmstruct.tm_year)+1900,( tmstruct.tm_mon)+1, tmstruct.tm_mday,tmstruct.tm_hour , tmstruct.tm_min, tmstruct.tm_sec);
  Serial.println("");

  MDNS.begin(host);
  Serial.print("www Open http://");
  Serial.print(host);
  Serial.print(".local:");
  Serial.print(port);
  Serial.println("/list?dir=/data   or /edit to see the file browser");

  //SERVER INIT
  
  //   http://esp32fs.local:1234/list?dir=/data
  //   http://192.168.1.215:1234/edit
  //   [{"type":"file","name":"data/readings.csv"}]

  server.on("/", handleRoot);
  server.on("/favicon.ico",handleFavicon);
  //list directory
  server.on("/list", HTTP_GET, handleFileList);
  //print file
  server.on("/print",HTTP_GET, handleFilePrint);
  //print download
  server.on("/download",HTTP_GET, handleFileDownload);
  //load editor
  server.on("/edit", HTTP_GET, []() {
    if (!handleFileRead("/edit.htm")) {
      server.send(404, "text/plain", "FileNotFound");
    }
  });
  //create file
  server.on("/edit", HTTP_PUT, handleFileCreate);
  //delete file
  server.on("/edit", HTTP_DELETE, handleFileDelete);
  //first callback is called after the request has ended with all parsed arguments
  //second callback handles file uploads at that location
  server.on("/edit", HTTP_POST, []() {
    server.send(200, "text/plain", "");
  }, handleFileUpload);

  //called when the url is not defined here
  //use it to load content from FILESYSTEM
  server.onNotFound([]() {
    if (!handleFileRead(server.uri())) {
      server.send(404, "text/plain", "FileNotFound");
    }
  });

  //get heap status, analog input value and all GPIO statuses in one json call
  server.on("/all", HTTP_GET, []() {
    String json = "{";
    json += "\"heap\":" + String(ESP.getFreeHeap());
    json += ", \"analog\":" + String(analogRead(A0));
    json += ", \"gpio\":" + String((uint32_t)(0));
    json += "}";
    server.send(200, "text/json", json);
    json = String();
  });
  server.begin();
  Serial.println("www HTTP server started");

}

void wifi_run() {
  server.handleClient(); // _________________________________________ WIFI update
}