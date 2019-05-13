/*=========================================================================*/
/*                   Written by James Tanner - DO NOT SHARE                */
/*                  https://www.facebook.com/TheMeroving1an                */
/*                                  IAmOrion                               */
/*=========================================================================*/

/*=========================================================================*/
/*                    AT+BLEKEYBOARDCODE KEY RELEASED                      */
/*=========================================================================*/

#define KEY_RELEASED_AT "AT+BLEKEYBOARDCODE=00-00" // "AT+BLEKEYBOARDCODE=00-00-00-00-00-00-00"


/*=========================================================================*/
/*              Assignment for the individual iDrive buttons               */
/*           http://www.freebsddiary.org/APC/usb_hid_usages.php            */
/*=========================================================================*/

#ifdef ANDROID
  #define KEY_MENU_AT "AT+BLEKEYBOARDCODE=00-00-10-00-00-00-00" // 'm'
  #define KEY_BACK_AT "AT+BLEKEYBOARDCODE=00-00-05-00-00-00-00" // 'b'
  #define KEY_OPTION_AT "AT+BLEKEYBOARDCODE=00-00-12-00-00-00-00" // 'o'
  #define KEY_RADIO_AT "AT+BLEKEYBOARDCODE=00-00-15-00-00-00-00" // 'r'
  #define KEY_CD_AT "AT+BLEKEYBOARDCODE=00-00-06-00-00-00-00" // 'c'
  #define KEY_NAV_AT "AT+BLEKEYBOARDCODE=00-00-11-00-00-00-00" // 'n'
  #define KEY_TEL_AT "AT+BLEKEYBOARDCODE=00-00-17-00-00-00-00" // 't'
#else
  #define KEY_MENU_AT "AT+BLEKEYBOARDCODE=00-00-10-00-00-00-00" // 'm'
  #define KEY_BACK_AT "AT+BLEKEYBOARDCODE=00-00-05-00-00-00-00" // 'b'
  #define KEY_OPTION_AT "AT+BLEKEYBOARDCODE=00-00-12-00-00-00-00" // 'o'
  #define KEY_RADIO_AT "AT+BLEKEYBOARDCODE=00-00-15-00-00-00-00" // 'r'
  #define KEY_CD_AT "AT+BLEKEYBOARDCODE=00-00-06-00-00-00-00" // 'c'
  #define KEY_NAV_AT "AT+BLEKEYBOARDCODE=00-00-11-00-00-00-00" // 'n'
  #define KEY_TEL_AT "AT+BLEKEYBOARDCODE=00-00-17-00-00-00-00" // 't'
#endif

#define KEY_MENU_KB 'm'
#define KEY_BACK_KB 'b'
#define KEY_OPTION_KB 'o'
#define KEY_RADIO_KB 'r'
#define KEY_CD_KB 'c'
#define KEY_NAV_KB 'n'
#define KEY_TEL_KB 't'

/*=========================================================================*/
/*                Assignment for movements of the Joystick                 */
/*=========================================================================*/

#ifdef ANDROID
  #define KEY_CENTER_AT "AT+BLEKEYBOARDCODE=00-00-28-00-00-00-00" // Return (Enter)
  #define KEY_UP_AT "AT+BLEKEYBOARDCODE=00-00-52-00-00-00-00" // Up Arrow
  #define KEY_DOWN_AT "AT+BLEKEYBOARDCODE=00-00-51-00-00-00-00" // Down Arrow
  #define KEY_LEFT_AT "AT+BLEKEYBOARDCODE=00-00-50-00-00-00-00" // Left Arrow
  #define KEY_RIGHT_AT "AT+BLEKEYBOARDCODE=00-00-4F-00-00-00-00" // Right Arrow
#else
  #define KEY_CENTER_AT "AT+BLEKEYBOARDCODE=00-00-16-00-00-00-00" // 's' // when center of joystick is pressed/released
  #define KEY_UP_AT "AT+BLEKEYBOARDCODE=00-00-18-00-00-00-00" // 'u' 
  #define KEY_DOWN_AT "AT+BLEKEYBOARDCODE=00-00-07-00-00-00-00" // 'd' 
  #define KEY_LEFT_AT "AT+BLEKEYBOARDCODE=00-00-0F-00-00-00-00" // 'l' 
  #define KEY_RIGHT_AT "AT+BLEKEYBOARDCODE=00-00-15-00-00-00-00" // 'r'
#endif

#define KEY_CENTER_KB 's' // when center of joystick is pressed/released
#define KEY_UP_KB 'u' 
#define KEY_DOWN_KB 'd' 
#define KEY_LEFT_KB '<' 
#define KEY_RIGHT_KB '>'

/*=========================================================================*/
/*                Assignment for rotation of the Joystick                  */
/*=========================================================================*/

#ifdef ANDROID
  #define KEY_ROTATE_PLUS_AT "AT+BLEHIDCONTROLKEY=VOLUME+" // Android - Volume Up
  #define KEY_ROTATE_MINUS_AT "AT+BLEHIDCONTROLKEY=VOLUME-" // Android - Volume Down
#else
  #define KEY_ROTATE_PLUS_AT "AT+BLEKEYBOARDCODE=00-00-80-00-00-00-00" // Volume Up
  #define KEY_ROTATE_MINUS_AT "AT+BLEKEYBOARDCODE=00-00-81-00-00-00-00" // Volume Down
#endif

#define KEY_ROTATE_PLUS_KB '+' // vol up
#define KEY_ROTATE_MINUS_KB '-' // vol down
