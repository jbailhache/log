<HTML>
<head>
<title>Recherche de partenaire</title>
</head>
<body>
<h3>Recherche de partenaire</h3>
<?php
 include ("util.php");
 include ("utilaf.php");
 echo ("<p>Profils $actiont triés par ordre d'affinité pour $but décroissant par rapport à $action $pseudo $profil : <p>");
 $max = 20;
 $n = 0;
 $query = "SELECT * FROM afprofil WHERE action = '$actiont'";
 $data = mysql_query ($query);
 while ($rec = mysql_fetch_object ($data))
 {
  /*echo ("<p>$rec->action $rec->pseudo $rec->profil : ");*/
  $af = affinite ($but, $action, $pseudo, $profil, $rec->action, $rec->pseudo, $rec->profil);
  /*echo ($af);*/
  
  for ($i=0; $i<$n; $i++)
  {
   if ($af > $tab[$i]["af"])
    break;
  }
  for ($j=$n-1; $j>=$i; $j--)
  {
   $tab[$j+1] = $tab[$j];
   /*$af[$j+1] = $af[$j];*/
   /*$af[$j+1] = array ("af"=>$af[$j]["af"], "action"=>$af[$j]["action"], "pseudo"=>$af[$j]["pseudo"], "profil"=>$af[$j]["profil"]);*/
  }
  $tab[$i] = array ("af"=>$af, "action"=>$rec->action, "pseudo"=>$rec->pseudo, "profil"=>$rec->profil);
  $n++;
  if ($n > $max)
   $n = $max;
 }
 echo ("<table border=1><tr><td>Affinité</td><td>Profil</td></tr>");
 for ($i=0; $i<=$max; $i++)
 {
   if ($tab[$i]["action"] == "")
    break;
   echo ("<tr><td>");
   echo ($tab[$i]["af"]);
   echo ("</td><td>");
   echo ($tab[$i]["action"]);
   echo (" de ");
   echo ($tab[$i]["pseudo"]);
   echo (" ");
   echo ($tab[$i]["profil"]);
   echo ("</td></tr>");
 }
 echo ("</table>");
  
 
?>
<p><a href="http://teledev.multimania.com/log/"><b>log</b></a> - 
Ne passez pas à côté des choses compliquées !
</body>
</HTML>
