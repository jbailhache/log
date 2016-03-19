<HTML>
<head>
<title>Enregistrement</title>
<body>
<h3>Enregistrement de votre réponse</h3>

<?php
	include ("connexion.php3");

	function npos ($pos1, $pos2)
	{
		for ($incr=10; ; $incr /= 10)
		{
			/*echo ("pos1=$pos1, pos2=$pos2, incr=$incr<br>");*/
			if ($pos1 + 2 * $incr <= $pos2)
				return $pos1 + $incr;
		}
	}

	if (connexion() > 0)
	{
		$discussion1 = str_replace ("\\", "", $discussion);
		/*$posrep = (9 * $position + $possuiv) / 10;*/
		$posrep = npos ($position, $possuiv);
		$date = date ("D d M Y, H:i");
		/* $query = "INSERT INTO $discussions (position, reponsea, date, auteur, texte) values ($posrep, $position, '$date', '$auteur', '$texte')"; */
		/*$query = "INSERT INTO discussions (discussion, position, reponsea, date, auteur, texte) values ('$discussion', $posrep, $position, '$date', '$auteur', '$texte')"; */
		$query = "INSERT INTO $table (discussion, position, reponsea, date, auteur, texte) values ('$discussion', $posrep, $position, '$date', '$auteur', '$texte')"; 
		$status = mysql_query ($query);
		if ($status)
		{
			echo ("<p>Votre réponse est enregistrée.");
		}
		else
		{
			echo ("Erreur dans la requête $query.");
		}
		$discussionurl = urlencode ($discussion1);
		echo ("<p><a href=debat.php3?table=$table&discussion=$discussionurl>Retour au texte</a>");		
	}

?>

</body>
</HTML>
