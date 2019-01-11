#include "taskFtp.h"


void taskFtp::startFTP(){
        if (SPIFFS.begin()) {
          #ifdef DEBUG_NICO
          Serial.println("SPIFFS opened!");
          #endif

          // format if it is not formatted
          // it is formatted when file formatted.txt is present

          if(!SPIFFS.exists(FILE_FORMMATTED_FLAG)){
            #ifdef DEBUG_NICO
            Serial.println("Please wait for SPIFFS to be formatted");
            #endif
            SPIFFS.format();
            #ifdef DEBUG_NICO
            Serial.println("SPIFFS formatted");
            #endif

            File f=SPIFFS.open(FILE_FORMMATTED_FLAG,"w");
            if (!f){
              #ifdef DEBUG_NICO
              Serial.println("file open failed");
              #endif
            }
            else
            {
              #ifdef DEBUG_NICO
              Serial.println("Format completed");
              #endif
            }

          }
          else{
            #ifdef DEBUG_NICO
            Serial.println("SPIFFS is formatted. list of file found:");
            Dir dir=SPIFFS.openDir("");
            while (dir.next()){
              Serial.print(dir.fileName());
              File f=dir.openFile("r");
              Serial.println(f.size());
            }
            #endif

          }


          ftpSrv.begin("esp8266","esp8266"); //username, password for ftp.  set ports in ESP8266FtpServer.h  (default 21, 50009 for PASV)
        }
}

/*taskFtp::taskFtp(unsigned long aInterval,long aIterations,Scheduler* aS, const char* name) :
  Task(aInterval, aIterations, aS,false),
  taskInfo(name)*/
  taskFtp::taskFtp(unsigned long aInterval,long aIterations,Scheduler* aS, const char* name) :
    taskInfo(aInterval, aIterations, aS,/*false,*/name){
        this->startFTP();
      #ifdef DEBUG_NICO
        Serial.println("Start FTP server!");
      #endif
}

bool taskFtp::Callback(){
        ftpSrv.handleFTP();
        return true;
}
