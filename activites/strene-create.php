<html>
<body>
<?php
 if ($db = sqlite_open ('mysqlitedb', 0666))
 {
  echo ("<p>Base de données ouverte");
  sqlite_query ($db, 'CREATE TABLE strene (annee INTEGER, mois INTEGER, jour INTEGER, sujet TEXT)');
  echo ("<p>Table créée");
 }
 else
 { 
  echo ("<p>Erreur ouverture base de données");
 }
?>
</body>
</html>