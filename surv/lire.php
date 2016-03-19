<html>
<body>
<br><br><br><br><br><br><br><br><br><br><br><br>

<?php
 $f = fopen ("data.dat", "rb");
 echo "<br>f=$f";
 $s = fgets ($f, 100);
 echo "<br>data=[" . $s . "]";
?>

</body>
</html>
