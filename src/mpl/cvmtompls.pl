#!/usr/bin/perl

my $inf;
if ($ARGV[0]) { $inf = $ARGV[0]; }
else {
 print "Input file ? ";
 $inf = <STDIN>;
 chomp $inf;
}
open IN, $inf or die "$inf: $!\n";

my $outf;
if ($ARGV[1]) { $outf = $ARGV[1]; }
else {
 print "Output file ? ";
 $outf = <STDIN>;
 chomp $outf;
}
open OUT, ">$outf" or die "$outf: $!\n";

print OUT "[ZERO*1400] #6000H\n";
print OUT "

( hexadecimal digits )
h,[digits]W
{0123456789ABCDEF}

( coding of loading of R0 )
h,[code_load_R0]W
{[code_load_R0_ret]W
#23_
#1C,[load_value]R>,#F&,[digits]R+r_
#18,[load_value]R>,#F&,[digits]R+r_
#14,[load_value]R>,#F&,[digits]R+r_
#10,[load_value]R>,#F&,[digits]R+r_
#C,[load_value]R>,#F&,[digits]R+r_
#8,[load_value]R>,#F&,[digits]R+r_
#4,[load_value]R>,#F&,[digits]R+r_
[load_value]R,#F&,[digits]R+r_
#,[code_load_R0_ret]R?}

h,[START]W {
";

while (<IN>)
{
 chomp $_;
 print OUT "($_) " . " " x (20 - length $_);
 if (($_ =~ /^(.*)@(.*)$/) and not ($_ =~ /defword/))
 {
  $_ = "$1\$$2\$";
 }
 if ($_ =~ /^\s*rem (.*)$/)
 {
 }
 elsif ($_ =~ /^\s*:(.*)/)
 {
  print OUT ":$1:";
 }
 elsif ($_ =~ /^\s*res ([0-9A-F]*)/)
 {
  print OUT "}h,#$1+H{";
 }
 elsif ($_ =~ /^\s*align 4\s*$/)
 {
  print OUT "}h,#3+,[ADR]W#2,[ADR]R>H{";
 }
 elsif ($_ =~ /^\s*defword (.*)\s*$/)
 {
  print OUT "(defword $1)";

 }
 elsif ($_ =~ /^\s*r([012ABC])=r([012ABC])\s*$/)
 {
  print OUT "[R$2]R,[R$1]W";
 }
 elsif ($_ =~ /^\s*code r([01A])=\{r0\}\s*$/)
 {
  print OUT "[R0]R,[load_value]W#,[code_load_R0]R?{,[R$1]W}}{";
 }
 elsif ($_ =~ /^\s*r([012ABC])=\[r([012ABC])\]\s*$/)
 {
  print OUT "[R$2]RR,[R$1]W";
 }
 elsif ($_ =~ /^\s*r([012ABC])=byte\[r([012ABC])\]\s*$/)
 {
  print OUT "[R$2]Rr,[R$1]W";
 }
 elsif ($_ =~ /^\s*\[r([012ABC])\]=r([012ABC])\s*$/)
 {
  print OUT "[R$2]R,[R$1]RW";
 }
 elsif ($_ =~ /^\s*byte\[r([012ABC])\]=r([012ABC])\s*$/)
 {
  print OUT "[R$2]R,[R$1]Rw";
 }
 elsif ($_ =~ /^\s*r([012ABC])=\[r([012ABC])\+([0-9A-F]*)\]\s*$/)
 {
  print OUT "#$3,[R$2]R+R,[R$1]W";
 }
 elsif ($_ =~ /^\s*\[r([012ABC])\+([0-9A-F]*)\]=r([012ABC])\s*$/)
 {
  print OUT "#$2,[R$1]R+,[ADR]W[R$3]R,[ADR]RW";
 }
 elsif ($_ =~ /^\s*r([012ABC])=\[r([012ABC])\+\+\]\s*$/)
 { 
  print OUT "[R$2]RR,[R$1]W#4,[R$2]R+,[R$2]W";
 }
 elsif ($_ =~ /^\s*\[r([012ABC])\+\+\]=r([012ABC])\s*$/)
 { 
  print OUT "[R$2]R,[R$1]RW#4,[R$1]+,[R$1]W";
 }
 elsif ($_ =~ /^\s*\[--r([012ABC])\]=r([012ABC])\s*$/)
 {
  print OUT "#4,[R$1]R-,[R$1]W[R$2]R,[R$1]RW";
 } 
 elsif ($_ =~ /^\s*r0=r0(\+|\*|[-&])r1\s*$/)
 {
  print OUT "[R1]R,[R0]R$1,[R0]W";
 }
 elsif ($_ =~ /^\s*bz (.*)\s*$/)
 {
  print OUT "[R0]R,#$1?";
 }
 elsif ($_ =~ /^\s*call r([012ABC])\s*$/)
 {
  print OUT "#,[R$1]R?";
 }
 elsif ($_ =~ /^\s*call (.*)\s*$/)
 {
  print OUT "#,#$1?";
 }
 elsif ($_ =~ /^\s*getchar\s*$/)
 {
  print OUT "G,[R0]W";
 }
 elsif ($_ =~ /^\s*putchar\s*$/)
 {
  print OUT "[R0]RP";
 }
 elsif ($_ =~ /^\s*code\s*$/)
 {
  print OUT "[RC]RH{";
 }
 elsif ($_ =~ /^\s*endcode\s*$/)
 {
  print OUT "}}{h,[RC]W";
 }
 elsif ($_ =~ /^\s*beginf\s*$/)
 {
  print OUT "[ADR]W#4,[RS]R+,[RS]W[ADR],[RS]RW";
 }
 elsif ($_ =~ /^\s*endf\s*$/)
 {
  print OUT "[RS]RR,[ADR]W#4,[RS]R-,[RS]W#,[ADR]R?";
 }
 elsif ($_ =~ /^\s*setbradr\s*$/)
 {

 }
 elsif ($_ =~ /^\s*exit\s*$/)
 {
  print OUT "Q";
 }
 elsif ($_ =~ /^\s*r([012ABC])=(.*)\s*$/)
 {
  print OUT "#$2,[R$1]W";
 }
 elsif (not ($_ =~ /^\s*$/))
 {
  print OUT "(*** UNDEFINED INSTRUCTION ***)";
 }
 

 print OUT "\n";
}

print OUT "} #,[START]R?\n";

close IN;
close OUT;


