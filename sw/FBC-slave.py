#===========================================
#!/usr/bin/python
#==================================================
# FCB-slave.py
# 2016-05-02
#
# Note: This application requres set up of scp without password
# Client:
#	ssh-keygen -t rsa
#	ssh-copy-id <server> example adam@nilm.se
#
#==================================================
import time
import httplib
import os
#==================================================
# Read configuration
#==================================================
g_sender    = 803
g_debug     = 'YES';
g_delay     = 2
g_name      = 'FBC-slave'
g_scp       = 'folke@nilm.se:/home/folke/FBC'
#==================================================
local_ipaddress = 'x.x.x.x'
os.system("ifconfig > ipaddress.work")
file = open('ipaddress.work','r')
for line in file:
    if 'Bcast' in line:
        words=line.split(' ')
        work=words[11].split(':')
        local_ipaddress = work[1] 
        print 'local ipaddress ' + local_ipaddress
file.close()

cmd = 'touch xFBC-%s.register' % (g_sender)
os.system(cmd)
cmd = 'touch xFBC-%s.delete' % (g_sender)
os.system(cmd)
cmd = 'touch xFBC-%s.clear' % (g_sender)
os.system(cmd)

# Delete files from Master
cmd = 'SSH_AUTH_SOCK=0 scp xFBC-%s.register %s/.' % (g_sender, g_scp)
print cmd
os.system(cmd)
time.sleep(5)

while 1:
    # Get files from Master
    cmd = 'SSH_AUTH_SOCK=0 scp -r %s/%s .' % (g_scp,g_sender)
    print cmd
    os.system(cmd)
    # Delete files from Master
    cmd = 'SSH_AUTH_SOCK=0 scp xFBC-%s.clear %s/.' % (g_sender,g_scp,)
    print cmd
    os.system(cmd)
    
    cmd = 'ls -R 2>/dev/null %s/* > FBC.work' % (g_sender)
    os.system(cmd)
    file = open('FBC.work','r')
    for line in file:
        work=line.split('.')
        sender = work[0]
        action = work[1]
        print 'Sender= ' + sender + 'Action=' + action
        if 'benny' in action:
            print 'benny'
        if 'adam' in action:
            print 'adam'
    file.close()
    cmd = 'rm -r -R 2>/dev/null %s/*' % (g_sender)
    os.system(cmd)
    time.sleep(g_delay)
# End of file