<HTML>
<head>
<title>Calcul matriciel en PHP</title>
</head>
<body>
<h3>Calcul matriciel en PHP</h3>

<?php

function add_mat ($a, $b)
{
	$c = array ();
	$c[nl] = $a[nl];
	$c[nc] = $a[nc];
	for ($i=0; $i<$a[nl]; $i++)
	{
		$c[$i] = array();
		for ($j=0; $j<$a[nc]; $j++)
			$c[$i][$j] = $a[$i][$j] + $b[$i][$j];
	}
	return $c;
}

function prod_mat ($a, $b)
{
	$c = array ();
	$c[nl] = $a[nl];
	$c[nc] = $b[nc];
	for ($i=0; $i<$c[nl]; $i++)
	{
		$c[$i] = array();
		for ($j=0; $j<$c[nc]; $j++)
		{
			$c[$i][$j] = 0;
			for ($k=0; $k<$a[nc]; $k++)
				$c[$i][$j] += $a[$i][$k] * $b[$k][$j];
			/*
			echo ("<br>");
			echo ($i);
			echo (" ");
			echo ($j);
			echo (" : ");
			echo ($c[$i][$j]);
			*/
		}
	}
	return $c;
}

function transpose_mat ($a)
{
	$b = array();
	$b[nl] = $a[nc];
	$b[nc] = $a[nl];
	for ($i=0; $i<$b[nl]; $i++)
	{
		$b[$i] = array();
		for ($j=0; $j<$b[nc]; $j++)
			$b[$i][$j] = $a[$j][$i];
	}
	return $b;
}

function print_mat ($a)
{
        echo ("<p><table border=1>");
	for ($i=0; $i<$a[nl]; $i++)
	{
		echo ("<tr>");
		for ($j=0; $j<$a[nc]; $j++)
		{
			echo ("<td align=right>");
			echo ($a[$i][$j]);
			echo ("</td>");
		}
		echo ("</tr>");
	}
	echo ("</table>");

}

function test ()
{
	$a = 3;
	echo ("a = ");
	echo ($a);
	$a = array ( nl => 3, nc => 4,
		     array (1, 4, 3, 9),
		     array (0, 5, 2, 1),
                     array (3, 9, 6, 4) );
	echo (" ord = ");
	echo ($a["ord"]);
	echo (" a = ");
	print_r ($a);
	echo ("<p>");
	for ($i=0; $i<$a[nl]; $i++)
	{
		for ($j=0; $j<$a[nc]; $j++)
		{
			echo (" ");
			echo ($a[$i][$j]);
		}
		echo ("<br>");
	}
	$a2 = add_mat ($a, $a);
	echo ("ord a2 = "); echo ($a2[nl]); 
	print_mat ($a2);
	$ta = transpose_mat ($a);
	echo ("<p>Transposée : ");
	print_mat ($ta);
	$p = prod_mat ($a, $ta);
	echo ("<p>Produit :");
	print_mat ($p);
}

echo ("Test: ");
test();

?>

</body>
</HTML>

