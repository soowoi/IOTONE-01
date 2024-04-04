#ifndef IOT-ONE-CONFIG_H
#define IOT-ONE-CONFIG_H

#include <WiFi.h>
#include <WiFiManager.h>
#include <PubSubClient.h>
#include <HS3UKA_PCF8574.h>

#define AUTO_PIN 25
#define MANUAL_PIN 34

#define MODE_AUTO 1
#define MODE_MANUAL 2
#define MODE_OFF 3
HS3UKA_PCF8574 pcf8574;


// Pin assignments
// Pin assignments
const int buttonPins[5] = {36, 26, 27, 14, 15 };
const int relayPins[5] = {1, 2, 3, 4, 5};

// Variables will change:
//int buttonPushCounter = 0;   // counter for the number of button presses
//int currentState1 = 0;         // current state of the button
//int lastState1 = 0;     // previous state of the button

//int currentState2 = 0;         // current state of the button
//int lastState2 = 0;     // previous state of the button

int current_mode = 0;

//unsigned long lastDebounceTime1 = 0;  // the last time the output pin was toggled
//unsigned long lastDebounceTime2 = 0;  // the last time the output pin was toggled
//unsigned long debounceTime = 50;

// Debounce variables
const unsigned long debounceDelay = 50;
unsigned long lastDebounceTime[5] = {0};
bool buttonStates[5] = {LOW};

//mqtt configuration
const char* mqtt_server = "mqtt.iotone-sys.com";
const int mqtt_port = 1883;
const char* mqtt_Client ="iotone-device";
const char* mqtt_username = "iotone04";
const char* mqtt_password = "yesnook#99";
const char* subscribe_topic ="iotone04/toggle/relays";
//wifimanager configuration
const char* ssid = "IOTONE_AP" ;
const char* passwd = "iotone-sys.com" ;

WiFiClient espClient;
WiFiManager wifiManager;
PubSubClient mqtt_client(espClient);
char msg[100];
int mqtt_fail = 0;


int getCurrentMode();
void setup();
void loop();
void toggleRelay(int relay_no);
void mn_fnc(); //manual function button handle
void auto_fnc(); //auto function handle
void loadConfiguration();
void saveConfiguration();
void mqtt_reconnect();
bool reConnectWiFi();


#endif
