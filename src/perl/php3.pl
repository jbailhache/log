
$d = "c:\\phpdev\\www\\seg7";
 
sub traiter
{
 $f = shift;

 unlink "php3.tmp";
 rename $d."\\".$f, "php3.tmp";
 open IN, "php3.tmp";
 open OUT, "> ".$d."\\".$f;
 while (<IN>)
 {
  s/\.php/\.php3/g;
  print OUT;
 }
 close IN;
 close OUT;
}

opendir D, $d; 

$f = readdir D;
while ($f)
{
 print $f;
 $_ = $f;
 if (/\.php/ || /\.PHP/ || /\.htm/ || /\.HTM/)
 {
  print " - Fichier … traiter";
  traiter ($f);
 }
 $_ = $f;
 if (/.php$/)
 {
  print " - Fichier php";
  rename $d."\\".$f, $d."\\".$f."3";
 }
 print "\n";
 $f = readdir D;
}


