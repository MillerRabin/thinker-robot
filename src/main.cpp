#define COUNT_LOW 1638
#define COUNT_HIGH 7864

#define TIMER_WIDTH 16

#include "esp32-hal-ledc.h"
#include <Arduino.h>
#include <WiFi.h>
#include<vector>
#include "HTTPRequest.h"
#include "armErrors.h"

#define Engine vector<unsigned int>
#define Engines vector<Engine>

const char* ssid = "DragonSlayer";
const char* password = "ifyouwanttohave";

WiFiServer server(80);

void WiFIConnect() {
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }  
  Serial.println("");
  Serial.println("WiFi connected.");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  server.begin();
}


void send(WiFiClient client, String code, String text) {
   client.println("HTTP/1.1 " + code);   
   client.println("Content-Type: text/plain");
   String ln = String(text.length());
   String ls = "Content-Length: " + ln;
   client.println(ls);
   client.println("Access-Control-Allow-Origin: *");   
   client.println("Connection: close");
   client.println();   
   client.println(text);    
}

Engine getEngineParameters(string query) {
   Engine res;
   size_t pos = query.find(":");
   if (pos == string::npos) return res;
   string fp = query.substr(0, pos);
   string sp = query.substr(pos+1, query.length() - pos + 1);      
   res.push_back(atoi(fp.c_str()));
   res.push_back(atoi(sp.c_str()));
   return res;
}


Engines getEngines(vector<string> queryParams) {
   unsigned int ln = queryParams.size();
   Engines res;
   for (unsigned int i = 0; i < ln; i++) {
      string qr = queryParams[i];
      Engine engine = getEngineParameters(qr);
      res.push_back(engine);
   }  
   return res;    
}


void setEngine(unsigned int engineIndex, unsigned int value) {
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
}


void setEngines(Engines engines) {
   unsigned int ln = engines.size();
   for (unsigned int i =0; i < ln; i++) {
      Engine ep = engines[i];
      if (ep.size() < 2) {
         string msg = "Invalid parameter for engine " + i;
         throw invalid_argument(msg);
      }
      unsigned int engine = ep[0];
      unsigned int value = ep[1];      
      setEngine(engine, value);
   }
}

unsigned int route(string clientData) {
   HTTPRequest* hr = new HTTPRequest(clientData);   
   vector<string> qr = hr->queryParams;      
   delete hr;
   if (qr.size() < 4) {            
      return 1;
   }
         
   Engines en = getEngines(qr);
   if (en.size() < 4) {            
      return 1;
   }
      
   setEngines(en);         
   return 0;
}


string readClient(WiFiClient client) {
   string header = "";
   string currentLine = "";
   while (client.connected()) {    
      if (!client.available()) return header;
      char c = client.read();
      header += c;
      if (c == '\n') {            
         if (currentLine.length() == 0) {            
            return header;
         }
         currentLine = "";
         continue;
      }      
      if (c == '\r') 
         continue;
      currentLine += c;      
   }
   return header;
}


void dispatchClient() {
   WiFiClient client = server.available();      
   if (!client) return;   
   string clientData = readClient(client);
   try {
      unsigned int res = route(clientData); 
      ArmErrors::Error rc = ArmErrors::get(res);
      if (res == 0) {
         send(client, "200", "OK");
      } else {
         throw invalid_argument("Must be 4 parameters for 4 engines");
      }
   } catch (const std::exception& e) {
      Serial.write(e.what());
      Serial.write("\n");
      send(client, "400", e.what());
   } catch (...) {
      Serial.write("Error");
   }   
   client.stop();   
}


void setupEngines() {
   ledcSetup(1, 50, TIMER_WIDTH); // channel 1, 50 Hz, 16-bit width
   ledcAttachPin(22, 1);   // GPIO 22 assigned to channel 1
   
   ledcSetup(2, 50, TIMER_WIDTH); // channel 2, 50 Hz, 16-bit width
   ledcAttachPin(19, 2);   // GPIO 19 assigned to channel 2
   
   ledcSetup(3, 50, TIMER_WIDTH); // channel 3, 50 Hz, 16-bit width
   ledcAttachPin(23, 3);   // GPIO 23 assigned to channel 3
   
   ledcSetup(4, 50, TIMER_WIDTH); // channel 4, 50 Hz, 16-bit width
   ledcAttachPin(18, 4);   // GPIO 18 assigned to channel 4
}

void setup() {
   setupEngines();
   Serial.begin(115200);
   WiFIConnect();
   server.begin();
}

void loop() {
   dispatchClient();
}
