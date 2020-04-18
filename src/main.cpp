 #define COUNT_LOW 1638
 #define COUNT_HIGH 7864

 #define TIMER_WIDTH 16

#include "esp32-hal-ledc.h"
#include <Arduino.h>
#include <WiFi.h>
#include<vector>

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
   client.println("HTTP/1.1 " + code + " OK");
   client.println("Content-type:text/html");
   client.println("Connection: close");
   client.println();   
   client.println(text);    
}

bool isNewLine(char symbol) {
   return ((symbol == '\n') || (symbol == '\r'));
}


std::vector<String> parseRequest(String clientData) {
   unsigned int ln = clientData.length();   
   std::vector<String> pr;   
   unsigned int sIndex = 0;
   for (unsigned int i = 0; i < ln; i++) {
      const char smb = clientData[i];
      if (isNewLine(smb)) continue;
      if ((smb == ' ') || (i == ln - 1)) {
         if (sIndex != i)
            pr.push_back(clientData.substring(sIndex, i));
         sIndex = i + 1;
      }      
   }
   return pr;
}

String getQuery(String URL) {
   int pos = URL.indexOf("?");
   if (pos == -1) return "";   
   return URL.substring(pos+1, URL.length());
}

std::vector<unsigned long> getEngineParameters(String query) {
   std::vector<unsigned long> res;
   int pos = query.indexOf(":");
   if (pos == -1) return res;
   String fp = query.substring(0, pos);
   String sp = query.substring(pos+1, query.length());
   res.push_back(fp.toInt());
   res.push_back(sp.toInt());   
   return res;
}

void setEngine(unsigned long engine, unsigned long angle) {
   ledcWrite(engine, angle);
}

void route(WiFiClient client, String clientData) {
   std::vector<String> pr = parseRequest(clientData);
   if (pr.size() < 2) {
      send(client, "400", "Invalid request");
      return;
   }
   String url = pr[1];   
   String query = getQuery(url);
   std::vector<unsigned long> ep;
   try {
      ep = getEngineParameters(query);
      if (ep.size() < 2) {
         send(client, "400", "Invalid query");
         return;
      }
   } catch (...){
      send(client, "400", "Invalid query");
      return;
   }

   unsigned long engine = ep[0];
   unsigned long value = ep[1];

   if ((engine < 1) || (engine > 4)) {
      send(client, "400", "Invalid engine number");
      return;
   }

   if (value < COUNT_LOW) {
      send(client, "400", "Value too low");
      return;
   }

   if (value > COUNT_HIGH) {
      send(client, "400", "Value too high");
      return;
   }

   setEngine(engine, value);
   send(client, "200", "OK");
}


String readClient(WiFiClient client) {
   String header = "";
   String currentLine = "";
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
   String clientData = readClient(client);
   route(client, clientData);   
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
