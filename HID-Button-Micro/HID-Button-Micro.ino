//--------------------------------------------//
//           HID-Button-Micro V1.1            //
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
#define enable_reboot         // enabled reboot on maximum duration button press else credential pair 3 is typed
#define enable_german_layout  // define german keyboard layout, if disabled US keyboard layout will be used
//#define enable_pin_mode       // password will not be typed with a finalizing RETURN, used for e.g. for PIN inputs which auto-accept a fixed count of digits

// includes
#include "Credentials.h"      // header file with stored USER|PASSWORD credentials
#include <Keyboard.h>         // library for emulating a HID keyboard
#include "Bounce2.h"          // library for handling the button hardware bouncing

/// -----------------------------------------------------------------------------------------------------------------------
/// do not change code behind this line if not needed !!!
/// -----------------------------------------------------------------------------------------------------------------------

/// -----------------------------------------------------------------------------------------------------------------------
/// hardware & software definitions
/// -----------------------------------------------------------------------------------------------------------------------
// input pin for pushbutton (other pin with according wiring change)
const int buttonPin = A0;

// reset pin for hardware reset trigger (other pin with according wiring change)
#ifdef enable_reboot
  const int resetPin = A1;
#endif

// first run
bool firstRun = true;

// state of the builtin LED for blinking
int previousLedState = HIGH;

// time to define a long button press in ms
int longPressTimeSpan = 1000;

// time to define a maximum duration button press in ms (must be longer than longPressTimeSpan)
#ifdef enable_reboot
  int maxPressTimeSpan = 5000;  //used longer timespan for the reboot to prevent unwanted trigger
#else
  int maxPressTimeSpan = 3000;  //use shorter timespan for typing the credential pair 2
#endif

// create a Bounce2::Button object
Bounce2::Button button = Bounce2::Button();

/// -----------------------------------------------------------------------------------------------------------------------
/// start the hardware initialization
/// -----------------------------------------------------------------------------------------------------------------------
void setup() { 
#ifdef enable_reboot
  // configure the reset pin as output with default HIGH
  digitalWrite(resetPin, HIGH);
  pinMode(resetPin, OUTPUT);
#endif

  // configure the pushbutton pin as input with internal pullup resistor
  pinMode(buttonPin, INPUT_PULLUP);

  // configure the builtin LED pin as output since the Micro does not blink without
  digitalWrite(LED_BUILTIN, LOW);
  pinMode(LED_BUILTIN, OUTPUT);

  // Bounce2::Button configuration
  button.attach(buttonPin, INPUT_PULLUP);
  button.interval(5);
  button.setPressedState(LOW); 

  // initialize control over the keyboard:
  Keyboard.begin();
}

/// -----------------------------------------------------------------------------------------------------------------------
/// program loop
/// -----------------------------------------------------------------------------------------------------------------------
void loop() {
  // wait some time on first run, to give hardware time to initialize and blink the builtin LED as start confirmation
  if (firstRun)
  {
    firstRun = false;
    for (int i = 0; i < 20; i++)
    {
      blink(100);
    }    
  }

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
  // validate if button was pressed over the maximal time within a small timespan window to prevent unwanted repeats
  else if (button.isPressed() && button.currentDuration() > maxPressTimeSpan && button.currentDuration() < maxPressTimeSpan + 25)
  {
#ifdef enable_reboot
    // blink as signalization for reboot
    for (int i = 0; i < 10; i++)
    {
      blink(100);
    }
    digitalWrite(LED_BUILTIN, previousLedState);

    //execute reboot
    reboot();
#else
    // type login credentials pair 3
    Type(USER3, PASSWORD3);
#endif
  }

  // keep alive
  delay(25);

  ///program loop ends here and will repeat now in a new cycle
}

/// -----------------------------------------------------------------------------------------------------------------------
/// set the keyboard layout
/// -----------------------------------------------------------------------------------------------------------------------
void KeyBoardBegin()
{
#ifdef enable_german_layout
  Keyboard.begin(KeyboardLayout_de_DE);
#else
  Keyboard.begin(KeyboardLayout_en_US);
#endif  
}

/// -----------------------------------------------------------------------------------------------------------------------
/// type the desired user credentials as USB HID keyboard
/// -----------------------------------------------------------------------------------------------------------------------
void Type(char User[], char Password[])
{
    // turn the builtin LED on while login is typed
    digitalWrite(LED_BUILTIN, HIGH);
    
    // type login credentials pair or only the password if user is left empty in the credentials
    if (User[0] != 0)
    {
      TypeInSingleChars(User);
      sendKeyStroke(KEY_TAB);   // switches from the user input field to the password input field (should work for 99% of login masks)
    }
    TypeInSingleChars(Password);
    
    #ifndef enable_pin_mode
    sendKeyStroke(KEY_RETURN);  // confirms login input witha RETURN (will not be send if PIN mode is active)
    #endif

    // clear potential write error
    if (Keyboard.getWriteError() != 0)
      Keyboard.clearWriteError();
    
    // turn the builtin LED off after login has been typed
    digitalWrite(LED_BUILTIN, LOW);
}

/// -----------------------------------------------------------------------------------------------------------------------
/// print a longer text as single chars
/// remark: this should prevent freezing when typing longer texts
/// -----------------------------------------------------------------------------------------------------------------------
void TypeInSingleChars(char Text[])
{
  // split text array in single chars and type char
  KeyBoardBegin();
  for (byte i = 0; Text[i] != 0; i++)
  {
    Keyboard.print(Text[i]);
    // delay should prevent freezing when typing longer texts
    delay(10);
  }
  Keyboard.end();
}

/// -----------------------------------------------------------------------------------------------------------------------
/// sendKeyStroke is used as wrapper to add a keep alive delay
/// -----------------------------------------------------------------------------------------------------------------------
void sendKeyStroke(uint8_t key)
{
    // press single key and release
    KeyBoardBegin();
    Keyboard.press(key);
    Keyboard.releaseAll();
    delay(50);
    Keyboard.end();
}

/// -----------------------------------------------------------------------------------------------------------------------
/// blink builtin LED
/// -----------------------------------------------------------------------------------------------------------------------
void blink(long milli)
{
    digitalWrite(LED_BUILTIN, previousLedState);
    previousLedState = !previousLedState;
    delay(milli);
}

#ifdef enable_reboot
/// -----------------------------------------------------------------------------------------------------------------------
/// reboot via RESET pin low (needs wiring Arduino RESET pin to IO pin which is used as trigger)
/// -----------------------------------------------------------------------------------------------------------------------
void reboot()
{
  digitalWrite(resetPin, LOW);
}
#endif