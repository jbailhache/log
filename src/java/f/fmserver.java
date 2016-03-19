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

import com.java4less.rmail.*;
import com.java4less.rmime.*;


class F
{
	String field (String s)
	{
		return s; /* MySQL */
		/* return "\"" + s + "\""; */ /* Pervasive */
	}
}

public class fmserver extends JComponent
{
	/* F f = new F(); */
	/*
	String loginBDD = "faxmailer";
	String passwordBDD = "xvnrfk";
	*/

	String loginBDD = "";
	String passwordBDD = "";

	Vector fax_to_send = new Vector();

	String fdl = "\""; /* délimiteur de nom de champ, " pour Pervasive */

	/** fenetre principale */
	public JFrame mainWindow = null;
	private boolean quitte = false;
	private StyledDocument doc = new DefaultStyledDocument();
	private nJTextPane tp = new nJTextPane(doc);
	private JScrollPane sp = new JScrollPane(tp);
	private ServerSocket socket;
	/**
	 * nombre de transactions clients en cours
	 * ne JAMAIS modifier sa valeur autrement que par changeNumTransactions()
	 */
	public int numTransactions = 0;
	/** timeout client (en secondes) */
	public int timeout = /*5*/ 600; /* a faire */
	/** delai d attente d un paquet reseau (en millisecondes) */
	public int delai_attente = 100;
	/** taille d un bloc reseau (en octets) */
	public int bloc_reseau = 512;
	/** numero du port de connexion reseau */
	public int port_reseau = 6020;
	/** indique si le serveur est en mode de reception */
	public boolean running = false;
	/** repertoire de stockage des mailings */
	public String stockDir = new String("MAILING_CLIENTS");
	/**
	 * nom du DSN (Database Source Name) de la base contenant les
	 * informations client, a definir dans les sources de donnees (ODBC)
	 */
	public String baseBDD = "MySQL";
	/** nom de la table contenant les informations client */
	public String tableBDD = "clients";
	/** nom de la colonne contenant les logins client */
	public String colonneBDDlogin = "login";
	/** nom de la colonne contenant les mots de passe client */
	public String colonneBDDpass = "pwd";
	/** nom de la colonne contenant les credits client */
	public String colonneBDDcredits = "credit";
	/** nom de la colonne contenant les credits client bloques */
	public String colonneBDDcreditsbloques = "bloque";

	public String term = "";	

 
	
	/** nom du fichier de configuration */
	public String configFile = "fmjs.ini";

	/**
	 * le main appelle le constructeur
	 */
	public static void main(String args[])
	{
		System.out.println ("fmserver nouvelle version");
		fmserver sv = new fmserver();
    }

	/**
	 * Constructeur, initialise la fenetre principale
	 */
	public fmserver()
	{
		lireConfig();
		GraphicsEnvironment ge = GraphicsEnvironment.getLocalGraphicsEnvironment();
		GraphicsDevice[] gs = ge.getScreenDevices();
		GraphicsDevice gd = gs[0];
		GraphicsConfiguration gc = gd.getDefaultConfiguration();
		JFrame f = new JFrame("Serveur de connection - FAX-Mailer JAVA");
		f.setDefaultCloseOperation(WindowConstants.DO_NOTHING_ON_CLOSE);
		mainWindow = f;
		init_menus(f);
		f.getContentPane().setLayout(new BorderLayout());
		// on ajoute le bouton de fermeture
		f.addWindowListener(new wevt(this, f, true));
		// on impose une taille minimum
		f.setSize(640, 480);
		// on centre sur l'ecran
		f.setLocation(gc.getBounds().x + (gc.getBounds().width - f.getWidth())/2,
					  gc.getBounds().y + (gc.getBounds().height - f.getHeight())/2);

		tp.setEditable(false);
		tp.setBackground(Color.white);
		
		sp.setVerticalScrollBarPolicy(JScrollPane.VERTICAL_SCROLLBAR_ALWAYS);
		
		sp.setWheelScrollingEnabled(true);
		
		f.getContentPane().add(sp, BorderLayout.CENTER);
		f.show();
		int state = f.getExtendedState();
		 // Set the iconified bit 
		state |= Frame.ICONIFIED; 
		// Iconify the frame 
		f.setExtendedState(state); 

		try
		{
			int credits = 270000;
			String login = new String("TOTO");
			String pass = new String("test");
			Connection con = null;
			con = connexion();
			Statement s = con.createStatement();
			/* 	String requete = new String("UPDATE " + tableBDD + " SET \"" + colonneBDDcredits + "\" = " + credits + " WHERE \"" + colonneBDDlogin + "\"='" + login + "' AND \"" + colonneBDDpass + "\"='" + pass + "'"); */
			String requete = new String("UPDATE " + tableBDD + " SET " + fdl + colonneBDDcredits + fdl + " = " + credits + " WHERE " + fdl + colonneBDDlogin + fdl + "='" + login + "' AND " + fdl + colonneBDDpass + fdl + "='" + pass + "'");
			/* String requete = new String("UPDATE " + tableBDD + " SET " + f.field(colonneBDDcredits) + " = " + credits + " WHERE " + fdl + colonneBDDlogin + fdl + "='" + login + "' AND " + fdl + colonneBDDpass + fdl + "='" + pass + "'"); */


			s.executeUpdate(requete);
			requete = new String("UPDATE " + tableBDD + " SET " + fdl + colonneBDDcreditsbloques + fdl +  " = " + 0 + " WHERE " + fdl + colonneBDDlogin + fdl + "='" + login + "' AND " + fdl + colonneBDDpass + fdl + "='" + pass + "'");
			s.executeUpdate(requete);
			con.close();
		}
		catch (Exception e )
		{
			e.printStackTrace();
		}

		try
		{
			append("Mise en place du serveur...", Color.lightGray, true, true, false);
			socket = new ServerSocket(port_reseau);
			append("OK\nAttente de connexion...\n", Color.lightGray, true, true, false);
			running = true;
			for(;;)
			{
				// on n accepte plus de connexion si le serveur doit s arreter
				if(quitte)
					break;
				Socket sc = socket.accept();
			      append("OK\nConnexion acceptée\n", Color.lightGray, true, true, false);
				client cl = new client(this, sc);
				cl.start();
			}
		}
		catch(Exception e)
		{
			e.printStackTrace();
		}
	}

	/**
	 * Lit le fichier de configuration et le cree s'il n'existe pas.
	 */
	public void lireConfig()
	{
		String strtmp;
		File f = new File(configFile);
		// creer le fichier s il n existe pas
		fichierINI fi = new fichierINI(this, f);
		if(!f.exists())
		{
			fi.setParametre("Mailings", "BasePath", stockDir);
			fi.setParametre("Network", "Port", "" + port_reseau);
			fi.setParametre("Network", "BlockSize", "" + bloc_reseau);
			fi.setParametre("DataBase", "Name", baseBDD);
			fi.setParametre("DataBase", "Table", tableBDD);
			fi.setParametre("DataBase", "LoginColumn", colonneBDDlogin);
			fi.setParametre("DataBase", "PasswordColumn", colonneBDDpass);
			fi.setParametre("DataBase", "CreditsColumn", colonneBDDcredits);
			fi.setParametre("DataBase", "BlockedCreditsColumn", colonneBDDcreditsbloques);
		}
		// on suppose que tous les champs sont presents
		// si un champ manque pour une raison quelconque -> crash
		// todo: verifs de retours null sur getParametre()
		stockDir = new String(fi.getParametre("Mailings", "BasePath"));
		port_reseau = new Integer(fi.getParametre("Network", "Port")).intValue();
		bloc_reseau = new Integer(fi.getParametre("Network", "BlockSize")).intValue();
		baseBDD = new String(fi.getParametre("DataBase", "Name"));
		tableBDD = new String(fi.getParametre("DataBase", "Table"));
		colonneBDDlogin = new String(fi.getParametre("DataBase", "LoginColumn"));
		colonneBDDpass = new String(fi.getParametre("DataBase", "PasswordColumn"));
		colonneBDDcredits = new String(fi.getParametre("DataBase", "CreditsColumn"));
		colonneBDDcreditsbloques = new String(fi.getParametre("DataBase", "BlockedCreditsColumn"));
	}

	/**
	 * Affichage d'une dialogue permettant de changer le repertoire de stockage
	 * des mailings.
	 */
	public void dialogueMailings()
	{
		// initialisation et centrage du dialogue dans la fenetre principale
		JDialog dlg = new JDialog(mainWindow, " " + "Configuration des mailings", true);
		dlg.setSize(500, 150);
		dlg.setResizable(false);
		dlg.setLocation(mainWindow.getLocation().x + (mainWindow.getSize().width - dlg.getSize().width)/2,
						mainWindow.getLocation().y + (mainWindow.getSize().height - dlg.getSize().height)/2);
		dlg.getContentPane().setLayout(null);
		
		// label "emplacement des mailings"
		JLabel lbl1 = new JLabel("Emplacement des mailings" + " :");
		lbl1.setSize(340, 20);
		lbl1.setLocation(20, 20);
		dlg.getContentPane().add(lbl1);
		// champ "emplacement des mailings"
		JTextField tf1 = new JTextField(stockDir);
		tf1.setSize(340, 20);
		tf1.setLocation(20, 40);
		dlg.getContentPane().add(tf1);
		// bouton "parcourir"
		JButton bt1 = new JButton("Parcourir");
		Object[] cp1 = {tf1};
		bt1.addActionListener(new devt(this, dlg, "ParcourirEmplacementMailings", cp1));
		bt1.setSize(100, 26);
		bt1.setLocation(370, 37);
		dlg.getContentPane().add(bt1);

		// bouton "ok"
		JButton bt2 = new JButton("OK");
		bt2.addActionListener(new devt(this, dlg, "OKMailings", cp1));
		bt2.setSize(90, 26);
		bt2.setLocation((dlg.getSize().width - 220 - 6)/2, dlg.getSize().height - 68);
		dlg.getContentPane().add(bt2);
		// bouton "annuler"
		JButton bt3 = new JButton("Annuler");
		bt3.addActionListener(new devt(this, dlg, "Cancel", null));
		bt3.setSize(90, 26);
		bt3.setLocation((dlg.getSize().width - 220 - 6)/2 + 130, dlg.getSize().height - 68);
		dlg.getContentPane().add(bt3);

		dlg.show();
	}

	/**
	 * Affiche le dialogue permettant de configurer la connexion reseau.
	 */
	public void dialogueReseau()
	{
		// creation de la boite et centrage sur la fenetre principale
		JDialog dlg = new JDialog(mainWindow, " " + "Configuration réseau", true);
		dlg.setResizable(false);
		dlg.setSize(280, 160);
		dlg.setLocation(mainWindow.getLocation().x + (mainWindow.getSize().width - dlg.getSize().width)/2,
						mainWindow.getLocation().y + (mainWindow.getSize().height - dlg.getSize().height)/2);
		dlg.getContentPane().setLayout(null);

		// champs caches contenant les valeurs minimales et maximales du port
		JLabel hlbl1 = new JLabel("1000");
		JLabel hlbl2 = new JLabel("65000");
		// champs caches contenant les valeurs minimales et maximales de la taille des blocs
		JLabel hlbl3 = new JLabel("128");
		JLabel hlbl4 = new JLabel("8192");
		

		// label "port"
		JLabel lbl1 = new JLabel("port de connexion" + " :");
		lbl1.setLocation(20, 18);
		lbl1.setSize(140, 20);
		dlg.getContentPane().add(lbl1);
		// champ "port"
		JTextField tf1 = new JTextField("" + port_reseau);
		Object[] cp2 = {hlbl1, hlbl2, tf1};
		tf1.addActionListener(new devt(this, dlg, "ConfirmeInteger", cp2));
		tf1.setLocation(180, 18);
		tf1.setSize(70, 20);
		dlg.getContentPane().add(tf1);
		// label "taille des blocs"
		JLabel lbl2 = new JLabel("Taille des blocs réseau" + " :");
		lbl2.setLocation(20, 58);
		lbl2.setSize(160, 20);
		dlg.getContentPane().add(lbl2);
		// champ "taille des blocs"
		JTextField tf2 = new JTextField("" + bloc_reseau);
		Object[] cp3 = {hlbl3, hlbl4, tf2};
		tf2.addActionListener(new devt(this, dlg, "ConfirmeInteger", cp3));
		tf2.setLocation(180, 58);
		tf2.setSize(70, 20);
		dlg.getContentPane().add(tf2);

		// bouton "OK"
		JButton bt1 = new JButton("OK");
		Object[] cp1 = {tf1, tf2, hlbl1, hlbl2, hlbl3, hlbl4};
		bt1.addActionListener(new devt(this, dlg, "OKReseau", cp1));
		bt1.setLocation((dlg.getSize().width - 220 - 6)/2, dlg.getSize().height - 68);
		bt1.setSize(90, 26);
		dlg.getContentPane().add(bt1);
		// bouton "Annuler"
		JButton bt2 = new JButton("Annuler");
		bt2.addActionListener(new devt(this, dlg, "Cancel", null));
		bt2.setLocation((dlg.getSize().width - 220 - 6)/2 + 130, dlg.getSize().height - 68);
		bt2.setSize(90, 26);
		dlg.getContentPane().add(bt2);

		dlg.show();
	}

