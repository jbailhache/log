<html>
<body>

<br><br><br><br><br><br><br><br><br><br><br><br><br><br><br><br>

<?php
 echo "Donn�es re�ues: [" . $_GET["data"] . "]";
 $f = fopen ("data.dat", "wb");
 echo "<br>f=$f";
 $n = fwrite ($f, $_GET["data"]);
 echo "<br>n=$n";

?>

</body>
</html>
