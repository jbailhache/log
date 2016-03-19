<HTML>
<head>
<title>Télédev - Enregistrement de la proposition</title>
</head>
<body>
<p>
 
<?php
	$cnx = mysql_connect ("localhost", "login", "password");
	if (! $cnx)
		echo ("Connexion impossible<p>");
	else
		echo ("Connexion établie<p>");

	mysql_select_db ("teledev")
		or die ("Impossible d'accéder à la base teledev<p>");

	$r = mysql_query (
		"INSERT INTO propositions (projet, nom, prix, delai) VALUES ('$nom', '$prix', '$delai')");

	echo ("Proposition de $nom pour le projet $projet enregistrée<p>");

  
?>

<a href="teledev.htm">Retour au sommaire</a>

</body>
</HTML>
