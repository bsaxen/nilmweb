<?php
//=======================================
function getConfigValue($sid,$par)
//======================================= 
{
  $filename = $sid.'.conf';
    $handle = fopen($filename, "r");
    if ($handle) 
    {    
        while (($line = fgets($handle)) !== false) 
        {
          $line = trim($line);
          
        }
        fclose($handle);
    } 
}

//==============================================================================
// Main program
//==============================================================================


if(isset($_GET['sid']) && isset($_GET['config']))
{

$sid    = $_GET['sid'];
$config = $_GET['config'];

}

?>
