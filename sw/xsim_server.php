<?php
// houseRpi-server.response
// server-houseRpi.order

$x_from = $_GET['from']; // From client Id
$x_to   = $_GET['to']; // To client Id
$x_msg  = $_GET['msg']; // Transaction Message type: 1 = any orders for me?, 2 = answer on latest order

function saveAnswer($from,$to,$answer)
{
  $file = $from.'-'.$to.'.response';
  $fh = fopen($file, 'w') or die("saveAnswer can't open file: $file");
  fwrite($fh, $answer);    
  //fwrite($fh, "\n");
  fclose($fh);
}
function getNextTransaction($id)
{
  $file = $id.'.order';
  $fh = fopen($file, 'r') or die("getNextTransaction can't open file: $file");
  $row = fgets($fh);
  echo("$row");                       
  fclose($fh);
  unlink($file);
}

if ($x_msg == 2)
{
  $x_response = $_GET['x_answer'];
  //echo("<br>From:$x_from To:$x_to Answer:$x_answer");
  saveAnswer($x_from,$x_to,$x_answer);
}
else if ($x_msg == 1)
{
  $order = getNextTransaction($x_from);
  //if ($sender > 0)
  //  echo("<br>From:$order[oid] To:$x_from Order:$order[txt]");
  //else 
  //  echo("<br>Client polling $x_from");
}
else
  //echo("Unknown message type");

?>
