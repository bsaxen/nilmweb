<?php
//=======================================
function logData($sid,$data)
//=======================================
{
   $yymmdd = date('Y-m-d');
   $hhmmss = date('H:i:s'); 
   $filename = $sid.'-'.$yymmdd.'.elerpi';
   $item = $hhmmss.' '.$data." \n";

   if (file_exists($filename))
   {
      $fh = fopen($filename, "a");
      fwrite($fh, $item);
      fclose($fh);
   }
   else
   {
      $fh = fopen($filename, "w");
      fwrite($fh, $item);
      fclose($fh);
      chmod($filename,0777);
   }              
}

if(isset($_GET['sid']))
{
    $sid    = $_GET["sid"];
    $data   = $_GET["data"];
    logData($sid,$data);
}

?>

