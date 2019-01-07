#include "taskFtp.h"


void taskFtp::startFTP(){
        if (SPIFFS.begin()) {
          #ifdef DEBUG_NICO
          Serial.println("SPIFFS opened!");
          #endif

          ftpSrv.begin("esp8266","esp8266"); //username, password for ftp.  set ports in ESP8266FtpServer.h  (default 21, 50009 for PASV)
        }
}

taskFtp::taskFtp(unsigned long aInterval,long aIterations,Scheduler* aS) :  Task(aInterval, aIterations, aS, false){
        this->startFTP();
      #ifdef DEBUG_NICO
        Serial.println("Start FTP server!");
      #endif
}


bool taskFtp::Callback(){
        ftpSrv.handleFTP();
        return true;
}
