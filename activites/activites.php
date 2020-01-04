<html>
<head>
<title>Activités philosophiques et culturelles à Paris</title>
<!--meta http-equiv="refresh" content="1; url="http://philosophie.olympe-network.com/activites/" /-->


</head>
<body>
<br><br><br><br><br><br>

<h2>Activités philosophiques et culturelles à Paris</h2>

<p>

<h2>Cette page n'est plus mise à jour. La nouvelle adresse est : 
<a href="http://philosophie.olympe-network.com/activites/">http://philosophie.olympe-network.com/activites/</a>
</h2>

<p>

<h3>Agenda</h3>

<form method=post action=agenda.php>
<p>
Début dans <input type=text name=debut> jours
<br>
Fin dans <input type=text name=fin> jours
<br>
<input type=submit value="Afficher">
</form>

<p> (*) Activités périodiques affichées automatiquement en fonction du calendrier. 

<?php
 
 include ('platform.php');
 include ('afagenda.php');

 $debut = $_POST['debut'];
 $fin = $_POST['fin'];
 
 if (strlen($debut)==0)
  $debut = -1;
 if (strlen($fin)==0)
  $fin = 8;
 afagenda ($debut,$fin);

?>

<h3>Cafés philo et autres débats périodiques</h3>

<ul>
<li> Lundi de 18h à 20h : café philo à la brasserie "Les Patios", 58 bd Saint Michel
<li> Mardi de 18h à 20h : café philo à la Contrescarpe, place de la Contrescarpe 
- <a href="http://philosophie.olympe-network.com/activites/contrescarpe.php">site web</a>
<!-- <li> 1er et 2ème mardi du mois à 20h30 : café philo au Génie, 4 bd Beaumarchais - <a href=http://www.rei-filos.fr/>site web</a> -->
<li> Mercredi à 18h30 : café philo au Mandarin Sorbonne, 18 rue Cujas
<li> Mercredi à 20h30 : café Créa au Lady's, 138 rue du Faubourg Saint Antoine 
<li> 1er mercredi du mois à 19h15 : café philo à  La Rotonde de la Muette 12 rue chaussée de la Muette. - <a href="http://www.accordphilo.com/"> site web </a>
<li> 2ème et 4ème mardis du mois à 20h : café philo au Dupont café, 84 avenue de France  - <a href="http://www.accordphilo.com/"> site web </a>

<li> 3ème mercredi du mois à 20h : café psy au Papille, 9 rue Godefroy Cavaignac - <a href="http://cafe-psy.over-blog.com/"> site web </a>

<li> Jeudi de 19h à 21h: café psycho au Bastille, place de la Bastille
<li> 1er jeudi du mois à 18h30 : café juridique au Luxembourg, 58 bd Saint Michel
<li> Vendredi de 19h30 à 21h30 : café socio à la brasserie "Les Patios", place de la Sorbonne
<li> 2ème vendredi du mois à 19h30 : café débat NDI aux Délices Royales, 
 43 rue Saint Antoine
<li> Samedi de 16h30 à 18h30 : café philo au Phare du Canal, 32 rue du Faubourg du Temple
<li> 3ème samedi du mois à 15h : café communication, exercices, jeux de rôles 
 au Falstaff, place de la Bastille 
<li> Dimanche de 11h à 13h : café philo au café des Phares, place de la bastille 
- <a href="http://www.cafe-philo-des-phares.info/">site web</a>
<li> Dimanche de 11h à 13h : café philo au Bastille, place de la Bastille
- <a href="http://cafephilobastille.exprimetoi.net/">site web</a>
<li> 1er dimanche du mois à 15h : café philo au Falstaff, place de la Bastille
<!-- - <a href="http://pagesperso-orange.fr/mfturpaud/Cafe_philo.htm">site web</a> -->
<li> Dimanche de 18h à 20h30 : café philo au restaurant "Via Italia", 21 avenue d'Italie
- <a href="http://www.accordphilo.com/">site web</a>
<li> D'autres débats périodiques sont indiqués dans les sites ci dessous.
</ul>

<p>
<h3>Sites des cafés philo et autres activités culturelles</h3>
<ul>
<li><a href="http://www.cafe-philo-des-phares.info/">Café des Phares</a> Place de la Bastille
<li><a href="http://cafephilobastille.exprimetoi.net/">Bastille</a> 
<li><a href="http://www.accordphilo.com/">Accord Philo</a>
<li><a href="http://membres.lycos.fr/teledev/contrescarpe.php">Café philo de la Contrescarpe</a>
<li><a href="http://cafeidees.free.fr/accueil.php">Café des idées</a>
<!--
<li>Honoré Philo : <a href=http://marielle75.orangeblog.fr>Blog de Marielle</a>
    <a href="http://pagesperso-orange.fr/mfturpaud/Cafe_philo.htm">Ancien site</a>
