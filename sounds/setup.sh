
# DFPlayer uses single subdir called: mp3/
# mp3 files must be numbered 0001.mp3, 0002.mpg, ...

# by shorting ADKEY1 (pin 12) to ground, we play mp3/0001.mpg
# by shorting ADKEY2 (pin 13) to ground, we play mp3/0005.mpg

# connect pin 1 to VCC (4.2V)
# connect pin 7 to GND

# NOTE: use high freq choke (HFC) to power, to clean Arduino CPU noise
# also use 1K resistor on both RX, TX to cut noise from serial -> speaker.

# See http://www.dfrobot.com/wiki/index.php/DFPlayer_Mini_SKU:DFR0299
# Use (3)  I/O Mode example

# http://www.banggood.com/DFPlayer-Mini-MP3-Player-Module-For-Arduino-p-969191.html

rm -rf             /media/saj/IMG/mp3
mkdir -p           /media/saj/IMG/mp3

# 1 - 10 are good for intro/begin
cp welcome.mp3     /media/saj/IMG/mp3/0001.mp3
cp beware.mp3      /media/saj/IMG/mp3/0002.mp3
cp cannoteach.mp3  /media/saj/IMG/mp3/0003.mp3
cp clear.mp3       /media/saj/IMG/mp3/0004.mp3
cp do.mp3          /media/saj/IMG/mp3/0005.mp3
cp know.mp3        /media/saj/IMG/mp3/0006.mp3
cp learn.mp3       /media/saj/IMG/mp3/0007.mp3
cp lose.mp3        /media/saj/IMG/mp3/0008.mp3
cp unlearn.mp3     /media/saj/IMG/mp3/0009.mp3
cp willsee.mp3     /media/saj/IMG/mp3/0010.mp3

# 20 - 27 are good for closing...
cp bewithyou.mp3   /media/saj/IMG/mp3/0020.mp3
cp control.mp3     /media/saj/IMG/mp3/0021.mp3
cp cowards.mp3     /media/saj/IMG/mp3/0022.mp3
cp difficult.mp3   /media/saj/IMG/mp3/0023.mp3
cp passon.mp3      /media/saj/IMG/mp3/0024.mp3
cp patience.mp3    /media/saj/IMG/mp3/0025.mp3
cp powerful.mp3    /media/saj/IMG/mp3/0026.mp3
cp usefeelings.mp3 /media/saj/IMG/mp3/0027.mp3