	/**
	 * Affiche la boite de dialogue de configuration de la BDD.
	 */
	public void dialogueBDD()
	{
		// on cree le dialogue et on centre dans la fenetre principale
		JDialog dlg = new JDialog(mainWindow, " " + "Configuration de la base de données", true);
		dlg.setResizable(false);
		dlg.setSize(480, 440);
		dlg.setLocation(mainWindow.getLocation().x + (mainWindow.getSize().width - dlg.getSize().width)/2,
						mainWindow.getLocation().y + (mainWindow.getSize().height - dlg.getSize().height)/2);
		dlg.getContentPane().setLayout(null);

		// label "source de donnees"
		JLabel lbl1 = new JLabel("Source de données" + " :");
		lbl1.setLocation(20, 20);
		lbl1.setSize(150, 20);
		dlg.getContentPane().add(lbl1);
		// champ "source de donnees"
		JTextField tf1 = new JTextField(baseBDD);
		tf1.setLocation(170, 20);
		tf1.setSize(150, 20);
		dlg.getContentPane().add(tf1);

		// label "table"
		JLabel lbl2 = new JLabel("Table contenant les informations client" + " :");
		lbl2.setLocation(20, 50);
		lbl2.setSize(250, 20);
		dlg.getContentPane().add(lbl2);
		// combobox "table"
		String[] l1 = {""};
		JComboBox cbb1 = new JComboBox(l1);
		cbb1.setLocation(300, 48);
		cbb1.setSize(150, 24);
		dlg.getContentPane().add(cbb1);

		// label "contenu"
		JLabel lbl3 = new JLabel("Contenu" + " :");
		lbl3.setLocation(20, 80);
		lbl3.setSize(250, 20);
		dlg.getContentPane().add(lbl3);
		// table "contenu"
		JTable table = new JTable();
		JTableHeader th = table.getTableHeader();
		th.setReorderingAllowed(false);
		table.setTableHeader(th);
		// on ne peut rien selectionner
		table.setSelectionMode(ListSelectionModel.SINGLE_SELECTION);
		table.setRowSelectionAllowed(false);
		table.setCellSelectionEnabled(false);
		// on insere le tableau dans un scrollpane
		JScrollPane scrollpane = new JScrollPane(table);
		// ajout tableau
		scrollpane.setSize(428, 100);
		scrollpane.setLocation(22, 100);
		dlg.getContentPane().add(scrollpane);

		// panel "champs"
		JPanel pane1 = new JPanel(null);
		pane1.setBorder(new TitledBorder(new EtchedBorder(), "Champs"));
		pane1.setLocation(20, 210);
		pane1.setSize(432, 140);
		// label "login client"
		JLabel lbl4 = new JLabel("Login client" + " :");
		lbl4.setLocation(32, 20);
		lbl4.setSize(150, 20);
		pane1.add(lbl4);
		// combobox "login client"
		String[] l2 = {""};
		JComboBox cbb2 = new JComboBox(l2);
		cbb2.setLocation(32, 40);
		cbb2.setSize(150, 24);
		pane1.add(cbb2);
		// label "mot de passe client"
		JLabel lbl5 = new JLabel("Mot de passe client" + " :");
		lbl5.setLocation(250, 20);
		lbl5.setSize(150, 20);
		pane1.add(lbl5);
		// combobox "mot de passe client"
		String[] l3 = {""};
		JComboBox cbb3 = new JComboBox(l3);
		cbb3.setLocation(250, 40);
		cbb3.setSize(150, 24);
		pane1.add(cbb3);
		// label "Credits client"
		JLabel lbl6 = new JLabel("Crédits client" + " :");
		lbl6.setLocation(32, 75);
		lbl6.setSize(150, 20);
		pane1.add(lbl6);
		// combobox "Credits client"
		String[] l4 = {""};
		JComboBox cbb4 = new JComboBox(l4);
		cbb4.setLocation(32, 95);
		cbb4.setSize(150, 24);
		pane1.add(cbb4);
		// label "Credits bloques client"
		JLabel lbl7 = new JLabel("Crédits bloqués client" + " :");
		lbl7.setLocation(250, 75);
		lbl7.setSize(150, 20);
		pane1.add(lbl7);
		// combobox "Credits bloques client"
		String[] l5 = {""};
		JComboBox cbb5 = new JComboBox(l5);
		cbb5.setLocation(250, 95);
		cbb5.setSize(150, 24);
		pane1.add(cbb5);
		// ajout du panel "champs"
		dlg.getContentPane().add(pane1);

		// on selectionne tous les elements deja definis
		if(!baseBDD.trim().equals(""))
		{
			if(testConnexion(baseBDD))
			{
				Connection conn = connexion();
				Vector tables = getDatabaseTables(conn, mainWindow);
				cbb1.removeAllItems();
				for(int i = 0; i < tables.size(); i++)
				{
					cbb1.addItem(tables.elementAt(i));
				}
				if(!tableBDD.trim().equals(""))
				{
					cbb1.setSelectedItem(tableBDD);
					if(cbb1.getSelectedItem().equals(tableBDD))
					{
						conn = connexion();
						fillTable(conn, tableBDD, table, cbb2, cbb3, cbb4, cbb5, mainWindow);
						cbb2.setSelectedItem(colonneBDDlogin);
						cbb3.setSelectedItem(colonneBDDpass);
						cbb4.setSelectedItem(colonneBDDcredits);
						cbb5.setSelectedItem(colonneBDDcreditsbloques);
					}
				}
			}
		}

		// bouton "appliquer"
		JButton bt1 = new JButton("Appliquer", new ImageIcon(getClass().getResource("images/refresh.gif")));
		Object[] cp1 = {tf1, cbb1, cbb2, cbb3, cbb4, cbb5, table};
		bt1.addActionListener(new devt(this, dlg, "AppliquerDSN", cp1));
		bt1.setLocation(330, 17);
		bt1.setSize(120, 26);
		dlg.getContentPane().add(bt1);

		cbb1.addActionListener(new devt(this, dlg, "ChangeTable", cp1));

		// bouton "OK"
		JButton bt2 = new JButton("OK");
		Component[] cp10 = {tf1, cbb1, cbb2, cbb3, cbb4, cbb5};
		bt2.addActionListener(new devt(this, dlg, "OKChoixBDD", cp10));
		bt2.setLocation((dlg.getSize().width - 220 - 6)/2, dlg.getSize().height - 68);
		bt2.setSize(100, 26);
		dlg.getContentPane().add(bt2);
		// bouton "Annuler"
		JButton bt3 = new JButton("Annuler");
		bt3.addActionListener(new devt(this, dlg, "Cancel", null));
		bt3.setLocation((dlg.getSize().width - 220 - 6)/2 + 130, dlg.getSize().height - 68);
		bt3.setSize(100, 26);
		dlg.getContentPane().add(bt3);

		dlg.show();
	}

	/**
	 * Permet d'etablir une connexion avec la base de donnees
	 */
	public Connection connexion()
	{		
		Connection con = null;
		try
		{
			Class.forName("sun.jdbc.odbc.JdbcOdbcDriver").newInstance();
			System.out.println ("Connexion a la base " + baseBDD);
			con = DriverManager.getConnection("jdbc:odbc:" + baseBDD, loginBDD, passwordBDD);
		}
		catch(Exception e)
		{
			JOptionPane.showMessageDialog(mainWindow, "connexion (1) : Erreur de connexion à la base de données " + baseBDD + " :\n" + e.getMessage(),
										  " " + "Erreur", JOptionPane.ERROR_MESSAGE);
			return null;
		}
		return con;
	}

	/**
	 * Permet d'etablir une connexion avec une base de donnees
	 */
	public Connection connexion(String dsn, Window wnd)
	{
		Connection con = null;
		try
		{
			Class.forName("sun.jdbc.odbc.JdbcOdbcDriver").newInstance();
			con = DriverManager.getConnection("jdbc:odbc:" + dsn, loginBDD, passwordBDD);
		}
		catch(Exception e)
		{
			JOptionPane.showMessageDialog(wnd, "connexion (2) : Erreur de connexion à la base de données" + " :\n" + e.getMessage(),
										  " " + "Erreur", JOptionPane.ERROR_MESSAGE);
			return null;
		}
		return con;
	}

	/**
	 * Permet de tester une connexion avec une base de donnees
	 */
	public boolean testConnexion(String dsn)
	{
		try
		{
			Class.forName("sun.jdbc.odbc.JdbcOdbcDriver").newInstance();
			Connection con = DriverManager.getConnection("jdbc:odbc:" + dsn, loginBDD, passwordBDD);
			con.close();
		}
		catch(Exception e)
		{
			return false;
		}
		return true;
	}

	public Vector getDatabaseTables(Connection conn, Window wnd)
	{
		Vector vect = new Vector();
		ResultSet rs = null;
		ResultSet rs2 = null;
		/*
		try
		{
			DatabaseMetaData dbmd = conn.getMetaData();
			rs = dbmd.getSchemas();
			while(rs.next()) 
			{
				String schema = rs.getString(1);
				rs2 = dbmd.getTables(null, schema, "%", null);
				while(rs2.next()) 
				{
					if(rs2.getString(4).equals("TABLE"))
						vect.addElement(rs2.getString(3));
				}
			}
		}
		catch(Exception e)
		{
			JOptionPane.showMessageDialog(wnd, "getDatabaseTables : Erreur de connexion à la base de données" + " :\n" + e.getMessage(),
										  " " + "Erreur", JOptionPane.ERROR_MESSAGE);
		}
		finally
		{
			try {rs.close(); rs2.close(); conn.close();}catch(Exception e) {}
		}*/
		/* la fonction getSchemas ne marche pas */
		vect.addElement("clients");
		 
		/**/
		return vect;
	}

	public Vector getTableFields(Connection conn, String table, Window wnd)
	{
		Vector vect = new Vector();
		ResultSet rs = null;
		try
		{
			Statement s = conn.createStatement();
			// on fait une requete qui ne renvoie aucune ligne, mais on a quand meme
			// acces aux informations de la table renvoiee
                        String requete = new String("SELECT * FROM " + table + " WHERE 1=2");
			rs = s.executeQuery(requete);
			int count = rs.getMetaData().getColumnCount();
			for(int i = 1; i <= count; i++)
			{
				vect.addElement(rs.getMetaData().getColumnName(i));
			}
		}
		catch(Exception e)
		{
			JOptionPane.showMessageDialog(wnd, "getTableFields : Erreur de connexion à la base de données" + " :\n" + e.getMessage(),
										  " " + "Erreur", JOptionPane.ERROR_MESSAGE);
		}
		finally
		{
			try {rs.close(); conn.close();}catch(Exception e) {}
		}
		return vect;
	}

	public void fillTable(Connection conn, String t, JTable table, JComboBox cbb1, JComboBox cbb2, JComboBox cbb3, JComboBox cbb4, Window wnd)
	{
		ResultSet rs = null;
		String requete = "";
		try
		{
			cbb1.removeAllItems();
			cbb2.removeAllItems();
			cbb3.removeAllItems();
			cbb4.removeAllItems();
			cbb1.setEnabled(false);
			cbb2.setEnabled(false);
			cbb3.setEnabled(false);
			cbb4.setEnabled(false);

			Statement s = conn.createStatement();
			// on lit toute la table
			/*requete = new String("SELECT * FROM " + t + ";");*/
			requete = new String("SELECT * FROM " + t + term);

			rs = s.executeQuery(requete);
			int count = rs.getMetaData().getColumnCount();
			Vector titles = new Vector();
			for(int i = 1; i <= count; i++)
			{
				titles.addElement(rs.getMetaData().getColumnName(i));
			}
			Vector rows = new Vector();
			while(rs.next())
			{
				Vector vrow = new Vector();
				for(int i = 1; i <= count; i++)
				{
					vrow.addElement(rs.getString(i));
				}
				String[] row = new String[vrow.size()];
				for(int i = 0; i < vrow.size(); i++)
				{
					row[i] = (String)vrow.elementAt(i);
				}
				rows.addElement(row);
			}
			table.setModel(new nTableModel(this, titles, rows));
			// on change le mode de scroll par defaut s il y a beaucoup de colonnes,
			// sinon la table est illisible
			if(titles.size() > 10)
				table.setAutoResizeMode(JTable.AUTO_RESIZE_OFF);
			else
				table.setAutoResizeMode(JTable.AUTO_RESIZE_SUBSEQUENT_COLUMNS);

			// on met les listes de colonnes a jour
			for(int i = 0; i < titles.size(); i++)
			{
				cbb1.addItem(titles.elementAt(i));
				cbb2.addItem(titles.elementAt(i));
				cbb3.addItem(titles.elementAt(i));
				cbb4.addItem(titles.elementAt(i));
			}
			if(titles.size() > 0)
			{
				cbb1.setEnabled(true);
				cbb2.setEnabled(true);
				cbb3.setEnabled(true);
				cbb4.setEnabled(true);
			}
		}
		catch(Exception e)
		{
			JOptionPane.showMessageDialog(wnd, "fillTable : Erreur de connexion à la base de données : " + requete + " :\n" + e.getMessage(),
										  " " + "Erreur", JOptionPane.ERROR_MESSAGE);
		}
		finally
		{
			try {rs.close(); conn.close();}catch(Exception e) {}
		}
	}

	/**
	 * sortie du programme
	 */
	public void quitter(boolean confirm)
	{
		quitte = true;
		if(confirm)
		{
			int rep = JOptionPane.showConfirmDialog(mainWindow, "Si vous quitter ce programme, FAX-Mailer JAVA ne\npourra plus fonctionner.\nEtes-vous sur de vouloir quitter?",
										  "Attention", JOptionPane.YES_NO_OPTION, JOptionPane.WARNING_MESSAGE);
			if(rep != JOptionPane.OK_OPTION)
				return;
		}
		// on lance le thread d attente de finb des transactions
		fin q = new fin(this);
		q.run();
	}
	
	public synchronized void changeNumTransactions(int n)
	{
		numTransactions += n;
		if(quitte && numTransactions != 0 && n < 0)
			append("Attente de fermeture des transactions (" + numTransactions + " en cours) ...", Color.gray, true, true, false);
	}
	public synchronized void append(String str, Color color, boolean bold, boolean italic, boolean underline)
	{
		MutableAttributeSet attr = new SimpleAttributeSet();
		StyleConstants.setBold (attr, bold);
		StyleConstants.setItalic (attr, italic);
		StyleConstants.setUnderline (attr, underline);
		StyleConstants.setForeground(attr, color);
		StyleConstants.setFontFamily (attr, "Monospaced");
		try
		{
			doc.insertString(doc.getLength(), str, attr);
		}
		catch(Exception e)
		{
			e.printStackTrace();
		}
		if(!sp.getVerticalScrollBar().getValueIsAdjusting())
			tp.setCaretPosition(doc.getLength());
	}
	
	public void init_menus(JFrame f)
	{
		// definition de la barre de menus
		JMenuBar mb = new JMenuBar();
		// menu "fichier"
		JMenu m_fichier = new JMenu("Fichier");
		// item "quitter"
                JMenuItem mi_fichier1;
                try { 
                        mi_fichier1 = new JMenuItem("Quitter", new ImageIcon(getClass().getResource("images/blank.gif")));
                } catch (Exception exc) {
                        mi_fichier1 = new JMenuItem("Quitter");
                }
		mi_fichier1.addActionListener(new evt(this, "Quitter"));
		// ajout des items
		m_fichier.add(mi_fichier1);
		// ajout du menu dans la barre
		mb.add(m_fichier);

		// menu "options"
		JMenu m_options = new JMenu("Options");
		// item "Mailings"
                JMenuItem mi_options1;
                try {
                        mi_options1 = new JMenuItem("Mailings", new ImageIcon(getClass().getResource("images/folder.gif")));
                } catch (Exception exc) {
                        mi_options1 = new JMenuItem ("Mailings");
                }

		mi_options1.addActionListener(new evt(this, "DialogueMailings"));
		// item "Reseau"               
                JMenuItem mi_options2;
                try {
                        mi_options2 = new JMenuItem("Réseau", new ImageIcon(getClass().getResource("images/network.gif")));
                } catch (Exception exc) {
                        mi_options2 = new JMenuItem("Réseau");
                }
		mi_options2.addActionListener(new evt(this, "DialogueReseau"));
		// item "Base de données"
                JMenuItem mi_options3;
                try {
                        mi_options3 = new JMenuItem("Base de données", new ImageIcon(getClass().getResource("images/database.gif")));
                } catch (Exception exc) {
                        mi_options3 = new JMenuItem("Base de données");
                }
		mi_options3.addActionListener(new evt(this, "DialogueBDD"));
		// ajout des items
		m_options.add(mi_options1);
		m_options.add(mi_options2);
		m_options.add(mi_options3);
		// ajout du menu dans la barre
		mb.add(m_options);

		f.setJMenuBar(mb);
	}
}

/**
 * sous classe de JTextPane pour supprimer le linewrap
 */
class nJTextPane extends JTextPane
{
	public nJTextPane(StyledDocument doc)
	{
		super(doc);
	}
	
	public boolean getScrollableTracksViewportWidth()
	{
		return false;
	}
	
	public void setSize(Dimension d)
	{
		if(d.width < getParent().getSize().width)
			d.width = getParent().getSize().width;

		super.setSize(d);
	}
}

/**
 * gestionnaire d evenements de fenetres
 */
class wevt extends WindowAdapter
{
	private Window wnd = null;
	private boolean exit = false;
	private fmserver sv = null;
	
	public wevt(fmserver psv, Window pWnd, boolean pexit)
	{
		wnd = pWnd;
		exit = pexit;
		sv = psv;
	}
	
	public void windowClosing(WindowEvent e)
	{
		if(exit)
			sv.quitter(true);
		else
		{
			wnd.dispose();
			wnd = null;
		}
	}
}

/**
 * thread client
 */
class client extends Thread
{
	String fdl = "\""; /* délimiteur de nom de champ, " pour Pervasive */


	private Socket sc = null;
	private fmserver sv = null;
	public String term = "";
      public String dirfax, faxdir, basefax, basedroits, baseresend, _login, _password;

	 

        public String loginBDD = "faxmailer";
        public String passwordBDD = "xvnrfk";
	
	/*Vector fax_to_send = new Vector();*/

	Droits droits;

	public client(fmserver psv, Socket psc)
	{
		sv = psv;
		sc = psc;
		baseresend = "RESEND";
	}

