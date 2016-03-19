<HTML>
<?php
	include ("util.php");
	if (connexion() > 0)
	{
		echo ("<head><title>Informations concernant $pseudo</title></head><body><h3>Informations concernant $pseudo</h3><p>");
		$query = "SELECT * FROM afinscr WHERE pseudo = '$pseudo'";
		$data = mysql_query ($query);
		while ($rec = mysql_fetch_object ($data))
		{
			echo ("<p>$rec->prenom $rec->nom");
			$photo = $rec->photo;
			if (substr($photo,0,7) != "http://")
				$photo = "http://" . $photo;
			echo (" <img src=$photo>");
			echo ("<p>Adresse : $rec->adresse");
			echo ("<p>Téléphone : $rec->telephone");
			echo ("<p>email : <a href=mailto:$rec->email>$rec->email</a>");
			$web = $rec->web;
			if (substr($web,0,7) != "http://")
				$web = "http://" . $web;
			echo ("<p>web : <a href=$web>$rec->web</a>");
			echo ("<p>autres informations : $rec->autre");
			echo ("<p>");
		}
		$queryprofil = "SELECT * FROM afprofil WHERE pseudo = '$pseudo'";
		$dataprofil = mysql_query ($queryprofil);
		while ($recprofil = mysql_fetch_object ($dataprofil))
		{
		    echo ("<p><h4>Profil $recprofil->action $recprofil->profil</h4>");
			
		    echo ("<li>");
                    $querybut = "SELECT * FROM afbut";
                    $databut = mysql_query ($querybut);
                    while ($recbut = mysql_fetch_object ($databut))
                    {
                        echo ("<p><ul> But $recbut->but : ");

			$querycateg = "SELECT * FROM afcateg";
			$datacateg = mysql_query ($querycateg);
			while ($reccateg = mysql_fetch_object ($datacateg))
			{
				echo ("<p>$reccateg->categorie : <ul>");
				$querycarac = "SELECT * FROM afcarac WHERE categorie = '$reccateg->categorie'";
				$datacarac = mysql_query ($querycarac);
				while ($reccarac = mysql_fetch_object ($datacarac))
				{
					echo ("<li> $reccarac->caractere : ");
					$queryaf = "SELECT * FROM affinite WHERE pseudo = '$pseudo' AND action = '$recprofil->action' AND profil = '$recprofil->profil' AND but = '$recbut->but' AND categorie = '$reccateg->categorie' AND caractere = '$reccarac->caractere'";
					$dataaf = mysql_query ($queryaf);
					while ($recaf = mysql_fetch_object ($dataaf))
					{
						echo ($recaf->reponse);
						echo (". ");
					}					
				}	
				echo ("</ul>");						 
			}
			echo ("</ul>");
                    }
 		}
	}
?>
</body>
</HTML>
