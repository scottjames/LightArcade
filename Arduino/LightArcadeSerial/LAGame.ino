
/**
 * Light Arcade Game Model.
 *
 * Use state machine for showing game play: DEMO --> START --> PLAY --> WINNER --> DEMO
 *
 * DEMO - flash graphics until 'new game' utility pin toggled
 * START - display short sequence "Ready, Set, Go!"
 * PLAY - actual game play, service buttons, score, LED strips, 1 second reports on serial
 * WINNER - display short sequence "Winner is XXX", then goto DEMO
*/

// ===  MODULE GLOBALS ==============================

#define gsDEMO    0 // idle sequence
#define gsSTART   1 // "ready go" sequence (flash green)
#define gsPLAY    2 // game play for a while
#define gsPAUSE   3 // pause game, no update, no clock tick
#define gsWINNER  4 // flash winner side at current score, black out loser side.
int gameState;

int winner;  // -1=start/playing/demo/tie  0=player0, 1=player1

#define PLAY_CLOCK_SECS  10
//#define PLAY_CLOCK_SECS  5   // DEBUG short time for testing

int gameClockSecs; // countdown clock for current state 10, 9, 8, ...

long clockTick; // millis tick value during clocktick


#define MAX_SCORE  100
uint16_t  playerScore[2]; // score: 0-100 for player1, player2

uint16_t  demoDelay[2]; // fake button delay for demo

/** NOTE: can alter these values with Serial input: g=## d=## f=## m=## ...
* acceleration force calc with:  ( FORCE * Delay ) >> DIVISOR_BITS
*/
//int DIVISOR_BITS   =  7;  // 1<<7  =  128  serial option 'd'
//int DIVISOR_BITS   =  6;  // 1<<6  =   64  serial option 'd'
int DIVISOR_BITS   =  10;   // 1<<10 = 1024  serial option 'd'

int DEMOBIT = 2;  // serial option 'b'
int DEMOMOD = 80; // (use 90 * 10)  serial option 'o'

// typical fast delay: 93, 140, 48, 164...

/**
 * TODO: put physics back in...
 *
int FORCE          = 10;  // force would be:  (10 * delay) / 128
int MASS           = 38;
int GRAVITY        =  3;

int FORCE          = 16;  // force would be:  (10 * delay) / 128
int MASS           = 256;
int GRAVITY        =  0;
//int FRICTION       = 64;  // divisor: f=v/4 (or f=a/4 ??)  FIXME
int DRAGCONST      = 128;  // option 'r'
 *
 **/


struct kinematics {
  int v; // velocity
  int d; // drag force (V * V / DRAGCONST)
  int a; // acceleration
  int vlast; // velocity of previous loop
  int alast; // acceleration of previous loop
} kin[2];


// ========== FUNCTIONS ==============================



/**
 * Set fake delay values for DEMO mode.
 * The main thing is to extract values from X counter to show LED movement.
 *
 * TODO (make better) try to mix 2 odd harmonic periods, maybe now>>5 + now>>3 ?
 *
 * take lower bits from X and shift up to proper delay range.
 * 1<<10 = 1024
 * 1<<11 = 2048
 * 2048 = 0x0800 ; 2047 = 0x07FF
 * dont need whole range, can use just partial with increment of 256 or whatever.
 * use upper 3 bits (7 of 7FF)
 */
void gameSetDemoValues() {

  kin[0].v  = (   (x & (0xff << 2)) >> 2 );
  kin[1].v  = (   (x & (0xff << 3)) >> 3 );

  /***
  if ( kin[0].v > 200 ) {
     kin[0].v  = 0;
  }
  if ( kin[1].v > 200 ) {
     kin[1].v  = 0;
  }
  ***/

  limitValues(kin[0]);
  limitValues(kin[1]);
  //updateKinematics(kin[0], demoDelay[0]);
  //updateKinematics(kin[1], demoDelay[1]);
}


/** limit velocity from 0 - 100 (MAX_SCORE)
 */
void limitValues(struct kinematics& k) {
  if (k.v < 0) k.v = 0;
  if (k.v > MAX_SCORE) k.v = MAX_SCORE;
}

