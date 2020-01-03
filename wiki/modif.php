<?php

	require ("framework.php");

	$id = $_GET['id'];

	connexion();

	$requete = "SELECT * FROM wiki WHERE id = $id AND dernier = 1";
	//echo $requete;

	$data = query ($requete);
	//echo "data=".print_r($data);

	$ligne = fetch_object ($data);
	//echo "ligne=".print_r($ligne);

	if ($ligne)
	{
		//echo "<p>La page existe</p>";

		$dat = $ligne->dat;
		$ip = $ligne->ip;
		$auteur = $ligne->auteur;
		$titre = $ligne->titre;
		$texte = $ligne->texte;
		
		head ($titre);

		echo "<p>Page créée le $dat par $auteur [$ip]</p>";
		//echo "<p>$titre</p>";
?>
		<form method="post" action="page.php?action=display">
		<input type="hidden" name="id" value="<?php echo $id; ?>">
		<textarea name="texte" rows=20 cols=120><?php echo $texte; ?></textarea>
		<p>
		Votre nom ou pseudo : <input type="text" name="auteur" value="">
		Votre adresse IP : <?php echo ip(); ?>
		<input type="submit" value="Enregistrer">
<?php

	}
	else
	{
		head ("Page inexistante");
	}

	tail();



