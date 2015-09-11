/**
 * Light Arcade Graphics
 * Functions to initialize and service LED strips.
 * We use NeoPixel library to drive WS2812 RGB strips.
 */


// ===  MODULE GLOBALS ==============================

// predefine some colors to use in the LED strips
static uint32_t colorRed    = Adafruit_NeoPixel::Color(255, 0, 0);
static uint32_t colorGreen  = Adafruit_NeoPixel::Color(0, 255, 0);
static uint32_t colorBlue   = Adafruit_NeoPixel::Color(0, 0, 255);
static uint32_t colorBlack  = Adafruit_NeoPixel::Color(0, 0, 0);


// ========== FUNCTIONS ==============================

/**
 * Draw strips with current scores.
 * Calculate pixel position for strip using score (0-100)
 * Velocity is already scaled from 0 - 100, so just pass to Score.
 */
void updateGraphics(uint16_t x) {

  if (playerScore[0] != kin[0].v) {
    playerScore[0] = kin[0].v;
    colorWipeScoreX(strip[0], colorBlue, playerScore[0], x); // Left/Blue
    //powerChaseRainbowX(strip[s], playerScore[s], x); // TODO someday animate colors...
  }

  if (playerScore[1] != kin[1].v) {
    playerScore[1] = kin[1].v;
    colorWipeScoreX(strip[1], colorRed, playerScore[1], x); // Right/Red
    //powerChaseRainbowX(strip[s], playerScore[s], x); // TODO someday animate colors...
  }


}

/**
 * Test sequence for R,G,B on strips, button LEDs
 * this was shortened to only flash Green on each strip to show they are connected.
 */
void initGraphics() {

  for (int s = 0; s < 2; s++) {
    strip[s].begin();
    strip[s].show(); // Initialize all pixels to 'off'

    //    Serial.print("# STRIP red ");
    //    Serial.println(s);
    //    colorWipeBlocking(strip[s], colorRed, 5); // Red

    Serial.print("# STRIP green ");
    Serial.println(s);
    colorWipeBlocking(strip[s], colorGreen, 5); // Green

    //   Serial.print("# STRIP blue ");
    //   Serial.println(s);
    //   colorWipeBlocking(strip[s], colorBlue, 5); // Blue
  }

  // make both strips green ("GO")
  //  colorWipeBlocking(strip[0], colorGreen, 10); // Green
  //  colorWipeBlocking(strip[1], colorGreen, 10); // Green

  colorWipeBlocking(strip[0], colorBlack, 5); // Black
  colorWipeBlocking(strip[1], colorBlack, 5); // Black

  //  Serial.println("# BUTTON LED FLASH");
  //  flashLedsBlocking();

}



/** do START sequence for "Ready, Set, Go!"
 */
void doStartGraphicsBlocking() {
  // wipe down together in 3 steps
  for (int score = MAX_SCORE; score >= 0; score -= MAX_SCORE / 4) {
    colorWipeScoreX(strip[0], colorGreen, score, x); // Green
    colorWipeScoreX(strip[1], colorGreen, score, x); // Green
    delay(800); // BLOCKING
  }
  colorWipeBlocking(strip[0], colorBlack, 5); // Black
  colorWipeBlocking(strip[1], colorBlack, 5); // Black

}


/** do WINNER sequence.
 * flash winner side a few times .. TODO make this cooler :)
 */
