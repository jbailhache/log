<br><br><br><br><br><br><br><br><br><br><br><br><br><br><br>
<?php
 include ("connexion.php");
 if (!connexion()) echo " ERREUR CONNEXION ";
 if ($block == 1)
 {
  $req = "DELETE FROM files WHERE name = '$name'";
  mysql_query($req);
 }
 else
 {
  $req = "DELETE FROM files WHERE name = '$name' AND block = '$block'";
  mysql_query($req);
 }
 $req = "INSERT INTO files (name, block, time, data) values ('$name', $block, '$time', '$data')";
 echo $req;
 $status = mysql_query ($req);
 echo "<p>status = $status ";
?>
  	
