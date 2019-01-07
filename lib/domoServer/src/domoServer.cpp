#include "domoServer.h"
#include "taskWebServer.h"
#include "taskWifi.h"
#include "taskFtp.h"
#include "taskWebSockets.h"

/*void prinScanResult(int networksFound){
        if (numScan!=networksFound) {
                const size_t bufferSize=
                        JSON_ARRAY_SIZE(networksFound)+JSON_OBJECT_SIZE(1)
 +JSON_OBJECT_SIZE(2)+networksFound*JSON_OBJECT_SIZE(3);

                numScan=networksFound;

                DynamicJsonBuffer jsonBuffer(bufferSize);
                JsonObject& root=jsonBuffer.createObject();
                JsonObject& wifiscanJson=root.createNestedObject("WiFiScan");
                wifiscanJson["number"]=numScan;

                JsonArray& wifiScan_wifi=wifiscanJson.createNestedArray("wifi");

 #ifdef DEBUG_NICO
                Serial.printf("%d network(s) found\n", networksFound);
 #endif
                for (int i = 0; i < networksFound; i++)
                {
 #ifdef DEBUG_NICO
                        Serial.printf("%d: %s, Ch:%d (%ddBm) %s\n", i + 1, WiFi.SSID(i).c_str(), WiFi.channel(i), WiFi.RSSI(i), WiFi.encryptionType(i) == ENC_TYPE_NONE ? "open" : "");
 #endif
                        JsonObject& wifiScan_wifi_i=wifiScan_wifi.createNestedObject();
                        wifiScan_wifi_i["numero"]=i;
                        wifiScan_wifi_i["name"]=WiFi.SSID(i).c_str();
                        wifiScan_wifi_i["puissance"]=WiFi.RSSI(i);
                }
                String chaine;
                root.printTo(chaine);
                webSocketNicolas.broadcastTXT(chaine);
        }
        scanOn=false;
   }
 */

void domoServer::execute(){
        ts->execute();
}

// constructeur
domoServer::domoServer (Scheduler* tsInput){
        int i;

        #ifdef DEBUG_NICO
        Serial.print("domoserver initialisation");
        #endif
        ts=tsInput;
        ts->init(); // init of Scheduler


        nbrTask=4;
        tableTask[0]=new taskWifi(DELAY_WIFI_CALLBACK,TASK_FOREVER,ts); // initilaise WIFI
        tableTask[1]=new taskWebServer(0,TASK_FOREVER,ts);
        tableTask[2]=new taskFtp(DELAY_FTP_CALLBACK,TASK_FOREVER,ts); // initilaise WIFI
        tableTask[3]=new taskWebSockets(DELAY_FTP_CALLBACK,TASK_FOREVER,ts); // initilaise WIFI
        //tableTask[0]=new Task(0, TASK_FOREVER, &toto,ts,false,NULL,NULL);
        for(i=0; i<nbrTask; i++) {
                tableTask[i]->enable();
        }
}
