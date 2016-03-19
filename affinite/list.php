<HTML>
<head>
<title>Liste des inscrits</title>
</head>
<body>
<h3>Liste des inscrits</h3>
<ul>
<?php
	include ("util.php");
	if (connexion() > 0)
	{
		$query = "SELECT * FROM afinscr ORDER BY pseudo";
		$data = mysql_query ($query);
		while ($rec = mysql_fetch_object($data))
		{
			echo ("<li>");
   			echo ("<a href=afinscr.php?pseudo=");
   			echo (urlencode($rec->pseudo));
   			echo (">");
   			echo ($rec->pseudo);
   			echo ("</a>");
		}
	}
?>
</ul>
</body>
</HTML>
