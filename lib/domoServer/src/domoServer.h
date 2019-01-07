/*
   The code is to  define common library to get common templet for
   house sensors domotic system

   The aim is to get sensors with:
    - proper wifi Access point to be configuarted at first power on
    - web interface to configure WiFi
    - websocket server to help wenserver to configure
    - task scheduler to:
      - treat Wifi, wenServer and WebSocketsServer
      - have the ability to add easily other task
 */

#ifndef _DOMO_SERVER_HPP
#define _DOMO_SERVER_HPP

/*------------------- library including ------------------*/
#include <Arduino.h>

/*library for WIFI,FTP server, webserver,
   websocket and Task Scheduler*/
 //#include <ESP8266WiFi.h>
 //#include <ESP8266WebServer.h>
// #include <ESP8266mDNS.h>
 //#include <FS.h>
// #include <ESP8266FtpServer.h>
// #include <WebSocketsServer.h>
// #include <ArduinoJson.h>

#define _TASK_SLEEP_ON_IDLE_RUN // Enable 1 ms SLEEP_IDLE powerdowns between tasks if no callback methods were invoked during the pass
#define _TASK_STATUS_REQUEST    // Compile with support for StatusRequest functionality - triggering tasks on status change events in addition to time only
#define _TASK_WDT_IDS           // Compile with support for wdt control points and task ids
#define _TASK_PRIORITY          // Support for layered scheduling priority
#define _TASK_OO_CALLBACKS // Support for dynamic callback method binding
#define _TASK_STD_FUNCTION      // Support for std::function (ESP8266 ONLY)

#define DELAY_WIFI_CALLBACK 1000 // 1000 ms
#define DELAY_FTP_CALLBACK 0 // 1000 ms

#include <TaskSchedulerDeclarations.h>

/* classe declaration */
class domoServer{
public:
  domoServer(Scheduler*);
  void execute();
  //Scheduler* ts;
  Task *tableTask[255];
  //bool addTask(Task *);

private:
  bool scanOn=false; //flag to say if WIFI scanning is ON or not
  int numScan=0; // number of wifi network found
  //ESP8266WebServer* server;
//  WebSocketsServer* webSocket;
//  FtpServer* ftpSrv;
  Scheduler* ts;
  //taskWebServer* server;
  /*void startServer();
  void handleNotFound();
  bool handleFileRead(String);*/
//  void webSocketEvent(uint8_t , WStype_t , uint8_t * , size_t );
//  void startWebSocket();
  //void webserverLoop();
  int nbrTask; // number of task recorded
};

#define MAX_WIFI_SCAN 255
/*void prinScanResult(int );
void scanWifi();*/


#endif
