//--------------------------------------------//
//              HID-Button V1.2               //
//         (c) Alexander Feuster 2024         //
//  http://www.github.com/feuster/HID-Button  //
//--------------------------------------------//

// define german keyboard layout
#define kbd_de_de
// includes
#include "DigiKeyboard.h"
#include "Bounce2.h"

/// --------------------------------------------------------------------------------------------
/// login credentials pair 1 for short button press which can be changed to real credentials
/// --------------------------------------------------------------------------------------------
const char USER1[] = "USER1";
const char PASSWORD1[] = "PASSWORD1";
/// --------------------------------------------------------------------------------------------
/// login credentials pair 2 for long button press which can be changed to real credentials
/// --------------------------------------------------------------------------------------------
const char USER2[] = "USER2";
const char PASSWORD2[] = "PASSWORD2";
// do not change code behind this line if not needed !!!
/// --------------------------------------------------------------------------------------------

/// --------------------------------------------------------------------------------------------
/// hardware definitions
/// --------------------------------------------------------------------------------------------
// input pin for pushbutton (PB0-PB4 with according wiring change, never use PB5 which is RESET)
const int buttonPin = PB0;
// state of te builtin LED
int previousLedState = HIGH;
// time to define a long button press in ms
int longPressTimeSpan = 1000;
// create a Bounce2::Button object
Bounce2::Button button = Bounce2::Button();
// map an own virtual key for TAB which is missing in DigiKeyboard.h
const int KEY_TAB = 43;

/// --------------------------------------------------------------------------------------------
/// start the initialization
/// --------------------------------------------------------------------------------------------
void setup() {
  // configure the pushbutton pin as input with internal pullup resistor
  pinMode(buttonPin, INPUT_PULLUP);

  // Bounce2::Button configuration
  button.attach(buttonPin, INPUT_PULLUP);
  button.interval(5);
  button.setPressedState(LOW); 

  // wait some time before first run, to give hardware time to initialize and blink the builtin LED as start confirmation
  for (int i = 0; i < 25; i++)
  {
    digitalWrite(LED_BUILTIN, previousLedState);
    previousLedState = !previousLedState;
    DigiKeyboard.delay(100);
  }
  digitalWrite(LED_BUILTIN, LOW);
}

/// --------------------------------------------------------------------------------------------
/// program loop
/// --------------------------------------------------------------------------------------------
void loop() {
  // read the pushbutton state once per loop
  button.update();

  // validate if button was pressed short
  if (button.released() && button.previousDuration() < longPressTimeSpan) 
  {
    // type login credentials pair 1
    Type(USER1, PASSWORD1);
  }
  // validate if button was pressed long
  else if (button.released() && button.previousDuration() > longPressTimeSpan) 
  {
    // type login credentials pair 2
    Type(USER2, PASSWORD2);
  }
  else
  {
    // keep alive internal update
    DigiKeyboard.delay(125);
  }

  ///program loop ends and will repeat now in a new cycle
}

/// --------------------------------------------------------------------------------------------
/// type the desired user credentials as USB HID keyboard
/// --------------------------------------------------------------------------------------------
void Type(char User[], char Password[])
{
    // turn the builtin LED on while login is typed
    digitalWrite(LED_BUILTIN, HIGH);

    // type login credentials pair (remark: sendKeyStroke(0) is used as workaround for compatibility problems with older hardware and can be removed if this isn't needed)
    DigiKeyboard.sendKeyStroke(0);
    DigiKeyboard.print(User);
    DigiKeyboard.delay(125);
    DigiKeyboard.sendKeyStroke(0);
    DigiKeyboard.sendKeyStroke(KEY_TAB);  // switches from the user input field to the password input field (should work for 99% of login masks)
    DigiKeyboard.delay(125);
    DigiKeyboard.sendKeyStroke(0);
    DigiKeyboard.print(Password);
    DigiKeyboard.delay(125);
    DigiKeyboard.sendKeyStroke(0);
    DigiKeyboard.sendKeyStroke(KEY_ENTER); // confirms login input

    // turn the builtin LED off after login has been typed
    digitalWrite(LED_BUILTIN, LOW);
}
