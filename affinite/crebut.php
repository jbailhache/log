<HTML>
<head>
<title>Création d'un but</title>
</head>
<body>
<form method=post action=enrbut.php>
<?php 
 /*echo ("action = $action <p>");*/
 echo ("<input type=hidden name=action value=$action>"); 
?>
Nouveau but :<p>
<input type=text name=but>
<p>
<input type=submit value=Valider>
</form>
</body>
</HTML>
