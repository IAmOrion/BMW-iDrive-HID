/*=========================================================================*/
/*                   Written by James Tanner - DO NOT SHARE                */
/*                  https://www.facebook.com/TheMeroving1an                */
/*                                  IAmOrion                               */
/*=========================================================================*/

/*=========================================================================*/
/*        Board Settings - Eg, Feather Bluefruit or Arduino Micro          */
/*=========================================================================*/

#define USE_BLEKEYBOARDCODE
//#define ANDROID
const int SPI_CS_PIN = A4; // A4;  // Use A4 for Featherwing Shield, 12 for the Featherwing Uno MakerHawk Shield, or 9 for Makerhawk Shield (Or the CS of whatever can shield you're using)

/*=========================================================================*/
/*                              iDrive Settings                            */
/*=========================================================================*/

const long iDrivePollTime = 500, iDriveInitLightTime = 1000, iDriveLightTime = 10000, controllerCoolDown = 750;
const int TouchpadInitIgnoreCount = 2, min_mouse_travel = 10;

//#define iDriveJoystickAsMouse
#define JOYSTICK_MOVE_STEP = 50;

#define MOUSE_V1
//#define MOUSE_V2

#define LIGHT_OFF_BUTTON KEY_OPTION_KB

int ignored_responses[] = { 0x264, 0x267, 0x277, 0x567, 0x5E7, 0xBF };

/*=========================================================================*/
/*                              Debugging Options                          */
/*=========================================================================*/

#define SERIAL_DEBUG
#define DEBUG_CanResponse
//#define DEBUG_CanID
//#define DEBUG_SpecificCanID
//#define DEBUG_ID MSG_IN_TOUCH

#define DEBUG_Keys
//#define DEBUG_TouchPad

/*=========================================================================*/
/*                              Bluefruit Settings                         */
/*=========================================================================*/
/*=========================================================================
    
  FACTORYRESET_ENABLE       Perform a factory reset when running this sketch
   
                            Enabling this will put your Bluefruit LE module
                            in a 'known good' state and clear any config
                            data set in previous sketches or projects, so
                            running this at least once is a good idea.
   
                            When deploying your project, however, you will
                            want to disable factory reset by setting this
                            value to 0.  If you are making changes to your
                            Bluefruit LE device via AT commands, and those
                            changes aren't persisting across resets, this
                            is the reason why.  Factory reset will erase
                            the non-volatile memory where config data is
                            stored, setting it back to factory default
                            values.
       
                            Some sketches that require you to bond to a
                            central device (HID mouse, keyboard, etc.)
                            won't work at all with this feature enabled
                            since the factory reset will clear all of the
                            bonding data stored on the chip, meaning the
                            central device won't be able to reconnect.
                            
  MINIMUM_FIRMWARE_VERSION  Minimum firmware version to have some new features
/*=========================================================================*/

#define FACTORYRESET_ENABLE            0
#define MINIMUM_FIRMWARE_VERSION       "0.8.0"

#define BUFSIZE                        128
#define VERBOSE_MODE                   false  // If set to 'true' enables debug output

#define BLUEFRUIT_SPI_CS               8
#define BLUEFRUIT_SPI_IRQ              7
#define BLUEFRUIT_SPI_RST              4
