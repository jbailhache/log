<?
// header
Header("Content-Type: image/jpeg");

// set up image and colours
$im = ImageCreate(300, 300);
$white = ImageColorAllocate($im, 255, 255, 255);
$black = ImageColorAllocate($im, 0, 0, 0);
$couleur = ImageColorAllocate ($im, 20, 200, 150);

// variables for arc
// circle center
$center_x = 150;
$center_y = 150;

// height and width
$width = 150;
$height = 150;

// start and end angle
$start = 0;
$end = 360;

// fill with white
ImageFill($im, 0, 0, $white);

// draw pentagon
ImageArc($im, $center_x, $center_y, $width, $height, $start, $end, $couleur);

ImageArc ($im, 100, 80, 1, 1, 0, 360, $black);

ImageLine ($im, 105, 70, 105, 70, $couleur);

$couleur = ImageColorAllocate ($im, 20, 30, 250);
ImageLine ($im, 40, 30, 130, 120, $couleur);

// ImageSetPixel ($img, 100, 80, $black);

/*
for ($i=1; $i<300; $i++)
for ($j=1; $j<200; $j++)
{
	$c[i*300+j] = ImageColorAllocate ($im, $i, $j, 0);
	ImageLine ($im, $i, $j, $i, $j, $c[i*300+j]);
}
*/

for ($i=1; $i<300; $i++)
{
/*50*/	$j = 100 + $i * $i / 20;
	ImageLine ($im, $i, $j, $i, $j, $couleur); 
}


$couleurs[1] = ImageColorAllocate ($im, 120, 130, 30);
$couleurs[2] = ImageColorAllocate ($im, 250, 240, 10);

ImageString ($im, 1, 200, 100, ImageColorAllocate($im, 0, 0, 0), $couleur);

for ($i=1; $i<100; $i++)
for ($j=1; $j<80; $j++)
{
	/*
	if ($i > 40)
		$coul = ImageColorAllocate ($im, 120, 130, 30);
	else
		$coul = ImageColorAllocate ($im, 250, 240, 10);
	ImageLine ($im, $i, $j, $i, $j, $coul);
	*/
	
	if ($i > 40)
		ImageLine ($im, $i, $j, $i, $j, $couleurs[1]);
	else
		ImageLine ($im, $i, $j, $i, $j, $couleurs[2]);
	
	/*
	if ($i > 40)
	{
		$couleurs[1] = ImageColorAllocate ($im, 120, 130, 30);
		ImageLine ($im, $i, $j, $i, $j, $couleurs[1]);
	}
	else
	{
		$couleurs[2] = ImageColorAllocate ($im, 250, 240, 10);
		ImageLine ($im, $i, $j, $i, $j, $couleurs[2]);
	}
	*/
}

for ($i=0; $i<=255; $i++)
	$gris[$i] = ImageColorAllocate ($im, $i, $i, $i);

for ($i=1; $i<100; $i++)
for ($j=20; $j<80; $j++)
{
	ImageLine ($im, $i, $j, $i, $j, $gris[$i+$j]);
}
 
$s1x = 80;
/*100*/ $s1y = 40;
$s2x = 160;
$s2y = 60;
$dz = 40;
$lambda = 1;

for ($x=70; $x<200; $x++)
for ($y=30; $y<120; $y++)
{
	/*ImageLine ($im, $i, $j, $i, $j, $couleur);*/
	$a = 0;
	$b = 0;

	$dx = $x - $s1x;
	$dy = $y - $s1y;
	$d2 = $dx * $dx + $dy * $dy + $dz * $dz;
	$d = sqrt ($d2);
	$a += sin ($d / $lambda);
	$b += cos ($d / $lambda);	

	$dx = $x - $s2x;
	$dy = $y - $s2y;
	$d2 = $dx * $dx + $dy * $dy + $dz * $dz;
	$d = sqrt ($d2);
	$a += sin ($d / $lambda);
	$b += cos ($d / $lambda);	

	$c = sqrt ($a * $a + $b * $b);
	ImageLine ($im, $x, $y, $x, $y, $gris[$c * 100]);
} 

// output to browser
ImageJPEG($im);
?>

<html>
<head>
<title>test image</title>
</head>
<body>
Test
</body>
</html>
