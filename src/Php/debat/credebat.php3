<HTML>
<head>
<title>Création d'un débat</title>
</head>
<body>

<h3>Création d'un débat</h3>

<?php

	include ("connexion.php3");
	if (connexion () > 0)
	{
		$f = fopen ($filename, "r");
		$started = 0;
		$position = 0;
		while (!feof($f))
		{
			$line = fgets ($f, 1000);
			/* echo ($line); */
			if (substr ($line, 0, 7) == "<p><li>")
			{
				/*echo ("<p>Ligne lue : $line<p>");*/
				if ($started > 0)
				{
					echo ("Auteur : $auteur<br> ");
					echo ("Texte : $texte<p>");

					$position = $position + 10;
					$texte = str_replace ("'", "''", $texte);
					$query = "INSERT INTO discussions (discussion, position, auteur, date, reponsea, texte) VALUES ('$discussion', $position, '$auteur', '$date', $position-10, '$texte')";
					$status = mysql_query ($query);
					if (!$status)
						echo ("Erreur dans la requête $query.");								

					$texte = "";
				}
				$started = 1;
                                $pos = strpos ($line, " :");
                                /*if (is_string ($pos) && !$pos)*/
				if ($pos === false)
				{
					$auteur = substr ($line, 7);
					/*echo ("Debut auteur ($auteur)<br>");*/
					$line = fgets ($f, 1000);		
					$pos = strpos ($line, " :");
					/*if (is_string ($pos) && !$pos)*/
					if ($pos === false)
					{
						/*echo ("Pas d'auteur<br>");*/
						$texte = $auteur . $line;
						$auteur = "Anonyme";
					}
					else
					{
						$auteur = $auteur . substr ($line, 0, $pos);
						/*echo ("Auteur complet ($auteur)<br>");*/
						$texte = substr ($line, $pos+2);
						/*echo ("Debut texte ($texte)<br>");*/
					}
				}
				else
				{
					$auteur = substr ($line, 7, $pos - 7);
					$texte = substr ($line, $pos + 2);
				}
			}
			else
			{
				$xxx = substr ($line, 0, 7);
				/*echo ("Ca commence mal : $xxx.<br>");*/
				/*echo ("Test<br>");*/
				if ($started > 0)
				{	
					$texte = $texte . $line;
					/*echo ("<br>...$texte<br>");*/
				}
	
			}
		}		
	}

?>

</body>
</HTML>
