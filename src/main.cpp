// plafonnier_cuisine
/*
   The code is to  treat light by IR command and dennd state
   to MQTT broker.
   It will work in wifi with the following procedure:
   - at start WIFI start in AP mode
   - if it find known WIFI network it connects on them
   - if not it stays in AP mode.

   there is Web interface to find new network and configure it
   to autoconnect on them
 */

/*------------------- library including ------------------*/

#include <Arduino.h>

//library for WIFI and webserver
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <FS.h>
#include <ESP8266FtpServer.h>
#include <WebSocketsServer.h>
#include <ArduinoJson.h>

//librairy for IR control
#include <IRremoteESP8266.h>
#include <IRrecv.h>
#include <IRsend.h>

//library ticker
#include <Ticker.h>  //Ticker Library

#define TIME_WIFI_SCAN 2.0 // delay to perform new WIFI scan
/*--------------------------------------------------------*/

//-------------------- PIN Configuration -----------------*/
#define PIN_RECV 14 //PIN wher IR receiver is connected
#define PIN_SEND 13 //PIN wher IR receiver is connected
/*--------------------------------------------------------*/

/*----------------------- debug mode--------------------- */
#define DEBUG
/*--------------------------------------------------------*/

/*-------------web server declaration-----------*/
ESP8266WebServer server(80);
WebSocketsServer webSocket(81);
void handleNotFound();

void startServer() { // Start a HTTP server with a file read handler and an upload handler
        server.onNotFound(handleNotFound);    // if someone requests any other file or page, go to function 'handleNotFound'
                                              // and check if the file exists
        server.begin();                       // start the HTTP server
        Serial.println("HTTP server started.");
}


void webSocketEvent(uint8_t num, WStype_t type, uint8_t * payload, size_t lenght);
void startWebSocket() { // Start a WebSocket server
        webSocket.begin();                    // start the websocket server
        webSocket.onEvent(webSocketEvent);    // if there's an incomming websocket message, go to function 'webSocketEvent'
  #ifdef Debug
        Serial.println("WebSocket server started.");
  #endif
}
/*--------------------------------------------------------*/

/*-------------WIFI declaration-----------*/
#define AP_NAME "plafonnier"
#define AP_PASSWD_DEFAULT "default"
#define MAX_WIFI_SCAN 255
bool scanOn=false; //flag to say if WIFI scanning is ON or not
int numScan=0; // number of wifi network found
void prinScanResult(int networksFound){
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

                #ifdef DEBUG
                Serial.printf("%d network(s) found\n", networksFound);
                #endif
                for (int i = 0; i < networksFound; i++)
                {
                        #ifdef DEBUG
                        Serial.printf("%d: %s, Ch:%d (%ddBm) %s\n", i + 1, WiFi.SSID(i).c_str(), WiFi.channel(i), WiFi.RSSI(i), WiFi.encryptionType(i) == ENC_TYPE_NONE ? "open" : "");
                        #endif
                        JsonObject& wifiScan_wifi_i=wifiScan_wifi.createNestedObject();
                        wifiScan_wifi_i["numero"]=i;
                        wifiScan_wifi_i["name"]=WiFi.SSID(i).c_str();
                        wifiScan_wifi_i["puissance"]=WiFi.RSSI(i);
                }
                String chaine;
                root.printTo(chaine);
                webSocket.broadcastTXT(chaine);
        }
        scanOn=false;
}
void scanWifi(){
        if (!scanOn)
        {
                WiFi.scanNetworksAsync(prinScanResult);
                scanOn=true;
        }
}

//Initialize Ticker every 0.5s
bool WifiScanBool=false; // flag to start wifiscan
Ticker WifiScan(scanWifi,TIME_WIFI_SCAN * 1000 );
/*------------------------------------------------------*/

/*------------------ IR variable -------------------------*/
IRrecv irrecv(PIN_RECV);//IRRecv object declaration
decode_results results; // IR message received
IRsend irsend(PIN_SEND);// IRSend object declaration
/*--------------------------------------------------------*/

/*-------------------- WEB Server routine -------------*/
String getContentType(String filename); // convert the file extension to the MIME type
bool handleFileRead(String path);       // send the right file to the client (if it exists)
/*--------------------------------------------------------*/

/*------------- ftp server --------------------------------*/
FtpServer ftpSrv;
/*---------------------------------------------------------*/

