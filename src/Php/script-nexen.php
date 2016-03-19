<!--	script-nexen.php,v 1.5 2001/05/30 13:02:01 pierre Exp	-->
<!--	db.inc,v 1.238 2001/05/30 15:43:51 pierre Exp	-->

<?php
/*
 *
 *
 * Avertissement : Cette librairie de fonctions PHP est distribuee avec l'espoir 
 * qu'elle sera utile, mais elle l'est SANS AUCUNE GARANTIE; sans meme la garantie de 
 * COMMERCIALISATION ou d'UTILITE POUR UN BUT QUELCONQUE.
 * Elle est librement redistribuable tant que la presente licence, ainsi que les credits des 
 * auteurs respectifs de chaque fonctions sont laisses ensembles. 
 * En aucun cas, Nexen.net ne pourra etre tenu responsable de quelques consequences que ce soit
 * de l'utilisation ou la mesutilisation de ces fonctions PHP.

*/

/****
 * Titre : Camembert statistique 
 * Auteur : Cedric Fronteau 
 * Email : charlie@nexen.net
 * Url : 
 * Description : Représente les valeurs d'un tableau sous forme de camembert.

****/
function draw_camembert($stats,$x,$y,$margin,$filename){
/* Parametres : 
   *     $stats : tableau de valeurs
   *     $x : largeur de l'image à créer
   *     $y : hauteur de l'image à créer
   *     $margin : marge entre le bord, le cercle et la legende
   *     $filename : nom à donner à de l'image
   */
   if (!is_array($stats) || ($x <= 0) || ($y <= 0))
      return FALSE;
   $vals = array_values($stats);
   $keys = array_keys($stats);
   
   $pic = imagecreate($x,$y);
   $min = min($vals);
   $max = max($vals);
   $taille = sizeof($vals);
   
   $white = imagecolorallocate($pic,255,255,255);
   $black = imagecolorallocate($pic,0,0,0);
   
   /* remplit le fond de l'image */
   imagefill($pic,0,0,$white);
   
   $total = somme($vals);
   $diametre = min($y,2/3*$x) - 2*$margin;
   $cx = floor($diametre/2) + $margin;
   $cy = floor($diametre/2) + $margin;
   srand((double)microtime()*1000000);
  
   /* cadre de la legende */
   $legendeW = $x - $diametre - 2*$margin;
   $legendeH = min(20 + (20 + 15)*$taille,$y-$margin);
   $nbmaxcar = floor(($legendeW-35)/8) - 1;
   imagerectangle($pic,
                  $x - $legendeW,$y - $legendeH,
                  $x - $margin,$y - $margin,
                  $black);
   /* dessin du graphe et de la legende */    
   $legende_y = $y - $margin - 20;
   $debut = 270;
   for($i=0;$i<$taille;$i++)
      {
      $r = rand(1,254);
      $g = rand(1,254);
      $b = rand(1,254);
      $col = imagecolorallocate($pic,$r,$g,$b);
      
      $fin = round($debut + $vals[$i]/$total*360);
      imagearc($pic,$cx,$cy,$diametre,$diametre,$debut,$fin,$col);

      $tx1 = round(cos(deg2rad($debut))*$diametre/2+$cx);
      $ty1 = round(sin(deg2rad($debut))*$diametre/2+$cy);
      $tx2 = round(cos(deg2rad($fin))*$diametre/2+$cx);
      $ty2 = round(sin(deg2rad($fin))*$diametre/2+$cy);
      
      imageline($pic,$cx,$cy,$tx1,$ty1,$col);
      imageline($pic,$cx,$cy,$tx2,$ty2,$col);

      $mid = floor(($fin - $debut)/2 + $debut);
      $px = round(cos(deg2rad($mid))*($diametre/2-$margin)+$cx);
      $py = round(sin(deg2rad($mid))*($diametre/2-$margin)+$cy);

      imagefilltoborder($pic,$px,$py,$col,$col);
      imagestring($pic,2,$px,$py,$vals[$i],$black);

      $debut = $fin;
      
      if ($legende_y >= ($y - $legendeH))
         {
         $xb = floor($x - $legendeW +10);
         $yb = floor($legende_y);
         $xe = floor($x - $legendeW +25);
         $ye = floor($legende_y + 15);
         imagefilledrectangle($pic,
                     $xb,$yb,
                     $xe,$ye,
                     $col);
      
         if (is_string($keys[$i]))
            {
            if (strlen($keys[$i]) > $nbmaxcar)
               $str = substr($keys[$i],0,$nbmaxcar - 1);
            else
               $str = $keys[$i];
            }
         else
            {
            if ($nbmaxcar > 8)
               $str = "Clé : ".$keys[$i];
            else
               $str = $keys[$i];
            }
         imagestring($pic,2,$x - $legendeW +35,$legende_y,$str,$black);
         }
      $legende_y -= (20+15);
      }
   imagearc($pic,$cx,$cy,$diametre,$diametre,0,360,$black);
   imagearc($pic,$cx,$cy,$diametre+7,$diametre+7,0,360,$black);
   imagefilltoborder($pic,$cx+($diametre+3)/2,$cy,$black,$black);
      
   @imagepng($pic,$filename);
   if (!file_exists($filename))
      return FALSE;
   return TRUE;
}

/****
 * Titre : Somme 
 * Auteur : Damien Seguy 
 * Email : damien.seguy@nexen.net
 * Url : www.nexen.net/
 * Description : Somme tous les arguments.
****/
function somme(){
$args = func_get_args();
        $elements = array_pop($args);
        foreach ($args as $a) {
                $elements = array_merge($elements, $a);
        }
        $b = 0;
        @eval('$b = intval("'.implode('") + intval("', $elements).'");');
        return $b;
}


?>