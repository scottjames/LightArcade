#!/usr/bin/python

# Light Arcade backend driver
# 2015 Scott James
#
# Read text reports from Arduino
# Parse reports and display on scoreboard
# During DEMO cycle, show pictures of building Light Arcade
# Those pics are stored in attached thumbdrive (/media/PHOTOS/...)
# We played with various sizes of images to balance fast load / good quality
#
# This is a first attempt to use Pygame to display info in a scoreboard.
# it's not pretty, but it works. Lots of room for improvement...


import time
import glob
import pygame
import serial
import os, sys
from pygame.locals import *


# read serial text data reports from Arduino with Light Arcade
# parse game clock to display, player score, and game State
class LASerial:
    port = None
    state = None
    gameClock = None
    theClock = None 
    score0 = None
    score1 = None
    winner = None

    def __init__(self):
        self.port = serial.Serial("/dev/ttyAMA0", baudrate=115200, timeout=1.0)
        pass

    def doParseLine(self,buff):
        #print "PARSE buff=",buff
        terms = buff.split(';')
        for t in (terms):
            ###print "term: >%s<" % (t) 
            if t == '' or t is None:
                continue
            (k,v) = t.split('=')
            #print "key=%s / value=%s" % (k, v)
            if k == 'CLOCK':
                self.theClock = v 
            elif k == 'gameClock':
                self.gameClock = v 
            elif k == 'SCORE0':
                self.score0 = v 
            elif k == 'SCORE1':
                self.score1 = v 
            elif k == 'state':
                self.state = v 
            elif k == 'winner':
                self.winner = v 


# return Dict with game model info
    def getModel(self):
        d = {'score0':self.score0, 'score1':self.score1,
            'msg':'Clock', 'clock':self.gameClock,
            'winner':self.winner, 'theClock':self.theClock }
        return d

# handle TICK report
    def doTICK(self,buff):
        print "TICK: buff=",buff
        self.doParseLine(buff)

    def doSerialEvent(self):
        buff=''
        line = self.port.readline()
        buff = line.strip("\r\n")
        #print "buf=%s" % buff
# skip lines with #, which are comments not to be processed
        if buff == '' or buff[0] == '#':
            print "skip empty/comment line:", buff
            self.state = ''
            return
# other lines are COMMAND:<LIST;OF;TERMS>
        (cmd,terms) = buff.split(':')
        if (cmd == 'TICK'):
            self.doTICK(terms)

        print "... INFO: state=%s gameClock=%s Score0=%s Score1=%s" % (self.state, self.gameClock, self.score0, self.score1)

# Do something once values are set...
# if DEMO, then show slides show.. have slideCounter for 3-5 secs
# if START, then show Ready Go, play buzz/GO sound
# if WINNER, then show Winner RED/BLUE.. play sound??
# if PLAY, then show clock, scores

        # return the state
        return self.state


# Slide show for displaying images of building Light Arcade.
class Photo:
    photos = []
    #iterphoto = None
    photoposttime = 0  # time when shown, 
    screen = None
    background = None
    photosecs = 3  # show for a few secs
    fullscreen = False   # current state; toggle for fullscreen
    # play around with photo sizes, balance for best quality/fast load
    ##height = 480
    ##width = int(height/1.777)
    width = 640
    height = int(width / 1.333)
    ## width = 800
    ## height = int(width/1.777)

    def __constructor__():
        pass

    # read photos/ for list of files
    def loadPhotos(self,srcdir):
        self.photos = glob.glob(srcdir)
        self.photos.sort()
        pass

    def getNextPhoto(self):
        p = self.photos.pop(0)
        #print "p=",p
        self.photos.append(p)
        return p

    # show next img in list, update photoposttime timestamp
    def showPhoto(self,ph):
        print "photo: ", ph
        self.img = pygame.image.load(ph).convert()
# TODO get dimensions, and resize to window...
        ## rot = pygame.transform.rotate(self.img, -90)
        ## trans = pygame.transform.scale(rot, (self.width,self.height))
        ## trans = pygame.transform.scale(img, (self.width,self.height))
# blit photo to screen
        self.screen.blit(self.background, (0, 0))
        self.screen.blit(self.img,
                (self.screen.get_rect().centerx - 
                    self.img.get_rect().centerx, 0))
        #self.screen.blit(self.img, (0, 0))
        ## self.screen.blit(trans, (0, 0))
        pygame.display.flip()
# update timestamp
        self.photoposttime = pygame.time.get_ticks()
        pass

    # service photo show, return immediately
    # if photo shown > photosecs seconds, then rotate photo
    def showPhotos(self):
        # Event loop

        #pygame.time.delay(100)
        for event in pygame.event.get():
            if event.type == QUIT:
                return False
                #pygame.display.quit()
            elif event.type == VIDEORESIZE:
                print "got: video resize"
                self.screen=pygame.display.set_mode(event.dict['size'],HWSURFACE|DOUBLEBUF|RESIZABLE)
                #screen.blit(pygame.transform.scale(pic,event.dict['size']),(0,0))
                self.makeBackground()
                self.screen.blit(self.background, (0, 0))
                self.screen.blit(self.img,
                        (self.screen.get_rect().centerx - 
                            self.img.get_rect().centerx, 0))
                pygame.display.flip()
        if (self.photoposttime+(self.photosecs*1e3) < pygame.time.get_ticks() ):
            self.showPhoto( self.getNextPhoto() )
        return True

