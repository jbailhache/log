
<?php

	include ("connexion.php3");

	if (connexion() > 0)
	{
		mysql_query ("CREATE TABLE discussions (discussion VARCHAR(45) not null, numero INT not null auto_increment, position FLOAT not null, reponsea FLOAT not null, date VARCHAR(30) not null, auteur VARCHAR(40) not null, texte TEXT not null, KEY numero(numero))");
	}

?>
