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

//librairy for IR control
#include <IRremoteESP8266.h>
#include <IRrecv.h>
#include <IRsend.h>
/*--------------------------------------------------------*/

//-------------------- PIN Configuration -----------------*/
#define PIN_RECV 14 //PIN wher IR receiver is connected
#define PIN_SEND 13 //PIN wher IR receiver is connected
/*--------------------------------------------------------*/

/*----------------------- debug mode--------------------- */
#define DEBUG
/*--------------------------------------------------------*/


/*-------------WIFI and web server declaration-----------*/
#define AP_NAME "plafonnier"
#define AP_PASSWD_DEFAULT "default"
ESP8266WebServer server(80);
/*--------------------------------------------------------*/

/*------------------ IR variable -------------------------*/
IRrecv irrecv(PIN_RECV);//IRRecv object declaration
decode_results results; // IR message received
IRsend irsend(PIN_SEND);// IRSend object declaration
/*--------------------------------------------------------*/

/*-------------------- WEB Server routine -------------*/
void handleRoot() {
  server.send(200, "text/html", "<h1>You are connected</h1>");
}
/*--------------------------------------------------------*/

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

  Serial.println((result) ?"Ready" :"Failed!");
  #ifdef DEBUG
#endif

  server.on("/", handleRoot);
  server.begin();
#ifdef DEBUG
  Serial.println("HTTP server started");
#endif
/*-------------------------------------------------------*/
}

void loop() {
/*----------------- software algorthm ------------------*/
  // if IR signal the signal treatment
  if (irrecv.decode(&results)){
    // NEC protocole is waited for
    if (results.decode_type==NEC){
      irsend.sendNEC(results.value);
    }
    irrecv.resume(); // receive new value
  }
  delay(100); // wait before check new message
/*------------------------------------------------------*/
  server.handleClient();

}
