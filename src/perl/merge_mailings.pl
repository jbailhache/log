
use Win32::ODBC;

sub copytab
{
 my $a = shift;
 my %c = ();
 foreach $k (keys %$a)
 {
  $c{$k} = $$a{$k};
 }
 return \%c;
}

@champs = (REF,NAME,FAX,ID,LST,ADATE,SDATE,RETRIED,RETRY,PGNB,PGOK,PRI,HASH,CNX,DCNX,TIME,RTR,LINE,HGN,STATUS,TAX,FCSI,DELAY,ASTOP,SSTOP,FLID,SPEED,NOISE,SIGNALS);

print "Nom de la base ? ";
$DSN = <>;
chop($DSN);

if (!($db=new Win32::ODBC($DSN))) {
		print "Error connecting to $DSN\n";
	    print "Error: " . Win32::ODBC::Error() . "\n";
	} else {
	   print "Database opened...\n";
	}

@noms = ();

$nom = "";

$cont = 1;

print "Nom du nouveau mailing ? ";
$nouveau = <>;
chop($nouveau);

while ($cont)
{
 print "Nom mailing à fusionner ? ";
 $nom = <>;
 chop($nom);
# print "<" . $nom . "> " . length($nom) . "\n";
 if (length($nom) == 0) { $cont=0; }
 else
 {
  @noms = ( @noms, $nom );
 }
}

print "Fichier de sortie ? ";
$outfile = <>;
chop($outfile);
open OUT, "> $outfile";

print "Fusion des mailings:\n";
foreach $nom (@noms)
{
 print $nom . "\n";
}

%recs = ();

	$SqlStatement = "select * from SEND";	
	if ($db->Sql($SqlStatement)) {
		print "SQL failed.\n";
	        print "Error: " . $db->Error() . "\n";
	} else  {
		$n = 0;
		while($db->FetchRow()) {
			$rowcnt++;

			%Data = $db->DataHash();



			# print "Numero de fax:" . $Data{'FAX'} . "\n";

			if (!(length ($Data{'FAX'}) == 4))
			{
				$trouve = 0;
				
				foreach $nom (@noms)
				{
					if ($nom eq $Data{'NAME'})
					{
						$trouve = 1;
						goto nom_trouve;
					}
				
				}			

				nom_trouve:
				
				if ($trouve)
				{
					$trouve = 0;
					 
					$recs{$Data{'FAX'}} = copytab(\%Data);
					$n++;
					print $n . "\r";
 						 
				}
			}

		} #the while loop to walk through the data

		print "\n";

		$n = 0;

                foreach $fax (keys %recs)
		{
			$rec = $recs{$fax};
			%Data = %$rec;	
		
			if ($n == 0)
			{
				$p = 0;
				foreach $k (@champs)
				{
					if ($p > 0) { print OUT "\t"; }
					print OUT $k;
					$p++;
				}
				print OUT "\n";		
			}
			
			$p = 0;
			$Data{'NAME'} = $nouveau;
			# print $nouveau . "-";
			foreach $k (@champs)
			{
				if ($p > 0) {print OUT "\t";}
				print OUT $Data{$k};
				$p++;
			}
			print OUT "\n";

			$n++;
			print $n . "\r";
		}


	} #the else, the statement worked
