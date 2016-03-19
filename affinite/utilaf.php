<?php

function valeur ($but, $action, $pseudo, $profil, $categorie, $caractere)
{
 $val = 0.0;
 $query = "SELECT * FROM affinite WHERE but = '$but' AND action = '$action' AND pseudo = '$pseudo' AND profil = '$profil' AND categorie = '$categorie' AND caractere = '$caractere'";
 /*echo ("<p>$query");*/
 $data = mysql_query ($query);
 while ($rec = mysql_fetch_object ($data))
 {
  /*echo ("<br>Valeur : $rec->valeur.");*/
  $val += $rec->valeur;
  /*echo (" Nouvelle valeur : $val.");*/
 }
 /*echo ("<p>Valeur finale $val  pour $but $action $pseudo -$profil- $categorie $caractere.");*/
 return $val;
}

function affinite ($but, $action1, $pseudo1, $profil1, $action2, $pseudo2, $profil2)
{
 $af = 0.0;
 $query = "SELECT * FROM afcarac";
 $data = mysql_query ($query);
 while ($rec = mysql_fetch_object ($data))
 {
  $val1 = valeur ($but, $action1, $pseudo1, $profil1, $rec->categorie, $rec->caractere);
  $val2 = valeur ($but, $action2, $pseudo2, $profil2, $rec->categorie, $rec->caractere);
  $af += $val1 * $val2;
 }
 return $af;
}

function selectbut ()
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
	echo ("</select>");
}
?>

