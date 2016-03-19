Remplissage de la base

<?php

 include "connex.php3";

 function query ($q)
 {
  $r = mysql_query ($q);
  if (!$r)
   echo ("<pZError: $q.");
 }
 
 if (connexion() > 0)
 {
  include ("tbl_dump.php3"); 
 }
 echo ("<p>Done");
?>

