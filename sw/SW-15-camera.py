#!/usr/bin/python
#==================================================
# SW-15-camera.py
# 2016-04-28
#==================================================
import time
import httplib
import os
#==================================================
# Read configuration
#==================================================
print 'Version 2016-04-28'
g_debug     = 'NO';
g_server    = 'xx.xx.xx.xx'
g_sercon    = 'config.nilm.se'
g_path      = '/sxndata/index.php'
g_ipaddress = 'xx.xx.xx.xx'
g_delay     = 10
g_name      = 'SW-15-camera'
#==================================================
os.system("ifconfig > ipaddress.work")
file = open('ipaddress.work','r')
for line in file:
    if 'Bcast' in line:
        words=line.split(' ')
        work=words[11].split(':')
        g_ipaddress = work[1] 
        print 'local ipaddress ' + g_ipaddress

req = g_path+ '?mid=5'+'&appid=15'+'&ip=' + g_ipaddress + '&name=' + g_name  
     
while 1:
	conn = httplib.HTTPConnection(g_server)
	try:
		conn.request("GET", req)
		try:
			r1 = conn.getresponse()
			if g_debug == 'YES':
                		print ("Server Response:-_- %s %s " % (r1.status, r1.reason))
            		data1 = r1.read()
            		# Check if FF_PICTURE
			os.system("raspistill -o myimage.jpg -t 3000pi");
            		if g_debug == 'YES':
                		print data1
        	except:
            		print '-_- No response from nb server'
    	except:
        	print '-_- Not able to connect to server '+g_server
    	conn.close()
    	time.sleep(g_delay)
# End of file

