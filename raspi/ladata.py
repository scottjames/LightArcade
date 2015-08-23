import serial
import time

#port = serial.Serial("/dev/ttyAMA0", baudrate=115200, timeout=3.0)
port = serial.Serial("/dev/ttyUSB0", baudrate=115200, timeout=5.0)

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
    else:
        print "?? report ", cmd
