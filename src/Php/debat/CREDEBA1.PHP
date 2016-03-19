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
		while (!feof($f))
		{
			$line = fgets ($f, 30000);
			/* echo ($line); */
			if (substr ($line, 0, 7) == "<p><li>")
			{
				if ($started > 0)
				{
					echo ("Texte :<br>$texte<p>");
				}
				$started = 1;
                                $pos = strpos ($line, ":");
                                if (is_string ($pos) && !$pos)
				{
					$auteur = substr ($line, 7);
					$line = fgets ($f, 30000);		
					$pos = strpos ($line, ":");
					if (is_string ($pos) && !$pos)
					{
						$texte = $auteur . $line;
						$auteur = "Anonyme";
				}
				else
				{
					$auteur = substr ($line, 7, $pos - 7);
					$texte = substr ($line, $pos + 1);
				}
			}
		}		
	}

?>

</body>
</HTML>