/**
 * Physics model
 * The idea is to implement inertial/gravity/acceleration using button toggle rate.
 * This was greatly simplified to make it work.
 * TODO: put physics back in someday...
 */

/***
// convert delay (0 - MAX_DELAY) to rate
int delayToRate(unsigned int dly) {
  int newrate = (MAX_DELAY - dly) / DIVISOR;
  return (newrate + GRAVITY);
}
***/

// maintain physics, calc new velocity to display (0 - MAX_SCORE)
void updateKinematics(struct kinematics& k, uint16_t dly) {
  // OR... f=ma -> a=f/m
  //  int accel = ((MAX_DELAY - dly) / DIVISOR); // orig
  //  int accel = ( (FORCE * (MAX_DELAY - dly)) >> DIVISOR_BITS ) / MASS; // a=f/m

  int accel = (MAX_DELAY - dly) >> DIVISOR_BITS; // a=f/m

  //  int accel = ( (FORCE * (MAX_DELAY - dly)) / DIVISOR ) / MASS; // a=f/m
  //  k.a =  accel - GRAVITY;
  k.a =  accel;

  // Ffric = mu * Fnorm; or divide out MASS, then: Afric = mu * Anorm
  // calc Ffric from 'm*a' or, Ffric = MASS * k.a
  // but add this back to Accel (a=f/m) so Afric = MASS * K.a / MASS my multiplying MASS
  //int Ffric = MASS * k.a / FRICTION;  // or Afric = Ffric/MASS
  //int Afric = k.a / FRICTION;  // or Afric = Ffric/MASS

  // Drag = Cd * d * V**2/2 * A
  // Cd=coef d=density A=area == "constants"
  // Drag = DRAGCONST * V * V  or...   V * V / DRAGCONST
  //  k.d = k.vlast * k.vlast / DRAGCONST;
  //  k.a -= k.d;

  // TODO... drag/friction
  //  k.a =  accel - GRAVITY - (k.a/FRICTION); // add drag, Afric = A * mu

  // add drag/friction to pull back...
  // Vf = Vi + a*t  (t=1 for discrete steps)
  k.v = k.vlast + k.a; // add drag

  //k.v = k.vlast + (k.a - GRAVITY); // add drag
  //k.v = k.vlast + k.a - k.vlast/FRICTION; // add drag
  // k.v -= k.d;  // need k.a to be more than just 1 !!

  // Simulate drag/friction by knocking down V every 1-2 secs.
  //if ( 0 == (x & 0x000f) ) {  // match 16*5msec = .08 secs ...degrades way too fast, unplayable
  //if ( 0 == (x & 0x00ff) ) {  // match 0x0ff * 5msecs = 1.275 secs ...pretty good/slow
  //if ( 0 == (x & 0x007f) ) {  // match 0x07f * 5msecs = 0.635 secs ...faster, easy play
  //if ( 0 == (x & 0x001f) ) {  // match 0x01f * 5msecs = 0.155 secs ...extremely challenging! (too much)
  if ( 0 == (x & 0x003f) ) {    // match 0x03f * 5msecs = 0.315 secs ...pretty challenging...nice!
    k.d = 1;
    k.v--;     // little bit of drag.
    //    sendKinematicsReport(1); // DEBUG for testing drag rate
    k.d = 0;
  }

  // if ( 0 == x%100 ) sendKinematicsReport(1); // DEBUG samples for testing

  // limit velocity from 0 - 100 (MAX_SCORE)
  limitValues(k);
  //if (k.v < 0) k.v = 0;
  //if (k.v > MAX_SCORE) k.v = MAX_SCORE;

  // remember for next go around
  k.vlast = k.v;
  k.alast = k.a;
}

// reset kinematics model.
void initKinematics(struct kinematics& k) {
  k.a = k.v = k.vlast = k.alast = 0;
}


// TICK clock.  increment tick, decrement game clock, send TICK report.
void gameTick(uint16_t x) {

  // wait for 1 second.
  if ( (now - clockTick) < 1000)
    return;
  clockTick += 1000;

  sendTickReport(); // send TICK report

  // other stuff to do each second...
  // TODO/FIXME: if (gameClockSecs >= 0)  (we should send "0" tick to pi)
  if (gameClockSecs > 0)
    gameClockSecs--;

}

