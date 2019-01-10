#include "taskWifiScan.h"

/* working case
   - at start we take old configuration recorded in SPIFFS memory
   - try to connect to know wifi
   - if yes let's do oter thing
   - if not try three attents
   - after thre attents swicth in AP mode
 */

void taskWifiScan::prinScanResult(int networksFound){
        int i;
/* if wifi network found, they were stored in networkAvailable
   if recorded network is found, try to connect it */
   #ifdef DEBUG_NICO
        Serial.printf("NumScan:%i \n",numScan);
   #endif
        if (numScan!=networksFound) {
                for (i=0; i<networksFound; i++) {
                        WiFi.SSID(i).toCharArray(networkAvailable[i].ssid,32);
                        networkAvailable[i].rrsi=WiFi.RSSI(i);
                        /*if(!(strcmp(networkAvailable[i].ssid,tabKnownWiFi.ssid))) {
                                STAmodeSwitch(tabKnownWiFi);
                        }*/
                        #ifdef DEBUG_NICO
                        Serial.printf("%i. ssid:",i);
                        Serial.println(networkAvailable[i].ssid);
                        #endif
                }
        }
        scanOn=false;
}

bool taskWifiScan::Callback(){
        if (!scanOn) {
                scanOn=true;
                WiFi.scanNetworksAsync(std::bind(&taskWifiScan::prinScanResult,this,std::placeholders::_1));
        }
        return true;
}

taskWifiScan::taskWifiScan(unsigned long aInterval,long aIterations,Scheduler* aS, const char* name) :
  Task(aInterval, aIterations, aS, false),
  taskInfo(name),
  scanOn(false){

}
