<HTML>
<head>
<title>Discussion</title>
</head>
<body>

<?php

	include ("connexion.php3");

  	$discussion1 = str_replace ("\\", "", $discussion);

	echo ("<h3>Discussion $discussion1</h3>");

	$posprec = 0;
        $texteurl = ""; 
	if (connexion () > 0)
	{
		/*$query = "SELECT * FROM $discussion ORDER BY position";*/
		/*$query = "SELECT * FROM discussions WHERE discussion = '$discussion' ORDER BY position";*/
		$query = "SELECT * FROM $table WHERE discussion = '$discussion' ORDER BY position";
		/*echo ($query);*/
		$data = mysql_query ($query);
		echo ("<ul>");
		while ($interv = mysql_fetch_object($data))
		{
			if ($interv->position > 0)
			{
				$discussionurl = urlencode ($discussion1);
				echo ("<br><a href=repondre.php3?table=$table&discussion=$discussionurl&position=$posprec&possuiv=");
				echo ($interv->position);
				/*
				echo ("&texte=");
 				echo ($texteurl);
				*/
				echo (">Répondre</a>");
			}
			echo ("<p><li> ");
			echo ($interv->position);
			echo (" : ");
			echo ($interv->auteur);
			echo (" répond à ");
			echo ($interv->reponsea);
			if ($interv->date && !($interv->date == ""))
				echo (" le ");
			echo ($interv->date);
			echo (" : <br>");
			echo ($interv->texte);
			/*$texteurl = urlencode ($interv->texte);*/
			$posprec = $interv->position;
		}
	}
	
?>
<p>
<a href=debats.php3>Retour au sommaire</a>
</body>
</HTML>
