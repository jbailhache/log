[SURVCONFIG]
periode=1
heure=0
seconde=10
duree=3
timeout=480
hote=membres.lycos.fr
dir=/teledev/
config=survconfig.php
storefile=storefile.php
localcapturefile=c:\surv\surv%02d%02d.avi
distcapturefile=surv%02d%02d.avi
<?php
 if ($id == "")
 {
  echo 'flags=5
minute=14
localtraitement=c:\surv\surv1.exe
disttraitement=surv.exe
param=1
';
 }
 else if ($id == "1")
 {
  echo 'flags=5
minute=26
localtraitement=c:\surv\traitement.exe
disttraitement=traitement.exe
param=0
';
 }
?>
$