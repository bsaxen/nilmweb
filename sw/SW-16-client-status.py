#!/usr/bin/python
#==================================================
# SW-16-client-status.py
# 2016-05-03
#==================================================
import time
import httplib
import os
#==================================================
# Read configuration
#==================================================
g_sid       = 901
g_mid       = 6
g_swid      = 16
g_debug     = 'YES';
g_server    = '78.67.160.17'
g_path      = '/sxndata/index.php'
g_ipaddress = 'xx.xx.xx.xx'
g_delay     = 10
g_name      = 'SW-16-client-status'
#==================================================
os.system("ifconfig > ipaddress.work")
file = open('ipaddress.work','r')
for line in file:
    if 'Bcast' in line:
        words=line.split(' ')
        work=words[11].split(':')
        g_ipaddress = work[1] 
        print 'local ipaddress ' + g_ipaddress

req = g_path+ "?appid=%d&mid=%d&name=%s&ip=%s&nsid=1&sid1=%d" % (g_swid,g_mid,g_name,g_ipaddress,g_sid)     
while 1:
	conn = httplib.HTTPConnection(g_server)
	try:
		conn.request("GET", req)
		try:
			r1 = conn.getresponse()
			if g_debug == 'YES':
        			print ("Server Response:-_- %s %s " % (r1.status, r1.reason))
      			data1 = r1.read()
      			if "STATUS" in data1:
        			if g_debug == 'YES':
         				print data1
        			print 'blink leds....'
    		except:
      			print '-_- No response from server'
  	except:
    		print '-_- Not able to connect to server '+g_server
  	conn.close()
  	time.sleep(g_delay)
# End of file
