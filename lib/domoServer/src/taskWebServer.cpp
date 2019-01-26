#include "taskWebServer.h"

String getContentType(String filename) { // convert the file extension to the MIME type
           if (filename.endsWith(".html")) return "text/html";
           else if (filename.endsWith(".css")) return "text/css";
           else if (filename.endsWith(".js")) return "application/javascript";
           else if (filename.endsWith(".ico")) return "image/x-icon";
           return "text/plain";
      }

bool taskWebServer::handleFileRead(String path) { // send the right file to the client (if it exists)
    #ifdef DEBUG_NICO
    Serial.println("handleFileRead: " + path);
    #endif
    if (path.endsWith("/")) path += "index.html";   // If a folder is requested, send the index file
    #ifdef DEBUG_NICO
    Serial.println("handleFileRead modified: " + path);
    #endif
    String contentType = getContentType(path);      // Get the MIME type
    #ifdef DEBUG_NICO
    Serial.println("content Type" + contentType);
    #endif
    if (SPIFFS.exists(path)) {                      // If the file exists
      File file = SPIFFS.open(path, "r");     // Open it
      int siz = file.size();
      //server->sendHeader("Content-Length", (String)siz);
      /*server->send(200, contentType, "");
      #ifdef DEBUG_NICO
      Serial.print("file found.");
      Serial.print("size:");
      Serial.println(siz);
      #endif
      char buf[1024];
      while(siz > 0) {
        size_t len = std::min((int)(sizeof(buf) - 1), siz);
        #ifdef DEBUG_NICO
        Serial.print("buffer sent size:");
        Serial.println(len);
        #endif
        file.read((uint8_t *)buf, len);
        server->sendContent(buf);
        siz -= len;
        }
      server->sendContent("");*/
      server->streamFile(file, contentType); // And send it to the client
      #ifdef DEBUG_NICO
      Serial.println("file streamed");
      #endif
      file.close();                           // Then close the file again
      return true;
      }
    #ifdef DEBUG_NICO
    Serial.println("\tFile Not Found");
    #endif
    return false;                                   // If the file doesn't exist, return false
    }

void taskWebServer::handleNotFound(){ // if the requested file or page doesn't exist, return a 404 not found error
           if(!(this->handleFileRead(server->uri()))) {   // check if the file exists in the flash memory (SPIFFS), if so, send it
                   server->send(404, "text/plain", "404: File Not Found");
           }
         }

void taskWebServer::startServer() {

   }

bool taskWebServer::Callback() {
  server->handleClient();
  return true;
}

taskWebServer::taskWebServer(unsigned long aInterval,long aIterations,Scheduler* aS,const char* name) :
  taskInfo(aInterval, aIterations, aS/*false)*/,name){
  // Start a HTTP server with a file read handler and an upload handler
  // if someone requests any other file or page, go to function 'handleNotFound'
  // and check if the file exists
  //SPIFFS.begin();
  #ifdef DEBUG_NICO
         Serial.println("start webserver...");
  #endif
  server=new  ESP8266WebServer(80);
  server->onNotFound(std::bind(&taskWebServer::handleNotFound,this));
  server->begin();                       // start the HTTP server
  Serial.println("HTTP server started.");
}
