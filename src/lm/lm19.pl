#!/usr/bin/perl

my $trace = 0;

my $script = $ARGV[0];
open SCRIPT, $script or die "$script: $!\n";

my @mem = ();
my $adr = 0;
my %labels = ();
my $state = 'normal';
my @chars = ();
my $char;
my $start;
my @digits = ('0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F');

=pod
sub coder0im 
{
	my $adr = shift;
	my $val = shift;
	# if ($trace != 0) { print "coder0im $val\n"; }
	my @a = ();
	$mem[$adr++] = ord '#';
	while ($val != 0)
	{
		# if ($trace != 0) { print "val=$val\n"; }
		push @a, $val % 16;
		$val = int ($val / 16);
	}
	foreach $x (reverse @a)
	{
		# if ($trace != 0) { print "x=$x\n"; }
		$mem[$adr++] = ord(@digits[$x]);
	}
	return $adr;
}
=cut

# print "Reading code...\n";
while (<SCRIPT>)
{
	if ($trace != 0) { print $_; }
	@chars = split //, $_;
	foreach $char (@chars)
	{
=pod
		if ($state eq 'deflab')
		{
			if ($trace != 0) { print "labels{$char}=$adr\n"; }
			$labels{$char} = $adr;
			$state = 'normal';
		}
		elsif ($state eq 'reflab')
		{
			if ($trace != 0) { print "reflab $char " . $labels{$char} . "\n"; }
			# $mem[$adr++] = ord '#';
			# $mem[$adr++] = $labels{$char};
			$adr = coder0im ($adr, $labels{$char});
			$state = 'normal';
		}
		elsif ($state eq 'special')
		{
			$state = 'normal';
			if ($char eq ':')
			{
				$state = 'deflab';
			}
			elsif ($char eq '$')
			{
				$state = 'reflab';
			}
			elsif ($char eq 'S')
			{
				$start = $adr;
			}
			else
			{
				$mem[$adr++] = ord $char;
			}
		}
		elsif ($char eq '\\')
		{
			# print "special\n";
			$state = 'special';
		}
		# elsif (not ($char =~ /[ \t\n\r]/))
		else
		{
=cut
			$mem[$adr++] = ord $char;
#		}				
	}
}

@chars = ();
my $nchars = 0;
my $ptr = 0;
my $line = '';

=pod
sub getchar
{
	if ($ptr >= $nchars)
	{
		$line = <STDIN>;
		@chars = split //, $line;
		$ptr = 0;
	}	
	$char = $chars[$ptr++];
	if ($trace != 0) { print "\ngetchar -> $char"; }
	return $char;
}
=cut

my $goon = 1;
my $pc = $start;
my $r0 = 0;
my $r1 = 0;
my $rc = $adr;
my $t;
my $instr;
$state = 'normal';

# print "Running...\n";
while (($goon != 0) and ($pc < 30000))
{
	$instr = chr($mem[$pc++]);
	# print $instr;

	# if ($trace != 0) { print "\nr0=$r0\tr1=$r1\trc=$rc\tpc=$pc\tinstr=$instr " . (ord $instr) . " "; my $r = <STDIN>; }
	if ($trace != 0) { print ((sprintf "\nr0=%4X\tr1=%4X\trc=%4X\tpc=%4X\tinstr=%s ",$r0,$r1,$rc,$pc,$instr) . (ord $instr) . " "); my $r = <STDIN>; }

	if ($state eq 'code') 
	{
		if ($instr eq '}') { $state = 'normal'; }
		else { $mem[$rc++] = ord $instr; }
	}
	elsif ($instr eq ',') { $r1 = $r0; }
	elsif ($instr eq '^') { $r0 = $pc; }
	elsif ($instr eq 'Q') { $goon = 0; }
	# elsif ($instr eq '?') { if ($r1 == 0) { $t = $r0; $r0 = $pc; $pc = $t } }
	elsif ($instr eq '?') { if ($r1 == 0) { $r1 = $pc; $pc = $r0; } }
	elsif ($instr eq '@') { $r0 = $mem[$r0]; }
	elsif ($instr eq '!') { $mem[$r0] = $r1; }
	elsif ($instr eq '#') { $r0 = 0; }

	elsif ($instr eq '0') { $r0 = $r0*16; }
	elsif ($instr eq '1') { $r0 = $r0*16+1; }
	elsif ($instr eq '2') { $r0 = $r0*16+2; }
	elsif ($instr eq '3') { $r0 = $r0*16+3; }
	elsif ($instr eq '4') { $r0 = $r0*16+4; }
	elsif ($instr eq '5') { $r0 = $r0*16+5; }
	elsif ($instr eq '6') { $r0 = $r0*16+6; }
	elsif ($instr eq '7') { $r0 = $r0*16+7; }
	elsif ($instr eq '8') { $r0 = $r0*16+8; }
	elsif ($instr eq '9') { $r0 = $r0*16+9; }
	elsif ($instr eq 'A') { $r0 = $r0*16+10; }
	elsif ($instr eq 'B') { $r0 = $r0*16+11; }
	elsif ($instr eq 'C') { $r0 = $r0*16+12; }
	elsif ($instr eq 'D') { $r0 = $r0*16+13; }
	elsif ($instr eq 'E') { $r0 = $r0*16+14; }
	elsif ($instr eq 'F') { $r0 = $r0*16+15; }

	elsif ($instr eq '~') { $r0 = !$r0; }
	elsif ($instr eq '+') { $r0 = $r0 + $r1; }
	elsif ($instr eq '-') { $r0 = $r0 - $r1; }
	elsif ($instr eq '*') { $r0 = $r0 * $r1; }
	elsif ($instr eq '/') { $r0 = $r0 / $r1; }
	elsif ($instr eq '%') { $r0 = $r0 % $r1; }
	elsif ($instr eq '&') { $r0 = $r0 & $r1; }
	elsif ($instr eq '|') { $r0 = $r0 | $r1; }
	elsif ($instr eq '<') { $r0 = $r0 << $r1; }
	elsif ($instr eq '>') { $r0 = $r0 >> $r1; }

	elsif ($instr eq 'G') { $r0 = ord getc(STDIN); }
	elsif ($instr eq 'P') { print chr($r0); }
	elsif ($instr eq 'H') { $rc = $r0; }
	elsif ($instr eq 'h') { $r0 = $rc; }
	# elsif ($instr eq "'") { $mem[$rc++] = '#'; $mem[$rc++] = $r0; }
	# elsif ($instr eq "'") { $rc = coder0im ($rc, $r0); }
	elsif ($instr eq '{') { $state = 'code'; }
	elsif ($instr eq '}') { $mem[$rc++] = ord $instr; }
	elsif ($instr eq '_') { $mem[$rc++] = $r0; }

	elsif ($instr eq 'T') { $trace = 1; }

	# else { $r0 = ord $instr; }

}

# print "\nDone.\n";


