#===========================================
#!/usr/bin/python
#==================================================
# FCB-master.py
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
g_sid       = 901
g_user      = 'folke'
g_debug     = 'YES';
g_server    = 'nilm.se'
g_path      = '/FBC'
g_delay     = 10
g_name      = 'FBC-master'
g_scp       = 'folke@nilm.se:/home/folke/FBC/'
#==================================================
local_ipaddress = 'x.x.x.x'
os.system("ifconfig > ipaddress.work")
file = open('ipaddress.work','r')
for line in file:
    if 'Bcast' in line:
        words=line.split(' ')
        work=words[11].split(':')
        g_ipaddress = work[1] 
        print 'local ipaddress ' + local_ipaddress
file.close()
r_scp = '%s@%s:/home/%s/%s' % (g_user,g_server,g_user,g_path)
if g_debug:
  print r_scp
while 1:
    # read local mailbox
    os.system("ls > FBC.work")
    file = open('FBC.work','r')
    for line in file:
        if 'FBC' in line:
            words=line.split('-')
            work=words[2].split('.')
            sender = work[1]
            action = work[2]
            print 'Sender= ' + sender + 'Action=' + action
            if action == 'clear':
                cmd = 'rm %s/%d/msg/*.msg' % (g_path,sender)
                os.system(cmd)
            if action == 'register'
                cmd = 'mkdir %s/%d/msg' % (g_path,sender)
                os.system(cmd)
    file.close()
    # FBC-<id>.register - register and create slave directory FBC/<id>/msg
    # FBC-<id>.clear - clear mailbox 
    time.sleep(g_delay)
# End of file

