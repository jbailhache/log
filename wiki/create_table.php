<?php
	try
	{
		$pdo = new PDO('sqlite:wiki.sqlite');
		$query = "CREATE TABLE IF NOT EXISTS wiki (
					id INTEGER PRIMARY KEY AUTOINCREMENT,
					idorig INTEGER,
					idpre INTEGER,
					numero INTEGER,
					version INTEGER,
					dernier INTEGER,
					dat DATETIME DEFAULT NULL,
					ip TEXT DEFAULT NULL,
					auteur TEXT DEFAULT NULL,
					titre TEXT DEFAULT NULL,
					texte TEXT DEFAULT NULL
				 )";
		$status = $pdo->query($query);
		if ($status) {
			echo "<p>Table creee";
		} else {
			echo "<p>Erreur creation table<br><pre>";
			print_r($pdo->errorInfo());
		}
	}
	catch (Exception $e)
	{
		echo "<pre>";
		print_r($e);
	}
					
