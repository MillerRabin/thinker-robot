const char* appVersion = "1.0.0";

#define COUNT_LOW 1638
#define COUNT_HIGH 7864

#define TIMER_WIDTH 16

#include <Arduino.h>
#include <WiFi.h>
#include <ESPAsyncWebServer.h>
#include "updateHandler.h"
#include "initServer.h"
#include "engineHandler.h"

const char* ssid = "DragonSlayer";
const char* password = "ifyouwanttohave";
const char* http_username = "admin";
const char* http_password = "ifyouwanttohave";

bool enableWIFI() {
   WiFi.mode(WIFI_STA);
   WiFi.begin(ssid, password);   
   if (WiFi.waitForConnectResult() != WL_CONNECTED) {      
      Serial.printf("WiFi Failed!\n");
      return false;
   }
  Serial.println("");
  Serial.println("WiFi connected.");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  return true;
}

void enableVersion() {
  Server.on("/version", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(200, "text/plain", String(appVersion));
  });
}

void setup(){
  Serial.begin(115200);
  if (!enableWIFI())
      return;
       
  enableVersion();
  enableUpdate();
  enableEngines();
  serverBegin();
}

void loop() {
   if(shouldReboot){
    Serial.println("Rebooting...");
    delay(100);
    ESP.restart();
  }
}
