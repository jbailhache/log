
import java.io.*;
import java.text.*;

public class Actions
{
 static int maxActions = 1000;
 static int maxCours = 10000;
 static int nActions;
 static String[] nomsActions;
 static int nCours;
 static Cours[] coursActions;
 
 static int anneeRef, moisRef, jourRef, heureRef, minRef;

 static void effacer ()
 {
  nActions = 0;
  nCours = 0;
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
    System.out.print ("Nom de l'action ? ");
    reponse = in.readLine();
    if (reponse.length() == 0)
    {
     System.out.println ("Fin de la lecture des noms des actions");
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
   System.out.println ("Erreur dans lireNomsActions : " + e.getMessage());
   return nActions;
  }
 }

 static void afficherNomsActions ()
 {
  int i;
  for (i=0; i<nActions; i++)
   System.out.println (" - " + nomsActions[i]);
 }

 static int lireCoursActions ()
 {
  try
  {
   String reponse;
   Cours cours; 
   BufferedReader in = new BufferedReader( new InputStreamReader( System.in ));
   int i;
   System.out.print ("Annee   ? ");
   reponse = in.readLine();
   int annee = Integer.parseInt(reponse);
   System.out.print ("Mois    ? ");
   reponse = in.readLine();
   int mois = Integer.parseInt(reponse);
   System.out.print ("Jour    ? ");
   reponse = in.readLine();
   int jour = Integer.parseInt(reponse);
   System.out.print ("Heure   ? ");
   reponse = in.readLine();
   int heure = Integer.parseInt(reponse);
   System.out.print ("Minutes ? ");
   reponse = in.readLine();
   int min = Integer.parseInt(reponse);
   for (i=0; i<nActions; i++)
   {
    cours = new Cours();
    cours.annee = annee;
    cours.mois = mois;
    cours.jour = jour;
    cours.heure = heure;
    cours.min = min;
    cours.nomAction = nomsActions[i];
    System.out.print ("Cours de l'action " + nomsActions[i] + " ? ");
    reponse = in.readLine();
    reponse = reponse.replace(',','.');
    if (!reponse.equals(""))
    {
     cours.cours = Float.valueOf(reponse).floatValue();
     coursActions[nCours++] = cours;
    }
   }
   afficherProgression (annee, mois, jour); 
   return nCours;
  }
  catch (Exception e)
  {
   System.out.println ("Erreur dans lireCoursActions : " + e.getMessage());
   return nCours;
  }
 }

