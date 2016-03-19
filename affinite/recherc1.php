<HTML>
<head>
<title>Recherche de partenaire</title>
</head>
<body>
<h3>Recherche de partenaire</h3>
<?php
 include ("util.php");
 include ("utilaf.php");
 echo ("<p>Profils $actiont triés par ordre d'affinité pour $but décroissant par rapport à $action $pseudo $profil : ");
 $query = "SELECT * FROM afprofil WHERE action = '$actiont'";
 $data = mysql_query ($query);
 while ($rec = mysql_fetch_object ($data))
 {
  echo ("<p>$rec->action $rec->pseudo $rec->profil : ");
  $af = affinite ($but, $action, $pseudo, $profil, $rec->action, $rec->pseudo, $rec->profil);
  echo ($af);
 }
?>

</body>
</HTML>
