<?php
 try {
  $pdo = new PDO('sqlite:forums.sqlite');
  // $pdo->query("DROP TABLE forums");
  $status = $pdo->query("CREATE TABLE IF NOT EXISTS forums (
  numero integer primary key autoincrement,
  forum text  DEFAULT NULL,
  discussion int(11) DEFAULT NULL,
  titre text  DEFAULT NULL,
  reponse int(11) DEFAULT NULL,
  auteur text  DEFAULT NULL,
  ip text DEFAULT NULL,
  date datetime DEFAULT NULL,
  dat datetime DEFAULT NULL,
  statut int(11) DEFAULT NULL,
  texte text  DEFAULT NULL,
  supprimepar text NULL,
  motif text NULL
)");
  if ($status) 
  {
   echo "<p>Table creee</p>";
  }
  else
  {
   echo "<p>Erreur creation table<br><pre>";
   print_r($pdo->errorInfo());
  }
 } catch (Exception $e) { 
  echo "<pre>"; print_r($e); 
 }
?>


