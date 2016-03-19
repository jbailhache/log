
#!/usr/local/bin/perl
#
# Author name:
# Creation date:
#
# Description:
#
 
 use TripodCGI;
  
 $cgi = new TripodCGI;
 $partition = $cgi->param('partition');    

 sub trace
 {
 }

 sub header
 {
  $h = "\x4D\x54\x68\x64\x00\x00\x00\x06\x00\x01\x00\x02\x00\xF0\x4D\x54\x72\x6B\x00\x00\x00\x13\x00\xFF\x58\x04\x04\x02\x18\x08\x00\xFF\x51\x03\x07\xA1\x20\x00\xFF\x2F\x00";
  return $h;
 }

 sub varlength
 {
  my $x = shift;
  if ($x < 128)
   { return chr($x); }
  if ($x < 128*128)
   { return chr(128+$x/128) . chr($x%120); }
  return chr(128+$x/(128*128)) . chr(128+($x/128)%128) . chr($x%128);
 }

 sub delay
 {
  my $t = shift;
  return varlength($t);
 }

 sub setinstr 
 {
  my $canal = shift;
  my $i = shift;
#  return "\xC0" . chr($i);
  return chr(0xC0 + $canal) . chr($i);
 }

 sub noteon 
 {
  my $c = shift;
  my $n = shift;
  my $v = shift;
  return chr(0x90 + $c) . chr($n) . chr($v);
 }

 sub noteoff
 {
  my $c = shift;
  my $n = shift;
  return chr(0x80 + $c) . chr($n) . "\x00";
 }
    
 sub track
 {
  my $m = shift;
  my $l = length ($m);
  my $t = "MTrk" . "\x00\x00" . chr($l/256) . chr ($l%256) . $m;
  return $t;
 }

 sub frequence 
 {
  my $note = shift;
  my $octave = shift;
  trace "frequence ", $note, ",", $octave;
  $note1 = $note + 12 * $octave;
  $ecart = $note1 - 57;
  $ecartoct = $ecart / 12.0;
  $ecarte = log(2) * $ecartoct;
  $rapport = exp($ecarte);
  $freq = 440 * $rapport;
  print "<p> note=", $note, " octave=", $octave, " note1=", $note1, " ecart=", $ecart, " ecartoct=", $ecartoct, " ecarte=", $ecarte, " rapport=", $rapport, " freq=", $freq, " ";
#  $freq = 440 * pow (2, ($note + 12 * $octave - 57) / 12.0 );
   return $freq;
 }

 sub numnote 
 {
  my $note = shift;
  my $octave = shift;
  trace "frequence ", $note, ",", $octave;
  $note1 = $note + 12 * $octave;
  return $note1;
 }

 $music1 = delay(0) . setinstr(0x6F) . delay(60) .
  noteon(0,0x3C) . delay(100) . noteoff(0,0x3C) . delay(10) .
  noteon(0,0x40) . delay(60) . noteoff(0,0x40) . delay(10) .
  noteon(0,0x3E) . delay(180) . noteoff(0,0x3E) . delay(0) .
  "\xFF\x2F\x00";

 $music = "";
 
# A à P : canal
# :nnn : instrument n
# a à l : jouer note
# 0 à 9 : octave
# _ : double durée
# ' : demi durée
# +n : durée + n * 0.1 s
# -n : durée - n * 0.1 s
# ; : silence
# T : tempo
# ( : note qui dure
# *...*) : fin note qui dure

# exemples : aaac_e_ca_ecc_abc'def(afj_h)c
#  :018!1005(afj_h(ce*)h)f

