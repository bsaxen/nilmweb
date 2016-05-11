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
                if($pos !== null)
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
            fwrite($handle, "RCFS_SERVER 78.67.160.17");
            fwrite($handle, "RCFS_PATH /sxndata/index.php");
            fwrite($handle, "RCFS_DELAY 10");
            fwrite($handle, "RCFS_NAME NoName_$sid");
            fwrite($handle, "RCFS_DIR_PHOTO folke@nilm.se:/var/www/html/sxn_photo");
        }
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
}

?>
