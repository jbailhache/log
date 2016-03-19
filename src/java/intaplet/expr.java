 
public class expr
{
 public String noeud;
 public double valeur;
 public String chaine;
 public Object objet;
 public int arite;
 public expr sousexpr[];
 public String carspec = " #\"\\;:()[]{}^%&";

 expr (String nd, double x, String s, Object o, int a, expr[] se)
 {
  noeud = nd;
  valeur = x;
  chaine = s;
  objet = o;
  arite = a;
  sousexpr = se;
 }

 expr (String nd, double x, String s, int a, expr[] se)
 {
  noeud = nd;
  valeur = x;
  chaine = s;
  objet = null;
  arite = a;
  sousexpr = se;
 }

 expr (String nd)
 {
  noeud = nd;
  arite = 0;
  valeur = 0;
  chaine = "";
  objet = null;
 }

 expr (String nd, double x)
 {
  noeud = nd;
  valeur = x;
  chaine = "";
  objet = null;
  arite = 0;
 }

 expr (double x)
 {
  noeud = "NBR";
  valeur = x;
  chaine = "";
  objet = null;
  arite = 0;
 }

 expr (String nd, String s)
 {
  noeud = nd;
  valeur = 0;
  chaine = s;  
  objet = null;
  arite = 0;
  
 }

 expr (String nd, Object o)
 {
  noeud = nd;
  valeur = 0;
  chaine = "";
  objet = o;
  arite = 0;
 }

 expr (Object o)
 {
  noeud = "OBJ";
  valeur = 0;
  chaine = "";
  objet = o;
  arite = 0;
 }

 expr (String nd, int a, expr[] se)
 {
  noeud = nd;
  valeur = 0;
  chaine = "";
  objet = null;
  arite = a;
  sousexpr = se;
 }

 expr (String nd, int a)
 {
  noeud = nd;
  valeur = 0;
  chaine = "";
  objet = null;
  arite = a;
  sousexpr = new expr[a];
  for (int i=0; i<a; i++)
   sousexpr[i] = new expr ("NIL");

 }

 expr (String nd, expr x)
 {
  noeud = nd;
  valeur = 0;
  chaine = "";
  objet = null;
  arite = 1;
  sousexpr = new expr[1];
  sousexpr[0] = x;
 }

 expr (String nd, expr x, expr y)
 {
  noeud = nd;
  valeur = 0;
  chaine = "";
  objet = null;
  arite = 2;
  sousexpr = new expr[2];
  sousexpr[0] = x;
  sousexpr[1] = y;
 }

 expr (String nd, expr x0, expr x1, expr x2)
 {
  noeud = nd;
  valeur = 0;
  chaine = "";
  objet = null;
  arite = 3;
  sousexpr = new expr[3];
  sousexpr[0] = x0;
  sousexpr[1] = x1;
  sousexpr[2] = x2;
 }

 expr (String nd, expr x0, expr x1, expr x2, expr x3)
 {
  noeud = nd;
  valeur = 0;
  chaine = "";
  objet = null;
  arite = 4;
  sousexpr = new expr[4];
  sousexpr[0] = x0;
  sousexpr[1] = x1;
  sousexpr[2] = x2;
  sousexpr[3] = x3;
 }

 expr (String nd, expr x0, expr x1, expr x2, expr x3, expr x4)
 {
  noeud = nd;
  valeur = 0;
  chaine = "";
  objet = null;
  arite = 5;
  sousexpr = new expr[5];
  sousexpr[0] = x0;
  sousexpr[1] = x1;
  sousexpr[2] = x2;
  sousexpr[3] = x3;
  sousexpr[4] = x4;
 }

 expr (String nd, expr x0, expr x1, expr x2, expr x3, expr x4, expr x5)
 {
  noeud = nd;
  valeur = 0;
  chaine = "";
  objet = null;
  arite = 6;
  sousexpr = new expr[6];
  sousexpr[0] = x0;
  sousexpr[1] = x1;
  sousexpr[2] = x2;
  sousexpr[3] = x3;
  sousexpr[4] = x4;
  sousexpr[5] = x5;
 }




 void modarite (int n)
 {
  arite = n;
  sousexpr = new expr[n];
 } 

 expr niveau (int n)
 {
  expr x = this;
  for (int i=0; i<n; i++)
   x = x.sousexpr[1];
  return x.sousexpr[0];
 }

 String rep ()
 { 
  /*return rep (" #\"\\;:()[]{}^%");*/
  return rep (carspec);
 }

 String rep (String carspec)
 {
  if (noeud.equals ("NBR"))
   /* return "#" + valeur + "#"; */
   return ("" + carspec.charAt(1)) + valeur + ("" + carspec.charAt(1));
  if (noeud.equals ("STR"))
   /* return "\"" + chaine + "\""; */
   return carspec.charAt(2) + chaine + carspec.charAt(2);
  if (arite == 0 && valeur == 0 && chaine == "")
   return noeud;
  String r = /*"("*/ "" + carspec.charAt(6);
  r += noeud;
  if (valeur != 0)
  {
   /*System.out.println ("valeur="+valeur);*/
   /*r += " #" + valeur + "#";*/
   r += /* carspec.charAt(0) + */ "" + carspec.charAt(1) + valeur + carspec.charAt(1);
  }
  if (!chaine.equals(""))
   /*r += " \"" + chaine + "\"";   */
   r += /*carspec.charAt(0) + */ "" + carspec.charAt(2) + chaine + carspec.charAt(2);
  if (objet != null)
   r += " *";
  for (int i=0; i<arite; i++)
   /*r += " " + sousexpr[i].rep();*/
   r += carspec.charAt(0) + sousexpr[i].rep();
  r += /*")"*/ carspec.charAt(7);
  return r;
 }

}
 
