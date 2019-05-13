/*=========================================================================*/
/*                   Written by James Tanner - DO NOT SHARE                */
/*                  https://www.facebook.com/TheMeroving1an                */
/*                                  IAmOrion                               */
/*=========================================================================*/

/*=========================================================================*/
/*                             iDrive CAN-Codes                            */
/*=========================================================================*/

#define MSG_IN_INPUT 0x267
#define MSG_INPUT_BUTTON 0xC0
#define MSG_INPUT_BUTTON_MENU 0x01
#define MSG_INPUT_BUTTON_BACK 0x02
#define MSG_INPUT_BUTTON_OPTION 0x04
#define MSG_INPUT_BUTTON_RADIO 0x08
#define MSG_INPUT_BUTTON_CD 0x10
#define MSG_INPUT_BUTTON_NAV 0x20
#define MSG_INPUT_BUTTON_TEL 0x40
#define MSG_INPUT_CENTER 0xDE
#define MSG_INPUT_STICK 0xDD
#define MSG_INPUT_STICK_UP 0x1 // 0x11
#define MSG_INPUT_STICK_RIGHT 0x2 // 0x21
#define MSG_INPUT_STICK_DOWN 0x4 // 0x41
#define MSG_INPUT_STICK_LEFT 0x8 // 0x81
#define MSG_INPUT_STICK_CENTER 0x0

#define MSG_INPUT_RELEASED 0x00
#define MSG_INPUT_PRESSED 0x01
#define MSG_INPUT_HELD 0x02

#define MSG_IN_ROTARY 0x264
#define MSG_IN_ROTARY_INIT 0x277
#define MSG_STATUS_NO_INIT 0x6 // Rotary Init-State (6 false, 0 or 1 true)

#define MSG_IN_STATUS 0x5E7 // Example: 5E7 8 99 01 0A 05 01 00 00 00 | Data 0: Can ID, Data 1: Data Length, Data 5: Counter, Data 6: Rotary Init-State (6 false, 1 true)
#define MSG_IN_TIMEOUT 0x567
#define MSG_IN_ERROR 0x667

#define MSG_IN_TOUCH 0xBF // 0x0BF
#define FINGER_REMOVED 0x11 // 0 Fingers Touching (Eg, fingers removed)
#define SINGLE_TOUCH 0x10 // 1 Finger touching
#define MULTI_TOUCH 0x00 // 2 Finger touching
#define TRIPLE_TOUCH 0x1F // 3 Finger touching
#define QUAD_TOUCH 0x0F // 4 Finger touching

#define MSG_IN_TOUCH_V2 0xFFFFFFBF // Purely for key debuggng purposes - no used for anything else, and not needed by anything other than 1 simple serial.print switch(Key) case.

#define MSG_OUT_ROTARY_INIT 0x273
#define MSG_OUT_LIGHT 0x202
#define MSG_OUT_POLL 0x501 //0x563

// 0x4E7 = Status
// 0x5E7 = Status with Counter
