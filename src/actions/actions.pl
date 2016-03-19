#!/usr/bin/perl
print "
import java.io.*;
import java.text.*;
import java.util.*;
";
if ($ARGV[0] eq "new") { print "
public class ActionsNew";
} else { print "
public class Actions";
} print "
{
 static String dossier;
 static Parametres params;
 static int maxActions = 1000;
 static int maxCours = 100000;
 static int nActions;
 static String[] nomsActions;
 static int nActionsPrerenseignees;
 static String[] actionsPrerenseignees;
 static int nCours;
 static Cours[] coursActions;
 static String actionDetenue = \"\";

 static int nEnregs;
 static DateHeure[] enregs;
 
 static int anneeRef, moisRef, jourRef, heureRef, minRef;
 static int anneeDebut, moisDebut, jourDebut, heureDebut, minDebut;

 static String choixMenu;

 static FilenameFilter filter = new FilenameFilter ()
 {
  public boolean accept (File dir, String name)
  {
   return name.endsWith(\"-cours.txt\");
  }
 };

 static FilenameFilter filterAPR = new FilenameFilter ()
 {
  public boolean accept (File dir, String name)
  {
   return name.endsWith(\"-apr.txt\");
  }
 };


 static boolean verifierDateHeure (int annee, int mois, int jour, int heure, int min)
 {
  Calendar cal = Calendar.getInstance();
  cal.setLenient(false);
  try
  {
   cal.set (annee, mois-1, jour, heure, min, 0);
   Date d = cal.getTime();
   return true;
  }
  catch (Exception e)
  {
   return false;
  }
 }

 static int ecartDates (int annee1, int mois1, int jour1, int annee2, int mois2, int jour2)
 {
  Calendar cal1 = Calendar.getInstance();
  Calendar cal2 = Calendar.getInstance();
  cal1.setLenient(false);
  cal2.setLenient(false);
  try 
  {
   cal1.set (annee1, mois1, jour1, 12, 0, 0);
   cal2.set (annee2, mois2, jour2, 12, 0, 0);
   Date d1 = cal1.getTime();
   Date d2 = cal2.getTime();
   long t1 = d1.getTime();
   long t2 = d2.getTime();
   long ms = t2 - t1;
   long nj = ms / (24 * 60 * 60 * 1000);
   return (int)nj;
  }
  catch (Exception e)
  {
   return 0;
  }
 }

 static double lireCoursWeb (String action)
 {
  try
  {
   HTTPLoader hl = new HTTPLoader();
   String reponse = hl.getTextFile(\"http://finance.yahoo.com/d/quotes.csv?s=\" + action + \"&f=l1\");
   double cours = Double.parseDouble(reponse);
   return cours;
  }
  catch (Exception e)
  {
   return 0.0;
  }
 }

"; if ($ARGV[0] eq "new") { print "

 static String lireActionsWeb (String symbole)
 {
  try
  {
   HTTPLoader hl = new HTTPLoader();
   String url = \"http://finance.yahoo.com/d/quotes.csv?s=%40%5E\" + symbole + \"&f=sn\";
   System.out.println (\"url=\"+url);
   String reponse = hl.getTextFile(url);
   // System.out.println(\"reponse=\"+reponse);
   return reponse;
  }
  catch (Exception e)
  {
   return \"\";
  }
 }

"; } print "

 static void effacer ()
 {
  nActions = 0;
  nCours = 0;
  System.out.println (\"Les donnees ont ete effacees\");
 }

 public static void modifierParam (String nom, String valeur, boolean afficher)
 {
    // System.out.println (\"modifierParam nom=\" + nom + \" valeur=\" + valeur);
    if (nom.equals(\"nDatesChoix\"))
     params.nDatesChoix = Integer.parseInt(valeur);
    else if (nom.equals(\"seuil\"))
     params.seuil = Double.parseDouble(valeur);
    else if (nom.equals(\"ordre\"))
     // params.ordre = valeur;
    {
     if (\"saisie\".startsWith(valeur.toLowerCase()))
      params.ordre = \"saisie\";
     else if (\"alphabetique\".startsWith(valeur.toLowerCase()))
      params.ordre = \"alphabetique\";
     else if (\"progression\".startsWith(valeur.toLowerCase()))
      params.ordre = \"progression\";
     else
      params.ordre = valeur;
    }
    else if (nom.equals(\"ordreActions\"))
    {
     if (\"saisie\".startsWith(valeur.toLowerCase()))
      params.ordreActions = \"saisie\";
     else if (\"alphabetique\".startsWith(valeur.toLowerCase()))
      params.ordreActions = \"alphabetique\";
     else
      params.ordreActions = valeur;
    }
    else if (nom.equals(\"nEnregs\"))
     params.nEnregs = Integer.parseInt(valeur);
    else
    {
     System.out.println (\"Le parametre \" + nom + \" n'existe pas.\");
     return;
    }
    // System.out.println (\"Fin modifierParam\");
    if (afficher)
    {
     System.out.print (\"Parametre modifie : nouvelle valeur : \");
     if (nom.equals(\"nDatesChoix\"))
      System.out.print (new Integer(params.nDatesChoix).toString());
     else if (nom.equals(\"seuil\"))
      System.out.print (new Float(params.seuil).toString());
     else if (nom.equals(\"ordre\"))
      System.out.print (params.ordre);
     else if (nom.equals(\"ordreActions\"))
      System.out.print (params.ordreActions);
     else if (nom.equals(\"nEnregs\"))
      System.out.print (params.nEnregs);
     System.out.println(\".\");  
    }
 }

 static void gererParametres ()
 {
  try
  {
   String reponse;
   BufferedReader in = new BufferedReader( new InputStreamReader( System.in ));
   System.out.println (\"Parametres\");
   System.out.print (\" Nombre de dates pour choix date de reference : \"+Integer.toString(params.nDatesChoix) + \" ? \");
   reponse = in.readLine();
   if (!(reponse.equals(\"\")))
    modifierParam (\"nDatesChoix\", reponse, true);
   System.out.print (\" Seuil : \" + Double.toString(params.seuil) + \" ? \");
   reponse = in.readLine();
   if (!(reponse.equals(\"\")))
    modifierParam (\"seuil\", reponse, true);
   System.out.print (\" ordre d'affichage des cours (saisie, alphabetique ou progression) : \" + params.ordre + \" ? \");
   reponse = in.readLine();
   if (!(reponse.equals(\"\")))
    modifierParam (\"ordre\", reponse, true); 
   System.out.print (\" ordre d'affichage des noms des actions (saisie ou alphabetique) : \" + params.ordreActions + \" ? \");
   reponse = in.readLine();
   if (!(reponse.equals(\"\")))
    modifierParam (\"ordreActions\", reponse, true); 
   System.out.print (\" nombre d'enregistrements a afficher : \" + params.nEnregs + \" ? \");
   reponse = in.readLine();
   if (!(reponse.equals(\"\")))
    modifierParam (\"nEnregs\", reponse, true);
  }
  catch (Exception e)
  {
   System.out.println (\"Erreur dans gererParametres\");
  }
 }

 static int lireNomsActions ()
 {
  try 
  {
   String reponse;
   BufferedReader in = new BufferedReader( new InputStreamReader( System.in ));
   int i;
   for (;;)
   {
    System.out.print (\"Nom de l'action ? \");
    reponse = in.readLine();
    if (reponse.length() == 0)
    {
     System.out.println (\"Fin de la lecture des noms des actions\");
     break;
    }
    else
    {
     nomsActions[nActions++] = reponse;
    }
   }
   return nActions;
  }
  catch (Exception e)
  {
   System.out.println (\"Erreur dans lireNomsActions : \" + e.getMessage());
   return nActions;
  }
 }

 static void afficherNomsActions ()
 {
  int i;
  if (params.ordreActions.equals(\"saisie\"))
  {
   for (i=0; i<nActions; i++)
    System.out.println (\" - \" + nomsActions[i]);
  }
  else if (params.ordreActions.equals(\"alphabetique\"))
  {
   String[] nomsActionsTrie;
   int j, k;
   boolean trouve;
   nomsActionsTrie = new String[maxActions];
   for (i=0; i<nActions; i++)
   {
    trouve = false;
    for (j=0; j<i; j++)
    {
     if (nomsActions[i].compareTo(nomsActionsTrie[j]) <= 0)
     {
      for (k=i; k>j; k--)
       nomsActionsTrie[k] = nomsActionsTrie[k-1];
      nomsActionsTrie[j] = nomsActions[i];
      trouve = true;
      break;
     }
    }
    if (!trouve)
     nomsActionsTrie[i] = nomsActions[i];
   }
   for (i=0; i<nActions; i++)
    System.out.println (\" - \" + nomsActionsTrie[i]);
  }
 }

 static void supprimerAction ()
 {
  try
  {
   int i, j;
   String reponse;
   BufferedReader in = new BufferedReader( new InputStreamReader( System.in ));
   j = 0;

   if (params.ordreActions.equals(\"saisie\"))
   {
    for (i=0; i<nActions; i++)
     System.out.println (new Integer(i+1).toString() + \" : \" + nomsActions[i]);
    System.out.print (\"Numero de l'action a supprimer ? \");
    reponse = in.readLine();
    j = Integer.parseInt(reponse);
   }
   else if (params.ordreActions.equals(\"alphabetique\"))
   {
    String[] nomsActionsTrie;
    int j1, k, l, m;
    boolean trouve;
    nomsActionsTrie = new String[maxActions];
    for (i=0; i<nActions; i++)
    {
     // System.out.println (\"i=\"+new Integer(i).toString());
     trouve = false;
     for (j1=0; j1<i; j1++)
     {
      if (nomsActions[i].compareTo(nomsActionsTrie[j1]) <= 0)
      {
       for (k=i; k>j1; k--)
        nomsActionsTrie[k] = nomsActionsTrie[k-1];
       nomsActionsTrie[j1] = nomsActions[i];
       trouve = true;
       break;
      }
     }
     if (!trouve)
      nomsActionsTrie[i] = nomsActions[i];
    }
    for (i=0; i<nActions; i++)
     System.out.println (new Integer(i+1).toString() + \" : \" + nomsActionsTrie[i]);
    System.out.print (\"Numero de l'action a supprimer ? \");
    reponse = in.readLine();
    l = Integer.parseInt(reponse);
    l = l-1;
    for (m=0; m<nActions; m++)
    {
     if (nomsActions[m].equals(nomsActionsTrie[l]))
      j = m+1;
    }
   }

   if ((j >= 1) && (j <= nActions))
   {
    j = j-1;
    for (i=j; i<nActions-1; i++)
     nomsActions[i] = nomsActions[i+1];
    nActions--;
   }
   else
   {
    System.out.println (\"Numero d'action incorrect\");
   }
  }
  catch (Exception e)
  {
   System.out.println (\"Numero d'action incorrect\");
  }
 }

 static int lireCoursActions (boolean ref)
 {
  try
  {
   String reponse;
   Cours cours; 
   BufferedReader in = new BufferedReader( new InputStreamReader( System.in ));
   int i, j;
   boolean trouve;
   System.out.println (\"Tapez Entree pour abandonner et revenir au menu.\");
   int annee, mois, jour, heure, min;
   for (;;)
   {
    System.out.print (\"Annee   ? \");
    reponse = in.readLine();
    annee = Integer.parseInt(reponse);
    System.out.print (\"Mois    ? \");
    reponse = in.readLine();
    mois = Integer.parseInt(reponse);
    System.out.print (\"Jour    ? \");
    reponse = in.readLine();
    jour = Integer.parseInt(reponse);
    System.out.print (\"Heure   ? \");
    reponse = in.readLine();
    heure = Integer.parseInt(reponse);
    System.out.print (\"Minutes ? \");
    reponse = in.readLine();
    min = Integer.parseInt(reponse);
    boolean verif = verifierDateHeure (annee, mois, jour, heure, min);
    if (verif)
     break;
    else
     System.out.println (\"Date incorrecte, retapez la date.\");
   }

   if (ref)
   {
    anneeRef = annee;
    moisRef = mois; 
    jourRef = jour;
    heureRef = heure;
    minRef = min;
   }
   for (i=0; i<nActions; i++)
   {
    cours = new Cours();
    cours.annee = annee;
    cours.mois = mois;
    cours.jour = jour;
    cours.heure = heure;
    cours.min = min;
    cours.nomAction = nomsActions[i];
    System.out.print (\"Cours de l'action \" + nomsActions[i] + \" ? \");
    reponse = in.readLine();
    reponse = reponse.replace(',','.');
    if (!reponse.equals(\"\"))
    {
     cours.cours = Float.valueOf(reponse).floatValue();
     trouve = false;
     for (j=0; j<nCours; j++)
     {
      if ((coursActions[j].annee == annee) &&
          (coursActions[j].mois == mois) &&
          (coursActions[j].jour == jour) &&
          (coursActions[j].heure == heure) &&
          (coursActions[j].min == min) &&
          (coursActions[j].nomAction.equals(nomsActions[i])))
      {
       coursActions[j] = cours;
       trouve = true;
       break;
      }
     }
     if (!trouve)
      coursActions[nCours++] = cours;
    }
   }
   if (!ref)
   {
    afficherProgression (annee, mois, jour); 
    System.out.print (\"Voulez-vous que le \" + formatDateHeure(annee,mois,jour,heure,min) + \" devienne la nouvelle date de reference (o/n) ? \");
    reponse = in.readLine();
    if (reponse.charAt(0) == 'o' || reponse.charAt(0) == 'O')
    {
     anneeRef = annee;
     moisRef = mois;
     jourRef = jour;
     heureRef = heure;
     minRef = min;
    }
   }
   return nCours;
  }
  catch (Exception e)
  {
   // System.out.println (\"Erreur dans lireCoursActions : \" + e.getMessage());
   System.out.println (\"Operation abandonnee, retour au menu.\");
   return nCours;
  }
 }

 static String myFormat (String f, double x)
 {
  String s = new DecimalFormat(f).format(x);
  String t = \"\";
  int i;
  for (i=0; i<s.length(); i++)
  {
   if (s.charAt(i) == '0')
    t = t + \" \";
   else
   {
    t = t + s.substring(i);
    break;
   }
  }
  return t;
 }
   
 static String formatAction (String s)
 {
  int i;
  int size = 20;
  if (s.length() > size)
   return s.substring(0,size);
  else
  {
   String r;
   r = s;
   for (i=0; i<size-s.length(); i++)
    r = r + \" \";
   return r;
  }
 }

 static String formatDate (int annee, int mois, int jour)
 {
   return new DecimalFormat(\"00\").format(jour) + \"/\" +
     new DecimalFormat(\"00\").format(mois) + \"/\" +
     new DecimalFormat(\"0000\").format(annee);
 }

 static String formatDateHeure (int annee, int mois, int jour, int heure, int min)
 {
   return new DecimalFormat(\"00\").format(jour) + \"/\" +
     new DecimalFormat(\"00\").format(mois) + \"/\" +
     new DecimalFormat(\"0000\").format(annee) + \" \" +
     new DecimalFormat(\"00\").format(heure) + \":\" +
     new DecimalFormat(\"00\").format(min);
 }

 static String formatDateRef ()
 {
   return new DecimalFormat(\"00\").format(jourRef) + \"/\" +
     new DecimalFormat(\"00\").format(moisRef) + \"/\" +
     new DecimalFormat(\"0000\").format(anneeRef) + \" \" +
     new DecimalFormat(\"00\").format(heureRef) + \":\" +
     new DecimalFormat(\"00\").format(minRef);
 }
 
 static void afficherCoursActions ()
 {
  int i;
  Cours cours;
  for (i=0; i<nCours; i++)
  {
   cours = coursActions[i];
   System.out.println (
    new DecimalFormat(\"00\").format(cours.jour) + \"/\" +
    new DecimalFormat(\"00\").format(cours.mois) + \"/\" +
    new DecimalFormat(\"0000\").format(cours.annee) + \" \" +
    new DecimalFormat(\"00\").format(cours.heure) + \":\" +
    new DecimalFormat(\"00\").format(cours.min) + \" \" + 
    myFormat (\"000000.000\",cours.cours) + \" \" + cours.nomAction);
   /* System.out.format (null, \"%02d/%02d/%04d\\t%8.2d\\t%s\\n\", 
    cours.jour, cours.mois, cours.annee, cours.cours, cours.nomAction); */
  }
 }

 static void lireDateRef ()
 {
  try 
  {
   System.out.println (\"Tapez Entree pour abandonner et revenir au menu.\");
   BufferedReader in = new BufferedReader( new InputStreamReader( System.in ));
   String reponse;
   System.out.println (\"Date de reference\");
   for (;;)
   {
    System.out.print (\"Annee ? \");
    reponse = in.readLine();
    anneeRef = Integer.parseInt(reponse);
    System.out.print (\"Mois  ? \");
    reponse = in.readLine();
    moisRef = Integer.parseInt(reponse);
    System.out.print (\"Jour  ? \");
    reponse = in.readLine();
    jourRef = Integer.parseInt(reponse);
    System.out.print (\"Heure ? \");
    reponse = in.readLine();
    heureRef = Integer.parseInt(reponse);
    System.out.print (\"Minutes ? \");
    reponse = in.readLine();
    minRef = Integer.parseInt(reponse);
    boolean verif = verifierDateHeure (anneeRef, moisRef, jourRef, heureRef, minRef);
    if (verif)
     break;
    else
     System.out.println (\"Date incorrecte, retapez la date.\");
   }
  }
  catch (Exception e)
  {
   // System.out.println (\"Erreur dans LireDateRef\");
   System.out.println (\"Operation abandonnee, retour au menu\"); 
  }
 }

 static int compareDateHeure (DateHeure dh1, DateHeure dh2)
 {
  if (dh1 == null)
   return -1;
  if (dh2 == null)
   return 1;
  if (dh1.annee < dh2.annee)
   return -1;
  if (dh1.annee > dh2.annee)
   return 1;
  if (dh1.mois < dh2.mois)
   return -1;
  if (dh1.mois > dh2.mois)
   return 1;
  if (dh1.jour < dh2.jour)
   return -1;
  if (dh1.jour > dh2.jour)
   return 1;
  if (dh1.heure < dh2.heure)
   return -1;
  if (dh1.heure > dh2.heure)
   return 1;
  if (dh1.min < dh2.min)
   return -1;
  if (dh1.min > dh2.min)
   return 1;
  return 0;
 }

 static void choisirDateRef ()
 {
  try
  {
   System.out.println (\"Choix de la date de reference\");
   int jourDem = 32;
   int moisDem = 13;
   int anneeDem = 30000;
   int n = 10;
   DateHeure[] dha = new DateHeure[n];
   DateHeure dh, dh1, dhd;
   dhd = new DateHeure();
   
   while (true)
   {
    dhd.annee = anneeDem;
    dhd.mois = moisDem;
    dhd.jour = jourDem;
    dhd.heure = 23;
    dhd.min = 59;
    // System.out.println (\"jourDem=\"+Integer.toString(jourDem)+\" moisDem=\"+Integer.toString(moisDem)+\" anneeDem=\"+Integer.toString(anneeDem));
    int ndh = 0;
    int i, j, k, c, c1;
    for (i=0; i<n; i++)
     dha[i] = null;
    for (i=0; i<nCours; i++)
    {
     // System.out.println (\"jourDem=\"+Integer.toString(jourDem)+\" moisDem=\"+Integer.toString(moisDem)+\" anneeDem=\"+Integer.toString(anneeDem));
     // System.out.println (\"i=\"+Integer.toString(i)+\" ndh=\"+Integer.toString(ndh));
     dh = new DateHeure();
     dh.annee = coursActions[i].annee;
     dh.mois = coursActions[i].mois;
     dh.jour = coursActions[i].jour;
     dh.heure = coursActions[i].heure;
     dh.min = coursActions[i].min;
     c1 = compareDateHeure (dh, dhd);  
     // System.out.println(\"jour=\"+Integer.toString(dh.jour)+\" mois=\"+Integer.toString(dh.mois)+\" annee=\"+Integer.toString(dh.annee)+\" c1=\"+Integer.toString(c1));
     if (c1 <= 0)
     {
      for (j=0; j<=ndh && j<n; j++)
      {
       // System.out.println (\"j=\"+Integer.toString(j));
       c = compareDateHeure(dh,dha[j]);
       if (c == 0)
        break;
       if (c == 1)
       {
        for (k=n-2; k>=j; k--)
         dha[k+1] = dha[k];
        dha[j] = dh;
        if (ndh < n)
         ndh++;
        break;
       }
      }
     }
    }
    // for (i=0; i<n && i<ndh; i++)
    // {
    //  System.out.println (String.valueOf(i+1)+\": \"+formatDateHeure(dha[i].annee,dha[i].mois,dha[i].jour,dha[i].heure,dha[i].min));
    // }
    int n1;
    if (n<ndh)
     n1 = n;
    else
     n1 = ndh;
    for (i=n1-1; i>=0; i--)
     System.out.println (String.valueOf(i+1)+\": \"+formatDateHeure(dha[i].annee,dha[i].mois,dha[i].jour,dha[i].heure,dha[i].min)); 
    System.out.print (\"Votre choix ou autre date ? \");
    BufferedReader in = new BufferedReader( new InputStreamReader( System.in ));
    String reponse = in.readLine();
    System.out.println (\"\");
    if (reponse.equals(\"\"))
    {
     System.out.println (\"Operation abandonnee\");
     break;
    }
    if (reponse.indexOf('/') != -1)
    {
     String[] a = reponse.split(\"/\");
     // System.out.println (\"jour=\"+a[0].toString()+\" mois=\"+a[1]+\" annee=\"+a[2]);
     jourDem = Integer.parseInt(a[0]);
     moisDem = Integer.parseInt(a[1]);
     anneeDem = Integer.parseInt(a[2]);
     continue;
    }
    try
    {
     i = Integer.parseInt(reponse) - 1;
    }
    catch (Exception e)
    {
     i = -1;
    }
    if (i>=0 && i<n && i<ndh)
    {
     anneeRef = dha[i].annee;
     moisRef = dha[i].mois;
     jourRef = dha[i].jour;
     heureRef = dha[i].heure;
     minRef = dha[i].min;
     System.out.println (\"Date de reference modifiee : \"+formatDateHeure(dha[i].annee,dha[i].mois,dha[i].jour,dha[i].heure,dha[i].min));
     break;
    }
    else
    {
     System.out.println (\"Choix incorrect, date de reference non modifiee\");
    }
   }
  }
  catch (Exception e)
  {
   System.out.println (\"Erreur dans choisirDateRef\");
  }
 }

 static void enregistrerFichier (String param)
 {
  try
  {
   String nomFichier;
   BufferedReader in = new BufferedReader( new InputStreamReader( System.in ));
   int i;
   if (param.equals(\"\"))
   {
    System.out.print (\"Nom du fichier ? \");
    nomFichier = in.readLine();
   }
   else
    nomFichier = param;
   nomFichier = dossier + nomFichier + \"-cours.txt\";
   // System.out.println (\"Enregistrement des donnees dans le fichier \" + nomFichier);
   PrintWriter fichier = new PrintWriter(new FileOutputStream(nomFichier));
   fichier.println (\"nDatesChoix=\"+Integer.toString(params.nDatesChoix)+\";seuil=\"+Double.toString(params.seuil)+\";ordre=\"+params.ordre+\";ordreActions=\"+params.ordreActions);
   if (nActions > 0)
   {
    fichier.print (nomsActions[0]);
    for (i=1; i<nActions; i++)
     fichier.print(\";\"+nomsActions[i]);
   }
   fichier.println(\"\");
   fichier.println (new DecimalFormat(\"00\").format(jourRef) + \"/\" +
     new DecimalFormat(\"00\").format(moisRef) + \"/\" +
     new DecimalFormat(\"0000\").format(anneeRef) + \";\" +
     new DecimalFormat(\"00\").format(heureRef) + \":\" +
     new DecimalFormat(\"00\").format(minRef));
   for (i=0; i<nCours; i++)
   {
    Cours cours = coursActions[i];
    fichier.println (cours.nomAction + \";\" +
     new DecimalFormat(\"00\").format(cours.jour) + \"/\" +
     new DecimalFormat(\"00\").format(cours.mois) + \"/\" +
     new DecimalFormat(\"0000\").format(cours.annee) + \";\" +
     new DecimalFormat(\"00\").format(cours.heure) + \":\" + 
     new DecimalFormat(\"00\").format(cours.min) + \";\" +
     new DecimalFormat(\"#0.000\").format(cours.cours));
   }
   fichier.close();
  }
  catch (Exception e)
  {
   System.out.println (\"Erreur dans enregistrerFichier : \" + e.getMessage());
  }
 }

 static void ajouterActions (String[] nomsActions1)
 {
  int i, j;
  boolean trouve;
  for (i=0; i<nomsActions1.length; i++)
  {
   trouve = false;
   for (j=0; j<nActions; j++)
   {
    if (nomsActions[j].equals(nomsActions1[i]))
    {
     trouve = true;
     break;
    }
   }
   if (!trouve)
   {
     nomsActions[nActions++] = nomsActions1[i];
   }
  }
 }

 public static void init ()
 {
  modifierParam(\"nDatesChoix\",\"10\",false);
  modifierParam(\"seuil\",\"2.0\",false);
  modifierParam(\"ordre\",\"progression\",false);
  modifierParam(\"ordreActions\",\"alphabetique\",false);
  modifierParam(\"nEnregs\",\"1\",false);
  nActions = 0;
  nActionsPrerenseignees = 0;
  nCours = 0;
 }

 public static void lireFichier (String param)
 {
  try 
  {
   String nomFichier;
   String reponse;
   BufferedReader in = new BufferedReader( new InputStreamReader( System.in ));
   int i, j;

   if (param.equals(\"\"))
   {

    File dir = new File(dossier);
    String[] files = dir.list(filter);
    for (i=0; i<files.length; i++)
    {
     System.out.println(new Integer(i+1).toString() + \": \" + files[i].substring(0,files[i].length()-10));
    }
   
    System.out.print (\"Fichier a lire ? \");
    reponse = in.readLine();
    try
    {
     i = Integer.parseInt(reponse);
     nomFichier = dossier + files[i-1].substring(0,files[i-1].length()-10);
    }
    catch (Exception e)
    {
     nomFichier = dossier + reponse;
    }
   }
   else
    nomFichier = dossier + param;
   nomFichier = nomFichier + \"-cours.txt\";
   System.out.println (\"Lecture du fichier \" + nomFichier);
   BufferedReader fichier = new BufferedReader (new InputStreamReader (new FileInputStream (nomFichier)));
   String ligne = fichier.readLine();
   // System.out.println (\"Ligne parametres : \" + ligne);
   String[] paramsLus = ligne.split(\";\");
   // System.out.println (\"apres split\");
   for (j=0; j<paramsLus.length; j++)
   {
    // System.out.println (\"j=\" + Integer.toString(j));
    String[] paramLu = paramsLus[j].split(\"=\");
    // System.out.println (\"avant modifier\");
    modifierParam (paramLu[0], paramLu[1], false);
    // System.out.println (\"dans modifier\");
   }
   // System.out.println (\"ligne suivante\");
   ligne = fichier.readLine();
   // System.out.println(\"Ligne noms actions : \" + ligne + \".\");
   if (ligne.length() > 0)
   {
    // System.out.println (\"Longueur \" + ligne.length());
    // for (int k=0; k<ligne.length(); k++)
    //  System.out.println (\"Caractere \" + k + \" : code \" + (int)(ligne.charAt(k)));
    String[] nomsActions1 = ligne.split(\";\");
    ajouterActions (nomsActions1);
   }
   else
   {
    // System.out.println (\"vide nActions = \" + nActions);
   }
   ligne = fichier.readLine();
   String[] champs = ligne.split(\";\");
   String[] date = champs[0].split(\"/\");
   anneeRef = Integer.parseInt(date[2]);
   moisRef = Integer.parseInt(date[1]);
   jourRef = Integer.parseInt(date[0]);
   String[] hm = champs[1].split(\":\");
   heureRef = Integer.parseInt(hm[0]);
   minRef = Integer.parseInt(hm[1]);

   System.out.println (\"Date de reference : \" + champs[0] + \" \" + champs[1]);

   for (;;)
   {
    try
    {
     ligne = fichier.readLine();
     champs = ligne.split(\";\");
     Cours cours = new Cours();
     cours.nomAction = champs[0];
     date = champs[1].split(\"/\");
     cours.annee = Integer.parseInt(date[2]);
     cours.mois = Integer.parseInt(date[1]);
     cours.jour = Integer.parseInt(date[0]);
     hm = champs[2].split(\":\"); 
     cours.heure = Integer.parseInt(hm[0]);
     cours.min = Integer.parseInt(hm[1]);
     cours.cours = Float.valueOf(champs[3].replace(',','.')).floatValue();
     coursActions[nCours++] = cours;    
    }
    catch (Exception e)
    {
     break;
    }
   }
   fichier.close();
  }
  catch (Exception e)
  {
   System.out.println (\"Erreur dans lireFichier : \" + e.getMessage()); 
   e.printStackTrace();
  }
 }

 public static double coursDate (String nomAction, int annee, int mois, int jour)
 {
  int i;
  for (i=0; i<nCours; i++)
  {
   if (coursActions[i].nomAction.equals(nomAction) &&
       (coursActions[i].annee == annee) &&
       (coursActions[i].mois == mois) &&
       (coursActions[i].jour == jour))
    return coursActions[i].cours;
  }
  return 0;
 }

 public static Cours coursDateComplet (String nomAction, int annee, int mois, int jour)
 {
  int i;
  for (i=0; i<nCours; i++)
  {
   if (coursActions[i].nomAction.equals(nomAction) &&
       (coursActions[i].annee == annee) &&
       (coursActions[i].mois == mois) &&
       (coursActions[i].jour == jour))
    return coursActions[i];
  }
  return null;
 }

 public static Cours existeCoursDate (int annee, int mois, int jour)
 {
  int i;
  for (i=0; i<nCours; i++)
  {
   if ((coursActions[i].annee == annee) &&
       (coursActions[i].mois == mois) &&
       (coursActions[i].jour == jour))
    return coursActions[i];
  }
  return null;
 }

 public static void progression ()
 {
  try
  {
   BufferedReader in = new BufferedReader( new InputStreamReader( System.in ));
   String reponse;
   int annee1, mois1, jour1, annee2, mois2, jour2;
/*
   System.out.print (\"Premiere date - annee ? \");
   reponse = in.readLine();
   annee1 = Integer.parseInt(reponse);
   System.out.print (\"Premiere date - mois  ? \");
   reponse = in.readLine();
   mois1 = Integer.parseInt(reponse);
   System.out.print (\"Premiere date - jour  ? \");
   reponse = in.readLine();
   jour1 = Integer.parseInt(reponse);
*/
   System.out.println (\"Tapez Entree pour abandonner et revenir au menu.\");
   System.out.println (\"Progression des cours entre la date de reference \" + formatDateRef() + \" et la date suivante :\");
   for (;;)
   {
    System.out.print (\"annee ? \");
    reponse = in.readLine();
    annee2 = Integer.parseInt(reponse);
    System.out.print (\"mois  ? \");
    reponse = in.readLine();
    mois2 = Integer.parseInt(reponse);
    System.out.print (\"jour  ? \");
    reponse = in.readLine();
    jour2 = Integer.parseInt(reponse);
    boolean verif = verifierDateHeure (annee2, mois2, jour2, 12, 0);
    if (verif)
     break;
    else
     System.out.println (\"Date incorrecte, retapez la date.\");
   }
   afficherProgression (annee2, mois2, jour2);
  }
  catch (Exception e)
  {
   // System.out.println (\"Erreur dans progression : \" + e.getMessage());
   System.out.println (\"Operation abandonnee, retour au menu.\");
  }
 }

 public static void afficherProgression (int annee2, int mois2, int jour2)
 {
  try 
  {
   int i;
   int annee1, mois1, jour1;
   annee1 = anneeRef;
   mois1 = moisRef;
   jour1 = jourRef;
   System.out.println (\"\");
   if (choixMenu.equals(\"5\"))
    System.out.println (\"Cours de reference :\");
   else
    System.out.println (\"Progression des cours entre la date de reference \" + formatDateRef() + 
    \" et le \" + formatDate(annee2,mois2,jour2) + \" :\");
   System.out.println (\"\");
   System.out.println (\"Detenue Nom action            Reference      Cours Progression Difference\");
   System.out.println (\"------- ----------            ---------      ----- ----------- ----------\");

   // tri par ordre de taux de progression decroissant

   int nTri = 0;
   int j, k;
   LigneProgression tri[] = new LigneProgression[nActions];
   LigneProgression ligne;

   double progressionActionDetenue = 0.0;

   if (!(actionDetenue.equals(\"\")))
   {
    double cours1 = coursDate (actionDetenue, annee1, mois1, jour1);
    double cours2 = coursDate (actionDetenue, annee2, mois2, jour2);
    double p = ((cours2 / cours1) - 1) * 100;
    progressionActionDetenue = p;
    String afProgr;
    if (p >= 0)
     afProgr = new DecimalFormat(\"+0.00\").format(p) + \"%\";
    else
     afProgr = new DecimalFormat(\"-0.00\").format(-p) + \"%\";
    System.out.println ( \"******* \" +
     formatAction(actionDetenue) + \" \" + 
     myFormat(\"000000.000\",cours1) + \" \" +
     myFormat(\"000000.000\",cours2) + \" \" +
     afProgr);     
    System.out.println(\"\");
   }

   for (i=0; i<nActions; i++)
   {
    double cours1 = coursDate (nomsActions[i], annee1, mois1, jour1);
    double cours2 = coursDate (nomsActions[i], annee2, mois2, jour2);
    if ((cours1 != 0) && (cours2 != 0))
    {
     double p = ((cours2 / cours1) - 1) * 100;
     for (j=0; j<nTri; j++)
     {
      // if (p < tri[j].progression)
      boolean apres;
      if (params.ordre.equals(\"progression\"))
       apres = (p > tri[j].progression);
      else if (params.ordre.equals(\"alphabetique\"))
       apres = (nomsActions[i].compareTo(tri[j].nomAction) > 0);
      else
       apres = true;

      if (apres)
       continue;
      else
       break;
     }
     for (k=nTri-1; k>=j; k--)
      tri[k+1] = tri[k];
     ligne = new LigneProgression();
     ligne.nomAction = nomsActions[i];
     ligne.reference = cours1;
     ligne.cours = cours2;
     ligne.progression = p;
     tri[j] = ligne;
     nTri++;
    }
   }
   
   boolean seuilAffiche = false;
   for (i=0; i<nTri; i++)
   {
     double p = tri[i].progression;
     String nom = tri[i].nomAction;
     double cours1 = tri[i].reference;
     double cours2 = tri[i].cours;
     String afProgr;
     double dif = 0.0;
     if (p >= 0)
      afProgr = new DecimalFormat(\"+0.00\").format(p) + \"%\";
     else
      afProgr = new DecimalFormat(\"-0.00\").format(-p) + \"%\";
     String detenue;
     String difference;
     if (nom.equals(actionDetenue))
     {
      detenue = \"******* \";
     }
     else
     {
      detenue = \"        \";
     }
     if (actionDetenue.equals(\"\"))
      difference = \"\";
     else
     {
      dif = progressionActionDetenue - p;
      if (dif >= 0)
       difference = new DecimalFormat(\"+0.00\").format(dif) + \"%\";
      else
       difference = new DecimalFormat(\"-0.00\").format(-dif) + \"%\";
     }
     if (params.ordre.equals(\"progression\") && 
         !actionDetenue.equals(\"\") && 
         dif < params.seuil &&
         !seuilAffiche)
     {
      System.out.println (\"------------------- SEUIL \" + 
       new DecimalFormat(\"0.00\").format(params.seuil) + \"% ------------------------------------- \");
      seuilAffiche = true;
     }
     System.out.println ( detenue +
      formatAction(nom) + \" \" + 
      myFormat(\"000000.000\",cours1) + \" \" +
      myFormat(\"000000.000\",cours2) + \" \" +
      afProgr + \"      \" + difference);     

   }
/*
   for (i=0; i<nActions; i++)
   {
    double cours1 = coursDate (nomsActions[i], annee1, mois1, jour1);
    double cours2 = coursDate (nomsActions[i], annee2, mois2, jour2);
    if ((cours1 != 0) && (cours2 != 0))
    {
     double p = ((cours2 / cours1) - 1) * 100;
     String afProgr;
     if (p >= 0)
      afProgr = new DecimalFormat(\"+0.00\").format(p) + \"%\";
     else
      afProgr = new DecimalFormat(\"-0.00\").format(-p) + \"%\";
     System.out.println (
      formatAction(nomsActions[i]) + \" \" + 
      myFormat(\"000000.000\",cours1) + \" \" +
      myFormat(\"000000.000\",cours2) + \" \" +
      afProgr);     
    }
   }
*/
  }

  catch (Exception e)
  {
   System.out.println (\"Erreur dans afficherProgression : \" + e.getMessage());
  }
 }

 public static void indiquerActionDetenue ()
 {
  try
  {
   BufferedReader in = new BufferedReader( new InputStreamReader( System.in ));
   String reponse;
   if (actionDetenue.equals(\"\"))
    System.out.println(\"Actuellement aucune action n'est detenue\");
   else
    System.out.println(\"Actuellement l'action detenue est : \"+actionDetenue);
   System.out.print(\"Nouvelle action detenue ? \");
   reponse = in.readLine();
   int i=0;
   boolean trouve = false;
   if (reponse.equals(\"\"))
    actionDetenue = \"\";
   else
   {
    while (i<nActions && !trouve)
    {
     if (nomsActions[i].toLowerCase().startsWith(reponse.toLowerCase()))
     {
      actionDetenue = nomsActions[i];
      trouve = true;
     }
     i++;
    }
    if (!trouve)
     actionDetenue = reponse;
   }
   if (actionDetenue.equals(\"\"))
    System.out.println(\"Actuellement aucune action n'est detenue\");
   else
    System.out.println(\"Actuellement l'action detenue est : \"+actionDetenue);
  }
  catch (Exception e)
  {
   System.out.println(\"Erreur dans actionDetenue\");
  }
 }

 public static void lireTousCoursWeb ()
 {
  Calendar cal = Calendar.getInstance();
  cal.setLenient(false);
  Date now = new Date();
  // System.out.println (\"now=\"+now.toString());
  cal.setTime(now);
  // System.out.println(\"cal=\"+cal.toString());
  // System.out.println(\"YEAR=\"+String.valueOf(cal.get(Calendar.YEAR)));
  int i;
  for (i=0; i<nActions; i++)
  {
   String action = nomsActions[i];
   System.out.println (\"Telechargement du cours de \" + action + \" ... \");
   String[] s = action.split(\" \");
   double c = lireCoursWeb(s[0]);
   if (c != 0.0)
   {
    Cours cours = new Cours();
    cours.nomAction = action;
    cours.annee = cal.get(Calendar.YEAR);
    cours.mois = cal.get(Calendar.MONTH)+1;
    cours.jour = cal.get(Calendar.DAY_OF_MONTH);
    cours.heure = cal.get(Calendar.HOUR_OF_DAY);
    cours.min = cal.get(Calendar.MINUTE);
    cours.cours = c; 
    System.out.println (
     new DecimalFormat(\"00\").format(cours.jour) + \"/\" +
     new DecimalFormat(\"00\").format(cours.mois) + \"/\" +
     new DecimalFormat(\"0000\").format(cours.annee) + \" \" +
     new DecimalFormat(\"00\").format(cours.heure) + \":\" +
     new DecimalFormat(\"00\").format(cours.min) + \" \" + 
     myFormat (\"000000.000\",cours.cours) + \" \" + cours.nomAction);
    coursActions[nCours++] = cours;
   }
  }
 } 

 public static void creerTableauEnregs ()
 {
  enregs = new DateHeure[maxCours];
  nEnregs = 0;
  int i, j, k;
  DateHeure dh;
  for (i=0; i<nCours; i++)
  {
   dh = new DateHeure();
   dh.annee = coursActions[i].annee;
   dh.mois = coursActions[i].mois;
   dh.jour = coursActions[i].jour;
   dh.heure = coursActions[i].heure;
   dh.min = coursActions[i].min;
   // System.out.println (\"Cours numero \" + i + \" : \" + dh.jour + \"/\" + dh.mois + \"/\" + dh.jour + \" \" + dh.heure + \":\" + dh.min);
   boolean traite = false;
   for (j=0; j<nEnregs; j++)
   {
    // System.out.println (\"j=\"+j);
    int c;
    c = compareDateHeure(dh,enregs[j]);
    if (c == -1)
    {
     for (k=nEnregs; k>j; k--)
      enregs[k] = enregs[k-1];
     nEnregs++;
     enregs[j] = dh;
     traite = true;
     break;
    }
    else if (c == 0)
    {
     traite = true;
     break;
    }
   }  
   if (!traite)
   {
    enregs[nEnregs++] = dh;
   }
  }
  // System.out.println (\"Tableau des enregistrements :\");
  // for (j=0; j<nEnregs; j++)
  //  System.out.println (j + \" : \" + enregs[j].jour + \"/\" + enregs[j].mois + \"/\" + enregs[j].annee + \" \" + enregs[j].heure + \":\" + enregs[j].min);
  // System.out.println (\"Fin du tableau.\");
 }

 public static void afficherDerniersEnregistrements ()
 {
  creerTableauEnregs ();
  int i, j, k;
  k = nEnregs - params.nEnregs;
  if (k < 0)
   k = 0;
  for (j=k; j<nEnregs; j++)
  {
   for (i=0; i<nCours; i++)
   {
    DateHeure dh = new DateHeure();
    dh = new DateHeure();
    dh.annee = coursActions[i].annee;
    dh.mois = coursActions[i].mois;
    dh.jour = coursActions[i].jour;
    dh.heure = coursActions[i].heure;
    dh.min = coursActions[i].min;
    int c;
    c = compareDateHeure (dh, enregs[j]);
    if (c == 0)
    {
     Cours cours;
     cours = coursActions[i];
     System.out.println (
      new DecimalFormat(\"00\").format(cours.jour) + \"/\" +
      new DecimalFormat(\"00\").format(cours.mois) + \"/\" +
      new DecimalFormat(\"0000\").format(cours.annee) + \" \" +
      new DecimalFormat(\"00\").format(cours.heure) + \":\" +
      new DecimalFormat(\"00\").format(cours.min) + \" \" + 
      myFormat (\"000000.000\",cours.cours) + \" \" + cours.nomAction);
    }
   }
  }
 }

";
if ($ARGV[0] eq "new") { print "
 public static void importer ()
 {
  try
  {
   BufferedReader in = new BufferedReader( new InputStreamReader( System.in ));
   String nomFichier;
   String ohbc;
   String reponse;
   int i;
   boolean trouve;

   System.out.print (\"Fichier a importer ? \");
   nomFichier = in.readLine();
   System.out.print (\"Cours a importer (ouverture/haut/bas/cloture) ? \");
   ohbc = in.readLine();

   System.out.print (\"Heure   ? \");
   reponse = in.readLine();
   int heure = Integer.parseInt(reponse);
   System.out.print (\"Minutes ? \");
   reponse = in.readLine();
   int min = Integer.parseInt(reponse);

   BufferedReader fichier = new BufferedReader (new InputStreamReader (new FileInputStream (nomFichier)));
   String ligne;
   ligne = fichier.readLine();

   for (;;)
   {
    try
    {
     ligne = fichier.readLine();
     String[] champs = ligne.split(\"\\t\");
     trouve = false;
     for (i=0; i<nActions; i++)
     {
      if (nomsActions[i].equals(champs[1]))
      {
       trouve = true;
       break;
      }
     } 
     if (!trouve)
      nomsActions[nActions++] = champs[1];
     String[] date = champs[2].split(\"/\");
     int annee = Integer.parseInt(date[2]);
     if (annee < 100)
      annee = 2000 + annee;
     int mois = Integer.parseInt(date[1]);
     int jour = Integer.parseInt(date[0]);
     String sCours;
     sCours = \"0\";
     if (ohbc.charAt(0) == 'o')
      sCours = champs[3];
     else if (ohbc.charAt(0) == 'h')
      sCours = champs[4];
     else if (ohbc.charAt(0) == 'b')
      sCours = champs[5];
     else if (ohbc.charAt(0) == 'c')
      sCours = champs[6];
     double fCours = Float.valueOf(sCours.replace(',','.')).floatValue();
     Cours cours = new Cours();
     cours.annee = annee;
     cours.mois = mois;
     cours.jour = jour;
     cours.heure = heure;
     cours.min = min;
     cours.nomAction = champs[1];
     cours.cours = fCours;
     coursActions[nCours++] = cours;
    }
    catch (Exception e)
    {
     break;
    }
   }
   fichier.close();
  }
  catch (Exception e)
  {
   System.out.println (\"Erreur dans importer : \" + e.getMessage());
  }
 }

 public static void simulation ()
 {
  try
  {
   BufferedReader in = new BufferedReader( new InputStreamReader( System.in ));
   System.out.print (\"Seuil (%) ? \");
   String reponse = in.readLine();
   double seuilP100 = Float.valueOf(reponse.replace(',','.')).floatValue();
   double seuil = seuilP100 / 100.0;
   System.out.print (\"Frais de courtage ? \");
   reponse = in.readLine();
   double frais = Float.valueOf(reponse.replace(',','.')).floatValue();
   System.out.print (\"Capital ? \");
   reponse = in.readLine();
   double capitalDepart = Float.valueOf(reponse.replace(',','.')).floatValue();
   int anneeDebut, moisDebut, jourDebut, anneeFin, moisFin, jourFin;
   System.out.print (\"A partir du : \\nannee ? \");
   reponse = in.readLine();
   anneeDebut = Integer.parseInt(reponse);
   System.out.print (\"Mois  ? \");
   reponse = in.readLine();
   moisDebut = Integer.parseInt(reponse);
   System.out.print (\"Jour  ? \");
   reponse = in.readLine();
   jourDebut = Integer.parseInt(reponse);
   System.out.print (\"Jusqu'au : \\nannee ? \");
   reponse = in.readLine();
   anneeFin = Integer.parseInt(reponse);
   System.out.print (\"Mois  ? \");
   reponse = in.readLine();
   moisFin = Integer.parseInt(reponse);
   System.out.print (\"Jour  ? \");
   reponse = in.readLine();
   jourFin = Integer.parseInt(reponse);
   System.out.print (\"Action detenue initialement ? \");
   String actDetenue = in.readLine();
   System.out.print (\"Afficher les progressions (oui/non) ? \");
   reponse = in.readLine();
   boolean afProgr;
   if (reponse.charAt(0) == 'o' || reponse.charAt(0) == 'O')
    afProgr = true;
   else
    afProgr = false;

   anneeRef = anneeDebut;
   moisRef = moisDebut;
   jourRef = jourDebut;
   double liq = capitalDepart;
   int annee = anneeDebut;
   int mois = moisDebut;
   int jour = jourDebut;
   int qteDetenue = 0;
   double progrActDetenue;
   double cours1, cours2, cours3, cours4;
   double plusForteBaisse;
   String actPlusForteBaisse;
   double coursPlusForteBaisse;
   int i;
   double progrAct;
   double capital;
   double valActions;

   if (!actDetenue.equals(\"\"))
   {
    cours1 = coursDate (actDetenue, anneeRef, moisRef, jourRef);
    if (cours1 == 0.0)
     actDetenue = \"\";
    else
    {
      // liq = liq - frais;
      qteDetenue = (int) (liq / cours1);
      valActions = qteDetenue * cours1;
      liq = liq - valActions;
    }

    capital = liq + qteDetenue * cours1;
    valActions = qteDetenue * cours1; 
    System.out.println (
      new DecimalFormat(\"00\").format(jour) + \"/\" +
      new DecimalFormat(\"00\").format(mois) + \"/\" +
      new DecimalFormat(\"0000\").format(annee) + 
      \": capital \" + myFormat(\"0000.00\", capital) + 
      \" ; liquidites \" + myFormat(\"0000.00\", liq) +
      \" ; \" + myFormat(\"0000\",(double)qteDetenue) + \" actions \" + actDetenue + \" valant \" + 
      myFormat(\"000.00\",cours1) +  \" = \" + myFormat(\"0000.00\",valActions));
   }

   for (;;)
   {
    jour = jour + 1;
    if ((annee >= anneeFin) && (mois >= moisFin) && (jour >= jourFin))
     break;
    if (jour > 31)
    {
     jour = 1;
     mois = mois + 1;
     if ((annee >= anneeFin) && (mois > moisFin))
      break;
     if (mois > 12)
     {
      mois = 1;
      annee = annee + 1;
      if (annee > anneeFin)
       break;
     }
    }

    // si date ref plus ancienne que nombre de jours defini en parametre
    // date ref = date courante - nombre de jour 
    // ou dernier jour pour lequel il existe un cours avant cette date
    // determiner la date du cours le plus ancien
    int ecart = ecartDates (anneeRef, moisRef, jourRef, annee, mois, jour);
    if (ecart > params.maxDateRef)
    {
     Calendar cal = Calendar.getInstance();

     cal.set (anneeRef, moisRef-1, jourRef, heureRef, minRef, 0);
     for (;;)
     {
      cal.add (Calendar.DATE, -1);
      Cours c = existeCoursDate (cal.YEAR, cal.MONTH+1, cal.DAY_OF_MONTH);
      if (c != null)
      {
       anneeRef = c.annee;
       moisRef = c.mois;
       jourRef = c.jour;
       heureRef = c.heure;
       minRef = c.min;
       break;
      }
     }
    }

    /*
    System.out.println (
      new DecimalFormat(\"00\").format(jour) + \"/\" +
      new DecimalFormat(\"00\").format(mois) + \"/\" +
      new DecimalFormat(\"0000\").format(annee));
    */
    if (actDetenue.equals(\"\"))
    {
     progrActDetenue = 0.0;
     cours2 = 0.0;
    }
    else
    {
     cours1 = coursDate (actDetenue, anneeRef, moisRef, jourRef);
     cours2 = coursDate (actDetenue, annee, mois, jour);
     if ((cours1 == 0.0) || (cours2 == 0.0)) 
      progrActDetenue = 0.0;
     else
      progrActDetenue = (cours2 / cours1) - 1;
    }

    if (cours2 != 0.0)
    {
     capital = liq + qteDetenue * cours2;
     valActions = qteDetenue * cours2; 
     System.out.println (
      new DecimalFormat(\"00\").format(jour) + \"/\" +
      new DecimalFormat(\"00\").format(mois) + \"/\" +
      new DecimalFormat(\"0000\").format(annee) + 
      \": capital \" + myFormat(\"0000.00\", capital) + 
      \" ; liquidites \" + myFormat(\"0000.00\", liq) +
      \" ; \" + myFormat(\"0000\",(double)qteDetenue) + \" actions \" + actDetenue + \" valant \" + 
      myFormat(\"000.00\",cours2) +  \" = \" + myFormat(\"0000.00\",valActions));
    }

    plusForteBaisse = 10000.0;
    actPlusForteBaisse = \"\";
    coursPlusForteBaisse = 0.0;
    for (i=0; i<nActions; i++)
    {
     cours3 = coursDate (nomsActions[i], anneeRef, moisRef, jourRef);
     cours4 = coursDate (nomsActions[i], annee, mois, jour);
     if ((cours3 == 00) || (cours4 == 0.0))
      progrAct = 0.0;
     else
     {
      progrAct = (cours4 / cours3) - 1;
      if (afProgr)
      {
       String signe;
       if (progrAct >= 0)
        signe = \"+\";
       else
        signe = \"\";
       System.out.println (
        myFormat(\"0000.000\",cours3) + \" -> \" +
        myFormat(\"0000.000\",cours4) + \" : \" + signe +
        new DecimalFormat(\"0.00\").format(100.0*progrAct) + \"% \" +
        nomsActions[i]);
      } 
     }
     if (progrAct < plusForteBaisse)
     {
      plusForteBaisse = progrAct;
      actPlusForteBaisse = nomsActions[i];
      coursPlusForteBaisse = cours4;
     }
    }

    if ((progrActDetenue - plusForteBaisse >= seuil) 
      && ((cours2 != 0) || (actDetenue.equals(\"\"))) 
      && (coursPlusForteBaisse != 0.0))
    {
      System.out.print (\"On vend \" + actDetenue);
      capital = liq + qteDetenue * cours2;
      valActions = qteDetenue * cours2;
      if (actDetenue.equals(\"\"))
       liq = liq - frais;
      else
       liq = liq + valActions - 2 * frais;
      actDetenue = actPlusForteBaisse;
      qteDetenue = (int) (liq / coursPlusForteBaisse);
      valActions = qteDetenue * coursPlusForteBaisse;
      liq = liq - valActions;
      capital = liq + valActions;
      anneeRef = annee;
      moisRef = mois;
      jourRef = jour;
      System.out.println (\" et on achete \" + actDetenue);
      System.out.println (
       new DecimalFormat(\"00\").format(jour) + \"/\" +
       new DecimalFormat(\"00\").format(mois) + \"/\" +
       new DecimalFormat(\"0000\").format(annee) + 
       \": capital \" + myFormat(\"0000.00\", capital) + 
       \" ; liquidites \" + myFormat(\"0000.00\", liq) +
       \" ; \" + myFormat(\"0000\",(double)qteDetenue) + \" actions \" + actDetenue + \" valant \" + 
       myFormat(\"000.00\",coursPlusForteBaisse) +  \" = \" + myFormat(\"0000.00\",valActions));
     
    }
   }
   /*
   cours2 = coursDate (actDetenue, annee, mois, jour);
   capital = liq + qteDetenue * cours2;
   valActions = qteDetenue * cours2; 
   System.out.println (
      new DecimalFormat(\"00\").format(jour) + \"/\" +
      new DecimalFormat(\"00\").format(mois) + \"/\" +
      new DecimalFormat(\"0000\").format(annee) + 
      \": capital \" + myFormat(\"0000.00\", capital) + 
      \" ; liquidites \" + myFormat(\"0000.00\", liq) +
      \" ; \" + myFormat(\"0000\",(double)qteDetenue) + \" actions \" + actDetenue + \" valant \" + 
      myFormat(\"000.00\",cours2) +  \" = \" + myFormat(\"0000.00\",valActions));
   */
  }
  catch (Exception e)
  {
   System.out.println (\"Erreur dans simulation : \" + e.getMessage());
  }
 }

 

";
} print " 

 public static void commandesSpeciales ()
 {
  try
  {
   String reponse;
   BufferedReader in = new BufferedReader( new InputStreamReader( System.in ));
   System.out.println (\" 1: Renseigner les noms des actions\");
   System.out.println (\" 2: Afficher tous les cours des actions pour toutes les dates\");
   System.out.println (\"\");
   System.out.print (\" Votre choix ? \");
   reponse = in.readLine();
   if (reponse.equals(\"1\"))
    lireNomsActions();
   else if (reponse.equals(\"2\"))
    afficherCoursActions();  
  }
  catch (Exception e)
  {
   System.out.println (\"Erreur dans commandesSpeciales : \" + e.getMessage());
  }
 }

 public static void entreePourContinuer ()
 {
  try
  {
   String reponse;
   BufferedReader in = new BufferedReader( new InputStreamReader( System.in ));
   System.out.print (\"--- Tapez Entree pour continuer ---\");
   in.readLine();
  }
  catch (Exception e)
  {
   System.out.println (\"Erreur dans entreePourContinuer : \" + e.getMessage());
  }
 }

 public static void afficherActionsPrerenseignees ()
 {
  int i;
  if (params.ordreActions.equals(\"saisie\"))
  {
   for (i=0; i<nActionsPrerenseignees; i++)
   {
    System.out.println (\" - \" + actionsPrerenseignees[i]);
    if (i % 20 == 19)
     entreePourContinuer();
   }
  }
  else if (params.ordreActions.equals(\"alphabetique\"))
  {
   String[] actionsPrerenseigneesTrie;
   int j, k;
   boolean trouve;
   actionsPrerenseigneesTrie = new String[maxActions];
   for (i=0; i<nActionsPrerenseignees; i++)
   {
    trouve = false;
    for (j=0; j<i; j++)
    {
     if (actionsPrerenseignees[i].compareTo(actionsPrerenseigneesTrie[j]) <= 0)
     {
      for (k=i; k>j; k--)
       actionsPrerenseigneesTrie[k] = actionsPrerenseigneesTrie[k-1];
      actionsPrerenseigneesTrie[j] = actionsPrerenseignees[i];
      trouve = true;
      break;
     }
    }
    if (!trouve)
     actionsPrerenseigneesTrie[i] = actionsPrerenseignees[i];
   }
   for (i=0; i<nActionsPrerenseignees; i++)
   {
    System.out.println (\" - \" + actionsPrerenseigneesTrie[i]);
    if (i % 20 == 19)
     entreePourContinuer();
   }
  }
 }

 public static void lireActionsPrerenseignees ()
 {
  try 
  {
   String reponse;
   BufferedReader in = new BufferedReader( new InputStreamReader( System.in ));
   int i;
   for (;;)
   {
    System.out.print (\"Nom de l'action ? \");
    reponse = in.readLine();
    if (reponse.length() == 0)
    {
     System.out.println (\"Fin de la lecture des noms des actions\");
     break;
    }
    else
    {
     actionsPrerenseignees[nActionsPrerenseignees++] = reponse;
    }
   }
  }
  catch (Exception e)
  {
   System.out.println (\"Erreur dans lireActionsPrerenseignees : \" + e.getMessage());
  }
 }
 
 public static void supprimerActionPrerenseignee ()
 {
  try
  {
   int i, j;
   String reponse;
   BufferedReader in = new BufferedReader( new InputStreamReader( System.in ));
   j = 0;

   if (params.ordreActions.equals(\"saisie\"))
   {
    for (i=0; i<nActionsPrerenseignees; i++)
    {
     System.out.println (new Integer(i+1).toString() + \" : \" + actionsPrerenseignees[i]);
     if (i % 20 == 19)
      entreePourContinuer();
    }
    System.out.print (\"Numero de l'action a supprimer ? \");
    reponse = in.readLine();
    j = Integer.parseInt(reponse);
   }
   else if (params.ordreActions.equals(\"alphabetique\"))
   {
    // System.out.println (\"ordre alphabetique\");
    String[] nomsActionsTrie;
    int j1, k, l, m;
    boolean trouve;
    nomsActionsTrie = new String[maxActions];
    // System.out.println (\"tri\");
    for (i=0; i<nActionsPrerenseignees; i++)
    {
     // System.out.println (\"i=\"+new Integer(i).toString()+\":\"+actionsPrerenseignees[i]);
     trouve = false;
     for (j1=0; j1<i; j1++)
     {
      // System.out.println (\"j1=\"+new Integer(j1).toString()+\":\"+nomsActionsTrie[j1]);
      if (actionsPrerenseignees[i].compareTo(nomsActionsTrie[j1]) <= 0)
      {
       for (k=i; k>j1; k--)
        nomsActionsTrie[k] = nomsActionsTrie[k-1];
       nomsActionsTrie[j1] = actionsPrerenseignees[i];
       trouve = true;
       break;
      }
     }
     if (!trouve)
      nomsActionsTrie[i] = actionsPrerenseignees[i];
    }
    // System.out.println (\"fin tri\");
    for (i=0; i<nActionsPrerenseignees; i++)
    {
     System.out.println (new Integer(i+1).toString() + \" : \" + nomsActionsTrie[i]);
     if (i % 20 == 19)
      entreePourContinuer();
    }
    System.out.print (\"Numero de l'action a supprimer ? \");
    reponse = in.readLine();

    l = Integer.parseInt(reponse);
    l = l-1;
    for (m=0; m<nActionsPrerenseignees; m++)
    {
     if (actionsPrerenseignees[m].equals(nomsActionsTrie[l]))
      j = m+1;
    }
   }
   else
    System.out.println(\"Erreur : ordre inconnu : \" + params.ordreActions);

   // System.out.println (\"j=\"+new Integer(j).toString());
   if ((j >= 1) && (j <= nActionsPrerenseignees))
   {
    j = j-1;
    for (i=j; i<nActionsPrerenseignees-1; i++)
     actionsPrerenseignees[i] = actionsPrerenseignees[i+1];
    nActionsPrerenseignees--;
   }
   else
   {
    System.out.println (\"Numero d'action incorrect\");
   }
  }
  catch (Exception e)
  {
   System.out.println (\"Numero d'action incorrect\");
  }
 }

 public static void enregistrerFichierActionsPrerenseignees (String param)
 {
  try
  {
   String nomFichier;
   BufferedReader in = new BufferedReader( new InputStreamReader( System.in ));
   int i;
   if (param.equals(\"\"))
   {
    System.out.print (\"Nom du fichier ? \");
    nomFichier = in.readLine();
   }
   else
    nomFichier = param;
   nomFichier = dossier + nomFichier + \"-apr.txt\";
   // System.out.println (\"Enregistrement des donnees dans le fichier \" + nomFichier);
   PrintWriter fichier = new PrintWriter(new FileOutputStream(nomFichier));
   // fichier.println (\"nDatesChoix=\"+Integer.toString(params.nDatesChoix)+\";seuil=\"+Double.toString(params.seuil)+\";ordre=\"+params.ordre+\";ordreActions=\"+params.ordreActions);
   if (nActionsPrerenseignees > 0)
   {
    fichier.print (actionsPrerenseignees[0]);
    for (i=1; i<nActionsPrerenseignees; i++)
     fichier.print(\";\"+actionsPrerenseignees[i]);
   }
   fichier.println(\"\");
   // fichier.println (new DecimalFormat(\"00\").format(jourRef) + \"/\" +
   //  new DecimalFormat(\"00\").format(moisRef) + \"/\" +
   //  new DecimalFormat(\"0000\").format(anneeRef) + \";\" +
   //  new DecimalFormat(\"00\").format(heureRef) + \":\" +
   //  new DecimalFormat(\"00\").format(minRef));
   // for (i=0; i<nCours; i++)
   // {
   //  Cours cours = coursActions[i];
   //  fichier.println (cours.nomAction + \";\" +
   //   new DecimalFormat(\"00\").format(cours.jour) + \"/\" +
   //   new DecimalFormat(\"00\").format(cours.mois) + \"/\" +
   //   new DecimalFormat(\"0000\").format(cours.annee) + \";\" +
   //   new DecimalFormat(\"00\").format(cours.heure) + \":\" + 
   //   new DecimalFormat(\"00\").format(cours.min) + \";\" +
   //   new DecimalFormat(\"#0.000\").format(cours.cours));
   // }
   fichier.close();
  }
  catch (Exception e)
  {
   System.out.println (\"Erreur dans enregistrerFichier : \" + e.getMessage());
  }
 }
  
 static void ajouterActionsPrerenseignees (String[] nomsActions1)
 {
  int i, j;
  boolean trouve;

  for (i=0; i<nomsActions1.length; i++)
  {
   trouve = false;
   for (j=0; j<nActionsPrerenseignees; j++)
   {
    if (actionsPrerenseignees[j].equals(nomsActions1[i]))
    {
     trouve = true;
     break;
    }
   }
   if (!trouve)
   {
     actionsPrerenseignees[nActionsPrerenseignees++] = nomsActions1[i];
   }
  }
 }

 static void ajouterActionPrerenseignee (String nomAction)
 {
   int i, j;
   boolean trouve;

   trouve = false;
   for (j=0; j<nActionsPrerenseignees; j++)
   {
    if (actionsPrerenseignees[j].equals(nomAction))
    {
     trouve = true;
     break;
    }
   }
   if (!trouve)
   {
     actionsPrerenseignees[nActionsPrerenseignees++] = nomAction;
   }
  
 }


 public static void lireFichierActionsPrerenseignees (String param)
 {
  try 
  {
   String nomFichier;
   String reponse;
   BufferedReader in = new BufferedReader( new InputStreamReader( System.in ));
   int i, j;
   if (param.equals(\"\"))
   {
    File dir = new File(dossier);
    String[] files = dir.list(filterAPR);
    for (i=0; i<files.length; i++)
    {
     System.out.println(new Integer(i+1).toString() + \": \" + files[i].substring(0,files[i].length()-8));
    }
    System.out.print (\"Fichier a lire ? \");
    reponse = in.readLine();
    try
    {
     i = Integer.parseInt(reponse);
     nomFichier = dossier + files[i-1].substring(0,files[i-1].length()-8);
    }
    catch (Exception e)
    {
     nomFichier = dossier + reponse;
    }
   }
   else
    nomFichier = dossier + param;
   nomFichier = nomFichier + \"-apr.txt\";
   System.out.println (\"Lecture du fichier \" + nomFichier);
   BufferedReader fichier = new BufferedReader (new InputStreamReader (new FileInputStream (nomFichier)));
   String ligne;
   // String ligne = fichier.readLine();
   // String[] paramsLus = ligne.split(\";\");
   // for (j=0; j<paramsLus.length; j++)
   // {
   //  String[] paramLu = paramsLus[j].split(\"=\");
   //  modifierParam (paramLu[0], paramLu[1], false);
   // }
   ligne = fichier.readLine();
   // System.out.println(\"Ligne noms actions : \" + ligne + \".\");
   if (ligne.length() > 0)
   {
    // System.out.println (\"Longueur \" + ligne.length());
    // for (int k=0; k<ligne.length(); k++)
    //  System.out.println (\"Caractere \" + k + \" : code \" + (int)(ligne.charAt(k)));
    String[] nomsActions1 = ligne.split(\";\");
    ajouterActionsPrerenseignees (nomsActions1);
   }
   else
   {
    // System.out.println (\"vide nActions = \" + nActions);
   }
   fichier.close();
  }
  catch (Exception e)
  {
   System.out.println (\"Erreur dans lireFichierActionsPrerenseignees : \" + e.getMessage()); 
   e.printStackTrace();
  }
 }

 public static void suivreAction (String nomAction)
 {
   int i, j;
   boolean trouve;
   trouve = false;
   for (j=0; j<nActions; j++)
   {
    if (nomsActions[j].equals(nomAction))
    {
     trouve = true;
     break;
    }
   }
   if (!trouve)
   {
     nomsActions[nActions++] = nomAction;
   }
 }

 public static void suivreActionsPrerenseignees ()
 {
  try
  {
   int i, j, h;
   String reponse;
   String[] numeros;
   BufferedReader in = new BufferedReader( new InputStreamReader( System.in ));
   j = 0;
   if (params.ordreActions.equals(\"saisie\"))
   {
    for (i=0; i<nActionsPrerenseignees; i++)
    {
     System.out.println (new Integer(i+1).toString() + \" : \" + actionsPrerenseignees[i]);
     if (i % 20 == 19)
      entreePourContinuer();
    }
    System.out.print (\"Numeros des actions a suivre separees par des virgules ? \");
    reponse = in.readLine();
    numeros = reponse.split(\",\");
    for (h=0; h<numeros.length; h++)
    {
     j = Integer.parseInt(numeros[h]);
     suivreAction (actionsPrerenseignees[j-1]);
    }
    // j = Integer.parseInt(reponse);
   }
   else if (params.ordreActions.equals(\"alphabetique\"))
   {
    String[] nomsActionsTrie;
    int j1, k, l, m;
    boolean trouve;
    nomsActionsTrie = new String[maxActions];
    for (i=0; i<nActions; i++)
    {
     // System.out.println (\"i=\"+new Integer(i).toString());
     trouve = false;
     for (j1=0; j1<i; j1++)
     {
      if (actionsPrerenseignees[i].compareTo(nomsActionsTrie[j1]) <= 0)
      {
       System.out.println(\"Trouve\");
       for (k=i; k>j1; k--)
        nomsActionsTrie[k] = nomsActionsTrie[k-1];
       nomsActionsTrie[j1] = nomsActions[i];
       trouve = true;
       break;
      }
     }
     if (!trouve)
      nomsActionsTrie[i] = actionsPrerenseignees[i];
    }
    for (i=0; i<nActionsPrerenseignees; i++)
    {
     System.out.println (new Integer(i+1).toString() + \" : \" + nomsActionsTrie[i]);
     if (i % 20 == 19)
      entreePourContinuer();
    }
    System.out.print (\"Numero des actions a suivre separees par des virgules ? \");
    reponse = in.readLine();
    numeros = reponse.split(\",\");
    for (h=0; h<numeros.length; h++)
    {
     l = Integer.parseInt(reponse);
     suivreAction(nomsActionsTrie[l-1]);
    }    
   }

   // suivreAction (actionsPrerenseignees[j-1]);

  }
  catch (Exception e)
  {
   System.out.println (\"Numero d'action incorrect\");
  }
 }
 
"; if ($ARGV[0] eq "new") { print "

 public static void telechargerActionsPrerenseignees ()
 {
  try
  {
   BufferedReader in = new BufferedReader( new InputStreamReader( System.in ));
   System.out.print (\"Symbole de l'indice (par exemple FCHI pour le CAC 40) ? \");
   String reponse = in.readLine();
   String actions = lireActionsWeb(reponse);
   String[] lignes = actions.split(\"\\n\");
   int i;
   for (i=0; i<lignes.length; i++)
   {
    String[] champs = lignes[i].split(\",\");
    String[] champs0 = champs[0].split(\"\\\"\");
    String[] champs1 = champs[1].split(\"\\\"\");
    String nomAction = champs0[1] + \" \" + champs1[1];
    System.out.println (new Integer(i).toString() + \" : \" + nomAction);
    ajouterActionPrerenseignee(nomAction);
   }
  }
  catch (Exception e)
  {
   System.out.println (\"Erreur dans telechargerActionsPrerenseignees : \" + e.getMessage());
  }
 }
 
"; } print "

 public static void gererActionsPrerenseignees ()
 {
  try
  {
   BufferedReader in = new BufferedReader( new InputStreamReader( System.in ));
   String reponse;
   System.out.println (\" 0: Effacer les actions prerenseignees\");
   System.out.println (\" 1: Afficher les noms des actions prerenseignees\");
   System.out.println (\" 2: Renseigner les noms des actions prerenseignees\");
   System.out.println (\" 3: Supprimer une action prerenseignee\");
   System.out.println (\" 4: Enregistrer les actions prerenseignees dans un fichier\");
   System.out.println (\" 5: Lire un fichier d'actions prerenseignees\");
   System.out.println (\" 6: Indiquer des actions prerenseignees a suivre\");
   System.out.println (\"\");
   System.out.print (\" Votre choix ? \");
   reponse = in.readLine();
   if (reponse.equals(\"0\"))
    nActionsPrerenseignees = 0;
   else if (reponse.equals(\"1\"))
    afficherActionsPrerenseignees();
   else if (reponse.equals(\"2\"))
    lireActionsPrerenseignees();
   else if (reponse.equals(\"3\"))
    supprimerActionPrerenseignee();
   else if (reponse.equals(\"4\"))
    enregistrerFichierActionsPrerenseignees(\"\");
   else if (reponse.equals(\"5\"))
    lireFichierActionsPrerenseignees(\"\");
   else if (reponse.equals(\"6\"))
    suivreActionsPrerenseignees();
"; if ($ARGV[0] eq "new") { print "
   else if (reponse.equals(\"7\"))
    telechargerActionsPrerenseignees();
"; } print "
  }
  catch (Exception e)
  {
   System.out.println (\"Erreur dans gererActionsPrerenseignees\");
  }
 }

 public static void main (String args[])
 {
  try
  {
   anneeDebut = 30000;
   moisDebut = 12;
   jourDebut = 31;
   heureDebut = 23;
   minDebut = 59;
   params = new Parametres();
   params.maxDateRef = 7;
   String reponse;
   BufferedReader in = new BufferedReader( new InputStreamReader( System.in ));
   nActions = 0;
   nCours = 0;
   nomsActions = new String[maxActions];
   actionsPrerenseignees = new String[maxActions];
   coursActions = new Cours[maxCours];
   String fichierSauvegarde;
   // if (args.length > 0)
   //  fichierSauvegarde = args[0];
   // else
   // fichierSauvegarde = \"sauvegarde\";
   if (args.length > 0)
    dossier = args[0];
   else
    dossier = \"\";
   // fichierSauvegarde = dossier + \"sauvegarde\";
   fichierSauvegarde = \"sauvegarde\";
   // System.out.println (\"length = \" + args.length + \" fichierSauvegarde = \" + fichierSauvegarde);
   init();
   if (new File(fichierSauvegarde + \"-cours.txt\").exists())
    lireFichier(fichierSauvegarde);
   if (new File(fichierSauvegarde + \"-apr.txt\").exists())
    lireFichierActionsPrerenseignees(fichierSauvegarde);
   boolean cont = true;
   while (cont)
   {
    System.out.println (\"\");
    System.out.println (\"ACTIONS Version 13 du 24/07/2013\");
    System.out.println (\"Calcul de la progression d'une selection d'actions\");
    System.out.println (\"--------------------------------------------------\");
    System.out.println (\"\");
    System.out.println (\"0: Effacer les donnees dans le fichier\");
    System.out.println (\"1: Parametres\");
    // System.out.println (\"2: Lire les noms des actions\");
    System.out.println (\"2: Afficher les noms des actions\");
    System.out.println (\"3: Indiquer la date de reference et renseigner les cours correspondants\");
    System.out.println (\"4: Renseigner les cours finaux des actions et afficher les progressions\");
    System.out.println (\"5: Afficher les cours de reference\"); 
    // System.out.println (\"7: Afficher tous les cours des actions pour toutes les dates\");
    System.out.println (\"6: Indiquer la date de reference\");
    System.out.println (\"7: Enregistrer les cours dans un fichier\");
    System.out.println (\"8: Lire un fichier de cours\");
    System.out.println (\"9: Calculer les taux de progression\");
    System.out.println (\"10: Enregistrer les donnees dans le fichier courant et terminer\");
    System.out.println (\"11: Indiquer une action detenue\");
    System.out.println (\"12: Telecharger les derniers cours sur Internet\"); 
    System.out.println (\"13: Afficher les derniers enregistrements\"); 
    System.out.println (\"14: Commandes speciales\");
    System.out.println (\"15: Supprimer une action\");
    System.out.println (\"16: Gerer les actions prerenseignees\");
"; if ($ARGV[0] eq "new") { print "
    System.out.println (\"21: Importer un fichier de cours\");
    System.out.println (\"22: Simulation\"); 
    System.out.println (\"23: Telecharger les derniers cours sur Internet\"); 
"; } print "
    System.out.println (\"\");
    System.out.print (\"Votre choix ? \");
    reponse = in.readLine();
    choixMenu = reponse;
    System.out.println (\"\");
    if (reponse.equals(\"0\"))
     effacer();
    else if (reponse.equals(\"1\"))
     gererParametres();
//    else if (reponse.equals(\"2\"))
//     nActions = lireNomsActions ();
    else if (reponse.equals(\"2\"))
     afficherNomsActions ();
    else if (reponse.equals(\"3\"))
     nCours = lireCoursActions(true);
    else if (reponse.equals(\"4\"))
     nCours = lireCoursActions (false);
    else if (reponse.equals(\"5\"))
     afficherProgression (anneeRef, moisRef, jourRef);
//    else if (reponse.equals(\"7\"))
//     afficherCoursActions ();
    else if (reponse.equals(\"6\"))
     choisirDateRef ();
    else if (reponse.equals(\"7\"))
     enregistrerFichier (\"\");
    else if (reponse.equals(\"8\"))
     lireFichier (\"\");
    else if (reponse.equals(\"9\"))
     progression ();
    else if (reponse.equals(\"10\"))
    {
     enregistrerFichier(fichierSauvegarde);
     System.out.println (\"Les donnees ont ete enregistrees dans le fichier courant.\");
     System.out.print (\"Voulez-vous vraiment quitter le programme (o/n) ? \");
     try
     {
      reponse = in.readLine();
      if (reponse.charAt(0) == 'o' || reponse.charAt(0) == 'O')
       cont = false;
     } 
     catch (Exception e)
     {
     }  
    }
    else if (reponse.equals(\"11\"))
     indiquerActionDetenue();
    else if (reponse.equals(\"12\"))
     lireTousCoursWeb();
    else if (reponse.equals(\"13\"))
     afficherDerniersEnregistrements();
    else if (reponse.equals(\"14\"))
     commandesSpeciales();
    else if (reponse.equals(\"15\"))
     supprimerAction();
    else if (reponse.equals(\"16\"))
     gererActionsPrerenseignees();
"; if ($ARGV[0] eq "new") { print "
    else if (reponse.equals(\"21\"))
     importer ();
    else if (reponse.equals(\"22\"))
     simulation();
    else if (reponse.equals(\"23\"))
     lireTousCoursWeb();
"; } print "
    // System.out.println (\"fichierSauvegarde=\"+fichierSauvegarde);
    enregistrerFichier(fichierSauvegarde);
    enregistrerFichierActionsPrerenseignees(fichierSauvegarde);
   }
  }
  catch (Exception e)
  {
   System.out.println (\"Erreur dans main : \" + e.getMessage());
  }
 }
}

";