void setup() {
/*----------------- Debug Serial print -------------------*/
        #ifdef DEBUG
        Serial.begin(115200);
        Serial.println();
        #endif
/*--------------------------------------------------------*/

/*--------------------IR setup----------------------------*/
        irrecv.enableIRIn();//start the IR receiver
        irsend.begin();//start the IR send
/*--------------------------------------------------------*/

//------------------ WIFi setup --------------------------*/
        #ifdef DEBUG
        Serial.print("Setting soft-AP ... ");
        #endif
        boolean result = WiFi.softAP(AP_NAME, AP_PASSWD_DEFAULT);
        #ifdef DEBUG
        Serial.println((result) ? "Ready" : "Failed!");
        #endif
        WifiScan.start();

/*-------------------- SPIFFS setup-------------------------*/
        SPIFFS.begin();
/*----------------------------------------------------------*/

/*-------------------------FTP setup-------------------------*/
        if (SPIFFS.begin()) {
                #ifdef DEBUG
                Serial.println("SPIFFS opened!");
                #endif
                ftpSrv.begin("esp8266","esp8266"); //username, password for ftp.  set ports in ESP8266FtpServer.h  (default 21, 50009 for PASV)
        }
/*-------------------------------------------------------------*/

/*----------------------WEBSocket Setup -----------------------*/
        startWebSocket();
/*----------------------WEB server Setup ----------------------*/
        startServer();
/*------------------------------------------------------------*/
}

void loop() {
/*----------------- software algorthm ------------------*/
// if IR signal the signal treatment
        /*if (irrecv.decode(&results)) {
                // NEC protocole is waited for
                if (results.decode_type==NEC) {
                        irsend.sendNEC(results.value);
                }
                irrecv.resume(); // receive new value
           }
           delay(100);*/// wait before check new message
/*-------------------------------------------------------------*/

/*------------ resume wifi_scan ticker ------------------------*/
        if (WifiScanBool) WifiScan.update();
/*-------------------------------------------------------------*/

/*-----------------------WEBsocket treatment ------------------*/
        webSocket.loop();
/*------------------------------------------------------*/

/*-----------------------WEB server treatment ------------------*/
        server.handleClient();
/*------------------------------------------------------*/

/*-----------------------FTP treatment ------------------*/
        ftpSrv.handleFTP();
/*-------------------------------------------------------*/
}

void handleNotFound(){ // if the requested file or page doesn't exist, return a 404 not found error
        if(!handleFileRead(server.uri())) {   // check if the file exists in the flash memory (SPIFFS), if so, send it
                server.send(404, "text/plain", "404: File Not Found");
        }
}

String getContentType(String filename) { // convert the file extension to the MIME type
        if (filename.endsWith(".html")) return "text/html";
        else if (filename.endsWith(".css")) return "text/css";
        else if (filename.endsWith(".js")) return "application/javascript";
        else if (filename.endsWith(".ico")) return "image/x-icon";
        return "text/plain";
}

bool handleFileRead(String path) { // send the right file to the client (if it exists)
        #ifdef DEBUG
        Serial.println("handleFileRead: " + path);
        #endif
        if (path.endsWith("/")) path += "index.html";   // If a folder is requested, send the index file
        String contentType = getContentType(path);      // Get the MIME type
        if (SPIFFS.exists(path)) {                      // If the file exists
                File file = SPIFFS.open(path, "r");     // Open it
                /*size_t sent = */ server.streamFile(file, contentType); // And send it to the client
                file.close();                           // Then close the file again
                return true;
        }
        #ifdef DEBUG
        Serial.println("\tFile Not Found");
        #endif
        return false;                                   // If the file doesn't exist, return false
}

void webSocketEvent(uint8_t num, WStype_t type, uint8_t * payload, size_t lenght) { // When a WebSocket message is received
        switch (type) {
        case WStype_DISCONNECTED:         // if the websocket is disconnected
      #ifdef DEBUG
                Serial.printf("[%u] Disconnected!\n", num);
      #endif
                break;
        case WStype_CONNECTED: {          // if a new websocket connection is established
                IPAddress ip = webSocket.remoteIP(num);
        #ifdef DEBUG
                Serial.printf("[%u] Connected from %d.%d.%d.%d url: %s\n", num, ip[0], ip[1], ip[2], ip[3], payload);
        #endif
                // Turn rainbow off when a new connection is established
        }
        break;
        case WStype_TEXT:                 // if new text data is received
                #ifdef DEBUG
                Serial.printf("[%u] get Text: %s\n", num, payload);
                #endif
                
                break;
        }
}
