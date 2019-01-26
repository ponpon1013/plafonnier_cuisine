#include "taskWifi.h"
#include <ESP_EEPROM.h>
/* to use ESP_EEPROM I add the following line to platformio.ini:
   lib_deps =
 # Using library Id
     2795

 # Using library Name
     ESP_EEPROM
 */

/* working case
   - at start we take old configuration recorded in SPIFFS memory
   - try to connect to know wifi
   - if yes let's do oter thing
   - if not try three attents
   - after thre attents swicth in AP mode
 */

bool setWiFi2EEprom(EEConf tabKnownWiFi){
  EEPROM.begin(4096);
  #ifdef DEBUG_NICO
        Serial.println("saving network configuration:");
  #endif
        EEPROM.put(0,1);// number of wifi recorded
        EEPROM.put(1,tabKnownWiFi);
}

bool getFromEepromWifi(EEConf* tabKnownWiFi){
        // i choose the following structure
        // fist record: int to get the number of record
        // following are EEconf values to get wifi known network
        #ifdef DEBUG_NICO
        Serial.println("getting old network configuration:");
        #endif
        int nbrWifi /*,i*/;
        EEPROM.begin(4096); // init of EEPROM access I put 4096 to have the max
        if(EEPROM.get(0,nbrWifi)==1) { // to get the number of records
                #ifdef DEBUG_NICO
                Serial.printf("found %i records:\n",nbrWifi);
                #endif
                EEPROM.get(1,tabKnownWiFi); //ssid of records
/*
                EEPROM.get(1+SSID_SIZE,tabKnownWiFi->password); // password of record
                EEPROM.get(1+SSID_SIZE+PASSWD_SIZE,tabKnownWiFi->myhostname);// hostaname of record*/
                #ifdef DEBUG_NICO
                Serial.printf("ssid: %s",tabKnownWiFi->ssid);
                Serial.printf("password: %s",tabKnownWiFi->password);
                Serial.printf("myhostname: %s",tabKnownWiFi->myhostname);
                #endif
                return true;
        }
        else{
          #ifdef DEBUG_NICO
                Serial.printf("found %i records: error\n",nbrWifi);
          #endif
                return false;
        }

}

/*/void APmodeSwicth(bool *isConnect){
        #ifdef DEBUG_NICO
        Serial.print("Setting soft-AP ... ");
        #endif
        WiFi.disconnect();
        WiFi.enableSTA(false);
        //WiFi.setAutoConnect (false);
        //WiFi.setAutoReconnect (false);
        //WiFi.enableAP(true);
        WiFi.softAP(AP_NAME, AP_PASSWD_DEFAULT);

        #ifdef DEBUG_NICO
        Serial.println(WiFi.softAPIP());
        #endif
        *isConnect=false;
}*/

/*void STAmodeSwitch(bool *isConnect,EEConf tabKnownWiFi){
        #ifdef DEBUG_NICO
        Serial.println("Setting wifi connexion as station ... ");
        Serial.print("ssid: ");
        Serial.println(tabKnownWiFi.ssid);
        Serial.print("password");
        Serial.println(tabKnownWiFi.password);
        #endif
        //Serial.setDebugOutput(true);

        WiFi.enableAP(false); // remove AP mode if it was enable
        WiFi.disconnect();
        WiFi.enableSTA(true);
        WiFi.setAutoConnect (true);
        WiFi.setAutoReconnect (true);
        WiFi.begin (tabKnownWiFi.ssid,tabKnownWiFi.password);
        *isConnect=true;
}*/

void taskWifi::addParam(String *record){
  #ifdef DEBUG_NICO
        Serial.println("Receive new Param for Wifi Config");
  #endif
        DynamicJsonBuffer jsonBuffer;// allocate JSOn buffer
        JsonObject& root=jsonBuffer.parseObject(*record);
        strcpy(tabKnownWiFi.ssid, root["command"]["param"]["ssid"]);
        strcpy(tabKnownWiFi.password, root["command"]["param"]["password"]);
        strcpy(tabKnownWiFi.myhostname,"");
        WiFi.begin (tabKnownWiFi.ssid,tabKnownWiFi.password);
        attent=0;
}

