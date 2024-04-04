#include "iot-one-config.h"

int getCurrentMode() {
  int autopin = digitalRead(AUTO_PIN);
  int manualpin = digitalRead(MANUAL_PIN);

  //Serial.println("autopin=" + String(autopin) + ",manualpin=" + String(manualpin));

  if (autopin == 0 && manualpin == 1 ) {
    return MODE_AUTO;
  }
  else if (autopin == 1 && manualpin == 0 ) {
    return MODE_MANUAL;
  }
  else {
    return MODE_OFF;
  }
}

void mqtt_callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.println("] ");
  String message = "";
  
  if (current_mode != MODE_AUTO)
    return ;

  if(strcmp(topic, subscribe_topic) == 0){
    
    for(int i=0; i< length; i++){
      message += (char)payload[i];
    }
  }
  Serial.println("Message="+message);
  
  int relayNumber = message.toInt();
  if( relayNumber >= 1 && relayNumber <=5) {

    int relayIndex = relayNumber -1;
    toggleRelay(relayIndex);
    Serial.print("Toggled relay: ");
    Serial.println(relayNumber);
  }else{
    Serial.println("Invalid relay number in message");
  }

  
}

void setup() {
  Serial.begin(115200);
  pcf8574.begin(0x20);  // Set the PCF8574 address
  current_mode = getCurrentMode();
  pinMode(MANUAL_PIN, INPUT_PULLUP);
  pinMode(AUTO_PIN, INPUT_PULLUP);

  // Initialize button pins with pull-up resistors
  for (int i = 0; i < 5; i++) {
    pinMode(buttonPins[i], INPUT_PULLUP);
  }

  wifiManager.setConnectTimeout(15);
  if (!wifiManager.autoConnect( ssid, passwd)) {
    Serial.println("Failed to connect, starting config portal...");
    reconnectWiFi();
  }
  mqtt_client.setServer(mqtt_server, mqtt_port);
  mqtt_client.setCallback(mqtt_callback);
}

void toggleRelay(int relay_no) {

  pcf8574.digitalWrite(relayPins[relay_no], !pcf8574.digitalRead(relayPins[relay_no]));

}

void auto_fnc(){
  //network resolution
  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("Connection lost, attemping reconnection..");
    reconnectWiFi();
  }

  if (!mqtt_client.connected()) {
    mqtt_reconnect();
  }
  mqtt_client.loop(); 
}

void mn_fnc() {
  if (current_mode != MODE_MANUAL)
    return ;
  //Serial.println("Button State1");
  for (int i = 0; i < 5 ; i++) {
    if (millis() - lastDebounceTime[i] >= debounceDelay) {
      if (digitalRead(buttonPins[i]) != buttonStates[i]) {
        buttonStates[i] = !buttonStates[i];
        if (buttonStates[i] == LOW) {  // Button pressed

          toggleRelay(i);
        }
      }
      lastDebounceTime[i] = millis();
    }
  }
}

void loop() {

  current_mode = getCurrentMode();
  mn_fnc();
  auto_fnc();
  

  delay(10); // Optional delay for loop rate control

}


void loadConfiguration() {

}


void saveConfiguration() {

}

bool reconnectWiFi() {
  int retryCount = 0;
  while (WiFi.status() != WL_CONNECTED && retryCount < 5) {
    Serial.print("Reconnecting attemp: ");
    Serial.println( retryCount + 1);
    delay(500);
    WiFi.begin(ssid, passwd);
  }
  if ( retryCount == 5 && WiFi.status() != WL_CONNECTED) {
    ESP.restart();
  }
}

void mqtt_reconnect() {
  Serial.println("Connection to MQTT..");
  if ( WiFi.status() == WL_CONNECTED) {
    if (mqtt_client.connect(mqtt_Client, mqtt_username, mqtt_password)) {
      Serial.println("mqtt.iotone-sys.com connected");
      mqtt_client.subscribe(subscribe_topic);
    }
    else {
      Serial.print("failed connect mqtt client with error: ");
      Serial.print(mqtt_client.state());
      Serial.println();
    }
  }
  else {
    Serial.println("WiFi not connected, skipping MQTT reconnect");
  }
}
