#!/usr/bin/python

# Light Arcade

import time
import glob
import pygame
import serial
import os, sys
from pygame.locals import *


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
        print "PARSE buff=",buff
        #a = buff.split(':')[1].split(';')
        #terms = buff.split(':')[1].split(';')
        terms = buff.split(';')
        for t in (terms):
            print "term: >%s<" % (t) 
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
#        while (1):
#        for i in range(200):
        line = self.port.readline()
        buff = line.strip("\r\n")
        #buff = line[:-2]  # strip \r\n from Arduino
        print "buf=%s" % buff
# skip lines with #, which are comments not to be processed
        #if buff == '' or buff[0] == '#' or (buff.find(':') < 0):
        if buff == '' or buff[0] == '#':
            print "skip empty/comment line:", buff
            self.state = ''
            return
            #continue
# other lines are COMMAND:<LIST;OF;TERMS>
        (cmd,terms) = buff.split(':')
        if (cmd == 'TICK'):
            self.doTICK(terms)
        #elif (cmd != 'TICK'):
        #    next

        print "... INFO: state=%s gameClock=%s Score0=%s Score1=%s" % (self.state, self.gameClock, self.score0, self.score1)

# Do something once values are set...
# if DEMO, then show slides show.. have slideCounter for 3-5 secs
# if START, then show Ready Go, play buzz/GO sound
# if WINNER, then show Winner RED/BLUE.. play sound??
# if PLAY, then show clock, scores

        # return the state
        return self.state


class Photo:
    photos = []
    #iterphoto = None
    photoposttime = 0  # time when shown, 
    screen = None
    background = None
    photosecs = 3  # show for a few secs
    fullscreen = False   # current state; toggle for fullscreen
    #width = 5312
    #height = 2988
    #width = 768
    #height = int(width * 1.777)
    #height = 1024
    height = 800
    width = int(height/1.777)
    ## width = 800
    ## height = int(width/1.777)

    def __constructor__():
        pass

    # read photos/ for list of files
    def loadPhotos(self,srcdir):
# FIXME read photos/ dir
        self.photos = glob.glob(srcdir)
        self.photos.sort()
        #self.photos.append('photos/20150619_080550.jpg')
        #self.photos.append('photos/20150619_080629.jpg')
        #self.photos.append('photos/20150805_000822.jpg')
        #self.iterphoto = iter(self.photos)
        pass

    def getNextPhoto(self):
        p = self.photos.pop(0)
        print "p=",p
        self.photos.append(p)
        return p

    # show next img in list, update photoposttime timestamp
    def showPhoto(self,ph):
# get next photo to show
        #self.ph = self.getNextPhoto()
        print "photo: ", ph
        self.img = pygame.image.load(ph).convert()
        #self.img = pygame.image.load(self.ph).convert()
# TODO get dimensions, and resize to window...
        ## rot = pygame.transform.rotate(self.img, -90)
        ## trans = pygame.transform.scale(rot, (self.width,self.height))
        ## trans = pygame.transform.scale(img, (self.width,self.height))
# blit photo to screen
        self.screen.blit(self.background, (0, 0))
        self.screen.blit(self.img, (0, 0))
        ## self.screen.blit(trans, (0, 0))
        pygame.display.flip()
# update timestamp
        self.photoposttime = pygame.time.get_ticks()
        pass

    # service photo show, return immediately
    # if photo shown > photosecs seconds, then rotate photo
    def showPhotos(self):
        # Event loop
# FIXME put while loop outside showPhotos(), so we can service Serial, looking for commands...
        #while 1:

        #TODO serial.readlin()  ... parse line...
        #pygame.time.delay(100)
        for event in pygame.event.get():
            if event.type == QUIT:
                return False
            if event.type == K_f:
                self.fullscreen = not self.fullscreen
                print "toggle full screen/window:"
                if self.fullscreen:
                    print "Fullscreen"
                else: 
                    print "Fullscreen"
        if (self.photoposttime+(self.photosecs*1e3) < pygame.time.get_ticks() ):
            self.showPhoto( self.getNextPhoto() )
        return True

# show scoreboard with info
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
        # Display to screen
        pygame.display.flip()

    def showTick(self):
        pass

    def showDEMO(self,d):
        # do slide show...
        pass

    def showSTART(self,d):
        d['msg'] = 'Ready'
        d['clock'] = 'Go!'
# TODO: play audio 'start'
        self.showScoreboard(d)
        pass

    def showWINNER(self,d):
        d['msg'] = 'Winner'
        d['clock'] = 'RED/BLUE'
# TODO: play audio 'winner'
        self.showScoreboard(d)
        pass

    def showPLAY(self,d):
        #d = {'score0':self.score0, 'score1':self.score1,
        #        'msg':'Clock', 'clock':self.gameClock }
        self.showScoreboard(d)
        pass

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

    def setup(self):
        # Initialise screen
        pygame.init()
# TODO get screenhight, calc self.height/width and setup for full-size window
        #screen = pygame.display.set_mode((640, 480))
        self.screen = pygame.display.set_mode((self.width, self.height))
        pygame.display.set_caption('Light Arcade')

        # setup medium-sized font
        self.medfont = pygame.font.Font(None, 48)

        # Make a background surface
        self.background = pygame.Surface(self.screen.get_size())
        self.background = self.background.convert()
        self.background.fill((20, 20, 20))
        # TODO draw alternate dark/gray lines for texture

        # Blit everything to the screen
        self.screen.blit(self.background, (0, 0))
        pygame.display.flip()



if __name__ == '__main__':
    show = Photo()
    show.setup()
    show.showTitle()
    #show.loadPhotos('/home/pi/photos/*.jpg')
    #show.loadPhotos('/media/lightarcade640h/*.jpg')
    show.loadPhotos('/media/PHOTOS/lightarcade800h/*.jpg')

#    show.showNextPhoto()
#    time.sleep(2)
#    show.showNextPhoto()
#    time.sleep(2)
#    show.showNextPhoto()
#    time.sleep(2)

# FIXME read photos/ dir
    ser = LASerial()

    #show.showSTART()
    #time.sleep(5)
    #sys.exit()

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


#    main()