# show scoreboard with Player scores, game clock countdown
    def showScoreboard(self,d):
        topmargin = 10
        leftmargin = 10
        rightmargin = 10
        # Blue score upper left corner
        bluesfc = self.medfont.render("Blue", 1, (100, 100, 200))
        bluepos = bluesfc.get_rect()
        bluepos.x = leftmargin
        bluepos.y = topmargin
        score0sfc = self.medfont.render(str(d['score0']), 1, (250, 250, 250))
        score0pos = (leftmargin, bluepos.y + score0sfc.get_rect().height)
        # Red score upper right corner
        redsfc = self.medfont.render("Red", 1, (200, 100, 100))
        redpos = redsfc.get_rect()
        redpos.x = self.background.get_rect().width - redsfc.get_rect().width - rightmargin
        redpos.y = topmargin
        score1sfc = self.medfont.render(str(d['score1']), 1, (250, 250, 250))
        score1pos = (redpos.x, redpos.y + score1sfc.get_rect().height)
        # Msg in upper/center
        msgsfc = self.medfont.render(d['msg'], 1, (200, 200, 200))
        msgpos = msgsfc.get_rect()
        msgpos.centerx = self.background.get_rect().centerx
        msgpos.centery = self.background.get_rect().centery - 50
        # Clock in mid/center
        clocksfc = self.medfont.render(str(d['clock']), 1, (200, 200, 200))
        clockpos = clocksfc.get_rect()
        clockpos.centerx = self.background.get_rect().centerx
        clockpos.centery = self.background.get_rect().centery + 50
        # Blit everything to the screen
        self.screen.blit(self.background, (0, 0))
        self.screen.blit(bluesfc, bluepos)
        self.screen.blit(redsfc, redpos)
        self.screen.blit(msgsfc, msgpos)
        self.screen.blit(clocksfc, clockpos)
        self.screen.blit(score0sfc, score0pos)
        self.screen.blit(score1sfc, score1pos)
        # Display to screen
        pygame.display.flip()

#    def showTick(self):
#        pass

    def showDEMO(self,d):
        # do slide show...
        pass

    def showSTART(self,d):
        d['msg'] = 'Ready'
        d['clock'] = 'Go!'
        self.showScoreboard(d)
#  play audio 'start'
        chanstart = self.soundstart.play()
        while chanstart.get_busy():
            pygame.time.delay(50)


    def showWINNER(self,d):
        d['msg'] = 'Winner'
        winner = 'Tie'
        if d['score0'] > d['score1']:
            winner = 'BLUE'
        elif d['score0'] < d['score1']:
            winner = 'RED'
        d['clock'] = winner
        self.showScoreboard(d)
#  play audio 'win'
        chanwin = self.soundwin.play()
        while chanwin.get_busy():
            pygame.time.delay(50)

    def showPLAY(self,d):
        self.showScoreboard(d)
        pass

# start up title page
    def showTitle(self):
        # Display some text
        textsfc = self.medfont.render("Light Arcade", 1, (200, 200, 200))
        textpos = textsfc.get_rect()
        textpos.centerx = self.background.get_rect().centerx
        textpos.centery = self.background.get_rect().centery
        # Blit everything to the screen
        self.screen.blit(self.background, (0, 0))
        self.screen.blit(textsfc, textpos)
        pygame.display.flip()

# make 80's style green crt screen as background image
    def makeBackground(self):
        # Make a background surface for current screen
        self.background = pygame.Surface(self.screen.get_size())
        #self.background = self.background.convert()
        self.background.fill((20, 20, 20))
        w,h = self.screen.get_size()
        print "w,h=",w,h
        for y in range(0,h,4):
            points = [ (0,y) , (w,y) ]
            thickness = 1
            pygame.draw.lines(self.background,(30,60,30), False, points, thickness)
            pass

# make resizeable screen
    def setup(self):
        # Initialise screen
        pygame.init()
        self.screen = pygame.display.set_mode((self.width, self.height),HWSURFACE|DOUBLEBUF|RESIZABLE)
        pygame.display.set_caption('Light Arcade')

# init sounds
        pygame.mixer.init()
        print "get sound tick.wav"
        self.soundtick= pygame.mixer.Sound("tick.wav")  #works!
        self.soundwin= pygame.mixer.Sound("win.wav")  #works!
        self.soundstart= pygame.mixer.Sound("start.wav")  # just a pop

        self.makeBackground()

        # setup medium-sized font
        self.medfont = pygame.font.Font(None, 48)

        # Blit everything to the screen
        self.screen.blit(self.background, (0, 0))
        pygame.display.flip()



#
# run it
#
if __name__ == '__main__':
    show = Photo()
    show.setup()
    show.showTitle()
    #show.loadPhotos('/media/PHOTOS/lightarcade800h/*.jpg')
    show.loadPhotos('/media/PHOTOS/lightarcade640h/*.jpg')
    ser = LASerial()

    working = True
    while working:
        gamestate = ser.doSerialEvent()
        d = ser.getModel()
        print "state=",gamestate

        if gamestate == 'DEMO':
            working = show.showPhotos()
            show.showDEMO(d)
        elif gamestate == 'START':
            show.showSTART(d)
        elif gamestate == 'WINNER':
            show.showWINNER(d)
        elif gamestate == 'PLAY':
            show.showPLAY(d)

