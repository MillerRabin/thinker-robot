#include <ESPAsyncWebServer.h>

AsyncWebServer Server(80);


void onRequest(AsyncWebServerRequest *request){
  request->send(404);
}

void serverBegin() {
    Server.onNotFound(onRequest);
    Server.begin();
}