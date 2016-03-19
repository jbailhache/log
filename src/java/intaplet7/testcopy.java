
class testcopy
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
  String src = " &dup ^x {x x} { \"Bonjour !\" printout \"testfile.in\" \"rw\" file dup \"F:\" printout rep printout readall \"R:\" printout rep printout \",\" printout dup rep printout \"testfile.out\" \"rw\" file \" F out:\" printout dup rep printout write \".\" printout AuRevoir HALT } ";
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
