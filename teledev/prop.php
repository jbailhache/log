<HTML>
<head>
<title>T�l�dev - Proposition de d�veloppement</title>
</head>
<body>
<h3>Proposition de d�veloppement pour le projet 
<?php echo ($projet); ?>
</h3>
  
<form type="post" action="enrproj.php">

<input type="hidden" name="projet" value=<?php echo($projet); ?>>

<table>
<tr>
<td>Votre nom :</td>
<td><input type="text" name="nom" value=""></td>
</tr><tr>
<td>Prix :</td>
<td><input type="text" name="prix" value=""></td>
</tr><tr>
<td>D�lai :</td>
<td><input type="text" name="delai" value=""></td>
</tr></table>
<p>

<p>
<input type="submit" value="Valider">
 
</body>
</HTML>

