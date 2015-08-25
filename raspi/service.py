
import glob
import pygame

# classes and script for raspi service to LightArcade Arduino
# commands:
#   DEMO
#   START
#   TICK
#   WINNER
#

class Photo():
    # DEMO -- show photos, rotate images in 'photos/*.jpg', 
    self._files = []
    def load(self,photodir):
        self._files = glob.glob(photodir)
        self._files.sort()

        pass
    def showNext():
        pass


class Clock():
    self._clocksecs
    def self.__constructor__():
        self._clocksecs = 0

        pass

    # START -- show 'Ready...'

    # TICK -- show clock value, count down

    # WINNER -- show 'Winner is Red'
    # WINNER -- show 'Winner is Blue'
    # WINNER -- show 'Tie'


class Command():
    # setup - open serial
    def setup(self):
        pass
    # monitor - listen to text lines, parse command
    def monitor(self):
        pass

class Route():
# switch gamestate, call above
    def routeState(self):
        cmd = Command();
        pass

#if main...
    photo = Photo()
    photo.load('photos/*.jpg')

    c=Clock()
    cmd=Command()
    r=Route(cmd)

