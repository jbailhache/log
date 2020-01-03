<?php

	require ("framework.php");
	
	head("Mini Wiki","texture_2.jpg");

	connexion();

	$action = $_GET['action'];

	$auteur = post('auteur');
	$titre = post('titre');
	$texte = post('texte');

	if ($action == 'create')
	{
		$ok = TRUE;
		$messages = array();	

		if (empty($auteur))
		{
			$ok = FALSE;
			$messages[] = "Vous devez indiquer un nom ou un pseudo.";
		}
		if (empty($titre))
		{
			$ok = FALSE;
			$messages[] = "Vous devez indiquer un titre.";
		}
		if (empty($texte))
		{
			$ok = FALSE;
			$messages[] = "Vous devez saisir le texte de la page.";
		}
		if (post('code1') != post('code2'))
		{
			$ok = FALSE;
			$messages[] = "Le code de contrôle est incorrect.";
		}

		if ($ok)
		{
			$auteurq = format_query($auteur);
			$titreq = format_query($titre);
			$texteq = format_query($texte);
			$ip = ip();
			trace ("ip: $ip<p>auteur: $auteur<p>titre: $titre<p>texte: $texte");
			$requete = "INSERT INTO wiki (version, dernier, dat, ip, auteur, titre, texte) 
						VALUES (1, 1, datetime('now'), '$ip', '$auteurq', '$titreq', '$texteq')";
			trace ($requete);
			query($requete);
		}
		else
		{
			foreach ($messages as $message)
			{
				// echo "<div style='background:#ee1111;'>$message</div>";
				echo "<table><tr><td bgcolor=#ee1111>$message</td></tr></table>";
			}
		}
	}
?>
	<p>
	Créer une page
	<form method="post" action="index.php?action=create">
	<table><tr><td>
	Votre adresse IP : </td><td> <?php echo ip(); ?>
	</td></tr><tr><td>
	Votre nom ou pseudo : </td><td> <input type="text" name="auteur" size=60 value="<?php echo format_value($auteur); ?>">
	</td></tr><tr><td>
	Titre de la page : </td><td> <input type="text" name="titre" size=60 value="<?php echo format_value($titre); ?>">
	</td></tr></table>
	Texte de la page<br>
	<textarea name="texte" rows=20 cols=120><?php echo $texte; ?></textarea>
	<br>
	<?php captcha(); ?>
	<input type="submit" value="Créer la page">
	</form>

	<p>
	<table border=3 cellspacing=3 cellpadding=6>
	<tr>
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
		echo $ligne->dat;
		echo "</td><td>";
		echo $ligne->ip;
		echo "</td><td>";
		echo $ligne->auteur;
		echo "</td><td>";
		echo "<a href=page.php?action=display&id=" . $ligne->id . ">" . $ligne->titre . "</a>"; 
		echo "</td></tr>";
	}
?>
	</table>
<?php
    tail();

