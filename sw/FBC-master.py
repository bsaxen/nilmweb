#===========================================
#!/usr/bin/python
#==================================================
# FCB-master.py
# 2016-05-02
#==================================================
import time
import httplib
import os
#==================================================
# Configuration
#==================================================
g_debug     = 'NO';
g_delay     = 2
g_name      = 'FBC-master'
#==================================================
while 1:
    if g_debug == 'YES':
        print '.'
    os.system("ls -R 2>/dev/null xFBC-* > FBC.work")
    file = open('FBC.work','r')
    for line in file:
        if 'FBC' in line:
            words=line.split('-')
            work=words[1].split('.')
            sender = work[0]
            action = work[1]
            if g_debug == 'YES':
                print 'Sender = ' + sender + ' Action = ' + action
            #-------------------------------------------------
            if 'clear' in action:
                cmd = 'rm -R 2>/dev/null %s/*' % (sender)
                os.system(cmd)
            if 'register' in action:
                cmd = 'mkdir %s' % (sender)
                os.system(cmd)
                cmd = 'touch %s.slave' % (sender)
                os.system(cmd)
            if 'delete' in action:
                cmd = 'rm -r %s' % (sender)
                os.system(cmd)
                cmd = 'rm %s.slave' % (sender)
                os.system(cmd)
            #-------------------------------------------------
    file.close()
    cmd = 'rm -R 2>/dev/null xFBC-*'
    os.system(cmd)
    time.sleep(g_delay)
# End of file