//--------------------------------------------//
//           HID-Button Credentials           //
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

/// -----------------------------------------------------------------------------------------------------------------------
/// define login credentials pairs for the HID-Button
/// set user blank if you need only a password input, example: USER1[] = "";
/// credential pair 3 is only used when reboot is not define in HID-Button-Micro.ino
/// -----------------------------------------------------------------------------------------------------------------------

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
/// login credentials pair 3 for maximum duration button press which can be changed to real credentials (reboot disabled)
/// -----------------------------------------------------------------------------------------------------------------------
const char USER3[] = "USER3";
const char PASSWORD3[] = "PASSWORD3";
