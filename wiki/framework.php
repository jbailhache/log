<?php

 $s = session_start();

 require ("platform.php");
 require ("util.php");
 //require ("config.php");

 function head($titre,$fond)
 {
  echo "<html>
<head>
<meta http-equiv=\"Content-Type\" content=\"text/html; charset=UTF-8\" />
<title>$titre</title>
</head>
<body background=\"$fond\">
<h2>$titre</h2>
";
 }

 function tail()
 {
  echo "</body></html>";
 }

 function captcha()
 {
  ?>
Recopiez ce code dans la case ci-contre pour prouver que vous n'&ecirc;tes pas un robot : 
<?php
 $s = "";
 for ($i=0; $i<6; $i++)
 {
  $s = $s . chr(65+rand(0,25));
 }
 echo $s;
?>
<input type=hidden name=code1 value="<?php echo $s; ?>">
<input type=text name=code2 size=7>
  <?php
 }

