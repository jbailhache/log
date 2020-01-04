<html>
<body>
<br><br><br><br><br><br>
<h2>Maintenance agenda</h3>
<p>
<h3>Activités uniques</h2>

<form method=post action=agenda-mu.php>
Ajouter ou modifier une activité
<p> supprimer <input type=checkbox name=suppr>
<br> code : <input type=text name=code>
<br> jour : <input type=text name=jdm size=2> 
     mois : <input type=text name=mois size=2>
    annee : <input type=text name=annee size=4>
<br> complément : <input type=checkbox name=compl>
     annulation : <input type=checkbox name=annul>
<br> description :
<br>
<textarea name=descr rows=10 cols=60>
</textarea>
<br><input type=submit value='Enregistrer'>
</form>


<?php
 $code = $_POST['code'];
 $jdm = $_POST['jdm'];
 $mois = $_POST['mois'];
 $annee = $_POST['annee'];
 $compl = $_POST['compl']?1:0;
 $annul = $_POST['annul']?1:0;
 $descr = $_POST['descr'];
 $suppr = $_POST['suppr'];

 include ("platform.php");
 $jours = array ('dimanche', 'lundi', 'mardi', 'mercredi', 'jeudi', 'vendredi', 'samedi');
 if (connexion() < 0)
  echo '<p>Erreur connexion';
 else
 {
  echo '<p>connexion';
  if (strlen($jdm)>0 && strlen($mois)>0 && strlen($annee)>0)
  {
   $req = 'DELETE FROM agendau WHERE code = \'' . $code . '\' AND jdm = ' . $jdm . ' AND mois = ' . $mois . ' AND annee = ' . $annee;
   echo '<p>' . $req;
   query ($req); 
   if ($suppr != 'on')
   {
    $req = 'INSERT INTO agendau (code, jdm, mois, annee, compl, annul, descr) VALUES (\'' . $code . '\', ' .  $jdm . ', ' . $mois . ', ' . $annee . ', ' . $compl . ', ' . $annul . ', \'' . $descr . '\')';
    echo '<p>' . $req;
    query ($req); 

   }
  }
  $req = "SELECT * FROM agendau ORDER BY annee, mois, jdm";
  $data = query ($req);
  echo '<p>Activités uniques :';
  while ($r = fetch_object($data))
  {
   echo '<li> ' . $r->jdm . '/' . $r->mois . '/' . $r->annee . ' : ' . $r->code . ' compl=' . $r->compl . ' annul=' . $r->annul . ' : ' . $r->descr;

  }
 }
?>
</body>
</html>