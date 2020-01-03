<?php

	require ("framework.php");

	function display ()
	{
		global $champ, $id;

		$requete = "SELECT * FROM wiki WHERE $champ = $id AND dernier = 1";
		//echo $requete;
	
		$data = query ($requete);
		//echo "data=".print_r($data);
	
		$ligne = fetch_object ($data);
		//echo "ligne=".print_r($ligne);
	
		if ($ligne)
		{
			//echo "<p>La page existe</p>";
	
			$newid = $ligne->id;
			$dat = $ligne->dat;
			$ip = $ligne->ip;
			$auteur = $ligne->auteur;
			$titre = $ligne->titre;
			$texte = $ligne->texte;
			
			head ($titre, "");
	
			echo "<p>Page créée le $dat par $auteur [$ip]</p>";
			//echo "<p>$titre</p>";
			echo "<p>$texte</p>";
	
			echo "<p><a href=page.php?action=modify&id=$newid>Modifier cette page</a></p>";
	
		}
		else
		{
			head ("Page inexistante", "");
		}
	}

	function save ()
	{
			global $champ, $id;
			//echo " empty ";
	
			$id = post('id');
			$auteur = post('auteur');
			$auteurq = format_query($auteur);
			$texte = post('texte');
			$texteq = format_query($texte);
	
			// $requete = "UPDATE wiki SET texte = '$texteq' WHERE id = $id";
	
			$requete = "UPDATE wiki SET dernier = 0 WHERE id = $id";
			trace ($requete);
			query ($requete);
	
			$requete = "SELECT * FROM wiki WHERE id = $id";
			//echo $requete;
	
			$data = query ($requete);
			//echo "data=".print_r($data);
	
			$ligne = fetch_object ($data);
			//echo "ligne=".print_r($ligne);
		
			if ($ligne)
			{
				//echo "<p>La page existe</p>";
		
				$idorig = $ligne->idorig;
				if (empty($idorig)) { $idorig = $id; }
				$version = $ligne->version;
				$ip = ip();
				$titre = $ligne->titre;
				$titreq = format_query($titre);
			
				$requete = "INSERT INTO wiki (idorig, idpre, version, dernier, dat, ip, auteur, titre, texte) 
							VALUES ($idorig, $id, $version+1, 1, datetime('now'), '$ip', '$auteurq', '$titreq', '$texteq')";
				trace ($requete);
				query ($requete);
			}
			else
			{
				echo "<p>Page inexistante</p>";
			}
			$champ = "idpre";
	}
	
	function modify ($re, $messages)
	{
		global $id;

		if (!$re)
		{
			$requete = "SELECT * FROM wiki WHERE id = $id AND dernier = 1";
			//echo $requete;
	
			$data = query ($requete);
			//echo "data=".print_r($data);
	
			$ligne = fetch_object ($data);
			//echo "ligne=".print_r($ligne);
	
			if (!$ligne)
			{
				head ("Page inexistante", "");
				return;
			}

			//echo "<p>La page existe</p>";
	
			$dat = $ligne->dat;
			$ip = $ligne->ip;
			$auteur = $ligne->auteur;
			$titre = $ligne->titre;
			$texte = $ligne->texte;
		}
		else
		{
			//echo "<h3>Nom non renseigné</h3>";
	
			trace ("_POST=".print_r($_POST,TRUE));

			$id = post('id');
			$dat = post('dat');
			$ip = post('ip');
			$auteur = post('auteur1');
			$titre = post('titre');
			$texte = post('texte');

		}			

		head ($titre, "texture_2.jpg");

		foreach ($messages as $message)
		{
			// echo "<p>$message</p>";
			echo "<table><tr><td bgcolor=#ee1111>$message</td></tr></table>";

		}
	
		echo "<p>Page créée le $dat par $auteur [$ip]</p>";
		//echo "<p>$titre</p>";
	?>
			<form method="post" action="page.php?action=save">
			<input type="hidden" name="id" value="<?php echo $id; ?>">
			<input type="hidden" name="dat" value="<?php echo $dat; ?>">
			<input type="hidden" name="ip" value="<?php echo $ip; ?>">
			<input type="hidden" name="auteur1" value="<?php echo format_value($auteur); ?>">
			<input type="hidden" name="titre" value="<?php echo format_value($titre); ?>">
			<textarea name="texte" rows=20 cols=120><?php echo $texte; ?></textarea>
			<p>
			Votre nom ou pseudo : <input type="text" name="auteur" value="">
			Votre adresse IP : <?php echo ip(); ?>
			<br>
			<?php captcha(); ?>
			<input type="submit" value="Enregistrer">
	<?php
	
	}

	function remodify ()
	{
		echo "<h3>Nom non renseigné</h3>";
	
		echo "_POST=";
		print_r($_POST);

		$id = post('id');
		$dat = post('dat');
		$ip = post('ip');
		$auteur = post('auteur1');
		$titre = post('titre');
		$texte = post('texte');

		head ($titre, "");

		echo "<p>Page créée le $dat par $auteur [$ip]</p>";
		//echo "<p>$titre</p>";
?>
		<form method="post" action="page.php?action=save">
		<input type="hidden" name="id" value="<?php echo $id; ?>">
		<input type="hidden" name="dat" value="<?php echo $dat; ?>">
		<input type="hidden" name="ip" value="<?php echo $ip; ?>">
		<input type="hidden" name="auteur1" value="<?php echo format_value($auteur); ?>">
		<input type="hidden" name="titre" value="<?php echo format_value($titre); ?>">
		<textarea name="texte" rows=20 cols=120><?php echo $texte; ?></textarea>
		<p>
		Votre nom ou pseudo : <input type="text" name="auteur" value="">
		Votre adresse IP : <?php echo ip(); ?>
		<input type="submit" value="Enregistrer">
<?php
	}

	$id = $_GET['id'];
	//echo " id=$id ";

	connexion();

	$action = $_GET['action'];

	$auteur = post('auteur');

	trace ("action=$action id=$id auteur=$auteur");

	/*if ($action == 'display' && !(empty($id) && empty($auteur))) {
		$champ = "id";	
		if (empty($id))
		{
			save ();
		}	
		echo " suite ";
		//echo "<p>id=$id</p>";	
		display ();
	} elseif ($action == 'display' && empty($id) && empty($auteur)) {
		remodify ();
	} elseif ($action == "modify") {
		modify ();
	}*/

	if ($action == 'display')
	{
		$champ = "id";
		display ();
	}
	elseif ($action == 'modify')
	{
		modify (FALSE, array());
	}
	elseif ($action == 'save') 
	{
		$ok = TRUE;
		$messages = array();
		
		if (empty($auteur))
		{
			$ok = FALSE;
			$messages[] = "Vous devez indiquer un nom ou un pseudo.";
		}
		if (post('code1') != post('code2'))
		{
			$ok = FALSE;
			$messages[] = "Le code de contrôle est incorrect.";
		}
		if ($ok)
		{
			save ();
			display ();
		}
		else
		{
			modify (TRUE, $messages);
		}
	}

?>
	<p><a href="index.php">Retour</a></p>
<?php
	tail();

