<?php
 
include ("../Php/connex.php3");

function connexion1 ()
{
	$cnx = mysql_connect ("sql.free.fr", "teledev", "123s7sn1");
	if (! $cnx)
	{
		echo ("Connexion à la base de données impossible<p>");
		return 0;
	}
	else
        {
	    echo ("<!--Connexion établie-->");
	    $status = mysql_select_db ("teledev");
	    if ($status)
	    {
		echo ("<!--Base de données sélectionnée-->");
	        return 1;
            }
	    else
	    {
		echo ("Impossible d acceder a la base de donnees.");
		return 0;            
	    }
 	}
}

?>