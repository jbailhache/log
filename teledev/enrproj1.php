<HTML>
<head>
<title>T�l�dev - Enregistrement de la proposition</title>
</head>
<body>
<p>
 
<?php
	$cnx = mysql_connect ("localhost", "login", "password");
	if (! $cnx)
		echo ("Connexion impossible<p>");
	else
		echo ("Connexion �tablie<p>");

	mysql_select_db ("teledev")
		or die ("Impossible d'acc�der � la base teledev<p>");

	$r = mysql_query (
		"INSERT INTO propositions (projet, nom, prix, delai) VALUES ('$nom', '$prix', '$delai')");

	echo ("Proposition de $nom pour le projet $projet enregistr�e<p>");

  
?>

<a href="teledev.htm">Retour au sommaire</a>

</body>
</HTML>
