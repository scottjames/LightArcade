#include <Adafruit_NeoPixel.h>

/**
 * TitleSignFlasher for Light Arcade
 * Acrylic sign with laser-etch logo is side lit with strip of WS2812
 * this flashes white, and cycles through colors
 */
 
// Parameter 1 = number of pixels in strip
// Parameter 2 = Arduino pin number (most are valid)
// Parameter 3 = pixel type flags, add together as needed:
//   NEO_KHZ800  800 KHz bitstream (most NeoPixel products w/WS2812 LEDs)
//   NEO_KHZ400  400 KHz (classic 'v1' (not v2) FLORA pixels, WS2811 drivers)
//   NEO_GRB     Pixels are wired for GRB bitstream (most NeoPixel products)
//   NEO_RGB     Pixels are wired for RGB bitstream (v1 FLORA pixels, not v2)

//Adafruit_NeoPixel strip1 = Adafruit_NeoPixel(8, 10, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel strip1 = Adafruit_NeoPixel(24, 10, NEO_GRB + NEO_KHZ800);


uint32_t x; // loop counter to index sequences


// IMPORTANT: To reduce NeoPixel burnout risk, add 1000 uF capacitor across
// pixel power leads, add 300 - 500 Ohm resistor on first pixel's data input
// and minimize distance between Arduino and first pixel.  Avoid connecting
// on a live circuit...if you must, connect GND first.

void setup() {
  strip1.begin();
  strip1.show(); // Initialize all pixels to 'off'
  //strip2.begin();
  //strip2.show(); // Initialize all pixels to 'off'
  x = 0;
}

void loop() {

  // white glowing surge pulses
//  for (int i = 0; i < 3; i++) {
//    uint8_t wt = 1+2*random(3);
//    colorFade(strip1, strip1.Color(255, 255, 255), wt); // white with random wait speed
//    colorFade(strip1, strip1.Color(0, 0, 0), 5); // black
//    delay(1+4*random(3));    
//  }

  colorGlowingSurgePulses(strip1, strip1.Color(255, 255, 255) );

  delay(1000);
  strip1.setBrightness(255); // min
  colorLightning(strip1, strip1.Color(255, 255, 255), 20); // 20 flashes 
  strip1.setBrightness(0); // min
  strip1.show();  
  delay(2000);
  
  strip1.setBrightness(255); // min
  colorLightning(strip1, strip1.Color(255, 255, 255), 20); // 10 flashes
  strip1.setBrightness(0); // min
  strip1.show();  
  delay(1000);
  

  // colorFade speed=10 (dramatic glow), speed=2 (pulse/surge)
  //colorFade(strip1, strip1.Color(255, 255, 255), 8); // white with random wait speed
  //colorFade(strip1, strip1.Color(0, 0, 0), 1); // black

  strip1.setBrightness(255); // max
  strip1.show();
  
  colorWipe(strip1, strip1.Color(255, 0, 0), 30); // Red
  delay(1000);
  colorWipe(strip1, strip1.Color(0, 255, 0), 30); // Green
  delay(1000);
  colorWipe(strip1, strip1.Color(0, 0, 255), 30); // Blue
  delay(1000);
  
//  theaterChaseRainbow(strip1, 20);
  
  colorWipe(strip1, strip1.Color(0, 0, 0), 1); // Black
  strip1.setBrightness(255); // max
  strip1.show();  
  delay(2000);

  theaterChase(strip1, strip1.Color(255, 0, 0), 150); // red
  theaterChase(strip1, strip1.Color(0, 255, 0), 150); // green
  theaterChase(strip1, strip1.Color(0, 0, 255), 150); // blue
  theaterChase(strip1, strip1.Color(255, 255, 0), 150); // yellow
  theaterChase(strip1, strip1.Color(0, 255, 255), 150); // cyan
  theaterChase(strip1, strip1.Color(255, 0, 255), 150); // magenta
  
  colorGlowingSurgePulses(strip1, strip1.Color(255, 0, 0) ); // red
  colorGlowingSurgePulses(strip1, strip1.Color(0, 255, 0) ); // green
  colorGlowingSurgePulses(strip1, strip1.Color(0, 0, 255) ); // blue
  colorGlowingSurgePulses(strip1, strip1.Color(255, 255, 0) ); // yellow
  colorGlowingSurgePulses(strip1, strip1.Color(0, 255, 255) ); // cyan
  colorGlowingSurgePulses(strip1, strip1.Color(255, 0, 255) ); // magenta

  
  x++; // loop counter
}


