<!DOCTYPE html PUBLIC "-//W3C//DTD HTML 4.01//EN">
<html>
<head>
  <title>NILM.SE</title>
  <link REL="StyleSheet" TYPE="text/css" HREF="nilm.css">
</head>

<body>
    
<?php
session_start(); 
$page  = $_SESSION['page'];
if(isset($_GET['page']))
{
    $page  = $_GET['page'];
}
$_SESSION['page'] = $page;
               
echo("<ul class=\"navbar\">");
    
if($page=="home")
    echo("<li>home");
else 
    echo("<li><a href=\"index.php?page=home\">home</a>");

if($page=="login")
    echo("<li>login");
else 
    echo("<li><a href=\"index.php?page=login\">login</a>");

if($page=="system")
    echo("<li>system");
else 
    echo("<li><a href=\"index.php?page=system\">system</a>");

if($page=="download")
    echo("<li>download");
else 
    echo("<li><a href=\"index.php?page=download\">download</a>");

    
if($page=="hardware")
    echo("<li>hardware");
else 
    echo("<li><a href=\"index.php?page=hardware\">hardware</a>");

if($page=="shop")
    echo("<li>shop");
else 
    echo("<li><a href=\"index.php?page=shop\">shop</a>");
    
if($page=="about")
    echo("<li>about");
else 
    echo("<li><a href=\"index.php?page=about\">about</a>");
    
if($page=="contact")
    echo("<li>contact");
else 
    echo("<li><a href=\"index.php?page=contact\">contact</a>");

  echo("<li><a href=\"http://forum.nilm.se\">forum</a>");
echo("</ul>");

echo("<h1>NILM.SE - $page</h1>");
//==============================================================
if($page == "home")
{
echo("   

<p>Welcome to build your own data collecting system.

<p>Download software for client and server (it is free).

<p>Buy the hardware yourself or buy a kit from us.

<p>Support and discussion on the forum.
");
}
//==============================================================
if($page == "login")
{
echo("login");
}
//==============================================================
if($page == "system")
{
echo("
<li>System Solution 1: <b>Platform:</b> Arduino + RaspberryPi <b>Communication:</b> Serial USB, Wifi, Ethernet
<li>System Solution 2: <b>Platform:</b> RaspberryPi <b>Communication:</b> Wifi, Ethernet
<li>System Solution 3: <b>Platform:</b> ESP8266 <b>Communication:</b> Wifi
<li>System Solution 4: <b>Platform:</b> Arduino with Ethernet Shield  <b>Communication:</b> Ethernet
");
}
//==============================================================
if($page == "download")
{
echo("
<p>Application SW<br>
<table border=1>
<tr><td>Implementation</td><td>Target</td><td>Source Code</td><td>System</td></tr>
<tr><td>.py</td><td>RaspberryPi 3</td><td><a href=\"sw/SW-10-nilm.py\" download=\"SW-10-nilm.py\">
  <input type=\"button\" value=\"RaspberryPi NILM Electric Power\" />
</a></td><td>-</td></tr>

<tr><td>.ino</td><td>ESP8266</td><td><a href=\"sw/SW-13-ESP8266-nilm.ino\" download=\"SW-13-ESP8266-nilm.ino\">
  <input type=\"button\" value=\"ESP8266 NILM Electric Power\" />
</a></td><td>-</td></tr>

<tr><td>.ino</td><td>Arduino</td><td><a href=\"sw/SW-12-temperature.ino\" download=\"SW-12-temperature.ino\">
  <input type=\"button\" value=\"Arduino Temperature\" />
</a></td><td>1</td></tr>
    
<tr><td>.ino</td><td>Arduino+EthShield</td><td><a href=\"sw/SW-14-ethShield-temperature.ino\" download=\"SW-14-ethShield-temperature.ino\">
  <input type=\"button\" value=\"Arduino EthernetShield Temperature\" />
</a></td><td>-</td></tr>

<tr><td>.ino</td><td>Arduino+EthShield</td><td><a href=\"sw/SW-11-stepper.ino\" download=\"SW-11-stepper.ino\">
  <input type=\"button\" value=\"Arduino Stepper Control\" />
</a></td><td>-</td></tr>
    
<tr><td>.ino</td><td>Raspberry Pi 2,3</td><td><a href=\"sw/SW-15-camera.py\" download=\"SW-15-camera.py\">
  <input type=\"button\" value=\"Camera\" />
</a></td><td>-</td></tr>

<tr><td>.py</td><td>Raspberry Pi 2,3</td><td><a href=\"sw/SW-98-beacon.py\" download=\"SW-98-beacon.py\">
  <input type=\"button\" value=\"Beacon\" />
</a></td><td>-</td></tr>

</table>
");
//----------------------------------------
echo("
<p>Generic SW<br>
<table border=1>
<tr><td>Implementation</td><td>Target</td><td>Source Code</td><td>System</td></tr>

<tr><td>.py</td><td>Raspberry Pi 2,3</td><td><a href=\"sw/serial-to-http.py\" download=\"serial-to-http.py\">
  <input type=\"button\" value=\"Serial to http\" />
</a></td><td>1</td></tr>
   
</table>
");
}
//==============================================================
if($page == "hardware")
{
echo("
<p>IR Detector for electric power meters</p>

<p>Arduino</p>

<p>RaspberryPi</p>

<p>ESP8266</p>
");
}
//==============================================================
if($page == "shop")
{
echo("Under construction");
}
//==============================================================
if($page == "about")
{
echo("lUnder construction");
}
//==============================================================
if($page == "contact")
{
echo("
<br>E-mail: contact@nilm.se<br>
    
<address>NILM.SE 2016</address>
");
}

echo("<address>NILM.SE 2016</address>");
?>
    
</body>
</html>
