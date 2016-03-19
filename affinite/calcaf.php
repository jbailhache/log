<HTML>
<head>
<title>Calcul d'affinité</title>
</head>
<body>
<h3>Calcul d'affinité entre deux profils</h3>
<?php
 include ("util.php");
 include ("utilaf.php");
 echo ("<ul><li>But : $but");
 echo ("<li> Premier profil : $action1 de $pseudo1 $profil1");
 echo ("<li> Second profil  : $action2 de $pseudo2 $profil2</ul>");
 $af = affinite ($but, $action1, $pseudo1, $profil1, $action2, $pseudo2, $profil2);
 echo ("<p>Coefficient d'affinité : $af.");
?>
<p>
<a href=index.php>Retour au sommaire</a>
<p>
<b><a href="http://teledev.multimania.com/log/">log</a></b> Ne passez pas à côté des choses compliquées !

</body>
</HTML>

