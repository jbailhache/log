<HTML>
<head>
<title>T�l�dev - Liste des propositions de d�veloppement</title>
</head>
<body>
<h3>Liste des propositions de d�veloppement pour le projet
<?php echo($projet); ?>
</h3>
   
<?php

	$cnx = mysql_connect ("localhost", "login", "password");
	if (! $cnx)
		echo ("Connexion impossible<p>");
	else
		echo ("Connexion �tablie");

	mysql_select_db ("teledev")
		or die ("Impossible d'acc�der � la base teledev");
	
	$req = mysql_query ("SELECT * FROM propositions WHERE projet = '$projet' ");
 	echo ("<table border=1>");
	echo ("<tr><td> Consultant </td><td> Prix </td><td> D�lai </td><td> Plateforme </td><td> Langage </td></tr>");
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