// Fill the dots one after the other with a color
void colorGlowingSurgePulses(Adafruit_NeoPixel& strip, uint32_t c) {
  
  // some glowing surge pulses
  for (int i = 0; i < 5; i++) {
    uint8_t wt = 1+2*random(3);
    colorFade(strip1, c, wt); // with random wait speed
    colorFade(strip1, strip1.Color(0, 0, 0), 5); // black
    delay(1+4*random(3));    
  }
  
}


// Fill the dots one after the other with a color
void colorWipe(Adafruit_NeoPixel& strip, uint32_t c, uint8_t wait) {
  for (uint16_t i = 0; i < strip.numPixels(); i++) {
    strip.setPixelColor(i, c);
    strip.show();
    delay(wait);
  }
}


// Fill the dots one after the other with a color
void colorFade(Adafruit_NeoPixel& strip, uint32_t c, uint8_t wait) {

    for (uint16_t i = 0; i < strip.numPixels(); i++) {
      strip.setPixelColor(i, c);
    }
    strip.show();
    
  for (uint8_t b = 1; b <255; b++) {
    strip.setBrightness(b);
    strip.show();
    delay(wait);
  }
  for (uint8_t b = 255; b > 1; b--) {
    strip.setBrightness(b);
    strip.show();
    delay(wait);
  }
}



// Make random flashing lightning
void colorLightning(Adafruit_NeoPixel& strip, uint32_t c, uint8_t cnt) {

  for (uint16_t i = 0; i < strip.numPixels(); i++) {
    strip.setPixelColor(i, c);
  }
    
  for (uint8_t d = 0; d <cnt; d++) {
    
    uint16_t rnd_flash = 2 + 12*random(4);
    uint16_t rnd_delay = 2 + 12*random(12);
    
    strip.setBrightness(255);
    strip.show();
    //delay( 2 + 8 * (d & 0x0f) );
    delay(rnd_flash);
    
    strip.setBrightness(1);
    strip.show();
    //delay( 4 + 12 * (d & 0x0f) );
    delay(rnd_delay);
  }
  
}





/*********
void rainbow(uint8_t wait) {
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
void rainbowCycle(uint8_t wait) {
  uint16_t i, j;

  for(j=0; j<256*5; j++) { // 5 cycles of all colors on wheel
    for(i=0; i< strip.numPixels(); i++) {
      strip.setPixelColor(i, Wheel(((i * 256 / strip.numPixels()) + j) & 255));
    }
    strip.show();
    delay(wait);
  }
}


*****/

//Theatre-style crawling lights.
void theaterChase(Adafruit_NeoPixel& strip, uint32_t c, uint8_t wait) {
  
  for (int j=0; j<30; j++) {  //do 30 cycles of chasing
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


//Theatre-style crawling lights with rainbow effect
// increment counter 'x' after each call.
void theaterChaseRainbow(Adafruit_NeoPixel& strip, uint16_t x) {

  //  for (int j=0; j < 256; j++) {     // cycle all 256 colors in the wheel
  //    for (int q=0; q < 3; q++) {
  int j = x % 256;
  int q = x % 4;

  for (int i = 0; i < strip.numPixels(); i = i + 3) {
    strip.setPixelColor(i + q, Wheel( (i + j) % 255)); //turn every third pixel on
  }
  strip.show();

  delay(50);

  for (int i = 0; i < strip.numPixels(); i = i + 3) {
    strip.setPixelColor(i + q, 0);      //turn every third pixel off
  }
  //    }
  //  }
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

