
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
 
/** Gestion des droits des utilisateurs */
public class Droits
{

        static String baseBDD = "droits";
	 
	static String fdl = "\"";

	/** Connexion à la base des droits */
      public static Connection connexion()
	{
		Connection con = null;
		try
		{
			Class.forName("sun.jdbc.odbc.JdbcOdbcDriver").newInstance();
                        con = DriverManager.getConnection("jdbc:odbc:" + baseBDD);
		}
		catch(Exception e)
		{
			JOptionPane.showMessageDialog(null, "connexion (1) : Erreur de connexion à la base de données " + baseBDD + " :\n" + e.getMessage(),
										  " " + "Erreur", JOptionPane.ERROR_MESSAGE);
			return null;
		}
		return con;
	}

	/** Vérifie si un utilisateur est autorisé à effectuer une action 
		@param login nom de l'utilisateur
		@param appelant numéro de fax appelant
		@param appele numéro de fax appelé
		@param action action à effectuer
		@param param paramètre de l'action
		@param options options de l'action
	*/
      public static boolean autorise (String login, String appelant, String appele, String action, String param, int options)
      {
		System.out.println ("autorisation pour " + login + " " + action + "...");

	   try
	   {
		Connection con = connexion();
		if (con != null)
		{
                        boolean aut = clientAutorise (con, login, appelant, appele, action, param, options);
                        con.close();
				System.out.println ("autorisation pour " + login + " " + action + " : " + aut);
                        return aut;
            }
			System.out.println ("autorisation pour " + login + " " + action + " : false (1)");

                return false;                
         }
	   catch (Exception e)
	   {
		System.out.println (e.getMessage());
		System.out.println ("autorisation pour " + login + " " + action + " : false (2)");
		return false;
	   }
      }
     
      public static boolean clientAutorise (Connection con, String login, String appelant, String appele, String action, String param, int options)
      throws java.sql.SQLException
      {
		/* Groupes auxquels appartient l'utilisateur */
		/*Statement s = con.createStatement();*/
		String requete = "SELECT * FROM appartenances WHERE " + fdl + "login" + fdl + " = '" + login + "'";
		System.out.println (requete);
		ResultSet rs = con.createStatement().executeQuery (requete);
		while (rs.next())
		{
			String groupe = rs.getString("groupe");
			System.out.println ("groupe " + groupe);
			if (groupeAutorise (con, groupe, appelant, appele, action, param, options))
				return true;
		}
		return false;
      }

        public static boolean groupeAutorise (Connection con, String groupe, String appelant, String appele, String action, String param, int options)
      throws java.sql.SQLException
	{
		System.out.println ("groupeAutorise " + groupe);
		if (groupe == null || groupe.equals(""))
			return false;
		int aut = groupeAutorise1 (con, groupe, appelant, appele, action, param, options);
		if (aut == 1)
			return true;
		if (aut == -1)
			return false;
		String requete = "SELECT * FROM groupes WHERE " + fdl + "sousgroupe" + fdl + " = '" + groupe + "'";
		System.out.println (requete);
		ResultSet rs = con.createStatement().executeQuery (requete);
		while (rs.next())
		{
			String groupe1 = rs.getString("groupe");
			if (groupeAutorise (con, groupe1, appelant, appele, action, param, options))
				return true;
		}
		return false;

	}

        public static int groupeAutorise1 (Connection con, String groupe, String appelant, String appele, String action, String param, int options)        
      throws java.sql.SQLException
	{
		System.out.println ("groupeAutorise1 " + groupe);
		String requete = "SELECT * FROM droits WHERE " + fdl + "groupe" + fdl + " = '" + groupe + "' AND " + fdl + "action" + fdl + " = '" + action + "'";
		System.out.println (requete);
		ResultSet rs = con.createStatement().executeQuery (requete);
		while (rs.next())
		{
			System.out.println ("trouve");
			String appelant1 = rs.getString("appelant");
			String appele1 = rs.getString("appele");
			String param1 = rs.getString("parametre");
			String operation = rs.getString("operation");
			System.out.println ("operation:" + operation);
			System.out.println ("appelant1=" + appelant1);
			if ((appelant1 == null || appelant1.equals("") || appelant1.equals(appelant)) &&
			    (appele1 == null || appele1.equals("") || appele1.equals(appele)) &&
			    (param1 == null || param1.equals("") || param1.equals(param)))
			{
				if (operation.equals("+"))
					return 1;
				else
					return -1;
			}
		}
		return 0;
	}

