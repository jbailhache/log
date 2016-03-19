

import java.applet.*;
import java.awt.*;
import java.net.*;
import java.lang.*;
 

public class entree
{
 public char c;
 public String s;
 boolean skipped;
 public TextArea messages;
 public String carspec;

 entree (String cs)
 {
  /*
  c = cs.charAt(0);
  s = cs.substring(1,cs.length()-1);
  */
  carspec = " #\"\\;:()[]{}^%&";
  c = 0;
  s = cs + ".";
  suivant();
 }

 void suivant1 ()
 {
  /*skipped = false;*/
  
   /*System.out.println ("s:" + s + ".");*/
   if (s.length() < 1)
   {
    System.out.println ("Erreur: entrée vide");  
    if (messages != null)
     messages.setText (messages.getText() + "\nError: Unexpected end of source.");
    c = (char)-1;
   }
   c = s.charAt(0);
   System.out.print(c);
   s = s.substring (1, s.length());
    
  
  /*System.out.print(c);*/

 }
 
/*

 void suivant ()
 {
  skipped = false;
  for (;;)
  {
    if (s.length() < 1)
   {
    System.out.println ("Erreur: entrée vide");  
    c = (char)-1;
    if (messages != null)
     messages.setText (messages.getText() + "\nError: Unexpected end of source.");
    break;
   }
   c = s.charAt(0);
   System.out.print(c);
   s = s.substring (1, s.length());
   if (!(c == ' ' || c == '\t' || c == '\n' || c == '\r' || c == carspec.charAt(0)))
    break;
   else
    skipped = true;
  }
 
 }
 


*/

/*
 void suivant ()
 {
  skipped = false;
  for (;;)
  {
   suivant1();
   if (!(c == ' ' || c == '\t' || c == '\n' || c == '\r' || c == carspec.charAt(0)))
    break;
   else
    skipped = true;
  }
 }
 */

 void suivant2 ()
 {
  suivant1();
  if (c == carspec.charAt(13))
  {
   for (;;)
   {
    suivant1();
    if (c == carspec.charAt(13))
     break;
   }
   suivant1();
  }
 }

 void suivant ()
 {
  skipped = false;
  for (;;)
  {
   suivant2();
   if (!(c == ' ' || c == '\t' || c == '\n' || c == '\r' || c == carspec.charAt(0)))
    break;
   else
    skipped = true;
  } 
 }



 void suivantb ()
 {
  skipped = false;
  for (;;)
  {
   /*System.out.println ("s:" + s + ".");*/
   if (s.length() < 1)
   {
    System.out.println ("Erreur: entrée vide");  
    c = (char)-1;
    if (messages != null)
     messages.setText (messages.getText() + "\nError: Unexpected end of source.");
    break;
   }
   c = s.charAt(0);
   System.out.print(c);
   s = s.substring (1, s.length());
   if (c == carspec.charAt(13))
   {
    skipped = true;
    if (s.length() < 1)
    {
     System.out.println ("Erreur: entrée vide");  
     c = (char)-1;
     if (messages != null)
      messages.setText (messages.getText() + "\nError: Unexpected end of source.");
     break;
    }
    c = s.charAt(0);
    s = s.substring (1, s.length());
    if (c == carspec.charAt(13))
    {
     if (s.length() < 1)
     {
      System.out.println ("Erreur: entrée vide");  
      c = (char)-1;
      if (messages != null)
       messages.setText (messages.getText() + "\nError: Unexpected end of source.");
      break;
     }
     c = s.charAt(0);
     s = s.substring (1, s.length());
     break;
    }
   }
   if (!(c == ' ' || c == '\t' || c == '\n' || c == '\r' || c == carspec.charAt(0)))
    break;
   else
    skipped = true;
  }
  /*System.out.print(c);*/

 }
 
 char carlu()
 {
  return c;
 }

 String liresym ()
 {
  String z = "";
  for (;;)
  {
   if (!((carlu() >= 'a' && carlu() <= 'z') ||
         (carlu() >= 'A' && carlu() <= 'Z') ||
         (carlu() >= '0' && carlu() <= '9') ||
	   carlu() == '_'))
   {
    /*System.out.println("symbole:"+z);*/
    return z;
   }
   z = z + carlu();
   suivant();
   if (skipped)
    return z; 
  }
  
 }

