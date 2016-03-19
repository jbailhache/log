<HTML>
<head>
<title>Affinité</title>
</head>
<body>
<h3>Affinité</h3>
<form method=post action=enraf.php>

<?php
 include ("util.php");
 if (connexion() > 0)
 {
  echo ("Votre but :<ul>");
  $query = "SELECT * FROM afbut";
  $data = mysql_query ($query);
  while ($rec = mysql_fetch_object ($data))
  {
   echo ("<li><input type=checkbox name=$rec->but> $rec->but");
  }
  echo ("<li> <a href=crebut.php>autre</a>");
  echo ("</ul>");
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
    echo ($rec1->caractere);
    echo (" size=3> ");
    echo ($rec1->caractere);
   }
   $categurl = urlencode ($rec->categorie);
   echo ("<li> <a href=crecarac.php?categorie=$categurl>autre</a></ul>");
  }
  echo ("<p><a href=crecateg.php>Autre catégorie</a>");
 }

?>
<p>
<input type=submit value="Valider">
</form>
</body>
</HTML>



