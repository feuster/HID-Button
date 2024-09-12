//--------------------------------------------//
//              HID-Button V1.5               //
//         (c) Alexander Feuster 2024         //
//  http://www.github.com/feuster/HID-Button  //
//                                            //
//             powered by coffee              //
//                                            //
//                    ( (                     //
//                    ) )                     //
//                  .______.                  //
//                  |      |                  //
//                  |      |                  //
//                  |______|                  //
//                  \______/                  //
//                                            //
//--------------------------------------------//

// defines
#define kbd_de_de             // german keyboard layout
#define use_null_keystroke    // enable compatibility function sendNullKeyStroke()
#define enable_reboot         // enabled reboot on button press

// includes
#include "DigiKeyboard.h"     // library for emulating a HID keyboard
#include "Bounce2.h"          // library for handling the button hardware bouncing
#ifdef enable_reboot
  #include <avr/wdt.h>        // AVR watchdog library
#endif

/// -----------------------------------------------------------------------------------------------------------------------
/// login credentials pair 1 for short button press which can be changed to real credentials
/// -----------------------------------------------------------------------------------------------------------------------
const char USER1[] = "USER1";
const char PASSWORD1[] = "PASSWORD1";
/// -----------------------------------------------------------------------------------------------------------------------
/// login credentials pair 2 for long button press which can be changed to real credentials
/// -----------------------------------------------------------------------------------------------------------------------
const char USER2[] = "USER2";
const char PASSWORD2[] = "PASSWORD2";
/// -----------------------------------------------------------------------------------------------------------------------
/// do not change code behind this line if not needed !!!
/// -----------------------------------------------------------------------------------------------------------------------

/// -----------------------------------------------------------------------------------------------------------------------
/// hardware & software definitions
/// -----------------------------------------------------------------------------------------------------------------------
// input pin for pushbutton (PB0-PB4 with according wiring change, never use PB5 which is RESET)
const int buttonPin = PB0;
// state of the builtin LED for blinking
int previousLedState = HIGH;
// time to define a long button press in ms
int longPressTimeSpan = 1000;
// time to define a maximal button press in ms (must be longer than longPressTimeSpan)
int maxPressTimeSpan = 5000;
// create a Bounce2::Button object
Bounce2::Button button = Bounce2::Button();
// map an own virtual key for TAB which is missing in DigiKeyboard.h
const int KEY_TAB = 43;

/// -----------------------------------------------------------------------------------------------------------------------
/// start the hardware initialization
/// -----------------------------------------------------------------------------------------------------------------------
void setup() {
  // wait some time before first run, to give hardware time to initialize and blink the builtin LED as start confirmation
  for (int i = 0; i < 30; i++)
  {
    blink(100);
  }

#ifdef enable_reboot
  // disable watchdog for reboot function
  wdt_disable();
#endif

  // configure the pushbutton pin as input with internal pullup resistor
  pinMode(buttonPin, INPUT_PULLUP);

  // Bounce2::Button configuration
  button.attach(buttonPin, INPUT_PULLUP);
  button.interval(5);
  button.setPressedState(LOW); 

  // forced LED switch off in case LED is still on after loop
  digitalWrite(LED_BUILTIN, LOW);
  DigiKeyboard.delay(25);
}

/// -----------------------------------------------------------------------------------------------------------------------
/// program loop
/// -----------------------------------------------------------------------------------------------------------------------
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
  else if (button.released() && button.previousDuration() > longPressTimeSpan && button.previousDuration() < maxPressTimeSpan) 
  {
    // type login credentials pair 2
    Type(USER2, PASSWORD2);
  }
#ifdef enable_reboot  
  // validate if button was pressed over the maximal time
  else if (button.isPressed() && button.currentDuration() > maxPressTimeSpan)
  {
    // blink as signalization for reboot
    for (int i = 0; i < 10; i++)
    {
      blink(100);
    }
    digitalWrite(LED_BUILTIN, previousLedState);

    //execute reboot
    reboot();
  }
#endif  

  // keep alive USB HID communication
  DigiKeyboard.delay(25);

  ///program loop ends here and will repeat now in a new cycle
}

/// -----------------------------------------------------------------------------------------------------------------------
/// type the desired user credentials as USB HID keyboard
/// -----------------------------------------------------------------------------------------------------------------------
void Type(char User[], char Password[])
{
    // turn the builtin LED on while login is typed
    DigiKeyboard.delay(25);
    digitalWrite(LED_BUILTIN, HIGH);
    DigiKeyboard.delay(25);
    
    // type login credentials pair
    // remark: sendNullKeyStroke() is used as workaround for compatibility problems with older hardware
    //         and can be disabled with the use_null_keystroke define if this isn't needed
    sendNullKeyStroke();
    TypeInSingleChars(User);
    DigiKeyboard.delay(50);
    sendNullKeyStroke();
    DigiKeyboard.sendKeyStroke(KEY_TAB);  // switches from the user input field to the password input field (should work for 99% of login masks)
    DigiKeyboard.delay(50);
    sendNullKeyStroke();
    TypeInSingleChars(Password);
    DigiKeyboard.delay(50);
    sendNullKeyStroke();
    DigiKeyboard.sendKeyStroke(KEY_ENTER); // confirms login input
    DigiKeyboard.delay(50);

    // clear potential write error
    if (DigiKeyboard.getWriteError() != 0)
      DigiKeyboard.clearWriteError();
    DigiKeyboard.delay(50);
    
    // turn the builtin LED off after login has been typed
    digitalWrite(LED_BUILTIN, LOW);
    DigiKeyboard.delay(50);
}

/// -----------------------------------------------------------------------------------------------------------------------
/// print a longer text as singles and keep alive Keyboard with special delay
/// remark: this should prevent freezing when typing longer texts
/// -----------------------------------------------------------------------------------------------------------------------
void TypeInSingleChars(char Text[])
{
  // split text array in single chars and type char
  for (byte i = 0; Text[i] != 0; i++)
  {
    DigiKeyboard.print(Text[i]);
    // delay should prevent freezing when typing longer texts
    DigiKeyboard.delay(10);
  }
}

/// -----------------------------------------------------------------------------------------------------------------------
/// sendNullKeyStroke is used as workaround for compatibility problems with older hardware
/// -----------------------------------------------------------------------------------------------------------------------
void sendNullKeyStroke()
{
#ifdef use_null_keystroke
    DigiKeyboard.sendKeyStroke(0);
    DigiKeyboard.delay(50);
#endif
}

/// -----------------------------------------------------------------------------------------------------------------------
/// blink builtin LED
/// -----------------------------------------------------------------------------------------------------------------------
void blink (long milli)
{
    digitalWrite(LED_BUILTIN, previousLedState);
    previousLedState = !previousLedState;
    DigiKeyboard.delay(milli);
}

#ifdef enable_reboot
/// -----------------------------------------------------------------------------------------------------------------------
/// reboot via watchdog reset
/// remark: function derived from
///         https://bigdanzblog.wordpress.com/2015/07/20/resetting-rebooting-attiny85-with-watchdog-timer-wdt/
/// -----------------------------------------------------------------------------------------------------------------------
void reboot()
{
  cli();
  WDTCR = 0xD8 | WDTO_1S;
  sei();
  wdt_reset();
  while (true) {}
}
#endif