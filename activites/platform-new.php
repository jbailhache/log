<?php

 $platform = "log";

 function initv ($vars)
 {
  foreach ($vars as $var)
  {
   $instr = '$GLOBALS[\'' . $var . '\'] = $_POST[\'' .  $var . '\'];';
   echo "<li>eval [" . $instr . "]";
   eval ($instr);
  }
 }

 function connexion ()
 {
	global $cnx;
	try 
	{
    		// $cnx = new mysqli ("mysql.hostinger.fr", "u251290875_ps", "sapientia", "u251290875_ps");
		// $cnx = new PDO('sqlite:' . $path_to_root . '/db/cafes_philo_paris.sqlite');
		// $cnx = new PDO('sqlite:' . getcwd() . '/../db/cafes_philo_paris.sqlite');
		$cnx = new PDO('sqlite:activites.sqlite');
		if (! $cnx)
		{
			echo ("<p>Connexion à la base de données impossible<p>");
			return false;
		}
		return true;
	}
	catch (Exception $e)
	{	
		echo "<pre>";
		print_r($e);
	}
 }

 function query ($q)
 {
  global $cnx;
  //echo "<p>query $q.";
  try
  {
   //$r = $cnx->query ($q);
   $stmt = $cnx->prepare($q);
   if (!$stmt)
   {
    echo "<p>Erreur prepare $q</p>";
    echo "<p>"; 
    echo $cnx->errorCode();
    echo "<pre>";
    print_r($cnx->errorInfo());
    echo "</pre>";
    echo "</p>";
    return $stmt;
   }
   // echo "stmt=<pre>"; print_r($stmt); echo "</pre>";
   $status = $stmt->execute();
   if (!$status)
   {
    echo "<p>Erreur dans la requ&ecirc;te:<p>" . $q;
    echo $cnx->errorCode();
    echo "<pre>";
    print_r($cnx->errorInfo());
    echo "</pre>";
    echo "</p>";
    return $stmt;
   }
   return $stmt;
  }
  catch (Exception $e)
  {	
	echo "<pre>";
	print_r($e);
  }
 }

 function fetch_object ($data) 
 {
  global $cnx;
  if ($data)
   return $data->fetchObject ();
  else
   return 0;
 }

 function error ()
 {
  return $cnx->errorCode();
 }


