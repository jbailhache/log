[SURVCONFIG]
periode=1
heure=0
seconde=10
duree=3
timeout=600
hote=membres.lycos.fr
dir=/teledev/
config=survconfig.php
storefile=storefile.php
traitement=traitement.exe
<?php
 echo 'flags=';
 echo $id%2;
 echo '
';
 echo 'minute=';
 echo $id*10-5;
 echo '
';
?>
$
