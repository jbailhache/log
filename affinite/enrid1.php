<?php
 include ("util.php");
 $titre = "<HTML><head><title>Contrôle de l'identification</title></head><body>";
 $cnx = mysql_connect ("localhost", "login", "password");
 if (! $cnx)
                echo ("<p>Connexion impossible<p>");
 else
 {
                mysql_select_db ("teledev_db")
                        or die ("Impossible d'accéder à la base teledev");
                $req = mysql_query ("SELECT * FROM afinscr WHERE pseudo ='$pseudo' AND motdepasse ='$motdepasse'");
                $rec = mysql_fetch_object ($req);
                if (!$rec)
		{
			echo ($titre);
			echo ("Mot de passe incorrect<p>");
			echo ("<a href=ident.php>Recommencer</a>");
		}
		else
		{
			setcookie ("afpseudo");
			setcookie ("afmotdepasse");
			setcookie ("afpseudo", $pseudo);
			setcookie ("afmotdepasse", $motdepasse);
			echo ($titre);
			echo ("Identification correcte);
		}
 }
?>
</body>
</HTML>