// reset TICK game clock
// ONLY call after blocking functions to fix clockTick.
void gameResetTick() {
  now = millis();
  clockTick = now;
}




/**
 * Module service handler for Game Model
 */
void updateGameModel(uint16_t x) {

  // try to advance game clock
  gameTick(x);

  // handle current state
  switch (gameState) {

    case gsDEMO:
      // DEMO mode: 'seed' fake demoDelay to show some movement
      // use something like this but more 'random'
      // TODO (make better) try to mix 2 odd harmonic periods, maybe now>>5 + now>>3
      // NOTE:  DEMOMOD and DEMOBIT can be tweaked from Serial input.
      if (0 == x % 2) {
        gameSetDemoValues();
      }

      //      if (0 == x % 0x80)
      //        sendDemoReport(1); // only for testing...

      break;

    case gsPLAY:

      if (gameClockSecs == 0) {
        gameGotoWinner();
      }

      //      if (0 == x % 8) {
      //      if (0 == x % 2) {
      updateKinematics(kin[0], toggleDelay[0]);
      updateKinematics(kin[1], toggleDelay[1]);
      //      }

      // if (0 == x % 100) sendKinematicsReport(1);  // for testing

      break;
    case gsPAUSE:
      // no clock tick, button handler will setback to gsPLAY
      break;
    case gsSTART:
      // TODO flash both green "ready, set, go!"
      gameStartBlocking();
      break;
    case gsWINNER:
      // TODO: flash current score of winner, black out loser
      gameWinnerBlocking();

      // goto DEMO
      //if (gameClockSecs == 0) {
      initGameModel();
      //}

      break;
  }

}


// do "Ready, Set, Go!"
void gameStartBlocking() {

  initGameModel();
  newGame();

  //sendStartReport(1);
  sendTickReport();

  // flash both strips green 3 times... NOTE: this blocks around 4 seconds.
  doStartGraphicsBlocking();

  //initGameModel();
  initKinematics(kin[0]);
  initKinematics(kin[1]);

  gameResetTick();  // fix clockTick after blocking call.

  // reset clock, goto PLAY
  gameState = gsPLAY;
  gameClockSecs = PLAY_CLOCK_SECS;

  sendTickReport(); // send tick after blocking call.
}


// flash winning side green, start demo
void gameWinnerBlocking() {

  //initGameModel();
  initKinematics(kin[0]);
  initKinematics(kin[1]);

  //sendWinnerReport(1);
  sendTickReport();

  // flash winning strip ...   NOTE: This BLOCKS for around 5 secs
  doWinnerGraphicsBlocking();
  gameResetTick();  // reset time after blocking call.

  // Go back to DEMO
  gameState = gsDEMO;
  //gameClockSecs = PLAY_CLOCK_SECS;

  sendTickReport(); // added to send tick after blocking call.
}



/** calculate winner, goto WINNER state
 * called when game clock is 0
 */
void gameGotoWinner() {

  // TODO set winner
  if (playerScore[0] > playerScore[1])
    winner = 0;
  else if (playerScore[1] > playerScore[0])
    winner = 1;
  else
    winner = -1; // tie

  gameClockSecs = -1; // goto WINNER state and show winner; winner graphics are BLOCKING call!
  gameState = gsWINNER;
  //sendWinnerReport(1);  // winner info was moved into TICK report

}


// reset game
void initGameModel() {
  playerScore[0] = playerScore[1] = 0;
  setDelay(MAX_DELAY);
  winner = -1;
  gameState = gsDEMO;
  gameClockSecs = -1;

}

// start new game, should only call from PIN_NEWGAME handler
void newGame() {
  //gameState = gsPLAY;
  gameState = gsSTART;
  gameClockSecs = PLAY_CLOCK_SECS;
}

// pause/resume game, call from PIN_PAUSE handler
void pauseGame() {
  if (gameState == gsPAUSE)
    gameState = gsPLAY;
  else if (gameState == gsPLAY)
    gameState = gsPAUSE;
  //gameClockSecs = PLAY_CLOCK_SECS;
}

