<?php

function trace ($s)
{
	// echo "<p>TRACE: $s</p>";
}

function coalesce ($a,$e)
{
 foreach ($a as $x)
 {
  if (!empty($x))
  {
   return $x;
  }
 }
 return $e;
}

function ip ()
{
 return coalesce(array(
  $_SERVER["HTTP_X_REAL_IP"],
  $_SERVER["HTTP_X_FORWARDED_FOR"],
  $_SERVER["HTTP_CLIENT_IP"],
  $_SERVER["REMOTE_ADDR"]),'');
}

function format_query ($s)
{
 $s1 = str_replace('\'', '\'\'', $s);
 return $s1;
}

function format_input ($s)
{
 trace ("format_input($s)");
 if (get_magic_quotes_gpc())
 {
  trace ("magic quotes");
  $r = stripslashes($s);
  trace("return $r");
  return $r;
 }
 else
 {
  trace ("no magic quotes, return $s");
  return $s;
 }
}

function post ($champ)
{
	return format_input($_POST[$champ]);
}

function format_value ($s)
{
	//echo " format_value($s) ";
	$r = str_replace('"','&quot;',$s);
	//echo " return $r ";
	return $r;
}
