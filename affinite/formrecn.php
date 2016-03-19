<HTML>
<head>
<title>Recherche de partenaires</title>
</head>
<body>
<h3>Recherche de partenaires</h3>
<p>
<form method=post action=recherch.php>
<input type=submit value=Chercher> les 
<input type=text name=max value=20 size=5> personnes
<select name=recherche>
<option>qui plaisent le plus à
<option>qui aiment le plus
<option>qui ressemblent le plus à
<option>qui ont les goûts les plus voisins de ceux de
</select>
<?php
	include ("util.php");
	include ("utilaf.php");
	if (connexion() > 0)
	{
		
		echo ("<input type=text name=pseudo value=$afpseudo> (profil le cas échéant <input type=text name=profil>) pour ");
		selectbut();
		/*
		echo ("<select name=but>");
    		$query = "SELECT * FROM afbut";
	    	$data = mysql_query ($query);
    		while ($rec = mysql_fetch_object ($data))
    		{
     			$butvar = varencode ($rec->but);
      			echo ("<option>$rec->but");
    		}
		echo ("</select>");
		*/
	}
?>
</form>
</body>
</html>
