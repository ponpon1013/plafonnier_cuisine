#ifndef _TASK_INFO_HPP
#define _TASK_INFO_HPP

#define _TASK_SLEEP_ON_IDLE_RUN // Enable 1 ms SLEEP_IDLE powerdowns between tasks if no callback methods were invoked during the pass
#define _TASK_STATUS_REQUEST    // Compile with support for StatusRequest functionality - triggering tasks on status change events in addition to time only
#define _TASK_WDT_IDS           // Compile with support for wdt control points and task ids
#define _TASK_PRIORITY          // Support for layered scheduling priority
#define _TASK_OO_CALLBACKS // Support for dynamic callback method binding
#define _TASK_STD_FUNCTION      // Support for std::function (ESP8266 ONLY)

#include <TaskSchedulerDeclarations.h>


class taskInfo: public Task{

public:
  const char* m_name;
  void* m_param;
  //taskInfo(const char*);
  taskInfo(unsigned long,long,Scheduler*,const char* );
  virtual void addParam(void *);
};

#endif
