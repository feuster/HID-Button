//--------------------------------------------//
//              HID-Button V1.1               //
//         (c) Alexander Feuster 2024         //
//  http://www.github.com/feuster/HID-Button  //
//--------------------------------------------//

// define german keyboard layout
#define kbd_de_de
#include "DigiKeyboard.h"
#include "Bounce2.h"

/// --------------------------------------------------------------------------------------------
/// login credentials which can be changed to real credentials
/// --------------------------------------------------------------------------------------------
const char USER[] = "USER";
const char PASSWORD[] = "PASSWORD";
// do not change code behind this line if not needed !!!
/// --------------------------------------------------------------------------------------------

/// --------------------------------------------------------------------------------------------
/// hardware definitions
/// --------------------------------------------------------------------------------------------
// input pin for pushbutton
const int buttonPin = PB0;
// state of te builtin LED
int previousLedState = HIGH;
// create a Bounce2::Button object
Bounce2::Button button = Bounce2::Button();

/// --------------------------------------------------------------------------------------------
/// start the initialization
/// --------------------------------------------------------------------------------------------
void setup() {
  // configure the pushbutton pin as input:
  pinMode(buttonPin, INPUT_PULLUP);

  // wait some time before first run, to give hardware time to initialize
  DigiKeyboard.delay(2500);

  // Bounce2::Button configuration
  button.attach(buttonPin, INPUT_PULLUP);
  button.interval(100);
  button.setPressedState(LOW); 

  // blink the builtin LED as start confirmation
  for (int i = 0; i < 10; i++)
  {
    digitalWrite(LED_BUILTIN, previousLedState);
    previousLedState = !previousLedState;
    delay(100);
  }
  digitalWrite(LED_BUILTIN, LOW);
}

/// program loop
void loop() {
  // read the pushbutton state
  button.update();

  // if the button state has changed, and it's currently pressed type login credentials
  if (button.pressed()) 
  {
    // turn the builtin LED on while login is typed
    digitalWrite(LED_BUILTIN, HIGH);

    // type out login credentials (remark: sendKeyStroke(0) is used as workaround for compatibility problems with older hardware and can be removed if this isn't needed)
    DigiKeyboard.sendKeyStroke(0);
    DigiKeyboard.print(USER);
    DigiKeyboard.delay(125);
    DigiKeyboard.sendKeyStroke(0);
    DigiKeyboard.sendKeyStroke(43); //translates to TAB
    DigiKeyboard.delay(125);
    DigiKeyboard.sendKeyStroke(0);
    DigiKeyboard.print(PASSWORD);
    DigiKeyboard.delay(125);
    DigiKeyboard.sendKeyStroke(0);
    DigiKeyboard.sendKeyStroke(KEY_ENTER);

    // turn the builtin LED off after login has been typed
    digitalWrite(LED_BUILTIN, LOW);
  }

  ///program loop ends and will repeat now in a new cycle
}