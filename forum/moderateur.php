<?php

 if (isset($_GET['forum']))
 {
  $forum = $_GET['forum'];
 }
 else
 {
  $forum = "log";
 }

 if (isset($_GET['accueil'])) 
 { 
  $accueil = $_GET['accueil']; 
 } 
 else 
 { 
  $accueil = "../index.htm";
 }

?>

<html>
<head>
<title>Acc&egrave;s mod&eacute;rateur</title>
</head>
<body>
<h2>Acc&egrave;s mod&eacute;rateur</h2>
<br>
<form method=post action=forum.php?forum=<?php echo $forum; ?>&accueil=<?php echo $accueil; ?>>
<input type=hidden name=forum value="<?php echo $forum; ?>">
<input type=hidden name=accueil value="<?php echo $accueil; ?>">
Mot de passe : 
<input type=text name=pass width=60>
<input type=submit value=OK>
</form>
</body>
</html>