	/** Vérifie si un utilisateur a le droit d'administrer un groupe */
	public static boolean admin (String login, String groupe)
	{
		System.out.println ("autorisation admin pour " + login + " " + groupe + "...");

	   try
	   {
		Connection con = connexion();
		if (con != null)
		{
                        boolean aut = admin1 (con, login, groupe);
                        con.close();
				System.out.println ("autorisation admin pour " + login + " " + groupe + " : " + aut);
                        return aut;
            }
			System.out.println ("autorisation admin pour " + login + " " + groupe + " : false (1)");

                return false;                
         }
	   catch (Exception e)
	   {
		System.out.println (e.getMessage());
		System.out.println ("autorisation admin pour " + login + " " + groupe + " : false (2)");
		return false;
	   }
	} 

      public static boolean admin1 (Connection con, String login, String groupe)
	 throws java.sql.SQLException
	{
		boolean aut = clientAutorise (con, login, "", "", "ADMIN", groupe, 0);
		if (aut)
			return true;

		String requete = "SELECT * FROM groupes WHERE " + fdl + "sousgroupe" + fdl + " = '" + groupe + "'";
		System.out.println (requete);
		ResultSet rs = con.createStatement().executeQuery (requete);
		while (rs.next())
		{
			String groupe1 = rs.getString("groupe");
			if (admin1 (con, login, groupe1))
				return true;
		}
		return false;
	}

	/** Modification des droits */
	public static boolean modify (String op1, String operation, String action, String param, int options, String groupe, String appelant, String appele)
	{
	   try
	   {
		Connection con = connexion();
		if (con != null)
		{
				String requete;
				if (op1.equals("ADD"))
					requete = "INSERT INTO " + fdl + "droits" + fdl + " (" + fdl + "operation" + fdl + ", " + fdl + "action" + fdl + ", " + fdl + "parametre" + fdl + ", " + fdl + "options" + fdl + ", " + fdl + "groupe" + fdl + ", " + fdl + "appelant" + fdl + ", " + fdl + "appele" + fdl + ") VALUES ('" 
						+ operation 
						+ "', '" + action 
						+ "', '" + param 
						+ "', " + options 
						+ ", '" + groupe 
						+ "', '" + appelant 
						+ "', '" + appele + "')";
				else if (op1.equals("REMOVE"))
					requete = "DELETE FROM " + fdl + "droits" + fdl + " WHERE " + fdl + "operation" + fdl + " = '" + operation 
						+ "' AND " + fdl + "action" + fdl + " = '" + action
						+ "' AND " + fdl + "parametre" + fdl + " = '" + param 
						+ "' AND " + fdl + "options" + fdl + " = " + options
						+ " AND " + fdl + "groupe" + fdl + " = '" + groupe 
						+ "' AND " + fdl + "appelant" + fdl + " = '" + appelant
						+ "' AND " + fdl + "appele" + fdl + " = '" + appele + "'";
				else
					return false; 
				System.out.println (requete);

				con.setAutoCommit(false);
				Statement st = con.createStatement();
				st.executeUpdate (requete);
				con.commit();

                        con.close();
 				return true;
            }
 
                return false;                
         }
	   catch (Exception e)
	   {
		System.out.println (e.getMessage());
		System.out.println ("Erreur modif: " + e.getMessage());
		return false;
	   }

	}

