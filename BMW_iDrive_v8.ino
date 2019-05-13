/*=========================================================================*/
/*                   v8 uses the Seeed Studio CAN-BUS Libray               */
/*                 Works with either the 32u4 BLE or the M0 BLE            */
/*                                                                         */
/*                   Written by James Tanner - DO NOT SHARE                */
/*                  https://www.facebook.com/TheMeroving1an                */
/*                                  IAmOrion                               */
/*=========================================================================*/

/*=========================================================================*/
/*                        CAN_BUS Headers & Settings                       */
/*=========================================================================*/

#include <mcp_can.h>
#include <SPI.h>

/*=========================================================================*/
/*                       Include Settings & Variables                      */
/*=========================================================================*/

#include "Settings.h"
#include "Variables.h"
#include "KeyAssignments.h"

/*=========================================================================*/
/*                               iDrive CAN Codes                          */
/*=========================================================================*/

#include "iDrive.h"

/*=========================================================================*/
/*                         Adafruit Bluefruit Headers                      */
/*=========================================================================*/

#include <Arduino.h>
#include "Adafruit_BLE.h"
#include "Adafruit_BluefruitLE_SPI.h"
#include "Adafruit_BluefruitLE_UART.h"
Adafruit_BluefruitLE_SPI ble(BLUEFRUIT_SPI_CS, BLUEFRUIT_SPI_IRQ, BLUEFRUIT_SPI_RST);
void error(const __FlashStringHelper*err) { Serial.println(err); while (1); }

/*=========================================================================*/
/*                               Everything Else                           */
/*=========================================================================*/

MCP_CAN CAN(SPI_CS_PIN);

/*=========================================================================*/
/*                                 void setup()                            */
/*=========================================================================*/

void setup() {
  
  #ifdef SERIAL_DEBUG
    Serial.begin(9600);
  #endif
  
  pinMode(CAN_SPEED_SELECT_PIN, INPUT_PULLUP);

  #ifdef SERIAL_DEBUG
    delay(1000);
    Serial.println(F("BMW iDrive Bluetooth Controller"));
    Serial.println(F("---------------------------------------"));
    Serial.println(F("Initialising the Bluefruit LE module: "));
  #endif
  if ( !ble.begin(VERBOSE_MODE) ) { error(F("Couldn't find Bluefruit, make sure it's in CoMmanD mode & check wiring?")); }
  if ( FACTORYRESET_ENABLE ) { 
    #ifdef SERIAL_DEBUG
      Serial.println(F("Performing a factory reset: ")); 
    #endif
    if ( ! ble.factoryReset() ){ error(F("Couldn't factory reset")); } else { Serial.println(F("Reset Succesfully")); while(1); }
  }
  ble.echo(false);
  
  #ifdef SERIAL_DEBUG
    Serial.println(F("Requesting Bluefruit info:"));
    //ble.info();
  #endif

  /* Change the device name to make it easier to find */
  #ifdef SERIAL_DEBUG
    Serial.println(F("Setting device name to 'BMW iDrive Bluetooth Controller': "));
  #endif
  if (! ble.sendCommandCheckOK(F( "AT+GAPDEVNAME=BMW iDrive Bluetooth Controller" )) ) {
    error(F("Could not set device name?"));
  }

  /* Enable HID Service */
  #ifdef SERIAL_DEBUG
    Serial.println(F("Enable HID Service (including Keyboard): "));
  #endif
  if ( ble.isVersionAtLeast(MINIMUM_FIRMWARE_VERSION) ) {
    if ( !ble.sendCommandCheckOK(F( "AT+BleHIDEn=On" ))) {
      error(F("Could not enable Keyboard"));
    }
  } else {
    Serial.print(F("Sorry, but your device MUST be on firmware version ")); 
    Serial.print(MINIMUM_FIRMWARE_VERSION); Serial.println(F(" (BLESPIFRIEND)"));
    while(1);
  }

  /* Add or remove service requires a reset */
  #ifdef SERIAL_DEBUG
    Serial.println(F("Performing a SW reset (service changes require a reset): "));
  #endif
  if (! ble.reset() ) {
    error(F("Couldn't reset??"));
  }

  int CAN_SPEED_SELECT_PIN_IN = digitalRead(CAN_SPEED_SELECT_PIN);

  if (CAN_SPEED_SELECT_PIN_IN == HIGH) { CAN_SPEED = CAN_500KBPS; } else { CAN_SPEED = CAN_100KBPS; }
  #ifdef SERIAL_DEBUG
    Serial.print(F("CAN Speed Set To: ")); if (CAN_SPEED == CAN_500KBPS) { Serial.println(F("500KBPS")); } else { Serial.println(F("100KBPS")); }
  #endif

  
  while (CAN_OK != CAN.begin(CAN_SPEED)) {
    init_can_count++;
    Serial.println(F("CAN BUS Shield init fail"));
    Serial.println(F(" Init CAN BUS Shield again"));
    delay(100);
    if (init_can_count >= max_can_init_attempts) {
      Serial.println(F("CAN INIT FAILED Multiple times - HALTING"));
      while(1);
    }
  }
  Serial.println(F("CAN-BUS Featherwing ok!"));
}

