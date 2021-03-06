# LightArcade

## Arduino-based arcade game with WS2812 LED strip, similar to 1980's Track &amp; Field

##Made by Greg Schroeder & Scott James

We are members of [Suncoast Makers](http://suncoastmakers.org/)

Hosted in Sarasota, FL at [Suncoast Science Center](http://www.suncoastscience.org)

We wanted to create a very interactive and intense video arcade game that could be enjoyed by all at the [Orlando Maker Faire 2015](http://www.makerfaireorlando.com/2015/).  I do most electronic projects with Eagle CAD and homemade PCB, with switch debouncing done in software.

However, this time I wanted to try some new techniques.  Emphasis was placed on learning new build styles rather than just "best" practice.  It worked, I've learned a lot and had lots of fun doing it.  

## Goals in the hardware/software build:

  1. Use a veroboard, with DIYLC for layout, instead of custom PCB.
  2. Use DIYLC instead of Eagle CAD.
  3. Use custom plugs with Silicon RTV and Sugru connectors instead of off-the-shelf PCB connectors.  This seems silly, but I really wanted to try it.
  4. Instead of just Arduino, use Firmata along with Raspberry Pi.
  5. Finally, put game in Arduino, and push out text reports on Serial bus for use by listening Raspberry Pi.
     

## Things to try (if time allows):

  1. Integrate Sound playback, with MP3 decoder/playback; use DFPlayer module.
  2. Use WiFi bridge (with ESP8266) to broadcast high scores, engage users.

## The Build

Pictures of the [Build of LightArcade](https://goo.gl/photos/uvECDhv7FLh9xkrC6 "LightArcade")

## Building and Programming Resources

http://blog.oscarliang.net/raspberry-pi-and-arduino-connected-serial-gpio/

http://elinux.org/RPi_Low-level_peripherals#Model_A.2B.2C_B.2B_and_B2

http://makezine.com/projects/tutorial-raspberry-pi-gpio-pins-and-python/

## Pygame for Raspberry Pi graphics and sounds

http://www.pygame.org/docs/tut/newbieguide.html

## Sounds

http://www.freesound.org/search/?q=GO

## Source Code
https://github.com/scottjames/LightArcade

# Parts (some)

1" arcade style buttons
http://www.ebay.com/itm/291190314286

ATMEGA328P 16MHz 5V Compatible to Arduino PRO mini
http://www.ebay.com/itm/191679508570

Prototyping PCB Circuit Board Stripboard 94x53mm (veroboard)
http://www.ebay.com/itm/Prototyping-PCB-Circuit-Board-Stripboard-94x53mm-/260829423263

# Software

DIYLC - Do It Yourself Layout Creator
https://code.google.com/p/diy-layout-creator/

For another alternative, try Fritzing
http://fritzing.org/home/


