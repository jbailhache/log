<HTML>
<head>
<title>T�l�dev - Enregistrement de l'inscription</title>
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
		"INSERT INTO projets (client, nom, description, plateforme, langage, cahier) VALUES ('$client', '$nom', '$description', '$plateforme', '$langage', '$cahier')");

	echo ("Inscription du projet $nom enregistr�e<p>");
 
?>

<a href="teledev.htm">Retour au sommaire</a>

</body>
</HTML>
