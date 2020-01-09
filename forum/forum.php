<?php
 // $s = session_start();
 // require ("platform.php");
 // require ("util.php");
 // require ("config.php");
 require ("framework.php");

 $trace = 0;

 // echo "<pre>GET="; print_r($_GET); echo "\nPOST="; print_r($_POST); echo "</pre>";

 init();

 // echo "<p>forum=($forum) accueil=($accueil)</p>";

 if (connexion('..'))
 {
  if ($_GET['deconnexion'] == 'oui')
  {
   $_SESSION['moderateur'] = 'non';
  }

  $pass = $_POST['pass'];
  if ($pass == $passmod)
  {
   $_SESSION['moderateur'] = 'oui';
   head("Forum");
   echo "Identification correcte<br>";
  }
  else if (isset($_POST['pass']))
  {
   head("Forum");
   echo "Identification incorrecte<br>";
  }
  else
  {
   head("Forum");
  }

  if ($_SESSION['moderateur'] == 'oui')
  {
   echo "<h2>Acc&egrave;s mod&eacute;rateur</h2>";
  }

  $message = "";

  $forum2 = format_query($forum);

  if (isset($_POST['auteur']) && $_POST['auteur'] != '' && 
      isset($_POST['titre']) && $_POST['titre'] != '' &&
      isset($_POST['texte']) && $_POST['texte'] != '')
  { 
   if ($_POST['code1'] !== $_POST['code2'])
   {
    $message = $message_code_incorrect;
    $couleur = $couleur_fond_erreur;
    $auteur = stripslashes_if_mq($_POST['auteur']);
    $titre = stripslashes_if_mq($_POST['titre']);
    $texte = stripslashes_if_mq($_POST['texte']);
   }
   else
   {  
    // $message = "Discussion cr&eacute;&eacute;e.";
	$message = $texte_discussion_creee;
    $couleur = $couleur_fond_message;
	$auteur = "";
	$titre = "";
	$texte = "";
    // echo "Cr&eacute;ation discussion par " . $_POST['auteur'] . " titre: [" . $_POST['titre'] . "]  texte: <br>" . $_POST['texte'] . "<br>";
    // $query = "INSERT INTO forums (forum, titre, reponse, auteur, date, statut, texte) VALUES ('$forum', '" . format($_POST['titre']) . "', 0, '" . format($_POST['auteur']) . "', NOW(), 0, '" . format($_POST['texte']) . "')";
    $titre1 = $_POST['titre'];
    $titre2 = format($titre1);
    trace("titre1=$titre1 titre2=$titre2");
    $auteur1 = $_POST['auteur'];
    $auteur2 = format($auteur1);
    trace("auteur1=$auteur1 auteur2=$auteur2");
    $texte1 = $_POST['texte'];
    $texte2 = format($texte1);
    trace("texte1=$texte1 texte2=$texte2");
    $query = "INSERT INTO forums (forum, titre, reponse, auteur, ip, date, statut, texte) VALUES ('$forum2', '" . $titre2 . "', 0, '" . $auteur2 . "', '" . ip() . "', datetime('now'), 0, '" . $texte2 . "')";
    trace("query=$query");
    if ($trace) { echo "<br>$query<br>";  } 
    query ($query);
   }
  }

  if (isset($_GET['supprimer']) && is_numeric($_GET['supprimer'])) 
  {
   $query = "SELECT * FROM forums WHERE forum = '$forum2' AND numero = " . $_GET['supprimer'];
   if ($trace) { echo "<br>$query<br>";  }
   $data = query ($query);
   $rec = fetch_object ($data);
   if ($rec)
   {
    /* echo "<br>trouve<br>"; */
    if (($rec->statut & 4) == 0 || $_SESSION['moderateur'] == 'oui')
    {
     if ($_SESSION['moderateur'] == 'oui')
     {
      $statut = ($rec->statut & 1) | 2;
     }
     else
     {
      $statut = $rec->statut | 1;
     }
     $supprimepar = format($_POST['supprimepar']) . "@" . ip();
     $motif = format($_POST['motif']);
     $query = "UPDATE forums SET statut = $statut, supprimepar = '" . $supprimepar . "', motif = '" . $motif . "' WHERE forum = '" . $forum2 . "' AND numero = " . $_GET['supprimer'];
     if ($trace) { echo "<br>$query<br>"; }
     query ($query); 
    }
   }
  }

?>

<table>
<tr>
<td style="width:10%">
</td>
<td style="width:80%">

<h2>Forum</h2>

<p>
<a href="<?php echo $accueil; ?>">Retour &agrave; la page d'accueil</a>
</p>

<div style="line-height: 2em;">

<h3><?php echo $texte_creer_discussion; ?></h3>
<?php
 echo '<div style="background:'.$couleur.';">';
 echo $message;
 echo '</div>';
?>
<p></p>
<form method=post action=forum.php?forum=<?php echo $forum; ?>&accueil=<?php echo $accueil; ?>>
<table><tr><td>
Votre adresse IP : </td><td> <?php echo ip(); ?>
</td></tr><tr><td>
Votre nom : </td><td> <input type=text name=auteur size=60 value="<?php echo $auteur; ?>"><br>
</td></tr><tr><td>
<?php echo $texte_titre_discussion; ?></td><td> <input type=text name=titre size=60 value="<?php echo $titre; ?>"><br>
</td></tr></table>
<?php echo $texte_presentation_discussion; ?><br>
<textarea name=texte rows=20 cols=120>
<?php echo $texte; ?>
</textarea>
<br>
<?php captcha() ?>
<br>
<input type=submit value="<?php echo $texte_bouton_creer_discussion; ?>">
</form>

<h3><?php echo $texte_discussions; ?></h3>
<table border=3 cellspacing=3 cellpadding=6>
<tr>
<td>Date</td>
<td>Auteur</td>
<td>Titre</td>
<?php
 if ($_SESSION['moderateur'] == 'oui')
 {
  echo "<td>Statut</td>";
 }
?>
</tr>
<?php

 $query = "SELECT * FROM forums WHERE forum = '$forum2' AND reponse = 0 ORDER BY numero DESC";
 $data = query ($query);
 while ($rec = fetch_object($data))
 {
  if (affichable ($rec))
  {
   echo "<tr><td>" . format_date($rec->date) . "</td><td>" . $rec->auteur . "</td><td><a href=discussion.php?forum=$forum&accueil=$accueil&discussion=" . $rec->numero . ">" . $rec->titre . "</a></td>";
/*
   echo "<td>";
   if (($rec->statut & 4) == 0)
   {
    echo "<a href=forum.php?forum=$forum&accueil=$accueil&supprimer=" . $rec->numero . ">Supprimer</a>";
   }
   echo "</td>";
*/
   if ($_SESSION['moderateur'] == 'oui')
   {
    echo "<td>" . libelle_statut($rec->statut) . "</td>";
   }
   echo "</tr>"; 
  }
 }
}

?>
</table>

<br>
<?php
 if ($_SESSION['moderateur'] != 'oui')
 {
  echo "<a href=moderateur.php?forum=$forum&accueil=$accueil>Acc&egrave;s mod&eacute;rateur</a>";
 }
 else
 {
  echo "<a href=forum.php?forum=$forum&accueil=$accueil&deconnexion=oui>D&eacute;connexion mod&eacute;rateur</a>";
 }
?>

</div>

</td>
<td style="width:10%">
</td>
</table>

</body>
</html>
