#!/usr/bin/python
#==================================================
# SW-21-switch.py
# 2016-07-10 
#==================================================
import time
import datetime
import RPi.GPIO as GPIO
import httplib
import os

GPIO.setmode(GPIO.BOARD)
GPIO.setup(12, GPIO.OUT) # Write file led
GPIO.setup(16, GPIO.IN, pull_up_down=GPIO.PUD_UP)
GPIO.setup(18, GPIO.OUT) # HTTP req led
GPIO-setup(22, GPIO.OUT)
#-------------------
#	3.3		5.0	-------->to 3.3 v
#	-		-	
#	-		GND	-------->to ground
#	7		8
#	-		10
#	17		12	-------->to LED (write to file)
#	-		-
#	-		16	-------->to signal
#	-		18	-------->to LED (HTTP request)
#	-		20
#	-		22	-------->to LED (status switch)
#-------------------
t1 = 0
t2 = 0
dt = 0
p1 = 0
p2 = 0
dp = 0
#==================================================
# Configuration
#==================================================
sid        = 308
boti       = 100
ip_server  = 'data.nabton.com'
log_local  = 1
log_server = 1
meas_name  = 'your_switch'
#==================================================
GPIO.output(12,False)
GPIO.output(18,False)
GPIO.output(22,False)
os.system("ifconfig > ipaddress.work")
file = open('ipaddress.work','r')
for line in file:
    if 'Bcast' in line:
        words=line.split(' ')
        work=words[11].split(':')
        sxn_ipaddress = work[1] 
        print 'local ipaddress ' + sxn_ipaddress
#==================================================
def doorOpen(x):
	global t1,t2,dt,log_file,nb_server,p1,p2,dp
	t2 = t1
	t1 = time.time()
	dt = t1 - t2
	if GPIO.input(16):
		switch_status = 1 # Closed
	else:
		switch_status = 0 # Open
		
	if log_server == 1:
		GPIO.output(18,True)
		p1 = time.time()
		conn = httplib.HTTPConnection(ip_server)
		t_req = "/index.php?appid=21&mid=1&name=%s&ip=%s&nsid=1&sid1=%d&dat1=%d" % (meas_name,sxn_ipaddress,sid,switch_status)
		try:
			conn.request("GET", t_req)
			try:
				r1 = conn.getresponse()
				if r1.status == 200:
					GPIO.output(18,False)
			except:
				print "exeption GET"
		except:
			print "exception HTTP"
		conn.close()
		p2 = time.time()
		dp = p2 - p1
		
	if log_local == 1:
		GPIO.output(12,True)
		with open(log_file, 'a') as out_file:
			ts = datetime.datetime.fromtimestamp(t1).strftime('%H:%M:%S')
			out_file.write("%s door_status: %d http: %.3f\n" % (ts,switch_status,dp))
			out_file.close()
			GPIO.output(12,False)

#==================================================
# Interrupt
GPIO.add_event_detect(16, GPIO.BOTH, callback=doorOpen,bouncetime=boti)
#==================================================
while True:
	try:
		today = datetime.date.today()
		log_file = "data-%d-%s.nbc" % (sid,today)
		time.sleep(1)
		if(switch_status == 1):
			GPIO.output(22,True) # closed
		else:
			GPIO.output(22,False) # open
	except KeyboardInterrupt:
		GPIO.cleanup()
		quit()
#==================================================		
GPIO.cleanup()
