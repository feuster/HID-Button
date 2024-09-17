# HID-Button-Micro

## Introduction
The HID-Button-Micro is based on the Arduino Micro board and acts as an automatic keyboard replacement which types credentials into a Windows (or similiar) login request with just a single button press.

## Hardware
- Arduino Micro or compatible board
- A regular pushbutton of your choice
- Circuit:<br> ![Circuit](./docs/Circuit.png)
- Wiring:<br> ![Wiring](./docs/Wiring.png)

## Manual / Remarks
- It is recommended to use a programmer to upload the sketch like the USBtinyISP or similiar
- Choose Board and Arduino IDE Settings as shown here (__optional:__ choose different programming or upload method):<br> ![Arduino IDE Settings](./docs/Arduino_IDE_Settings.png)
- Install the _Keyboard_ library via the Arduino IDE library manager:<br> ![Arduino IDE Settings](./docs/Arduino_Keyboard_Library.png)
- Before uploading the INO sketch to the board you must change in [Credentials.h](./Credentials.h) the login credentials USER|PASSWORD pairs to the desired real credentials
- If only a password input is needed leave the according USER value in [Credentials.h](./Credentials.h) empty
- __Optional:__ if a 3rd USER|PASSWORD pair credential is needed the reboot function can be disabled by commenting out `#define enable_reboot` in exchange to enable this. The wiring from pin _A1_ to pin _RESET_ is also not needed in this configuration.

## Usage
### Depending if the reboot is enabled or not there are 2 usage schemes:
#### Default configuration with reboot enabled:
- Press the button shorter than 1 second to type credentials pair 1 as login
- Press the button between 1 and 5 seconds to type credentials pair 2 as login
- Press the button longer than 5 seconds to reboot the HID-Button
#### Alternative configuration with reboot disabled:
- Press the button shorter than 1 second to type credentials pair 1 as login
- Press the button between 1 and 3 seconds to type credentials pair 2 as login
- Press the button longer than 3 seconds to type credentials pair 3 as login

## Known Issues
- none (as of date 09/2024)

## License / Copyright
The HID-Button-Micro is licensed under [GPL-2.0-only](./LICENSE).

© Alexander Feuster 2024
