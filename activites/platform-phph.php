<?php

 $platform = "phph";

 function init ($vars)
 {
  foreach ($vars as $var)
  {
   $instr = '$GLOBALS[\'' . $var . '\'] = $_POST[\'' .  $var . '\'];';
   echo "<li>eval [" . $instr . "]";
   eval ($instr);
  }
 }

 function connexion ()
 {
  global $db;
  $db = sqlite_open ('mysqlitedb', 0666);
  if ($db)
   return 1;
  else
   return 0;
 }

 function query ($q)
 {
  global $db;
  return sqlite_query ($db, $q);
 }

 function fetch_object ($data) 
 {
  return sqlite_fetch_array ($data);
 }

 function error ()
 {
  return 'ERROR';
 }

?>
