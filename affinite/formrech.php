<HTML>
<head>
<title>Recherche de partenaire</title>
</head>
<body>
<h3>Recherche de partenaire</h3>

Ceci permet d'obtenir une liste de profils triés par affinité (en fonction du but indiqué) décroissante par rapport à un profil de référence 
(normalement le vôtre mais vous pouvez aussi en indiquer un autre).
<p>
<form method=post action=recherch.php>
But :
<?php
   include ("util.php");
   if (connexion () > 0)
   {
    echo ("<select name=but>");
    $query = "SELECT * FROM afbut";
    $data = mysql_query ($query);
    while ($rec = mysql_fetch_object ($data))
    {
     $butvar = varencode ($rec->but);
     /*echo ("<li><input type=checkbox name=$butvar value=yes> $rec->but");*/
     echo ("<option>$rec->but");
    }
   }
 echo ("</select><p>Profil de référence : ");
 echo ("<br>Pseudo : <input type=text name=pseudo value=\"$afpseudo\">");
 echo ("<br>Nom du profil (le cas échéant) : <input type=text name=profil>");
 echo ("<br><input type=radio name=action value=inscription> description personnelle ou ");
 echo (    "<input type=radio name=action value=recherche checked> profil recherché ");
 echo ("<p>Profils triés : ");
 echo ("<br><input type=radio name=actiont value=inscription checked> description personnelle ou ");
 echo (    "<input type=radio name=actiont value=recherche> profil recherché ");
 echo ("<p><ul><li>Pour chercher celles ou ceux qui vous plaisent, indiquez profil de référence profil recherché et profils triés description personnelle.");
 echo ("<li>Pour chercher celles ou ceux à qui vous plaisez, indiquez profil de référence description personnelle et profils triés profil recherché.");
 echo ("<li>Pour chercher celles ou ceux qui vous ressemblent, indiquez description personnelle dans les 2 cas.");
 echo ("<li>Pour chercher celles ou ceux qui ont les mêmes goûts que vous, indiquez profil recherché dans les 2 cas.");
 echo ("</ul>"); 
?>
<input type=submit value=Rechercher>
</form>
</body>
</HTML>
