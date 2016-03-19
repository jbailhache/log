<HTML>
<head>
<title>Télédev - Liste des propositions de développement</title>
</head>
<body>
<h3>Liste des propositions de développement pour le projet
<?php echo($projet); ?>
</h3>
   
<?php

	$cnx = mysql_connect ("localhost", "login", "password");
	if (! $cnx)
		echo ("Connexion impossible<p>");
	else
		echo ("Connexion établie");

	mysql_select_db ("teledev")
		or die ("Impossible d'accéder à la base teledev");
	
	$req = mysql_query ("SELECT * FROM propositions WHERE projet = '$projet' ");
 	echo ("<table border=1>");
	echo ("<tr><td> Consultant </td><td> Prix </td><td> Délai </td><td> Plateforme </td><td> Langage </td></tr>");
 	while ($rec = mysql_fetch_object ($req))
	{
		echo ("<tr><td>");
		echo ($rec->consultant);
		echo ("</td><td>");
		echo ($rec->prix);
		echo ("</td><td>");
		echo ($rec->delai);
                echo ("</td></tr>");

        }

	echo ("</table>");
	echo ("<p><a href=teledev.htm>Retour au sommaire</a>");

?>

<p>

</body>
</HTML>
