
class testmailnew
{
 public static void main (String[] args)
 {
  interp intrp = new interp();
/*
  String topctxrep = "(CTX ;(loop ;(readin);(compil)(eval)) (HALT) (EMPTY) (EMPTY) (EMPTY))";
  entree e = new entree (topctxrep);
  expr topctx = e.lirexpr();
  System.out.println ("Toplevel: " + topctx.rep());
*/
/*  String src = ";(loop ;(readin);(compil)(eval)) (HALT)";*/
  /*String src = " &dup ^x {x x} &swap ^x ^y { x y } { \"Bonjour !\" printout  \" S:\" printout #6029# \"localhost\"  socket dup rep printout \" D:\" printout dataoutputstream dup rep printout \" W:\" printout \"TEST SOCKET\n\" swap writebytes rep printout AuRevoir HALT } ";*/

/*  String src = " { [print rep sendtcp \"localhost\" #6029# \" { \\\"Test\\\" print bye HALT } \"] } ";*/

/*	String src = " { [message \"Test\" \"Bonjour\" #300# #100#] } ";*/
	
	String src = " { [mail \"smtp.noos.fr\" \"jacques.bailhache@lycos.fr\" (TO \"j.bailhache@amplitude.fr\") \"Test mail\" \"Ceci est un test\" (PAGES \"aaa\" \"bbb\" \"ccc\") \".dat\"] } "; 

/*	String src = " { [getpages \"localhost\" #6020# \"admin\" \"padmin\" (FAXINFO #0.0# #0.0# \"Unknown ID\" \"\" \"\" #6.0# #0.0# #33.0# #1.0# \"12040910\" \"04/12/2002-09:10:38\" \"04/12/2002-09:10:38\") ] } ";*/



 
  entree e = new entree (src);
  expr top = e.lirexpr();
  System.out.println ("Toplevel: " + top.rep());
 
  expr global = new expr ("GLOBAL", 10);
  intrp.global = global;
  /*intrp.carspec = " ='!-/[]()<>*%";*/
  System.out.println ("Démarrage de l'interprète...");
  /*intrp.runctx (topctx);*/
  expr r = intrp.run (top, new expr("Test"));
  System.out.println ("Result: " + r.rep());
 }
}

/* exemple :
 ;(quote ^x ;(quote (quote));x adse) ^kwote ;(quote xxx); kwote eval
*/
