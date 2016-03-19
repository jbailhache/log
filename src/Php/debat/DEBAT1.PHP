<HTML>
<head>
<title>Discussion</title>
</head>
<body>
<h3>Discussion</h3>

<?php

	include ("connexion.php3");
	$posprec = 0;
 
	if (connexion () > 0)
	{
		$query = "SELECT * FROM debat1 ORDER BY position";
		$data = mysql_query ($query);
		echo ("<ul>");
		while ($interv = mysql_fetch_object($data))
		{
			if ($interv->numero > 1)
			{
				echo ("<br><a href=repondre.php3?position=$posprec&possuiv=");
				echo ($interv->position);
				echo (">Répondre</a>");
			}
			echo ("<p><li> ");
			echo ($interv->position);
			echo (" : ");
			echo ($interv->auteur);
			echo (" répond à ");
			echo ($interv->reponsea);
			echo (" le ");
			echo ($interv->date);
			echo (" : <br>");
			echo ($interv->texte);
			$posprec = $interv->position;
		}
	}
	
?>

</body>
</HTML>
