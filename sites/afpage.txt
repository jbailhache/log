
<?php
 
 	include ("util.php3");
	if (connexion() > 0)
	{
		$query = "SELECT * FROM pages WHERE site = '$site' AND titre = '$titre'";
		$data = mysql_query ($query);
		$rec = mysql_fetch_object ($data);
		
		echo ("<HTML><head><title>");
		echo ($rec->titre);
		echo ("</title><meta name=keywords content=\"");
		echo ($rec->motscles);
		echo ("\">");
		echo ($rec->head);
		echo ("</head><body ");
		echo ($rec->attributs);
		echo (">\n");
		/* echo ("<h3>$rec->titre</h3><p>\n"); */
		echo ($rec->texte);
		echo ("</body></HTML>");

	}

?>