# print "Document-type: text/html\n\n";
 

 $canal = 0;
 $tempo = 100;
 $tempo1 = $tempo;
 $octave = 4;
 $garde = 0;
 @pile = ();
 $niveau = 0;
 $silence = 0;
 $volume = 0x40;
 $numnote1 = 0;

 trace "partition=", $partition;

 for ($i=0; $i<length($partition); $i++)
 {
  $cmd = substr ($partition, $i, 1);
  trace " CMD",$cmd;
  trace "<p>cmd=", $cmd, " ord=", ord($cmd);
  if (ord($cmd) >= ord('A') && ord($cmd) <= ord('P'))
   { $canal = ord($cmd) - ord('A'); }
  elsif (ord($cmd) >= ord('0') && ord($cmd) <= ord('9'))
   { $octave = ord($cmd) - ord('0'); }
  elsif ($cmd eq "(")
  {
   $garde = 1;
  }
  elsif ($cmd eq "*")
   { $niveau++; }
  elsif ($cmd eq '$')
   {
    $n = scalar(@pile);
    $rec = $pile[$n-1-$niveau];
    $canal = $$rec[0];
    $freq = $$rec[1];
    $music .= delay(0) . noteoff ($canal, $freq);    
    $niveau = 0;
   }
  elsif ($cmd eq ";")
  {
   $music .= delay ($tempo1) . noteoff ($canal1, $numnote1);
   $numnote1 = -1;   
  }
  elsif ($cmd eq ")")
  {
   $music .= delay ($tempo1) . noteoff ($canal1, $numnote1);
   $numnote1 = 0;
   trace " *** note off *** ";
   $i++;
   trace " code suivant:", substr($partition,$i,1);
   $note = ord(substr($partition,$i,1)) - ord('a');
   trace " note: ", $note;
   $numnote = numnote ($note, $octave);
   $music .= delay(0) . noteoff ($canal, $numnote);
   trace "**** ";
#   for ($j=0; $j<length($music); $j++)
#    {  printf "%02X ", ord(substr ($music, $j, 1)); }
   trace "****";
  }
  elsif ($cmd eq ":")
  {
   $instrument = substr ($partition, $i+1, $i+3);
   $music .= delay(0) . setinstr ($canal, $instrument);
   $i += 3;
  }
  elsif ($cmd eq "!")
  {
   $volume = substr ($partition, $i+1, $i+3);
   $i += 3;
  }
  elsif (ord($cmd) >= ord('a') && ord($cmd) <= ord('z'))
  {
   trace " ***note*** ";
   $note = ord($cmd) - ord('a');
   trace "<p>note=", $note, " octave=", $octave;
   $numnote = numnote ($note, $octave);
#   $freq = 440;
   
   if ($garde == 0 && $numnote1 > 0)
   {
     $music .= delay ($tempo1) . noteoff ($canal1, $numnote1);
   }
   
   trace " numnote1=", $numnote1;
   if ($numnote1 == -1)
    { $silence = $tempo1; }
   $music .= delay($silence) . noteon ($canal, $numnote, $volume);

#   if ($garde == 0)
#   {
#     $music .= delay ($tempo1) . noteoff ($canal, $numnote);
#   }
#   else

   if ($garde != 0)
   {
    $rec = [$canal, $numnote];
    push (@pile, $rec);
   }
   $tempo1 = $tempo;
   $garde = 0;
   $silence = 0;
   $canal1 = $canal;
   $numnote1 = $numnote;   
  }
  elsif ($cmd eq "W")
  {
#   $music .= delay ($tempo1);
#   $tempo1 = $tempo;
   $silence = $tempo1;
   $tempo1 = $tempo;
  }
  elsif ($cmd eq "T")
  {
   $tempo = substr ($partition, $i+1, $i+3);
   $i += 3;   
  }
  elsif ($cmd eq "U")
  {
   $i++;

   if (substr ($partition, $i, 1) eq "_")
    { $tempo *= 2; }
   elsif (substr ($partition, $i, 1) eq "'")
    { $tempo /= 2; }
   elsif (substr ($partition, $i, 1) eq "+")
   {
    $i++;
    $tempo += 100 * ord (substr ($partition, $i, 1)) - ord("0");
   }
   elsif (substr ($partition, $i, 0) eq "-")
   {
    $i++;
    $tempo -= 100 * ord (substr ($partition, $i, 1)) - ord("0");
   }
  }
  elsif ($cmd eq "_")
   { $tempo1 *= 2; }
  elsif ($cmd eq "'")
   { $tempo1 /= 2; }
  elsif ($cmd eq "+")
   {
    $i++;
    $tempo1 += 100 * ord (substr ($partition, $i, 1)) - ord("0");
   }
  elsif ($cmd eq "-")
   {
    $i++;
    $tempo1 -= 100 * ord (substr ($partition, $i, 1)) - ord("0");
   }
 }
 $music .= delay ($tempo1) . noteoff ($canal1, $numnote1);
   



# print ("Content-type: audio/x-midi\n\n");
# print header();

# $t = track ($music);
# print ($t);

 $midi = header() . track($music . delay(0) . "\xFF\x2F\x00");

 print ("Content-type: audio/x-midi\n\n");
 print $midi;

# print "Document-type: text/html\n\n"; 
# for ($i=0; $i<length($midi); $i++)
#  {  printf "%02X ", ord(substr ($midi, $i, 1)); }




















