/**
 * Light Arcade Serial Interface Reports
 * Functions to initialize and service Serial text reports to Raspberry Pi.
 * Also support to input values to tweak physics model parameters (DEBUG only)
 *
 * Send text reports on Serial to Raspberry Pi.
 * These can be used on the Pi to trigger various sight/sound media options.
 *
 * Example Signals to Pi (outdated?)
 *  TICK:GAMECLOCK=xx;SCORE0=43;SCORE1=22;state=DEMO
 *  TICK:GAMECLOCK=xx;SCORE0=43;SCORE1=22;state=PLAY
 *  TICK:GAMECLOCK=xx;SCORE0=43;SCORE1=22;state=WINNER;winner=0
 *
 * No signals are read from Pi. (except debug settings)
 *
 */

// ===  MODULE GLOBALS ==============================

// to read serial buffer and parse commands
#define BUFFIMAX 20
int buffi;
char buff[BUFFIMAX];




// ========== FUNCTIONS ==============================


/**
 *  Serial handler
 *  send TICK reports with seconds remaining
 *  send GAME report with gameState
 */
void updateSerial(uint16_t x) {

  updateInputSerial(); // process input debug commands

  //sendTickReport();  // send TICK from gameTick() instead.

}


/**
 *  Serial handler for input commands
 */
void updateInputSerial() {
  // read 20 byte command into buffer and handle it.
  if (Serial.available() > 0) {
    int val = -1;
    buffi = 0;
    while (buffi < BUFFIMAX && Serial.available() > 0) {
      // get incoming byte:
      buff[buffi++] = Serial.read();
    }
    buff[buffi++] = '\0';
    Serial.print("Gotbuff:");
    Serial.println(buff);

    if (buff[1] == '=') {
      Serial.print("Set value ");
      if (buff[3] == '\0') {
        Serial.print("1-digit ");
        val = buff[2] - '0';
      }
      else if (buff[4] == '\0') {
        Serial.print("2-digit ");
        val  = 10 * (buff[2] - '0');
        val +=    (buff[3] - '0');
      }
      Serial.print("val=");
      Serial.print(val, DEC);
    }

    // enter * (or non g,f,m,d) to see all values
    switch (buff[0]) {
      /***
            case 'g':
              Serial.print(" Gravity=");
              Serial.println(GRAVITY);
              if (val >= 0) GRAVITY = val;
              //if (buff[0] == 'g') break;
              break;
            case 'f':
              Serial.print(" Force=");
              Serial.println(FORCE);
              if (val >= 0) FORCE = val;
              break;
            case 'm':
              Serial.print(" Mass=");
              Serial.println(MASS);
              if (val >= 0) MASS = val;
              break;
            case 'r':
              Serial.print(" dRag=");
              Serial.println(DRAGCONST);
              if (val >= 0) DRAGCONST = val;
              break;
      ***/
      case 'd':
        Serial.print(" Divisor bits=");
        Serial.println(DIVISOR_BITS);
        if (val >= 0) DIVISOR_BITS = val;
        break;
      case 'b':
        Serial.print(" demo Bit=");
        Serial.println(DEMOBIT);
        if (val >= 0) DEMOBIT = val;
        break;
      case 'o':
        Serial.print(" demo mOd=");
        Serial.println(DEMOMOD);
        if (val >= 0) DEMOMOD = val;
        break;

      default:
        Serial.print(" BAD command");
    }

    Serial.println(" *");

    for (buffi = 0; buffi < BUFFIMAX; buffi++)
      buff[buffi] = '\0';
  }

}



// output clock tick every 1 second
// li
// called by gameTick()
void ZZZZsendPiReport() {
  Serial.print("PI:gameClock=");
  Serial.print(gameClockSecs);
  Serial.print(";");
  sendGameStateReport(0);
  sendMaskReport(0);
  Serial.println("");
}




// output clock tick every 1 second
// called by gameTick()
void sendTickReport() {
  Serial.print("TICK:");
  Serial.print("CLOCK=");
  Serial.print(clockTick);
  Serial.print(";gameClock=");
  Serial.print(gameClockSecs);
  Serial.print(";");
  sendScoreReport(0);
  sendGameStateReport(0);
  //  sendMaskReport(0);
  if ( gsWINNER == gameState ) {
     sendWinnerReport(0);
  }
  Serial.println("");
}



// send report
// called by processButtons()
void sendToggleReport(uint16_t x) {

  Serial.print("TOGGLE:time=");
  Serial.print(now);
  Serial.print(";");

  sendMaskReport(0);

  sendToggleMaskReport(0);
  sendKinematicsReport(0);

  Serial.println("");

}

void serialPrintBinary(int bin, int digits) {
    for (int b=0; b<digits; b++) {
        if (bin & (1<<(digits-b-1))) {
           Serial.print("1");
        } else {
           Serial.print("0");
        }
    }
}

void sendMaskReport(int eol) {
  Serial.print("buttonCurrMask=");
  //Serial.print(buttonCurrMask,BIN);
  serialPrintBinary(buttonCurrMask,4);
  Serial.print(";");
  //sendScoreReport(0);
  if (eol) Serial.println("");
}


void sendScoreReport(int eol) {
  Serial.print("SCORE0=");
  Serial.print(playerScore[0]);
  Serial.print(";SCORE1=");
  Serial.print(playerScore[1]);
  Serial.print(";");
  if (eol) Serial.println("");
}


void sendDemoReport(int eol) {
  Serial.print("demoDelay[0]=");
  Serial.print(demoDelay[0]);
  Serial.print(";demoDelay[1]=");
  Serial.print(demoDelay[1]);
  Serial.print(";");
  if (eol) Serial.println("");
}


void sendKinematicsReport(int eol) {
  Serial.print("a[0]=");
  Serial.print(kin[0].a);
  Serial.print(";v[0]=");
  Serial.print(kin[0].v);
  Serial.print(";d[0]=");
  Serial.print(kin[0].d);
  Serial.print(";a[1]=");
  Serial.print(kin[1].a);
  Serial.print(";v[1]=");
  Serial.print(kin[1].v);
  Serial.print(";d[1]=");
  Serial.print(kin[1].d);
  Serial.print(";");
  if (eol) Serial.println("");
}

void sendGameStateReport(int eol) {
  Serial.print("state=");
  switch (gameState) {
    case gsDEMO:
      Serial.print("DEMO");
      break;
    case gsSTART:
      Serial.print("START");
      break;
    case gsPLAY:
      Serial.print("PLAY");
      break;
    case gsWINNER:
      Serial.print("WINNER");
      break;
  }
  Serial.print(";");
  if (eol) Serial.println("");
}


void sendToggleMaskReport(int eol) {
  Serial.print("toggleMask=");
  //Serial.print(toggleMask,BIN);
  serialPrintBinary(toggleMask,4);
  Serial.print(";toggleDelay[0]=");
  Serial.print(toggleDelay[0]);
  Serial.print(";toggleDelay[1]=");
  Serial.print(toggleDelay[1]);
  Serial.print(";");
  if (eol) Serial.println("");
}

/*****
// starting count-down sequence 'ready set go'
void ZZZsendStartReport(int eol) {
  Serial.print("START");
  if (eol) Serial.println("");
}
*****/

//void sendReport(uint16_t x, int b, int state) {
void sendWinnerReport(int eol) {
  Serial.print("winner=");
  Serial.print(winner);
  Serial.print(";");
  if (eol) Serial.println("");
}


/*********
void establishContact() {
  while (Serial.available() <= 0) {
    Serial.println("0,0,0");   // send an initial string
    delay(300);
  }
}
*****/





