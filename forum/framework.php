<?php
 $s = session_start();

 require ("platform.php");
 require ("util.php");
 require ("config.php");

 function head($titre)
 {
  echo "<html>
<head>
<meta http-equiv=\"Content-Type\" content=\"text/html; charset=ISO-8859-1\" />
<title>$titre</title>
</head>
<body background=\"texture_2.jpg\">
";
 }

 function init()
 {
  global $forum, $accueil;
  if (isset($_GET['forum']))
  {
   $forum = $_GET['forum'];
  }
  elseif (isset($_POST['forum']))
  {
   $forum = $_POST['forum'];
  }

  if (isset($_GET['accueil'])) 
  { 
   $accueil = $_GET['accueil']; 
  } 
  elseif (isset($_POST['accueil']))
  {
   $accueil = $_POST['accueil'];
  }
 }

 function captcha()
 {
  ?>
Recopiez ce code dans la case ci-contre pour prouver que vous n'&ecirc;tes pas un robot : 
<?php
 $s = "";
 for ($i=0; $i<6; $i++)
 {
  $s = $s . chr(65+rand(0,25));
 }
 echo $s;
?>
<input type=hidden name=code1 value="<?php echo $s; ?>">
<input type=text name=code2 size=7>
  <?php
 }

 function libelle_statut ($statut)
 {
  if ($statut == 0) return "post&eacute;";
  if ($statut == 1) return "supprim&eacute; par un visiteur";
  if ($statut == 2) return "supprim&eacute; par un mod&eacute;rateur";
  if ($statut == 3) return "supprim&eacute par un visiteur et par un mod&eacute;rateur";
  if ($statut == 4) return "valid&eacute;";
  if ($statut == 5) return "valid&eacute; apr&egrave;s suppression";
  return $statut;
 }

?>
