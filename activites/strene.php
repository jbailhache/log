<title>Café philo du dimanche soir au Saint-René</title>
</head>
<body>
<br><br><br><br><br><br>
<div align=center>
<h2>Café philo</h2>
<h3>Tous les dimanches à 18 heures 
<br>au Saint-René
<br>148, bd de Charonne, Paris 20e (métro Alexandre Dumas)
</h3>

Animé en alternance par Claudine et Jean-François
<br>

<h3>
<p>Sujets des débats :
</h3>

<?php

 include ("platform.php");
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
  $req = "SELECT * FROM strene ORDER BY annee DESC, mois DESC, jour DESC";
  /* $data = mysql_query ($req); */
  /* $data = sqlite_query ($db, $req); */
  $data = query ($req);
  echo ("<table>");
  /* while ($r = mysql_fetch_object ($data)) */
  /* while ($r = sqlite_fetch_array ($data)) */
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
   echo '<tr><td>' . $r->jour . '/' . $r->mois . '/' . $r->annee . ' : </td><td>' . $r->sujet . '</td></tr>';
   
  }

  echo ("</table>");
 



 }
?>
</div>

</body>
</html>
