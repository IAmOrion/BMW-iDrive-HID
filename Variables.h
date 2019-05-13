/*=========================================================================*/
/*                   Written by James Tanner - DO NOT SHARE                */
/*                  https://www.facebook.com/TheMeroving1an                */
/*                                  IAmOrion                               */
/*=========================================================================*/

/*==========================================================================*/
/* Variables used throughout - you shouldn't really need to change any of   */
/* these. If you do change something, make sure you know what you're doing! */
/*==========================================================================*/

const int CAN_SPEED_SELECT_PIN = 6; byte CAN_SPEED, stati[256]; unsigned long previousMillis = 0, CoolDownMillis = 0;
unsigned int rotaryposition = 0;
boolean iDriveLightOn = true, RotaryInitSuccess = false, PollInit = false, TouchpadInitDone = false, LightInitDone = false, RotaryInitPositionSet = false, controllerReady = false, touching = false, rotaryDisabled = false;
const int8_t mouse_low_range = -128, mouse_high_range = 127, mouse_center_range = 0; const float powerValue = 1.4;
const int mouse_low_range_v2 = 0, mouse_high_range_v2 = 510, mouse_center_range_v2 = mouse_high_range_v2/2, x_multiplier = 2, y_multiplier = 12; int PreviousX = 0, PreviousY = 0, ResultX = 0, ResultY = 0;
int TouchpadInitIgnoreCounter = 0, init_can_count = 0, max_can_init_attempts = 15;