-->
<li><a href="http://www.rencontres-et-debats-autrement.fr/">Autrement</a>
<!-- <li><a href="http://activitesalain.ifrance.com/">Activités d'Alain</a> -->
<li><a href="http://cafe-psy.over-blog.com/">café-psy</a>
<li><a href="http://rei-filos.fr">REI Filos</a>
 - <a href="http://www.rei-films.com/REI-FILMS/index_rei-films.html">REI films</a>
<li><a href="http://www.philomag.com/list-agenda.php">Philomag</a>
<!--
<li><a href="http://www.philosophie-en-france.net/">Philosophie-en-France</a>
 - <a href="http://www.philosophie-en-france.net/agenda.htm">Evènements</a>
-->
<li><a href="http://www.conferencesetdebats.fr/">Conférences et débats</a>
<li><a href="http://www.cine-philo.fr">Ciné philo MK2 Bibliothèque</a> 
<li><a href="http://www.ciph.org/fichiers_programme/ciph_programmeHebdo.pdf">Collège international de philosophie</a>
<li><a href=http://www.bnf.fr/pages/zNavigat/frame/cultpubl.htm>BNF</a>
<li><a href=http://www.ens.fr/actualites/>Ecole normale supérieure</a>
<li><a href="http://www.bardessciences.net/index.php?option=com_content&task=view&id=51&Itemid=38">Bar des sciences</a>
Au Viaduc Café, 
43 avenue Daumesnil 75012 Paris

<li><a href="http://www.lentrepot.fr/">L'Entrepôt</a>
7 / 9 rue Francis de Pressensé - 
75014 Paris

<li><a href="http://www.arts-et-metiers.net/musee.php?P=28&lang=fra&flash=f">Arts et métiers</a>
60 rue Réaumur
75003 Paris

<li><a href="http://www.forum104.org/">Forum 104</a> 104 rue de Vaugirard
</ul>

<p>
<h3><a href=cafes-rencontres.pdf>Cafés-rencontres</a></h3>

<p>
<h3>Sites de participants des cafés philo</h3>
<ul>
<li><a href="http://attila-jean-jacques.net/">Attila</a>
<li><a href="http://rjondeau.canalblog.com/">conversation</a>
<li><a href="http://inquietanteetrangete.blog.20minutes.fr/">Inquiétant Indfrisable</a>
<li><a href="http://www.chez.com/log/">log</a>
<li><a href="http://philosophie.olympe-network.com/reflexions">Réflexions</a>
</ul>
<p>
<h3>Sorties diverses, rencontres</h3>
<ul>
<li><a href="http://www.peuplade.fr/index.php?idr=10">Peuplade</a>
<li><a href="http://paris.onvasortir.com/vue_sortie_all.php">On va sortir</a>
<li><a href="http://www.amiez.org/Login/Acces_reserve.php?PageRetour=/Activites/Futures_Sorties.php?">Amiez</a>
<li><a href="http://www.kikepartant.com/">Kikepartant</a>
<li><a href="http://www.allons-sortir.fr/">Allons sortir</a>
</ul>

<?php
 /*include ("platform.php");*/
 
 $url = $_POST['url'];
 $descr = $_POST['descr'];

 if (connexion() > 0)
 {
  /* echo ("<br>Nom : [$nom]<br>URL : [$url]<br>Description : [$descr]"); */
  /* if ($descr != "") */
  /* if (strcmp ($descr, '') != 0) */
  if (strlen ($descr) > 0)
  {
   /* echo ("descr non vide"); */
   $req = "INSERT INTO activites (url, descr, visible) VALUES ('$url', '$descr', 0)";
   $status = query ($req);
   /* echo '<p>' . $req; */
   if (!$status)
   {
    /* $er = mysql_error(); */
    echo ("<br>Erreur : Impossible d'ajouter le lien<br>Erreur : Requete : [$req]");
   }
  } 
  /* else echo ("descr vide"); */
  $req = "SELECT * FROM activites";
  $data = query ($req);
  echo ("<p><h3>Activités et pages web ajoutées par les visiteurs de cette page :</h3>");
  echo ("<ul>");
  while ($act = fetch_object ($data))
  {
   if ($act->visible == 1)
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
    }
   }
  }
  echo ("</ul>");
  
 }
 else echo ("Erreur connexion");
?>
<p>
<form method=post action=index.php>

Si vous connaissez une autre activité ou une autre page web d'informations, vous pouvez la proposer ci-dessous :
<br>
Description :
<br>
<textarea name=descr rows=10 cols=60>
</textarea>
<br>
Page web : 
<input type=text name=url size=60 value="http://www.">
<br>
<input type=submit value="Envoyer">
</form>

</body>
</html>
