#ifndef _TASK_FTP_HPP
#define _TASK_FTP_HPP


#include "Arduino.h"
#include <FS.h>
#include <ESP8266FtpServer.h>



#define _TASK_SLEEP_ON_IDLE_RUN // Enable 1 ms SLEEP_IDLE powerdowns between tasks if no callback methods were invoked during the pass
#define _TASK_STATUS_REQUEST    // Compile with support for StatusRequest functionality - triggering tasks on status change events in addition to time only
#define _TASK_WDT_IDS           // Compile with support for wdt control points and task ids
#define _TASK_PRIORITY          // Support for layered scheduling priority
#define _TASK_OO_CALLBACKS // Support for dynamic callback method binding
#define _TASK_STD_FUNCTION      // pio run -v commandSupport for std::function (ESP8266 ONLY)

#include <TaskSchedulerDeclarations.h>

/*default FTP USER */
#ifndef FTP_USER
  #define FTP_USER "esp8266"
#endif
#ifndef FTP_PASSWD_DEFAULT
  #define FTP_PASSWD_DEFAULT "esp8266"
#endif


class taskFtp : public Task {
public:
taskFtp(unsigned long,long,Scheduler* );
bool Callback();
void startWifi();
FtpServer ftpSrv;


private:
  void startFTP();

};
#endif