 String lirechaine ()
 {
  String z = "";
  suivant1();
  for (;;)
  {
   if (carlu() == /*'\\'*/ carspec.charAt(3))
   {
    suivant1();
    if (carlu() == 'n')
     z = z + '\n';
    else if (carlu() == 'x')
    {
     suivant1();
     char c1 = carlu();
     suivant1();
     char c2 = carlu();
     char[] hex = new char[2];
     hex[0] = c1;
     hex[1] = c2;
     String shex = new String(hex);
     /*byte b = Byte.parseByte (shex, 16); */
     int x = Integer.parseInt (shex, 16);
     byte b = (byte)x;
     byte[] a = new byte[1];
     a[0] = b;
     try
     {
      z = z + new String(a,"ISO-8859-1");
     }
     catch (Exception e) {}
    }
    else
     z = z + carlu();
    suivant1();
   }
   else if (carlu() == /*'"'*/ carspec.charAt(2))
   {
    suivant();
    return z;
   }
   else if (carlu() == (char)-1)
   {
    return z;
   }
   else
   {
    z = z + carlu();
    suivant1(); 
   }
  }
 }

 double lirevaleur ()
 {
  String z = "";
  suivant();
  for (;;)
  {
   if (carlu() == /*'#'*/ carspec.charAt(1) || carlu() == (char)-1)
   {
    suivant();
    Double x = new Double(z);
    double y = x.doubleValue();
    return y;
   }    
   z = z + carlu();
   suivant(); 
  }
 }

 expr subvar (int i, String v, expr x, boolean ev)
 {
  if (x.noeud.equals(v))
  {
   if (ev)
    return new expr ("SEQ", new expr ("arg", (double)i), new expr("eval"));
   else
    return new expr ("arg", (double)i);
  }
  else if (x.noeud.equals("LDB"))
  {
   expr y = subvar (i+1, v, x.sousexpr[0], ev);
   return new expr ("LDB", y);
  }
  else
  {
   expr se[] = new expr[x.arite];
   for (int j=0; j<x.arite; j++)
    se[j] = subvar (i, v, x.sousexpr[j], ev);
   expr y = new expr (x.noeud, x.valeur, x.chaine, x.objet, x.arite, se);
   return y;
  }
 }

 expr lambda (String v, expr x)
 {
  expr y = subvar (0, v, x, false);
  /*expr z = new expr ("SEQ", new expr("in"), new expr ("SEQ", y, new expr("ex")));*/
  expr z = new expr ("LDB", y);
  return z;
 }

 expr elambda (String v, expr x)
 {
  expr y = subvar (0, v, x, true);
  /*expr z = new expr ("SEQ", new expr("in"), new expr ("SEQ", y, new expr("ex")));*/
  expr z = new expr ("LDB", y);
  return z;
 }

 public boolean equal (expr x, expr y)
 { 
  if (!x.noeud.equals(y.noeud))
   return false;
  if (x.valeur != y.valeur)
   return false;
  if (!x.chaine.equals(y.chaine))
   return false;
  if (x.arite != y.arite)
   return false;
  if (x.objet != y.objet) 
   return false;
  for (int i=0; i<x.arite; i++)
  {
   if (!equal(x.sousexpr[i], y.sousexpr[i]))
    return false;
  }
  return true;
 }


 public expr subst (expr ca, expr par, expr dans)
 {
  if (equal (ca, dans))
   return par;
  else
  {
   expr se[] = new expr[dans.arite];
   for (int j=0; j<dans.arite; j++)
    se[j] = subst (ca, par, dans.sousexpr[j]);
   expr y = new expr (dans.noeud, dans.valeur, dans.chaine, 
	dans.objet, dans.arite, se);
   return y;
  }    
 }

 expr lirexpr ()
 {
  expr x = lirexpr1();
  x.carspec = carspec;
  return x;
 }

