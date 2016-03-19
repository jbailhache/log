<HTML>
<head>
<title>LCEI</title>
</head>
<body>
<h3>LCEI</h3>
<p>

<?php

function node ($x)
{
	return $x[0];
}

function mkdem ($n, $a, $b)
{
	return array ($n, $a, $b);
}

/* function node_ap () { return "-"; } */

function K ()
{
	return mkdem ("K", 0, 0);
}

function S ()
{
	return mkdem ("S", 0, 0);
}

function ap ($f, $a)
{
	return mkdem ("-", $f, $a);
}

function apsubst ($f, $a)
{
	return mkdem ("_", $f, $a);
}

function rtransym ($a, $b)
{
	return mkdem ("/", $a, $b);
}

function axiom ($a, $b)
{
	return mkdem ("=", $a, $b);
}

function lambda ($a)
{
	return mkdem ("^", $a, 0);
}

function nleft ($a)
{
	return mkdem ("<", $a, 0);
}

function nright ($a)
{
	return mkdem (">", $a, 0);
}

function nrefl ($a)
{
	return mkdem ("`", $a, 0);
}

function nRefle ($a)
{
	return mkdem ("$", $a, 0);
}

function variable ($n)
{
	return mkdem ("V", $n, 0);
}

function symbol ($s)
{
	return mkdem ("s", $s, 0);
}

function readexpr ($s) /* returns array(0=>result, 1=>remaining string) */
{
	$c = substr ($s, 0, 1);
	$r = substr ($s, 1);
	if ($c == "-")
	{
		$x = readexpr ($r);
 		$a = $x[0];
		
		/*echo ("a=");
		printexpr ($a);
		echo ("<p>");*/

		$r1 = $x[1];
		$y = readexpr ($r1);
		$b = $y[0];

		/*echo ("b=");
		printexpr ($b);
		echo ("<p>");*/

		$r2 = $y[1];
		$c = ap ($a, $b);

		/*echo ("c=");
		printexpr ($c);
		echo ("<p>");*/

		$z = array ($c, $r2); 
		
		/*echo ("z0=");
		printexpr ($z[0]);
		echo ("<p>");*/

		return $z;
	}
	else if ($c == "_")
	{
		$x = readexpr ($r);
		$a = $x[0];
		$r1 = $x[1];
		$y = readexpr ($r1);
		$b = $y[0];
		$r2 = $y[1];
		$c = apsubst ($a, $b);
		$z = array ($c, $r2);
		return $z;
	}
	else if ($c == "/")
	{
		$x = readexpr ($r);
		$a = $x[0];
		$r1 = $x[1];
		$y = readexpr ($r1);
		$b = $y[0];
		$r2 = $y[1];
		$c = rtransym ($a, $b);
		$z = array ($c, $r2);
		return $z;
	}
	else if ($c == "=")
	{
		$x = readexpr ($r);
		$a = $x[0];
		$r1 = $x[1];
		$y = readexpr ($r1);
		$b = $y[0];
		$r2 = $y[1];
		$c = axiom ($a, $b);
		$z = array ($c, $r2);
		return $z;
	}
	else if ($c == "^")
	{
		$x = readexpr ($r);
		$a = $x[0];
		$r1 = $x[1];
		$b = lambda ($a);
		$z = array ($b, $r1);
		return $z;
	}
	else if ($c == "<")
	{
		$x = readexpr ($r);
		$a = $x[0];
		$r1 = $x[1];
		$b = nleft ($a);
		$z = array ($b, $r1);
		return $z;
	}
	else if ($c == ">")
	{
		$x = readexpr ($r);
		$a = $x[0];
		$r1 = $x[1];
		$b = nright ($a);
		$z = array ($b, $r1);
		return $z;
	}
	else if ($c == "`")
	{
		$x = readexpr ($r);
		$a = $x[0];
		$r1 = $x[1];
		$b = nrefl ($a);
		$z = array ($b, $r1);
		return $z;
	}
	else if ($c == "$")
	{
		$x = readexpr ($r);
		$a = $x[0];
		$r1 = $x[1];
		$b = nRefle ($a);
		$z = array ($b, $r1);
		return $z;
	}

	else if ((ord($c) >= ord("0")) && (ord($c) <= ord("9")))
	{
		$a = variable (ord($c) - ord("0"));
		$z = array ($a, $r);
		return $z;
	}
	else if ($c == "K")
	{
		return array (K(), $r);
	}
	else if ($c == "S")
	{
		return array (S(), $r);
	}
	else
	{
		$a = symbol ($c);
		$z = array ($a, $r);
		return $z;
	}
}

