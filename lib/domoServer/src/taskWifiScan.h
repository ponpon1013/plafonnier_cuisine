#ifndef _TASK_WIFI_SCAN_HPP
#define _TASK_WIFI_SCAN_HPP


#include "Arduino.h"
#include <FS.h>
#include <ESP8266WiFi.h>
#include <WebSocketsServer.h>

#define WIFISCAN_TASK_NAME "WiFiScan"
#define DELAY_WIFISCAN_CALLBACK 10000 // 10s


#define _TASK_SLEEP_ON_IDLE_RUN // Enable 1 ms SLEEP_IDLE powerdowns between tasks if no callback methods were invoked during the pass
#define _TASK_STATUS_REQUEST    // Compile with support for StatusRequest functionality - triggering tasks on status change events in addition to time only
#define _TASK_WDT_IDS           // Compile with support for wdt control points and task ids
#define _TASK_PRIORITY          // Support for layered scheduling priority
#define _TASK_OO_CALLBACKS // Support for dynamic callback method binding
#define _TASK_STD_FUNCTION      // pio run -v commandSupport for std::function (ESP8266 ONLY)

#include <TaskSchedulerDeclarations.h>
#include "taskInfo.h"

#define SSID_SIZE 32
#define PASSWD_SIZE 64
#define MYHOSTNAME_SIZE 32
#define MAX_WIFI_RECORD 255

/*struct networkInfo{
  char ssid[32];
  int32_t rrsi;
};*/

class taskWifiScan : public /*virtual Task,*/taskInfo {
public:
bool scanOn,scanRequest;
taskWifiScan(unsigned long,long,Scheduler*, const char*,WebSocketsServer* );
bool Callback();
int numScan;
//struct networkInfo networkAvailable[255];

private:
int networksFound;
void prinScanResult(int);
virtual void addParam(void *);
WebSocketsServer* m_WebSocketsServer;
int numSockets; // Websockets number to answer
};


#endif
