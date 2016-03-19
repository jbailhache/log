<HTML>
<head>
<title>Enregistrement d'un caractère</title>
</head>
<body>
<?php
 include ("util.php");
 if (connexion() > 0)
 {
  $query = "INSERT INTO afcarac (categorie, caractere) VALUES ('$categorie', '$caractere')";
  $r = mysql_query ($query);
  if (!$r)
   echo ("Requête $query invalide");
  else
   echo ("$categorie $caractere enregistré.");
 }
 echo ("<p><a href=affinite.php?action=$action>Retour</a>");
?>
<p>
 
</body>
</html>
