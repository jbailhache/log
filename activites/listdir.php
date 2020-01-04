<?php
$mydir = '.'; 
if ($dir = @opendir($mydir)) 
{ while (($file = readdir($dir)) !== false) 
{ if($file != ".." && $file != ".") { echo $file;  echo "<br>";} } 
closedir($dir); } 
?>