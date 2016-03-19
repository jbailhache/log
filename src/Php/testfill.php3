Remplissage de la base

<?php

 include "connex.php3";

 function query ($q)
 {
  mysql_query ($q);
 }
 
 if (connexion() > 0)
 {
 query ("INSERT INTO afbut VALUES ('amitié');");
 query ("INSERT INTO afbut VALUES ('amour');");
 query ("INSERT INTO afbut VALUES ('flirt');");
 query ("INSERT INTO afbut VALUES ('loisirs');");
 query ("INSERT INTO afbut VALUES ('sport');");
 query ("INSERT INTO afbut VALUES ('selon affinité');");
 }
?>

