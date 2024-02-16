#include <HS3UKA_PCF8574.h>
//#include <WiFi.h>
//#include <WiFiManager.h>

HS3UKA_PCF8574 pcf8574;
#define AUTO_PIN 25
#define MANUAL_PIN 34

void setup() {
    Serial.begin(115200);
    pcf8574.begin(0x20);  // Set the PCF8574 address
     pinMode(MANUAL_PIN, INPUT_PULLUP);
    pinMode(AUTO_PIN, INPUT_PULLUP);
   
}

void loop() {

    int autopin = digitalRead(AUTO_PIN);
    int manualpin = digitalRead(MANUAL_PIN);
    

    Serial.println(String(autopin)+String(manualpin));
    delay(2000);

  
}
