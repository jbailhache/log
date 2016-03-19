<HTML>
<head>
<title>Création d'une nouvelle discussion</title>
</head>
<body bgcolor=navy link=red vlink=yellow text=cyan>

<center>
<table cellpadding=10 border=1 bordercolor=red>
<tr><td bgcolor=C0B030>
<center>
<font face=arial size=5 color=navy>Création d'une nouvelle discussion</td></tr></table>
 
<form method=post action=creer.php3>

<table>
<tr>
<td>Nom de la discussion : </td>
<td><input type=text name=discussion value=""></td>
</tr><tr>
<td>Votre nom : </td>
<td><input type=text name=nom value=""></td>
</tr><tr>
<td>Phrase d'introduction :</td>
<td><input type=text name=introduction size=40 value="Bonjour, je vous propose de commencer la discussion."></td>
</tr><tr>
<td>Phrase de conclusion :</td>
<td><input type=text name=conclusion size=40 value="Merci et au revoir."></td>
</tr></table>
<p>Discussion 
<input type=radio name=statut value="pub" checked> 
publique ou 
<input type=radio name=statut value="priv"> privée.

<p>

<input type=submit value="Créer la discussion">

</form>

</body>
</HTML>
