<HTML>
<head>
<title>Suppression d'une rubrique</title>
</head>
<body>
<h3>Suppression d'une rubrique</h3>

<?php
	include ("util.php3");
	$query = "SELECT * FROM sites WHERE site = '$site'";
	$data = mysql_query ($query);
	$rec = mysql_fetch_object ($data);
	if ($motdepasse != $rec->motdepasse)
		echo ("<p>Mot de passe incorrect.<p>");
	else
	{
 		$query = "DELETE FROM rubriques WHERE site = '$site' AND rubrique = '$rubrique'";
		$r = mysql_query ($query);
		if ($r)
			echo ("Rubrique $rubrique supprimée.");
		else
			echo ("Erreur dans la requête $query.");
	}
?>

<p>
<a href=gestion.php3>Retour au sommaire</a>

</body>
</HTML>
