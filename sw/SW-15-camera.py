#!/usr/bin/python
#==================================================
# SW-15-camera.py
# 2016-04-29
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
g_mid       = 3
g_swid      = 15
g_debug     = 'YES';
g_server    = 'x.x.x.x'
g_sercon    = 'config.nilm.se'
g_path      = '/sxndata/index.php'
g_ipaddress = 'xx.xx.xx.xx'
g_delay     = 10
g_name      = 'SW-15-camera'
g_scp       = 'folke@nilm.se:/var/www/html/sxn_photo/.'
#---------------------------------------------------
def RCFS(sid,par):
#---------------------------------------------------
req = "?config=%s&sid=%d" % (par,sid)     
while config == FALSE:
	conn = httplib.HTTPConnection(g_sercon)
	try:
		conn.request("GET", req)
		try:
			r1 = conn.getresponse()
			if g_debug == 'YES':
                		print ("Server Response:-_- %s %s " % (r1.status, r1.reason))
            		data1 = r1.read()
            		# Check if FF_PHOTO
                    	if "RCFS_SERVER" in data1:
                    		
				
				
				
                    		
				
            		if g_debug == 'YES':
                		print data1
        	except:
            		print '-_- No response from config server'
    	except:
        	print '-_- Not able to connect to config server ' + g_sercon
    	conn.close()
	
	
	return;
#==================================================
# RCFS - Read configuration from server
#==================================================
# g_server = RCFS(g_sid,"SERVER")
# g_path   = RCFS(g_sid,"PATH")
# g_delay  = RCFS(g_sid,"DELAY")
# g_name   = RCFS(g_sid,"NAME")
# g_scp    = RFCS(g_sid,"DIR_PHOTO")
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
            		# Check if FF_PHOTO
                    	if "FF_PHOTO" in data1:
                    		now = time.strftime("%Y-%m-%d-%H-%M-%S")
				photo_name    = "SW-15-sid%d-%s.jpg" % (g_sid,now)
				take_photo    = "raspistill -o %s -t 1000pi" % (photo_name)
				scp_photo     = "scp %s %s" % (photo_name,g_scp)
                    		os.system(take_photo);
				os.system(scp_photo);
            		if g_debug == 'YES':
                		print data1
        	except:
            		print '-_- No response from server'
    	except:
        	print '-_- Not able to connect to server '+g_server
    	conn.close()
    	time.sleep(g_delay)
# End of file