void doWinnerGraphicsBlocking() {

  colorWipeBlocking(strip[0], colorBlack, 5); // Black
  colorWipeBlocking(strip[1], colorBlack, 5); // Black

  /***
    for (int s = 0; s < 5; s++) {
      if (winner == 0)
        colorWipeScoreX(strip[0], Adafruit_NeoPixel::Color(0, 255, 0), MAX_SCORE, x); // Green
      else if (winner == 1)
        colorWipeScoreX(strip[1], Adafruit_NeoPixel::Color(0, 255, 0), MAX_SCORE, x); // Green
  ***/

  if (winner == 0)
    theaterChaseBlocking(strip[0], colorGreen, 50);
  else if (winner == 1)
    theaterChaseBlocking(strip[1], colorGreen, 50);
  else if (winner == -1) {
    //for (int i = 0; i < 0xffff; i++) {
    //colorWipeScoreX(strip[0], colorGreen, MAX_SCORE, i); // Green
    //colorWipeScoreX(strip[1], colorGreen, MAX_SCORE, i); // Green
    //}
    rainbowCycleAllStripsBlocking(3); // tie 10   FIXME: ....use 2strip chase instead
  }

  colorWipeBlocking(strip[0], colorBlack, 5); // Black
  colorWipeBlocking(strip[1], colorBlack, 5); // Black
  //  }


}



// ===  UTILS ============================

// Fill the dots one after the other with a color
void colorWipeBlocking(Adafruit_NeoPixel & strip, uint32_t c, uint8_t wait) {
  for (uint16_t i = 0; i < strip.numPixels(); i++) {
    strip.setPixelColor(i, c);
    strip.show();
    delay(wait);
  }
}


// Slightly different, this makes the rainbow equally distributed throughout
void rainbowCycleAllStripsBlocking(uint8_t wait) {
  uint16_t i, j;
  uint16_t num = strip[0].numPixels();

  for (j = 0; j < 256 * 5; j++) { // 5 cycles of all colors on wheel
    // HACK.. same# pixels in strip0 and strip1
    for (i = 0; i < num; i++) {
      strip[0].setPixelColor(i, Wheel(((i * 256 / num) + j) & 255));
      strip[1].setPixelColor(i, Wheel(((i * 256 / num) + j) & 255));
    }
    strip[0].show();
    strip[1].show();
    delay(wait);
  }
}

// Fill the dots one after the other with a color
// set score 0 - 100 before calling.
void colorWipeScoreX(Adafruit_NeoPixel & strip, uint32_t c, uint16_t score,  uint16_t x) {
  //  static long lastx = 0;

  int num = map( score, 0, MAX_SCORE, 0, strip.numPixels() );

  //uint32_t colorBlack = Adafruit_NeoPixel::Color(0, 0, 0);

  for (uint16_t i = 0; i < strip.numPixels(); i++) {
    if (i < num) {
      strip.setPixelColor(i, c);
    } else {
      strip.setPixelColor(i, colorBlack);
    }
    //strip.show();
    //delay(wait);
  }
  strip.show();

  //lastx = x;
}


//Theatre-style crawling lights with rainbow effect
// increment counter 'x' after each call.
void powerChaseRainbowX(Adafruit_NeoPixel & strip, uint8_t score, uint16_t x) {

  int crawl = 3; // 4 LED crawl

  int j = x % 256; // 256 colors in the wheel
  int q = x % crawl;

  //int del = x % 20; // delay loop (5 msec * 20)

  // FIXME... timing...

  int num = map(score, 0, 100, 0, strip.numPixels());

  if (x & 0x00f0) {
    for (int i = 0; i < num; i = i + crawl) {
      strip.setPixelColor(i + q, Wheel( (i + j) % 255)); //turn every third pixel on
    }
    strip.show();
  }
  else {
    for (int i = 0; i < num; i = i + crawl) {
      strip.setPixelColor(i + q, 0);      //turn every third pixel off
    }
  }

}


// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
uint32_t Wheel(byte WheelPos) {
  if (WheelPos < 85) {
    return Adafruit_NeoPixel::Color(WheelPos * 3, 255 - WheelPos * 3, 0);
  } else if (WheelPos < 170) {
    WheelPos -= 85;
    return Adafruit_NeoPixel::Color(255 - WheelPos * 3, 0, WheelPos * 3);
  } else {
    WheelPos -= 170;
    return Adafruit_NeoPixel::Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
}


//Theatre-style crawling lights.
void theaterChaseBlocking(Adafruit_NeoPixel & strip, uint32_t c, uint8_t wait) {
  for (int j = 0; j < 30; j++) { //do 30 cycles of chasing (was 10 before lab demo)
    for (int q = 0; q < 3; q++) {
      for (int i = 0; i < strip.numPixels(); i = i + 3) {
        strip.setPixelColor(i + q, c);  //turn every third pixel on
      }
      strip.show();

      delay(wait);

      for (int i = 0; i < strip.numPixels(); i = i + 3) {
        strip.setPixelColor(i + q, 0);      //turn every third pixel off
      }
    }
  }
}




// ===  OLD OLD OLD ============================


/**
 * Most of this came from NeoPixel strandtext example code.
 * some is works in progress to use X modulo counters for non-blocking asynchronous animation within loop()
 */


#if 0

void ZZZbuttonDisplayTest() {
  for (int b = 0; b < 4; b++) {

    if (buttonCurrMask & (1 << b) ) {

      uint32_t c;

      switch (b) {
        case 0:
          c = Adafruit_NeoPixel::Color(255, 0, 0);
          Serial.println("255-0-0");
          break;
        case 1:
          c = Adafruit_NeoPixel::Color(0, 255, 0);
          Serial.println("0-255-0");
          break;
        case 2:
          c = Adafruit_NeoPixel::Color(0, 0, 255);
          Serial.println("0-0-255");
          break;
        case 3:
          c = Adafruit_NeoPixel::Color(255, 255, 0);
          Serial.println("255-255-0");
          break;
      }
      colorWipeBlocking(strip[0], c, 10);
      colorWipeBlocking(strip[1], c, 10);
    }
  }
}



/**
 * MORE OLD STUFF...
 */


//Theatre-style crawling lights with rainbow effect
// increment counter 'x' after each call.
void ZZZtheaterChaseRainbowX(Adafruit_NeoPixel & strip, uint16_t x) {

  int j = x % 256; // 256 colors in the wheel
  int q = x % 4; // 4 LED crawl

  int del = x % 4; // delay loop (10 msec * 4)

  for (int i = 0; i < strip.numPixels(); i = i + 4) {
    strip.setPixelColor(i + q, Wheel( (i + j) % 255)); //turn every third pixel on
  }
  strip.show();

  delay(50); // FIXME use 'del' delay with loop counter instead of this delay.

  for (int i = 0; i < strip.numPixels(); i = i + 4) {
    strip.setPixelColor(i + q, 0);      //turn every third pixel off
  }
}


void ZZZrainbow(uint8_t wait) {
  uint16_t i, j;

  for(j=0; j<256; j++) {
    for(i=0; i<strip.numPixels(); i++) {
      strip.setPixelColor(i, Wheel((i+j) & 255));
    }
    strip.show();
    delay(wait);
  }
}

// Slightly different, this makes the rainbow equally distributed throughout
void ZZZrainbowCycle(uint8_t wait) {
  uint16_t i, j;

  for(j=0; j<256*5; j++) { // 5 cycles of all colors on wheel
    for(i=0; i< strip.numPixels(); i++) {
      strip.setPixelColor(i, Wheel(((i * 256 / strip.numPixels()) + j) & 255));
    }
    strip.show();
    delay(wait);
  }
}

//Theatre-style crawling lights.
void ZZZtheaterChase(uint32_t c, uint8_t wait) {
  for (int j=0; j<10; j++) {  //do 10 cycles of chasing
    for (int q=0; q < 3; q++) {
      for (int i=0; i < strip.numPixels(); i=i+3) {
        strip.setPixelColor(i+q, c);    //turn every third pixel on
      }
      strip.show();

      delay(wait);

      for (int i=0; i < strip.numPixels(); i=i+3) {
        strip.setPixelColor(i+q, 0);        //turn every third pixel off
      }
    }
  }
}

#endif




