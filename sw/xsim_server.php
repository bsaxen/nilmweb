<?php

$x_cid = $_GET['x_cid']; // Client Id
$x_tid = $_GET['x_tid']; // Transaction Id

if ($x_tid > 0)
{
  $mode = 1;  // Response from xsim client
  $x_response = $_GET['x_res'];
  saveResponse($x_cid,$x_response);
}
else
{
  $mode = 2;  // Request for new transaction
  $res = sendNextTransaction($x_cid);
  if ($res == 1)
    echo("<br>new transaction to client $x_cid");
  else 
    echo("<br>Client polling $x_cid");
}








?>