	public int verif_login(String base, String login, String pass)
	{
		System.out.println ("verif_login " + login);
		int state = 0;
		Connection conn = null;
		try
		{
			System.out.println ("connexion a " + sv.baseBDD);
			conn = sv.connexion();
			Statement s = conn.createStatement();
			/*String requete = new String("SELECT * FROM " + sv.tableBDD + " WHERE " + sv.colonneBDDlogin + "='" + login + "' AND " + sv.colonneBDDpass + "='" + pass + "'" + term);*/
			/*String requete = new String("SELECT * FROM " + sv.tableBDD + " WHERE [" + sv.colonneBDDlogin + "]='" + login + "' AND [" + sv.colonneBDDpass + "]='" + pass + "'" + term);*/
			/* String requete = new String("SELECT * FROM " + sv.tableBDD + " WHERE \"" + sv.colonneBDDlogin + "\"='" + login + "' AND \"" + sv.colonneBDDpass + "\"='" + pass + "'" + term); */
			String requete = new String("SELECT * FROM " + sv.tableBDD + " WHERE " + fdl + sv.colonneBDDlogin + fdl + "='" + login + "' AND " + fdl + sv.colonneBDDpass + fdl + "='" + pass + "'" + term);
			/*String requete = new String("SELECT * FROM " + sv.tableBDD + " WHERE "  + fdl + sv.colonneBDDbase + fdl + " = '" + base + "' AND " + fdl + sv.colonneBDDlogin + fdl + "='" + login + "' AND " + fdl + sv.colonneBDDpass + fdl + "='" + pass + "'" + term);*/

			System.out.println (requete);
 
			ResultSet rs = s.executeQuery(requete);
			if(rs.next())
			{
				state = 1;
				_login = login;
				_password = pass;
			}
			System.out.println("state="+state);
		}
		catch(Exception e)
		{
			sv.append("*** " + "Erreur de connexion à la base de données! (1) - " + e.getMessage() + " ***\n", Color.red, true, false, false);
			state = 2;
			e.printStackTrace();
		}
		finally
		{
			try {conn.close();}catch(Exception e) {}
		}
		/* return 1; */ /* !!! test à enlever */
		return state;
	}
	public int verif_credits(String login, String pass, int credits)
	{
		int state = credits;
		Connection conn = null;
		try
		{
			conn = sv.connexion();
			Statement s = conn.createStatement();
			String requete = new String("SELECT " + "*" /* fdl + sv.colonneBDDcredits + fdl*/ + " FROM " + sv.tableBDD + " WHERE " + fdl + sv.colonneBDDlogin + fdl + "='" + login + "' AND " + fdl + sv.colonneBDDpass + fdl + "='" + pass + "'" /* "';"*/);
			ResultSet rs = s.executeQuery(requete);
			if(rs.next())
			{
				state = credits - rs.getInt(sv.colonneBDDcredits);
				if(state < 0)
					state = 0;
			}
		}
		catch(Exception e)
		{
			sv.append("*** " + "Erreur de connexion à la base de données! (2) - " + e.getMessage() + " ***\n", Color.red, true, false, false);
			e.printStackTrace();
		}
		finally
		{
			try {conn.close();}catch(Exception e) {}
		}
		return state;
	}
	public int update_credits(String login, String pass, int credits)
	{
		int ret = -1;
		Connection conn = null;
		try
		{
			conn = sv.connexion();
			Statement s = conn.createStatement();
			// on recupere les credits en cours
                        String requete = new String("SELECT " + "*" /*sv.colonneBDDcredits*/ + " FROM " + sv.tableBDD + " WHERE " + fdl + sv.colonneBDDlogin + fdl + "='" + login + "' AND " + fdl + sv.colonneBDDpass + fdl + "='" + pass + "'" /* "';"*/);
			ResultSet rs = s.executeQuery(requete);
			int state = 0;
			if(rs.next())
				state = rs.getInt(sv.colonneBDDcredits);
			else
				return ret;
			ret = state - credits;
			// on met a jour les credits en cours (on retranche credits)
                        requete = new String("UPDATE " + sv.tableBDD + " SET " + fdl + sv.colonneBDDcredits + fdl + " = " + (state - credits) + " WHERE " + fdl + sv.colonneBDDlogin + fdl + "='" + login + "' AND " + fdl + sv.colonneBDDpass + fdl + "='" + pass + "'" /*"';"*/);
			s.executeUpdate(requete);
			// on recupere les credits bloques
			requete = new String("SELECT " + "*" /*sv.colonneBDDcreditsbloques*/ + " FROM " + sv.tableBDD + " WHERE " + fdl + sv.colonneBDDlogin + fdl + "='" + login + "' AND " + fdl + sv.colonneBDDpass + fdl + "='" + pass + "'" /*"';"*/);
			rs = s.executeQuery(requete);
			state = 0;
			rs.next();
			state = rs.getInt(sv.colonneBDDcreditsbloques);
			// on met a jour les credits bloques (on ajoute credits)
                        requete = new String("UPDATE " + sv.tableBDD + " SET " + fdl + sv.colonneBDDcreditsbloques + fdl + " = " + (state + credits) + " WHERE " + fdl + sv.colonneBDDlogin + fdl + "='" + login + "' AND " + fdl + sv.colonneBDDpass + fdl + "='" + pass + "'");
			s.executeUpdate(requete);
			/*
			sv.append("bloque : " + (state + credits) + "\n", Color.red, true, true, true);
                        requete = new String("SELECT " + "*"  sv.colonneBDDcredits + " FROM " + sv.tableBDD + " WHERE " + sv.colonneBDDlogin + "='" + login + "' AND " + sv.colonneBDDpass + "='" + pass + "'");
			rs = s.executeQuery(requete);
			if(rs.next())
				ret = rs.getInt(sv.colonneBDDcredits);
			*/
		}
		catch(Exception e)
		{
			sv.append("*** " + "Erreur de connexion à la base de données! (3) - " + e.getMessage() + " ***\n", Color.red, true, false, false);
			e.printStackTrace();
		}
		finally
		{
			try {conn.close();}catch(Exception e) {}
		}
		return ret;
	}
	public boolean client_timeout(InputStream is, int msec)
	{		
		int time = 0;
		try
		{
			while(is.available() == 0)
			{
				sleep(sv.delai_attente);
//				sv.append("Attente...\n", Color.darkGray, false, false, false);
				time += sv.delai_attente;
				if(time > msec)
				{
					sv.append("Timeout!\n", new Color(150, 0, 0), false, false, false);
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
	
	public String recoit_param(InputStream is, String filter)
	{		
		String str = new String("");
		boolean erreur = false;
		int count = 0;
		try
		{
			for(;;)
			{
				if(client_timeout(is, sv.timeout*1000))
				{
					erreur = true;
					break;
				}
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
	
	public boolean vide_buffer(InputStream is)
	{		
		try
		{
			byte[] tmp = new byte[10240];
			int i;
			while(is.available() != 0)
				i = is.read(tmp, 0, tmp.length);
		}
		catch(Exception e)
		{
			e.printStackTrace();
		}
		return false;
	}

	public String getTimeString()
	{
		Calendar c = Calendar.getInstance();
		int day = c.get(Calendar.DAY_OF_MONTH);
		int month = c.get(Calendar.MONTH) + 1;
		int year = c.get(Calendar.YEAR);
		int hour = c.get(Calendar.HOUR_OF_DAY);
		int minute = c.get(Calendar.MINUTE);
		int second = c.get(Calendar.SECOND);
		return new String((day < 10 ? "0" : "") + day + (month < 10 ? "/0" : "/") + month + "/" + year + " " + (hour < 10 ? "0" : "") + hour + (minute < 10 ? ":0" : ":") + minute + (second < 10 ? ":0" : ":") + second);
	}

	/*
	String fieldnames[] = { "ID", "FILE", "CNX", "DCNX", "TIME", "PGENB", "HGN", "REM", "READ", "RTR", "LINE", "SPEED", "NOISE", "SIGNALS", "SIGNALQ", "PRINTED", "OCR", "SAVED", "FRS_REF", "FRS_SAV", "FRS_OCR", "FRS_EXP", "FRS_PRT" }; 
	*/
	
	/*
	String fieldnames[] = { "ID", "FILE", "CNX", "PGENB", "SUBJECT", "IMPORTANCE" };
	*/
	
	String fieldnames[] = { "???" };

	int index (String s)
	{
		/*
		for (int i=0; i<fieldnames.length; i++)
		{
			if (fieldnames[i].equals(s))
				return i;
		}*/
		for (int i=0; i<droits.nDBFields; i++)
		{
			if (fieldnames[i+droits.firstDBField].equals(s))
				return i;
		}

		return 0;
	}

	public String[] getFields (String reponse)
	{

							String[] field = new String[fieldnames.length-1];
							GenericMessage m = new GenericMessage(reponse);
							System.out.println ("Message: " + m.toString());							
							for (int j=0; j<fieldnames.length-1; j++)
							{
								field[j] = (String)m.values.elementAt(j+2);
								System.out.print ("" + j + ":" + field[j] + "\t");
							}
							System.out.println ("");
							return field;
	}

	public void run()
	{
 		Object[][] tabfaxrec = new Object[1000][fieldnames.length];
		
		sv.changeNumTransactions(1);
		try
		{
			String client = new String(sc.getInetAddress().getHostName());
			sv.append(getTimeString() + "  " + "Connexion client" + " : \"" + client + "\" (" + sc.getInetAddress().getHostAddress() + ")\n", Color.blue, true, false, false);
			InputStream is = sc.getInputStream();
			OutputStream os = sc.getOutputStream();
			String commande = new String("");
			int bloc_transmission = 512;
			boolean erreur = false;
			String fileName = new String("");
			String directory = new String("Error");
			String base = new String("");
			String login = new String("");
			String pass = new String("");
			int credits = 0;
			boolean auth = /*true*/ false;
			boolean recu = false;
			for(;;)
			{
				erreur = false;
				// attente maximum entre chaque commande
				if(client_timeout(is, sv.timeout*1000))
				{
					erreur = true;
					break;
				}
				commande = new String(recoit_param(is, "alpha"));
				if(commande.equals(""))
				{
					erreur = true;
					break;
				}
				else if(commande.equals("\n"))
				   continue;
				else if (commande.equals("BASEFAX"))
				{
				   System.out.println (commande);
				   try
				   {
					dirfax = recoit_param(is,"any");
					System.out.println ("dirfax=" + dirfax);

					String language = recoit_param (is, "any");
					System.out.println ("T01");
					if (!dirfax.equals(""))
					{	
					System.out.println ("T02");
					File f = new File (dirfax + "\\baserights.txt");
					System.out.println("T1a");
					FileInputStream is1 = new FileInputStream (f);
					System.out.println("T2");					
					basedroits = "";
					System.out.println("T3");
					int c;
					System.out.println("T4");
					for (;;)
					{
						c = is1.read();
						System.out.println ("c=" + c);
						if (c == 13 ||c == -1)
							break;
						basedroits = basedroits + (char)c;
					}
					System.out.println("T5");
					is1.close();
					System.out.println ("basedroits=" + basedroits);
					sv.baseBDD = basedroits;

					f = new File (dirfax + "\\basefax.txt");
					is1 = new FileInputStream (f);
					basefax = "";
			 
					for (;;)
					{
						c = is1.read();
						
						if (c == 13 ||c == -1)
							break;
						basefax = basefax + (char)c;
					}
					is1.close();
					System.out.println ("basefax=" + basefax);

					/*
					f = new File (dirfax + "\\baseresend.txt");
					is1 = new FileInputStream (f);
					*/
					baseresend = "";
				 	/*
					for (;;)
					{
						c = is1.read();
						if (c == 13 ||c == -1)
							break;
						baseresend = baseresend + (char)c;
					}
					is1.close();
					System.out.println ("baseresend=" + baseresend);
					*/
						
					System.out.println();
					System.out.print(" T6 ");
					try
					{
					System.out.print(" T7 ");
					f = new File (dirfax + "\\faxdir.txt");
					System.out.print(" T8 ");
					is1 = new FileInputStream (f);
					System.out.print(" T9 ");
					faxdir = "";
					System.out.print(" T10 "); 
					for (;;)
					{
						c = is1.read();
						if (c == 13 ||c == -1)
							break;
						faxdir = faxdir + (char)c;
					}
					System.out.print(" T11 ");
					is1.close();
					System.out.print(" T12 ");
					}
					catch (Exception e)
					{
						faxdir = dirfax;
					}
					System.out.println ("faxdir=" + faxdir);


					droits = new Droits (dirfax);
					
					System.out.println ("droits cree");

					fieldnames = droits.getFieldNames();
					System.out.print(" T13 ");
					int[] fieldwidths = droits.getFieldWidths();
					System.out.print(" T14 ");
					String[] fielddescs = droits.getFieldDescs(language);
					System.out.print(" T15 ");
					int n = fieldnames.length;
					System.out.print(" T16 ");
					String field;
					System.out.println ("Sending fields");
					for (int i=0; i<n; i++)
					{
						field = fieldnames[i] + "\t" + fieldwidths[i] + "\t" + fielddescs[i] + "\n";
						os.write (field.getBytes("US-ASCII"));
						os.flush ();
					}
					System.out.println ("Sending empty line");
					field = "\n";
					os.write (field.getBytes("US-ASCII"));
					os.flush ();
					System.out.println("end");

				      }
				   }
				   catch (Exception e)
				   {
					e.printStackTrace();   
					System.out.println (e.toString());
				   }
				
				}
				else if(commande.equals("USER"))
				{
					login = new String(recoit_param(is, "any"));
					if(login.equals(""))
					{
						erreur = true;
						break;
					}
					directory = new String(login);
					sv.append(client + " : " + "USER " + login + ".\n", Color.black, true, false, false);
					System.out.println ("***USER " + login);
				}
				else if(commande.equals("PASS"))
				{
					pass = new String(recoit_param(is, "any"));
					if(pass.equals(""))
					{
						erreur = true;
						break;
					}
					String strtmp = new String("");
					for(int i = 0; i < pass.length(); i++)
					{
						strtmp = strtmp + "*";
					}
					sv.append(client + " : " + "PASS " + strtmp + "\n", Color.black, true, false, false);
					System.out.println ("PASS " + strtmp);
					// on verifie le couple login+password (directory+pass)
					System.out.println ("verif login " + directory);
					int result = verif_login(base, directory, pass);
					if(result == 1)
					{
						auth = true;
						sv.append(client + " : " + "Utilisateur authentifié" +" : \"" + directory + "\".\n", new Color(0, 150, 0), false, false, false);
					}
					else
					{
						auth = false;
						sv.append(client + " : " + "Login ou mot de passe incorrect!" + "\n", new Color(150, 0, 0), false, false, false);
					}
					// on renvoie si le login a reussi
					String com = new String("LOGIN\n" + result + "\n");
					System.out.println (com);
					os.write(com.getBytes("US-ASCII"));
					os.flush();
				}
				else if(commande.equals("SIZE"))
				{
					String size = new String(recoit_param(is, "num"));
					if(size.equals(""))
					{
						erreur = true;
						break;
					}
					credits = new Integer(size).intValue();
					
					int credits_manquants = verif_credits(login, pass, credits);

					sv.append(client + " : " + "Demande des crédits d'envoi" + " (" +  NumberFormat.getInstance().format(credits) + " demandés" + " - " + (credits_manquants != 0 ? "crédits manquants : " +  NumberFormat.getInstance().format(credits_manquants) : "ok") + ").\n", Color.black, false, false, false);
					// on renvoie les credits manquants
					String com = new String("SIZE\n" + credits_manquants + "\n");
					os.write(com.getBytes("US-ASCII"));
					os.flush();
				}
				else if(commande.equals("NAME"))
				{
					String name = new String(recoit_param(is, "any"));
					if(name.equals(""))
					{
						erreur = true;
						break;
					}
					File ftmp = new File(sv.stockDir + File.separatorChar + directory + File.separatorChar + name.substring(0, name.lastIndexOf('.')));
					int sp = 0;
					if(ftmp.exists())
						sp = 1;
					sv.append(client + " : " + "Demande d'existence du mailing (" + (sp == 0 ? "ok" : "existe déjà") + " - " + ftmp.getAbsolutePath() + ")\n", Color.black, false, false, false);
					// on renvoie la vitesse de transmission
					String com = new String("NAME\n" + sp + "\n");
					os.write(com.getBytes("US-ASCII"));
					os.flush();
				}
				else if(commande.equals("SPEED"))
				{
					String speed = new String(recoit_param(is, "num"));
					if(speed.equals(""))
					{
						erreur = true;
						break;
					}
					int sp = new Integer(speed).intValue();
					if(sp > sv.bloc_reseau)
						sp = sv.bloc_reseau;
					bloc_transmission = sp;
					sv.append(client + " : " + "Taille des blocs = " + NumberFormat.getInstance().format(sp) + " octets.\n", Color.black, true, false, false);
					// on renvoie la vitesse de transmission
					String com = new String("SPEED\n" + sp + "\n");
					os.write(com.getBytes("US-ASCII"));
					os.flush();
				}
				else if(commande.equals("TRANSMIT"))
				{
					String name = new String(recoit_param(is, "any"));
					if(name.equals(""))
					{
						erreur = true;
						break;
					}
					String size = new String(recoit_param(is, "num"));
					if(size.equals(""))
					{
						erreur = true;
						break;
					}
					if(!auth)
					{
						sv.append(client + " : " + "Transmission non autorisée!" + "\n", new Color(150, 0, 0), false, false, false);
						break;
					}
					File dir = new File(sv.stockDir);
					dir.mkdir();
					directory = new String(sv.stockDir + File.separatorChar + directory);
					dir = new File(directory);
					dir.mkdir();
					File fic = new File(directory + File.separator + name);
					if(new File(fic.getParent()).exists())
						sv.append(client + " : " + "Ecrasement du mailing" + " (\"" + name + "\", " + NumberFormat.getInstance().format(new Integer(size)) + " " + "octets" + ").\n", Color.black, false, false, false);
					else
						sv.append(client + " : " + "Reception du mailing" + " (\"" + name + "\", " + NumberFormat.getInstance().format(new Integer(size)) + " " + "octets" + ").\n", Color.black, false, false, false);
					fileName = new String(fic.getPath());
					System.out.println ("Mailing filename : " + fileName);
					FileOutputStream fo = new FileOutputStream(fic);
					int count = new Integer(size).intValue();
					byte[] buf = new byte[bloc_transmission];
					for(;;)
					{
						int i;
						if(client_timeout(is, sv.timeout*1000))
						{
							erreur = true;
							break;
						}
						if(count < buf.length)
							i = is.read(buf, 0, count);
						else
							i = is.read(buf, 0, buf.length);
						fo.write(buf, 0, i);
						count -= i;
						if(count == 0)
							break;
					}
					if(!erreur)
					{
						sv.append(client + " : " + "mailing recu" + ".\n", new Color(0, 100, 0), false, false, false);
						recu = true;
					}
					else
						sv.append(client + " : " + "Erreur à la réception du mailing" + ".\n", new Color(150, 0, 0), false, false, false);
					fo.close();
				}
				else if(commande.equals("END"))
					break;
				else if (commande.equals("LISTREC"))
				/* Le client demande la liste des fax reçus */
				{
					sv.append (client + "Demande de la liste des fax reçus", Color.black, false, false, false); /* à vérifier */
					String listrec = new String ("");
					String statustr = "OK";
					/* mettre dans listrec les données concernant les fax reçus */

					String param = new String (recoit_param(is,"any"));
					System.out.println ("param:" + param);
					
					String strsince = recoit_param (is, "any");
					System.out.println ("since:" + strsince);

					java.util.Date since = null;
					if (!strsince.equals(""))
						since = Util.dateStr(strsince);

					Origin origin = new Origin ();
					/*boolean aut = Droits.autorise (_login, origin, "LIST", "", 0);*/
					boolean aut = true;

					if (!auth || !aut)
					{
						String com = "NOTAUTH\n";
						os.write(com.getBytes("US-ASCII"));
						os.flush();

					}
					else
					{
        				Connection con = null;
	        			ResultSet rs = null;
					String query = "";
        				try
        				{
				                Class.forName ("sun.jdbc.odbc.JdbcOdbcDriver").newInstance();
						    System.out.println ("basefax=" + basefax);
						    						
				                con = DriverManager.getConnection ("jdbc:odbc:" + basefax, loginBDD, passwordBDD);
 
						    /*
						    query = "ALTER TABLE REC ADD \"FOLDER\" VARCHAR(254)";
						    con.setAutoCommit(false);
						    Statement stmt1 = con.createStatement();
						    stmt1.executeUpdate (query);
					          con.commit();
						    */

 				                query = "SELECT * FROM " + "REC" + " " + param;
						    System.out.println (query);
				                ResultSet results;
                            
				                Statement stmt = con.createStatement();
				                results = stmt.executeQuery (query);
                					
						    int n = 0;

					String com = new String("LISTREC\n" + statustr + "\n");
					System.out.println (com);
					/*sv.append (client + "Liste des fax reçus: " + com, Color.black, false, false, false);  */

					os.write(com.getBytes("US-ASCII"));
					os.flush();

						    java.util.Date lastfax = null;
						    System.out.println ("Reading base...");
				                while (results.next())
				                {
							/*System.out.print ("fax read ");*/
							String[] field = new String[fieldnames.length];
                                          for (int i=0; i<fieldnames.length-1; i++)
							{
                                                                /*System.out.print("[" + i + "]");*/
                                                                try {field[i] = results.getString(i+1);}
                                                                catch (Exception e)
                                                                { field[i] = "????"; }
                                                                /*System.out.print(field[i]);*/
							}
							/*System.out.println (field[0]);*/

							/*
							String name = results.getString("FILE");
							System.out.println (name);
							*/
							/*boolean aut1 = Droits.autorise(con,_login,name,"LIST","",0);*/
							
							String s_rtr = field[/*9*/index("RTR")] /*results.getString("RTR")*/;
							Double X;
                        		      try { X = new Double(s_rtr); }
		                              catch (Exception e) { X = new Double(-1); }
							int rtr = (int)(X.doubleValue());
		
							String s_line = field[/*10*/index("LINE")] /*results.getString("LINE")*/;
                        		      try { X = new Double(s_line); }
		                              catch (Exception e) { X = new Double(-1); }
							int line = (int)(X.doubleValue());
							/*System.out.println ("s_line="+s_line+" line="+line);*/
		
							String id = field[/*0*/index("ID")] /*results.getString("ID")*/;
							String called = ""; /* provisoirement */
							String calling = "";

							boolean er = false;
							java.util.Date datefax = null;
							try
							{
								datefax = Util.dateStr (field[3]);
								/*if (datefax.after(lastfax))
									lastfax = datefax;*/
							}
							catch (Exception e1)
							{
								er = true;
							}
							boolean ok = false;
							if (since == null) ok = true;
							else if (er) ok = false;
							else
							{ ok = /*datefax.after(since)*/
								(datefax.getTime()-since.getTime()>-900000)
							 	&& !datefax.toString().equals(since.toString());
							  // System.out.println ("datefax=" + datefax + " since=" + since + " after=" + ok);
							}
							/* System.out.println ("strsince=" + strsince 
								+ " since=" + since
								+ " er=" + er 
								+ " field[3]=" + field[3]
								+ " datefax=" + datefax								 
								+ " ok=" + ok); */
							if (ok)
							/* 
							if (!er)
							if (since == null || datefax.after(since))							
							*/
							{
							boolean aut1 = droits.autorise (_login, new Origin (rtr, line, id, called, calling), "LIST", "", 0);

							/*System.out.println ("aut1=" + aut1);*/
							if (aut1)
							{
								/*
				                        listrec = field[0];
								for (int i=2; i<=23; i++)
									listrec += "\t" + field[i-1];
								listrec += "\n";
								*/
								GenericMessage m = new GenericMessage();
								m.add ("MEDIA", "Fax");
								m.add ("BASE", basefax);

								int i;
								

								for (i=0; i<fieldnames.length-1; i++)
								{
									if (field[i] != null)
									{
									if (!field[i].equals("????"))
										m.add (fieldnames[i+1], field[i]);
									}
									else
										m.add (fieldnames[i+1], field[i]);
								}

								/*
								try
								{
									int wa = 60;
									int ha = 100;
									String filename1 = field[index("FILE")];								
									String filename = faxdir + "\\" + filename1 + ".000";
 									RandomAccessFile raf = new RandomAccessFile (filename, "r");
									Vector datavect = new Vector();
									int[] data = new int[5000000];
									int nbytes = 0;

									while (true)
									{
										byte b;
										try
										{
											b = raf.readByte();
										}
										catch (Exception e)
										{
											break;
										}	
										// datavect.addElement(new Byte(b));
										data[nbytes++] = (int)b;
										
									}

									TiffViewer tv = new TiffViewer (data, nbytes);
									tv.convert();
									// tv.createImage (Color.white, Color.black, 60, 100);
									String apercu = "";
									int x, y;
									
									for (y=0; y<ha; y++)
									{
									for (x=0; x<wa; x++)
									{
										if (tv.bitmap[y*tv.imageHeight/ha][x*tv.imageWidth/wa])
											apercu += "1";
										else
											apercu += "0";  
										
									}

									}
									m.add ("APERCU", apercu);


								}
								catch (Exception e)
								{
									e.printStackTrace();
								}
								System.out.println(m.toString());
								*/

							Connection con1 = null;
							try
							{
								Class.forName("sun.jdbc.odbc.JdbcOdbcDriver").newInstance();
                  					con1 = DriverManager.getConnection("jdbc:odbc:fax");

								String requete = 
									"SELECT * FROM fax WHERE \"FILE\" = '" 
									+ m.getField("FILE") + "'";
								// System.out.println(requete);

								ResultSet rs1 = con1.createStatement().executeQuery (requete);
								String apercu = "";
								if (rs1.next())
								{
									apercu = rs1.getString("APERCU");
								}

								m.add ("APERCU", apercu);
								m.add ("EXT", "");

				                        con1.close();

							}
							catch (Exception eCon)
							{
								eCon.printStackTrace();
							}


								
								listrec = m.toString() + "\n";
								
							
					com = listrec;
					/*System.out.println (com);*/
					/*sv.append (client + "Liste des fax reçus: " + com, Color.black, false, false, false); */

					os.write(com.getBytes("US-ASCII"));
					os.flush();

								/*
								for (int i=0; i<23; i++)
									tabfaxrec[n][i] = results.getString(i+1);
								*/
								n++;
							}
							}
				                }
						/*listrec += "\n";*/
					com = "\n";
					System.out.println (com);
					/*sv.append (client + "Liste des fax reçus: " + com, Color.black, false, false, false); */

					os.write(com.getBytes("US-ASCII"));
					os.flush();

				        }
				        catch (Exception e)
				        {
						sv.append (client + "Error: " + e.getMessage() , Color.black, false, false, false); /* à vérifier */				                 
						System.out.println ("Error LISTREC: " + e.toString());
						e.printStackTrace();
						listrec = "\n";
						statustr = query + " : " + e.getMessage();
				        }

					/*String com = new String("LISTREC\n" + statustr + "\n" + listrec + "\n");  */
					/*com = "\n";
					System.out.println (com);*/
					/*sv.append (client + "Liste des fax reçus: " + com, Color.black, false, false, false); */
					/*
					os.write(com.getBytes("US-ASCII"));
					os.flush();*/
					}

				}
				else if (commande.equals ("THUMBNAIL"))
				{
					System.out.println (commande);
					String name = recoit_param (is, "any");

					// GenericMessage m = new GenericMessage ();

					System.out.println ("Creating thumbnail...");
					FaxMailerFunctions.create_thumbnail (name, faxdir);
					System.out.println ("... thumbnail created");

					String com = "OK\n";
					os.write (com.getBytes("US-ASCII"));
					os.flush();

				}

				else if (commande.equals ("FAXDATA"))
				/* Le client demande les données d'un fax */
				{
					System.out.println ("FAXDATA");
					sv.append (client + "Demande des données d'un fax", Color.black, false, false, false); /* à vérifier */
					String name = new String(recoit_param(is, "any"));
                                        System.out.println ("login:" + _login + " name:" + name);
					sv.append (client + "Nom du fichier: " + name, Color.black, false, false, false); /* à vérifier */

					boolean aut = droits.autorise (_login, name, /*new Origin(),*/ "SHOW", "", 0);
					System.out.println ("aut(" + _login + "," + name + ",...)=" + aut);
					if (!aut)
					{
						String com = "NOTAUTH\n";
						os.write(com.getBytes("US-ASCII"));
						os.flush();

					}
					else
					{

					/* String com = new String ("FAXDATA\n<...données du fax...>\n"); */
					/* pour tester; à faire : récupérer les données dans le fichier */		
					
					for (int np=0; ; np++)
					{
					String com = new String ("FAXDATA\n");
					/* A faire : répertoire */
 
					String filename = new String (/*"C:\\fmw\\SendTmp\\"*/ faxdir + "\\" + name + ".00" + new Integer(np).toString());   /* send ou sendtmp en local */ 
				 
					/* String filename = new String ("D:\\ATWEB1\\FAXMAILER\\fmw\\SendTmp\\" + name + ".00" + new Integer(np).toString());  */  /* sur le serveur */


					/* pour tester : */ /* filename = "img1.TIF"; if (np >= 3) break; */
					System.out.println (filename);
                                        RandomAccessFile raf;
                                        try
                                        {
                                                raf = new RandomAccessFile (filename, "r");
                                        }
                                        catch (Exception e)
                                        {
                                                System.out.println ("Fin des fichiers");
                                                break;
                                        }
					byte b;
					int n = 0;
					/*String data = new String("");*/
					/* byte[] data = new byte[60000]; */
					Vector datavect = new Vector();
					try
					{
						while (true)
						{
							b = raf.readByte();
							/*
							System.out.print(b);
							System.out.print (" ");
							*/
							/*data += (char)b;*/
							/*data[n] = b;*/
							datavect.addElement(new Byte(b));
							n++;
						}
					}
					catch (EOFException e)
					{
						/* normal, on ne fait rien */
					}
					catch (Exception e)
					{
						System.out.println ("Autre exception");
						System.out.println (e.toString());
						break;
					}			
					byte[] data = new byte[n];
					/*datavect.copyInto (data);*/
					for (int k=0; k<n; k++)
						data[k] = ((Byte)(datavect.elementAt(k))).byteValue();
					System.out.println ("Fin du fichier");
					System.out.println ("0:" + com);					
					com += new Integer(np).toString();
					System.out.println ("1:" + com + ".");
					com += "\n";
					System.out.println ("2:" + com);
					com += new Integer(n).toString();
					System.out.println ("3:" + com);
					com += "\n";
					System.out.println ("4:" + com);
					os.write(com.getBytes("US-ASCII"));
					System.out.println ("send:" + com);
					os.write (data);
					com = new String("\n");
					os.write (com.getBytes("US-ASCII"));
					}
					String com = new String ("ENDFAXDATA\n");
					System.out.println (com);
					os.write(com.getBytes("US-ASCII"));
				 
					
					os.flush();
					}
				}
				else if (commande.equals("REFAX"))
				{
					System.out.println ("REFAX");
					String numfax = recoit_param (is, "any");
					System.out.println ("numfax=" + numfax);

					String s_retry = recoit_param (is, "any");
					Integer I1;
					try { I1 = new Integer(s_retry); }
					catch (Exception e) { I1 = new Integer(3); }
					int retry = I1.intValue();
					System.out.println ("retry=" + retry);					

					String s_pri = recoit_param (is, "any");
					try { I1 = new Integer(s_pri); }
					catch (Exception e) { I1 = new Integer(9); }
					int pri = I1.intValue();
					System.out.println ("pri=" + pri);

					String s_delai = recoit_param (is, "any");
					try { I1 = new Integer(s_delai); }
					catch (Exception e) { I1 = new Integer(120); }
					int delay = I1.intValue();					
					System.out.println ("delay=" + delay);

					String id = recoit_param (is, "any");
					System.out.println ("id=" + id);

					/* String name = recoit_param (is, "any"); */

					String strnames = recoit_param (is, "any");
					System.out.println ("filenames: " + strnames);
					String[] names = strnames.split("\t");
			  		
					for (int i=0; i<names.length; i++)
					{
					String name = names[i];

					System.out.println ("numfax=" + numfax 
						+ " retry=" + retry 
						+ " pri=" + pri 
						+ " delay=" + delay
						+ " id=" + id
						+ " filename=" + name);
		
 
					/*
					String s_rtr = new String(recoit_param(is,"any"));
					Integer I1;
                              try { I1 = new Integer(s_rtr); }
                              catch (Exception e) { I1 = new Integer(-1); }
					int rtr = I1.intValue();

					String s_line = new String(recoit_param(is,"any"));
                              try { I1 = new Integer(s_line); }
                              catch (Exception e) { I1 = new Integer(-1); }
					int line = I1.intValue();

					String id = new String(recoit_param(is,"any"));
					String appelant = new String(recoit_param(is, "any"));
					String appele = new String(recoit_param(is, "any"));
					*/

					try
					{
						/*
						Connection con = null;										

				            Class.forName ("sun.jdbc.odbc.JdbcOdbcDriver").newInstance();
				            con = DriverManager.getConnection ("jdbc:odbc:" + basefax, loginBDD, passwordBDD);
					
 				            String query = "SELECT * FROM REC WHERE " + fdl + "FILE" + fdl + " = '" + name + "'";

				            ResultSet results;
                            
				            Statement stmt = con.createStatement();
				            results = stmt.executeQuery (query);
                					
						int n = 0;

				            if (!results.next())
				            {
							String com = "Error: fax not found";
							os.write (com.getBytes("US-ASCII"));
							os.flush();
						}
						else
						{				                   
							
							String s_rtr = results.getString("RTR");
							Double X;
                        		      try { X = new Double(s_rtr); }
		                              catch (Exception e) { X = new Double(-1); }
							int rtr = (int)(X.doubleValue());
		
							String s_line = results.getString("LINE");
                        		      try { X = new Integer(s_line); }
		                              catch (Exception e) { X = new Double(-1); }
							int line = (int)(X.doubleValue());
		
							String id = results.getString("ID");
							String appelant = "";  
							String appele = "";
						

							boolean aut = droits.autorise (_login, new Origin (rtr, line, id, calling, called), "REFAX", numfax, 0);
						*/

							boolean aut = droits.autorise (_login, name, "FAX", numfax, 0);
                                                              
							if (!aut)
							{
								String com = "NOTAUTH\n";
								os.write (com.getBytes("US-ASCII"));
								os.flush();
							}
							else
							{
						/*** initialisation ***/

								Fax f = new Fax();
								f.basefax = basefax;
								f.baseresend = baseresend;
								f.faxdir = faxdir;
								f.numfax = numfax;
								f.retry = retry;
								f.pri = pri;
								f.delay = delay;
								f.id = id;
								



						/*** fin initialisation ***/
						
								/* ajouter les données */
 
						for (int np=0; ; np++)
						{
 
							/* A faire : répertoire */
 
							String filename = new String (/*"C:\\fmw\\SendTmp\\"*/ faxdir + "\\" + name + ".00" + new Integer(np).toString());   /* send ou sendtmp en local */ 
							/* String filename = new String ("D:\\ATWEB1\\FAXMAILER\\fmw\\SendTmp\\" + name + ".00" + new Integer(np).toString());  */  /* sur le serveur */

							/* pour tester : */ /* filename = "img1.TIF"; if (np >= 3) break; */
							System.out.println (filename);
                                          RandomAccessFile raf;
                                          try
                                          {
                                                raf = new RandomAccessFile (filename, "r");
                                          }
                                          catch (Exception e)
                                          {
                                                System.out.println ("Fin des fichiers");
                                                break;
                                          }
							byte b;
					            int n = 0;
 
							Vector datavect = new Vector();
							try
							{
								while (true)
								{
									b = raf.readByte();
		 							datavect.addElement(new Byte(b));
									n++;
								}
							}
							catch (EOFException e)
							{
								/* normal, on ne fait rien */
							}
							catch (Exception e)
							{
								System.out.println ("Autre exception");
								System.out.println (e.toString());
								break;
							}			

							byte[] data = new byte[n];
		 					for (int k=0; k<n; k++)
								data[k] = ((Byte)(datavect.elementAt(k))).byteValue();
							System.out.println ("Fin du fichier");
 
 							/*** traitement des données d'une page dans data ***/
 							f.addPage(data);


						}
										 
						/*** traitement final ***/
 								/*String status = f.send();*/
								f.d = sv.fax_to_send.size();
								String rlst = f.send(false);
								System.out.println ("add fax to send, lst=" + f.lst + "," + rlst);
								f.lst = rlst;
								System.out.println ("add fax to send, lst=" + f.lst + "," + rlst);
								sv.fax_to_send.add(f);	
								System.out.println ("added, " + sv.fax_to_send.size() + " faxes to send");
								/*String com = status + "\n";*/
								String com = "OK\n";
								os.write (com.getBytes("US-ASCII"));
								os.flush();
							}
						/*}*/
					}
					catch (Exception e)
					{
						String com = "Error refax: " + e.getMessage();
						os.write (com.getBytes("US-ASCII"));
						os.flush();
					}
					}
					
				}

				else if (commande.equals("MAIL"))
				{
 
					/*String name = recoit_param (is, "any");	*/

					String strnames = recoit_param (is, "any");
					System.out.println ("filenames: " + strnames);
					String[] names = strnames.split("\t");

					String smtp = recoit_param (is, "any");
					String from = recoit_param (is, "any");				
					String[] to = recoit_param (is, "any") . split(";");
					String subject = recoit_param (is, "any");
					String text = recoit_param (is, "any").replace('$','\n');								
  
					for (int j=0; j<names.length; j++)
					{
					String name = names[j];
					System.out.println ("filename: " + name);
					System.out.println ("text: " + text);

					try
					{

				         boolean aut = droits.autorise (_login, name, /*new Origin(),*/ "SHOW", "", 0);

					   if (!aut)
					   {
						String com = "NOTAUTH\n";
						os.write(com.getBytes("US-ASCII"));
						os.flush();

					   }
					   else
					   {
						System.out.println ("autorise");
						/*** initialisation ***/

						MailMsg m = new MailMsg();
						System.out.println ("Mail created");
						m.smtpServer = smtp; /*"smtp.noos.fr";*/ /* a faire : parametrage */
						m.from = from;
						for (int i=0; i<to.length; i++)
						{
							if (droits.autorise (_login, name, "EMAIL", to[i], 0))    
							{                                     
								m.addRecipient(to[i]);
								System.out.println ("" + to[i] + " added");
							}
						}
						m.subject = subject;
						MailMsgPart part = new MailMsgPart();
        					part.setData (text, MimeEncoder.QUOTED);
	        				m.addPart(part);
						System.out.println ("text added");
						MailMsgPart at;

						/*** fin initialisation ***/

	CustomPrinter cp = new CustomPrinter();
	cp.port = "";
	cp.print_option = 3;

	FaxImage fi = new FaxImage();
	fi.curpage = 0;
	// fi.nbrpages = npages;
	fi.pages = new Vector();
	fi.sizes = new Vector();

						for (int np=0; ; np++)
						{
 							fi.nbrpages = np;
							/* A faire : répertoire */
 
							String filename = new String (/*"C:\\fmw\\SendTmp\\"*/ faxdir + "\\" + name + ".00" + new Integer(np).toString());   /* send ou sendtmp en local */ 
							/* String filename = new String ("D:\\ATWEB1\\FAXMAILER\\fmw\\SendTmp\\" + name + ".00" + new Integer(np).toString());  */  /* sur le serveur */

							/* pour tester : */ /* filename = "img1.TIF"; if (np >= 3) break; */
							System.out.println (filename);
                                          RandomAccessFile raf;
                                          try
                                          {
                                                raf = new RandomAccessFile (filename, "r");
                                          }
                                          catch (Exception e)
                                          {
                                                System.out.println ("Fin des fichiers");
                                                break;
                                          }
							byte b;
					            int n = 0;
 
							Vector datavect = new Vector();
							try
							{
								while (true)
								{
									b = raf.readByte();
		 							datavect.addElement(new Byte(b));
									n++;
								}
							}
							catch (EOFException e)
							{
								/* normal, on ne fait rien */
							}
							catch (Exception e)
							{
								System.out.println ("Autre exception");
								System.out.println (e.toString());
								break;
							}			

							byte[] data = new byte[n];
		 					for (int k=0; k<n; k++)
								data[k] = ((Byte)(datavect.elementAt(k))).byteValue();
							System.out.println ("Fin du fichier");
 
 							/*** traitement des données d'une page dans data ***/
								/*at.setData (strdata, MimeEncoder.BASE64);*/

								// ci-dessous envoi fichier TIFF
								/*
								at = new MailMsgPart();
								at.ContentType = "Application";
								at.ContentSubType = "Octet-stream";
								at.FileName = "page" + (np+1) + ".tif";
								at.Name = "page" + (np+1) + ".tif";
								at.setBinaryData (FaxMailerFunctions.stdTiffBytes(data));
								m.addPart (at);
								*/
				
								// ci-dessous envoi fichier PS
								
		byte[] bytes = data;
		int size = bytes.length;
		fi.sizes.add(new Integer(size));
                int[] page = new int[size];
		for (int k=0; k<size; k++)
			page[k] = (int)bytes[k];
		fi.pages.add (page);		




						}
										 
						/*** traitement final ***/

						// ci-dessous envoi fichier PS

								cp.faximage = fi;
								cp.print();

	ByteArrayOutputStream os1 = new ByteArrayOutputStream();
	GZIPOutputStream gzos = new GZIPOutputStream (os1);
	gzos.write (cp.postscript.getBytes("US-ASCII"));
	gzos.close();
	byte[] out = os1.toByteArray();
	os.close();

								at = new MailMsgPart();
								at.ContentType = "Application";
								at.ContentSubType = "Octet-stream";
								at.FileName = "fax.ps.gz";
								at.Name = "fax.ps.gz";

								at.setBinaryData (out);
								m.addPart (at);

						// fin envoi PS

						System.out.println ("Envoi email");
						m.mail();
						System.out.println ("envoye");

						String com = "OK\n";
						os.write (com.getBytes("US-ASCII"));
						os.flush();
	                           }
					}
					catch (Exception e)
					{
						String com = "Error send email: " + e.getMessage();
						os.write (com.getBytes("US-ASCII"));
						os.flush();
					}
					}
					System.out.println ("Fin des fax à envoyer par email");
				}


				else if (commande.equals("DELETE"))
				{
					/*String name = recoit_param (is, "any");*/

					String strnames = recoit_param (is, "any");
					System.out.println ("filenames: " + strnames);
					String[] names = strnames.split("\t");					 
  
					for (int j=0; j<names.length; j++)
					{
					String name = names[j];
					System.out.println ("filename: " + name);
					 




					boolean aut = droits.autorise (_login, name, /*new Origin(),*/ "DELETE", "", 0);

					if (!aut)
					{
						String com = "NOTAUTH\n";
						os.write(com.getBytes("US-ASCII"));
						os.flush();

					}
					else
					{
						System.out.println ("Suppression du fax " + name);

						String com;
						try
						{

						    String filename;

				                Class.forName ("sun.jdbc.odbc.JdbcOdbcDriver").newInstance();
				                Connection con = DriverManager.getConnection ("jdbc:odbc:" + basefax, loginBDD, passwordBDD);
														
						    int nbrpages = 0;

						    String query;
		
						    query = "SELECT * FROM " + "REC" + " WHERE " + fdl + "FILE" + fdl + " = '" + name + "'";

				                ResultSet results;
                            
				                Statement stmt = con.createStatement();
				                results = stmt.executeQuery (query);
                					
						    int n = 0;

				                if (results.next())
				                {
							nbrpages = results.getInt ("PGENB");
						    }

						    for (int np=0; np<nbrpages; np++)
						    {
						    	filename = new String (/*"C:\\fmw\\SendTmp\\"*/ faxdir + "\\"  + name + ".00" + new Integer(np).toString());   /* send ou sendtmp en local */ 
							System.out.println (filename);
							/* filename = new String ("D:\\ATWEB1\\FAXMAILER\\fmw\\SendTmp\\" + name + ".00" + new Integer(np).toString());  */  /* sur le serveur */

						   	try
							{
								File f = new File (filename);
								f.delete();
							}
							catch (Exception e1)
							{
								break;
							}
						    }


 				                query = "DELETE * FROM REC WHERE " + fdl + "FILE" + fdl + " = '" + name + "'";;
						    System.out.println (query);
				                 
						    con.setAutoCommit(false);
				                stmt = con.createStatement();
				                stmt.executeUpdate (query);
						    con.commit();
						    con.close();
						
						    com = "DELETE\n";
						    os.write (com.getBytes("US-ASCII"));
						    os.flush();
						}
						catch (Exception e)
						{
							com = e.getMessage() + "\n";
							os.write(com.getBytes("US-ASCII"));
							os.flush();
						}
						}

					}

				}
				else if (commande.equals("GETRESEND"))
				{
				   /*System.out.println ("GETRESEND");*/
				   try
				   {
					/*
					System.out.println ("baseresend=" + baseresend);
					String baseBDD = baseresend;
 
					Connection con = null;
					Class.forName("sun.jdbc.odbc.JdbcOdbcDriver").newInstance();
            		      con = DriverManager.getConnection("jdbc:odbc:" + baseBDD);

					int ref = 0;
			
					String com = "GETRESEND\n";

					String requete = "SELECT * FROM " + "SEND";
					System.out.println (requete);
					ResultSet rs = con.createStatement().executeQuery (requete);
					while (rs.next())
					{
						String fax1 = rs.getString("FAX");
						String lst1 = rs.getString("LST");
						int retry1 = rs.getInt("RETRY");
						int pgnb1 = rs.getInt("PGNB");
						int pri1 = rs.getInt("PRI");
						int hash1 = rs.getInt("HASH");
						int delay1 = rs.getInt("DELAY1");
						String flid1 = rs.getString("FLID");
					*/

					String com = "GETRESEND\n";
					/* System.out.println ("" + sv.fax_to_send.size() + " fax a envoyer"); */
					for (int i=0; i<sv.fax_to_send.size(); i++)
					{
						Fax f = (Fax)(sv.fax_to_send.elementAt(i));
						String fax1 = f.numfax;
						String lst1 = f.lst;
						int retry1 = f.retry;
						int pgnb1 = f.pgnb;
						int pri1 = f.pri;
						int hash1 = f.hash;
						int delay1 = f.delay;
						String flid1 = f.id;

						String com1 = fax1 + "\t"  
							+ lst1 + "\t" + retry1 + "\t" + pgnb1 + "\t"
							+ pri1 + "\t" + hash1 + "\t" 
							+ delay1 + "\t" + flid1 + "\n";

						System.out.println ("fax numero " + i + " : " + com1);

						com += com1;

					}
					sv.fax_to_send.removeAllElements();
					com += "\n";

					/*System.out.println ("GETRESEND: " + com);*/

				    	os.write (com.getBytes("US-ASCII"));
					os.flush();

					/*
					requete = "DELETE FROM " + fdl + "SEND" + fdl;

					con.setAutoCommit(false);
				      Statement stmt = con.createStatement();
				      stmt.executeUpdate (requete);
					con.commit();

					con.close();
					*/
				   }
				   catch (Exception e1)
				   {
					System.out.println (e1.toString());
				   }
 
				}
				else if (commande.equals("RIGHT"))
				{
					String op1 = new String(recoit_param(is, "any"));
					String op2 = new String(recoit_param(is, "any"));
					String action = new String(recoit_param(is, "any"));
					String param = new String(recoit_param(is, "any"));
					String options = new String(recoit_param(is, "any"));
                                        String base1 = new String(recoit_param(is,"any"));
					String groupe = new String(recoit_param(is, "any"));

					/*
					String s_rtr = new String(recoit_param(is,"any"));
					Integer I1;
                              try { I1 = new Integer(s_rtr); }
                              catch (Exception e) { I1 = new Integer(-1); }
					int rtr = I1.intValue();

					String s_line = new String(recoit_param(is,"any"));
                              try { I1 = new Integer(s_line); }
                              catch (Exception e) { I1 = new Integer(-1); }
					int line = I1.intValue();
					*/
		
					String s_rtr = new String(recoit_param(is,"any"));
					Double X;
                              try { X = new Double(s_rtr); }
                              catch (Exception e) { X = new Double(-1); }
					int rtr = (int)(X.doubleValue());

					String s_line = new String(recoit_param(is,"any"));
                              try { X = new Double(s_line); }
                              catch (Exception e) { X = new Double(-1); }
					int line = (int)(X.doubleValue());


					String id = new String(recoit_param(is,"any"));
					String appelant = new String(recoit_param(is, "any"));
					String appele = new String(recoit_param(is, "any"));

					Integer I;
                              try { I = new Integer(options); }
                              catch (Exception e) { I = new Integer(0); }

					System.out.println ("RIGHT " + op1 + " " + op2 + " " + action + " " + param + " " + options + " " + groupe + " " + appelant + " " + appele);

					/* On vérifie si le client est autorisé à administrer le groupe */
					boolean aut1 = droits.admin (_login, groupe);
								
					/* Dans le cas d'un ajout de droit, on vérifie si le client a lui-même ce droit */
					boolean aut2;
					if (op1.equals("ADD") && op2.equals("+"))
						aut2 = droits.autorise (_login, new Origin (rtr, line, id, appelant, appele), action, param, I.intValue());
					else
						aut2 = true;

					System.out.println ("Autorisation admin: " + aut1 + " " + aut2);


					if (!(aut1 && aut2))
					{
						String com = "NOTAUTH\n";
						os.write(com.getBytes("US-ASCII"));
						os.flush();
					}
					else
					{
						/* ajouter ou enlever le droit */

                                                
                                                boolean status = droits.modify (op1, op2, action, param, I.intValue(), base1, groupe, new Origin (rtr, line, id, appelant, appele)); 

						String com;
						if (status)
							com = "OK\n";
						else
							com = "ERROR\n";
						os.write(com.getBytes("US-ASCII"));	
						os.flush();
					}
					System.out.println ("fin admin");
				}

				else if (commande.equals("GROUP"))
				{
					String op1 = new String(recoit_param(is, "any"));
					String groupe = new String(recoit_param(is, "any"));
					String sgroupe = new String(recoit_param(is, "any"));
 
					System.out.println ("GROUP " + op1 + " " + sgroupe + " " + groupe);
 
					boolean aut = droits.admin (_login, groupe);
					
					System.out.println ("Autorisation admin: " + aut);

					if (!aut)
					{
						String com = "NOTAUTH\n";
						os.write(com.getBytes("US-ASCII"));
						os.flush();
					}
					else
					{
						/* ajouter ou enlever le groupe */
 
						boolean status = droits.modifyGroup (op1, groupe, sgroupe);

						String com;
						if (status)
							com = "OK\n";
						else
							com = "ERROR\n";
						os.write(com.getBytes("US-ASCII"));	
						os.flush();
					}
					System.out.println ("fin admin");
				}


				else if (commande.equals("CLIENT"))
				{
					String op1 = new String(recoit_param(is, "any"));
					String login1 = new String(recoit_param(is, "any"));
					String pass1 = new String(recoit_param(is, "any"));
					String scredits1 = new String(recoit_param(is, "any"));
					String sbloques1 = new String(recoit_param(is, "any"));
                                        String groupe1 = new String(recoit_param(is, "any"));

					float credits1, bloques1;
					try { credits1 = (new Float(scredits1)).floatValue(); }
						catch (Exception e) { credits1 = 0; }
					try { bloques1 = (new Float(sbloques1)).floatValue(); }
						catch (Exception e) { bloques1 = 0; }

                                        System.out.println ("CLIENT " + op1 + " " + login + " " + credits1 + " " + bloques1 + " " + groupe1);
 
					boolean aut = droits.admin (_login, groupe1);
					
					System.out.println ("Autorisation admin: " + aut);

					if (!aut)
					{
						String com = "NOTAUTH\n";
						os.write(com.getBytes("US-ASCII"));
						os.flush();
					}
					else
					{
						/* ajouter ou enlever le groupe */
 
                                                boolean status = droits.modifyClient (op1, login1, pass1, credits1, bloques1, groupe1);

						String com;
						if (status)
							com = "OK\n";
						else
							com = "ERROR\n";
						os.write(com.getBytes("US-ASCII"));	
						os.flush();
					}
					System.out.println ("fin admin");
				}

				else if (commande.equals("LISTGROUPS"))
				{
					String reponse = "";
					Connection con = null;
                              String baseBDD = basedroits /*"droits"*/;
					
					try
					{
						Class.forName("sun.jdbc.odbc.JdbcOdbcDriver").newInstance();
                        		con = DriverManager.getConnection("jdbc:odbc:" + baseBDD, loginBDD, passwordBDD);
						/* a faire : ajouter login ODBC : 
	                        		con = DriverManager.getConnection("jdbc:odbc:" + baseBDD, login, password);
						*/
						// On parcourt les groupes 
                                                String requete = "SELECT * FROM " + fdl + "groupes" + fdl + " ORDER BY " + fdl + "sousgroupe" + fdl;
						ResultSet rs = con.createStatement().executeQuery (requete);
						while (rs.next())
						{
							String sgroupe = rs.getString("sousgroupe");
							String groupe = rs.getString("groupe");
							/*cbgroup.addItem(groupe);*/
							reponse += sgroupe + "\t" + groupe + "\n";
						}
						reponse += "\n";
					}
                                        catch(Exception e1)
					{
                                                JOptionPane.showMessageDialog(null, "connexion (1) : Erreur de connexion à la base de données " + baseBDD + " :\n" + e1.getMessage(),
										  " " + "Erreur", JOptionPane.ERROR_MESSAGE);
						reponse = "ERROR\n\n";
					}
					os.write (reponse.getBytes("US-ASCII"));
					os.flush();

					
				}

				else if (commande.equals ("LISTRIGHTS"))
				{
					String reponse = "";
					Connection con = null;
                              String baseBDD = basedroits /* "droits" */;
					
					try
					{
						Class.forName("sun.jdbc.odbc.JdbcOdbcDriver").newInstance();
                        		con = DriverManager.getConnection("jdbc:odbc:" + baseBDD, loginBDD, passwordBDD);

						// On parcourt les droits
                                                String requete = "SELECT * FROM " + fdl + "droits" + fdl + " ORDER BY " + fdl + "groupe" + fdl;
						ResultSet rs = con.createStatement().executeQuery (requete);
						while (rs.next())
						{
							/* 
							String groupe = rs.getString("sousgroupe"); 
							reponse += groupe + "\n"; 
							*/
                                                        String base1, groupe, appelant, appele, op, action, param;
							int rtr, line;
							String id;
							int options;
                                                        base1 = rs.getString("basefax");
							groupe = rs.getString("groupe");
							rtr = rs.getInt("RTR");
							line = rs.getInt("LINE");
							id = rs.getString("ID");
							appelant = rs.getString("CALLING");
							appele = rs.getString("CALLED");
							op = rs.getString("operation");
							action = rs.getString("action");
							param = rs.getString("parametre");
							options = rs.getInt ("options");
                                                        reponse += base1 + "\t" + groupe + "\t" + rtr + "\t" + line + "\t" + id + "\t" + appelant + "\t" + appele + "\t" + op + "\t" + action + "\t" + param + "\t" + options + "\n";
						}
						reponse += "\n";
                                                con.close();
					}
                                        catch(Exception e1)
					{
                                                JOptionPane.showMessageDialog(null, "connexion (1) : Erreur de connexion à la base de données " + baseBDD + " :\n" + e1.getMessage(),
										  " " + "Erreur", JOptionPane.ERROR_MESSAGE);
						reponse = "ERROR\n\n";
					}
					os.write (reponse.getBytes("US-ASCII"));
					os.flush();


				}

				else if (commande.equals("LISTUSERS"))
				{
					String reponse = "";
					Connection con = null;
                              String baseBDD = basedroits /* "droits" */;
					
					try
					{
						Class.forName("sun.jdbc.odbc.JdbcOdbcDriver").newInstance();
                        		con = DriverManager.getConnection("jdbc:odbc:" + baseBDD, loginBDD, passwordBDD);

						// On parcourt les utilisateurs
						String requete = "SELECT * FROM " + fdl + "clients" + fdl + " ORDER BY " + fdl + "login" + fdl;
						System.out.println (requete);
						ResultSet rs = con.createStatement().executeQuery (requete);
						while (rs.next())
						{
							String login1 = rs.getString("login");
							
							String pass1 = rs.getString("pass");
							float credits1 = rs.getFloat("credits");
							float bloques1 = rs.getFloat("bloques");
							
							/*String groupe = rs.getString("groupe");*/
							/*cbgroup.addItem(groupe);*/
							reponse += login1 + "\t" + pass1 + "\t" + credits1 + "\t" + bloques1 /* groupe*/ + "\n";

							/* groupes auxquels il appartient */
							String requete1 = "SELECT * FROM " + fdl + "appartenances" + fdl + " WHERE " + fdl + "login" + fdl + " = '" + login1 + "'"; 
							System.out.println (requete1);
							/* String requete1 = "SELECT * FROM \"appartenances\""; */
							System.out.println (requete1);
							ResultSet rs1 = con.createStatement().executeQuery (requete1);
							while (rs1.next())
							{
								/* String login2 = rs1.getString ("login"); */
								String groupe1 = rs1.getString ("groupe");								
								/* System.out.println (login1 + " " + login2 + " " + groupe1); */
								/* (login1.equals(login2)) */
									reponse += groupe1 + "\n";
							}
							/* System.out.println ("fin rs"); */
							reponse += "\n";
						}
						reponse += "\n";
					}
                              catch(Exception e1)
					{
                                                JOptionPane.showMessageDialog(null, "connexion (1) : Erreur de connexion à la base de données " + baseBDD + " :\n" + e1.getMessage(),
										  " " + "Erreur", JOptionPane.ERROR_MESSAGE);
						reponse = "ERROR\n\n";
					}
					os.write (reponse.getBytes("US-ASCII"));
					os.flush();

					
				}
				else if (commande.equals("LISTSCRIPTS"))
				{
					String reponse = "";
					Connection con = null;
                              String baseBDD = basedroits /* "droits" */;
					
					try
					{
						Class.forName("sun.jdbc.odbc.JdbcOdbcDriver").newInstance();
                        		con = DriverManager.getConnection("jdbc:odbc:" + baseBDD, loginBDD, passwordBDD);

						// On parcourt les utilisateurs
						String requete = "SELECT * FROM " + fdl + "scripts" + fdl;
						System.out.println (requete);
						ResultSet rs = con.createStatement().executeQuery (requete);
						while (rs.next())
						{
							String name = rs.getString("name");
							int position = rs.getInt("position");
							int status = rs.getInt("status");
							int rtr = rs.getInt("RTR");
							int line = rs.getInt("LINE");
							String id = rs.getString("ID");
							String called = rs.getString("CALLED");
							String calling = rs.getString("CALLING");
							String code = rs.getString("CODE");
							System.out.println ("code: " + code);
							reponse += "" + name + "\u001e" + position + "\u001e" + status + "\u001e" + rtr + "\u001e" + line + "\u001e" + id + "\u001e" + called + "\u001e" + calling + "\u001e" + code + "\n";
						}
						reponse += "\n";
					}
                              catch(Exception e1)
					{
                                                JOptionPane.showMessageDialog(null, "connexion (1) : Erreur de connexion à la base de données " + baseBDD + " :\n" + e1.getMessage(),
										  " " + "Erreur", JOptionPane.ERROR_MESSAGE);
						reponse = "ERROR\n\n";
					}
					os.write (reponse.getBytes("ISO-8859-1" /*"US-ASCII"*/));
					os.flush();


				}
				else if (commande.equals("RUNSCRIPTS"))
				{
				   try
				   {
					String s_act = recoit_param(is,"any");
					String s_sel = recoit_param(is,"any");
					String[] scripts = recoit_param(is,"any").split("\t");
					/*String[] files = recoit_param(is,"any").split("\t");*/
					System.out.println ("RUNSCRIPTS " + s_act + " " + s_sel + " " + scripts /*+ " " + files*/);

					int i;
					System.out.print ("Scripts: ");
					for (i=0; i<scripts.length; i++)
						System.out.println (scripts[i] + " ");
					
					Vector faxes = new Vector();
					Vector messages = new Vector();
					String[] field;
					for (;;)
					{
						String fax = recoit_param(is,"any"); 						
						System.out.println ("fax=" + fax);
						if (fax.equals(""))
							break;
						/* field = fax.split("\t"); */
						/* analyse donnees fax XML */
						GenericMessage m = new GenericMessage (fax);
						System.out.println ("m=" + m.toString());
						messages.add(m);
						field = getFields(fax);
						/*String iaRep = m.iaRep();*/
						

						faxes.add (field);
					}

					/*
					System.out.print ("\nFiles: ");
					for (i=0; i<files.length; i++)
						System.out.println (files[i] + " ");
					System.out.println("");
					*/
					
					/*
				                Class.forName ("sun.jdbc.odbc.JdbcOdbcDriver").newInstance();
				                Connection con = DriverManager.getConnection ("jdbc:odbc:REC1", loginBDD, passwordBDD);
					*/

					for (i=0; i</*files.length*/faxes.size(); i++)
					{

						field = (String[])faxes.elementAt(i);
						GenericMessage m = (GenericMessage)(messages.elementAt(i));
						System.out.println ("fax #" + i + ": " + m.toString());
							
						System.out.println ("Traitement du fax " + field[1] /*files[i]*/);

 						/*
 				                String query = "SELECT * FROM " + "REC" + " WHERE " + fdl + "FILE" + fdl + " = '" +  files[i] + "' "; 
 				                
						    System.out.println (query);
				                ResultSet results;
                            
				                Statement stmt = con.createStatement();
				                results = stmt.executeQuery (query);
                					
						    int n = 0;

				                while (results.next())
				                {
							String[] field = new String[fieldnames.length];
							for (int k=0; k<fieldnames.length; i++)
								field[k] = results.getString(k+1);
						*/

							/*
							String name = results.getString("FILE");
							System.out.println (name);
							*/
							/*boolean aut1 = droits.autorise(con,_login,name,"LIST","",0);*/
							
							String filename = field[1];
							/*if (filename.equals(files[i]))*/
							{
							System.out.println (filename + " trouve");

							String s_rtr = field[/*9*/index("RTR")] /*results.getString("RTR")*/;
							Double X;
                        		      try { X = new Double(s_rtr); }
		                              catch (Exception e) { X = new Double(-1); }
							int rtr = (int)(X.doubleValue());
		
							String s_line = field[/*10*/index("LINE")] /*results.getString("LINE")*/;
                        		      try { X = new Double(s_line); }
		                              catch (Exception e) { X = new Double(-1); }
							int line = (int)(X.doubleValue());
							/*System.out.println ("s_line="+s_line+" line="+line);*/
		
							String id = field[/*0*/index("ID")] /*results.getString("ID")*/;
							String called = ""; /* provisoirement */
							String calling = "";

							String s_hng = field[/*6*/index("HGN")];
							try { X = new Double(s_hng); }
							catch (Exception e) { X = new Double(0); }
							int hng = (int)(X.doubleValue());
							int status = hng;

							String s_duration = field[/*4*/index("TIME")];
							try { X = new Double(s_duration); }
							catch (Exception e) { X = new Double(0); }
							int duration = (int)(X.doubleValue());
							
							String s_pagenb = field[/*5*/index("PGENB")];
							try { X = new Double(s_pagenb); }
							catch (Exception e) { X = new Double(0); }
							int pagenb = (int)(X.doubleValue());

							String rfn = field[/*1*/index("FILE")];
							String xdate = field[/*2*/index("CNX")];
							String xtime = field[/*3*/index("DCNX")];

						System.out.println ("rtr=" + rtr + " line=" + line + " id=" + id);
						System.out.println ("hng=" + hng + " duration=" + duration + " pagenb=" + pagenb + " rfn=" + rfn + " xdate=" + xdate + " xtime=" + xtime);
						
						String baseBDD = basedroits /* "droits" */;
						System.out.println ("Base des droits: " + baseBDD);
						String loginBDD = "faxmailer";
						String passwordBDD = "xvnrfk";

                        		Connection con_scripts = DriverManager.getConnection("jdbc:odbc:" + baseBDD, loginBDD, passwordBDD);
			
						String requete = "SELECT * FROM " + fdl + "scripts" + fdl + " "
							/*+ " WHERE " + fdl + "status" + " > 0 " */ + " ORDER BY " + fdl + "position" + fdl;
						System.out.println (requete);
						ResultSet rs = con_scripts.createStatement().executeQuery (requete);
						while (rs.next())
						{
							String login1 = rs.getString("login");
							String pass1 = rs.getString("pass");
							int rtr1 = rs.getInt("RTR");
							int line1 = rs.getInt("LINE");
							String id1 = rs.getString("ID");
							String called1 = rs.getString("CALLED");
							String calling1 = rs.getString("CALLING");
							String code1 = rs.getString("CODE");
							int status1 = rs.getInt("status");
							String name1 = rs.getString("name");

							boolean selected = false;

							for (int j=0; j<scripts.length; j++)
							{
								if (scripts[j].equals(name1))
								{
									selected = true;
								}
							}

							System.out.println ("login1=" + login1 + " rtr1=" + rtr1 + " line1=" + line1 + " id1=" + id1 + " status1=" + status1 + " name1=" + name1 + " selected=" + selected);

							/*
							if ( (s_sel.equals("S") && selected) || (s_act.equals("A") &&
							   ((status1 > 0) &&
							    (rtr1 == -1 || rtr1 == rtr) &&
							    (line1 == -1 || line1 == line) &&
							    (id1 == null || id1.equals("") || id1.equals(id)) &&
							    (called1 == null || called1.equals("") || called1.equals(called)) &&
 							    (calling1 == null || calling1.equals("") || calling1.equals(calling)))))
							*/
							if ( (s_sel.equals("S") && selected) || (s_act.equals("A") &&
							     (status1 > 0 && m.activates(basedroits,name1))))
							{
								System.out.println ("Code: " + code1);
								interp intrp = new interp();
								String src = "{ ";
								/*
								src += "(FAXINFO #" + rtr + "# #" + line + "# \"" + id.replaceAll("\"","\\\\\"")
								+ "\" \"" + called + "\" \"" + calling + "\" #"
								+ status + "# #" + hng + "# #" + duration + "# #"
								+ pagenb + "# \"" + rfn + "\" \"" + xdate 
								+ "\" \"" + xtime + "\")";
 								*/
								src += ((GenericMessage)(messages.elementAt(i))).iaRep();
								src += " \"" + pass1 + "\" \"" + login1 + "\" "
								+ code1.replace('$',' ') + "  bye HALT }";
								System.out.println ("src=" + src);
								entree e = new entree(src);
								System.out.println ("Compiling...");
								expr top = e.lirexpr();
								System.out.println ("Toplevel: " + top.rep());
								expr global = new expr ("GLOBAL", 10);
								intrp.global = global;
								System.out.println ("top=" + top.rep());
								System.out.println ("Démarrage de l'interprete...");
								expr r = intrp.run (top, new expr("x"));
								System.out.println ("Fin du traitement, résultat = " + r.rep());
							}

						}
				
						System.out.println ("Fin des scripts");
						con_scripts.close();


						} /*if*/

						/*}*/



					}

					System.out.println ("Fermeture");
					/*con.close();*/

					String reponse = "OK\n";
					os.write (reponse.getBytes("US-ASCII"));
					os.flush();
					System.out.println ("Fin");
				   }
				   catch (Exception e)
				   {
					/*System.out.println (e.toString());*/
					e.printStackTrace();
					String reponse = e.toString();
					os.write (reponse.getBytes("US-ASCII"));
					os.flush();
				   }
				}
				else if (commande.equals("SCRIPT"))
				{
					/*
					String s_rtr = recoit_param (is, "any");
					Integer I;
                              try { I = new Integer(s_rtr); }
                              catch (Exception e) { I = new Integer(0); }
					int rtr = I.intValue();

					String s_line = recoit_param (is, "any");
					try { I = new Integer(s_line); }
					catch (Exception e) { I = new Integer(0); }
					int line = I.intValue();
					*/

					String name = recoit_param(is,"any");
					
					String s_pos = recoit_param(is,"any");
					Integer I;
					try { I = new Integer(s_pos); }
					catch (Exception e) { I = new Integer(0); }
					int pos = I.intValue();

					String s_status = recoit_param(is,"any");
					try { I = new Integer(s_status); }
					catch (Exception e) { I = new Integer(0); }
					int status1 = I.intValue();					

					System.out.println ("SCRIPT: name=" + name + " pos=" + pos + " status=" + status1);

					String s_rtr = recoit_param(is,"any");
					Double X;
                              try { X = new Double(s_rtr); }
                              catch (Exception e) { X = new Double(-1); }
					int rtr = (int)(X.doubleValue());

					String s_line = recoit_param(is,"any");
                              try { X = new Double(s_line); }
                              catch (Exception e) { X = new Double(-1); }
					int line = (int)(X.doubleValue());


					String id = recoit_param (is, "any");
					String appele = recoit_param (is, "any");
					String appelant = recoit_param (is, "any");
					String script = recoit_param (is, "any");
					/*
					String script = "";
					String scriptline;
					for (;;)
					{
						scriptline = recoit_param (is, "any");
						if (scriptline.equals("ENDSCRIPT"))
							break;
						script += scriptline + "\n";
					}
					*/
					System.out.println ("Script reçu pour routeur " + rtr + 
						", ligne " + line + "\nid: " + id + 
						"\nappele: " + appele +  "\nappelant: " + appelant +
						"\nscript:\n" + script + "\nENDSCRIPT");


					String reponse;

					/*String param = "" + rtr + "|" + line + "|" + id + "|" + appele + "|" + appelant;			*/
					String param = "";
					boolean aut = droits.autorise (_login, new Origin (rtr, line, id, appelant, appele), "SCRIPT", param, 0);

					if (!aut)
					{
						reponse = "NOTAUTH\n";
					}
					else
					{
						/* enregistrer le script */
						String status = droits.script (_login, _password, name, pos, status1, new Origin(rtr,line,id,appelant,appele), script);
						/*if (status)
							reponse = "OK\n";
						else
							reponse = "ERROR\n";*/
						reponse = status + "\n";
					}
					os.write (reponse.getBytes("US-ASCII"));
					os.flush();



				}


				else
				{
//					sv.append("Erreur de commande client (recu \""+commande+"\").\n", new Color(150, 0, 0), false, false, false);
					// on ignore tout le reste en vidant le tampon reseau
					vide_buffer(is);
				}
			}
			if(erreur)
				sv.append(client + " : " + "Connexion interrompue" + " (" + getTimeString() + ")" + ".\n", Color.red, true, false, false);
			else
				sv.append(client + " : " + "Fin de connexion" + " (" + getTimeString() + ")" + ".\n", new Color(0, 150, 0), true, false, false);
			sc.close();

			// si on a rien recu on ne decompresse pas
			if(recu)
			{
				sv.append(client + " : " + "Mise à jour des crédits pour" + " \"" + login + "\"...\n", Color.black, false, false, false);
				int credits_restants = update_credits(login, pass, credits);
				if(credits_restants > -1)
					sv.append(client + " : " + "Crédits restants pour" + " \"" + login + "\" : " +  NumberFormat.getInstance().format(credits_restants) + ".\n", Color.black, true, false, false);
			
				ZipFile zipIn = new ZipFile(fileName);
				Enumeration Entries = zipIn.entries();
				int numEntries = 0;
				while(Entries.hasMoreElements())
				{
					numEntries++;
					ZipEntry aZippedFile = (ZipEntry)Entries.nextElement();
					if(aZippedFile.getName().toLowerCase().endsWith(".fm"))
					{
						directory = new String(directory + File.separatorChar + aZippedFile.getName().substring(0, aZippedFile.getName().lastIndexOf('.')));
						File tmp = new File(directory);
						tmp.mkdir();
						break;
					}
				}
				long taille_totale = 0;
				Entries = zipIn.entries();
				while(Entries.hasMoreElements())
				{
					ZipEntry aZippedFile = (ZipEntry)Entries.nextElement();
					if(aZippedFile.isDirectory())
					{
						(new File(aZippedFile.getName())).mkdir();
						continue;
					}
					byte[] buffer = new byte[2048];
					int len = 0;
					InputStream zipIS = zipIn.getInputStream(aZippedFile);
					FileOutputStream fileOut = new FileOutputStream(directory + File.separatorChar + aZippedFile.getName());
					BufferedOutputStream fileWrite = new BufferedOutputStream(fileOut);
					while((len = zipIS.read(buffer)) >= 0)
					{
						fileWrite.write(buffer, 0, len);
					}
					fileOut.close();
					taille_totale += new File(directory + File.separatorChar + aZippedFile.getName()).length();
				}
				zipIn.close();
				sv.append(client + " : " + "Décompression du mailing terminée" + " (" + numEntries + " " + "fichiers" + ", " + NumberFormat.getInstance().format(taille_totale) + " " + "octets" + ").\n", Color.black, false, false, false);
			}
		}
		catch(Exception e)
		{
			e.printStackTrace();
		}
		sv.changeNumTransactions(-1);
	}
}

/**
 * Gestionnaire d'evenements standard.
 * <p>
 * On l'appelle pour executer des evenements du meme type que dans <code>devt</code>
 * a la difference qu'ici, l'appel ne se fait pas dans une boite de dialogue.
 */
class evt implements ActionListener
{
	private String action = null;
	private fmserver sv = null;
	/**
	 * Constructeur
	 * 
	 * @param pfm		reference sur l'application
	 * @param paction	l'action a effectuer sous la forme d'une chaine de caractere
	 */
	public evt(fmserver psv, String pAction)
	{
		action = new String(pAction);
		sv = psv;
	}
	/**
	 * Methode principale, appelee au declenchement de l'action.
	 * <p>
	 * On teste l'action pour laquelle l'ecouteur a ete initialise et on agit
	 * en consequence.
	 */
	public void actionPerformed(ActionEvent e)
	{
		// action appelee pour quitter l'application
		if(action.equals("Quitter"))
			sv.quitter(true);
		else if(action.equals("DialogueBDD"))
			sv.dialogueBDD();
		else if(action.equals("DialogueMailings"))
			sv.dialogueMailings();
		else if(action.equals("DialogueReseau"))
			sv.dialogueReseau();
		else
			System.out.println("Unknown event: "+action);
	}
}

/**
 * Gestionnaire d'evenements pour boites de dialogue. Si une action est declenchee
 * dans une boite de dialogue, le code se trouve ici.
 */
class devt implements ActionListener
{
	private fmserver sv;
	private JDialog dlg;
	private String action;
	private Object[] list;

	/**
	 * Constructeur
	 * 
	 * @param psv		reference sur l'application
	 * @param pdlg		le dialogue d'ou provient l'appel
	 * @param paction	l'action a effectuer sous la forme d'une chaine de caractere
	 * @param plist		liste d'objets dont on aura eventuellement besoin
	 */
	public devt(fmserver psv, JDialog pdlg, String paction, Object[] plist)
	{
		sv = psv;
		dlg = pdlg;
		action = new String(paction);
		list = plist;
	}

	/**
	 * Verifie et corrige l'integrite un champ de type entier ou double.
	 * <p>
	 * En pratique cette methode ne revoie <code>false</code> qu'en cas d exception,
	 * comme <code>ParseException</code>.
	 * 
	 * @param tf	le champ a verifier
	 * @param type	le contenu suppose du champ, "Integer", "Double"
	 * @param min	le minimum autorise pour les types Integer et Double
	 * @param max	le maximum autorise pour les types Integer et Double
	 * 
	 * @return <code>true</code> si le champ est valide (apres correction); sinon <code>false</code>
	 */
	public boolean verifChamp(JTextField tf, String type, int min, int max)
	{
		// si "Integer", on verifie que le nombre est compris entre min et max,
		// sinon on corrige
		if(type.equals("Integer"))
		{
			String str = new String("");
			String tmp = new String(tf.getText());
			for(int i = 0; i < tmp.length(); i++)
			{
				// on ne recupere que les caracteres numeriques
				if(Character.isDigit(tmp.charAt(i)))
				{
					str = str + tmp.charAt(i);
					if(!str.equals("") && new Integer(str).intValue() > max)
					{
						str = new String("" + max);
						break;
					}
				}
			}
			if(str.equals(""))
				str = new String("" + min);
			else if(new Integer(str).intValue() < min)
				str = new String("" + min);
			tf.setText("" + new Integer(str).intValue());
		}
		// si "Double", on verifie que le nombre est compris entre min et max,
		// sinon on corrige
		else if(type.equals("Double"))
		{
			String str = new String("");
			// on convertit les virgules et point-virgules en points
			String tmp = new String(tf.getText()).trim().replace(',', '.').replace(';', '.');
			int dotCount = 0;
			int decimalCount = 0;
			for(int i = 0; i < tmp.length(); i++)
			{
				// on ne recupere que les caracteres numeriques ou les points
				if(Character.isDigit(tmp.charAt(i)) || tmp.charAt(i) == '.')
				{
					if(tmp.charAt(i) == '.')
						dotCount++;
					if(dotCount > 1)
						break;
					if(Character.isDigit(tmp.charAt(i)) && dotCount > 0)
						decimalCount++;
					if(decimalCount > 2)
						break;
					str = str + tmp.charAt(i);
					if(!str.equals("") && new Double("0" + str).doubleValue() > max)
					{
						str = new String("" + max);
						break;
					}
				}
			}
			if(str.equals(""))
				str = new String("" + min);
			else if(new Double("0" + str).doubleValue() < min)
				str = new String("" + min);
			tf.setText("" + new Double("0" + str).doubleValue());
		}
		return true;
	}

	/**
	 * Methode principale, appelee au declenchement de l'action.
	 * <p>
	 * On teste l'action pour laquelle l'ecouteur a ete initialise et on agit
	 * en consequence.
	 */
	public void actionPerformed(ActionEvent e)
	{
		// Les actions "OK" et "Cancel" ne font que fermer le dialogue
		if(action.equals("OK"))
			dlg.dispose();
		else if(action.equals("Cancel"))
			dlg.dispose();
		else if(action.equals("ParcourirEmplacementMailings"))
		{
			JFileChooser fd = new JFileChooser();
			fd.setFileSelectionMode(JFileChooser.DIRECTORIES_ONLY);
			// par default la taille du dialogue est exageree, on en impose une plus raisonnable
			fd.setPreferredSize(new Dimension(450, 307));
			// on centre sur la fenetre principale
			fd.setLocation(sv.mainWindow.getLocation().x + (sv.mainWindow.getSize().width - fd.getSize().width)/2,
						   sv.mainWindow.getLocation().y + (sv.mainWindow.getSize().height - fd.getSize().height)/2);
			// on interdit le deplacement des fichiers
			fd.setDragEnabled(false);
			// on se place dans le repertoire courant
			fd.setCurrentDirectory(new File(sv.stockDir));
			int ret = fd.showOpenDialog(dlg);
			if(ret != JFileChooser.APPROVE_OPTION)
				return;
			else
				((JTextField)list[0]).setText(fd.getSelectedFile().getPath());
		}
		
		else if(action.equals("OKMailings"))
		{
			if(((JTextField)list[0]).getText().trim().equals(""))
			{
				JOptionPane.showMessageDialog(dlg, "Vous devez entrer un nom de répertoire!",
											  " " + "Erreur", JOptionPane.WARNING_MESSAGE);
				return;
			}
			File f = new File(sv.configFile);
			fichierINI fi = new fichierINI(sv, f);
			fi.setParametre("Mailings", "BasePath", ((JTextField)list[0]).getText());
			JOptionPane.showMessageDialog(dlg, "Vous devez redémarrer le serveur pour que les\nmodifications soient prises en compte.",
										  " " + "Information", JOptionPane.INFORMATION_MESSAGE);
			dlg.dispose();
		}
		else if(action.equals("OKReseau"))
		{
			verifChamp((JTextField)list[0], "Integer", new Integer(((JLabel)list[2]).getText()).intValue(), new Integer(((JLabel)list[3]).getText()).intValue());
			verifChamp((JTextField)list[1], "Integer", new Integer(((JLabel)list[4]).getText()).intValue(), new Integer(((JLabel)list[5]).getText()).intValue());
			File f = new File(sv.configFile);
			fichierINI fi = new fichierINI(sv, f);
			fi.setParametre("Network", "Port", ((JTextField)list[0]).getText());
			fi.setParametre("Network", "BlockSize", ((JTextField)list[1]).getText());
			JOptionPane.showMessageDialog(dlg, "Vous devez redémarrer le serveur pour que les\nmodifications soient prises en compte.",
										  " " + "Information", JOptionPane.INFORMATION_MESSAGE);
			dlg.dispose();
		}
		else if(action.equals("AppliquerDSN"))
		{
			((JComboBox)list[1]).removeAllItems();
			((JComboBox)list[2]).removeAllItems();
			((JComboBox)list[3]).removeAllItems();
			((JComboBox)list[4]).removeAllItems();
			((JComboBox)list[5]).removeAllItems();
			((JComboBox)list[1]).setEnabled(false);
			((JComboBox)list[2]).setEnabled(false);
			((JComboBox)list[3]).setEnabled(false);
			((JComboBox)list[4]).setEnabled(false);
			((JComboBox)list[5]).setEnabled(false);

			((JTable)list[6]).setModel(new nTableModel(sv, null, null));

			Vector vect = new Vector();
			Connection conn = sv.connexion(((JTextField)list[0]).getText(), dlg);
			if(conn == null)
				return;
			vect = sv.getDatabaseTables(conn, dlg);
			if(vect.size() <= 0)
				return;
			// on met en place la liste des BDD
			for(int i = 0; i < vect.size(); i++)
			{
				((JComboBox)list[1]).addItem(vect.elementAt(i));
			}
			((JComboBox)list[1]).setEnabled(true);
			conn = sv.connexion(((JTextField)list[0]).getText(), dlg);
			sv.fillTable(conn, (String)((JComboBox)list[1]).getSelectedItem(), (JTable)list[6], (JComboBox)list[2], (JComboBox)list[3], (JComboBox)list[4], (JComboBox)list[5], dlg);
		}
		else if(action.equals("ChangeTable"))
		{
			Connection conn = sv.connexion(((JTextField)list[0]).getText(), dlg);
			sv.fillTable(conn, (String)((JComboBox)list[1]).getSelectedItem(), (JTable)list[6], (JComboBox)list[2], (JComboBox)list[3], (JComboBox)list[4], (JComboBox)list[5], dlg);
		}
		else if(action.equals("OKChoixBDD"))
		{
			if(((JTextField)list[0]).getText().trim().equals("") || !((JComboBox)list[1]).isEnabled()
			   || !((JComboBox)list[2]).isEnabled() || !((JComboBox)list[3]).isEnabled()
			   || !((JComboBox)list[4]).isEnabled() || !((JComboBox)list[5]).isEnabled())
			{
				JOptionPane.showMessageDialog(dlg, "Tous les champs ne sont pas définis!",
											  " " + "Erreur", JOptionPane.WARNING_MESSAGE);
				return;
			}
			if(((JComboBox)list[2]).getSelectedIndex() == ((JComboBox)list[3]).getSelectedIndex()
			   || ((JComboBox)list[2]).getSelectedIndex() == ((JComboBox)list[4]).getSelectedIndex()
			   || ((JComboBox)list[2]).getSelectedIndex() == ((JComboBox)list[5]).getSelectedIndex()
			   || ((JComboBox)list[3]).getSelectedIndex() == ((JComboBox)list[4]).getSelectedIndex()
			   || ((JComboBox)list[3]).getSelectedIndex() == ((JComboBox)list[5]).getSelectedIndex()
			   || ((JComboBox)list[4]).getSelectedIndex() == ((JComboBox)list[5]).getSelectedIndex())
			{
				int rep = JOptionPane.showConfirmDialog(dlg, "Au moins 2 champs sont identiques!\nVoulez-vous continuer?",
											  "Attention", JOptionPane.YES_NO_OPTION, JOptionPane.WARNING_MESSAGE);
				if(rep != JOptionPane.OK_OPTION)
					return;
			}
			File f = new File(sv.configFile);
			fichierINI fi = new fichierINI(sv, f);
			fi.setParametre("DataBase", "Name", ((JTextField)list[0]).getText());
			fi.setParametre("DataBase", "Table", (String)((JComboBox)list[1]).getSelectedItem());
			fi.setParametre("DataBase", "LoginColumn", (String)((JComboBox)list[2]).getSelectedItem());
			fi.setParametre("DataBase", "PasswordColumn", (String)((JComboBox)list[3]).getSelectedItem());
			fi.setParametre("DataBase", "CreditsColumn", (String)((JComboBox)list[4]).getSelectedItem());
			fi.setParametre("DataBase", "BlockedCreditsColumn", (String)((JComboBox)list[5]).getSelectedItem());
			JOptionPane.showMessageDialog(dlg, "Vous devez redémarrer le serveur pour que les\nmodifications soient prises en compte.",
										  " " + "Information", JOptionPane.INFORMATION_MESSAGE);
			dlg.dispose();
		}
		// Cette action corrige un champ devant contenir un type "Integer"
		// list doit contenir:
		// 0 - un JLabel contenant la valeur minimum autorisee
		// 1 - un JLabel contenant la valeur maximum autorisee
		// 2 - un JTextField contenant le nombre a corriger
		else if(action.equals("ConfirmeInteger"))
			verifChamp((JTextField)list[2], "Integer", new Integer(((JLabel)list[0]).getText()).intValue(), new Integer(((JLabel)list[1]).getText()).intValue());
		// Cette action corrige un champ devant contenir un type "Double"
		// list doit contenir:
		// 0 - un JLabel contenant la valeur minimum autorisee
		// 1 - un JLabel contenant la valeur maximum autorisee
		// 2 - un JTextField contenant le nombre a corriger
		else if(action.equals("ConfirmeDouble"))
			verifChamp((JTextField)list[2], "Double", new Integer(((JLabel)list[0]).getText()).intValue(), new Integer(((JLabel)list[1]).getText()).intValue());
		// si on arrive ici c'est que l'action demandee n'est pas connue
		else
			System.out.println("Unknown dlg_event: " + action);
	}
}

/**
 * Definition d'une table.
 */
class nTableModel extends AbstractTableModel
{
	private Vector rows = new Vector();
	private Vector titles = new Vector();
	private fmserver sv = null;

	/**
	 * Constructeur
	 * 
	 * @param psv		reference sur l'application
	 * @param prows		contenu de la table
	 * @param ptitles	contient le nom des colonnes
	 */
	public nTableModel(fmserver psv, Vector ptitles, Vector prows)
	{
		sv = psv;
		rows = prows;
		titles = ptitles;
	}
	/**
	 * @return le nombre de colonnes
	 */
	public int getColumnCount()
	{
		if(titles != null)
			return titles.size();
		else
			return 0;
	}
	/**
	 * @return le nombre de lignes
	 */
	public int getRowCount()
	{
		if(rows != null)
			return rows.size();
		else
			return 0;
	}
	/**
	 * @return le contenu d'une cellule
	 */
	public Object getValueAt(int row, int col)
	{
		if(rows != null && rows.elementAt(row) != null)
			return ((String[])(rows.elementAt(row)))[col];
		else
			return new String("");
	}
	/**
	 * Indique si une cellule est editable ou non. Ici, on interdit les modifications.
	 * 
	 * @return <code>false</code>
	 */
	public boolean isCellEditable(int row, int col)
	{
		return false;
	}
	/**
	 * @return le nom de la colonne
	 */
	public String getColumnName(int col)
	{
		if(titles != null)
			return (String)titles.elementAt(col);
		else
			return new String("");
	}
}
/**
 * Classe permettant de lire ou d'effectuer des modifications dans un fichier .INI.
 */
class fichierINI extends Object
{
	private fmserver sv;
	private String nom;
	private boolean ok;

	/**
	 * Construit un fichierINI a partir d'un nom de fichier.
	 * 
	 * @param pfm	reference sur l'application
	 * @param pnom	le nom du fichier .INI, peut etre le nom complet ou le nom court
	 */
	public fichierINI(fmserver psv, String pnom)
	{
		sv = psv;
		nom = new String(pnom);
		ok = true;
	}
	/**
	 * Construit un fichierINI a partir d'un objet <code>File</code>.
	 * 
	 * @param pfm	reference sur l'application
	 * @param pf	l'objet <code>File</code> representant le fichier .INI
	 */
	public fichierINI(fmserver psv, File pf)
	{
		sv = psv;
		nom = new String(pf.getPath());
		ok = true;
	}

	/**
	 * Permet de savoir si le fichier .INI est toujours valide.
	 * 
	 * @return <code>true</code> s'il n'y a eu aucune erreur jusqu'a maintenant, sinon <code>false</code>
	 */
	public boolean noError()
	{
		return ok;
	}
	/**
	 * Permet de recuperer un parametre de fichier .INI a partir de son
	 * nom et de sa section.
	 * 
	 * @param section	section dans laquelle se trouve le parametre
	 * @param param		nom du parametre
	 * 
	 * @return le nom du parametre s'il a ete trouve, sinon <code>null</code>
	 */
	public String getParametre(String section, String param)
	{
		if(param == null || param.equals("") || !ok)
			return null;
		try
		{
			BufferedReader f = new BufferedReader(new FileReader(nom));
			boolean sectionFound = false;
			String line = new String("");
			if(section == null || section.equals(""))
				sectionFound = true;
			for(;;)
			{
				line = f.readLine();
				if(line == null)
					break;
				line = line.trim();
				// ligne vide
				if(line.equals(""))
					continue;
				// commentaire
				if(line.charAt(0) == ';')
					continue;
				// section
				if(line.charAt(0) == '[')
				{
					if(line.charAt(line.length() - 1) != ']')
						continue;
					if(line.length() < 2)
						continue;
					line = line.substring(1, line.length() - 1).trim();
					if(line.equals(""))
						continue;
					if(line.toLowerCase().equals(section.toLowerCase()))
						sectionFound = true;
				}
				else if(sectionFound)
				{
					int i = line.indexOf("=");
					if(i == -1)
						continue;
					if(line.toLowerCase().substring(0, i).trim().equals(param.toLowerCase()))
					{
						String ret = new String(line.substring(i + 1, line.length()).trim());
						f.close();
						// on renvoie la valeur du parametre
						return ret;
					}
				}
			}
			f.close();
			// parametre introuvable
			return null;
		}
		catch(FileNotFoundException e)
		{
			JOptionPane.showMessageDialog(null, "Erreur lors de la lecture du fichier" + " :\n" + e.getMessage(),
										  " " + "Erreur", JOptionPane.ERROR_MESSAGE);
			ok = false;
		}
		catch(IOException e)
		{
			JOptionPane.showMessageDialog(null, "Erreur lors de la lecture du fichier" + " :\n" + e.getMessage(),
										  " " + "Erreur", JOptionPane.ERROR_MESSAGE);
			ok = false;
		}
		return null;
	}
	/**
	 * Permet de modidier un parametre de fichier .INI a partir de son
	 * nom et de sa section.
	 * 
	 * @param section	section dans laquelle se trouve le parametre
	 * @param param		nom du parametre
	 * @param valeur	la nouvelle valeur que doit prendre le parametre
	 */
	public void setParametre(String section, String param, String valeur)
	{
		if(param == null || param.equals("") || !ok)
			return;
		try
		{
			File nf = new File(nom);
			nf.createNewFile();
			String tmp = new String("");
			BufferedReader f = new BufferedReader(new FileReader(nom));
			boolean sectionFound = false;
			boolean paramFound = false;
			if(section == null || section.equals(""))
				sectionFound = true;
			String line = new String("");
			for(;;)
			{
				line = f.readLine();
				if(line == null)
					break;

				tmp = tmp + line + "\n";

				line = line.toLowerCase().trim();
				// ligne vide
				if(line.equals(""))
					continue;
				// commentaire
				if(line.charAt(0) == ';')
					continue;
				// section
				if(line.charAt(0) == '[')
				{
					if(line.charAt(line.length() - 1) != ']')
						continue;
					if(line.length() < 2)
						continue;
					line = line.substring(1, line.length() - 1).trim();
					if(line.equals(""))
						continue;
					if(line.equals(section.toLowerCase()))
					{
						// section trouvee
						sectionFound = true;
					}
				}
				else if(sectionFound && !paramFound)
				{
					int i = line.indexOf("=");
					if(i == -1)
						continue;
					if(line.substring(0, i).trim().equals(param.toLowerCase()))
					{
						// parametre trouve
						paramFound = true;
					}
				}
			}
			f.close();
			BufferedWriter w = new BufferedWriter(new FileWriter(nom));
			BufferedReader r = new BufferedReader(new StringReader(tmp));
			boolean paramEcrit = false;
			boolean dansBonneSection = false;
			boolean premiereLigne = true;
			boolean commentaireAvant = false;
			if(section == null || section.equals(""))
			{
				// ajout avant la 1ere section
				dansBonneSection = true;
			}
			line = new String("");
			for(;;)
			{
				line = r.readLine();
				if(line == null)
					break;
				String linetmp = new String(line.trim());
				line = line.toLowerCase().trim();
				// ligne vide
				if(line.equals(""))
					continue;
				// section
				if(line.charAt(0) == '[')
				{
					if(line.charAt(line.length() - 1) != ']')
						continue;
					if(line.length() < 2)
						continue;
					line = line.substring(1, line.length() - 1).trim();
					if(line.equals(""))
						continue;
					// ceci est une nouvelle section
					// si le parametre n avait pas de section, on l insere maintenant
					if(dansBonneSection && !paramFound && !paramEcrit)
					{
						// ajout en fin de section
						paramEcrit = true;
						w.write(param + "=" + valeur);
						w.newLine();
					}
					// un saut de ligne sauf pour la 1ere ligne
					if(!premiereLigne)
						w.newLine();
					// on a trouve la section
					if(line.equals(section.toLowerCase()))
					{
						dansBonneSection = true;
						linetmp = new String("[" + section + "]");
					}
				}
				else if(sectionFound && paramFound && dansBonneSection && !paramEcrit && line.charAt(0) != ';')
				{
					int i = line.indexOf("=");
					if(i == -1)
						continue;
					if(line.substring(0, i).trim().equals(param.toLowerCase()))
					{
						// substitution
						paramEcrit = true;
						linetmp = new String(param + "=" + valeur);
					}
				}
				if(line.charAt(0) == ';')
				{
					commentaireAvant = true;
				}
				else
					commentaireAvant = false;
				w.write(linetmp);
				w.newLine();
				premiereLigne = false;
			}
			if(!sectionFound)
			{
				// ajout parametr + section en fin de fichier
				w.newLine();
				w.write("[" + section + "]");
				w.newLine();
				w.write(param + "=" + valeur);

			}
			else if(!paramEcrit)
			{
				// ajout du parametre en fin de fichier
				w.write(param + "=" + valeur);
			}
			r.close();
			w.close();
		}
		catch(FileNotFoundException e)
		{
			JOptionPane.showMessageDialog(null, "Erreur lors de l'écriture du fichier" + " :\n" + e.getMessage(),
										  " " + "Erreur", JOptionPane.ERROR_MESSAGE);
			ok = false;
		}
		catch(IOException e)
		{
			JOptionPane.showMessageDialog(null, "Erreur lors de l'écriture du fichier" + " :\n" + e.getMessage(),
										  " " + "Erreur", JOptionPane.ERROR_MESSAGE);
			ok = false;
		}
	}
}

/**
 * thread client
 */
class fin extends Thread
{
	private fmserver sv = null;
	
	public fin(fmserver psv)
	{
		sv = psv;
		this.setPriority(Thread.MIN_PRIORITY);
	}
	
	public void run()
	{
		try
		{
			for(;;)
			{
				if(sv.numTransactions == 0)
					break;
				sleep(100);
			}
			System.exit(0);
		}
		catch(Exception e)
		{
			e.printStackTrace();
		}
	}
}
