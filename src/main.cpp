const char* appVersion = "0.0.3";

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

/*void setEngine(unsigned int engineIndex, unsigned int value) {
   if (value < COUNT_LOW) {
      string msg = "value is too low " + value;
      throw invalid_argument(msg);      
   }

   if (value > COUNT_HIGH) {
      string msg = "value is too high " + value;
      throw invalid_argument(msg);      
   }

   if ((engineIndex < 1) && (engineIndex > 4)) {
         string msg = "Invalid engine number" + engineIndex;
         throw invalid_argument(msg);
   }      
   ledcWrite(engineIndex, value);   
}*/

/*void setupEngines() {
   ledcSetup(1, 50, TIMER_WIDTH); // channel 1, 50 Hz, 16-bit width
   ledcAttachPin(GRIPPER_PIN, 1);   // GPIO 22 assigned to channel 1
   
   ledcSetup(2, 50, TIMER_WIDTH); // channel 2, 50 Hz, 16-bit width
   ledcAttachPin(19, 2);   // GPIO 19 assigned to channel 2
   
   ledcSetup(3, 50, TIMER_WIDTH); // channel 3, 50 Hz, 16-bit width
   ledcAttachPin(23, 3);   // GPIO 23 assigned to channel 3
   
   ledcSetup(4, 50, TIMER_WIDTH); // channel 4, 50 Hz, 16-bit width
   ledcAttachPin(18, 4);   // GPIO 18 assigned to channel 4
}*/
