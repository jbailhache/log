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
<?php
 if ($id == "1")
  echo 'flags=2
minute=20
traitement=surv8b.exe
param=4
';
 else if ($id == "2")
  echo 'flags=1
minute=50
traitement=traitement.exe
param=0
';
 else if ($id == "3")
  echo 'flags=0
minute=35
traitement=traitement.exe
param=0
';
 else if ($id == "4")
  echo 'flags=1
minute=21
traitement=traitement.exe
param=0
';
?>
$