/*=========================================================================*/
/*                                  void loop()                            */
/*=========================================================================*/

void loop() {

  // Initial Inits
  if (!(RotaryInitSuccess)) { iDriveInit(); }  
  if (!(TouchpadInitDone) && (RotaryInitSuccess)) { Serial.println(F("Touchpad Init"));  iDriveTouchpadInit(); TouchpadInitDone = true; }
  if (!(PollInit) && (TouchpadInitDone) && (RotaryInitSuccess)) { Serial.println(F("Poll Init")); do_iDrivePoll(); PollInit = true; }
  if (!(LightInitDone)) { if (previousMillis == 0) { previousMillis = millis(); }; iDriveLightInit(); }
  if (!(controllerReady) && (RotaryInitSuccess) && (TouchpadInitDone) && (PollInit)) {  if (CoolDownMillis == 0) { CoolDownMillis = millis(); }; if (millis() - CoolDownMillis > controllerCoolDown) { controllerReady = true; } }
  
  iDrivePoll(iDrivePollTime);
  iDriveLight(iDriveLightTime);

  if(CAN_MSGAVAIL == CAN.checkReceive()) {
    unsigned char len = 0; unsigned char buf[8];
    CAN.readMsgBuf(&len, buf);
    unsigned long canId = CAN.getCanId();
    decodeCanBus(canId, len, buf);
  }
}

void iDriveInit() {
  //ID: 273, Data: 8 1D E1 0 F0 FF 7F DE 4
  const int msglength = 8; unsigned char buf[8] =  { 0x1D, 0xE1, 0x0, 0xF0, 0xFF, 0x7F, 0xDE, 0x4 };  
  CAN.sendMsgBuf(MSG_OUT_ROTARY_INIT, 0, msglength, buf);
  RotaryInitPositionSet = false;
}

void iDriveTouchpadInit() {
  //ID: BF, Length: 8, Data: 21 0 0 0 11 0 0 0
  const int msglength = 8; unsigned char buf[8] =  { 0x21, 0x0, 0x0, 0x0, 0x11, 0x0, 0x0, 0x0 };
  CAN.sendMsgBuf(MSG_IN_TOUCH, 0, msglength, buf);
}

void iDriveLightInit() {
  do_iDriveLight(); if (millis() - previousMillis > iDriveInitLightTime) { LightInitDone = true; }
}

void iDrivePoll(unsigned long milliseconds) {
  static unsigned long lastiDrivePing = 0;
  if (millis() - lastiDrivePing >= milliseconds)
  {
    #if defined(SERIAL_DEBUG) && defined(DEBUG_iDriveWakeup)
      Serial.println(F("iDrive Ping"));
    #endif
    lastiDrivePing += milliseconds;
    do_iDrivePoll();
  }
}