 expr lirexpr1 ()
 {
  String node;
  double val;
  String str;
  int arite;
  expr[] sousexpr1;
  expr[] sousexpr;
  char c1;
  boolean auto = false;

  if (carlu() == /*'('*/ carspec.charAt(6) || carlu() == /*':'*/ carspec.charAt(5))
  {
   c1 = carlu();
   suivant ();
   if (carlu() == '~')
   {
    auto = true;
    suivant();
   }

   node = liresym();

   val = 0;
   str = "";

   if (carlu() == '#' && !skipped)
   {
    val = lirevaleur();
    /*System.out.println ("val="+val);*/
   }
   else
    val = 0;
   if (carlu() == '"' && !skipped)
    str = lirechaine();
   else
    str = "";

   
   arite = 0;
   sousexpr1 = new expr[30];
   for (;;)
   {
    /*System.out.println ("c:"+c);*/
    if (carlu() == /*')'*/ carspec.charAt(7) || carlu() == (char)-1 ||
	(auto && ( carlu() == carspec.charAt(9) || carlu() == carspec.charAt(11) ) ))
     break;
    sousexpr1[arite++] = lirexpr();
   }
   if ((c1 == /*'('*/ carspec.charAt(6)) && !auto)
    suivant();
   sousexpr = new expr[arite];
   for (int i=0; i<arite; i++)
    sousexpr[i] = sousexpr1[i];
   return new expr (node, val, str, arite, sousexpr);
  }
  else if (carlu() == /*';'*/ carspec.charAt(4))
  {
   suivant();
   expr x = lirexpr();
   expr y = lirexpr();
   expr z = new expr ("SEQ", x, y);
   return z;
  }
  else if ((carlu() >= 'a' && carlu() <= 'z') ||
	     (carlu() >= 'A' && carlu() <= 'Z') ||
		carlu() == '_' )
  {
   node = liresym();
   return new expr(node);  
  }
  else if (carlu() == /*'#'*/ carspec.charAt(1))
  {
   double x = lirevaleur();
   return new expr(x);
  }
  else if (carlu() == /*'"'*/ carspec.charAt(2))
  {
   String s = lirechaine();
   return new expr ("STR", s);
  }
  else if (carlu() == /*'^'*/ carspec.charAt(12))
  {
   suivant();
   String v = liresym();
   expr x = lirexpr();
   expr y = lambda (v, x);
   return y;
  }
  else if (carlu() == carspec.charAt(14))
  {
   suivant();
   /* String v = liresym(); */
   expr v = lirexpr();
   expr f = lirexpr();
   expr x = lirexpr();
   /* expr y = new expr ("SEQ", new expr ("qt", f), elambda (v, x)); */
   expr y = subst (v, f, x);
   return y;
  }
  else if (carlu() == /*'['*/ carspec.charAt(8))
  {
   auto = false;
   suivant();
   if (carlu() == '~')
   {
	auto = true;
	suivant();
   }
   expr x = lirexpr();
   expr y;
   for (;;)
   {
    if (carlu() == /*']'*/ carspec.charAt(9) || carlu() == (char)-1 || 
	(auto && (carlu() == carspec.charAt(7) || carlu() == carspec.charAt(11))))
    {
     if (!auto)
	     suivant();
     return x;
    }
    y = lirexpr();
    x = new expr ("SEQ", y, x);
   }
  }
  else if (carlu() == /*'{'*/ carspec.charAt(10))
  {
   auto = false;
   suivant();
   if (carlu()=='~')
   {
	auto = true;
	suivant();
   }
   expr x = lirexpr();
   expr y;
   for (;;)
   {
    if (carlu() == /*'}'*/ carspec.charAt(11) || carlu() == (char)-1 ||
        ( auto && ( carlu()==carspec.charAt(7) || carlu()==carspec.charAt(9) )))
    {
	if (!auto)
	     suivant();
     return x;
    }
    y = lirexpr();
    x = new expr ("SEQ", x, y);
   }
  }
  else if (carlu() == (char)-1)
  {
   return new expr ("ERROR", 
			  new expr ("STR", "Unexpected end of source"), 
                    new expr ("NIL"));
  }
  /*suivant(); 
  return new expr ("ERROR", 
                   new expr ("STR", "Invalid character"), 
                   new expr((double) carlu())
	             );*/
  char c = carlu();
  suivant();
  return new expr ("" + c);
 }
 
}
