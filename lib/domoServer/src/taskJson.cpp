#include "taskJson.h"

bool taskJson::Callback(){
  if (!buffer.isEmpty()){
    String record=buffer.pop();
    #ifdef DEBUG_NICO
    Serial.println("record: "+record);
    #endif
    DynamicJsonBuffer jsonBuffer;// allocate JSOn buffer
    JsonObject& root=jsonBuffer.parseObject(record);
    if (!root.success()){
      #ifdef DEBUG_NICO
      Serial.println("json parsing failed!");
      #endif
    }
    else{
      const char* order=root["command"]["order"];
      const char* param=root["command"]["param"];
      #ifdef DEBUG_NICO
      Serial.print("order:");
      Serial.println(order);
      Serial.print("param:");
      Serial.println(param);
      #endif
    }
  }
  return true;
}

bool taskJson::addJson(String record){

  #ifdef DEBUG_NICO
  Serial.println("Get Record: "+record);
  #endif
  if (!buffer.isFull()){
    buffer.push(record);
    Serial.println("read last Record: "+buffer.last());
    return true;
  }
  else
    return false;
}

taskJson::taskJson(unsigned long aInterval,long aIterations,Scheduler* aS, const char* name) :
  Task(aInterval, aIterations, aS,false),
  taskInfo(name){

}
