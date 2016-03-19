<HTML>
<head>
<title>Modification d'une page</title>
</head>
<body background=fond.gif>
<h3>Modification d'une page</h3>
<p>
<form method=post action=enrpage.php3>

<?php

	include ("util.php3");
	if (connexion() > 0)
	{
	    $query = "SELECT * FROM sites WHERE site = '$site'";
	    $data = mysql_query ($query);
	    $rec = mysql_fetch_object ($data);
	    if ($motdepasse != $rec->motdepasse)
		echo ("<p>Mot de passe incorrect.<p>");
	    else
	    {
		$query = "SELECT * FROM pages WHERE site = '$site' and titre = '$titre'";
		$data = mysql_query ($query);
		$rec = mysql_fetch_object ($data);
		if (!$rec)
		{
			echo ("<p>Cette page n'existe pas.<p>");
		}
		else
		{
			/*echo ($titre);
			  echo ($rec->titre);*/
			echo ("<input type=hidden name=site1 value=\"$rec->site\">");
			echo ("<input type=hidden name=titre1 value=\"$rec->titre\">");
			echo ("<input type=hidden name=modif value=yes>");
			echo ("<input type=hidden name=motdepasse value=\"$motdepasse\">");
			echo ("Nom du site : <input type=text name=site value=\"$rec->site\"><p>");
			echo ("Rubrique : <input type=text name=rubrique value=\"$rec->rubrique\"><p>");
			echo ("Position : <input type=text name=position value=\"$rec->position\"><p>");
			echo ("Titre de la page : <input type=text name=titre value=\"$rec->titre\" size=60><p>");
			echo ("<input type=checkbox name=create> Créer une nouvelle page<p>");
			echo ("<input type=checkbox name=delete> Supprimer la page<p>");
			echo ("Mots clés : <input type=text name=motscles value=\"$rec->motscles\"><p>");
                        echo ("Entête : <br><textarea name=head rows=5 cols=70>");
                        echo ($rec->head);
 			echo ("</textarea>");
                        echo ("<p>&lt;body <textarea name=attributs rows=3 cols=70>");
			echo ($rec->attributs);
			echo ("</textarea> &gt;</p>");
/*<input type=text name=attributs value=\"$rec->attributs\"> &gt;<p>");*/
			echo ("Texte : &nbsp; &nbsp; &nbsp; ");
			echo ("<input type=submit value=\"Enregistrer les modifications\">");
			echo ("<p><textarea name=texte rows=20 cols=70>");
			echo ($rec->texte);
			echo ("</textarea>");
			echo ("<input type=hidden name=modif value=yes>");
		}
	    }
	}	
?>


</form>
</body>
</HTML>


