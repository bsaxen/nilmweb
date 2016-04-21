#!/usr/bin/python
#==================================================
# SW-98-beacon.py
# 2016-04-21
#==================================================
# apt-get install python-pip
import httplib
import os

#==================================================
# Read configuration
#==================================================
print 'Version 2016-04-21'
sxn_debug = 'YES';
sxn_server = '78.67.160.17'
sxn_sercon = 'config.nabton.com'
#sxn_server = '127.0.0.1'
sxn_path = '/sxndata/index.php'
sxn_device = 'ttyACM0'
sxn_ipaddress = 'x.x.x.x'
sxn_name = 'serial-to-http'
#==================================================
os.system("ifconfig > ipaddress.work")
file = open('ipaddress.work','r')
for line in file:
    if 'Bcast' in line:
        words=line.split(' ')
        work=words[11].split(':')
        sxn_ipaddress = work[1] 
        print 'local ipaddress ' + sxn_ipaddress
        
while 1:
    conn = httplib.HTTPConnection(sxn_server)
    try:
        conn.request("GET", req)
        try:
            r1 = conn.getresponse()
            if sxn_debug == 'YES':
                print ("Server Response:-_- %s %s " % (r1.status, r1.reason))
            data1 = r1.read()
            if sxn_debug == 'YES':
                print data1
        except:
            print '-_- No response from nb server'
    except:
        print '-_- Not able to connect to nb server'
    conn.close()

# End of file

Enter file contents here
