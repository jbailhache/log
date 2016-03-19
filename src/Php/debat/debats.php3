<HTML>
<head>
<title>Cyber-discussions</title>
</head>
<body bgcolor=navy link=red vlink=yellow text=cyan>

<center>
<table width=33% cellpadding=10 border=1 bordercolor=red>
<tr><td bgcolor=C0B030>
<center>
<font face=arial size=5 color=navy>Forums de discussion</td></tr></table>

<p>
<a href=creation.php3>Créer une nouvelle discussion</a>

<p>
<form method=post action=debat.php3>

<input type=hidden name=table value=discussions>

Lire la discussion
<input type=text name=discussion value="">

<input type=submit value="Valider">

</form>

<p><a href=debat.zip>Télécharger les sources PHP</a>

<p>
Discussions publiques 
<p>
<table border=1 bordercolor=red cellpadding=5>

<?php

	include ("connexion.php3");

	if (connexion () > 0)
	{
		$query = "SELECT * FROM discussions WHERE position = 0";
		$data = mysql_query ($query);
		/*echo ($query);*/
		while ($rec = mysql_fetch_object ($data))
		{
			if ($rec->statut & 1)
			{
				echo ("<tr><td bgcolor=cyan><center> <a href=debat.php3?table=discussions&discussion=");
				echo (urlencode($rec->discussion));
				echo ("><font color=navy>");
				echo ($rec->discussion);
				/*echo (" statut:");
				echo ($rec->statut);*/
				echo ("</a></td></tr>");
			}
		} 
		
	}
?>

</table>

</body>
</HTML>

