#include <time.h>

int loopc = 0;
const int loopclim = 1000; // _______________________________________ cycles until we check time
int thissec = 0;
int thismin = 0;

void loopC () {
  loopc += 1;
  if ( loopc >= loopclim ) {
    loopc = 0;
    time_t now = time(nullptr);
    struct tm ti;
    gmtime_r(&now, &ti);
    int nowsec = ti.tm_sec;
    if (nowsec != thissec) {
      thissec = nowsec;
      thismin = ti.tm_min;
      Serial.print(".");
      Job_1sec();
    }
  }
}

int loopNs = 0;
int loopNc = 0;
int loopNslim = 15; // ______________________________________________ make a 15 sec tick

void loopNsec() {
  if (loopNs != thissec) {
    loopNs = thissec;
    loopNc += 1;
    if ( loopNc >= loopNslim ) {
      loopNc = 0;
      Job_Nsec();
    }
  }
}

int loop1minc = 0;


void loop1min() { // ________________________________________________ you will see at startup it is executed, and also at the first minute change, but then dt is 1min
  if ( loop1minc != thismin ) {
    loop1minc = thismin;
    Job_1min();
  }
}

int loop1M = 0;
unsigned long lastTms = millis();
unsigned long nowms=lastTms;
bool showms = false; // true;
bool showT = false; // true;

void loopM() {
  loop1M += 1;
  if ( loop1M >= 1000000 ) { // _____________________________________ about 11 sec ? now 22 sec / without MDNBS.update now 9 sec
    loop1M = 0;
    if ( showms ) {
      nowms = millis();
      Serial.printf("\n___ loop1M dt : %.2f",(nowms - lastTms)/1000.0);
      lastTms = nowms;
    }
    if ( showT ) {
      Serial.println(); // ____________________________________________ after the "." need a LF
      Serial.print("___ loop1M: ");
      esp_io.nows();
      Serial.println(esp_io.get_tnows()); // set by nows();
    }
  }
}