	/** Modification d'un groupe */
	public static boolean modifyGroup (String op1, String groupe, String sousgroupe)
	{
	   try
	   {
		Connection con = connexion();
		if (con != null)
		{
				String requete;
				if (op1.equals("ADD"))
					requete = "INSERT INTO " + fdl + "groupes" + fdl + " (" + fdl + "groupe" + fdl + ", " + fdl + "sousgroupe" + fdl + ") VALUES ('"
						+ groupe + "', '" + sousgroupe + "')"; 
				else if (op1.equals("REMOVE"))
					requete = "DELETE FROM " + fdl + "groupes" + fdl + " WHERE " + fdl + "groupe" + fdl + " = '" + groupe + "' AND " + fdl + "sousgroupe" + fdl + " = '" + sousgroupe + "'";
				else
					return false; 
				System.out.println (requete);

				con.setAutoCommit(false);
				Statement st = con.createStatement();
				st.executeUpdate (requete);
				con.commit();

                        con.close();
 				return true;
            }
 
                return false;                
         }
	   catch (Exception e)
	   {
		System.out.println (e.getMessage());
		System.out.println ("Erreur modif: " + e.getMessage());
		return false;
	   }

	}

	/** Modification d'un utilisateur */
	public static boolean modifyClient (String op1, String login, String pass, float credits, float bloques, String groupe)
	{
	   try
	   {
		Connection con = connexion();
		if (con != null)
		{
				String requete;
				con.setAutoCommit(false);
				Statement st;
				if (op1.equals("ADD"))
				{
					requete = "SELECT * FROM " + fdl + "clients" + fdl + " WHERE " + fdl + "login" + fdl + " = '" + login + "'";
					System.out.println (requete);
					st = con.createStatement();
					ResultSet rs = st.executeQuery (requete);
					if (!rs.next())
					{
					/* S'il n'est pas déja dans clients on l'ajoute */
						requete = "INSERT INTO " + fdl + "clients" + fdl + " (" + fdl + "login" + fdl + ", " + fdl + "pass" + fdl + ", " + fdl + "credits" + fdl + ", " + fdl + "bloques" + fdl + ") VALUES ('"
                                                + login + "', '" + pass + "', " + credits + ", " + bloques + ")";
						System.out.println (requete);
						st = con.createStatement();
						st.executeUpdate (requete);
						con.commit();
					}

					requete = "INSERT INTO " + fdl + "appartenances" + fdl + " (" + fdl + "login" + fdl + ", " + fdl + "groupe" + fdl + ") VALUES ('" 
						+ login + "', '" + groupe + "')";
					System.out.println (requete);
					st = con.createStatement();
					st.executeUpdate (requete);
					con.commit();

				}
 				else if (op1.equals("REMOVE"))
				{
					requete = "DELETE FROM " + fdl + "appartenances" + fdl + " WHERE " + fdl + "login" + fdl + " = '" + login + "' AND " + fdl + "groupe" + fdl + " = '" + groupe + "'";
					System.out.println (requete);
					st = con.createStatement();
					st.executeUpdate (requete);
					con.commit();

					requete = "SELECT * FROM " + fdl + "appartenances" + fdl + " WHERE " + fdl + "login" + fdl + " = '" + login + "'";
;
					System.out.println (requete);
					st = con.createStatement();
					ResultSet rs = st.executeQuery (requete);
					if (!rs.next())
					{
					/* Si l'utilisateur n'est plus dans aucun groupe on supprime son compte */
						requete = "DELETE FROM " + fdl + "clients" + fdl + " WHERE " + fdl + "login" + fdl + " = '" + login + "'";
						System.out.println (requete);
						st = con.createStatement();
						st.executeUpdate (requete);
						con.commit();
					}
				}
				else
					return false; 
 
                        con.close();
 				return true;
            }
 
                return false;                
         }
	   catch (Exception e)
	   {
		System.out.println (e.getMessage());
		System.out.println ("Erreur modif: " + e.getMessage());
		return false;
	   }

	}




	public static void main (String[] args)
         {
            try
            {
		DataInputStream in=new DataInputStream(System.in); 
		String login, appelant, appele, action, param;
		int options = 0;

		System.out.print ("Login ? ");	login = in.readLine();
		System.out.print ("appelant ? ");	appelant = in.readLine();
		System.out.print ("appele ? ");	appele = in.readLine();
		System.out.print ("action ? ");	action = in.readLine();
                System.out.print ("parametre ? ");      param = in.readLine();
		
                boolean aut = autorise (login, appelant, appele, action, param, options);
		if (aut)
			System.out.println ("Autorise");
		else
			System.out.println ("Non autorise");

             }
             catch (Exception e) {}
	}

}

