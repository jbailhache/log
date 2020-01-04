<?php

include ("platform.php");

function cadre ($anim)
{
?>
<div align=center>
<table border><tr><td>
<div align=center>
<h2>Café philo de la Contrescarpe</h2>
Tous les mardis à 18h
<br>au café - brasserie <b>"La Contrescarpe"</b>
<br>57, rue Lacépède (place de la Contrescarpe) à Paris
<br>Tel. 01 43 36 82 88
<br><br>
<!--
<input type=text size=36 value="Animé par Marian"> 
-->
Animé en alternance par Marian Sznyter et Michel Turrini
<br><br>
Site internet :
<br>http://membres.lycos.fr/teledev/contrescarpe.php


<br>
<!-- form method=post action=contrescarpe-maintenance.php -->

</div>
<h4>
<p> Sujets des débats :
</h4>

<?php

 
 /* init (['annee','mois','jour','sujet']); */
 $vars = array ('annee','mois','jour','sujet');
 foreach ($vars as $var)
 {
  /* $$var = $_POST[$var]; */
  $instr = '$' . $var . ' = $_POST[\'' .  $var . '\'];';
  /* echo '<li>' . $instr; */
  eval ($instr);
 }

 if (connexion() <= 0)
 {
  echo "<p>Erreur connexion";
 } 
 else
 {
  $req = "SELECT * FROM contrescarpe ORDER BY annee, mois, jour";
  /* $data = mysql_query ($req); */
  /* $data = sqlite_query ($db, $req); */
  $data = query ($req);
  echo ("<table>");
  /* while ($r = mysql_fetch_object ($data)) */
  /* while ($r = sqlite_fetch_array ($data)) */
  $auj = time();
  $dureej = 24 * 60 * 60;
  while ($r = fetch_object ($data)) 
  {
   /*
   echo ("<tr><td>");
   echo ($r->jour);
   echo ("/");
   echo ($r->mois);
   echo ("/");
   echo ($r->annee);  
   echo (" : </td><td>");
   echo ($r->sujet); 
   echo ("</td></tr>");
   */
   $j = mktime (12, 0, 0, $r->mois, $r->jour, $r->annee);
   $d = ($auj - $j) / $dureej;
   /* echo 'hist='.($_GET['hist']=='oui');*/
   if (($_GET['hist'] == 'oui') || ($d <= 31))
   {
    echo '<tr><td>' . $r->jour . '/' . $r->mois . '/' . $r->annee . ' : </td><td>' . $r->sujet . '</td></tr>';
   }
  }

  
  echo ("</table>");

echo "<i>Un autre sujet que celui annoncé pourra être voté le soir même<br> en cas d'absence de l'intervenant prévu.</i>";
 
/* 
echo '<br><a href="http://membres.lycos.fr/teledev/contrescarpe.php?hist=oui">historique</a>';
*/

 }
?>
<br>

</td></tr><tr><td>

<h4> La consommation est obligatoire.</h4>

</tr></td><tr><td>

Autres cafés philo :
<br>
<a href=http://membres.lycos.fr/teledev/activites.php>
http://membres.lycos.fr/teledev/activites.php
</a>
</td></tr></table>
</div>
<?php
}
?>

<html>
<head>
<title>Café philo de la Contrescarpe</title>
</head>
<body>
<br><br><br><br><br><br>

<?php
 $anim = $_POST['anim'];
 echo '<table><tr><td>';
 cadre($anim);
 echo '</td><td>';
 cadre($anim);
 echo '</tr><tr><td>';
 cadre($anim);
 echo '</td><td>';
 cadre($anim);
 echo '</td></tr></table>';
?>

</body>
</html>
