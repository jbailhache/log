
class testmail
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
  String src = " &dup ^x {x x} { \"Bonjour !\" printout \"Test mail\" \"jbailhache@amplitude.fr\" \"smtp.noos.fr\" mailmsg  ^m { \" M:\" printout m rep printout \" AR:\" printout \"jacques.bailhache@lycos.fr\" m addrecipient printout \" BODY:\" printout \"Ceci est un test de mail\" \"text\" \"\" \"\" \"\" \"\" m addpart printout \" ATT:\" printout \"Ceci est un attachement\" \"binary\" \"f1.dat\" \"f1.dat\" \"Application\" \"Octet-Stream\" m addpart printout \" S:\" printout m sendmail printout \" END\" printout }  AuRevoir HALT } ";
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
