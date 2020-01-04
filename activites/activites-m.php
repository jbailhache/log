<html>
<head>
<title>Activités culturelles à Paris</title>
</head>
<body>
<br><br><br><br><br><br>

<h2>Activités culturelles à Paris</h3>

<?php
 include ("platform.php");

 $url = $_POST['url'];
 $descr = $_POST['descr'];
 $numero = $_GET['numero'];
 $visible = $_GET['visible'];

 if (connexion() > 0)
 {
  /* echo ("<br>Nom : [$nom]<br>URL : [$url]<br>Description : [$descr]"); */
  /* if ($descr != "") */
  if (strlen($descr) > 0)
  {
   /* echo ("descr non vide"); */
   $req = "INSERT INTO activites (url, descr, visible) VALUES ('$url', '$descr', 0)";
   $status = query ($req);
   if (!$status)
   {
    $er = error();
    echo ("<br>Erreur : Impossible d'ajouter le lien<br>Erreur : [$er]<br>Requete : [$req]");
   }
  }
  /* if ($numero != "") */
  if (strlen($numero) > 0)
  {
   $req = 'UPDATE activites SET visible = ' . $visible . ' WHERE numero = ' . $numero;
   /* echo '<p>' . $req; */
   $status = query ($req);
   if (!$status)
   {
    $er = error();
    echo ("Erreur : [$er]<br>Requete : [$req]");
   }

  } 
  /* else echo ("descr vide"); */
  $req = "SELECT * FROM activites";
  $data = query ($req);
  echo ("<p><h3>Activités et pages web ajoutées par les visiteurs de cette page :</h3>");
  echo ("<ul>");
  while ($act = fetch_object ($data))
  {
   echo ("<p><li>");
   echo ($act->descr);
   if ($act->url != "" && $act->url != "http://www.")
   {
    echo ("<br>Page web : <a href=\"");
    echo ($act->url);
    echo ("\">");
    echo ($act->url);
    echo ("</a>");
    echo ("<br>");
    if ($act->visible == 0)
    {
     echo ("<a href=\"activites-m.php?numero=");
     echo ($act->numero);
     echo ("&visible=1\">Rendre visible</a>");
    }
    else
    {
     echo ("<a href=\"activites-m.php?numero=");
     echo ($act->numero);
     echo ("&visible=0\">Rendre invisible</a>");
    }
   }
  }
  echo ("</ul>");
  
 }
 else echo ("Erreur connexion");
?>
<p>
<form method=post action=activites-m.php>

Si vous connaissez une autre activité ou une autre page web d'informations, vous pouvez l'ajouter ci-dessous :
<br>
Description :
<br>
<textarea name=descr rows=10 cols=60>
</textarea>
<br>
Page web : 
<input type=text name=url size=60 value="http://www.">
<br>
<input type=submit value="Enregistrer">
</form>

</body>
</html>
