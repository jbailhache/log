#!/usr/local/bin/perl
#
# Author name:
# Creation date:
#
# Description:
#

 require TripodMail;
 use TripodCGI;
 use TripodDate;

 $cgi = new TripodCGI;
 $texte = $cgi->param('texte');    

 $dat = new TripodDate;
   
 print ("Content-type: text/html\n\n");
 print ("<h1>log</h1><p>");
 print ("Ne passons pas à côté des choses compliquées !");
 print ("<p>Ce site est en construction, en attendant visitez <a href=http://www.chez.com/log/>mon autre site</a>.");
 print ("<p>This site is under construction. Meanwhile, visit <a href=http://www.chez.com/log/>my other site</a>.");

 print "<p>Essais Perl :<p>";
 @l = (1, 2, 3, 4, 5, 6);
 @l1 = map 10*$_ . " ", @l;
 print @l1;
 print "<p>";

 print "Nous sommes le ", $dat->currentDate(), "<p>";

 system ("ls >dir.txt");
 open (SF, "dir.txt");
  
 open (OUT, ">f.txt");
 print OUT "Bonjour";
 close OUT;
 open (IN, "f.txt");
 print while <IN>;

 print ("<form action=script65.pl method=get>
 Tapez un texte : 
 <input type=text name=texte>
 <input type=submit value=Envoyer>
 </form>");

 print "<p>Vous avez tapé : ";
 print $texte;
 print "<p>Données reçues :";
 print @array;
 
 open (MT,">mail.txt");
 print MT "To: log\@chez.com\n";
 print MT "From: Tripod\n";
 print MT "Subject: Test Perl\n";
 print MT "\n";
 print MT "Hello from Perl !\n";
 print MT " bye\n";
 close MT;

$MAIL = new TripodMail;
$mail_template = "mail.txt";
%variables = ('email' => 'log@chez.com',
'name' => 'jacques',
'number' => '1');
$MAIL->sendMail($mail_template, \%variables);

 print "<p>Voici le source de ce script : <pre>";
 open F,$0;
 print while <F>;
 close F;
 print "</pre>Fin du source";

 $x = 120;
 $s = "$x = 140;";
 eval $s;
 print "<p>",$x,"\n";
 $s = q/print "<p>Bonjour !\n";/;
 eval $s;

sub mymap 
{
 my ($t,$f) = @_;
 my $n = @t;
 my $i;
 my @r;
 print "<p>mymap:@_=",@_,", @$t=",@$t,", $f=",$f,"\n";
 for ($i=0; $i<$n; $i++)
 {
  @r[$i] = &$f(@$t[$i]);
 } 
 return @r;
}

sub f
{
 return @_[0] * 10;
}

@t = (4,5,6);

print "<p>Bonjour... \n";
@r = mymap (\@t, sub { return @_[0]*10; });

print "<p>result mymap : ", "@r", "\n";

sub f 
{
 $x = @_[0];
 return $x + 1;
}

$a = f(5);
print "<p>",$a,"\n";

sub f2 
{
 my ($x,$y) = @_;
 return $x + 1;
}

$a = f2(5,8);
print "<p>",$a,"\n";

print "<p>";
open (OUT, "ls |")
 or die "<p>Ca ne marche pas : $!_n";
while (<OUT>)
{
 print $_,"<br>";
}
close (OUT);

@couleurs = ("violet", "indigo", "bleu");
push (@couleurs, ( "vert", "jaune", "orange", "rouge" ));

print "<ul>";
foreach $couleur (@couleurs)
{
 print "<li>", $couleur;
}
print "</ul>";

print "Avec un e :";
@avece = grep { /e/ } @couleurs;
foreach $c (@avece) { print " ", $c; }
print "<p>";

@config = (" aaa = 111 ", " bbb = 222 # commentaire", " # blablabla"," ccc = 333 ");
foreach $_ (@config)
{
 s/#.*//;
 s/\s+//;
 next unless length;
 my ($var, $valeur) = split (/\s*=\s*/, $_, 2);
 print $var, "=", $valeur, ";","<br>"
}

print "<h3>Liste des scripts</h3>";
print "<ul>";
opendir (rep, ".") or die "erreur";
while (defined ($f = readdir(rep)))
{
 print "<li>", $f
}
closedir(rep);
print "</ul>";

%h1 = (a=>111, b=>222, c=>333, d=>{e=>555});

print "<p>", $h1{c};

# %h2 = %$h1{d};
$rh2 = $h1{d};
%h2 = %$rh2;

print "<p>", $h2{e};

print "<p>", $h1{d}{e};

$h3 = {a=>111, b=>222, c=>333, d=>{e=>555}};

print "<p>", $$h3{a}, ",", $h3->{a};

print "<p>h3=", %$h3;

sub getfieldc 
{
 # my ($rh) = $_;
 my $rh = shift;
 print "<p>h=", %$rh;
 # $r = %$h{c};
 # %h = %$rh;
 # $r = $h{c};
 $r = $$rh{c};
 # $r = $rh->{c}; also works
 return $r;
}

print "<p>", getfieldc($h3);
 
$h4 = {a=>aaa, 1=>{a=>bbb}};

print "<p>", $$h4{a}, $$h4{1}{a};

sub printdem 
{
 my $d = shift;
 print "dem-";
 print $$d{node};
 print $$d{val};
 if ($$d{node} eq l)
 {
  print " ";
  print "(";
  printdem ($$d{0});
  print ")";
 }
 if ($$d{node} eq ap)
 {
  print " ";
  print "(";
  printdem ($$d{0});
  print ") (";
  printdem ($$d{1});
  print ")";
 }
}

sub shft
{
 my $m = shift;
 my $n = shift; 
 my $x = shift;
 if ($$x{node} eq var)
 {
  if ($$x{val} >= $m)
   { return {node=>var, val=>1+$$x{val}}; }
  else
   { return $x; }
 }
 elsif ($$x{node} eq l)
  { return {node=>l, 0=>shft($m+1,$n,$$x{0})}; }
 elsif ($$x{node} eq sym)
  { return $x; }
 else
  { return { $$x{node}, shft($m,$n,$$x{0}), shft($m,$n,$$x{1}) }; }
}

sub subst
{
 my $n = shift;
 my $y = shift;
 my $z = shift;
 if ($$y{node} eq var)
 {
  if ($$y{val} == n)
   { return z; }
  elsif ($$y{val} > n)
   { return {node=>var, val=>$$y{val}-1}; }
  else
   { return $y; }
 }
 elsif ($$y{node} eq l)
  { my $n1 = $n+1;
    my $y1 = $$y{0};
    my $z1 = shft(0,1,$z);
    my $x = subst ($n1, $y1, $z1);
    # my $x = subst ($n+1, $$y{0}, shft(0,1,$z));
    return {node=>l, 0=>$x}; 
 }
 else
  { return {node=>$$y{node}, val=>$$y{val}, 
            0=>subst($n,$$y{0},$z), 
            1=>subst($n,$$y{1},$z)}; }
}


sub lr 
{
 # my ($r, $d) = $_;
 my $r = shift;
 my $d = shift;
 if ($$d{node} eq sym) { return $d; }
 elsif ($$d{node} eq var) { return $d; }
 elsif ($$d{node} eq ap) 
  { print "<p>node ap";
    $r = { node=>ap, 0=>lr($r,$$d{0}), 1=>lr($r,$$d{1}) };
    return $r;
  }
 elsif ($$d{node} eq axm)
  { return $$d{$r}; }
 elsif ($$d{node} eq rt)
  { if (lr(1,$$d{0}) == lr(1,$$d{1}))
     { return lr(0,$$d{$r}); }
    else
     { return $d; }
  }
 elsif ($$d{node} eq l)
  { return (node=>l, 0=>lr($r,$$d{0})); }
 elsif ($$d{node} eq as)
  {
   print "<p>node as ";
   my $d0 = $$d{0};
   my $f = lr (1, $$d{0});
   print "<p>d0="; printdem ($d0);
   print "<p>f="; printdem ($f);
   print "...";
   if ($r == 0 || ! ($$f{node} eq l))
    { return {node=>ap, 0=>lr($r,$$d{0}), 1=>lr($r,$$d{1})}; }
   else
    { return subst (0, $$f{0}, lr(1,$$d{1})); }
  }
 else { return $d; }
}


$d1 = {node=>ap, 0=>{node=>K}, 1=>{node=>I}};
$d2 = {node=>S};

print "<p>d2="; printdem($d2);
print "<p>d1="; printdem($d1);

$d1r = lr (1, $d1);

print "<p> d1r = "; printdem ($d1r);

$d3 = {node=>axm, 0=>{node=>a}, 1=>{node=>b}};

print "<p>d3r="; printdem(lr(1,$d3));

$d4 = {node=>as, 0=>{node=>l, 1=>{node=>var, val=>0}}, 1=>{node=>a}};
$d4r = lr(1,$d4);
print "<p>d4r="; printdem($d4r);


