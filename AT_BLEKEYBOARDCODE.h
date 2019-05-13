/*=========================================================================*/
/*                   Written by James Tanner - DO NOT SHARE                */
/*                  https://www.facebook.com/TheMeroving1an                */
/*                                  IAmOrion                               */
/*=========================================================================*/

/*
 * https://learn.adafruit.com/introducing-adafruit-ble-bluetooth-low-energy-friend/ble-services#at-plus-blekeyboardcode
 * https://learn.adafruit.com/introducing-the-adafruit-bluefruit-spi-breakout/ble-services#at-plus-blehidcontrolkey-14-39
 * 
 
AT+BLEKEYBOARDCODE
Sends a raw hex sequence of USB HID keycodes to the BLE keyboard interface including key modifiers and up to six alpha-numeric characters.

This command accepts the following string-encoded byte array payload, matching the way HID over GATT sends keyboard data:

Byte 0: Modifier
Byte 1: Reserved (should always be 00)
Bytes 2..7: Hexadecimal value(s) corresponding to the HID keys (if no character is used you can enter '00' or leave trailing characters empty)
After a keycode sequence is sent with the AT+BLEKEYBOARDCODE command, you must send a second AT+BLEKEYBOARDCODE command with at least two 00 characters to indicate the keys were released!

Modifier Values
The modifier byte can have one or more of the following bits set:

Bit 0 (0x01): Left Control
Bit 1 (0x02): Left Shift
Bit 2 (0x04): Left Alt
Bit 3 (0x08): Left Window
Bit 4 (0x10): Right Control
Bit 5 (0x20): Right Shift
Bit 6 (0x40): Right Alt
Bit 7 (0x80): Right Window

Parameters: A set of hexadecimal values separated by a hyphen ('-').  Note that these are HID scan code values, not standard ASCII values!

Output: None

HID Keyboard Codes - https://learn.adafruit.com/introducing-adafruit-ble-bluetooth-low-energy-friend/ble-services#hid-keyboard-codes-14-26

0x00  Reserved (no event indicated)
0x01  Keyboard ErrorRollOver
0x02  Keyboard POSTFail
0x03  Keyboard ErrorUndefined
0x04  Keyboard a and A
0x05  Keyboard b and B
0x06  Keyboard c and C
0x07  Keyboard d and D
0x08  Keyboard e and E
0x09  Keyboard f and F
0x0A  Keyboard g and G
0x0B  Keyboard h and H
0x0C  Keyboard i and I
0x0D  Keyboard j and J
0x0E  Keyboard k and K
0x0F  Keyboard l and L
0x10  Keyboard m and M
0x11  Keyboard n and N
0x12  Keyboard o and O
0x13  Keyboard p and P
0x14  Keyboard q and Q
0x15  Keyboard r and R
0x16  Keyboard s and S
0x17  Keyboard t and T
0x18  Keyboard u and U
0x19  Keyboard v and V
0x1A  Keyboard w and W
0x1B  Keyboard x and X
0x1C  Keyboard y and Y
0x1D  Keyboard z and Z
0x1E  Keyboard 1 and !
0x1F  Keyboard 2 and @
0x20  Keyboard 3 and #
0x21  Keyboard 4 and $
0x22  Keyboard 5 and %
0x23  Keyboard 6 and ^
0x24  Keyboard 7 and &
0x25  Keyboard 8 and *
0x26  Keyboard 9 and (
0x27  Keyboard 0 and )
0x28  Keyboard Return (ENTER)
0x29  Keyboard ESCAPE
0x2A  Keyboard DELETE (Backspace)
0x2B  Keyboard Tab
0x2C  Keyboard Spacebar
0x2D  Keyboard - and (underscore)
0x2E  Keyboard = and +
0x2F  Keyboard [ and {
0x30  Keyboard ] and }
0x31  Keyboard \ and |
0x32  Keyboard Non-US # and ~
0x33  Keyboard ; and :
0x34  Keyboard ' and "
0x35  Keyboard Grave Accent and Tilde
0x36  Keyboard, and <
0x37  Keyboard . and >
0x38  Keyboard / and ?
0x39  Keyboard Caps Lock
0x3A  Keyboard F1
0x3B  Keyboard F2
0x3C  Keyboard F3
0x3D  Keyboard F4
0x3E  Keyboard F5
0x3F  Keyboard F6
0x40  Keyboard F7
0x41  Keyboard F8
0x42  Keyboard F9
0x43  Keyboard F10
0x44  Keyboard F11
0x45  Keyboard F12
0x46  Keyboard PrintScreen
0x47  Keyboard Scroll Lock
0x48  Keyboard Pause
0x49  Keyboard Insert
0x4A  Keyboard Home
0x4B  Keyboard PageUp
0x4C  Keyboard Delete Forward
0x4D  Keyboard End
0x4E  Keyboard PageDown
0x4F  Keyboard RightArrow
0x50  Keyboard LeftArrow
0x51  Keyboard DownArrow
0x52  Keyboard UpArrow
0x53  Keypad Num Lock and Clear
0x54  Keypad /
0x55  Keypad *
0x56  Keypad -
0x57  Keypad +
0x58  Keypad ENTER
0x59  Keypad 1 and End
0x5A  Keypad 2 and Down Arrow
0x5B  Keypad 3 and PageDn
0x5C  Keypad 4 and Left Arrow
0x5D  Keypad 5
0x5E  Keypad 6 and Right Arrow
0x5F  Keypad 7 and Home
0x60  Keypad 8 and Up Arrow
0x61  Keypad 9 and PageUp
0x62  Keypad 0 and Insert
0x63  Keypad . and Delete
0x64  Keyboard Non-US \ and |
0x65  Keyboard Application
0x66  Keyboard Power
0x67  Keypad =
0x68  Keyboard F13
0x69  Keyboard F14
0x6A  Keyboard F15
0x6B  Keyboard F16
0x6C  Keyboard F17
0x6D  Keyboard F18
0x6E  Keyboard F19
0x6F  Keyboard F20
0x70  Keyboard F21
0x71  Keyboard F22
0x72  Keyboard F23
0x73  Keyboard F24
0x74  Keyboard Execute
0x75  Keyboard Help
0x76  Keyboard Menu
0x77  Keyboard Select
0x78  Keyboard Stop
0x79  Keyboard Again
0x7A  Keyboard Undo
0x7B  Keyboard Cut
0x7C  Keyboard Copy
0x7D  Keyboard Paste
0x7E  Keyboard Find
0x7F  Keyboard Mute
0x80  Keyboard Volume Up
0x81  Keyboard Volume Down
0x82  Keyboard Locking Caps Lock
0x83  Keyboard Locking Num Lock
0x84  Keyboard Locking Scroll Lock
0x85  Keypad Comma
0x86  Keypad Equal Sign
0x87  Keyboard International1
0x88  Keyboard International2
0x89  Keyboard International3
0x8A  Keyboard International4
0x8B  Keyboard International5
0x8C  Keyboard International6
0x8D  Keyboard International7
0x8E  Keyboard International8
0x8F  Keyboard International9
0x90  Keyboard LANG1
0x91  Keyboard LANG2
0x92  Keyboard LANG3
0x93  Keyboard LANG4
0x94  Keyboard LANG5
0x95  Keyboard LANG6
0x96  Keyboard LANG7
0x97  Keyboard LANG8
0x98  Keyboard LANG9
0x99  Keyboard Alternate Erase
0x9A  Keyboard SysReq/Attention
0x9B  Keyboard Cancel
0x9C  Keyboard Clear
0x9D  Keyboard Prior
0x9E  Keyboard Return
0x9F  Keyboard Separator
0xA0  Keyboard Out
0xA1  Keyboard Oper
0xA2  Keyboard Clear/Again
0xA3  Keyboard CrSel/Props
0xA4  Keyboard ExSel
0xE0  Keyboard LeftControl
0xE1  Keyboard LeftShift
0xE2  Keyboard LeftAlt
0xE3  Keyboard Left GUI
0xE4  Keyboard RightControl
0xE5  Keyboard RightShift
0xE6  Keyboard RightAlt
0xE7  Keyboard Right GUI

Example:

# send 'abc' with left shift key (0x02) --> 'ABC'
AT+BLEKEYBOARDCODE=02-00-04-05-06-00-00
OK
# Indicate that the keys were released (mandatory!)
AT+BLEKEYBOARDCODE=00-00
OK

--------------------------------------------------------------

AT+BLEHIDCONTROLKEY
Sends HID media control commands for the bonded device (adjust volume, screen brightness, song selection, etc.).

Codebase Revision: 0.6.6

Parameters: The HID control key to send, followed by an optional delay in ms to hold the button

The control key string can be one of the following values:

System Controls (works on most systems)
  BRIGHTNESS+
  BRIGHTNESS-

Media Controls (works on most systems)
  PLAYPAUSE
  MEDIANEXT
  MEDIAPREVIOUS
  MEDIASTOP
  
Sound Controls (works on most systems)
  VOLUME
  MUTE
  BASS
  TREBLE
  BASS_BOOST
  VOLUME+
  VOLUME-
  BASS+
  BASS-
  TREBLE+
  TREBLE-
  
Application Launchers (Windows 10 only so far)
  EMAILREADER
  CALCULATOR
  FILEBROWSER
  
Browser/File Explorer Controls (Firefox on Windows/Android only)
  SEARCH
  HOME
  BACK
  FORWARD
  STOP
  REFRESH
  BOOKMARKS
  
You can also send a raw 16-bit hexadecimal value in the '0xABCD' format. 
A full list of 16-bit 'HID Consumer Control Key Codes' can be found here:
http://www.freebsddiary.org/APC/usb_hid_usages.php (see section 12).

Output: Normally none.

Example:

# Toggle the sound on the bonded central device
AT+BLEHIDCONTROLKEY=MUTE
OK

# Hold the VOLUME+ key for 500ms
AT+BLEHIDCONTROLKEY=VOLUME+,500
OK

# Send a raw 16-bit Consumer Key Code (0x006F = Brightness+)
AT+BLEHIDCONTROLKEY=0x006F
OK

*/
