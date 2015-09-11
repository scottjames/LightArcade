/**
 * Light Arcade By Scott James & Greg Schroeder
 * 2015 released to public domain.
 * If you use/modify this please let us know!
 * 
 * This arcade style game is driven entirely on the Arduino.
 * read 2 player 'boxes' who toggle buttons to simulate acceleration in a car.
 * derive velocity as score, and display on 2 WS2812 (Neo Pixel) LED strips.
 * also provide Serial data reports for Raspberry Pi to show multimedia display.
 *
 * Things for next version...
 * TODO: use 250msec reports instead of 1 second reports for more interactive Raspi display (refactor reports to Rpi)
 * TODO: smarter button delay for variable 'power' based on toggle speed
 * TODO: sense faster button speed (use 1msec loop period?)
 * TODO: use 1msec loop period, need to fix MOD X levels inside service handlers.
 *
 * Long term things to do....
 * TODO: refactor code for extensible classes/api (make easier for others to hack this stuff!)
 * TODO: add selector knob, and multiple game models/modes.
 * TODO: implement 1D physics model with variable parameters.
 * TODO: use I2C to send detailed physics data to Raspi for display/animation/etc. Display with Processing/Pygame, etc.
 * TODO: other games...
 */

#include <Adafruit_NeoPixel.h>


// ===  PIN CONFIGURATION ====================

#define PIN_P1B1   6   // btn 1
#define PIN_P1L1   7   // led 1
#define PIN_P1B2   8   // btn 2
#define PIN_P1L2   9   // led 2

#define PIN_P2B1  13   // btn 1
#define PIN_P2L1  12   // led 1
#define PIN_P2B2  11   // btn 2
#define PIN_P2L2  10   // led 2

#define PIN_NEWGAME 2    // util pin 2
#define PIN_PAUSE   3    // util pin 3

#define PIN_SELECT_POT  A2  // voltage divider with Potentiometer to select game (someday)

#define PIN_STRIP0  A0   // strip0 / player 1
#define PIN_STRIP1  A1   // strip1 / player 2


// ===  MODULE GLOBALS ==============================

// store pin configuration in arrays for iteration later on
int  pinLed[4]    = {
     PIN_P1L1, PIN_P1L2,  // Player 0 (Left/Blue)
     PIN_P2L1, PIN_P2L2   // Player 1 (Right/Red)
};
int  pinButton[6] = {
     PIN_P1B1, PIN_P1B2, PIN_P2B1, PIN_P2B2, // P0, P1
     PIN_NEWGAME, PIN_PAUSE
};

int  pinUtil[2] = { PIN_NEWGAME, PIN_PAUSE }; // utility inputs

// ~48" strip is 39 LEDs
#define NUM_STRIP_ELEMENTS 39

Adafruit_NeoPixel strip[2] = {
  Adafruit_NeoPixel(NUM_STRIP_ELEMENTS, PIN_STRIP0, NEO_GRB + NEO_KHZ800),
  Adafruit_NeoPixel(NUM_STRIP_ELEMENTS, PIN_STRIP1, NEO_GRB + NEO_KHZ800)
};

long now; // current millis, global clock

uint16_t x; // global loop counter for sequencing of lights, cycles 0 - 0xFFFF

// IMPORTANT: To reduce NeoPixel burnout risk, add 1000 uF capacitor across
// pixel power leads, add 300 - 500 Ohm resistor on first pixel's data input
// and minimize distance between Arduino and first pixel.  Avoid connecting
// on a live circuit...if you must, connect GND first.



// ========== FUNCTIONS ==============================

void setup() {
  
  delay(2000); // delay at startup to power up

  // open fast serial port
  Serial.begin(115200);
  while (!Serial)
    ; // wait for serial port to connect. Needed for Leonardo only

  initButtons();

  // Some example procedures showing how to display to the pixels:
  Serial.println("# BEGIN SETUP");

  // setup strips and button LEDs; do lamp test.
  initGraphics();
  
  // setup new game
  initGameModel();

  x = 0;
  Serial.println("# END SETUP");
}

/**
 * Arduino loops with short period.
 * Only a couple calls are blocking (and they fix the clock)
 * Service Inputs, Game model, output to LED and Serial
 */
void loop() {

  now = millis();

  // service input buttons, debouncing and toggle delay values.
  updateButtons(x);

  // update game, calc button rate from toggle delay, calc player scores
  updateGameModel(x);

  // update display on led strips with current player scores
  updateGraphics(x);

  // send report to Raspberry Pi with: clockTick, button status, game state, etc.
  updateSerial(x);

  x++;  // this global 16 bit unsigned integer will overflow (recycle) at 0xFFFF

  delay(5); // 5 msec sleep/tick interval
}

