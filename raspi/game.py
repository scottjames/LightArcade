'''
Created on Jul 30, 2011
@author: jake (ninmonkeys@gmail.com)
@about: demo of Text class, easily extendable
see also:
    ...
    
Other people's projects:
    http://www.pygame.org/project-Reader-1813-.html
    http://www.pygame.org/docs/ref/font.html
    http://www.pygame.org/wiki/SimpleFontManager?parent=CookBook

# Todo: #
            -cleanup code comments before posting http://www.pygame.org
        #Todo: -unit testing
        -done? todo: test exception on bad fontname (load_font)
        
        - __all__ = TextLine, TextFPS, FontManager
        -
    
'''
import logging
import pygame
from pygame.locals import *
from pygame import Color, Rect, Surface
# works
from text.text import FontManager
#todo: fix import so * is not needed
    #from text.text import *

DEBUG_OFF = True
f = FontManager()
    #    t  = TextLine('foo')
    #    print 'done'
    
if not DEBUG_OFF:    
    WINDOW_TITLE = "demo.py nin's [ Text.py ]"
    fontnames = dict()
    #todo: fontthemes = dict() # "normal", "mono", "sans", "serif", "script", ... 
    
    color_fg = Color("gray80") 
    color_bg = Color('gray10')
    logging.basicConfig(level=logging.ERROR)
    font_ui = None
    #Todo: aliases: , fontmanager.alias(...)
        #fontnames = dict()
        #fontnames['ui'] = load_font('consolas', 22) 
    
    fontmanager = FontManager()
    
    class GameMain(object):
        done = False
        def __init__(self, width=600, height=400):
            """Initialize PyGame window."""
            self.color_bg, self.color_fg = color_bg, color_fg
            pygame.init()
    
            # save w, h, and screen
            self.width, self.height = width, height
            self.screen = pygame.display.set_mode(( self.width, self.height ))
            pygame.display.set_caption( "text/__init__.py : quick demo")        
    
            # fps clock, limits max fps
            self.clock = pygame.time.Clock()
            self.limit_fps = True
            self.fps_max = 60    
            
            # text init
            self.fonts = fontmanager
            fonts = self.fonts
            
            font_ui = fonts.add('consolas', 40)                                    
    #        font_ui = fonts.add('segoe ui', 30)                                    
            fonts.add('constantia', 36)
            
            self.text1 = TextLine("hi world!! hi ...")
            self.text2 = TextLine("hi world!! hi ...", fontkey = font_ui, aa=False)
    #        self.textfps = TextFPS()
            
            self.text1.rect.topleft = (10,300)
            self.text2.rect.topleft = (100,100)
            print self.text1        
            print self.text2        
            print self.fonts
            
            # text list / array
            self.textlist = [TextLine("foobar {}".format(i), fontkey=('arial',4*i+8)) for i in range(10)]
            r = Rect(0,0,1,1)
            for i,t in enumerate(self.textlist):
                t.rect.topleft = (200, i * 20)
                print t
                
                
            
        def draw(self):
            """draw screen"""
            self.screen.fill(self.color_bg)
            pygame.draw.rect(self.screen, Color("gray40"), Rect(0,0, self.width/2, self.height ))
            
            self.text1.draw()
            self.text2.draw()
            for t in self.textlist: t.draw()
    
            pygame.display.flip()
            
        def handle_events(self):
            """handle events: keyboard, mouse, etc."""
            events = pygame.event.get()
            
            for event in events:
                if event.type == pygame.QUIT: self.done = True
                # event: keydown
                elif event.type == KEYDOWN:
                    if event.key == K_ESCAPE or event.key == K_SPACE: self.done = True                
    
        def main_loop(self):
            """Game() main loop."""
            while not self.done:
                self.handle_events()        
    #            self.update()
                self.draw()
                
                # cap FPS if: limit_fps == True
                if self.limit_fps: self.clock.tick( self.fps_max )
                else: self.clock.tick()
                
            print self.fonts
        
        

if __name__ == "__main__":
    if not DEBUG_OFF:         
        logging.warning("import from demo doesn't work ATM")
        #    import text
        game = GameMain()
        game.main_loop()    
        print("Example does not use cached surfaced.")
    else: 
        print 'done'
        import sys
        print sys.version

