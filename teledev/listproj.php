<HTML>
<head>
<title>T�l�dev - Liste des projets</title>
</head>
<body>
<h3>Liste des projets</h3>
 
<?php

	$cnx = mysql_connect ("localhost", "login", "password");
	if (! $cnx)
		echo ("Connexion impossible<p>");
	else
		echo ("Connexion �tablie");

	mysql_select_db ("teledev")
		or die ("Impossible d'acc�der � la base teledev");
	
	$req = mysql_query ("SELECT * FROM projets");
        echo ("<p>Cliquez sur le nom du projet pour afficher les d�tails<p>");
	echo ("<table border=1>");
	echo ("<tr><td> Projet </td><td> Client </td><td> Description </td><td> Plateforme </td><td> Langage </td></tr>");
 	while ($rec = mysql_fetch_object ($req))
	{
                $projet = $rec->nom;
                echo ("<tr><td> <a href=detail.php?projet=$projet>");
                echo ($projet);
                echo ("</a> </td><td> ");
                echo ($rec->client);
                echo (" </td><td> ");
                echo ($rec->description);
                echo (" </td><td> ");
                echo ($rec->plateforme);
		echo (" </td><td> ");
		echo ($rec->langage);
                echo (" </td></tr>");

        }

	echo ("</table>");
	echo ("<p><a href=teledev.htm>Retour au sommaire</a>");

?>

<p>

</body>
</HTML>
