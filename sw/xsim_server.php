<?php
// houseRpi-server.response
// server-houseRpi.order

$x_from = $_GET['x_from']; // Client Id
$x_to   = $_GET['x_to']; // Client Id
$x_tid  = $_GET['x_tid']; // Transaction Id

function saveResponse($tid,$from,$to,$response)
{
  $file = $tid.'-'.$from.'-'.$to.'.response';
  $fh = fopen($file, 'w') or die("saveResponse can't open file: $file");
  fwrite($fh, $response);    
  //fwrite($fh, "\n");
  fclose($fh);
}
function getNextTransaction($from)
{
  system("ls $
  $fh = fopen($file, 'r') or die("getNextTransaction can't open file: $file");
  $row = fgets($fh);
  echo("$row");                       
  //sscanf($row, "%d", $id);
  fclose($fh);
}

if ($x_tid > 0)
{
  $mode = 1;  // Response from xsim client
  $x_response = $_GET['x_res'];
  echo("<br>From:$x_from To:$x_to Message:$x_response");
  saveResponse($_tid,$x_from,$x_to,$x_response);
}
else
{
  $mode = 2;  // Request for new transaction
  $order = getNextTransaction($x_from);
  if ($sender > 0)
    echo("<br>From:$order[oid] To:$x_from Order:$order[txt]");
  else 
    echo("<br>Client polling $x_from");
}








?>
