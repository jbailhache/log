<HTML>
<head>
<title>Enregistrement de la remarque</title>
</head>
<body>
<h3>Enregistrement de la remarque</h3>
 
<?php
	include ("util.php3");
	if (connexion() > 0)
	{
		$date = mydate ();
		$query = "INSERT INTO remarques (texte, date, site) VALUES ('$texte','$date', 'sites')";
		$r = mysql_query ($query);
		if (! $r)
		{
			echo ("Requête invalide : ");
			echo ($query);
		}
		else
		{
			echo ("Remarque enregistrée.");
		}
	}
?>

<p><a href=gestion.php3>Retour au sommaire</a>

</body>
</HTML>
