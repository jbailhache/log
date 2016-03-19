<HTML>
<head>
<title>Inscription</title>
</head>
<body>
<h3>Inscription</h3>

<?php
	include ("util.php");
	if (connexion() > 0)
	{
		$query = "SELECT * FROM afinscr WHERE pseudo = '$afpseudo'";
		$data = mysql_query ($query);
		while ($rec = mysql_fetch_object ($data))
		{
			echo ("<form method=post action=enrmodin.php>");
			/*echo ("<p>Pseudo : <input type=text name=pseudo value='$rec->pseudo'>");*/
			echo ("<p>Pseudo : $rec->pseudo");
			echo ("<p>Choisissez un mot de passe : <input type=text name=motdepasse value='$rec->motdepasse'>");
			echo ("<p>Prénom : <input type=text name=prenom value='$rec->prenom'>");
			echo ("<p>Nom : <input type=text name=nom value='$rec->nom'>");
			echo ("<p>Adresse : <input type=text name=adresse value='$rec->adresse'>");
			echo ("<p>Téléphone : <input type=text name=telephone value='$rec->telephone'>");
			echo ("<p>Email : <input type=text name=email value='$rec->email'>");
			echo ("<p>Web : <input type=text name=web value=$rec->web>");
			echo ("<p>Si vous avez une photo de vous sur le web indiquez son url :");
			echo (" <input type=text name=photo value='$rec->photo'>");
			echo ("<p>Autres informations : <input type=text name=autre value='$rec->autre'>");
			echo ("<p><input type=submit value=Valider></form>");
		}
	}
?>



</body>
</HTML>

