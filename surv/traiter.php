<html>
<body>

<br><br><br><br><br><br><br><br><br><br><br><br><br><br><br><br>

<?php
 echo "Donn�es re�ues: [" . $_POST["data"] . "]";
 $f = fopen ("data.dat", "wb");
 echo "<br>f=$f";
 $n = fwrite ($f, $_POST["data"]);
 echo "<br>n=$n";

?>

</body>
</html>
