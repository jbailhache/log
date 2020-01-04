<html>
<head>
<title>Welcome PocketHPH!</title>
<style type="text/css"><!--
body {background-color: #ffffff; color: #000000;}
body, td, th, h1, h2 {font-family: sans-serif;}
pre {margin: 0px; font-family: monospace;}
a:link {color: #000099; text-decoration: none; background-color: #ffffff;}
a:hover {text-decoration: underline;}
table {border-collapse: collapse;}
.center {text-align: center;}
.center table { margin-left: auto; margin-right: auto; text-align: left;}
.center th { text-align: center !important; }
td, th { border: 1px solid #000000; font-size: 75%; vertical-align: baseline;}
h1 {font-size: 150%;}
h2 {font-size: 125%;}
.p {text-align: left;}
.e {background-color: #ccccff; font-weight: bold; color: #000000;}
.h {background-color: #9999cc; font-weight: bold; color: #000000;}
.v {background-color: #cccccc; color: #000000;}
i {color: #666666; background-color: #cccccc;}
img {float: right; border: 0px;}
hr {width: 600px; background-color: #cccccc; border: 0px; height: 1px; color: #000000;}
//--></style>
</head>
<body>

<div align=center>
<table border=0 cellspacing=0 cellpadding=0 style='border-collapse:collapse;'>
 <tr>
  <td valign=bottom style='border:solid black 1.0pt;background:#9999CC;padding:3.5pt 3.5pt 2.5pt 2.5pt;'>
   <span style='font-size:13.5pt;color:black'>PocketHPH <?=phpversion()?></span>
  </td>
 </tr>
</table>
</div>
<h2>Supported PHP functions</h2>
<ul>
<?

    foreach(sort(hph_all_functions()) as $func) {
    $fn2 = str_replace('_','-',$func);
    $fn2 = preg_replace('/\(.*$/', '', $fn2);
    // echo "$fn2\n";
    if (ereg('^[A-Z]', $func) || eregi('xslt|xml', $func))
      ;//echo "<li>$func</li>\n";
    else
      echo "<li><a href='http://php.net/manual/en/print/function.$fn2.php'>$func</a></li>\n";
  }

?>
</ul>
</body>
</html>