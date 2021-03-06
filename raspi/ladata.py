
import pygame
import pygame.sndarray

import serial
import time
#import Sound

#pygame.mixer.init()
#pygame.mixer.music.load("tick.wav")


slideSecs = 0
slideNum  = 0

def showNextSlide():
    global slideNum
    print "SLIDE: show slide ", slideNum
    #if slideNum++ > 200:  ...whatever
    slideNum += 1
# show up to MAX slides, then repeat
    if (slideNum > 10):
        slideNum = 0


# arduino sends TICK each second, 3 calls here is 3 seconds
def doDEMO():
    global slideSecs
    print ""
    print "DEMO:"
    slideSecs -= 1
    if slideSecs <= 0:
        #print "DEMO: show slide..."
        showNextSlide()
        slideSecs = 5

    #print "DEMO: (play music?? nah, annoying...)"
    print ""
    pass

def doSTART():
    print ""
    print "START: Ready, Set, GO!"
    print "START: (play buzzz/go)"
    print ""
    pass

def doWINNER():
    print ""
    print "WINNER: player ", winner
    print "WINNER: (play victory music)"
    print ""
    pass

# use tick sound from 24-hours tv show??
def doPLAY():
    print "PLAY: clock tick..." 
    print ""
    print "score0=%s             score1=%s" %(score0,score1)
    print "              %s", gameClock
    print ""
    pass




#port = serial.Serial("/dev/ttyAMA0", baudrate=115200, timeout=3.0)
port = serial.Serial("/dev/ttyUSB0", baudrate=115200, timeout=5.0)

state = ''
gameClock = ''
theClock = ''
score0 = ''
score1 = ''
winner = ''

slideSecs=3
slideNum = 0


buff=''
# read 10 lines...
# while (1):
for i in range(200):
    #line = port.readline(eol="\r\n")
    line = port.readline()
    buff = line[:-2]  # strip \r\n from Arduino
    print "[%d] buf=%s" % (i,buff)
# skip lines with #, which are comments not to be processed
    if buff == '' or buff[0] == '#' or (buff.find(':') < 0): 
        print "skip comment line:", buff
        continue
# other lines are COMMAND:<LIST;OF;TERMS>
    (cmd,terms) = buff.split(':')
    if (cmd == 'TICK'):
        print "tick report..."
        print "buff=",buff
        a = buff.split(':')[1].split(';')
        for t in (terms.split(';')):
            print "term: >%s<" % (t)
            if t == '' or t is None:
                continue
            (k,v) = t.split('=')
            #print "key=%s / value=%s" % (k, v)
            if k == 'CLOCK':
                theClock = v
            elif k == 'gameClock':
                gameClock = v
            elif k == 'SCORE0':
                score0 = v
            elif k == 'SCORE1':
                score1 = v
            elif k == 'state':
                state = v
            elif k == 'winner':
                winner = v
    else:
        print "?? report ", cmd

    print "... INFO: state=%s gameClock=%s Score0=%s Score1=%s" % (state, gameClock, score0, score1)

# Do something once values are set...
# if DEMO, then show slides show.. have slideCounter for 3-5 secs
# if START, then show Ready Go, play buzz/GO sound
# if WINNER, then show Winner RED/BLUE.. play sound??
# if PLAY, then show clock, scores
    if state == 'DEMO':
        doDEMO()
    elif state == 'START':
        doSTART()
    elif state == 'WINNER':
        doWINNER()
    elif state == 'PLAY':
        doPLAY()
