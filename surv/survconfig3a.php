<?php
 if ($id == 'a')
  echo ('
[SURVCONFIG]
flags=0
periode=1
heure=0
minute=49
seconde=10
duree=2
timeout=120
hote=membres.lycos.fr
dir=/teledev/
config=survconfig.php
storefile=storefile.php
traitement=traitement.exe
$
');
 else if ($id == 'b')
  echo ('
[SURVCONFIG]
flags=1
periode=1
heure=0
minute=10
duree=4
hote=membres.lycos.fr
dir=/teledev/
config=survconfig.php
storefile=storefile.php
traitement=traitement.exe
$
');
?>
