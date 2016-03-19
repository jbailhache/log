
class testmap
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
/*  String src = " &dup ^x {x x} { \"Bonjour !\" printout \"testfile.java\" \"rw\" file dup \"F:\" printout rep printout readall \"R:\" printout rep printout \",\" printout rep printout \".\" printout AuRevoir HALT } ";*/

	String src = "";
	src += "&cons ^x ^y ";
	src += "{ [mkexpr (quote SEQ) #2#] ^z {~ ";
	src += "  [rplac z #0# x] ";
	src += "  [rplac z #1# y] ";
	src += "  z } ";
	/*src += "{ [print rep cons aaa bbb] bye HALT }";*/
	src += "&mapr ^rmap ^l ^f ";
	src += "{ [sign plus arity l #-2#] #-1# ";
	src += "  (~eqthen l ";
	src += "  [cons eval f subexpr l #0# ";
	src += "   eval dup rmap subexpr l #1# f] } ";
	src += "&map [mapr (quote mapr)] ";

	src += "&mapvect ^v ^f ";
	src += "{ [mkexpr VECT arity v] ^r {~ ";
	src += "  #0# ^i (for [plus arity v #-1#] ";
	src += "          [rplac r i eval f subexpr v i]) ";
	src += "  r }";
	src += "&contientr ^rcontient ^l ^f ";
	src += "{ [equal #2# arity l] ";
	src += "  (~ select #0# ";
	src += "  (~ or [print rep dup print \"found:\" eval f subexpr l #0# ] ";
	src += "  [~ eval rcontient rcontient subexpr l #1# f print \" ... \" } ";
	src += "&contient [contientr (quote contientr)] ";
	src += "&l (quote (SEQ a (SEQ b (SEQ c nil)))) ";
	src += "{ ";
	src += "[print rep sign plus print rep dup arity l #-2#] ";
	src += "[print rep l] ";
	src += "[print rep map l (quote ^x (MAP x))] "; 
	/*src += "[print rep mapr (quote mapr) l (quote ^x (MAP x))] ";*/
	src += "(VECT aaa bbb ccc) ^v1 {~ ";
	src += "[print rep v1] ";
	src += "[print rep mapvect v1 (quote ^x (MAP x))] ";
	src += "[print rep (quote contient)] ";
      src += "[print \"\nTest contient: \"] ";
	src += "[print rep contient (quote { (REC a aaa) (REC b bbb) (REC c ccc) }) ";
	src += " (quote ^x (and [equal subexpr x #0# b] (FOUND#1# [subexpr x #1#]))) ] ";
	src += " bye HALT }";
/*
	src += "&l (quote (SEQ a (SEQ b (SEQ c nil)))) ";
	src += "{ [print rep map l ^x (MAP x)] ";
	src += "  [print rep map (VECT a b c) ^x (MAP x)] ";
      src += "}";
*/

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
