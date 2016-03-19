
class testfaxdata
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

  String src = " &dup ^x { x x } { ";
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
  src += " [printout \"\\nwrite FAXDATA: \"] ";
  src += " [printout rep write os \"FAXDATA\\nAODGCLFK\\n\"] ";
  src += " (loop {~ ";
  src += " [printout \"\\nreadline 3: \"] ";
  src += " [printout rep dup readline is] ";
  src += " \"FAXDATA\" (~ eqthen { ";
  src += " [printout \"\\npage \"] ";
  src += " [printout readline is] ";
  src += " [printout \"\\ntaille \"] ";
  src += " [printout dup readline is] ";
  src += " ^str [compil concat \"#\" concat str \"#\"] ^n {~ ";
  src += " [printout \"\\ntaille \"] [printout rep n] ";
  src += " [drop readbytes is n] ";
  src += " [drop readline is] ";
  src += "  } exit ) ";  
 
  src += " [printout \"\\nlogout \"] ";
  src += " [printout rep write os \"END\\n\"] ";
  src += " [printout rep close sock1] ";
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
