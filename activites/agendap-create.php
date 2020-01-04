<html>
<body>
<?php

  require("platform.php");

  connexion();

  echo ("<p>Base de données ouverte");
  query ('CREATE TABLE agendap (numero INTEGER PRIMARY KEY AUTOINCREMENT, code TEXT,  descr TEXT, jdls INTEGER, nieme INTEGER)');
  echo ("<p>Table créée");

 ?>
</body>
</html>
