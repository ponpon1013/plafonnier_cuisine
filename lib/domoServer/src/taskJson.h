#ifndef _TASK_JSON_HPP
#define _TASK_JSON_HPP

#define _TASK_SLEEP_ON_IDLE_RUN // Enable 1 ms SLEEP_IDLE powerdowns between tasks if no callback methods were invoked during the pass
#define _TASK_STATUS_REQUEST    // Compile with support for StatusRequest functionality - triggering tasks on status change events in addition to time only
#define _TASK_WDT_IDS           // Compile with support for wdt control points and task ids
#define _TASK_PRIORITY          // Support for layered scheduling priority
#define _TASK_OO_CALLBACKS // Support for dynamic callback method binding
#define _TASK_STD_FUNCTION      // pio run -v commandSupport for std::function (ESP8266 ONLY)

#define JSON_CAPACITY 1000

#include "Arduino.h"
#include <CircularBuffer.h>
//#include <TaskSchedulerDeclarations.h>
#include <ArduinoJson.h>
#include "taskInfo.h"

class taskJson : public Task,taskInfo {
public:
CircularBuffer<String, 100> buffer;
taskJson(unsigned long,long,Scheduler*,const char* );
bool Callback();
bool addJson(String);

private:

};
#endif
