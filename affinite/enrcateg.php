<HTML>
<head>
<title>Enregistrement d'une catégorie</title>
</head>
<body>
<?php
 include ("util.php");
 if (connexion() > 0)
 {
  $query = "INSERT INTO afcateg (categorie, position) VALUES ('$categorie', 1000)";
  $r = mysql_query ($query);
  if (!$r)
   echo ("Requête $query incorrecte");
  else
   echo ("Question $categorie créée.");
 }
 echo ("<p>Créez les réponses possibles à cette question en cliquant sur \"autre\" dans le formulaire.");
 echo ("<p><a href=affinite.php?action=$action>Retour</a>");
?>
<p>
</body>
</HTML>

