/**
 * Light Arcade Buttons
 * read 2 buttons as they toggle, record delay of toggle
 * derive speed from that delay.
 */

// ===  MODULE GLOBALS ==============================

#define  MAX_DELAY   2047

// bit mask to store button state
unsigned char  buttonCurrMask; // bits 0-3 are player0:b0,b1  player1:b0,b1
unsigned char  buttonPrevMask; // previous button mask
unsigned char  buttonMaskDebounce; // buttonCurrMask from 10msec ago.

unsigned char  toggleMask = 0x05; // toggle bit mask for both pairs of buttons !MUST INIT 0x0101
long toggleTime[2]; // player 0,1 time of last toggle
unsigned int toggleDelay[2]; // player 0,1 msec of toggle, to calc button rate


// OLD STUFF...  SOMEDAY: fix toggle for more responsive/better delay, etc.
//long buttonMillisDebounce; // millis time from Debounce period 10msec ago.
//long decayMillis; // millis time from decay of toggle delay.
//unsigned char  buttonUtilCurrMask; // bits 0-1 are Arduino 2,3 (newgame/pause)
//unsigned char  buttonUtilPrevMask; // previous button utility mask




// ========== FUNCTIONS ==============================

// module service handler for buttons
void updateButtons(uint16_t x) {

  setDelay(MAX_DELAY);

  readButtonsDebounce(); // load buttonCurrMask using prev/curr button positions

  processButtons(x); // calc toggleDelay for each player

}



// read 6 buttons, debounce, load into bitmask
// 76543210 = mask
//     1100 = player 0/1
//     1010 = button 0/1
//   32     = util aka Pi gpio 2,3
int readButtonsDebounce() {

  buttonCurrMask = 0;
  for (int b = 0; b < 6; b++) {
    buttonCurrMask |= ( (digitalRead(pinButton[b]) ? 0 : 1) << b ); // invert, LOW=1
  }
}

// overwrite toggleDelay, for reset to long delay, or simulate in DEMO mode.
void setDelay(uint16_t dly) {
  toggleDelay[0] = dly;
  toggleDelay[1] = dly;
}


// TOGGLE: use toggleMask to track toggling of buttons btn0,1; btn2,3
// toggleMask always has bit 0 or 1 on; and bit 2 or 3 on.
// if toggled, also set toggleDelay for that player.
//
// states for player0 (toggle with player0 ANDMASK = 0011 = 0x03)
// if btn0 on and toggleMask 0001, then toggleMask = 0010
// if btn1 on and toggleMask 0010, then toggleMask = 0001
// states for player1 (toggle with player1 ANDMASK = 1100 = 0x0c)
// if btn2 on and toggleMask 0100, then toggleMask = 1000
// if btn3 on and toggleMask 1000, then toggleMask = 0100
// XOR with toggleBit of 0x03 (btn0,1) or 0x06 (btn2,3)
int tryToggleButton(int btn, uint16_t x) {
    
  if ( (toggleMask & (0x01 << btn)) ) {
    // toggle the mask and wait for the other button
    // NOTE: btn & 0x02 is 0 for btn0,1 and is 1 for btn 2,3
    uint16_t player = (btn & 0x02) >> 1;
    uint16_t toggleBit = ( player ? 0x0c : 0x03 ); // player 1 ANDMASK=0x0c (0b1100), player 0 ANDMASK=0x03 (0b0011)
    toggleMask ^= toggleBit; // toggle both bits in mask with XOR
    toggleDelay[player] = (now - toggleTime[player]);
    // scale down delay, limit to MAX_DELAY
    toggleDelay[player] = ( toggleDelay[player] > MAX_DELAY) ? MAX_DELAY : toggleDelay[player];
    toggleTime[player] = now;
    //sendToggleReport(x);  // DEBUG: send toggle report for testing...
    return true;
  }
  else {
    return false;
  }
}


// if button IS or WAS on, then handle it
bool processButtons(uint16_t x) {
  bool action = false;

  if (buttonPrevMask | buttonCurrMask) {
    for (int b = 0; b < 6; b++) {
      uint16_t changed = (buttonCurrMask ^ buttonPrevMask) & (0x01 << b);
      uint16_t state = buttonCurrMask & (0x01 << b);
      //int strip = (b & 0x02) >> 1; // buttons 0, 1 are strip0; buttons 2, 3 are strip1
      if (b < 4)
        digitalWrite( pinLed[b], state ? HIGH : LOW ); // light up button if pressed
      if (changed && state) {
        action = true;
        if ( (b < 4) && tryToggleButton(b, x) ) {
          sendToggleReport(x); // only report on toggle
        }
        else if (b == 4) {
          // newgame button
          //sendMaskReport(1); // for testing
          // FIXME: do like:   if (gameState == gsDEMO) ...
          newGame();
        }
        else if (b == 5) {
          // pause button
          //sendMaskReport(1); // DEBUG for testing
          pauseGame();
        }
        //sendToggleReport(x); // DEBUG for testing, send report
      } // if changed
    } // for
    buttonPrevMask = buttonCurrMask;
  } // if mask
  return action;
}



// setup pins, call once in setup()
void initButtons() {
  // set output/input; turn off button LEDs.
  for (int p = 0; p < 4; p++) {
    pinMode(pinLed[p], OUTPUT);
    digitalWrite(pinLed[p], LOW);
    pinMode(pinButton[p], INPUT); // not INPUT_PULLUP since we have hardware/resistor pullups in box.
  }

  pinMode(PIN_SELECT_POT, INPUT); // ADC to read POT position for game selection

  // utility pins are for Reset/Pause commands.
  for (int p = 0; p < 2; p++)
    pinMode(pinUtil[p], INPUT_PULLUP);

  setDelay(MAX_DELAY);
}




/**
 * === NOT USED ===========================
 *
 * This was originally used for testing/verify the LED strips
 */

#if 0

// NOTE: Blocking call, use only in setup()
// use for "begin game play" signal??
void flashLedsBlocking() {
  // flash all leds a few times - for lamp test.
  for (int c = 0; c < 3; c++) {
    for (int p = 0; p < 4; p++) {
      digitalWrite(pinLed[p], HIGH);
    }
    delay(500);
    for (int p = 0; p < 4; p++) {
      digitalWrite(pinLed[p], LOW);
    }
    delay(200);
  }

}

// non-blocking flash, uses 16-bit counter "X", assume 5msec loop period.
void flashLedsX(uint16_t x) {
  for (int p = 0; p < 4; p++) {
    // all flash:  if ( (x & 0x0018) ) {
    if ( (x & 0x0018) ) {
      digitalWrite(pinLed[p], HIGH);
    }
    else {
      digitalWrite(pinLed[p], LOW);
    }
  }
}

#endif


