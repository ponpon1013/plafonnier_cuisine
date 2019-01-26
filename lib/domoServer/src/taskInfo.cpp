#include "taskInfo.h"

void taskInfo::addParam(String* param){
  m_param=param;
}

taskInfo::taskInfo(unsigned long aInterval,long aIterations,Scheduler* aS, const char* name) :
  Task(aInterval, aIterations, aS,false),
  m_name(name){
//taskInfo::taskInfo(const char* name) :m_name(name){

}
