<title>Café philo de la Contrescarpe</title>
</head>
<body>
<br><br><br><br><br><br>
<div align=center>
<h2>Café philo de la Contrescarpe</h2>
<h3>Tous les mardis à 19h 
<br>au café - brasserie "La Contrescarpe"
<br>57, rue Lacépède (place de la Contrescarpe) à Paris
</h3>

Animé en alternance par Françoise et Marian.
<br>
La consommation est obligatoire.
<br>
<!-- form method=post action=contrescarpe-maintenance.php -->
<form method=post action=cm-sqlite.php>
<p>Ajouter un sujet :
<br>
jour : <input type=text name=jour size=2>
mois : <input type=text name=mois size=2>
année : <input type=text name=annee size=4>
<br>sujet : <input type=text name=sujet size=60>
<br><input type=submit value="Enregistrer">
</form>

<h3>
<p>Sujets des débats :
</h3>

<?php

 include ("sqlite.php");
 /* init (['annee','mois','jour','sujet']); */
 $vars = ['annee','mois','jour','sujet'];
 foreach ($vars as $var)
 {
  /* $$var = $_POST[$var]; */
  $instr = '$' . $var . ' = $_POST[\'' .  $var . '\'];';
  echo '<li>' . $instr;
  eval ($instr);
 }

 if (connexion() <= 0)
 {
  echo "<p>Erreur connexion";
 } 
 else
 {
  if ($annee != "" && $mois != "" && $jour != "") 
  {
   $req = "DELETE FROM contrescarpe WHERE annee = '$annee' AND mois = '$mois' AND jour = '$jour'";
   /* $status = mysql_query ($req); */
   /* $status = sqlite_query ($db, $req); */
   $status = query ($req);
   if ($sujet != "")
   {
    $req = "INSERT INTO contrescarpe (annee, mois, jour, sujet) VALUES ('$annee', '$mois', '$jour', '$sujet')";
    /* 
    $status = mysql_query ($req);
    if (!$status)
    {
     $er = mysql_error();
     echo ("<br>Erreur : [$er]<br>Requete : [$req]");
    } 
    */
    /* $status = sqlite_query ($db, $req); */
    $status = query ($req);
    if (!$status )
    {
     echo ("<br>Erreur requête [$req]");
    }
   }
  }

  $req = "SELECT * FROM contrescarpe ORDER BY annee DESC, mois DESC, jour DESC";
  /* $data = mysql_query ($req); */
  /* $data = sqlite_query ($db, $req); */
  $data = query ($req);
  echo ("<table>");
  /* while ($r = mysql_fetch_object ($data)) */
  /* while ($r = sqlite_fetch_array ($data)) */
  while ($r = sqlite_fetch_array ($data)) 
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

   echo '<tr><td>' . $r->jour . '/' . $r->mois . '/' . $r->annee . ' : </td><td>' . $r->sujet . '</td></tr>';
  }

  echo ("</table>");
 



 }
?>
</div>

</body>
</html>
