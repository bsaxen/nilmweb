#!/usr/bin/python
#==================================================
# SW-16-client-status.py
# 2016-05-12
# sudo apt-get install python-rpi.gpio python3-rpi.gpio
#==================================================
import time
import datetime
import RPi.GPIO as GPIO
import httplib
import os

GPIO.setmode(GPIO.BOARD)
GPIO.setup(12, GPIO.OUT) # Green Led
GPIO.setup(16, GPIO.OUT) # Yellow Led
GPIO.setup(18, GPIO.OUT) # Red led
#-------------------
#	3.3		5.0	--------> to 5.0 v
#	-		-	
#	-		GND	--------> to ground
#	7		8
#	-		10
#	17		12	--------> to LED (GREEN)
#	-		-
#	-		16	--------> to LED (YELLOW)
#	-		18	--------> to LED (RED)
#-------------------
led_green  = 12
led_red    = 18
led_yellow = 16
#==================================================
# Hard configuration
#==================================================
g_sid       = 901
g_mid       = 6
g_swid      = 16
g_sercon    = '78.67.160.17'
g_debug     = 'NO';
#==================================================
# Soft configuration
#==================================================
g_server    = 'x.x.x.x'
g_path      = '/sxndata/index.php'
g_delay     = 10
g_name      = 'SW-16-client-status'
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
        		g_ipaddress = work[1] 
        		print 'local ipaddress ' + g_ipaddress
     
	return

res = getConfiguration(g_sid)
getLocalIpAddress()
g_req = g_path+ "?appid=%d&mid=%d&name=%s&ip=%s&nsid=1&sid1=%d" % (g_swid,g_mid,g_name,g_ipaddress,g_sid)  




GPIO.output(led_green,False)
GPIO.output(led_red,False)
GPIO.output(led_yellow,False)
while 1:
	GPIO.output(led_yellow,True)
	conn = httplib.HTTPConnection(g_server)
	try:
		conn.request("GET", g_req)
		try:
			r1 = conn.getresponse()
			GPIO.output(led_yellow,False)
			time.sleep(0.5)
			if g_debug == 'YES':
        			print ("Server Response:-_- %s %s " % (r1.status, r1.reason))
      			data1 = r1.read()
      			words = data1.split()
      			ndata = len(words)
      			ndata = ndata - 1
      			if "CLIENT" in words[0] and "STATUS" in words[ndata]:
        			if g_debug == 'YES':
         				print data1
        			for num in range(1,ndata):
        				if '1' in words[num]:
        					print 'blink GREEN led'
        					GPIO.output(led_green,True)
	
        				if '2' in words[num]:
        					print 'blink RED led'
        					GPIO.output(led_red,True)
        				
        				if '3' in words[num]:
        					print 'blink YELLOW led'
        					GPIO.output(led_yellow,True)
        					
        				time.sleep(0.5)
        				GPIO.output(led_green,False)
        				GPIO.output(led_red,False)
        				GPIO.output(led_yellow,False)
        				time.sleep(0.5)
    		except:
      			print '-_- No response from server'
      		
  	except:
    		print '-_- Not able to connect to server '+g_server
  	conn.close()
  	time.sleep(g_delay)
  	GPIO.output(led_yellow,False)
  	time.sleep(0.5)
# End of file
