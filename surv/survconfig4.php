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
param=0
<?php
 if ($id == "1")
  echo 'flags=1
minute=5
';
 else if ($id == "2")
  echo 'flags=1
minute=35
';
 else if ($id == "3")
  echo 'flags=0
minute=50
';
?>
$
