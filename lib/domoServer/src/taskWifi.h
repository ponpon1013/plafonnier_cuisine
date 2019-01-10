#ifndef _TASK_WIFI_HPP
#define _TASK_WIFI_HPP


#include "Arduino.h"
#include <FS.h>
#include <ESP8266WiFi.h>


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

/*default WiFi AP */
#ifndef AP_NAME
  #define AP_NAME "domoServer"
#endif
#ifndef AP_PASSWD_DEFAULT
  #define AP_PASSWD_DEFAULT "default"
#endif


//typedef struct EEConf EEConf;
struct EEConf {
        char ssid[32];
        char password[64];
        char myhostname[32];
};


class taskWifi : public Task,taskInfo {
public:
bool isConnect;
taskWifi(unsigned long,long,Scheduler*,const char* );
bool Callback();
void startWifi();

private:
struct EEConf tabKnownWiFi;
int attent;   // number of attent to know if connexion is lost or not
};


#endif
