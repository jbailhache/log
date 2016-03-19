<html>
<body>

<br><br><br><br><br><br><br><br><br><br><br><br><br><br><br><br>

<?php

 function decode ($in)
 {
  $out = "";
  for ($i=0; $i<strlen($in)/2;$i++)
  {
   $code=ord(substr($in,2*$i,1))-0x40+0x10*(ord(substr($in,2*$i+1,1))-0x40);
   $out=$out.chr($code);
  }
  return $out;
}

 $f=fopen($_GET["filename"],"ab");
 echo "data=[".$_GET["data"]."]<br>";
 $dec=decode($_GET["data"]);
 echo "dec=[".$dec."] longueur=" . strlen($dec) . "<br>";
 $n=fwrite($f,$dec);
 fclose($f);

  echo "<p>Contenu du fichier:<br>";
  $fp = fopen ($_GET['filename'], 'r');
  if (!$fp)
  {
   echo "Impossible d'ouvrir le fichier [$fichier]";
  }
  else
  {
   while (!feof($fp))
   {
    $s = fgets ($fp, 256);
    echo $s;
    /*echo str_replace ('</text_area>', '</text_area>', $s);*/
   }
  }
  echo "<br>Fin du fichier";
  fclose($fp);
  

/*
 echo "Données reçues: [" . $_GET["data"] . "]";
 $f = fopen ("data.dat", "wb");
 echo "<br>f=$f";
 $n = fwrite ($f, $_GET["data"]);
 echo "<br>n=$n";
*/
?>

</body>
</html>
