
# DFPlayer uses single subdir called: mp3/
# mp3 files must be numbered 0001.mp3, 0002.mpg, ...

# by shorting ADKEY1 (pin 12) to ground, we play mp3/0001.mpg
# by shorting ADKEY2 (pin 13) to ground, we play mp3/0005.mpg

# connect pin 1 to VCC (4.2V)
# connect pin 7 to GND

# NOTE: use high freq choke (HFC) to power, to clean Arduino CPU noise

# See http://www.dfrobot.com/wiki/index.php/DFPlayer_Mini_SKU:DFR0299
# Use (3)  I/O Mode example

# http://www.banggood.com/DFPlayer-Mini-MP3-Player-Module-For-Arduino-p-969191.html

mkdir -p           /media/saj/IMG/01
cp welcome.mp3     /media/saj/IMG/01/0001.mp3
#cp hey.mp3         /media/saj/IMG/01/0002.mp3
#cp hey.mp3         /media/saj/IMG/01/0003.mp3
cp hey.mp3         /media/saj/IMG/01/0004.mp3
cp its.mp3         /media/saj/IMG/01/0005.mp3


