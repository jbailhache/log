<HTML>
<head>
<title>Teledev - Liste des participants</title>
</head>
<body>
<h3>Liste des participants</h3>

<?php

	$cnx = mysql_connect ("localhost", "login", "password");
	if (! $cnx)
		echo ("Connexion impossible<p>");
	else
		echo ("Connexion �tablie");

	mysql_select_db ("teledev")
		or die ("Impossible d'acc�der � la base teledev");
	
	$req = mysql_query ("SELECT * FROM participants");
	echo ("<table border=1>");
        echo ("<tr><td>Nom</td><td>Email</td><td>Client</td><td><Consultant></td></tr>");
	while ($rec = mysql_fetch_object ($req))
	{
                echo ("<tr><td> ");
                echo ($rec->nom);
                echo (" </td><td> ");
                echo ($rec->email);
                echo (" </td><td> ");
                echo ($rec->client);
                echo (" </td><td> ");
                echo ($rec->consultant);
                echo (" </td></tr>");
        }

	echo ("</table>");
	echo ("<p><a href=teledev.htm>Retour au sommaire</a>");

?>

<p>

</body>
</HTML>
