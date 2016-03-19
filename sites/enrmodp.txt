<HTML>
<head>
<title>Enregistrement de la page</title>
</head>
<body>
<h3>Enregistrement de la page</h3>

<?php
	include ("util.php3");
	if (connexion() > 0)
	{
		$query = "INSERT INTO pages (site, rubrique, position, titre, motscles, texte) VALUES ('$site', '$rubrique', $position, '$titre', '$motscles', '$texte')";
		$r = mysql_query ($query);
		if (! $r)
		{
			echo ("Requête invalide : ");
			echo ($query);
		}
		else
		{
			echo ("Page $titre créée.");
		}
	}
?>

<p><a href=gestion.php3>Retour au sommaire</a>

</body>
</HTML>
