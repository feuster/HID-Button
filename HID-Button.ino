//--------------------------------------------//
//              HID-Button V1.0               //
//         (c) Alexander Feuster 2024         //
//  http://www.github.com/feuster/HID-Button  //
//--------------------------------------------//

// define german keyboard layout
#define kbd_de_de
#include "DigiKeyboard.h"

// variable definitions
// input pin for pushbutton
const int buttonPin = PB0;
// for checking the state of a pushButton
int previousButtonState = HIGH;
// state of te builtin LED
int previousLedState = HIGH;


// start the initialization
void setup() {
  // make the pushButton pin an input:
  pinMode(buttonPin, INPUT_PULLUP);

  // wait some time before first run, to give target time to initialize
  DigiKeyboard.delay(2500);

  // blink the builtin LED as start confirmation
  for (int i = 0; i < 10; i++)
  {
    digitalWrite(LED_BUILTIN, previousLedState);
    previousLedState = !previousLedState;
    delay(100);
  }
  digitalWrite(LED_BUILTIN, LOW);
}

void loop() {
  // program loop start

  // read the pushbutton:
  int buttonState = digitalRead(buttonPin);

  // if the button state has changed, and it's currently pressed:
  if ((buttonState != previousButtonState) && (buttonState == HIGH)) 
  {
    // turn the LED on while message is typed
    digitalWrite(LED_BUILTIN, HIGH);

    // type out a message
    DigiKeyboard.print("USER");
    DigiKeyboard.delay(75);
    DigiKeyboard.sendKeyStroke(43); //translates to TAB
    DigiKeyboard.delay(75);
    DigiKeyboard.print("PASSWORD");
    DigiKeyboard.delay(75);
    DigiKeyboard.sendKeyStroke(0x28); //translates to ENTER
    DigiKeyboard.delay(75);

    // turn the LED off after message has been typed
    digitalWrite(LED_BUILTIN, LOW);
  }

  // forced delay for simple debouncing the hardware button
  delay(100);

  // save the current button state for comparison next time:
  previousButtonState = buttonState;

  //program loop end and repeat
}