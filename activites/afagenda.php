<?php
 
/* include ('platform.php'); */

function afagenda ($debut, $fin)
{
 
 $jours = array ('dimanche', 'lundi', 'mardi', 'mercredi', 'jeudi', 'vendredi', 'samedi');

 if (connexion() < 0)
 {
  echo '<p>Erreur connexion';
 }
 else
 {
  $auj = time();
  $dureej = 24 * 60 * 60;
  echo '<ul>';
  for ($i=$debut; $i<=$fin; $i++)
  {
   $jour = $auj + $i * $dureej;
   $jdm = gmdate ('d', $jour);
   $m = gmdate ('n', $jour);
   $a = gmdate ('Y', $jour);
   $jdls = gmdate ('w', $jour);
   /* $nieme = intval(($jm - 1) / 7) + 1;*/
   $nieme = ($jdm - 1 - (($jdm - 1) % 7)) / 7 + 1;
   /* echo "<p><li>J+$i: $nieme $jdls $jdm/$m/$a."; */
   echo '<p><li> ' /* . $nieme . 'ieme ' */ . $jours[$jdls] . ' ' . $jdm . '/' . $m . '/' . $a . ' :';

   $req = 'SELECT * FROM agendau WHERE jdm = ' . $jdm . ' AND mois = ' . $m . ' AND annee = ' . $a . ' AND compl = 0 AND annul = 0';
   /* echo '<br>' . $req; */
   $data = query ($req);
   echo '<ul>';
   while ($r = fetch_object($data))
   {
    echo '<li> ' . $r->descr;
   }
   /* echo '</ul>'; */

   /* echo 'Activités périodiques :'; */
   $req = 'SELECT * FROM agendap WHERE jdls = ' . $jdls . ' AND (nieme = ' . $nieme . ' OR nieme = 0)';
   /* echo '<br>'.$req; */
   $data = query ($req);
   /* echo '<ul>'; */
   while ($r = fetch_object($data))
   {
    $req1 = 'SELECT * FROM agendau WHERE code = \'' . $r->code . '\' AND jdm = ' . $jdm . ' AND mois = ' . $m . ' AND annee = ' . $a . ' AND (compl = 1 OR annul = 1)';
    /* echo '<br>' . $req1; */
    $data1 = query ($req1);
    $r1 = fetch_object ($data1);
    if ($r1)
    {
     if ($r1->annul == 0)
     {
      echo '<li> ' . $r->descr . '<br>' . $r1->descr; 
     }
    }
    else echo '<li> ' . $r->descr . ' (*)'; 
   }
   echo '</ul>';
 
  }
  echo '</ul>'; 
 }

}
?>
