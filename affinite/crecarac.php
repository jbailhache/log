<HTML>
<head>
<title>Création d'un caractère</title>
</head>
<form method=post action=enrcarac.php>
<?php
 include ("util.php");
 echo ("<input type=hidden name=action value=$action>");
 if (connexion() > 0)
 {
  /* echo ("Ajouter une réponse à la question $categorie : <p>"); */
  echo ("Autre $categorie : <p>");
  echo ("<input type=hidden name=categorie value=\"$categorie\">");
  echo ("<input type=text name=caractere>");
  echo ("<p><input type=submit value=Valider>");
 }
?>
</form>
</body>
</HTML>

