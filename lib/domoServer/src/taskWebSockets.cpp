#include "taskWebSockets.h"

void taskWebSockets::webSocketEvent(uint8_t num, WStype_t type, uint8_t * payload, size_t lenght) { // When a WebSocket message is received
         switch (type) {
         case WStype_DISCONNECTED:// if the websocket is disconnected
          #ifdef DEBUG_NICO
          Serial.printf("[%u] Disconnected!\n", num);
          #endif
          break;

         case WStype_CONNECTED:// if a new websocket connection is established
         {
           IPAddress ip = webSocket->remoteIP(num);
           #ifdef DEBUG_NICO
           Serial.printf("[%u] Connected from %d.%d.%d.%d url: %s\n", num, ip[0], ip[1], ip[2], ip[3], payload);
           #endif
           break;
         }
         case WStype_ERROR:
                 break;

         case WStype_BIN:
                 break;
         case WStype_FRAGMENT:
           break;
         case WStype_FRAGMENT_TEXT_START:
         break;
         case WStype_FRAGMENT_BIN_START:
           break;
         case WStype_FRAGMENT_FIN:
           break;

         case WStype_TEXT:                 // if new text data is received
          /*#ifdef DEBUG_NICO
          Serial.printf("[%u] get Text: %s\n", num, payload);
          int i;
          Serial.print("hex:");
          for (i=0;i<lenght;i++){
          Serial.printf("%x ",payload[i]);
          }
          Serial.println();
          #endif*/
          m_jsonTask->addJson(String((char *)payload));
          /*DynamicJsonBuffer jsonBuffer(JSON_CAPACITY);// allocate JSOn buffer
          JsonObject& root=jsonBuffer.parseObject(payload);
          if (!root.success()){
            #ifdef DEBUG_NICO
            Serial.println("json parsing failed!");
            #endif
          }*/
          break;
         }
    }

taskWebSockets::taskWebSockets(unsigned long aInterval,long aIterations,Scheduler* aS,const char* name,taskJson* jsonTask) :
 taskInfo(aInterval, aIterations, aS, /*false)*/name),
 m_jsonTask(jsonTask){
  // Start a WebSocket server
  webSocket = new WebSocketsServer(81);
  webSocket->begin();                    // start the webSocketNicolas server
  webSocket->onEvent(std::bind(&taskWebSockets::webSocketEvent,this,std::placeholders::_1,std::placeholders::_2,std::placeholders::_3,std::placeholders::_4));    // if there's an incomming websocket message, go to function 'webSocketEvent'
#ifdef DEBUG_NICO
  Serial.println("WebSocket server started.");
#endif
}


bool taskWebSockets::Callback(){
        webSocket->loop();
        return true;
}
