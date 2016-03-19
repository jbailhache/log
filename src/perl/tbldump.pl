
open IN, "c:/log/tbl_dump.txt";
open OUT, "> c:/log/tbl_dump.php";

print OUT "<?php\n";

while (<IN>)
{
 if (/INSERT/)
 {
  s/\n//;
  print OUT " query (\"$_\");\n";
 }
}

print OUT "?>\n";