function printexpr ($x)
{
	/*$x = array ("-", "S", "K");
	echo ($x);*/

	if ($x[0] == "K")
		echo ("K");
	else if ($x[0] == "S")
		echo ("S");
	else if ($x[0] == "^" || $x[0] == "<" || $x[0] == ">" || $x[0] == "`" || $x[0] == "$")
	{
		echo ($x[0]);
		printexpr ($x[1]);
	}
	else if ($x[0] == "-" || $x[0] == "/" || $x[0] == "_" || $x[0] == "=")
	{
		echo ($x[0]);
		printexpr ($x[1]);
		printexpr ($x[2]);
	}
	else if ($x[0] == "V" || $x[0] == "s")
	{
		echo ($x[1]);
	}


}

function shift ($m, $n, $x)
{
	if ($x == 0)
		return $x;
	if (node($x) == "V")
	{
		if ($x[1] >= $m)
			return variable ($x[1] + n);
		else
			return $x;
	}
	if (node($x) == "^")
	{
		return lambda (shift ($m+1, $n, $x[1]));
	}
	if (node($x) == "s")
	{
		return $x;
	}
	return mkdem (node($x), shift ($m, $n, $x[1]), shift ($m, $n, $x[2]));
}

function subst ($n, $y, $z)
{
	if ($y == 0)
		return 0;
	if (node($y) == "V")
	{
		if ($y[1] == $n)
			return $z;
		if ($y[1] > $n)
			return variable ($y[1] - 1);
	}
	if (node($y) == "-")
	{
		return ap (subst (n, $y[1], z),
			   subst (n, $y[2], z));
	}
	if (node($y) == "^")
	{
		return lambda (subst (n+1, $y[1], shift (0, 1, z)));
	}
	if (node ($y) == "s")
	{
		return $y;
	}
	return mkdem (node($y), subst ($n, $y[1], $z), subst ($n, $y[2], $z));
}

function equal ($x, $y)
{
	if ($x == $y)
		return true;
	if ($x == 0)
		return false;
 	if ($y == 0)
		return false;
	if (node($x) != node($y))
		return false;
	if (node($x) == "V" || node($x) == "s")
	{
		if ($x[1] == $y[1])
			return true;
		return false;
	}
	if (!equal ($x[1], $y[1]))
		return false;
	if (!equal ($x[2], $y[2]))
		return false;
	return true;
}

function lr ($r, $d)
{
	if (node($d) == "-")
	{
		$d1 = ap (lr($r,$d[1]), lr($r,$d[2]));		 
	}
	else if (node($d) == "_")
	{
		$f = lr (1, $d[1]);
		$a = lr (1, $d[2]);
		if ($r == 0 || node($f) != "^")
		{
			$s0 = lr ($r, $d[1]);
			$s1 = lr ($r, $d[2]);
			$d1 = ap ($s0, $s1);
		}
		else
			$d1 = subst (0, $f[1], $a);
	}
	else if (node($d) == "/")
	{
		if (equal (lr(1,$d[1]), lr(1,$d[2])))
			$d1 = lr (0, $d[$r+1]);
		else
			$d1 = $d;
	}
	else if (node($d) == "^")
	{
		$d1 = lambda (lr ($r, $d[1]));
	}
	else if (node($d) == "V" || node($d) == "s")
	{
		$d1 = $d;
	}
	else if (node($d) == "=")
	{
		return $d[$r+1];
	}
	else if (node($d) == "<")
	{
		if ($r == 0)
			$d1 = nleft (lr($r,$d[1]));
		else
			$d1 = lr(0,lr($r,$d[1]));
	}
	else if (node($d) == ">")
	{
		if ($r == 0)
			$d1 = nright (lr($r,$d[1]));
		else
			$d1 = lr(1,lr($r,$d[1]));
	}
	else if (node($d) == "`")
	{
		$d1 = $d[1];
	}
	else if (node($d) == "$")
	{
		$d1 = lr ($r, lr ($r, $d[1]));
	}
	return $d1;
		

}

	echo ("Test 5 : <textarea rows=10 cols=60>");
	$result = readexpr ($expr);
	$e = $result[0];
	$l = lr (0, $e);
	$r = lr (1, $e);
	/*
	$x = array ("-", "S", "K");
	$x = ap (S(), K());
	*/
	printexpr ($e);
	echo (" : ");
	printexpr ($l);
	echo (" = ");
	printexpr ($r);
	echo ("</textarea>");
?>

</body>
</HTML>
