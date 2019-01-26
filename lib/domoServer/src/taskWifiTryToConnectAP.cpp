#include "taskWifiTryToConnectAP.h"

bool taskWifiTryToConnectAP::Callback(){
  if(!(*m_isConnect)){
    Serial.println("try to reconnect");
    WiFi.enableAP(false);
    WiFi.enableSTA(true);
    WiFi.reconnect();
  }
  return true;
}

taskWifiTryToConnectAP::taskWifiTryToConnectAP(unsigned long aInterval,long aIterations,Scheduler* aS, const char* name,bool* isConnect ):
  taskInfo(aInterval, aIterations, aS/* false)*/, name){
    m_isConnect=isConnect;
}
