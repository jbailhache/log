<?php

function connexion ()
{
	$cnx = mysql_connect ("localhost", "login", "password");
	if (! $cnx)
	{
		echo ("Connexion à la base de données impossible<p>");
		return 0;
	}
	else
        {
	    echo ("<!--Connexion établie-->");
	    $status = mysql_select_db ("teledev_db");
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

function verif_motdepasse ()
{
		$query = "SELECT * FROM sites WHERE site = '$site'";
		$data = mysql_query ($query);
		$rec = mysql_fetch_object ($data);
		if ($motdepasse != $rec->motdepasse)
		{
			echo ("<p>Mot de passe incorrect.<p>");
			return false;
		}
		else
		{	
			echo ("<p>Mot de passe correct. $motdepasse = $rec->motdepasse <p>");
			return true;
		}

}

function select_site ()
{
	if (connexion() > 0)
	{
		echo ("<select name=site>");
		echo ("<option selected>Sélectionnez le site");
		$query = "SELECT * FROM sites ORDER BY site";
		$data = mysql_query ($query);
		while ($rec = mysql_fetch_object ($data))
		{
			echo ("<option>$rec->site");
		}
		echo ("</select>");		
		echo (" &nbsp; Mot de passe : <input type=password name=motdepasse> ");
	}	
}

function select_rubrique ()
{
	if (connexion() > 0)
	{
		echo ("<select name=rubrique>");
		echo ("<option selected>Sélectionnez la rubrique");
		$query = "SELECT * FROM rubriques ORDER BY rubrique";
		$data = mysql_query ($query);
		while ($rec = mysql_fetch_object ($data))
		{
			echo ("<option>$rec->rubrique");
		}
		echo ("</select>");		
	}
}

function select_titre ()
{
	if (connexion() > 0)
	{
		echo ("<select name=titre>");
		echo ("<option selected>Sélectionnez le titre de la page");
		$query = "SELECT * FROM pages ORDER BY titre";
		$data = mysql_query ($query);
		while ($rec = mysql_fetch_object ($data))
		{
			echo ("<option>$rec->titre");
		}
		echo ("</select>");		
	}
}
function mydate ()
{
	$date = date ("D d M Y, H:i");
	return $date;
}

function logevt ($typ, $message)
{
	/* $date = date ("D d M Y, H:i"); */
	$date = mydate();
	$query = "INSERT INTO log (date, typ, message) VALUES ('$date', '$typ', '$message')";
	$status = mysql_query ($query);	
}

function echopart ($nom)
{
	$nomurl = urlencode ($nom);
	echo ("<a href=detpart.php?nom=$nomurl>");
	echo ($nom);
	echo ("</a>");
}

function varencode ($s)
{
 $s1 = $s;
 $r = "V";
 while ($s1 != "")
 {
  $r = $r . sprintf ("%02X", ord($s1));
  $s1 = substr ($s1, 1);
 }
 return $r;
}

function signif ($s)
{
 $s1 = strtolower ($s);
 $r = "";
 while ($s1 != "")
 {
  if (ord($s1) >= ord("a") && ord($s1) <= ord("z"))
   $r = $r . substr ($s1, 0, 1);
  $s1 = substr ($s1, 1);
 }
 return $r;
}

function valnum ($s1)
{
 $s = signif ($s1);
 if ($s == "o" || $s == "y" || $s == "oui" || $s == "yes") 
  return 1;
 if ($s == "n" || $s == "no" || $s == "non")
  return 0;
 if ($s == "unpeu")
  return 0.5;
 if ($s == "tres" || $s == "trs" || $s == "beaucoup")
  return 2;
 if ($s == "pasdutout" || $s == "aucontraire")
  return -1;
 /*
 $r = 0.0 + $s1;
 echo (" valnum $s = $r ");
 return $r;
 */ 
 return $s1;
}


?>

