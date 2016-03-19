<?php
 include ("../Php/util.php3");
 
 class choix
 {
  var $objet;
  var $ident;
  var $valeur; 
 }

 function choisir ($objet)
 {
  $query = "SELECT * FROM choix WHERE site = 'exchoix' AND objet = '$objet'";
  $data = mysql_query ($query);
  $total = 0;
  $n = 0;
  while ($rec = mysql_fetch_object ($data))
  {
   /* echo ("<br>$rec->ident "); */
   $total += $rec->points;
   /* echo ($total); */
   $tab[$n++] = array
   ("objet"=>$objet,
    "ident"=>$rec->ident,
    "valeur"=>$rec->valeur,
    "limite"=>$total
    );
  }
  $r = rand (0, $total-1);
  /*echo (" r=$r "); */
  for ($i=0; $i<$n; $i++)
  {
   $l = $tab[$i]["limite"];
   /* echo (" l=$l "); */
   if ($r < $tab[$i]["limite"])
    return $tab[$i];
  }
  echo (" erreur ");
 }


 function transmettre_choix ($choix)
 {
  $objet = $choix["objet"];
  $ident = $choix["ident"];
  /*echo ("&$objet=$ident");*/
  echo ("$objet:$ident,");
 }

 function debut_choix ()
 {
  echo ("&choix=");
 }

 function fin_choix ()
 {
 }

 function recompenser ($choix, $x)
 {
  $tabchoix = explode (",", $choix);
  $n = count ($tabchoix);
  /*echo ("<p>n=$n");*/
  for ($i=0; $i<$n-1; $i++)
  {
   /*echo ($tabchoix[$i]);
   echo (" - ");*/
   $split = explode (":", $tabchoix[$i]);
   $objet = $split[0];
   $ident = $split[1];
   $query = "SELECT * FROM choix WHERE site = 'exchoix' AND objet = '$objet' AND ident = '$ident'";
   /* echo ($query); */
   $data = mysql_query ($query);
   if ($rec = mysql_fetch_object($data))
   {
    $npoints = $rec->points + $x;
    $query = "UPDATE choix SET points = $npoints WHERE site = 'exchoix' AND objet = '$objet' AND ident = '$ident'";
    $r = mysql_query ($query);
    if (!$r)
    {
     echo ("<p>Erreur $query :");
     echo (mysql_error());
    }
   }
  }
 }

 function recompenser1 ($x)
 {
  $query = "SELECT * FROM choix WHERE site = 'exchoix'";
  $data = mysql_query ($query);
  while ($rec = mysql_fetch_object ($data))
  {
   echo ("<p>");
   echo ($rec->objet);
   echo ("=");
   /*echo (${$rec->objet});*/
   $instr = '$val = $' . $rec->objet . ';';
   echo ($instr);
   eval ($instr);
   echo ($val);
   $var = $rec->objet;
   $val = $$var;
   echo ($val);
   $val = getenv ($var);
   echo ($val);
   $val = $titre;
   echo ($val);
   echo ($titre);
   echo ("coucou");
   echo ($page);
   if (${$rec->objet} == $rec->ident)
   {
    $npoints = $rec->points + $x;
    $query = "UPDATE choix WHERE site = 'exchoix' AND objet = '$rec->objet' AND ident = '$rec->ident SET points = $npoints";
    echo ($query);
    $r = mysql_query ($query);
    if (!$r)
     echo (" Erreur $query ");
   }
  }
 }

 if (connexion() > 0)
 {
  $seed = time();
  $seed = $seed * $seed;
  /*echo (" seed=$seed ");*/
  srand($seed);
  switch ($page)
  {
   case "accueil":
    echo ("<body>");
    $titre = choisir ("titre");
    echo ($titre["valeur"]);
    echo ("<p>");
    echo ("<a href=exchoix.php3?page=infos");
    debut_choix();
    transmettre_choix($titre);
    fin_choix();
    echo (">Plus d'informations</a>");
    echo ("</body>");
    break;
   case "infos":
    echo ("<body>");
    echo ("blablabla");
    echo ("</body>");
    recompenser ($choix, 10);
    break;

  }     
 }


?>
