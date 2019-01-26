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

//librairy for IR control
/*#include <IRremoteESP8266.h>
#include <IRrecv.h>
#include <IRsend.h>*/

//-------------------- PIN Configuration -----------------*/
#define PIN_RECV 14 //PIN wher IR receiver is connected
#define PIN_SEND 13 //PIN wher IR receiver is connected
/*--------------------------------------------------------*/

/*----------------------- DEBUG_NICO mode--------------------- */

/*--------------------------------------------------------*/

/*-------------WIFI declaration-----------*/
// to be included before domoserver.h inclusion
#define AP_NAME "plafonnier"
#define AP_PASSWD_DEFAULT "default"
/*-----------------------------------------*/

/*------------------ IR variable -------------------------*/
//IRrecv irrecv(PIN_RECV);//IRRecv object declaration
//decode_results results; // IR message received
//IRsend irsend(PIN_SEND);// IRSend object declaration
/*--------------------------------------------------------*/
#include <domoServer.h>
#include <TaskScheduler.h>

Scheduler *ts; // use scheduler in skecth is mandatory. I cannot declare it in included library
domoServer* domo;

void setup() {
/*----------------- DEBUG_NICO Serial print -------------------*/
        Serial.setDebugOutput(false);
        #ifdef DEBUG_NICO
        Serial.begin(115200);
        Serial.println("bonjour");
        #endif
/*--------------------------------------------------------*/

/*--------------------IR setup----------------------------*/
  //      irrecv.enableIRIn();//start the IR receiver
  //      irsend.begin();//start the IR send
        ts=new Scheduler();
        domo=new domoServer(ts);
}

void loop() {

  domo->execute();
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
}
