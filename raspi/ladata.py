import serial
import time

#port = serial.Serial("/dev/ttyAMA0", baudrate=115200, timeout=3.0)
port = serial.Serial("/dev/ttyUSB0", baudrate=115200, timeout=5.0)

state = ''
gameClock = ''
theClock = ''
score0 = ''
score1 = ''

buff=''
# read 10 lines...
for i in range(10):
    #port.write("\r\nSay something:")
    #rcv = port.read(10)
    #port.write("\r\nYou sent:" + repr(rcv))
    line = port.readline()
    #line = port.readline(eol="\n")
    buff = line[:-2]  # strip \n
    print "[%d] buf=%s" % (i,buff)
    if buff[0] == '#' or buff =='':
        print "skip comment line:", buff
        continue
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
            print "key=%s / value=%s" % (k, v)
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
    else:
        print "?? report ", cmd

    print "INFO: state=%s gameClock=%s Score0=%s Score1=%s" % (state, gameClock, score0, score1)


def doCommand(c,v):
    pass


