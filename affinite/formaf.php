<HTML>
<head>
<title>Calcul d'affinité</title>
</head>
<body>
<h3>Calcul d'affinité entre deux profils</h3>
<form method=post action=calcaf.php>
<p>
But :
<select name=but>
<?php
   include ("util.php");
   if (connexion () > 0)
   {
    $query = "SELECT * FROM afbut";
    $data = mysql_query ($query);
    while ($rec = mysql_fetch_object ($data))
    {
     $butvar = varencode ($rec->but);
     /*echo ("<li><input type=checkbox name=$butvar value=yes> $rec->but");*/
     echo ("<option>$rec->but");
    }
   }
?>
</select>
<p>
Premier profil :
<p>
<ul>
<li> <input type=radio name=action1 value=inscription> Profil personnel
<li> <input type=radio name=action1 value=recherche> Profil recherché
</ul>
Pseudo : <input type=text name=pseudo1>
<p>
Nom du profil (le cas échéant) : <input type=text name=profil1>
<p>
Second profil :
<p>
<ul>
<li> <input type=radio name=action2 value=inscription> Profil personnel
<li> <input type=radio name=action2 value=recherche> Profil recherché
</ul>
Pseudo : <input type=text name=pseudo2>
<p>
Nom du profil (le cas échéant) : <input type=text name=profil2>
<p>
<input type=submit value=Calculer>
</form>
<p>
<a href=index.php>Retour au sommaire</a>
</body>
</HTML>

