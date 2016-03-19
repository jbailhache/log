
class testloop
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
  String src1 = " &dup ^x {x x} &swap ^x ^y { x y } { \"Bonjour !\" printout  \" S:\" printout #6029# \"localhost\"  socket dup rep printout \" D:\" printout dataoutputstream dup rep printout \" W:\" printout \"TEST SOCKET\n\" swap writebytes rep printout AuRevoir HALT } ";

  String src = " { ";
  src += " Test ^var {~ ";
  src += " #0# ^i loop ( ";
  src += " 

  String src2 = " &dup ^x { x x } { ";
  src += " [socket \"localhost\" #6020#] ^sock {~ ";
  src += " [printout \"socket: \"] [printout rep sock] ";
  src += " [datainputstream sock] ^is {~ ";
  src += " [printout \"\\ninput stream: \"] [printout rep is] ";
  src += " [dataoutputstream sock] ^os {~ ";
  src += " [printout \"\\noutput stream: \"] [printout rep os] ";
  src += " [printout \"\\nwrite login: \"] ";
  src += " [printout rep write os \"USER\\nadmin\\nPASS\\npadmin\\n\"] ";
  src += " [printout \"\\nreadline 1: \"] ";
  src += " [printout rep readline is] ";
  src += " [printout \"\\nreadline 2: \"] ";
  src += " [printout rep readline is] ";
  src += " [printout \"\\nwrite LISTREC: \"] ";
  src += " [printout rep write os \"LISTREC\\n\\n\"] ";
  src += " [printout \"\\nreadline 3: \"] ";
  src += " [printout rep readline is] ";
  src += " [printout \"\\nreadline 4: \"] ";
  src += " [printout rep readline is] ";
  src += "  [printout \"\\nreadline 5: \\n\"] ";
  src += "  [printout rep readline is] ";  
  /*
  src += "  [printout \"\\nreadline 6: \\n\"] ";
  src += "  [printout rep readline is] ";  
  */
  /*src += " bye HALT ";*/
  src += " (loop {~ ";
  src += "  [printout \"\\nis=\"] [printout rep is] ";
  src += "  [printout \"\\nref is=\"] [printout rep (quote is)] ";
  /*src += "  [printout \"\\nargs=\"] [printout rep subexpr getctx #2#] ";*/
  src += " [printout \"\\narg1=\"] ";
  /*src += " [printout rep subexpr subexpr subexpr getctx #2# #1# #0#] ";*/
  src += " [printout rep (arg#1#)] ";
  src += "  [printout \"\\nreadline 8: \\n\"] ";
  /* src += "  [printout rep dup readline is] "; */
  /*
  src += "  [readline is] ^line {~ ";
  src += "  [printout rep line] line ";
  */
  src += "  [readline is] [printout rep dup] ";

  src += "  \"\" (eqthen { [printout \"\\nfin\"] exit } { [printout \"\\n...\"] } ) ";
  src += " ) ";
  src += " bye HALT } ";
  System.out.println (src);
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
