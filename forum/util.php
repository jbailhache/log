<?php

function trace ($s)
{
 // echo "<p>trace: $s ";
}

$tt = array (
 'à' => '&agrave;', 
 'â' => '&acirc;',
 'ä' => '&auml;',
 'ç' => '&ccedil;',
 'é' => '&eacute;',
 'è' => '&egrave;',
 'ë' => '&euml;',
 'ê' => '&ecirc;',
 'î' => '&icirc;',
 'ï' => '&iuml;',
 'ô' => '&ocirc;',
 'ö' => '&ouml;',
 'ù' => '&ugrave;',
 'û' => '&ucirc;',
 'ü' => '&uuml;'
  );
  
function format1 ($s)
{
 global $tt;
 $r = '';
 $n = strlen($s);
 for ($i=0; $i<$n; $i++)
 {
  $c = substr($s,$i,1);
  // echo " c=$c->" . $tt[$c];
  if (isset($tt[$c]))
  {
   // echo "O";
   $r = $r . $tt[$c];
   // echo " r=$r ";
  }
  else
  {
   // echo "N";
   $r = $r . $c;
   // echo " r=$r ";
  }
 }
 return $r;
}

function format ($s)
{
 trace("s=$s");
 //$s1 = addslashes (display($s));
 if (!get_magic_quotes_gpc())
 {
  $s = addslashes($s);
 }
 trace("s=$s");
 $s1 = display($s);
 $s1 = str_replace('\\\'','\'\'',$s1);
 $s1 = str_replace('\\"','"',$s1);
 $s1 = str_replace('\\\\','\\',$s1);
 trace("s1=$s1");
 $s2 = format1($s1);
 trace("s1=$s1,s2=$s2");
 return $s2;
}

function format_query ($s)
{
 $s1 = str_replace('\'', '\'\'', $s);
 return $s1;
}

function display ($s)
{
 return str_replace("\n", "<br>", str_replace (">", "&gt;", str_replace ("<", "&lt;", $s)));
}

function stripslashes_if_mq ($s)
{
 if (get_magic_quotes_gpc())
 {
  return stripslashes($s);
 }
 else
 {
  return $s;
 }
}

function decode ($s)
{
 return html_entity_decode($s);
}

function format_date ($dt)
{
 return $dt;
 //list($date, $time) = explode(" ", $dt);
 //list($year, $month, $day) = explode("-", $date);
 //return "$day/$month/$year $time";
}

function num ($s)
{
	if (strval(intval($s)) == $s)
		return $s;
	else
	{
		echo "<h2><font face=darkred>ALERTE !!! TENTATIVE DE PIRATAGE DU SITE !!!</h2><p>C'est pas bien &ccedil;a...</p>";
		exit();
	}
}

function affichable ($rec)
{
 if ($_SESSION['moderateur'] == 'oui')
 {
  return 1;
 }
 $statut = $rec->statut;
 return ($statut == 0 || $statut == 4 || $statut == 5);
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

?>
