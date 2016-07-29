<html>
<head>
  <title>Jupiter</title>
  <link REL="StyleSheet" TYPE="text/css" HREF="nilm.css">
  <script src='tinymce/js/tinymce/tinymce.min.js'></script>
  <script>
   tinymce.init({
	selector: '#mytextarea',
	width:600, height:300});
  </script>
</head>

<?php

$order = $_GET[order];


//system("ps -ef | grep bsaxen > work.txt");
system("pwd");
system("rm -f work.txt");
//==================================================
// rpi-index.php
// 2016-07-29
//==================================================
//=======================================
function lib_listFileContent($filename)
//=======================================
{

//echo("<br>BEGIN *********<br>");
    $handle = fopen($filename, "r");
    if ($handle)
    {
        while (($line = fgets($handle)) !== false)
        {
                $pos = strstr($line,"SW");
//echo("$line x $pos x<br>");
		if($pos)
		{
			$temp = trim($line);
			echo("<a href=\"index.php?order=$temp\">$line</a><br>");
		}
                else
                        echo("$line<br>");
        }
    }
    else
        echo("Error lib_listFileContent($filename)");
   fclose($handle);
//echo("<br>END *********<br>");
}
//=======================================
// Main
//=======================================

echo("<body>");
echo("<h1>Benny Raspberry Pi: Jupiter</h1>");
echo("<li><a href=\"index.php?order=processes\">Processes</a>");
echo("<li><a href=\"index.php?order=arduino\">Arduino</a>");
echo("<li><a href=\"index.php?order=sw\">SW-applications</a>");
if($order == 'arduino')
{
 system("cp /home/pi/beep/src/sketch.ino work.txt");
}
if($order == 'processes')
{
 system("ps -ef | grep SW | grep root > work.txt");
}
if($order == 'sw')
{
 system("ls /home/pi | grep SW > work.txt");
}


if($order)
{

echo(" 
	<form method=\"post\">
      	<textarea id=\"mytextarea\">");
lib_listFileContent("work.txt");
echo("
	</textarea>
    	</form>
");
}
echo("</body></html>");
