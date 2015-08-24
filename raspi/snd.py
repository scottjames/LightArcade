
import pygame

pygame.init()
pygame.mixer.init()

print "get sound tick.wav"
soundtick= pygame.mixer.Sound("tick.wav")  #works!
soundwin= pygame.mixer.Sound("win.wav")  #works!
soundstart= pygame.mixer.Sound("start.wav")  # just a pop

print "play sound tick.wav"
#chanwin = soundwin.play()
#chantick = soundtick.play()

chanstart = soundstart.play()
while chanstart.get_busy():
    pygame.time.delay(200)

pygame.time.delay(800)
soundtick.play()
pygame.time.delay(800)
soundtick.play()
pygame.time.delay(800)

chanwin = soundwin.play()
while chanwin.get_busy():
    pygame.time.delay(800)

