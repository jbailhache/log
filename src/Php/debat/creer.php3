<HTML>
<head>
<title>Création d'un nouveau débat</title>
</head>
<body>

<?php

	include ("connexion.php3");

	function query ($q)
	{
		$status = mysql_query ($q);
		if (!$status)
		{
			echo ("Erreur $q.<br>");
		}
	}
	
	if (connexion() > 0)
	{
		$date = date ("D d M Y, H:i");

/*		echo ("statut=$statut.");*/
		$statut1 = 0;
		if ($statut == "pub")
			$statut1 = 1;
/*		echo ("statut1=$statut1.");*/

		query ("INSERT INTO discussions (discussion, statut, position, auteur, date, reponsea, texte) VALUES ('$discussion', $statut1, 0, '$nom', '$date', 0, '$introduction')");
		query ("INSERT INTO discussions (discussion,         position, auteur, date, reponsea, texte) VALUES ('$discussion', 1000,        '$nom', '$date', 0, '$conclusion')");

		echo ("Discussion $discussion créée.");

		echo ("<p><a href=debats.php3>Retour au sommaire</a>");

		/* query ("CREATE TABLE $nom (numero INT not null auto_increment, position FLOAT not null, auteur VARCHAR(40) not null, date VARCHAR(30) not null, reponsea FLOAT not null, texte TEXT not null)"); */

		/*query ("CREATE TABLE $nom numero Counter");*/
		/*
		query ("ALTER TABLE $nom ADD numero INT not null");
		query ("ALTER TABLE $nom ADD position FLOAT not null");
		query ("ALTER TABLE $nom ADD auteur VARCHAR(40) not null");
		query ("ALTER TABLE $nom ADD date VARCHAR(30) not null");
		query ("ALTER TABLE $nom ADD reponsea FLOAT not null");
		query ("ALTER TABLE $nom ADD texte TEXT not null");
		*/
		/*
		query ("ALTER TABLE $nom ADD position Number not null");
		query ("ALTER TABLE $nom ADD auteur char(40) not null");
		query ("ALTER TABLE $nom ADD date char(30) not null");
		query ("ALTER TABLE $nom ADD reponsea Number not null");
		query ("ALTER TABLE $nom ADD texte char(1000) not null");
		*/
		/*
		query ("INSERT INTO $nom (position, auteur, date, reponsea, texte) VALUES (0, 'Animateur', '$date', 0, 'Bonjour')");
		query ("INSERT INTO $nom (position, auteur, date, reponsea, texte) VALUES (1000, 'Animateur', '$date', 0, 'Au revoir')");
	 	*/

	}
?>

</body>
</HTML>
