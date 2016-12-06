<?php

$x_from = $_GET['x_from']; // Client Id
$x_to = $_GET['x_to']; // Client Id
$x_tid = $_GET['x_tid']; // Transaction Id

if ($x_tid > 0)
{
  $mode = 1;  // Response from xsim client
  $x_response = $_GET['x_res'];
  echo("<br>From:$x_from To:$x_to Message:$x_response");
  saveResponse($x_from,$x_to,$x_response);
}
else
{
  $mode = 2;  // Request for new transaction
  $order = sendNextTransaction($x_from);
  if ($sender > 0)
    echo("<br>From:$order[oid] To:$x_from Order:$order[txt]");
  else 
    echo("<br>Client polling $x_from");
}








?>
