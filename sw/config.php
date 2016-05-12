<?php
//=======================================
function getConfigValue($sid,$par)
//======================================= 
{
    $value = 'void';
    $filename = $sid.'.conf';
    if (file_exists($filename)) 
    {

        $handle = fopen($filename, "r");
        if ($handle) 
        {    
            while (($line = fgets($handle)) !== false) 
            {
                $line = trim($line);
                $pos = strpos($line, $par);
                if($pos !== false)
                {
                    sscanf($line,"%s %s",$key,$value);
                }
            }
            fclose($handle);
        }
        return($value);
    }
    else
    {
        $handle = fopen($filename, "w");
        if ($handle) 
        {
            fwrite($handle, "RCFS_SERVER 78.67.160.17 \n");
            fwrite($handle, "RCFS_PATH /sxndata/index.php \n");
            fwrite($handle, "RCFS_DELAY 10 \n");
            fwrite($handle, "RCFS_NAME NoName_$sid \n");
            fwrite($handle, "RCFS_DIR_PHOTO folke@nilm.se:/var/www/html/sxn_photo \n");
            fclose($handle);
        }
    }
    
}
//=======================================
function listAllSids()
//======================================= 
{
    system("ls *.conf > sidList.work");   
}
//=======================================
function createSidConfig($sid)
//======================================= 
{
    $filename = $sid.'.conf';
    if (file_exists($filename)) 
    {
        echo("SID $sid configuration already exists");
    }
    else
    {
        $handle = fopen($filename, "w");
        if ($handle) 
        {
            fwrite($handle, "RCFS_SERVER 78.67.160.17 \n");
            fwrite($handle, "RCFS_PATH /sxndata/index.php \n");
            fwrite($handle, "RCFS_DELAY 10 \n");
            fwrite($handle, "RCFS_NAME NoName_$sid \n");
            fwrite($handle, "RCFS_DIR_PHOTO folke@nilm.se:/var/www/html/sxn_photo \n");
            fclose($handle);
        }
    }
}
//==============================================================================
// Main program
//==============================================================================


if(isset($_GET['sid']) && isset($_GET['config']))
{
    $sid    = $_GET['sid'];
    $config = $_GET['config'];
    $res = getConfigValue($sid,$config);
    echo("$sid $config $res");
    die;
}
else
{
    $action = $_POST['f_action'];
    if($action == 'createSidConfForm')
    {
        $sid = $_POST[f_sid];
        createSidConfig($sid);
    }
    
    echo("<h1>SID Configuration</h1>");
    echo("<table border=\"1\">
         <form action=\"config.php\" method=\"post\">
            <input name=\"f_action\" type=\"hidden\" value=\"createSidConfForm\" />
            
            <tr><td>Data Name:</td><td> <input name=\"f_sid\" type=\"text\" size=\"20\" value=\"901\"/></td></tr>
            
            <tr><td><input name=\"f_submit\" type=\"submit\" value=\"Create SID Configuration\" /></td><td></td></tr>
        </form></table>
  ");

    listAllSids();
    
}

?>
