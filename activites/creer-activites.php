<html>
<body>
<?php

  require ("platform.php");

  connexion();

  echo ("<p>Base de donn�es ouverte");
  query ('CREATE TABLE activites (numero INTEGER PRIMARY KEY AUTOINCREMENT, url TEXT, descr TEXT, visible INTEGER)');
  echo ("<p>Table cr��e");

?>
</body>
</html>
