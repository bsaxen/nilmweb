#!/usr/bin/python
#==================================================
# xsim_client.py
# 2016-12-06
# Benny Saxen
#==================================================
import time
import httplib
import os
g_server    = 'x.x.x.x'
g_path      = '/sxndata/index.php'
g_delay     = 10

while 1:
	conn = httplib.HTTPConnection(g_server)
	try:
		conn.request("GET", g_req)
		try:
			r1 = conn.getresponse()
			if g_debug == 'YES':
          print ("Server Response:-_- %s %s " % (r1.status, r1.reason))
        data1 = r1.read()
        os.system(data1);
    except:
        print '-_- No response from server'
  except:
      print '-_- Not able to connect to server '+g_server
  conn.close()
  time.sleep(g_delay)
# End of file
