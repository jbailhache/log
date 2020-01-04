<html>
<head>
<title>Agenda</title>
</head>
<body>
<br><br><br><br><br><br>
<h3>Agenda</h3>

<form method=post action=agenda.php>
<p>
Début dans <input type=text name=debut> jours
<br>
Fin dans <input type=text name=fin> jours
<br>
<input type=submit value="Afficher">
</form>
<p> (*) Activités périodiques affichées automatiquement en fonction du calendrier. 

<?php
 
 include ('platform.php');
 include ('afagenda.php');

 $debut = $_POST['debut'];
 $fin = $_POST['fin'];
 
 if (strlen($debut)==0)
  $debut = -1;
 if (strlen($fin)==0)
  $fin = 8;
 afagenda ($debut,$fin);

?>

</body>
</html>