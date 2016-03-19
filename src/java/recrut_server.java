import java.io.*;
import java.net.*;
import java.sql.*;
 
public class recrut_server 
{
	ServerSocket sock;
 	static int PORT;
 
	public recrut_server ()
	{
		try
		{
			sock = new ServerSocket (PORT);
			System.out.println ("Serveur actif");
		}
		catch (Exception e)
		{
			System.err.println ("Erreur: impossible de creer le socket serveur : " 
				+ e.getMessage());
		}
	}

	public static void main (String args[])
	{
		PORT = Integer.parseInt (args[0]); 
                recrut_server serv = new recrut_server ();
 		serv.demarrer ();
	}

	public void demarrer ()
	{
		Socket client = null;
 		while (true)
		{	
			if (sock == null)
				return;
			try { client = sock.accept (); }
			catch (Exception e)
			{
				System.err.println ("Erreur: impossible d'accepter l'appel : " 
					+ e.getMessage ());
			}
                        new recrut_thread(client,sock).start ();
		}
/*
		try { sock.close(); }
		catch (Exception e)
		{
			System.err.println ("Erreur: impossible d'accepter l'appel : " 
				+ e.getMessage ());
		}
*/
	}
}
 
class recrut_thread extends Thread
{

	private Socket client = null;
	private ServerSocket sock = null;

	public recrut_thread (Socket s, ServerSocket sock1)
	{
		super ("recrut_thread");
		client = s;
		sock = sock1;
	}

	public void run ()
	{
			String inLine;
			String inLine1;
 			String ref;
			String ref_update;
			String asked_ref;
			String asked_ref1;
			String profil;
			String email;
			String newtext;
			int n;

			try 
			{
				
				InputStreamReader isr = new InputStreamReader (client.getInputStream());
				BufferedReader is = new BufferedReader (isr);

				inLine = is.readLine();		
				asked_ref = inLine.substring(5,12);
				System.out.println ("Asked ref :" + asked_ref + ".");

				PrintWriter os = new PrintWriter (new BufferedOutputStream (client.getOutputStream()), false);
				String outline;
 				os.println ("<HTML><head><title>Recrutement</title></head><body>");
				os.println ("You requested :" + inLine + ".<p>");

				if (inLine.substring(0,4).equals("POST"))
				{
					try 
					{
						inLine1 = is.readLine(); 
						ref_update = inLine1.substring (inLine1.length()-7);

						os.println ("<p>");
						while (true)
						{
							inLine1 = is.readLine();
							os.println ("Data :" + inLine1 + ".<p>");
							System.out.println ("Data :" + inLine1 + ".");
							
							if (inLine1.length() > 6)
							if (inLine1.substring(0,6).equals("texte="))
							{
								newtext = inLine1.substring(6);
								os.println ("<p>Ref :" + ref_update + ".<p>");
								os.println ("<p>New text :" + newtext + ".<p>");
								System.out.println ("New text :" + newtext + ".");

				String data = "jdbc:odbc:postes";
	 			Class.forName ("sun.jdbc.odbc.JdbcOdbcDriver");

				Connection co = DriverManager.getConnection (data, "", "");
				Statement st = co.createStatement ();
 				
				int rowsUpdated = st.executeUpdate (
					"UPDATE Postes set Profil = '"
					+ newtext 
					+ "' where Reference = '"
					+ ref_update + "'");								
								
								break; 
							}
						}
					}
					catch (Exception e)
					{
					}					
				}

				else
				{
				os.println ("<ul>");

				String data = "jdbc:odbc:postes";
	 			Class.forName ("sun.jdbc.odbc.JdbcOdbcDriver");

				Connection co = DriverManager.getConnection (data, "", "");
				Statement st = co.createStatement ();

				if (asked_ref.equals ("UPDATE-"))
					asked_ref1 = inLine.substring (12, 19);
				else
					asked_ref1 = "";		
				
				ResultSet rec = st.executeQuery (
					"SELECT * FROM Postes");
				int i = 0;
				while (rec.next())
				{
					i++;
					ref = rec.getString("Reference");
					if (asked_ref.equals("-------")) 
						os.println ("<li><a href=" + ref + ">" + ref + "</a>");	
					else if (asked_ref.equals (ref))
					{
						profil = rec.getString ("Profil");
						os.println (profil);
						os.println ("<p>");
						email = rec.getString ("Email");
						os.println ("email : <a href=mailto:" + email + ">" + email + "</a>");
						os.println ("<p><a href=../UPDATE-" + ref + ">Update</a>");
  					}
					else if (asked_ref.equals ("UPDATE-") && asked_ref1.equals (ref))
					{
						profil = rec.getString ("Profil");
						os.println ("<form method=\"POST\" action=\"update\">");
						os.println ("Update the text :<p>");
						os.println ("<input name=\"texte\" size=60 maxlength=250 value=\"" + profil + "\"><p>");
						os.println ("<input type=\"submit\" value=\"Update\">");
						os.println ("</form>");
					}
 					n = i;
				}
				
				st.close ();
				}

				os.println ("</body></HTML>");
				os.flush();
				os.close();
				is.close();
				client.close (); 
 				/* sock.close (); */ 
         		}
			catch (Exception e)
			{
				System.err.println ("Error: " + e.getMessage());
			}				 
	}
}