void do_iDrivePoll() {
  //ID: 501, Data Length: 8, Data: 1 0 0 0 0 0 0 0
  const int msglength = 8; unsigned char buf[8] =  { 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
  CAN.sendMsgBuf(MSG_OUT_POLL, 0, msglength, buf);
}

void iDriveLight(unsigned long milliseconds) {
  static unsigned long lastiDriveLight = 0;
  if (millis() - lastiDriveLight >= milliseconds)
  {
    lastiDriveLight += milliseconds;
    do_iDriveLight();
  }
}

void do_iDriveLight() {
  //ID: 202, Data: 2 FD 0 == Light ON, D: 202, Data: 2 FE 0 == Light OFF
  unsigned char buf[2] = { 0x00, 0x0 };
  const int msglength = 2; if (iDriveLightOn) { buf[0] = 0xFD; } else { buf[0] = 0xFE; }
  CAN.sendMsgBuf(MSG_OUT_LIGHT, 0, msglength, buf);
}

void decodeCanBus(unsigned long canId, unsigned char len, unsigned char buf[8]) {
  #if defined(SERIAL_DEBUG) && defined(DEBUG_CanResponse)
    #ifdef DEBUG_SpecificCanID
      if (canId == DEBUG_ID) {
    #endif
        if (!(isvalueinarray(canId, ignored_responses, (sizeof(ignored_responses) / 2)))) {
          Serial.print(F("ID: ")); Serial.print(canId,HEX); Serial.print(F(", Data Length: ")); Serial.print(len,DEC); Serial.print(F(", Data: "));
          for(int i=0;i<len;i++) { Serial.print(buf[i],HEX); Serial.print(" "); } Serial.println();
        }
    #ifdef DEBUG_SpecificCanID
      }
    #endif
  #endif

  if (canId == MSG_IN_ROTARY_INIT) {
    Serial.println(F("Rotary Init Success"));
    RotaryInitSuccess = true; 
  }

  switch (canId) {
    case MSG_IN_INPUT:
      {
        byte state = buf[3] & 0x0F, input_type = buf[4], input = buf[5];
        switch (input_type) {
          case MSG_INPUT_BUTTON:
          {
            switch (input) {
              case MSG_INPUT_BUTTON_MENU:
                SendKey(KEY_MENU_AT, KEY_MENU_KB, state);
                break;
              case MSG_INPUT_BUTTON_BACK:
                SendKey(KEY_BACK_AT, KEY_BACK_KB, state);
                break;
              case MSG_INPUT_BUTTON_OPTION:
                SendKey(KEY_OPTION_AT, KEY_OPTION_KB, state);
                break;
              case MSG_INPUT_BUTTON_RADIO:
                SendKey(KEY_RADIO_AT, KEY_RADIO_KB, state);
                break;
              case MSG_INPUT_BUTTON_CD:
                SendKey(KEY_CD_AT, KEY_CD_KB, state);
                break;
              case MSG_INPUT_BUTTON_NAV:
                SendKey(KEY_NAV_AT, KEY_NAV_KB, state);
                break;
              case MSG_INPUT_BUTTON_TEL:
                SendKey(KEY_TEL_AT, KEY_TEL_KB, state);
                break;
              default:
                break;
            }
          }
          break;
          
          case MSG_INPUT_CENTER:
          {
            #ifdef iDriveJoystickAsMouse
              iDriveMoveMouse(KEY_CENTER_KB, state);
            #else
              SendKey(KEY_CENTER_AT, KEY_CENTER_KB, state);
            #endif
          }
          break;

          case MSG_INPUT_STICK:
          {
            byte joystick_input = buf[3] / 0x10;
            switch (joystick_input) {
              case MSG_INPUT_STICK_UP:
                #ifdef iDriveJoystickAsMouse
                  iDriveMoveMouse(KEY_UP_KB, state);
                #else
                  SendKey(KEY_UP_AT, KEY_UP_KB, state);
                #endif
                break;
              case MSG_INPUT_STICK_DOWN:
                #ifdef iDriveJoystickAsMouse
                  iDriveMoveMouse(KEY_DOWN_KB, state);
                #else
                  SendKey(KEY_DOWN_AT, KEY_DOWN_KB, state);
                #endif
                break;
              case MSG_INPUT_STICK_LEFT:
                #ifdef iDriveJoystickAsMouse
                  iDriveMoveMouse(KEY_LEFT_KB, state);
                #else
                  SendKey(KEY_LEFT_AT, KEY_LEFT_KB, state);
                #endif
                break;
              case MSG_INPUT_STICK_RIGHT:
                #ifdef iDriveJoystickAsMouse
                  iDriveMoveMouse(KEY_RIGHT_KB, state);
                #else
                  SendKey(KEY_RIGHT_AT, KEY_RIGHT_KB, state);
                #endif
                break;
              case MSG_INPUT_STICK_CENTER:
               #ifdef iDriveJoystickAsMouse
                  iDriveMoveMouse(KEY_UP_KB, MSG_INPUT_RELEASED);
                  iDriveMoveMouse(KEY_DOWN_KB, MSG_INPUT_RELEASED);
                  iDriveMoveMouse(KEY_LEFT_KB, MSG_INPUT_RELEASED);
                  iDriveMoveMouse(KEY_RIGHT_KB, MSG_INPUT_RELEASED);
                #else
                  SendKey(KEY_UP_AT, KEY_UP_KB, MSG_INPUT_RELEASED);
                  SendKey(KEY_DOWN_AT, KEY_DOWN_KB, MSG_INPUT_RELEASED);
                  SendKey(KEY_LEFT_AT, KEY_LEFT_KB, MSG_INPUT_RELEASED);
                  SendKey(KEY_RIGHT_AT, KEY_RIGHT_KB, MSG_INPUT_RELEASED);
                #endif
                break;
              default:
                break;
            }
          }
          break;
          default:
            break;
        }
      }
      break;

    case MSG_IN_ROTARY:
      {
        byte rotarystepa = buf[3], rotarystepb = buf[4];
        unsigned int newpos = (((unsigned int)rotarystepa) + ((unsigned int)rotarystepb) * 0x100);

        if (!(RotaryInitPositionSet)) { 
          switch (rotarystepb) {
            case 0x7F:
              rotaryposition = (newpos+1);
              break;
            case 0x80:
              rotaryposition = (newpos-1);
              break;
            default:
              rotaryposition = newpos;
              break;
          }
          RotaryInitPositionSet= true;
        }

        while (rotaryposition < newpos) {
          if (!(rotaryDisabled)) {
            SendKey(KEY_ROTATE_PLUS_AT, KEY_ROTATE_PLUS_KB, MSG_INPUT_PRESSED);
            SendKey(KEY_ROTATE_PLUS_AT, KEY_ROTATE_PLUS_KB, MSG_INPUT_RELEASED);
          }
          rotaryposition++ ;
        }
        
        while (rotaryposition > newpos) {
          if (!(rotaryDisabled)) {
            SendKey(KEY_ROTATE_MINUS_AT, KEY_ROTATE_MINUS_KB, MSG_INPUT_PRESSED);
            SendKey(KEY_ROTATE_MINUS_AT, KEY_ROTATE_MINUS_KB, MSG_INPUT_RELEASED);
          }
          rotaryposition-- ;
        }
      }
      break;

    case MSG_IN_TOUCH:
      {
        byte fingers = buf[4];
        
        int touch_count = 0;
        switch (fingers) {
          case FINGER_REMOVED:
            touch_count = 0;
            break;
          case SINGLE_TOUCH:
            touch_count = 1;
            break;
          case MULTI_TOUCH:
            touch_count = 2;
            break;
          default:
            touch_count = 1;
            break;
        }
        TouchPadMouse(buf[1], buf[3], buf[5], buf[7], buf[0], buf[2], buf[6], touch_count);
      }
      break;
      
    case MSG_IN_STATUS:
      {
        if (buf[4] == MSG_STATUS_NO_INIT) {
          RotaryInitSuccess = false; LightInitDone = false; previousMillis = 0; CoolDownMillis = 0; TouchpadInitIgnoreCounter = 0;
        }
      }
      break;
    default:
      break;
  }
  
}

void SendKey(const String& ATCommand, char Key, byte state) {
  byte oldstate = stati[Key];

  if(controllerReady) {
    if (state == MSG_INPUT_RELEASED and oldstate != MSG_INPUT_RELEASED) { 
      #if defined(SERIAL_DEBUG) && defined(DEBUG_Keys)
        if ((Key !=  KEY_ROTATE_PLUS_KB) && (Key !=  KEY_ROTATE_MINUS_KB)) {
          debugKeys(Key, "RELEASED");
        }
      #endif
      rotaryDisabled = false;
    }
  
    if (state == MSG_INPUT_PRESSED and oldstate == MSG_INPUT_RELEASED) {
      #if defined(SERIAL_DEBUG) && defined(DEBUG_Keys)
        debugKeys(Key, "PRESSED");
      #endif

      rotaryDisabled = true;
  
      #ifdef USE_BLEKEYBOARDCODE
        ble.println(ATCommand); ble.println(KEY_RELEASED_AT); //ble.println(KEY_RELEASED_AT);
      #else
        ble.print("AT+BleKeyboard="); ble.println(Key);
      #endif
    }
  
    if (state == MSG_INPUT_HELD and oldstate == MSG_INPUT_PRESSED)
    {
      #if defined(SERIAL_DEBUG) && defined(DEBUG_Keys)
        debugKeys(Key, "HELD");
      #endif

      rotaryDisabled = true;
      
      if (Key == LIGHT_OFF_BUTTON) { iDriveLightOn = !(iDriveLightOn); do_iDriveLight(); }
    }
  }
  
  stati[Key] = state;
}

void TouchPadMouse(byte x, byte y, byte x2, byte y2, byte counter, byte xLR, byte x2LR, int touchcount) {
  /*
  *  X is 0 - 255 | 0 - 255 (Left to Center, Center to Right) | X Home (0) = LEFT & Center
  *  Y is 0 - 30 (Bottom to Top) | Y Home (0) = BOTTOM
  *  
  *  The 0xBF Response is built of the following:
  *  Byte[0] = Counter
  *  Byte[1] = X Pos (Single Touch)
  *  Byte[2] = X Left or Right -> The X Axis is split in 2, with left to center being 0-255, then center to right being 0-255. Byte 2 is 0 for left range, 1 for right range
  *  Byte[3] = Y Pos (Single Touch)
  *  Byte[4] = Touch Count -> Eg, how many fingers are touchng the touchpad.  Can detect 1,2,3 or even 4 - but for practicality we only want single or multi (2) max
  *  Byte[5] = X2 Pos (Multi Touch)
  *  Byte[6] = X2 Left or Right (As Above)
  *  Byte[7] = Y2 Pos (Multi Touch)
  */

  xLR = xLR & 0x0F; x2LR = x2LR & 0x0F;
  int pos_x = ((int)x), pos_y = ((int)y), x_LR = (int)xLR;
  
  #if defined(SERIAL_DEBUG) && defined(DEBUG_TouchPad)
    Serial.print(F("X: ")); Serial.print(pos_x); Serial.print(F(", X-L/R: ")); Serial.print(x_LR); Serial.print(F(", Y: ")); Serial.print(pos_y);
    if (touchcount > 1) { Serial.print(F(", X2: ")); Serial.print(x2,DEC); Serial.print(F(", X2-L/R: ")); Serial.print(x2LR,HEX); Serial.print(F(", Y2: ")); Serial.print(y2,DEC); }
    Serial.println();
  #endif

  if (touchcount > 0) {
    if (TouchpadInitIgnoreCounter > TouchpadInitIgnoreCount) {
      #ifdef MOUSE_V1      
        if (x_LR == 0) { pos_x = map(pos_x, 0, 255, mouse_low_range_v2, mouse_center_range_v2); } else if (x_LR == 1) { pos_x = map(pos_x, 0, 255, mouse_center_range_v2, mouse_high_range_v2); } else { Serial.print(F("Doh - x_LR = ")); Serial.println(x_LR); }
        pos_y = map(pos_y, 0, 30, mouse_low_range_v2, 90);
    
        if (!(touching)) { touching = true; PreviousX = pos_x; PreviousY = pos_y; ResultX = 0; ResultY = 0; }
    
        //Serial.print(F("2) X: ")); Serial.print(x); Serial.print(F(", pos_x: ")); Serial.print(pos_x); Serial.print(F(", Y: ")); Serial.print(y); Serial.print(F(", pos_y: ")); Serial.println(pos_y);
    
        if ((pos_x != 0) || (pos_y != 0)) {
          int RawX = pos_x - PreviousX, RawY = pos_y - PreviousY;
          pos_x += ResultX; pos_y += ResultY;
          int XFinal = RawX, YFinal = RawY;
  
          if (controllerReady) {          
            //Serial.print(F("AT+BLEHIDMOUSEMOVE=")); Serial.print((int8_t)XFinal); Serial.print(F(", ")); Serial.println((((int8_t)(YFinal*6))*-1));
            ble.print(F("AT+BLEHIDMOUSEMOVE=")); ble.print((int8_t)XFinal*x_multiplier); ble.print(F(", ")); ble.println((((int8_t)(YFinal*y_multiplier))*-1)); 
          }

          ResultX = RawX - XFinal; ResultY = RawY - YFinal;
          PreviousX = pos_x; PreviousY = pos_y;
        }
      #endif
      #ifdef MOUSE_V2
        int8_t pos_x_mapped = 0, pos_y_mapped = 0, mousemove_x = 0, mousemove_y = 0;
        if (x_LR == 0) { pos_x_mapped = map(pos_x, 0, 255, mouse_low_range, mouse_center_range); } else if (x_LR == 1) { pos_x_mapped = map(pos_x, 0, 255, mouse_center_range, mouse_high_range); } else { Serial.print(F("Doh - x_LR = ")); Serial.println(x_LR); }
        pos_y_mapped = map(pos_y, 0, 30, mouse_low_range, mouse_high_range);
    
        if ((pos_x_mapped == 0) && ((pos_y_mapped == 0))) {
          mousemove_x = 0; mousemove_y = 0;
        } else {
          mousemove_x = pos_x_mapped; mousemove_y = pos_x_mapped;
          if (mousemove_x > 0) { mousemove_x = ((pos_x_mapped/4)+8); } else if (mousemove_x < 0) { mousemove_x = ((pos_x_mapped/4)-8); }
          if (mousemove_y > 0) { mousemove_y = ((pos_y_mapped/4)+8); } else if (mousemove_x < 0) { mousemove_y = ((pos_y_mapped/4)-8); }
        }
    
        if ((controllerReady) && (touchcount >= 1)) {
          Serial.print(F("AT+BLEHIDMOUSEMOVE=")); Serial.print(mousemove_x); Serial.print(F(", ")); Serial.println((mousemove_y*-1));
          ble.print(F("AT+BLEHIDMOUSEMOVE=")); ble.print(mousemove_x); ble.print(F(", ")); ble.println((mousemove_y*-1));
        }
      #endif
    } else {
      if (RotaryInitSuccess) { TouchpadInitIgnoreCounter++; }
    }
  } else {
    touching = false;
  }
}

bool isvalueinarray(int val, int *arr, int size) {
  int i;
  for (i = 0; i < size; i++) {
    if (arr[i] == val)
      return true;
  }
  return false;
}

#if defined(SERIAL_DEBUG) && defined(DEBUG_Keys)
  void debugKeys(char Key, const String& ButtonState)
  {
    switch(Key)
    {
      case KEY_MENU_KB:
        Serial.print(F("Button MENU ")); Serial.println(ButtonState);
        break;
      case KEY_BACK_KB:
        Serial.print(F("Button BACK ")); Serial.println(ButtonState);
        break;
      case KEY_OPTION_KB:
        Serial.print(F("Button OPTION ")); Serial.println(ButtonState);
        break;
      case KEY_RADIO_KB:
        Serial.print(F("Button RADIO / AUDIO ")); Serial.println(ButtonState);
        break;
      case KEY_CD_KB:
        Serial.print(F("Button MEDIA / CD ")); Serial.println(ButtonState);
        break;
      case KEY_NAV_KB:
        Serial.print(F("Button NAV ")); Serial.println(ButtonState);
        break;
      case KEY_TEL_KB:
        Serial.print(F("Button TEL ")); Serial.println(ButtonState);
        break;
      case KEY_CENTER_KB:
        Serial.print(F("Joystick Center ")); Serial.println(ButtonState);
        break;
      case KEY_UP_KB:
        Serial.print(F("Joystick UP ")); Serial.println(ButtonState);
        break;
      case KEY_DOWN_KB:
        Serial.print(F("Joystick DOWN ")); Serial.println(ButtonState);
        break;
      case KEY_LEFT_KB:
        Serial.print(F("Joystick LEFT ")); Serial.println(ButtonState);
        break;
      case KEY_RIGHT_KB:
        Serial.print(F("Joystick RIGHT ")); Serial.println(ButtonState);
        break;
      case KEY_ROTATE_PLUS_KB:
        Serial.println(F("Rotary RIGHT"));
        break;
      case KEY_ROTATE_MINUS_KB:
        Serial.println(F("Rotary LEFT"));
        break;
      case MSG_IN_TOUCH_V2:
        if (ButtonState == "0") {
          Serial.println(F("Fingers removed from TouchPad"));
        } else {
          Serial.print(F("Touchpad - ")); Serial.print(ButtonState); Serial.println(F(" Fingers"));
        }
        break;
      default:
        break;
    }
  }
#endif
