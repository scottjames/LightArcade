import pygame

pygame.init()
# pygame.FULLSCREEN mess up development screen
#screen = pygame.display.set_mode((640, 480), pygame.NOFRAME | pygame.FULLSCREEN)
screen = pygame.display.set_mode((640, 480), pygame.NOFRAME )
screen.fill((0, 0, 0))
clock = pygame.time.Clock()
done = False

pygame.font.init()

#font_path = "./fonts/neon.ttf"
#font_size = 32

#font_path = "./fonts/digital.ttf"
font_path = "./fonts/arcade.ttf"
font_size = 128

#font_path = "./fonts/Computerfont.ttf"
clockfont = pygame.font.Font(font_path, font_size)

#clockfont = pygame.font.SysFont("monospace", 96)



def drawit(secs):

# fill with black background
    screen.fill((0, 0, 0))

    title = clockfont.render("Drag Race", True, (255, 0, 0))
    screen.blit(title,
        (320 - title.get_width() // 2, 100 ) )

# get clock digits
    #text = clockfont.render( ("00:%02d" % secs), True, (255, 0, 0))
    text = clockfont.render( ("%02d" % secs), True, (255, 0, 0))

# draw clock digits to center of screen
    screen.blit(text,
        (320 - text.get_width() // 2, 240) )
#        (320 - text.get_width() // 2, 240 - text.get_height() // 2))

# show surface    
    pygame.display.flip()

# update clock 60 fps
#    clock.tick(60) 


seconds = 15
while not done and seconds > 0:
    for event in pygame.event.get():
        if event.type == pygame.QUIT:
            done = True
        if event.type == pygame.KEYDOWN and event.key == pygame.K_ESCAPE:
            done = True
    drawit(seconds)
# wait 1 sec
    pygame.time.wait(1000)
    seconds -= 1

