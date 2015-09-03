#!/usr/bin/python

import time
import glob
import pygame
from pygame.locals import *


class Photo:
    photos = []
    #iterphoto = None
    photoposttime = 0  # time when shown, 
    screen = None
    background = None
    photosecs = 3  # show for a few secs
    fullscreen = False
    size = []
    #width = 5312
    #height = 2988
    #width = 768
    #height = int(width * 1.777)
    height = 1024
    width = int(height/1.777)
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
        self.img = pygame.image.load(ph)
        #self.img = pygame.image.load(self.ph).convert()
# TODO get dimensions, and resize to window...
        rot = pygame.transform.rotate(self.img, -90)
        trans = pygame.transform.scale(rot, (self.width,self.height))
# blit photo to screen
        self.screen.blit(self.background, (0, 0))
        #self.screen.blit(self.img, (0, 0))
        self.screen.blit(trans, (0, 0))
        pygame.display.flip()
# update timestamp
        self.photoposttime = pygame.time.get_ticks()
        pass

    # service photo show, return immediately
    # if photo shown > photosecs seconds, then rotate photo
    def showPhotos(self):
        # Event loop
# FIXME put while loop outside showPhotos(), so we can service Serial, looking for commands...
        while 1:
            key = pygame.key.get_pressed()
            if key[pygame.K_x] or key[pygame.K_q] or key[pygame.K_ESCAPE]:
                return False
            if key[pygame.K_f]:
                self.fullscreen = not self.fullscreen
                if self.fullscreen:
                    print "Toggle fullscreen"
                    pygame.event.set_grab(True)  # fullscreen needs to grab all input
                    self.screen = pygame.display.set_mode(self.size, pygame.FULLSCREEN)
                    #pygame.display.toggle_fullscreen()
                else:
                    print "Toggle restore screen"
                    pygame.event.set_grab(False)
                    self.screen = pygame.display.set_mode(self.size, pygame.RESIZABLE)
                    #pygame.display.toggle_fullscreen()
                    pass
            for event in pygame.event.get():
                if event.type == QUIT:
                    return False
                if event.type == KEYDOWN:
                    print 'Got KEYDOWN'
                    pass
            if (self.photoposttime+(self.photosecs*1e3) < pygame.time.get_ticks() ):
                self.showPhoto( self.getNextPhoto() )
            pygame.time.delay(100)


    def setup(self):
            # Initialise screen
            pygame.init()
            #screen = pygame.display.set_mode((640, 480))
            #self.screen = pygame.display.set_mode((self.width, self.height))
            #pygame.display.init()
            pygame.display.set_caption('Light Arcade')

            self.size = (pygame.display.Info().current_w, pygame.display.Info().current_h)
            print "Display.Info()  size: %d x %d" % (self.size[0], self.size[1])
            #self.screen = pygame.display.set_mode(self.size, pygame.FULLSCREEN)
            self.screen = pygame.display.set_mode(self.size)
# Clear the screen to start
            self.screen.fill((0, 0, 0))        
# Initialise font support
            pygame.font.init()
# Render the screen
            pygame.display.update()


            # Fill background
            self.background = pygame.Surface(self.screen.get_size())
            self.background = self.background.convert()
            self.background.fill((250, 250, 250))

            # Display some text
            self.smallfont = pygame.font.Font(None, 36)
            self.largefont = pygame.font.Font(None, 72)
            self.hugefont = pygame.font.Font(None, 160)
            #text = self.largefont.render("Light Arcade", 1, (10, 10, 10))
            text = self.hugefont.render("Light Arcade", 1, (10, 10, 10))
            textpos = text.get_rect()
            textpos.centerx = self.background.get_rect().centerx
            textpos.centery = self.background.get_rect().centery
            self.background.blit(text, textpos)

            # Blit everything to the screen
            self.screen.blit(self.background, (0, 0))
            #self.screen.blit(self.background, textpos)
            pygame.display.flip()



if __name__ == '__main__':
    show = Photo()
    show.setup()
    show.loadPhotos('/home/saj/photos/*.jpg')

#    show.showNextPhoto()
#    time.sleep(2)
#    show.showNextPhoto()
#    time.sleep(2)
#    show.showNextPhoto()
#    time.sleep(2)

# FIXME read photos/ dir
    show.showPhotos()

#    main()

