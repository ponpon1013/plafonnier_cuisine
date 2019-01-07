#ifndef _TASK_WEBSOCKETS_HPP
#define _TASK_WEBSOCKETS_HPP


#include <Arduino.h>
#include <FS.h>
#include <Hash.h>
#include <WebSocketsServer.h>


#define _TASK_SLEEP_ON_IDLE_RUN // Enable 1 ms SLEEP_IDLE powerdowns between tasks if no callback methods were invoked during the pass
#define _TASK_STATUS_REQUEST    // Compile with support for StatusRequest functionality - triggering tasks on status change events in addition to time only
#define _TASK_WDT_IDS           // Compile with support for wdt control points and task ids
#define _TASK_PRIORITY          // Support for layered scheduling priority
#define _TASK_OO_CALLBACKS // Support for dynamic callback method binding
#define _TASK_STD_FUNCTION      // pio run -v commandSupport for std::function (ESP8266 ONLY)

#include <TaskSchedulerDeclarations.h>

class taskWebSockets : public Task {
public:
  WebSocketsServer* webSocket;
taskWebSockets(unsigned long,long,Scheduler* );
bool Callback();
void webSocketEvent(uint8_t , WStype_t , uint8_t * , size_t );

private:
};


#endif
