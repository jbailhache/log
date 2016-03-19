<HTML>
<head>
<title>Affinité</title>
</head>
<body>
<h3>Affinité</h3>
<form method=post action=enrmodpr.php>

<?php
 include ("util.php");
 if (connexion() > 0)
 {
  echo ("<input type=hidden name=action value=$action>");
  echo ("<input type=hidden name=but value='$but'>");
  if ($action == "inscription")
  {
   echo ("<p>Indiquez les renseignements vous concernant ci-dessous pour $but, votre profil inscription $profil : <p>");
   /* echo ("<p>Vous pouvez donner un nom à ce profil : <input type=text name=profil><p>"); */ 
  }
  if ($action == "recherche")
  {
   echo ("<p>Indiquez ci-dessous les caractéristiques des personnes que vous recherchez pour $but, votre profil recherche $profil : <p>");
   /*echo ("<p>Vous pouvez donner un nom au profil recherché : <input type=text name=profil><p>");*/
  }
  /*
   echo ("<p>Commencez par créer les réponses manquantes en cliquant sur \"autre\".");
   echo ("Ensuite cochez votre but ou vos buts.");
   echo ("Puis indiquez dans les cases à gauche de chaque réponse si cette réponse correspond.<p>");
  
   echo ("Votre but :<ul>");
   $query = "SELECT * FROM afbut";
   $data = mysql_query ($query);
   while ($rec = mysql_fetch_object ($data))
   {
    $butvar = varencode ($rec->but);
    echo ("<li><input type=checkbox name=$butvar value=yes> $rec->but");
   }
   echo ("<li> <a href=crebut.php?action=$action>autre</a>");
   echo ("</ul>");
  */
  $query = "SELECT * FROM afcateg";
  $data = mysql_query ($query);
  while ($rec = mysql_fetch_object ($data))
  {
   echo ("<p>");
   echo ($rec->categorie);
   echo (" : <ul>");
   $query1 = "SELECT * FROM afcarac WHERE categorie = '$rec->categorie' ORDER BY caractere";
   /*echo ($query1);*/
   $data1 = mysql_query ($query1);
   while ($rec1 = mysql_fetch_object ($data1))
   {
    echo ("<li> ");
    echo (" <input type=text name=");
    $caracvar = varencode ($rec->categorie) . varencode ($rec1->caractere);
    echo ($caracvar);
    /*echo ($rec1->caractere);*/
    echo (" size=10");

    $queryaf = "SELECT * FROM affinite WHERE action = '$action' AND pseudo = '$afpseudo'";
    /*if ($profil != "") */
     $queryaf = $queryaf . " AND profil = '$profil' ";
    $queryaf = $queryaf . " AND but = '$but' AND categorie = '$rec->categorie' AND caractere = '$rec1->caractere'";
    $dataaf = mysql_query ($queryaf);
    $recaf = mysql_fetch_object ($dataaf);
    if ($recaf)
     echo (" value=\"$recaf->reponse\"");
    
    echo ("> ");
    /*echo (" ### $queryaf ### ");*/
    echo ($rec1->caractere);
   }
   $categurl = urlencode ($rec->categorie);
   echo ("<li> <a href=crecarac.php?action=$action&categorie=$categurl>autre</a></ul>");
  }
  echo ("<p><a href=crecateg.php?action=$action>Autre catégorie</a>");
 }

?>
<p>
<input type=submit value="Valider">
</form>
<a href="http://teledev.multimania.com/log/">log</a> Ne passez pas à côté des choses compliquées !

</body>
</HTML>



