#ifndef _TASK_WEBSERVER_HPP
#define _TASK_WEBSERVER_HPP

#include "Arduino.h"
#include <FS.h>

#define _TASK_SLEEP_ON_IDLE_RUN // Enable 1 ms SLEEP_IDLE powerdowns between tasks if no callback methods were invoked during the pass
#define _TASK_STATUS_REQUEST    // Compile with support for StatusRequest functionality - triggering tasks on status change events in addition to time only
#define _TASK_WDT_IDS           // Compile with support for wdt control points and task ids
#define _TASK_PRIORITY          // Support for layered scheduling priority
#define _TASK_OO_CALLBACKS // Support for dynamic callback method binding
#define _TASK_STD_FUNCTION      // Support for std::function (ESP8266 ONLY)

#include <TaskSchedulerDeclarations.h>
#include "ESP8266WebServer.h"
#include "taskInfo.h"

class taskWebServer : public Task,taskInfo {
public:
  taskWebServer(unsigned long ,long ,Scheduler*,const char* );
  ESP8266WebServer *server;
  bool Callback();
  void startServer();
private:

  void handleNotFound();
  bool handleFileRead(String);
};


#endif
