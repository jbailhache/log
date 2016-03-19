
import java.lang.*;
import java.util.*;
import java.awt.*;
import java.awt.event.*;
import javax.swing.*;
import javax.swing.border.*;
import javax.swing.table.*;
import javax.swing.event.*;
import javax.swing.text.*;
import javax.swing.filechooser.*;
import java.text.*;
import java.io.*;
import java.net.*;
import java.util.zip.*;
import java.sql.*;
import com.java4less.rmail.*;
import com.java4less.rmime.*;

/** Connexion avec le serveur */
public class ServerConnection
{
   public String host;
   public int port;
   public Socket sock;
   public InputStream is;
   public OutputStream os;
   public String message;

public static boolean client_timeout(InputStream is, int msec)
	{		
		int delai_attente = 1;
		int time = 0;
		try
		{
			while(is.available() == 0)
			{
//				sleep(delai_attente);
// problème : fonction sleep dans thread uniquement, à corriger
//				sv.append("Attente...\n", Color.darkGray, false, false, false);
				time += delai_attente;
				if(time > msec)
				{
//					sv.append("Timeout!\n", new Color(150, 0, 0), false, false, false);
					return true;
				}
			}
		}
		catch(Exception e)
		{
			e.printStackTrace();
		}
		return false;
	}


public static String recoit_param(InputStream is, String filter)
	{	
		int timeout = 60;	
		String str = new String("");
		boolean erreur = false;
		int count = 0;
		try
		{
			for(;;)
			{
				/* le timeout ne marche pas, à résoudre
				if(client_timeout(is, timeout*1000))
				{
					erreur = true;
					break;
				}*/
				char c = (char)is.read();
				System.out.print(c);
				count++;
				if(count > 256)
				{
					erreur = true;
					break;
				}
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
			return str;
	}
 

   /** Création d'une connexion
	@param phost hôte
	@param pport port
   */
   ServerConnection (String phost, int pport)
   {
      host = phost;
      port = pport;
   }

   /** Envoi de l'identification au serveur
	@param user nom de l'utilisateur
	@param password mot de passe 
   */
   public boolean login (String user, String password)
   {
	try
	{
	
	/* connexion */
	
	sock = new Socket (host, 6020);
	is = sock.getInputStream();
	os = sock.getOutputStream();

	String com;
      String commande;
	boolean auth = false;
	boolean existe = false;
	
	// envoi du login et du mot de passe
	com = new String("USER\n" + user +"\n" + "PASS\n" + password +"\n");
	os.write(com.getBytes("US-ASCII"));
	os.flush();
	commande = new String(recoit_param(is, "alpha"));
        /*label.setText ("received (1): " + com);*/
	System.out.println ("received(1): " + commande);

	commande = new String(recoit_param(is, "any"));
	System.out.println ("received(2): " + commande);

	if (commande.equals("1")) 
	      return true;
	else
	{
		message = "Invalid login or password";
		return false;
	}
      }
      catch (Exception e)
      {
       System.out.println ("Error login:" + e.getMessage());
	 message = "Error connecting to host " + host + " : " + e.getMessage();
       return false;
      }
   }

   /** Envoi d'une commande au serveur */
   public boolean output (String com)
   {
	try
	{
		os.write (com.getBytes("US-ASCII"));
		os.flush();
		return true;
	}
	catch (Exception e)
	{
		System.out.println ("Error output: " + e.getMessage());
		return false;
	}
   }

   /** Réception de la réponse du serveur */
   public String input (String typ)
   {
	try
	{
		String commande = new String (recoit_param (is, typ));
		return commande;
	}
	catch (Exception e)
	{
		System.out.println ("Error input: " + e.getMessage());
		return null;
	}

   }

   /** Réception de la réponse du serveur sous forme d'un tableau d'octets 
	@param n nombre d'octets à recevoir 
	*/
   public byte[] inputBytes (int n)
   {
	byte[] data;
      try
      {
		data = new byte[n];
		for (int i=0; i<n; i++)
		{
			int c = is.read();
			byte b = (byte)c;
			data[i] = b;
		}
		return data;
      }
      catch (Exception e)
      {
		System.out.println ("Error inputBytes: " + e.getMessage());
		return new byte[0];
      }
   }


   /** Réception de la réponse du serveur sous forme d'un tableau d'entiers
	@param n nombre d'octets à recevoir 
	*/
   public int[] inputInts (int n)
   {
	int[] data;
      try
      {
		data = new int[n];
		for (int i=0; i<n; i++)
		{
			int c = is.read();
			/*
			byte b = (byte)c;
			data[i] = b;
			*/
			data[i] = c;
		}
		return data;
      }
      catch (Exception e)
      {
		System.out.println ("Error inputBytes: " + e.getMessage());
		return new int[0];
      }
   }

   /** Déconnexion */
   public boolean logout ()
   {
		try 
		{
									/* Déconnexion */
									String com = new String("END\n");
									os.write(com.getBytes("US-ASCII"));
									os.flush();
									sock.close();
									return true;
		}
		catch (Exception e)
		{
			System.out.println ("Error logout: " + e.getMessage());
			return false; 
		}
   }

}
