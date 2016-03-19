<HTML>
<head>
<title>Modification d'un site</title>
</head>
<body background=fond.gif>
<h3>Modification d'un site</h3>
<p>
<form method=post action="enrmsite.php3">

<?php
	include ("util.php3");
	if (connexion() > 0)
	{
		$site1 = slashback ($site);
		$query = "SELECT * FROM sites WHERE site = '$site'";
		$data = mysql_query ($query);
		$rec = mysql_fetch_object ($data);
		if (!$rec)
			echo ("<p>Ce site n'existe pas.<p>");
		else if ($motdepasse != $rec->motdepasse)
			echo ("<p>Mot de passe incorrect.<p>");
		else
		{
			echo ("Nom du site : <input type=text name=site value=\"$site1\"><p>");
			echo ("Mot de passe : <input type=password name=motdepasse value=\"$rec->motdepasse\"><p>");
			echo ("Texte : &nbsp; &nbsp; &nbsp; ");
			echo ("<input type=submit value=\"Enregistrer les modifications\"><p>");
			echo ("<textarea name=texte rows=20 cols=70>");
			echo ($rec->texte);
			echo ("</textarea>");
		}
	}	
?>

<p>

</form>
</body>
</HTML>
