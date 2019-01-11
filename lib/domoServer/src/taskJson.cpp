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
      int i;
      #ifdef DEBUG_NICO
      Serial.print(*m_sizeTabTask);
      #endif

      for (i=0;i<(*m_sizeTabTask);i++)
      {
        if (!strcmp_P(m_tabTask[i]->m_name, order))
        {
          #ifdef DEBUG_NICO
          Serial.println("order found");
          #endif
          m_tabTask[i]->addParam((void*)param);
          if (!m_tabTask[i]->isEnabled()){
            m_tabTask[i]->enable();
          }
        }
      }
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

taskJson::taskJson(unsigned long aInterval,long aIterations,Scheduler* aS, const char* name,taskInfo **tabTask,int* sizeTab) :
  //Task(aInterval, aIterations, aS,false),
  taskInfo(aInterval, aIterations, aS/*,false*/,name),
  m_tabTask(tabTask),
  m_sizeTabTask(sizeTab){

}