 static String myFormat (String f, float x)
 {
  String s = new DecimalFormat(f).format(x);
  String t = "";
  int i;
  for (i=0; i<s.length(); i++)
  {
   if (s.charAt(i) == '0')
    t = t + " ";
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
  int size = 12;
  if (s.length() > size)
   return s.substring(0,size);
  else
  {
   String r;
   r = s;
   for (i=0; i<size-s.length(); i++)
    r = r + " ";
   return r;
  }
 }

 static String formatDate (int annee, int mois, int jour)
 {
   return new DecimalFormat("00").format(jour) + "/" +
     new DecimalFormat("00").format(mois) + "/" +
     new DecimalFormat("0000").format(annee);
 }

 static String formatDateRef ()
 {
   return new DecimalFormat("00").format(jourRef) + "/" +
     new DecimalFormat("00").format(moisRef) + "/" +
     new DecimalFormat("0000").format(anneeRef) + " " +
     new DecimalFormat("00").format(heureRef) + ":" +
     new DecimalFormat("00").format(minRef);
 }
 
 static void afficherCoursActions ()
 {
  int i;
  Cours cours;
  for (i=0; i<nCours; i++)
  {
   cours = coursActions[i];
   System.out.println (
    new DecimalFormat("00").format(cours.jour) + "/" +
    new DecimalFormat("00").format(cours.mois) + "/" +
    new DecimalFormat("0000").format(cours.annee) + " " +
    new DecimalFormat("00").format(cours.heure) + ":" +
    new DecimalFormat("00").format(cours.min) + " " + 
    myFormat ("000000.000",cours.cours) + " " + cours.nomAction);
   /* System.out.format (null, "%02d/%02d/%04d\t%8.2d\t%s\n", 
    cours.jour, cours.mois, cours.annee, cours.cours, cours.nomAction); */
  }
 }

 static void lireDateRef ()
 {
  try 
  {
   BufferedReader in = new BufferedReader( new InputStreamReader( System.in ));
   String reponse;
   System.out.println ("Date de reference");
   System.out.print ("Annee ? ");
   reponse = in.readLine();
   anneeRef = Integer.parseInt(reponse);
   System.out.print ("Mois  ? ");
   reponse = in.readLine();
   moisRef = Integer.parseInt(reponse);
   System.out.print ("Jour  ? ");
   reponse = in.readLine();
   jourRef = Integer.parseInt(reponse);
   System.out.print ("Heure ? ");
   reponse = in.readLine();
   heureRef = Integer.parseInt(reponse);
   System.out.print ("Minutes ? ");
   reponse = in.readLine();
   minRef = Integer.parseInt(reponse);
  }
  catch (Exception e)
  {
   System.out.println ("Erreur dans LireDateRef");
  }
 }

 static void enregistrerFichier (String param)
 {
  try
  {
   String nomFichier;
   BufferedReader in = new BufferedReader( new InputStreamReader( System.in ));
   int i;
   if (param.equals(""))
   {
    System.out.print ("Nom du fichier ? ");
    nomFichier = in.readLine();
   }
   else
    nomFichier = param;
   PrintWriter fichier = new PrintWriter(new FileOutputStream(nomFichier));
   fichier.print (nomsActions[0]);
   for (i=1; i<nActions; i++)
    fichier.print(";"+nomsActions[i]);
   fichier.println("");
   fichier.println (new DecimalFormat("00").format(jourRef) + "/" +
     new DecimalFormat("00").format(moisRef) + "/" +
     new DecimalFormat("0000").format(anneeRef) + ";" +
     new DecimalFormat("00").format(heureRef) + ":" +
     new DecimalFormat("00").format(minRef));
   for (i=0; i<nCours; i++)
   {
    Cours cours = coursActions[i];
    fichier.println (cours.nomAction + ";" +
     new DecimalFormat("00").format(cours.jour) + "/" +
     new DecimalFormat("00").format(cours.mois) + "/" +
     new DecimalFormat("0000").format(cours.annee) + ";" +
     new DecimalFormat("00").format(cours.heure) + ":" + 
     new DecimalFormat("00").format(cours.min) + ";" +
     new DecimalFormat("#0.000").format(cours.cours));
   }
   fichier.close();
  }
  catch (Exception e)
  {
   System.out.println ("Erreur dans enregistrerFichier : " + e.getMessage());
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

 public static void lireFichier (String param)
 {
  try 
  {
   String nomFichier;
   BufferedReader in = new BufferedReader( new InputStreamReader( System.in ));
   int i;
   if (param.equals(""))
   {
    System.out.print ("Nom du fichier ? ");
    nomFichier = in.readLine();
   }
   else
    nomFichier = param;
   BufferedReader fichier = new BufferedReader (new InputStreamReader (new FileInputStream (nomFichier)));
   String ligne = fichier.readLine();
   String[] nomsActions1 = ligne.split(";");
   ajouterActions (nomsActions1);
   ligne = fichier.readLine();
   String[] champs = ligne.split(";");
   String[] date = champs[0].split("/");
   anneeRef = Integer.parseInt(date[2]);
   moisRef = Integer.parseInt(date[1]);
   jourRef = Integer.parseInt(date[0]);
   String[] hm = champs[1].split(":");
   heureRef = Integer.parseInt(hm[0]);
   minRef = Integer.parseInt(hm[1]);

   System.out.println ("Date de reference : " + champs[0] + " " + champs[1]);

   for (;;)
   {
    try
    {
     ligne = fichier.readLine();
     champs = ligne.split(";");
     Cours cours = new Cours();
     cours.nomAction = champs[0];
     date = champs[1].split("/");
     cours.annee = Integer.parseInt(date[2]);
     cours.mois = Integer.parseInt(date[1]);
     cours.jour = Integer.parseInt(date[0]);
     hm = champs[2].split(":"); 
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
   System.out.println ("Erreur dans lireFichier : " + e.getMessage()); 
   e.printStackTrace();
  }
 }

 public static float coursDate (String nomAction, int annee, int mois, int jour)
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

 public static void progression ()
 {
  try
  {
   BufferedReader in = new BufferedReader( new InputStreamReader( System.in ));
   String reponse;
   int annee1, mois1, jour1, annee2, mois2, jour2;
/*
   System.out.print ("Premiere date - annee ? ");
   reponse = in.readLine();
   annee1 = Integer.parseInt(reponse);
   System.out.print ("Premiere date - mois  ? ");
   reponse = in.readLine();
   mois1 = Integer.parseInt(reponse);
   System.out.print ("Premiere date - jour  ? ");
   reponse = in.readLine();
   jour1 = Integer.parseInt(reponse);
*/
   System.out.println ("Progression des cours entre la date de reference " + formatDateRef() + " et la date suivante :");
   System.out.print ("annee ? ");
   reponse = in.readLine();
   annee2 = Integer.parseInt(reponse);
   System.out.print ("mois  ? ");
   reponse = in.readLine();
   mois2 = Integer.parseInt(reponse);
   System.out.print ("jour  ? ");
   reponse = in.readLine();
   jour2 = Integer.parseInt(reponse);
   afficherProgression (annee2, mois2, jour2);
  }
  catch (Exception e)
  {
   System.out.println ("Erreur dans progression : " + e.getMessage());
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
   System.out.println ("");
   System.out.println ("Progression des cours entre la date de reference " + formatDateRef() + 
    " et le " + formatDate(annee2,mois2,jour2) + " :");
   System.out.println ("");
   System.out.println ("Nom action    Reference      Cours Progression");
   System.out.println ("----------    ---------      ----- -----------");
   for (i=0; i<nActions; i++)
   {
    float cours1 = coursDate (nomsActions[i], annee1, mois1, jour1);
    float cours2 = coursDate (nomsActions[i], annee2, mois2, jour2);
    if ((cours1 != 0) && (cours2 != 0))
    {
     float p = ((cours2 / cours1) - 1) * 100;
/*
     if (p > 0)
      System.out.println (new DecimalFormat("+00.000").format(p) + "% " + nomsActions[i]);
     else
      System.out.println (new DecimalFormat("-00.000").format(-p) + "% " + nomsActions[i]);
*/
     String afProgr;
     if (p >= 0)
      afProgr = new DecimalFormat("+0.00").format(p) + "%";
     else
      afProgr = new DecimalFormat("-0.00").format(-p) + "%";
     System.out.println (
      formatAction(nomsActions[i]) + " " + 
      myFormat("000000.000",cours1) + " " +
      myFormat("000000.000",cours2) + " " +
      afProgr);     
    }
   }
  }
  catch (Exception e)
  {
   System.out.println ("Erreur dans afficherProgression : " + e.getMessage());
  }
 }
 
 public static void main (String args[])
 {
  try
  {
   String reponse;
   BufferedReader in = new BufferedReader( new InputStreamReader( System.in ));
   nActions = 0;
   nCours = 0;
   nomsActions = new String[maxActions];
   coursActions = new Cours[maxCours];
   if (new File("cours.txt").exists())
    lireFichier("cours.txt");
   for (;;)
   {
    System.out.println ("");
    System.out.println ("Calcul de la progression d'une selection d'actions");
    System.out.println ("--------------------------------------------------");
    System.out.println ("");
    System.out.println ("0: Effacer les donnees");
    System.out.println ("1: Lire les noms des actions");
    System.out.println ("2: Afficher les noms des actions");
    System.out.println ("3: Lire les cours des actions");
    System.out.println ("4: Afficher les cours des actions");
    System.out.println ("5: Indiquer la date de reference");
    System.out.println ("6: Enregistrer les cours dans un fichier");
    System.out.println ("7: Lire un fichier de cours");
    System.out.println ("8: Calculer les taux de progression");
    System.out.println ("9: Terminer");
    System.out.println ("");
    System.out.print ("Votre choix ? ");
    reponse = in.readLine();
    System.out.println ("");
    if (reponse.equals("0"))
     effacer();
    else if (reponse.equals("1"))
     nActions = lireNomsActions ();
    else if (reponse.equals("2"))
     afficherNomsActions ();
    else if (reponse.equals("3"))
     nCours = lireCoursActions ();
    else if (reponse.equals("4"))
     afficherCoursActions ();
    else if (reponse.equals("5"))
     lireDateRef ();
    else if (reponse.equals("6"))
     enregistrerFichier ("");
    else if (reponse.equals("7"))
     lireFichier ("");
    else if (reponse.equals("8"))
     progression ();
    else if (reponse.equals("9"))
    {
     enregistrerFichier("cours.txt");
     break;
    }
   }
  }
  catch (Exception e)
  {
   System.out.println ("Erreur dans main : " + e.getMessage());
  }
 }
}

