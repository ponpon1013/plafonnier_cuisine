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

bool getFromEepromWifi(EEConf* tabKnownWifi){
        // i choose the following structure
        // fist record: int to get the number of record
        // following are EEconf values to get wifi known network
        #ifdef DEBUG_NICO
        Serial.println("getting old network configuration:");
        #endif
        int nbrWifi /*,i*/;
        EEPROM.begin(4096); // init of EEPROM access I put 4096 to have the max
        if(EEPROM.get(0,nbrWifi)>0) { // to get the number of records
                #ifdef DEBUG_NICO
                Serial.printf("found %i records:\n",nbrWifi);
                #endif
                EEPROM.get(1,tabKnownWifi->ssid); //ssid of records
                EEPROM.get(1+SSID_SIZE,tabKnownWifi->password); // password of record
                EEPROM.get(1+SSID_SIZE+PASSWD_SIZE,tabKnownWifi->myhostname);// hostaname of record
                #ifdef DEBUG_NICO
                Serial.printf("ssid: %s",tabKnownWifi->ssid);
                Serial.printf("password: %s",tabKnownWifi->password);
                Serial.printf("myhostname: %s",tabKnownWifi->myhostname);
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

void APmodeSwicth(){
        #ifdef DEBUG_NICO
        Serial.print("Setting soft-AP ... ");
        #endif
        WiFi.enableSTA(false);
        WiFi.setAutoConnect (false);
        WiFi.setAutoReconnect (false);
        //WiFi.enableAP(true);
        WiFi.softAP(AP_NAME, AP_PASSWD_DEFAULT);

        #ifdef DEBUG_NICO
        Serial.println(WiFi.softAPIP());
        #endif
}

void STAmodeSwitch(struct EEConf tabKnownWiFi ){
        #ifdef DEBUG_NICO
        Serial.print("Setting wifi conexion as station ... ");
        #endif
        WiFi.enableAP(false); // remove AP mode if it was enable
        WiFi.enableSTA(true);
        WiFi.setAutoConnect (true);
        WiFi.setAutoReconnect (true);
        WiFi.begin (tabKnownWiFi.ssid,tabKnownWiFi.password);
}

void taskWifi::prinScanResult(int networksFound){
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
                        if(!(strcmp(networkAvailable[i].ssid,tabKnownWiFi.ssid))) {
                                STAmodeSwitch(tabKnownWiFi);
                        }
                        #ifdef DEBUG_NICO
                        Serial.printf("%i. ssid:",i);
                        Serial.println(networkAvailable[i].ssid);
                        #endif
                }
        }
}

void taskWifi::scanWifi(){
        if(scanOn){
          WiFi.scanNetworksAsync(std::bind(&taskWifi::prinScanResult,this,std::placeholders::_1));
        }
}

bool taskWifi::Callback(){
        /* every time callback is launch:
           - launch scan if necessary to refresh web page
           - need to know in what WIFI mode we are:
           - already connected ( FLAG isConnect) -> check if always connected.
           - not connected : use scan results to know if configured server is available
         */
        if (isConnect) {
                #ifdef DEBUG_NICO
                Serial.println("IsConnect true");
                #endif
                if (WiFi.status() == WL_CONNECTED)
                // WiFi connected to ssid
                {
                        attent=0; // attent is turn at 0
                        // if scan was requested scan is launch
                        if (scanOn) {
                                this->scanWifi();
                        }
                }
                else
                {
                        attent++;
                        if (attent==3)
                        {
                                isConnect=false;
                                attent=2; // just to not have big values in case of no conenxion and in AP mode
                                APmodeSwicth(); // turn in AP MODE.
                        }
                }
        }
        else{
                // launch Wifi Scan to check if configured weifi server is now availble
                this->scanWifi();
                // if available turn in STA mode
        }
        return true;
}

taskWifi::taskWifi(unsigned long aInterval,long aIterations,Scheduler* aS) :  Task(aInterval, aIterations, aS, false){
        isConnect=false;
        attent=0;
        scanOn=false;
        numScan=0;
        if (getFromEepromWifi(&tabKnownWiFi)) {
                STAmodeSwitch(tabKnownWiFi);
        }
        else{
                APmodeSwicth();
        }
}
