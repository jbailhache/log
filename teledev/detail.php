<HTML>
<head>
<title>Télédev - Détail d'un projet</title>
</head>
<body>

<?php
	echo ("<h3>Détail du projet $projet</h3>");

	$cnx = mysql_connect ("localhost", "login", "password");
	if (! $cnx)
		echo ("<p>Connexion impossible<p>");
	else
		echo ("<p>Connexion établie<p>");

	mysql_select_db ("teledev")
		or die ("Impossible d'accéder à la base teledev");

	$req = mysql_query ("SELECT * FROM projets WHERE nom = '$projet'");
	$rec = mysql_fetch_object ($req);
	if (! $rec)
		echo ("Projet $projet non trouvé");
	else
	{
		echo ("<p><table>");
		echo ("<tr><td>Client :</td><td>");
		echo ($rec->client);
		echo ("</td></tr><tr><td>Description :</td><td>");
		echo ($rec->description);
		echo ("</td></tr><tr><td>Plateforme :</td><td>");
		echo ($rec->plateforme);
		echo ("</td></tr><tr><td>Langage :</td><td>");
		echo ($rec->langage);
		echo ("</td></tr>");
		echo ("</table>");
		echo ("<p>Cahier des charges :</p>");
		echo ($rec->cahier);
	}

	echo ("<p><a href=prop.php?projet=$projet>Consultants, proposez un développement</a><p>");
	echo ("<a href=listprop.php?projet=$projet>Clients, consultez la liste des propositions pour ce projet</a><p>");

?>

<a href="listproj.php">Retour à la liste</a><p>
<a href="teledev.htm">Retour au sommaire</a>

</body>
</HTML>
