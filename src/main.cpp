#include <Arduino.h>
#include <IRremoteESP8266.h>
#include <IRrecv.h>
#include <IRsend.h>

// PIN Configuration
#define PIN_RECV 14 //PIN wher IR receiver is connected
#define PIN_SEND 13 //PIN wher IR receiver is connected


IRrecv irrecv(PIN_RECV);//IRRecv object declaration
decode_results results; // IR message received
IRsend irsend(PIN_SEND);// IRSend object declaration

void setup() {
  irrecv.enableIRIn();//start the IR receiver
  irsend.begin();//start the IR send
}

void loop() {
  // software algorthm
  // if IR signal the signal treatment
  if (irrecv.decode(&results)){
    // NEC protocole is waited for
    if (results.decode_type==NEC){
      irsend.sendNEC(results.value);
    }
    irrecv.resume(); // receive new value
  }
  delay(100); // wait before check new message
  // software annex
  // WIFI check
  // MQTT check
  // MQTT signal treatment
  // MQTT info to send
}
