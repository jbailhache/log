<HTML>
<head>
<title>Enregistrement du profil</title>
</head>
<body>
<?php
 include ("util.php");
 
 $querycateg = "SELECT * FROM afcateg";
 $datacateg = mysql_query ($querycateg);
 while ($reccateg = mysql_fetch_object ($datacateg))
 {
     /*echo ("<p>Catégorie $reccateg->categorie");*/
     $querycarac = "SELECT * FROM afcarac WHERE categorie = '$reccateg->categorie' ORDER BY caractere";
     /*echo ($query1);*/
     $datacarac = mysql_query ($querycarac);
     while ($reccarac = mysql_fetch_object ($datacarac))
     {
      /*echo ("<p>Caractère $reccarac->caractere");*/
      $caracvar = varencode ($reccateg->categorie) . varencode ($reccarac->caractere);
      $instr = "\$reponse = $$caracvar;";
      /*echo ("<p>instr = $instr.");*/
      $reponse = "";
      eval ($instr);
      if ($reponse != "")
      {
       $valeur = valnum ($reponse);
       echo ("<p>Réponse pour $reccateg->categorie $reccarac->caractere = $reponse, valeur = $valeur.");

       $queryaf = "SELECT * FROM affinite WHERE action = '$action' AND pseudo = '$afpseudo' AND profil = '$profil' AND but = '$but' AND categorie = '$reccateg->categorie' AND caractere = '$reccarac->caractere'";
       $dataaf = mysql_query ($queryaf);
       $recaf = mysql_fetch_object ($dataaf);
       if (!$recaf)
        $queryaf = "INSERT INTO affinite (action, pseudo, profil, but, categorie, caractere, reponse, valeur) VALUES ('$action', '$afpseudo', '$profil', '$but', '$reccateg->categorie', '$reccarac->caractere', '$reponse', '$valeur')";
       else
        $queryaf = "UPDATE affinite SET reponse = '$reponse', valeur = '$valeur' WHERE action = '$action' AND pseudo = '$afpseudo' AND profil = '$profil' AND but = '$but' AND categorie = '$reccateg->categorie' AND caractere = '$reccarac->caractere'";

       echo ("<p>MySQL $queryaf.");
       $r = mysql_query ($queryaf);
       if (!$r)
        echo ("<p>Requete $queryaf incorrecte.");
      }
     }
 }
 echo ("<p>Profil enregistré.<p><a href=index.php>Retour au sommaire</a>");
?>
</body>
</HTML>
