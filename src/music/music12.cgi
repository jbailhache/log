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
  return chr(0x90 + $c) . chr($n) . "\x40";
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
  $freq = 440 * pow (2, ($note + 12 * $octave - 57) / 12.0 );
  return $freq;
 }

 print ("Content-type: audio/x-midi\n\n");
 print header();

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
 
 $canal = 0;
 $tempo = 100;
 $tempo1 = $tempo;
 $octave = 4;
 $garde = 0;
 @pile = ();
 $niveau = 0;

 for ($i=0; $i<length($partition); $i++)
 {
  $cmd = substr ($partition, $i, 1);
  if (ord($cmd) >= ord('A') && ord($cmd) <= ord('P'))
   { $canal = ord($cmd) - ord('A'); }
  else if (ord($cmd) >= ord('0') && ord($cmd) <= ord('9'))
   { $octave = ord($cmd) - ord('0'); }
  else if ($cmd eq "(")
  {
   $garde = 1;
  }
  else if ($cmd eq "*")
   { $niveau++; }
  else if ($cmd eq ")")
   {
    $n = scalar(@pile);
    $rec = $pile[$n-1-$niveau];
    $canal = $$rec[0];
    $freq = $$rec[1];
    $music .= noteoff ($canal, $freq);    
    $niveau = 0;
   }
  else if ($cmd eq ":")
  {
   $instrument = substr ($partition, $i+1, $i+3);
   $music .= setinstr ($canal, $instrument);
  }
  else if (ord($cmd) >= 'a' && ord($cmd) <= 'l')
  {
   $note = ord($cmd) - ord('a');
   $freq = frequence ($note, $octave);
   $music .= noteon ($canal, $freq);
   if ($garde == 0)
   {
     $music .= delay ($tempo1) . noteoff ($canal, $freq);
   }
   else
   {
    $rec = [$canal, $freq];
    push (@pile, $rec);
   }
   $tempo1 = $tempo;
   $garde = 0;
  }
  else if ($cmd eq ";")
  {
   $music .= delay ($tempo1);
   $tempo1 = $tempo;
  }
  
  else if ($cmd eq "T")
  {
   i++;

   if (substr ($partition, $i, 1) eq "_"))
    { $tempo *= 2; }
   else if (substr ($partition, $i, 1) eq "'")
    { $tempo /= 2; }
   else if (substr ($partition, $i, 1) eq "+"))
   {
    $i++;
    $tempo += 100 * ord (substr ($partition, $i, 1)) - ord("0");
   }
   else if (substr ($partition, $i, 0) eq "-")
   {
    $i++;
    $tempo -= 100 * ord (substr ($partition, $i, 1)) - ord("0");
   }
  }
  else if ($cmd eq "_")
   { $tempo1 *= 2; }
  else if ($cmd eq "'")
   { $tempo1 /= 2; }
  else if ($cmd eq "+")
   {
    $i++;
    $tempo1 += 100 * ord (substr ($partition, $i, 1)) - ord("0");
   }
  else if ($cmd eq "-")
   {
    $i++;
    $tempo1 -= 100 * ord (substr ($partition, $i, 1)) - ord("0");
   }
 }




 $t = track ($music);
 print ($t);
