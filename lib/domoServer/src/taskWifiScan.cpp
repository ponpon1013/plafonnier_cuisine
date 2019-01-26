#include "taskWifiScan.h"
#include "ArduinoJson.h"

/* working case
   - at start we take old configuration recorded in SPIFFS memory
   - try to connect to know wifi
   - if yes let's do oter thing
   - if not try three attents
   - after thre attents swicth in AP mode
 */

void taskWifiScan::prinScanResult(int networksFound){
        int i;
        char rootJson[1024];
/* if wifi network found, they were stored in networkAvailable
   if recorded network is found, try to connect it */
   #ifdef DEBUG_NICO
        Serial.printf("NumScan:%i \n",numScan);
   #endif
   #ifdef DEBUG_NICO
        Serial.printf("networksFound:%i \n",networksFound);
   #endif
        if (numScan!=networksFound) {
          numScan=networksFound;
          DynamicJsonBuffer jsonBuffer;// allocate JSOn buffer
          JsonObject& root=jsonBuffer.createObject();
          root["name"]="WiFiScan";
          JsonArray& WifiScanresults=root.createNestedArray("param");

                for (i=0; i<networksFound; i++) {
                        //WiFi.SSID(i).toCharArray(networkAvailable[i].ssid,32);
                        //networkAvailable[i].rrsi=WiFi.RSSI(i);
                        JsonObject& WiFiJson=WifiScanresults.createNestedObject();
                        /*#ifdef DEBUG_NICO
                        Serial.print("ssid: "+WiFi.SSID(i));
                        #endif*/
                        WiFiJson["ssid"]=WiFi.SSID(i);
                        WiFiJson["rrsi"]=WiFi.RSSI(i);
                        /*if(!(strcmp(networkAvailable[i].ssid,tabKnownWiFi.ssid))) {
                                STAmodeSwitch(tabKnownWiFi);
                        }*/
                }
                #ifdef DEBUG_NICO
                /*Serial.printf("%i. ssid:",i);
                Serial.print(networkAvailable[i].ssid);
                Serial.print(" ");
                Serial.print(networkAvailable[i].rrsi);
                Serial.println("%");*/
                root.printTo(Serial);
                #endif
                root.printTo(rootJson);
                //m_WebSocketsServer->broadcastTXT(rootJson);

        }
        m_WebSocketsServer->broadcastTXT(rootJson);
        scanOn=false;
}

void taskWifiScan::addParam(String* record){
  DynamicJsonBuffer jsonBuffer;// allocate JSOn buffer
  JsonObject& root=jsonBuffer.parseObject(*record);
  const char* param=root["command"]["param"];

  const char* request=(const char*) param;
  scanRequest=(!strcmp(request, "on"))?true:scanRequest;
  scanRequest=(!strcmp(request, "off"))?false:scanRequest;
}

bool taskWifiScan::Callback(){
        if(scanRequest){
        if (!scanOn) {
                scanOn=true;
                WiFi.scanNetworksAsync(std::bind(&taskWifiScan::prinScanResult,this,std::placeholders::_1));
        }
      }
      else{
        this->disable();
      }

        return true;
}

taskWifiScan::taskWifiScan(unsigned long aInterval,long aIterations,Scheduler* aS, const char* name, WebSocketsServer* server) :
  taskInfo(aInterval, aIterations, aS/* false)*/, name),
  scanOn(false),
  scanRequest(false),
  numScan(0){
  #ifdef DEBUG_NICO
    Serial.print("m_name WifiScan:");
    Serial.print(m_name);
  #endif
  m_WebSocketsServer=server;

}
