 #define COUNT_LOW 1638
 #define COUNT_HIGH 7864

 #define TIMER_WIDTH 16

#include "esp32-hal-ledc.h"
#include <Arduino.h>

void setup() {
   ledcSetup(1, 50, TIMER_WIDTH); // channel 1, 50 Hz, 16-bit width
   ledcAttachPin(22, 1);   // GPIO 22 assigned to channel 1
   
   ledcSetup(2, 50, TIMER_WIDTH); // channel 2, 50 Hz, 16-bit width
   ledcAttachPin(19, 2);   // GPIO 19 assigned to channel 2
   
   ledcSetup(3, 50, TIMER_WIDTH); // channel 3, 50 Hz, 16-bit width
   ledcAttachPin(23, 3);   // GPIO 23 assigned to channel 3
   
   ledcSetup(4, 50, TIMER_WIDTH); // channel 4, 50 Hz, 16-bit width
   ledcAttachPin(18, 4);   // GPIO 18 assigned to channel 4
}

void loop() {
  ledcWrite(1, COUNT_LOW);
  delay(2000);
  ledcWrite(1, COUNT_LOW + 700);
  delay(2000);
/*   for (int i=COUNT_HIGH ; i < COUNT_LOW ; i=i-100)
   {
      ledcWrite(1, i);       // sweep servo 1
      delay(200);
   }

   for (int i=COUNT_LOW ; i < COUNT_HIGH ; i=i+100)
   {
      ledcWrite(1, i);       // sweep servo 1
      delay(200);
   }*/
    
   /*for (int i=COUNT_LOW ; i < COUNT_HIGH ; i=i+100)
   {
      ledcWrite(2, i);       // sweep servo 2
      delay(200);
   }

   for (int i=COUNT_LOW ; i < COUNT_HIGH ; i=i+100)
   {
      ledcWrite(3, i);       // sweep the servo
      delay(200);
   }

   for (int i=COUNT_LOW ; i < COUNT_HIGH ; i=i+100)
    {
      ledcWrite(4, i);       // sweep the servo
      delay(200);
    }*/
}
