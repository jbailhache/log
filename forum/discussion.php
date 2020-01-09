<?php
 //include ("platform.php");
 //include ("util.php");
 //include ("config.php");
 require ("framework.php");

 head("Forum");
?>

<a href=forum.php?forum=<?php echo $forum; ?>&accueil=<?php echo $accueil; ?>>Retour</a>

<?php
 if ($_SESSION['moderateur'] == 'oui')
 {
  echo "<h2>Acc&egrave;s mod&eacute;rateur</h2>";
 }
?>

<!--
<h2>Discussion</h2>
-->

<?php
 
 connexion('..');

 $trace = 0;

 init();

 $discussion = num($_GET['discussion']);
 /* echo "<br>discussion=$discussion<br>"; */

 /* echo "<br>mod&eacute;rateur = [" . $_SESSION['moderateur'] . "]<br>"; */

 $query = "SELECT * FROM forums WHERE forum = '" . format_query($forum) . "' AND numero = $discussion AND reponse = 0";
 if ($trace) { echo "<br>$query<br>";  } 
 $data = query ($query);
 $rec = fetch_object ($data);

 if (isset($_GET['supprimer']) && is_numeric($_GET['supprimer']) && (($rec->statut & 4) == 0 || $_SESSION['moderateur'] == 'oui'))
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
   $query = "UPDATE forums SET statut = $statut, supprimepar = '" . $supprimepar . "', motif = '" . $motif . "' WHERE forum = '" . format_query($forum) . "' AND numero = " . $_GET['supprimer'];
   if ($trace) { echo "<br>$query<br>";  }
   query ($query);
 }

 if (isset($_GET['valider']) && is_numeric($_GET['valider']) && $_SESSION['moderateur'] == 'oui')
 {
   $statut = ($rec->statut & 1) | 4; 
   $query = "UPDATE forums SET statut = $statut WHERE forum = '" . format_query($forum) . "' AND numero = " . $_GET['valider'];
   if ($trace) { echo "<br>$query<br>"; }
   query ($query);
 }

 $query = "SELECT * FROM forums WHERE forum = '" . format_query($forum) . "' AND numero = $discussion AND reponse = 0";
 $data = query ($query);
 $rec = fetch_object ($data);
 if (affichable ($rec))
 {
  echo "<h2>" . $texte_entete_discussion . $rec->titre . "</h2>";
  echo '<div style="background:'.$couleur_fond_entete.';">';
  echo "Auteur : " . $rec->auteur . " ; Adresse IP : " . $rec->ip . "<br>Date : " . format_date($rec->date);
  if ($_SESSION['moderateur'] == 'oui')
  {
   echo "<br>Statut : " . libelle_statut($rec->statut);
   if ($rec->statut == 1)
   {
    echo "<br>Supprim&eacute; par " . $rec->supprimepar . ", motif : " . $rec->motif;
   }
  }
  if (($rec->statut & 4) == 0 || $_SESSION['moderateur'] == 'oui')
  {
   /* echo "<br><a href=forum.php?forum=$forum&accueil=$accueil&supprimer=" . $rec->numero . ">Supprimer</a>"; */
   echo "
    <form method=post action=forum.php?forum=$forum&accueil=$accueil&supprimer=" . $rec->numero . ">
    <br>Supprimer ce message : 
    <br>Votre nom : <input type=text name=supprimepar size=40> 
    <br>Motif : <input type=text name=motif size=60> 
    <br><input type=submit value=Supprimer>
    </form>
   ";
  }
  if ($_SESSION['moderateur'] == 'oui')
  {
   echo " - <a href=discussion.php?forum=$forum&accueil=$accueil&discussion=" . $discussion . "&valider=" . $rec->numero . ">Valider</a>";
  }
  echo "</div>";
  echo "<br>" . $rec->texte . "<br><br>";
  echo "<br>";

  $auteur = "";
  $texte = "";
  if (isset($_POST['auteur']))
  {
   if ($_POST['code1'] !== $_POST['code2'])
   {
    $message = $message_code_incorrect;
    $couleur = $couleur_fond_erreur;
    $auteur = stripslashes_if_mq($_POST['auteur']);
    $texte = stripslashes_if_mq($_POST['texte']);
   }
   else
   {
    $message = "R&eacute;ponse envoy&eacute;e.";
    $couleur = $couleur_fond_message;
   	$query = "INSERT INTO forums (forum, titre, discussion, reponse, auteur, ip, date, statut, texte) VALUES ('" . format_query($forum) . "', '" . format_query($rec->titre) . "', " .  $rec->numero . ", 1, '" . format($_POST['auteur']) .  "', '" . ip() ."', datetime('now'), 0, '" . format($_POST['texte']) . "')";
    if ($trace) { echo "<br>$query<br>";  }
    query ($query);
   }
  }
 }
