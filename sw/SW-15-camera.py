#!/usr/bin/python
#==================================================
# SW-15-camera.py
# 2016-05-11
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
# Hard configuration
#==================================================
g_sid       = 901
g_mid       = 3
g_swid      = 15
g_sercon    = 'config.nilm.se'
g_debug     = 'YES';
#==================================================
# Soft configuration
#==================================================
g_server    = 'x.x.x.x'
g_path      = '/sxndata/index.php'
g_delay     = 10
g_name      = 'SW-15-camera'
g_dir_photo = 'folke@nilm.se:/var/www/html/sxn_photo/.'
#==================================================
# Info configuration
#==================================================
g_ipaddress = 'xx.xx.xx.xx'

#---------------------------------------------------
def RCFS(sid,par):
#---------------------------------------------------
req = "config.php?config=%s&sid=%d" % (par,sid)     
count = 0;

while (count < 5):
	count = count + 1
	conn = httplib.HTTPConnection(g_sercon)
	try:
		conn.request("GET", req)
		try:
			r1 = conn.getresponse()
			if g_debug == 'YES':
                		print ("Server Response:-_- %s %s " % (r1.status, r1.reason))
                	if r1.status == 200:
                		count = 9
            			data1 = r1.read()
            			word = data1.split()
                    		if "RCFS_SERVER" in word[0]:
                    			global g_server = word[1]
                    		if "RCFS_PATH" in word[0]:
                    			global g_path = words[1]
                    		if "RCFS_DELAY" in word[0]:
                    			global g_delay = words[1]		
				if "RCFS_NAME" in word[0]:
                    			global g_name = words[1]	
				if "RCFS_DIR_PHOTO" in word[0]:
                    			global g_dir_photo = words[1]	
            			if g_debug == 'YES':
                			print data1
        	except:
            		print '-_- No response from config server'
    	except:
        	print '-_- Not able to connect to config server ' + g_sercon
    	conn.close()
	

	return(count)

#---------------------------------------------------
def getConfiguration(sid):
#---------------------------------------------------
	res = 0
	res = res + RCFS(sid,"RCFS_SERVER")
	res = res + RCFS(sid,"RCFS_PATH")
	res = res + RCFS(sid,"RCFS_DELAY")
	res = res + RCFS(sid,"RCFS_NAME")
	res = res + RFCS(sid,"RCFS_DIR_PHOTO")
	return(res)
#---------------------------------------------------
def getLocalIpAddress():
#---------------------------------------------------
	os.system("ifconfig > ipaddress.work")
	file = open('ipaddress.work','r')
	for line in file:
    		if 'Bcast' in line:
        		words=line.split(' ')
        		work=words[11].split(':')
        		global g_ipaddress = work[1] 
        		print 'local ipaddress ' + g_ipaddress
     
	return

res = getConfiguration(g_sid)
getLocalIpAddress()
g_req = g_path+ "?appid=%d&mid=%d&name=%s&ip=%s&nsid=1&sid1=%d" % (g_swid,g_mid,g_name,g_ipaddress,g_sid)  

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
			if "FF_RECONFIG" in data1:
				getConfiguration(g_sid)
				g_req = g_path+ "?appid=%d&mid=%d&name=%s&ip=%s&nsid=1&sid1=%d" % (g_swid,g_mid,g_name,g_ipaddress,g_sid)
			if "FF_LOCAL_IP_UPDATE" in data1:
				getLocalIpAddress()
				g_req = g_path+ "?appid=%d&mid=%d&name=%s&ip=%s&nsid=1&sid1=%d" % (g_swid,g_mid,g_name,g_ipaddress,g_sid)
            		if g_debug == 'YES':
                		print data1
        	except:
            		print '-_- No response from server'
    	except:
        	print '-_- Not able to connect to server '+g_server
    	conn.close()
    	time.sleep(g_delay)
# End of file
