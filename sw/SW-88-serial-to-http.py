#!/usr/bin/python
#==================================================
# SW-88-serial-to-http.py
# 2016-07-12
#==================================================
# apt-get install python-pip
# pip install pyserial
import serial
import httplib
import os

#==================================================
# Read configuration
#==================================================
print 'Version 2016-07-12'
sxn_debug = 'NO';
sxn_server = 'data.nabton.com'
#sxn_sercon = 'config.nabton.com'
#sxn_server = '127.0.0.1'
#sxn_path = '/sxndata/index.php'
sxn_device = 'ttyACM0'
sxn_ipaddress = 'x.x.x.x'
#sxn_name = 'serial-to-http'
#==================================================
nb = 2
dev_found = 0
os.system("ls /dev | grep ACM > arduino_device.work")
file = open('arduino_device.work','r')
for line in file:
    words=line.split()
    #print words
    if words[0]:
        sxn_device = words[0]
        print 'Arduino Device found ' + sxn_device
        dev_found = 1
        
if dev_found == 0:        
    os.system("ls /dev | grep USB > arduino_device.work")
    file = open('arduino_device.work','r')
    for line in file:
        words=line.split()
        #print words
        if words[0]:
            sxn_device = words[0]
            print 'Arduino  Device found ' + sxn_device
            dev_found = 1
            
if dev_found == 1:
    sxn_device = '/dev/'+sxn_device
else:
    print 'No device found'
    exit()
    
            
os.system("ifconfig > ipaddress.work")
file = open('ipaddress.work','r')
for line in file:
    if 'Bcast' in line:
        words=line.split(' ')
        work=words[11].split(':')
        sxn_ipaddress = work[1] 
        print 'local ipaddress ' + sxn_ipaddress
        
ser = serial.Serial(sxn_device, 9600, timeout=None)
t_req = ser.readline().decode('utf-8')[:-2]
#time.sleep(5)    
while 1:
    t_req = ser.readline().decode('utf-8')[:-2]
    if t_req:
        if sxn_debug == 'YES':
            print t_req
        if '?' in t_req:   
            url=t_req.split(":")
            if sxn_debug == 'YES':
                print 'x'+url[0]+'xz'+url[1]+'z'
            if url[0][0] == '?' and url[1][0] == ' ':
                slen = len(url[0])
                if sxn_debug == 'YES':
                    print url[1]
                cs = eval(url[1])
                if sxn_debug == 'YES':
                    print ("len=%d cs=%d" % (slen, cs))
                if url[0][0] == '?' and cs == slen:
                    if nb == 2:
                        req = url[0] + '&ip=' +sxn_ipaddress
                        if sxn_debug == 'YES':
                            print 'y'+req+'y'
                        conn = httplib.HTTPConnection(sxn_server)
                        try:
                            conn.request("GET", req)
                            try:
                                r1 = conn.getresponse()
                                if sxn_debug == 'YES':
                                    print ("SXN-_- %s %s " % (r1.status, r1.reason))
                                data1 = r1.read()
                                if sxn_debug == 'YES':
                                    print data1
                                ser.write(data1+'\r\n')
                            except:
                                print '-_- No response from nb server'
                        except:
                            print '-_- Not able to connect to nb server'
                        conn.close()
                    else:
                        print 'nb=2 No NB Server configured'
                else:
                    print 'No match msg length'
            else:
                print "Out of sync"
        else:
            print "? and :"
    else:
        print "ser read fail"
ser.close()
# End of file
