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
		"INSERT INTO participants (nom, email, client, consultant) VALUES ('$nom', '$email', '$client', '$consultant')");

	echo ("Inscription de $nom enregistr�e<p>");
	echo ("<a href="teledev.htm">Retour au sommaire</a><p>");


?>

</body>
</HTML>