bool taskWifi::Callback(){

  if (WiFi.getMode()!=WIFI_STA && !(*m_scanOn) && (isConnect)) {
    WiFi.enableAP(false);
    m_channel=WiFi.channel();
  }

/*        if (!(m_TaskWifiScan->scanOn)) {
                WiFiMode_t modeWiFi=WiFi.getMode();
                if (WiFi.status() == WL_CONNECTED) { // WiFi connected to ssid
                        attent=0; // attent is turn at 0
                        #ifdef DEBUG_NICO
                        Serial.println("WiFi STA Mode");
                        #endif
                        //Serial.setDebugOutput(false);
                        if (isConnect){
                          setWiFi2EEprom(tabKnownWiFi);
                          isConnect=false;
                        }
                }
                else if (isConnect){ // under connexion wait
                  this->delay(1000);
                  attent++;
                  isConnect=(attent<60)?true:false;
                }
                else if(modeWiFi!=WIFI_AP) {
                  APmodeSwicth(&isConnect); // turn in AP MODE.
                }
                else if(modeWiFi==WIFI_AP){ // in this case we are in APMode
                  m_TaskWifiScan->scanRequest=true;
                  if (!(m_TaskWifiScan->isEnabled())) m_TaskWifiScan->enable();
                }
        }
        else{
/* if in AP mode start scan mode to see if last
   recorded network is now availbale*/

  /*      #ifdef DEBUG_NICO
                Serial.print("Scanning...");
      #endif
    }*/

    #ifdef DEBUG_NICO
        Serial.print("WiFi Mode:");
        Serial.println(WiFi.getMode());
    #endif
        return true;
}


/*void taskWifi::onConnected(){
        isConnect=true;
}*/

void taskWifi::onDisconnect (const WiFiEventStationModeDisconnected& event){
  Serial.print("WiFi disconnected");
  Serial.print("mode:");
  Serial.println(WiFi.getMode());
  if(++attent>10){
    attent=10;
    isConnect=false;
    if (WiFi.getMode()!=WIFI_AP){
      WiFi.softAP(AP_NAME, AP_PASSWD_DEFAULT,m_channel);
      if (!(*m_scanOn)) {
        WiFi.enableSTA(false);
      }
    }
  }
  Serial.print("mode:");
  Serial.println(WiFi.getMode());

}

void taskWifi::onConnect (const WiFiEventStationModeConnected& event){
  Serial.print("WiFi Connected ");
  isConnect=true;
  attent=0;
}

taskWifi::taskWifi(unsigned long aInterval,long aIterations,Scheduler* aS,const char* name,bool* scanOn) :
        taskInfo(aInterval, aIterations, aS /* false)*/, name){

  attent=0;

  // trun Wifi in mode persistant to save wifi configure
  WiFi.persistent(true);
  m_scanOn=scanOn;
  // introduce wifi events
  disconnectedEventHandler=WiFi.onStationModeDisconnected(std::bind(&taskWifi::onDisconnect,this,std::placeholders::_1));
  connectedEventHandler=WiFi.onStationModeConnected(std::bind(&taskWifi::onConnect,this,std::placeholders::_1));
  m_channel=WiFi.channel();
  // turn in SoftAP to get last recorded CONFIGURATION
  //WiFi.begin("HUAWEI P8 lite 2017","estelle1013");
  WiFi.disconnect();
  WiFi.setAutoConnect (true);
  WiFi.setAutoReconnect (true);
  if (getFromEepromWifi(&tabKnownWiFi)) {
    WiFi.enableSTA(true);
    WiFi.enableAP(false);
    WiFi.begin (tabKnownWiFi.ssid,tabKnownWiFi.password);
  }
  else{
    WiFi.enableSTA(false);
    WiFi.enableAP(true);
    WiFi.softAP(AP_NAME, AP_PASSWD_DEFAULT);
  }
  //m_TaskWifiScan=WiFiScanTask;
/*        attent=0;
        //m_scanOn=scanOn;
        #ifdef DEBUG_NICO
        Serial.print("MAC: ");
        Serial.println(WiFi.macAddress());
        #endif
        if (getFromEepromWifi(&tabKnownWiFi)) {
                STAmodeSwitch(&isConnect,tabKnownWiFi);
        }
        else{
                APmodeSwicth(&isConnect);
        }
        //stationConnectedHandler=WiFi.onSoftAPModeStationConnected(std::bind(&taskWifi::onConnected,this));

*/


}
