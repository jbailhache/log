
<?php
	include  ("util.php3");
	if (connexion() > 0)
	{
		$query = "SELECT * FROM pages";
		$data = mysql_query ($query);
		while ($rec = mysql_fetch_object ($data))
		{
			echo ("<p> $rec->site $rec->rubrique $rec->position $rec->titre $rec->motscles ");
		}

	}
?>
