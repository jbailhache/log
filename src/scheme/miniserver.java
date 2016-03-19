
import java.lang.*;
import java.util.*;
import java.applet.*;
import java.awt.*;
import java.awt.event.*;
import javax.swing.*;
import javax.swing.text.*;
import javax.swing.border.*;
import javax.swing.table.*;
import javax.swing.event.*;
import java.io.*;
import java.net.*;
import java.text.*;
import java.util.zip.*;
import java.sql.*;

public class miniserver 
{

	public static String dirfax;
	public static String basedroits;
	public static String basefax;
	public static String faxdir;
	public static String language;
	public static String login;
	public static String pass;

      public static String recoit_param(InputStream is, String filter)
	{		
		String str = new String("");
		boolean erreur = false;
		int count = 0;
		try
		{
			for(;;)
			{
				/*
				if(client_timeout(is, sv.timeout*1000))
				{
					erreur = true;
					break;
				}
				*/
				char c = (char)is.read();
				count++;
				/*
				if(count > 256)
				{
					erreur = true;
					break;
				}
				*/
				if(c == '\n')
					break;
				if(filter.equals("alpha") && !Character.isLetter(c))
				   continue;
				if(filter.equals("num") && !Character.isDigit(c))
				   continue;
				if(filter.equals("alphanum") && !Character.isLetterOrDigit(c))
				   continue;
				str = str + c;
			}
		}
		catch(Exception e)
		{
			e.printStackTrace();
		}
		if(erreur)
			return new String("");
		else
		{
			/*System.out.println ("<- " + str);*/
			return str;
		}
	}

 public static String lire_fichier (String filename)
 {
  try
  {
  System.out.print ("Lecture fichier " + filename);
  File f = new File (filename);
  FileInputStream is = new FileInputStream (f);
  String buf = "";
  int c;
  for (;;)
  {
   c = is.read();
   if (c== 13 ||c == 10 || c == -1)
    break;
   buf = buf + (char) c;
  }
  is.close();
  System.out.println (" : " + buf);
  return buf;
  }
  catch (Exception e)
  {
   e.printStackTrace();
   return "";
  }
 }

 public static void main (String[] argv)
 {
  try
  {
  System.out.println ("miniserver starting");

  ServerSocket socket = new ServerSocket (6050);

  for (;;)
  {
   Socket sc = socket.accept();
   System.out.println ("accepted");
   InputStream is = sc.getInputStream();
   OutputStream os = sc.getOutputStream();
   String commande;
   String reponse;
   boolean cont=true;
   while (cont)
   {
    System.out.println ("Attente commande...");
    commande = recoit_param (is, "any");
    System.out.println ("Commande: " + commande);
    /*
    reponse = "recu:"+commande;
    os.write (reponse.getBytes("US-ASCII"));
    os.flush();
    */

    if (commande.equals ("BASEFAX"))
    { 
	dirfax = recoit_param (is, "any");
	language = recoit_param (is, "any");
	
	System.out.println ("dirfax=" + dirfax + " language=" + language);

      if (!dirfax.equals(""))
      {
	 basedroits = lire_fichier (dirfax + "\\" + "baserights.txt");
	 basefax = lire_fichier (dirfax + "\\" + "basefax.txt");
	 faxdir = lire_fichier (dirfax + "\\" + "faxdir.txt");
      }

      reponse = 
		"ID\tIdentifiant\t30\n" +
		"CNX\tDate\t20\n" +
		"TIME\tDurée\t5\n" +
		"PGENB\tPages\t5\n" +
		"HGN\tStatus\t5\n" +
		"FILE\tFichier\t15\n" +
		"\n";
      System.out.print(reponse);
	os.write (reponse.getBytes("US-ASCII"));
	os.flush();
    }
    else if (commande.equals ("USER"))
    {
	login = recoit_param (is, "any");
    }
    else if (commande.equals ("PASS"))
    {
	pass = recoit_param (is, "any");
	reponse = "LOGIN\n1\n";
      System.out.print(reponse);
	os.write (reponse.getBytes("US-ASCII"));
	os.flush();

    }
    else if (commande.equals ("LISTREC"))
    {
	String param = recoit_param (is, "any");
	String strsince = recoit_param (is, "any");

	Connection con = null;

	String loginBDD = "xxx";
	String passwordBDD = "yyy";

	try
      {
		Class.forName ("sun.jdbc.odbc.JdbcOdbcDriver").newInstance();
		System.out.println ("basefax=" + basefax);
						    						
		con = DriverManager.getConnection ("jdbc:odbc:" + basefax, loginBDD, passwordBDD);
 
	 	String query = "SELECT * FROM " + "REC" + " " + param;
		System.out.println (query);
		ResultSet results;
                            
		Statement stmt = con.createStatement();
		results = stmt.executeQuery (query);
                					
		int n = 0;

		String com = new String("LISTREC\n" + "" + "\n");
		System.out.println (com);
		os.write(com.getBytes("US-ASCII"));
		os.flush();

		java.util.Date lastfax = null;
		System.out.println ("Reading base...");
		while (results.next())
		{
			/*
			String[] field = new String[120];

                  for (int i=0; i<fieldnames.length-1; i++)
			{
                  	try {field[i] = results.getString(i+1);}
                        catch (Exception e)
                        { field[i] = "????"; }
			}
			*/
			String id = results.getString("ID");
			String cnx = results.getString("CNX");
			String time = results.getString("TIME");
			String pgenb = results.getString("PGENB");
			String hgn = results.getString("HGN");
			String file = results.getString("FILE");
			reponse = "<MESSAGE><BASE>FAXBT</BASE><ID>" + id
				+ "</ID><CNX>" + cnx 
				+ "</CNX><TIME>" + time 
				+ "</TIME><PGENB>" + pgenb
				+ "</PGENB><HGN>" + hgn
				+ "</HGN><FILE>" + file 
				+ "</FILE></MESSAGE>\n";
			// System.out.print(reponse);
			os.write (reponse.getBytes("US-ASCII"));
			os.flush();

		}
		reponse = "\n";
		os.write (reponse.getBytes("US-ASCII"));
		os.flush();

	}
	catch (Exception exc)
      {
		exc.printStackTrace();
      }


    }
    else if (commande.equals ("END"))
    {
	cont = false;
    }
   }
   
   
  }
  }
  catch (Exception exc)
  {
   exc.printStackTrace();
  }

 }
}