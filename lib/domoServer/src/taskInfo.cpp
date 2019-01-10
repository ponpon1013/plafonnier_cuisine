#include "taskInfo.h"

void taskInfo::addParam(void* param){
  m_param=param;
}

taskInfo::taskInfo(const char* name) :m_name(name){

}
