#include <HS3UKA_PCF8574.h>


#define AUTO_PIN 25
#define MANUAL_PIN 34
#define BUTTON_1 36
#define BUTTON_2 39

#define MODE_AUTO 1
#define MODE_MANUAL 2
#define MODE_OFF 3

HS3UKA_PCF8574 pcf8574;
// Variables will change:
int buttonPushCounter = 0;   // counter for the number of button presses
int currentState1 = 0;         // current state of the button
int lastState1 = 0;     // previous state of the button

int currentState2 = 0;         // current state of the button
int lastState2 = 0;     // previous state of the button
int current_mode = 0;

unsigned long lastDebounceTime = 0;  // the last time the output pin was toggled
unsigned long debounceTime = 50;


int getCurrentMode() {
  int autopin = digitalRead(AUTO_PIN);
  int manualpin = digitalRead(MANUAL_PIN);
  
  if (autopin == 0 && manualpin == 1 ) {
    return MODE_AUTO;
  }
  else if (autopin == 1 && manualpin == 0 ) {
    return MODE_MANUAL;
  }
  else if (autopin == 1 && manualpin == 1 ) {
    return MODE_OFF;
  }
  else {
    return 4;
  }
}

void setup() {
  Serial.begin(115200);
  pcf8574.begin(0x20);  // Set the PCF8574 address
  current_mode = getCurrentMode();
  pinMode(MANUAL_PIN, INPUT_PULLUP);
  pinMode(AUTO_PIN, INPUT_PULLUP);
  pinMode(BUTTON_1, INPUT_PULLUP);
  pinMode(BUTTON_2, INPUT_PULLUP);
  delay(500);

}

void toggleRelay(int relay_no) {

  pcf8574.digitalWrite(relay_no, !pcf8574.digitalRead(relay_no));

}

void closeAllRelay() {
  pcf8574.digitalWrite(1, HIGH);
  pcf8574.digitalWrite(2, HIGH);
}

void loop() {


  currentState1 = digitalRead(BUTTON_1);
  currentState2 = digitalRead(BUTTON_2);
  
  current_mode = getCurrentMode();
  Serial.println("Current Mode ="+String(current_mode) );
  if ( current_mode == MODE_MANUAL) {
    
    if ( currentState2 != lastState2 && (millis() - lastDebounceTime) > debounceTime && currentState2 == LOW) {
      toggleRelay(3);
    }
    if ( currentState1 != lastState1 && (millis() - lastDebounceTime) > debounceTime && currentState1 == LOW) {
      toggleRelay(1);
    }

   
  }

  /*
  else if (current_mode == MODE_OFF) {
    closeAllRelay();
  }*/

  Serial.println(String(digitalRead(MANUAL_PIN)) + String(digitalRead(AUTO_PIN)));

  lastState1 = currentState1;
  lastState2 = currentState2;
  delay(10); // Optional delay for loop rate control


}
