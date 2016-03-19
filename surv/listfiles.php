<br><br><br><br><br><br><br><br><br><br><br><br><br><br><br>
<?php
 include ("connexion.php");
 if (!connexion()) echo " ERREUR CONNEXION ";
 $req = "SELECT * FROM files ORDER BY name, block";
 $dat = mysql_query($req);
 echo "<table border=1>";
 while ($r = mysql_fetch_object($dat))
 {
  echo "<tr><td>";
  echo $r->name;
  echo "</td><td>";
  echo $r->block;
  echo "</td><td>";
  echo $r->data;
  echo "</td></tr>";
 }
 echo "</table>";

?>