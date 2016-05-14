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
g_sercon    = '78.67.160.17'
g_debug     = 'NO';
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
	global g_server
	global g_path
	global g_delay
	global g_name
	global g_dir_photo
	req = "/config.php?config=%s&sid=%d" % (par,sid)     
	count = 0;

	while (count < 5):
		count = count + 1
		conn = httplib.HTTPConnection(g_sercon)
		try:
			if g_debug == 'YES':
                		print req
			conn.request("GET", req)
			try:
				r1 = conn.getresponse()
				if g_debug == 'YES':
                			print ("Server Response:-_- %s %s " % (r1.status, r1.reason))
                		if r1.status == 200:
                			count = 9
            				data1 = r1.read()
            				word = data1.split()
					print word[0]
                    			if "RCFS_SERVER" in word[1]:
                    				g_server = word[2]
                    			if "RCFS_PATH" in word[1]:
                    				g_path = words[2]
                    			if "RCFS_DELAY" in word[1]:
                    				g_delay = words[2]		
					if "RCFS_NAME" in word[1]:
                    				g_name = words[2]	
					if "RCFS_DIR_PHOTO" in word[1]:
                    				g_dir_photo = words[2]	
            				if g_debug == 'YES':
                				print data1
        		except:
            			print '-_- No response from config server'
    		except:
        		print '-_- Not able to connect to config server ' + g_sercon
    		conn.close()
	
		if g_debug == 'YES':
                	print count
			print g_server
		return(count)

#---------------------------------------------------
def getConfiguration(sid):
#---------------------------------------------------
	res = 0
	res = res + RCFS(sid,"RCFS_SERVER")
	res = res + RCFS(sid,"RCFS_PATH")
	res = res + RCFS(sid,"RCFS_DELAY")
	res = res + RCFS(sid,"RCFS_NAME")
	res = res + RCFS(sid,"RCFS_DIR_PHOTO")
	return(res)
#---------------------------------------------------
def getLocalIpAddress():
#---------------------------------------------------
	global g_ipaddress
	os.system("ifconfig > ipaddress.work")
	file = open('ipaddress.work','r')
	for line in file:
    		if 'Bcast' in line:
        		words=line.split(' ')
        		work=words[11].split(':')
        		g_ipaddress = work[1] 
        		print 'local ipaddress ' + g_ipaddress
     
	return

res = getConfiguration(g_sid)
getLocalIpAddress()
g_req = g_path+ "?appid=%d&mid=%d&name=%s&ip=%s&nsid=1&sid1=%d" % (g_swid,g_mid,g_name,g_ipaddress,g_sid)  
if g_debug == 'YES':
	print g_req
	print g_server
while 1:
	conn = httplib.HTTPConnection(g_server)
	try:
		conn.request("GET", g_req)
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
				scp_photo     = "scp %s %s" % (photo_name,g_dir_photo)
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