?>

<?php
 echo '<div style="background:'.$couleur.';">';
 echo $message;
 echo '</div>';
?>

<h3>R&eacute;pondre</h3>
<form method=post action=discussion.php?forum=<?php echo $forum; ?>&accueil=<?php echo $accueil; ?>&discussion=<?php echo $discussion; ?>>
Votre nom : <input type=text name=auteur size=50 value="<?php echo $auteur; ?>">
Votre adresse IP : <?php echo ip(); ?> <br>
Votre r&eacute;ponse : <br>
<textarea name=texte rows=20 cols=120>
<?php echo $texte; ?>
</textarea>
<br>
<?php captcha(); ?>
<br>
<input type=submit value="Envoyer">
</form>


<h3>R&eacute;ponses</h3>

<?php
 $discussion = $_GET['discussion'];
 /* echo "<br>(2) discussion=$discussion<br>"; */
 if (affichable ($rec))
 {
  $query = "SELECT * FROM forums WHERE forum = '" . format_query($forum) . "' AND discussion = $discussion AND reponse = 1 ORDER BY numero DESC";
  $data = query ($query);
  while ($rec = fetch_object($data))
  {
   if (affichable ($rec))
   {
    echo "<br>";
    echo '<div style="background:'.$couleur_fond_entete.';">';
    echo "Auteur : " . $rec->auteur . " ; Adresse IP : " . $rec->ip . "<br>Date : " . $rec->date;
    if ($_SESSION['moderateur'] == 'oui')
    {
     echo "<br>Statut : " . libelle_statut($rec->statut);
     if ($rec->statut == 1)
     {
      echo "<br>Supprim&eacute; par " . $rec->supprimepar . ", motif : " . $rec->motif;
     }
    }
    if (($rec->statut & 4) == 0 || $_SESSION['moderateur'] == 'oui')
    {
     /* echo "<br><a href=discussion.php?forum=$forum&accueil=$accueil&discussion=$discussion&supprimer=" . $rec->numero . ">Supprimer</a>"; */
     echo "
    <form method=post action=forum.php?forum=$forum&accueil=$accueil&supprimer=" . $rec->numero . ">
    <br><?php echo $texte_supprimer; ?>
    <br>Votre nom : <input type=text name=supprimepar size=40> 
    <br>Motif : <input type=text name=motif size=60> 
    <br><input type=submit value=Supprimer>
    </form>
    ";
    }
    if ($_SESSION['moderateur'] == 'oui')
    {
     echo " - <a href=discussion.php?forum=$forum&accueil=$accueil&discussion=" . $discussion . "&valider=" . $rec->numero . ">Valider</a>";
    }

    echo "</div>";
    echo "<br>" . $rec->texte . "<br><br>";
    echo "<br>";
   }
  }
 }
 
?>

<br>
<a href=forum.php?forum=<?php echo $forum; ?>&accueil=<?php echo $accueil; ?>>Retour</a>

</body>
</html>
