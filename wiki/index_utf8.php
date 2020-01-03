<?php

	require ("framework.php");
	
	head("Mini Wiki");

	connexion();

	$auteur = $_POST['auteur'];
	$titre = $_POST['titre'];
	$texte = $_POST['texte'];

	if (!empty($auteur) && !empty($titre) && !empty($texte))
	{
		$auteurq = format_query($auteur);
		$titreq = format_query($titre);
		$texteq = format_query($texte);
		$ip = ip();
		echo "<p>ip: $ip<p>auteur: $auteur<p>titre: $titre<p>texte: $texte<p>";
		$requete = "INSERT INTO wiki (dernier, dat, ip, auteur, titre, texte) VALUES (1, datetime('now'), '$ip', '$auteurq', '$titreq', '$texteq')";
		echo "<p>$requete<p>";
		//query($requete);
	}
?>
	<p>
	Créer une page
	<form method="post" action="index.php">
	<table><tr><td>
	Votre adresse IP : </td><td> <?php echo ip(); ?>
	</td></tr><tr><td>
	Votre nom ou pseudo : </td><td> <input type="text" name="auteur" size=60 value="<?php echo $auteur; ?>">
	</td></tr><tr><td>
	Titre de la page : </td><td> <input type="text" name="titre" size=60 value="<?php echo $titre; ?>">
	</td></tr></table>
	Texte de la page<br>
	<textarea name="texte" rows=20 cols=120><?php echo $texte; ?></textarea>
	<br>
	<input type="submit" value="Créer la page">
	</form>

	<p>
	<table border=3 cellspacing=3 cellpadding=6>
	<tr>
	<td>Numéro</td>
	<td>Date</td>
	<td>Adresse IP</td>
	<td>Auteur</td>
	<td>Titre</td>
	</tr>
<?php
	$requete = "SELECT * FROM wiki WHERE dernier = 1 ORDER BY dat DESC";
	$data = query ($requete);
	while ($ligne = fetch_object($data))
	{
		echo "<tr><td>";
		echo $ligne->numero;
		echo "</td><td>";
		echo $ligne->dat;
		echo "</td><td>";
		echo $ligne->ip;
		echo "</td><td>";
		echo $ligne->auteur;
		echo "</td><td>";
		echo $ligne->titre;
		echo "</td></tr>";
	}
?>
	</table>
<?php
    tail();

