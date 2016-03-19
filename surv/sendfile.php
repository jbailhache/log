<br><br><br><br><br><br><br><br><br><br><br><br><br><br><br>
DEBUT DES DONNEES

<?php
 include ("connexion.php");
 if (!connexion()) echo " ERREUR CONNEXION ";
 $req = "SELECT * FROM files WHERE name = '$name' ORDER BY block";
 $dat = mysql_query($req);
 while($r = mysql_fetch_object($dat))
 {
  echo $r->data;
  echo "<br>";
 }
?>
$

