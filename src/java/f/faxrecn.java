/*
 * Administration des fax reçus
 * Auteur : Jacques Bailhache 
 * Société Amplitude

 * Septembre 2002
 *
 */
  

import java.lang.*;
import java.util.*;
import java.awt.*;
import java.awt.event.*;
import java.awt.event.*;
import java.awt.print.*;
import java.awt.datatransfer.*;
import java.awt.datatransfer.Clipboard.*;
import javax.swing.*;
import javax.swing.border.*;
import javax.swing.table.*;
import javax.swing.event.*;
import javax.swing.text.*;
import javax.swing.filechooser.*;
import javax.print.attribute.*;
import javax.print.attribute.standard.*;
import javax.print.attribute.standard.PrintQuality.*;

import java.text.*;
import java.io.*;
import java.net.*;
import java.util.zip.*;
import java.sql.*;
import com.java4less.rmail.*;
import com.java4less.rmime.*;

import java.awt.image.*;
import lizard.tiff.Tiff;

import javax.swing.DefaultCellEditor.*;

class MessageInfo
{
	String rep;
	GenericMessage message;
	String[] fields;
	String[] values;
	String[] viewed_values;
	int pos;

	MessageInfo ()
	{
		rep = "";
		message = null;
		fields = new String[0];
		values = new String[0];
	}

	MessageInfo (String str)
	{	
		rep = str;
		message = new GenericMessage(str);
		fields = new String[message.nfields];
		values = new String[message.nfields];
		// System.out.println ("nfields=" + message.nfields);
		for (int i=0; i<message.nfields; i++)
		{
			fields[i] = message.getName(i);
			values[i] = message.getValue(i);
			// System.out.println ("values[" + i + "]=" + values[i]);
		}
	}

	String getField (String field)
	{
		return message.getField(field);
	}
}

class MessageComparator implements Comparator
{
	public boolean compare (Object x, Object y)
	{
		return ((MessageInfo)x).getField("FILE").equals(((MessageInfo)y).getField("FILE"));
	}
}

class ListTableModel extends DefaultTableModel
{

}

class ViewTableModel extends AbstractTableModel
			{
				final int nc = 4;
				final int nr = 40;
				String[][] data;
				public ViewTableModel()
				{
					data = new String[nr][nc];
					for (int i=0; i<nr; i++)
					for (int j=0; j<nc; j++)
						data[i][j] = "";
				}
				public int getRowCount ()
				{
					return nr;
				}
				public int getColumnCount ()
				{
					return nc;
				}
				public String getColumnName (int i)
				{
					switch (i)
					{
						case 0: return "Champ";
						case 1: return "Taille";
						case 2: return "Libellé";
						case 3: return "Script"; 
						default: return "???";
					}
				}
				public Class getColumnClass (int i)
				{
					return String.class;
					/*
					switch (i)
					{
						case 0: return String.class;
						case 1: return Integer.class;
						case 2: return String.class;
						case 3: return String.class;
						default: return String.class;
					}
					*/	
				}
				public boolean isCellEditable (int i, int j)
				{
					return true;
				}
				public Object getValueAt (int i, int j)
				{
					return data[i][j];
				}
				public void setValueAt (Object x, int i, int j)
				{
					data[i][j] = x.toString();
				}
				public void addTableModelListener (TableModelListener l)
				{
				}
				public void removeTableModelListener (TableModelListener l)
				{
				}
			}

class ViewTableColumnModel extends DefaultTableColumnModel
{
	public TableColumn getColumn (int i)
	{
		int w;
		switch (i)
		{
			case 0: w=60; break;
			case 1: w=40; break;
			case 2: w=200; break;
			case 3: w=300; break;
			default: w=0; break;
		}
		return new TableColumn (i, w, new DefaultTableCellRenderer(), 
			new DefaultCellEditor(new JTextField()));
	}
}

class Ptr
{
	public int i;
}

/** Administration des fax reçus */
public class faxrecn extends Component {
    
/*
	Socket sock;
	InputStream is;
	OutputStream os;
*/ 
	String configFile = "faxrecn.ini";

	boolean flag_view;
	int nfields_view;
	String[] field_view;
	String[] script_view;
	int[] size_view;
	String[] text_view;

	boolean flag_local = false;
	boolean flag_refresh = true;
		// false; // pour tester, normalement true;
	String fdl = "\"";

	int print_option = 1;
	int print_none = 0;
	int print_direct = 1;
	int print_server = 2;
	int print_lptps = 3;

	/* public String host = "www.service-clients.net";   */
	/* public String host = "172.16.1.12";   */
	/* public String host = "212.198.247.9"; */
	/* public String host = "localhost";*/
	public String host;
	public int port;

	public static faxrecn fr;
	public static update_thread updt;
	public static restart_thread restart;
	ServerConnection sc;

	int viewHeight = 640;

	int maxfax = 10000; /* Nombre maxi de fax reçus affichables 
				à faire : taille du tableau définie dynamiquement */
	String[] listfax;
	Vector vlistfax;
	Vector vvlistfax;
	Vector vreverse;
	Vector vlistfaxd;
	Vector vmessagesd;
	Vector vlistfaxl;
        public JFrame frame /*= new JFrame("Administration des fax reçus")*/ ;

	public String language = "Francais";
	public int lang;
	public static String[] languages = {"Francais", "English", "Deutsche", "Italiano", "Español", "Português"};//, "langsup1", "langsup2", "langsup3", "langsup4", "langsup5"};
	public static String[] drapeaux = {"images/drapeaux/fr.gif", "images/drapeaux/uk.gif", "images/drapeaux/de.gif",
									   "images/drapeaux/it.gif", "images/drapeaux/es.gif", "images/drapeaux/pt.gif"};

	/*int nColumns = 23;*/
	
	/*
	String[] columnNames1 = { "Id", "File", "Cnx", "Dcnx", "Time", "Pge", "Hgn", "Rem", "Read", "Rtr", "Line", "Speed", "Noise", "SignalS", "SignalQ", "Printed", "OCR", "Saved", "Frs_ref", "Frs_sav", "Frs_OCR", "Frs_exp", "Frs_prt" };
	
	String[] columnNames1 = { "ID", "FILE", "CNX", "PGENB", "SUBJECT", "IMPORTANCE" };
	*/

	String[] columnNames1 = { "???" };
	
	String[] columnNames = columnNames1;

	int nColumns = columnNames1.length;

	String icondir = "../../icones/"; /* "c:/jacques/icones/"; */

      Object[][]tabledata;
	DefaultTableModel model;
	// final ListTableModel model;

	ExtVector messages;
	ExtVector pile;

	Vector vMessages;
	Vector vMessages1;
	JTable tablefaxrec;
	JLabel statusline;
	
	JScrollPane scrollPane;

	JLabel label;
	JMenu m_file, m_lang, m_admin, m_trt, m_options;
	JMenuItem mi_login, mi_quit, mi_listgroups, mi_listrights, mi_listusers, mi_groups, mi_rights, mi_users, mi_def, mi_listscripts, mi_run,
		mi_options, mi_view;
	JButton btn_details, btn_apercu, btn_view, btn_print, btn_refax, btn_email, btn_save, btn_new, btn_refresh, btn_sort, btn_sortrev, btn_select, btn_undo, btn_delete;

	Vignette vignette;
	int selectedRow;
	JScrollBar vsb;

	String email;

	int nfaxrec;

	String basefax, login, password;

	static Textes textes;

	final ViewTableModel vtm = new ViewTableModel();
	final Ptr nFieldView = new Ptr();

	final JPanel pane = new JPanel();

	public static String [][] noms_champs1 =
		/* Français Anglais */
	{ { "Identifiant", "Identifier" },
	  { "Fichier", "File" },
	  { "Heure de connexion", "Connection time" },
	  { "Heure de déconnexion", "Disconnection time" },
	  { "Durée de transmission", "Transmission time" },
	  { "Nombre de pages", "Number of pages" },
	  { "Statut", "Status" },
	  { "REM", "REM" },
	  { "Lu", "Read" },
	  { "Routeur", "Routeur" },
	  { "Ligne", "Line" },
	  { "Vitesse", "Speed" },
	  { "Bruit", "Noise" },
	  { "Force du signal", "Signal strength" },
	  { "Qualité du signal", "Signal quality" },
	  { "Imprimé", "Printed" },
	  { "OCR", "OCR" },
	  { "Saved", "Sauvegardé" },
	  { "FRS_REF", "FRS_REF" },
	  { "FRS_SAV", "FRS_SAV" },
	  { "FRS_OCR", "FRS_OCR" },
	  { "FRS_EXP", "FRS_EXP"},
	  { "FRS_PRT", "FRS_PRT" } };
	
	/*public static String[][] noms_champs;*/

	void trace (String str)
	{
		System.out.println (str);
	}

int index1 (String s)
	{
		/*
		for (int i=0; i<fieldnames.length; i++)
		{
			if (fieldnames[i].equals(s))
				return i;
		}*/
		for (int i=0; i<nColumns; i++)
		{
			/*System.out.println("column " + i + ":" + columnNames[i]);*/
			if (columnNames[i].equals(s))
				return i+1;
		}

		return 0;
	}

int fieldnum1 (int i)
{
	String field = columnNames[i];
	int j;
	for (j=0; j<nfields_view; j++)
	{
		if (field_view[j].equals(field))
			return j;
	}
	System.out.println ("Field " + i + "=" + field + " not found");
	return -1;
}

int fieldnum (int i)
{
	String field = field_view[i];
	int j;
	for (j=0; j<nColumns; j++)
	{
		if (columnNames[j].equals(field))
			return j+1;
	}
	System.out.println ("Field " + i + "=" + field + " not found");
	return -1;
}

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

/**
	Lit dans is un paramètre de type indiqué par filter
	@param is entrée
	@param filter type du paramètre
*/
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
				/*System.out.print(c);*/
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
 
	
	/** 
		Boîte de dialogue demandant le nom de l'hôte, le port, le nom de chemin de la base de fax,
		le login et le mot de passe
	*/	  
	public void login ()
	{
                Frame f = new Frame ();
                JDialog dlg = new JDialog (f, "Identification");
		dlg.setResizable (true);
		/*dlg.getContentPane().setLayout (new GridLayout(3,2));*/
		dlg.getContentPane().setLayout(null);

		dlg.setSize (350, 340);

		JLabel l0 = new JLabel ("Host : ");
		l0.setBounds(30,40,90,30);
		dlg.getContentPane().add(l0);

		JTextField tfhost = new JTextField(30);
		tfhost.setBounds (130,40,150,30);
		dlg.getContentPane().add(tfhost);

		JLabel lport = new JLabel ("Port : ");
		lport.setBounds (30, 80, 90, 30);
		dlg.getContentPane().add(lport);
		
		JTextField tfport = new JTextField (30);
		tfport.setBounds (130, 80, 150, 30);
		dlg.getContentPane().add(tfport);

		JLabel lbase = new JLabel ("Base : ");
		lbase.setBounds(30,120,90,30);
		dlg.getContentPane().add(lbase);

		JTextField tfbase = new JTextField(30);
		tfbase.setBounds(130,120,150,30);
		dlg.getContentPane().add(tfbase);

		JLabel l1 = new JLabel ("Login : ");
		l1.setBounds(30,160,90,30);
		dlg.getContentPane().add (l1); 		 

		JTextField tflogin = new JTextField(30);
		tflogin.setBounds (130,160,150,30);
		dlg.getContentPane().add (tflogin);

		JLabel l2 = new JLabel ("Password : ");
		l2.setBounds (30,200,90,30);
		dlg.getContentPane().add (l2);

		JPasswordField pwpassword = new JPasswordField(30);
		pwpassword.setBounds (130,200,150,30);
		pwpassword.setEchoChar('*');
		dlg.getContentPane().add (pwpassword);

		JButton btnok = new JButton ("OK");
		btnok.setBounds (90,250,100,30);
		btnok.addActionListener(new devt(dlg));
		dlg.getContentPane().add (btnok);

		dlg.setModal(true);
		dlg.show();
		host = tfhost.getText();
		basefax = tfbase.getText();
		login = tflogin.getText();
		password = pwpassword.getText();

		String s_port = tfport.getText();
		Integer I;
		try { I = new Integer(s_port); }
		catch (Exception e) { I = new Integer(6020); }
		port = I.intValue();

		System.out.println ("host=" + host + " port=" + port + " base=" + basefax + " login=" + login);
 
		sc = new ServerConnection (host, port);
		System.out.println ("ServerConnection created");

		boolean status = sc.login (basefax, login, password);
		System.out.println ("Logged in");

		columnNames = sc.aField;
		System.out.println ("(1) col0:" + columnNames[0]);
		nColumns = columnNames.length;
		System.out.println ("Number of columns: " + nColumns);
		sc.logout();
		/*noms_champs = new String[sc.nField][2];*/
			
		/*
						scrollPane.remove(tablefaxrec);
						tabledata = new Object[nfax][24];
						tablefaxrec = new JTable (tabledata, columnNames);
						scrollPane.add (tablefaxrec);
		*/
		/*remove_frame();*/
		/*setModel();*/

	}


      public static void main (String args[])
	{
		textes = new Textes();
		/*login();*/
		/*faxrecn fr = new faxrecn (host, base, login, password);*/

		// fr = new faxrecn (true, false, 0, new String[0], new int[0], new String[0]);
		// fr = new faxrecn (true, "", 0, "", "", "", false, 0, null, null, null);


 					   try
					   {



						String filename = "view.txt";
 						BufferedReader b = new BufferedReader( new  InputStreamReader( new FileInputStream(filename))); 
						int i, j;
						int nr=40; int nc=4;
						String[][] data = new String[nr][nc];
						for (i=0; i<nr; i++)
						for (j=0; j<nc; j++)
							data[i][j] = "";
						String line;	
						i=0;					
						while ((line = b.readLine()) != null)
						{
							System.out.println ("line read: " +  line);
 							String[] field = line.split("\t");
							for (j=0; j<nc; j++)
                                                        {
                                                                try {
                                                                        data[i][j] = field[j];
                                                                } catch (Exception exc) {
                                                                        data[i][j] = "";
                                                                }
                                                        }
							i++;
	
						}
						int nfieldview = i;
						// dlg.repaint();
						b.close();
							
							String[] champ = new String[nr];
							int[] taille = new int[nr];
							String[] libelle = new String[nr];
							String[] script = new String[nr];
							for (i=0; i<nr; i++)
							{
								champ[i] = data[i][0];
								Integer I;
								try { I = new Integer  (data[i][1]); }
								catch (Exception e1)
								{ I = new Integer(0); }
								taille[i] = I.intValue();
								libelle[i] = data[i][2];
								script[i] = data[i][3];
							}
							

							// System.out.println ("creating  faxrecn, host=" + host + " port=" + port + " base=" + basefax + " login=" + login + " nr=" +  vtm.nr + " nFieldView.i=" + nFieldView.i + " taille 0=" + taille[0]);
							
							faxrecn fr = new faxrecn (true, "",  0, "", "", "", true, nfieldview, champ, taille, libelle, script);
							
					   }
					   catch (Exception e)
					   {
						e.printStackTrace();
					   }




		System.out.println ("faxrecn created");

		System.out.println ("Creating update_thread");
		updt = new update_thread (fr);
		System.out.println ("Starting");
		updt.start();
		System.out.println ("update_thread started");

		restart = new restart_thread (fr);
		restart.start();
	}

	public void remove_frame()
	{
		frame.removeAll();
		frame = new JFrame ("Administration des fax reçus");

	}

	/** Affiche une arborescence 
		@param root racine de l'arborescence à afficher
		@param n nombre d'éléments du tableau
		@param table tableau de données
		@param pane où afficher l'arborescence
		@param px abcisse
		@param py ordonnée
		@param dx décalage horizontal
		@param dy espacement des lignes
		@param w largeur des labels
		@param h hauteur des labels
	*/
	public int dispTree (String root, int n, String[][] table, Container pane,
		int px, int py, int dx, int dy, int w, int h)
	{
		int x = px;
		int y = py;

		for (int i=0; i<n; i++)
		{
			if (table[i][1].equals(root))
			{
				JLabel l = new JLabel (table[i][0]);
				l.setBounds (x, y, w, h);
				pane.add(l);
				pane.getGraphics().drawLine (x-dx, y-dy+h/2, x-dx, y+h/2);
				pane.getGraphics().drawLine (x-dx, y+h/2, x-1, y+h/2);
				y = dispTree (table[i][0], n, table, pane, x+dx, y+dy, dx, dy, w, h);
			}
		}

		return y;
	}


	/** 
		Boîte de dialogue de création et modification de script de traitement de fax reçu 
		@param pname Nom du script
		@param ppos Position (si plusieurs scripts sont applicables, ils sont exécutés pas ordre de position)
		@param pstatus Indique si le script est actif c'est-à-dire s'il est exécuté automatiquement sur les fax reçus dont les paramètres correspondent
		@param prtr Numéro du routeur, -1 = tous les routeurs
		@param pline Numéro de la ligne, -1 = toutes les lignes
		@param pid Identifiant, chaine vide = tous les identifiants
		@param pcalled Numéro appelé, chaîne vide = tous les numéros
		@param pcalling Numéro appelant, chaîne vide = tous les numéros
		@param pcode Code du script
	*/
	public void defineScript (String pname, String ppos, String pstatus, String prtr, String pline, String pid, String pcalled, String pcalling, String pcode)
      {
				System.out.println ("defineScript code=" + pcode);
				String titre;
				if (language.equals("Francais"))
					titre = "Définition d'un traitement des fax reçus";
				else
					titre = "Definition of received faxes treatment";
				final MyDialog dlg = new MyDialog (frame, titre, true);
				dlg.setResizable (true);
				dlg.getContentPane().setLayout(null);
				dlg.setSize (650, 580);

				JLabel l_name = new JLabel (language.equals("Francais")?"Nom":"Name");
				l_name.setBounds (30, 20, 80, 20);
				dlg.getContentPane().add (l_name);

				JTextField tf_name = new JTextField (10);
				tf_name.setBounds (120, 20, 100, 20);
				tf_name.setText (pname);
				dlg.getContentPane().add (tf_name);

				JLabel l_pos = new JLabel (language.equals("Francais")?"Position":"Position");
				l_pos.setBounds (260, 20, 80, 20);
				dlg.getContentPane().add (l_pos);
				
				JTextField tf_pos = new JTextField (10);
				tf_pos.setBounds (350, 20, 60, 20);
				tf_pos.setText (ppos);
				dlg.getContentPane().add (tf_pos);

				JCheckBox cb_act = new JCheckBox (language.equals("Francais")?"Actif":"Active");
				cb_act.setBounds (450, 20, 70, 20);
	
				/* convertir la chaîne pstatus en entier status */
                        int status;
                        try { status = new Integer(pstatus).intValue(); }
                        catch (Exception e1)
                        { status = 0; }

				/* bit 0 = flag actif */
				cb_act.setSelected ((status & 1) != 0);
				dlg.getContentPane().add (cb_act);
                         
				JLabel l_rtr = new JLabel (language.equals("Francais")?"Routeur":"Router");
				l_rtr.setBounds (30, 50, 80, 20);
				dlg.getContentPane().add (l_rtr);

				JTextField tf_rtr = new JTextField (10);
				tf_rtr.setBounds (120, 50, 100, 20);
				tf_rtr.setText (prtr);
				dlg.getContentPane().add (tf_rtr);

				JLabel l_line = new JLabel (language.equals("Francais")?"Ligne":"Line");
				l_line.setBounds (270, 50, 80, 20);
				dlg.getContentPane().add (l_line);

				JTextField tf_line = new JTextField (10);
				tf_line.setBounds (360, 50, 100, 20);
				tf_line.setText(pline);
				dlg.getContentPane().add (tf_line);

				JLabel l_id = new JLabel ("Id");
				l_id.setBounds (30, 80, 80, 20);
				dlg.getContentPane().add (l_id);
			
				JTextField tf_id = new JTextField (120);
				tf_id.setBounds (120, 80, 300, 20);
				tf_id.setText(pid);
				dlg.getContentPane().add (tf_id);

				JLabel l_called = new JLabel (language.equals("Francais")?"Numéro appelé":"Called number");
				l_called.setBounds (30, 110, 120, 20);
				dlg.getContentPane().add (l_called);

				JTextField tf_called = new JTextField (40);
				tf_called.setBounds (160, 110, 120, 20);
				tf_called.setText(pcalled);
				dlg.getContentPane().add (tf_called);

				JLabel l_calling = new JLabel (language.equals("Francais")?"Numéro appelant":"Calling number");
				l_calling.setBounds (30, 140, 120, 20);
				dlg.getContentPane().add (l_calling);

				JTextField tf_calling = new JTextField (40);
				tf_calling.setBounds (160, 140, 120, 20);
				tf_calling.setText(pcalling);
				dlg.getContentPane().add (tf_calling);

				JLabel l_trt = new JLabel (language.equals("Francais")?"Traitement":"Treatment");
				l_trt.setBounds (30, 170, 120, 20);
				dlg.getContentPane().add (l_trt);

				final JTextArea ta_script = new JTextArea (10, 40);
				JScrollPane sp = new JScrollPane(ta_script);
				sp.setBounds (30, 200, 500, 250);
				ta_script.setText(pcode);
				dlg.getContentPane().add (sp);				

				JButton btn_modif = new JButton (language.equals("Francais")?"Modifier":"Modify");
				/*btn_modif.setBounds (160, 170, 90, 20);*/
				btn_modif.setBounds (130, 470, 80, 30);
				btn_modif.addActionListener (new ActionListener ()
					{
						public void actionPerformed (ActionEvent e)
						{
							try
							{
								String text = ta_script.getText();
								
								Clipboard cb = getToolkit().getSystemClipboard();
								/*
								StringSelection sel = new StringSelection(text);
								cb.setContents (sel, sel);
								*/
								/* Appel du générateur de code */
								Wizard w = new Wizard(ta_script.getText(),new CodeReceiver(ta_script));
								/* w.f.show(); */
								 
								/*
								Transferable data = cb.getContents (this);
								String text1 = (String) data.getTransferData(DataFlavor.stringFlavor);
								ta_script.setText (text);
								*/
							}
							catch (Exception e1)
							{
								System.out.println(e1);
							}
						}
					});
				dlg.getContentPane().add (btn_modif);

				JButton btn_send = new JButton (language.equals("Francais")?"Envoyer":"Send");
				btn_send.setBounds (30, 470, 80, 30);				
				btn_send.addActionListener (new ActionListener ()
					{
						public void actionPerformed (ActionEvent e)
						{
							dlg.ok = true;
							dlg.dispose();
						}
					});
				dlg.getContentPane().add (btn_send);

				JButton btn_copy = new JButton (language.equals("Francais")?"Copier":"Copy");
				btn_copy.setBounds (230, 470, 80, 30);
				btn_copy.addActionListener (new ActionListener ()
					{
						public void actionPerformed (ActionEvent e)
						{
							try
							{
								String text = ta_script.getSelectedText();
								Clipboard cb = getToolkit().getSystemClipboard();
								StringSelection sel = new StringSelection(text);
								cb.setContents (sel, sel);
							}
							catch (Exception e1)
							{
								System.out.println (e1.getMessage());
							}
						}
					});
				dlg.getContentPane().add (btn_copy);

				JButton btn_paste = new JButton (language.equals("Francais")?"Coller":"Paste");
				btn_paste.setBounds (330, 470, 80, 30);
				btn_paste.addActionListener (new ActionListener ()
					{
						public void actionPerformed (ActionEvent e)
						{
							try
							{
								Clipboard cb = getToolkit().getSystemClipboard();
								Transferable data = cb.getContents (this);
								String text = (String) data.getTransferData(DataFlavor.stringFlavor);
								ta_script.replaceRange (text, ta_script.getSelectionStart(), ta_script.getSelectionEnd());
							}
							catch (Exception e1)
							{
								System.out.println (e1.getMessage());
							}
						}
					});
				dlg.getContentPane().add (btn_paste);

				dlg.show();

				if (!dlg.ok)
					return;

				String nom = tf_name.getText();
				String pos = tf_pos.getText();
				String s_status;
				if (cb_act.isSelected())
					s_status = "1";
				else
					s_status = "0";

				System.out.println ("modif script: nom=" + nom + " pos=" + pos + " status=" + status);

				// récupération des données de la boîte de dialogue 
				String rtr = tf_rtr.getText();
				if (rtr.equals("")) rtr = "-1"; /* -1 = tous les routeurs */
				String line = tf_line.getText();
				if (line.equals("")) line = "-1";
				String id = tf_id.getText();
				String called = tf_called.getText();
				String calling = tf_calling.getText();
				String script = ta_script.getText();

				System.out.println ("code: " + script);
		
				// connexion au serveur 
                        boolean status1 = sc.login (basefax, login, password);
			
				// envoi au serveur de la commande de définition de script 
				String commande = "SCRIPT\n" + nom + "\n" + pos + "\n" + s_status + "\n" + rtr + "\n" + line + "\n" + id + "\n" + called + "\n" + calling + "\n" + script.replace('\n','$') + "\n" /*+ "ENDSCRIPT\n"*/;
				System.out.println ("commande: " + commande);
				sc.output(commande);

				String reponse = sc.input ("any");
				System.out.println ("réponse: " + reponse);

				if (reponse.equals ("NOTAUTH"))
				{
					JOptionPane.showMessageDialog(null, language.equals("English")?"You are not authorized to define this treatment.":"Vous n'êtes pas autorisé à définir ce traitement", "Administration", JOptionPane.WARNING_MESSAGE); 
				}
			 	else if (reponse.equals ("OK"))
				{
					JOptionPane.showMessageDialog(null, language.equals("English")?"Treatment definition saved.":"Définition du traitement enregistrée." , "Administration", JOptionPane.WARNING_MESSAGE); 
				}
				else /*if (reponse.equals ("ERROR"))*/
				{
					JOptionPane.showMessageDialog(null, language.equals("English")?"Error defining treatment":"Erreur dans la définition du traitement", "Administration", JOptionPane.WARNING_MESSAGE); 
				}

	}

	public void tablefaxrec_selection ()
	{
		tablefaxrec.setSelectionMode(ListSelectionModel.MULTIPLE_INTERVAL_SELECTION);

		ListSelectionModel rowSM = tablefaxrec.getSelectionModel();
		rowSM.addListSelectionListener(new ListSelectionListener() {
    			public void valueChanged(ListSelectionEvent e) {
        			//Ignore extra messages.
        			if (e.getValueIsAdjusting()) return;
        
        			ListSelectionModel lsm =
            			(ListSelectionModel)e.getSource();
        			if (lsm.isSelectionEmpty()) {
            				//no rows are selected
        			} else {
            				selectedRow = lsm.getMinSelectionIndex();
			            	//selectedRow is selected
					System.out.println ("Row " + selectedRow + " selected");
					vignette.repaint();
					
        				}
			 
    			}
		});

	}

	public void setModel1 ()
	{

	model = new DefaultTableModel();
	vMessages = new Vector();
	vmessagesd = new Vector();

	// Liste des fax reçus
	tablefaxrec = new JTable(model);
	model.addColumn("Media");
	for (int c=0; c<nColumns; c++)
		model.addColumn(columnNames[c]);
	tablefaxrec_selection();

	}

	public void setModel ()
	{

	model = new DefaultTableModel();
	vMessages = new Vector();
	vmessagesd = new Vector();
	System.out.println ("vmessagesd=" + vmessagesd);

	// Liste des fax reçus
	tablefaxrec = new JTable(model);
	// model.addColumn("Media");
	for (int c=0; c<nfields_view; c++)
		model.addColumn(field_view[c]);

	tablefaxrec_selection();

	}



	public String[] getFields (String reponse)
	{

							String[] field = new String[nColumns];
							GenericMessage m = new GenericMessage(reponse);
							/*System.out.println ("Message: " + m.XML());*/
							int j;
							for (j=0; j<nColumns; j++)
							{
								field[j] = (String)m.values.elementAt(j+1);
								//System.out.print ("" + j + ":" + field[j] + "\t");
							}
							System.out.println ("");
							nColumns = m.nfields - 1;
							System.out.println ("nColumns="+nColumns);
							columnNames = new String[m.nfields-1];
							for (j=0; j<m.nfields-1; j++)
								columnNames[j] = (String)m.names.elementAt(j+1);

							return field;
	}

	void saveVector (Vector v, String filename, boolean append)
	{
	   try
	   {
		FileWriter fw = new FileWriter (filename, append);
		BufferedWriter bw = new BufferedWriter (fw);
		PrintWriter pw = new PrintWriter (bw);
		for (int i=0; i<v.size(); i++)
		{
			/*System.out.println ("Saving: " + v.elementAt(i));*/
			pw.println (v.elementAt(i));
			pw.flush();
		}
	   }
	   catch (Exception e)
	   {
		e.printStackTrace();
	   }
	}

	Vector loadVector (String filename)
	{
		Vector v = new Vector ();
		try
		{
			File f = new File (filename);
			FileInputStream is = new FileInputStream (f);
			InputStreamReader isr = new InputStreamReader (is);
			BufferedReader br = new BufferedReader (isr);
 			while (true)
			{
				try 
				{
					String line = br.readLine();
					// System.out.println(line);
					if (line == null)
						break;
					v.add(line);
				}
				catch (Exception e)
				{
					break;
				}
			}
		
		}
		catch (Exception e)
		{
			e.printStackTrace();
		}
		return v;
	}

	void saveMessages (ExtVector v, String filename, boolean append)
	{
		ExtVector w = v.map (new Mapper () {
			public Object map (Object x)
			{
				return ((MessageInfo)x).rep;
			} });
		saveVector (w, filename, append);
	}


	java.util.Date lastloaded;

	ExtVector loadMessages (String filename)
	{
		lastloaded = null;
		java.util.Date date;
		Vector v = loadVector (filename);
		ExtVector r = new ExtVector ();
		for (int i=0; i<v.size(); i++)
		{
			// System.out.println (v.elementAt(i));
			MessageInfo mi = new MessageInfo((String)v.elementAt(i));
			r.add(mi);	
			date = Util.dateStr(mi.getField("DCNX"));
                        if (lastloaded == null)
                                lastloaded = date;
                        else if (date == null)
                                {}
                        else if (date.after(lastloaded))
				lastloaded = date;		
		}
		System.out.println ("" + r.size() + " messages loaded");

		return r;

	}

	java.util.Date last_refresh;
	int nfax;

	public ExtVector getMessages (String param, java.util.Date since)
	{
		ExtVector newmessages = new ExtVector();
		boolean status = sc.login (basefax, login, password);
		columnNames = sc.aField;
		nColumns = columnNames.length;

		java.util.Date lastfax = null;

		if (!status)
		{
			System.out.println ("Bad login");
		}
		else
		{
			String depuis = "";
			if (since != null)
			try {
			depuis = Util.strDate(since);
			} catch (Exception e) {
				System.out.println(e.toString());
			}

			String commande = "LISTREC\n" + param + "\n" + depuis + "\n";
			System.out.println (commande);
			sc.output (commande);
			String reponse = sc.input ("any");
			reponse = sc.input ("any");
			System.out.println ("Reponse:" + reponse);
			if (reponse.equals ("NOTAUTH"))
			{
				statusline.setText ("Not authorized");
			}
			else
			{
						for (int i=0; ; i++)
						{
							reponse = sc.input ("any");
							trace(reponse); 
							if (reponse.equals(""))
							{
								System.out.println ("Fin liste");
								break;
							}
					
							/* ajouter le message dans newmessages */
							MessageInfo mi = new MessageInfo ();
							mi.rep = reponse;
							mi.message = new GenericMessage (reponse);
							mi.fields = columnNames;
							mi.values = new String[nColumns];
							// mi.viewed_values = new String[vtm.nr];


                                                        // System.out.println ("(3) nColumns=" + nColumns);
							for (int j=0; j<nColumns; j++)
							{
                                                                try {
								mi.values[j] = (String)mi.message.values.elementAt(j);
                                                                } catch (Exception e) {}

                                                // System.out.println ("" + j + ": " + mi.values[j] + "\n");
							}
							newmessages.add (mi);
 							
	
							/*
							listfax[nfax++] = reponse;
							if (vlistfax.indexOf(reponse) == -1)							
								vlistfax.addElement(reponse);
 							
							String[] field = getFields(reponse);
							*/

							boolean er = false;
							java.util.Date datefax = null;
							try
							{
								/*datefax = Util.dateStr (field[3]);*/
								datefax = Util.dateStr (mi.message.getField("DCNX"));
								if (lastfax == null)
									lastfax = datefax;
								else if (datefax.after(lastfax))
									lastfax = datefax;
							}
							catch (Exception e1)
							{
								er = true;
							}
							

						}

			}
			if (lastfax == null)
				last_refresh = new java.util.Date(new java.util.Date().getTime() - 900000 /* 15 minutes */);
			else
				last_refresh = lastfax;
			System.out.println ("lastfax=" + lastfax + " last_refresh=" + last_refresh);
			sc.logout();


		}
		return newmessages;
	}
	

	/** Demande au serveur la liste des fax reçus
		@param param Paramètre de la commande SELECT
		@param newonly Demande seulement les nouveaux fax 
	*/
	public Vector getlistfax (String param, boolean newonly)
	{
				String reponse;

				java.util.Date lastfax = null;
				String depuis = "";
				/*Vector vlistfax = null;				*/
				boolean status = sc.login (basefax, login, password);
				columnNames = sc.aField;
				nColumns = columnNames.length;
				System.out.println ("(2) Number of columns: " + nColumns);

				if (status)
				{
				   	String commande;
						
				   	int h = 0;
					listfax = new String[maxfax];
					nfax = 0;
					vlistfax = new Vector();
					int i;
					GenericMessage message;

				   	if (flag_local)
				   	{
						
						for (i=0; i<maxfax; i++)
						{
							/* reponse = sc.input ("any");*/
							if (h >= vlistfaxl.size())
							{
								System.out.println ("fin liste");
								break;
							}
							reponse = (String)(vlistfaxl.elementAt(h++));							
							/*System.out.println ("reponse lue:" + reponse);*/
							message = new GenericMessage(reponse);
							if (reponse.equals(""))
							{
								System.out.println ("Fin liste");
								break;
							}
							listfax[nfax++] = reponse;
							if (vlistfax.indexOf(reponse) == -1)
							{
								vlistfax.addElement(reponse);
								/*System.out.println("ajoute fax lu: "  + reponse);*/
							}

							/* String[] field = reponse.split("\t"); */

							/*
							String[] field = new String[23];
							GenericMessage m = new GenericMessage(reponse);
							System.out.println ("Message: " + m.toString());							
							for (int j=0; j<23; j++)
							{
								field[j] = (String)m.values.elementAt(j+1);
								System.out.print ("" + j + ":" + field[j] + "\t");
							}
							System.out.println ("");
							*/
							String[] field = getFields(reponse);
			
							boolean er = false;
							java.util.Date datefax = null;
							try
							{
								/*datefax = Util.dateStr (field[3]);*/
								datefax = Util.dateStr (message.getField("CNX"));

								if (lastfax == null)
									lastfax = datefax;
								else if (datefax.after(lastfax))
									lastfax = datefax;
							}
							catch (Exception e1)
							{
								er = true;
							}

						}
 						try {
						depuis = Util.strDate(lastfax);
						} catch (Exception e) {}
						

				   }
				   else
				   {
					if (newonly)
					{
						try {
						depuis = Util.strDate(last_refresh);
						} catch (Exception e) {}
					}
				   }

				   {
					commande = "LISTREC\n" + param + "\n";

					/*if (newonly)
                                        {
                                                try {
                                                commande += Util.strDate(last_refresh);
                                                } catch (Exception e) {}
                                        }*/
					commande += depuis;
              
					commande += "\n";
					sc.output (commande);
					/*String*/ reponse = sc.input ("any");
					reponse = sc.input ("any");
					System.out.println ("Reponse:" + reponse);
					if (reponse.equals ("NOTAUTH"))
      				{
						statusline.setText ("Not authorized");
					}
					else
					{
						/*
						reponse = sc.input ("any");
						statusline.setText (reponse);
						*/
						/*
						listfax = new String[maxfax];
						nfax = 0;
						vlistfax = new Vector();
						int i;
						*/
						for (i=0; i<maxfax; i++)
						{
							reponse = sc.input ("any");
							/*System.out.println ("reponse:" + reponse);*/
							if (reponse.equals(""))
							{
								System.out.println ("Fin liste");
								break;
							}
							listfax[nfax++] = reponse;
							if (vlistfax.indexOf(reponse) == -1)
							{
								vlistfax.addElement(reponse);
								/*System.out.println ("ajoute fax recu: " + reponse);*/
							}

							/* String[] field = reponse.split("\t"); */

							/*
							String[] field = new String[23];
							GenericMessage m = new GenericMessage(reponse);
							System.out.println ("Message: " + m.toString());							
							for (int j=0; j<23; j++)
							{
								field[j] = (String)m.values.elementAt(j+1);
								System.out.print ("" + j + ":" + field[j] + "\t");
							}
							System.out.println ("");
							*/
							String[] field = getFields(reponse);
			
							boolean er = false;
							java.util.Date datefax = null;
							try
							{
								datefax = Util.dateStr (field[3]);
								if (lastfax == null)
									lastfax = datefax;
								else if (datefax.after(lastfax))
									lastfax = datefax;
							}
							catch (Exception e1)
							{
								er = true;
							}

						}
						/*setModel();*/
					}
					} /* if (flag_local) */
					if (lastfax == null)
						last_refresh = new java.util.Date(new java.util.Date().getTime() - 900000 /* 15 minutes */);
					else
						last_refresh = lastfax;
					System.out.println ("lastfax=" + lastfax + " last_refresh=" + last_refresh);
					sc.logout();
				}
				return vlistfax;
	}

	boolean _reverse;

	Vector reverseVect (Vector v)
	{
		Vector r = new Vector();
		for (int i=0; i<v.size(); i++)
			r.add (v.elementAt(v.size()-1-i));
		return r;
	}

	public void displayMessages1 (Vector messages)
	{
		model.setRowCount(0);
		for (int i=0; i<messages.size(); i++)
		{
			model.addRow (new String[nColumns]);
			// System.out.println (" nColumns=" + nColumns);
			for (int j=0; j<nColumns; j++)
			{
				MessageInfo mi = (MessageInfo)(messages.elementAt(i));
                                try { tablefaxrec.setValueAt (mi.values[j], i, j); }
                                catch (Exception e)
                                {
                                        try { tablefaxrec.setValueAt ("???", i, j); }
                                        catch (Exception e1)
                                        {
                                                // System.out.println ("");
                                                System.out.println (e1.toString());
                                                //e1.printStackTrace();
                                        }
                                }
			}
		}
	}

	public String formatField (String script, String field, MessageInfo mi)
	{
		// return "<" + script + "/" + field + ">";
		if (script.equals(""))
			return field;
		else if (script.charAt(0) == '/')
		{
			String[] sf = script.split ("/");
			String[] ff = field.split (sf[1]);
			Integer I;
			try { 
				I = new Integer (sf[2]);
				return ff[I.intValue()];
			} catch (Exception e) {
				return field;
			}
		}
		else
		{
			interp intrp = new interp();
			String m = mi.message.iaRep();
			String src = " { " + m + " \"" + field + "\" " + script + " } ";
			// System.out.println ("src = <" + src + ">");
			entree e = new entree(src);
			expr top = e.lirexpr();
			expr global = new expr ("GLOBAL", 10);
			intrp.global = global;
			expr r = intrp.run (top, new expr("x"));
			String res = r.chaine;
			System.out.println ("res = <" + res + ">");
			return res;
		}		
	}

	public void displayMessages (Vector messages)
	{
		model.setRowCount(0);
		for (int i=0; i<messages.size(); i++)
		{
			model.addRow (new String[nfields_view]);
			// System.out.println (" nColumns=" + nColumns);

			// vmessagesd.add (i, messages.elementAt(i));

			MessageInfo mi = (MessageInfo)(messages.elementAt(i));
			mi.viewed_values = new String[nfields_view];
			mi.pos = i;

			for (int j=0; j<nfields_view; j++)
			{
				
                                // try { tablefaxrec.setValueAt (mi.values[fieldnum(j)], i, j); }
                                // System.out.println ("script_view[" + j + "] = \"" + script_view[j] + "\"");                                                                                                
                                try { 
						String f = formatField (script_view[j], mi.values[fieldnum(j)], (MessageInfo)(messages.elementAt(i)));
						tablefaxrec.setValueAt (f, i, j); 
						mi.viewed_values[j] = f;
						mi.pos = i;
					  }
                                catch (Exception e)
                                {
                                        try { tablefaxrec.setValueAt ("???", i, j); }
                                        catch (Exception e1)
                                        {
                                                // System.out.println ("");
                                                System.out.println (e1.toString());
                                                //e1.printStackTrace();
                                        }
                                }
			}
			messages.set (i, mi);
		}
	}

	/** 
		Affichage de la liste de fax 
	*/
	public void displaylistfax1 (Vector vlistfax, /*Vector vMessages1,*/ boolean reverse, boolean newonly)
	{
						if (vlistfaxd == null)
							vlistfaxd = vlistfax;
						_reverse = reverse;
						int i;
						String reponse;
						/*vlistfaxd = new Vector();*/						
						/* 
						scrollPane.remove(tablefaxrec);
						tabledata = new Object[nfax][24];
						tablefaxrec = new JTable (tabledata, columnNames);
						scrollPane.add (tablefaxrec);
						*/

						/*DefaultTableModel model = (DefaultTableModel)(tablefaxrec.getModel());*/
						/*
						try
						{
							for (i=0; ; i++)
								model.removeRow(i);
						}
						catch (Exception e1) 
						{
							System.out.println (" " + i + "rows removed, " + e1.getMessage());
						}
						*/
						if (!newonly)
						{
							model.setRowCount(0);
							vMessages = new Vector();
							vmessagesd = new Vector();
							vlistfaxd = new Vector();
							
						}
						int nfax = vlistfax.size();
						for (i=0; i<nfax; i++)
						{ 
						   reponse = (String)(vlistfax.elementAt(i));	
						   /*GenericMessage message = (GenericMessage)(vMessages1.elementAt(i));													*/
 						   GenericMessage message = new GenericMessage (reponse);

						   /*String[] fields = reponse.split("\t");*/
						   String[] fields = getFields(reponse);					

						   boolean found = false;
						   if (newonly)
						   {							 
							for (int j=0; j<tablefaxrec.getRowCount(); j++)
							{
								/*System.out.println ("col0:" + columnNames[0]);*/
								/*if (fields[1].equals(tablefaxrec.getValueAt(j,1)))*/
								
								/*System.out.println ("fields[" + index1("FILE") + "]=" + fields[index1("FILE")] + " = " + 
									((GenericMessage)(vMessages.elementAt(j))).getField("FILE"));
								*/
								if (fields[index1("FILE")].equals(((GenericMessage)(vMessages.elementAt(j))).getField("FILE")))
								{
									/*System.out.println("oui");*/
									found = true;
									break;
								}
								/*else
									System.out.println("non");*/
							}

						   }
						   if (!found)
						   {	
						
							int i1;
							if (reverse)
							{
								model.insertRow (0, new String[nColumns]);
								vlistfaxd.add (0, reponse);
								i1 = 0;
							}
							else
							{
								model.addRow (new String[nColumns]);
								vlistfaxd.add (reponse);
								i1 = i;
							}
							

							/*System.out.println ("Fax numero " + i + ": " + reponse);*/

                                                        /*System.out.println("nColumns="+nColumns);*/
							vMessages.add (i, message);
							for (int j=0; j<nColumns; j++)
							{
							 
							 if (j == 5 || j == 6)
							 {
							  try
							  {
							   double dp = Double.valueOf(fields[j]).doubleValue();
							   int ip = (int)dp;
							   String sp = "" + ip;
							   tablefaxrec.setValueAt (sp, i1, j);							  
							  }
                                		  catch (Exception e1)
							  {
							  }
							 }
							 else
                                                         {
                                                          /*System.out.println ("cell("+i1+","+j+"):"+fields[j]);*/
							  tablefaxrec.setValueAt (fields[j], i1, j);
                                                         }
		 					}
							/*System.out.println ("reverse=" + reverse);*/
							/*if (reverse)
								vMessages = reverseVect (vMessages);*/

							/*System.out.println ("fin");*/
							/*
							reponse = sc.input("any");
							System.out.println ("reponse: " + reponse);
							*/
						    }
						}
	/*
					}
					sc.logout(); 
				}
		last_refresh = new java.util.Date();
 	*/
	}
	
	
	public void displaylistfax (Vector vlistfax, /*Vector vMessages1,*/ boolean reverse, boolean newonly)
	{
						if (vlistfaxd == null)
							vlistfaxd = vlistfax;
						_reverse = reverse;
						int i;
						String reponse;
						
						if (!newonly)
						{
							model.setRowCount(0);
							vMessages = new Vector();
							vlistfaxd = new Vector();
							vmessagesd = new Vector();
							System.out.println ("vmessagesd=" + vmessagesd);
						}
						int nfax = vlistfax.size();
						for (i=0; i<nfax; i++)
						{ 
						   reponse = (String)(vlistfax.elementAt(i));	
						   GenericMessage message = new GenericMessage (reponse);

						   String[] fields = getFields(reponse);					

						   boolean found = false;
						   if (newonly)
						   {							 
							for (int j=0; j<tablefaxrec.getRowCount(); j++)
							{
								if (fields[index1("FILE")].equals(((GenericMessage)(vMessages.elementAt(j))).getField("FILE")))
								{
									/*System.out.println("oui");*/
									found = true;
									break;
								}
							}
						   }
						   if (!found)
						   {	
						
							int i1;
							System.out.println ("add message");
							if (reverse)
							{
								model.insertRow (0, new String[nfields_view]);
								vlistfaxd.add (0, reponse);
								vmessagesd.add (0, message);
								i1 = 0;
							}
							else
							{
								model.addRow (new String[nfields_view]);
								vlistfaxd.add (reponse);
								vmessagesd.add (message);
								i1 = i;
							}
							

							vMessages.add (i, message);
							for (int j1=0; j1<nfields_view; j1++)
							{
							 int j = fieldnum(j1);
							 if (j == 5 || j == 6)
							 {
							  try
							  {
							   double dp = Double.valueOf(fields[j]).doubleValue();
							   int ip = (int)dp;
							   String sp = "" + ip;
							   tablefaxrec.setValueAt (sp, i1, j1);							  
							  }
                                		  catch (Exception e1)
							  {
							  }
							 }
							 else
                                           {
	 						  tablefaxrec.setValueAt (fields[j], i1, j1);
                                           }
		 					}
						    }
						}
	}
	

	
	public void initial_refresh ()
	{
            messages = new ExtVector();
		ExtVector initial_messages = loadMessages ("faxrecn.txt");
		System.out.println ("Loaded " + initial_messages.size() + " messages");
		ExtVector new_messages = getMessages ("", lastloaded);
		System.out.println ("got " + new_messages.size() + " messages");
		/*initial_messages.append (new_messages, new MessageComparator());*/
		initial_messages.insert (new_messages.reverse(), new MessageComparator());
		System.out.println ("total " + initial_messages.size() + " messages");
		messages = initial_messages/*.reverse()*/;
		/*System.out.println ("reverse " + messages.size() + " messages");*/
		displayMessages(messages);		
		last_refresh = new java.util.Date();
	}

/*
	vlistfaxl = loadVector ("faxrecn.txt");
	_reverse = true;
        System.out.println ("" + vlistfaxl.size() + " fax recus");
	if (vlistfaxl.size() == 0)
	{
		System.out.println ("Chargement depuis le serveur");
		refresh ("", true, false);
	}
	else
	{
		System.out.println ("Chargement en local");
		flag_local = true;
		refresh ("", true, false);
		flag_local = false;
	}
	

	last_refresh = new java.util.Date();
*/

	public void refresh (String param, boolean reverse, boolean newonly)
	{
		System.out.println ("flag_refresh=" + flag_refresh);
		if (!flag_refresh) return;
		flag_refresh = false; // pour éviter 2 refresh simultanés
		try
		{
		System.out.println ("refresh param=" + param + " reverse=" + reverse + " newonly=" + newonly);
		java.util.Date date;
		if (newonly)
			date = last_refresh;
		else
			date = null;
		System.out.println ("date=" + date);
		ExtVector messages1 = getMessages (param, date);
		System.out.println ("got " + messages1.size() + " messages");
                // System.out.println ("0: " + ((MessageInfo)(messages1.elementAt(0))).message);
                // System.out.println ("last: " + ((MessageInfo)(messages1.elementAt(messages1.size()-1))).message);

		if (!newonly)
			messages = new ExtVector();
		System.out.println ("added to " + messages.size() + " messages");
		ExtVector messages2 = messages1.reverse();
		System.out.println ("reverse: " + messages2.size() + " messages");
                // System.out.println ("0: " + ((MessageInfo) (messages2.elementAt(0))).message);
                // System.out.println ("last: " + ((MessageInfo)(messages2.elementAt(messages2.size()-1))).message);
		
		messages.insert (messages2, new MessageComparator());
		System.out.println ("result: " + messages.size() + " messages");
                // System.out.println ("0: " + ((MessageInfo)(messages.elementAt(0))).message);
                // System.out.println ("last: " + ((MessageInfo)(messages.elementAt(messages.size()-1))).message);

		last_refresh = new java.util.Date();
		System.out.println ("gives " + messages.size() + " messages");
		displayMessages(messages);
		saveMessages (messages, "faxrecn.txt", false);
                }
		catch (Exception e)
		{
			e.printStackTrace();
		}
		flag_refresh = true;
	}

	/**
		Demande au serveur la liste des fax et l'affiche
		@parametre param Paramètre de la commande SELECT
		@parametre reserse Affichage de la liste dans l'ordre inverse
		@parametre newonly Ne demande au serveur que les nouveaux fax
	*/
	public void refresh1 (String param, boolean reverse, boolean newonly)
	{
		Vector vlistfax1 = getlistfax (param, newonly);
		displaylistfax (vlistfax1, reverse, newonly);
		if (vlistfax == null)
			vlistfax = new Vector();
		if (_reverse)
			vlistfax.addAll(vlistfax1);
		else
			vlistfax.addAll(0,vlistfax1);
	      saveVector (vlistfax, "faxrecn.txt", newonly);
	}

	public void trier (final int cs, final boolean rev)
	{
				if (cs<0)
				{
					JDialog dlg = new JDialog(frame); /**/
					dlg.getContentPane().setLayout(new FlowLayout());
				      dlg.setSize (300, 100);
					JLabel lbl_message;
					String message;
					/*
					if (language.equals("Francais"))
						message = "Veillez cliquer sur la colonne à trier";
					else
						message = "Please click on the column to sort";
					*/
					message = textes.CliqColTri[lang];

					statusline.setText(message);
					
					lbl_message = new JLabel (message);
					dlg.getContentPane().add (lbl_message);
					/*dlg.show();*/
				} 
				else
				{
					statusline.setText("OK");
				}

				ExtVector messages_tries = messages.sort (new Sorter () {
					public int compare (Object x, Object y)
					{
						// int c = ((MessageInfo)x).values[fieldnum(cs)].compareTo(((MessageInfo)y).values[fieldnum(cs)]);
						int c = ((MessageInfo)x).viewed_values[cs].compareTo(((MessageInfo)y).viewed_values[cs]);
						if (rev)
							c = -c;
						return c;
					}});
				pile.add(0,messages);
				messages = messages_tries;
				displayMessages(messages);
				

	}

	/**
		Tri de la liste des fax
		@param cs Colonne à trier
		@param reverse Ordre décroissant
	*/
	public void trier1 (int cs, boolean reverse)
	{
				if (cs<0)
				{
					JDialog dlg = new JDialog(frame); /**/
					dlg.getContentPane().setLayout(new FlowLayout());
				      dlg.setSize (300, 100);
					JLabel lbl_message;
					String message;
					/*
					if (language.equals("Francais"))
						message = "Veillez cliquer sur la colonne à trier";
					else
						message = "Please click on the column to sort";
					*/
					message = textes.CliqColTri[lang];
					statusline.setText(message);
					lbl_message = new JLabel (message);
					dlg.getContentPane().add (lbl_message);
					/*dlg.show();*/
				} 
				else
				{
					statusline.setText("OK");
				}
				Vector vlistfaxt = new Vector();
				int nfax = vlistfax.size();
				
				for (int i=0; i<nfax; i++)
				{
					/* Insérer le ième élément de vlistfax à la bonne place dans vlistfaxt */
					/*String[] fields = ((String)(vlistfax.elementAt(i))).split("\t");*/
					String[] fields = getFields((String)(vlistfax.elementAt(i))); 
					boolean cont = true;
					for (int j=0; j<i && cont; j++)
					{
						/*String[] fields1 = ((String)(vlistfaxt.elementAt(j))).split("\t");*/
						String[] fields1 = getFields((String)(vlistfaxt.elementAt(j)));
						int comp;
						if (cs == 2 || cs == 3)
						     comp = Util.dateStr(fields[cs]).compareTo(Util.dateStr(fields1[cs]));
						else comp = fields[cs].compareTo(fields1[cs]);
						/*System.out.println ("Comparaison <" + fields[cs] + "> et <" + fields1[cs] + "> -> " + comp);*/
						if (comp < 0)
						{
							vlistfaxt.add (j, vlistfax.elementAt(i));
							cont = false;
						}
					}
					if (cont)
					{
						vlistfaxt.add (vlistfax.elementAt(i));
					}
				}
				displaylistfax (vlistfaxt, reverse, false);
				vvlistfax.add(0,vlistfax);
				vreverse.add(0,new Boolean(reverse));
				vlistfax = vlistfaxt;
			}

	public void setTextLang (int lang)
	{
		// Changement de langue 
				/*setState(true);*/
				frame.setTitle (textes.Admin_fax_rec[lang]);
				/*label.setText("Liste des fax reçus");*/
				m_file.setText (textes.Fichier[lang]);
				m_lang.setText (textes.Langue[lang]);
				mi_login.setText (textes.chgbase[lang]);
				mi_quit.setText(textes.Quitter[lang]);
				mi_listgroups.setText(textes.ListeGroupes[lang]);
				mi_listrights.setText(textes.ListeDroits[lang]);
				mi_listusers.setText(textes.ListeUtilisateurs[lang]);
				mi_groups.setText(textes.ModifierGroupes[lang]);
				mi_rights.setText(textes.ModifierDroits[lang]);
				mi_users.setText(textes.ModifierUtilisateurs[lang]);
				m_trt.setText(textes.Traitement[lang]);
				mi_listscripts.setText(textes.Liste[lang]);
				mi_def.setText(textes.Definir[lang]);
				mi_view.setText(textes.Vues[lang]);
				btn_details.setText(textes.Details[lang]);
				btn_apercu.setText(textes.Vignette[lang]);
				btn_view.setText(textes.Voir[lang]);				
				btn_print.setText(textes.Imprimer[lang]);
				btn_refax.setText(textes.Refaxer[lang]);
				btn_email.setText(textes.EnvoyerParEmail[lang]);
				btn_save.setText(textes.Enregistrer[lang]);
				btn_delete.setText(textes.Supprimer[lang]);
				btn_new.setText(textes.MettreAJour[lang]);
				btn_refresh.setText("Réafficher");
				btn_sort.setText(textes.Trier[lang]);
				btn_sortrev.setText(textes.Trier[lang]);
				btn_select.setText(textes.Selectionner[lang]);
				btn_undo.setText(textes.Annuler[lang]);

	}

	/** Crée la fenêtre principale */
	public void init_frame ()
	{
	  /*nfaxrec = n;*/
	  int width = 600;
	  int height = 500;
        frame = new JFrame ("Administration des fax reçus");
	   
	  frame.setSize (width, height);
        // JPanel pane = new JPanel();
	  /*pane.setSize (width, height);*/
        pane.setBorder(BorderFactory.createEmptyBorder(
                                        10, //top
                                        10, //left
                                        10, //bottom
                                        10) //right
                                        );
	// pane.setLayout (new BorderLayout());
	pane.setLayout (new BoxLayout(pane,BoxLayout.Y_AXIS));
	/*pane.setLayout (new FlowLayout());*/
	// pane.setLayout (new FlowLayout());

	// Création du menu
	JMenuBar mb = new JMenuBar();
 
	System.out.println ("Langue: " + language);
	/*
	if (language.equals("English"))
		m_file = new JMenu ("File");
	else
		m_file = new JMenu ("Fichier");
	*/
 	m_file = new JMenu (textes.Fichier[lang]);
	m_file.setIcon (new ImageIcon (icondir + "Files3.gif"));
	mb.add(m_file);

	mi_login = new JMenuItem ("Changer de base");
	mi_login.addActionListener (new ActionListener ()
		{
			public void actionPerformed (ActionEvent e)
			{
				/*login();*/

		fr = new faxrecn (true, "", 0, "", "", "", false, 0, null, null, null, null);
		System.out.println ("faxrecn created");

		System.out.println ("Creating update_thread");
		updt.stop();
		updt = new update_thread (fr);
		System.out.println ("Starting");
		updt.start();
		System.out.println ("update_thread started");

				refresh ("", true, false);
			}
		});
	m_file.add (mi_login);

	mi_quit = new JMenuItem ("Quitter");
      mi_quit.addActionListener ( new ActionListener ()
		{
			public void actionPerformed (ActionEvent e)
			{
				System.exit(0);
			}
		});

	m_file.add(mi_quit);
	
	m_admin = new JMenu ("Administration");
	m_admin.setIcon (new ImageIcon (icondir + "Homme3.gif"));
	mb.add(m_admin);

	mi_listgroups = new JMenuItem ("Liste des groupes");
	mi_listgroups.addActionListener (new ActionListener ()
		{
			public void actionPerformed (ActionEvent e)
			{
				// Boite de dialogue affichant la liste des groupes
				JDialog dlg = new JDialog (frame, language.equals("Francais")?"Liste des groupes d'utilisateurs":"List of users groups", false);
				dlg.setResizable(true);
				dlg.getContentPane().setLayout (null /*new FlowLayout()*/);
				dlg.setSize (400, 500);

				Vector reponses = new Vector();

				// Connexion au serveur
				sc.login (basefax, login, password);
				// Demande de la liste des groupes
				sc.output ("LISTGROUPS\n");
				// Lecture de la réponse du serveur
				for (String reponse=sc.input("any"); 
					reponse.length() > 0;
					reponse = sc.input("any"))	
					reponses.addElement(reponse);
			
				String fieldnames[];
				String fieldnames_fr[] = { "groupe", "sous-groupe de"};
				String fieldnames_en[] = { "group", "subgroup of"};
				if (language.equals("Francais"))
					fieldnames = fieldnames_fr;
				else
					fieldnames = fieldnames_en;
 
				String[][]table = new String[reponses.size()][2];
				String[] fields;

				/*String[][] tgroupes = new String[reponses.size()][2];*/

				for (int i=0; i<reponses.size(); i++)
				{
                                        System.out.println ((String)(reponses.elementAt(i)));
					fields = ((String)(reponses.elementAt(i))).split("\t");
                                        
					for (int j=0; j<2; j++)
                                        {
                                                try
                                                {
                                                        table[i][j] = fields[j];
                                                }
                                                catch (Exception e1)
                                                {
                                                        table[i][j] = "";
                                                }
                                        }
				}
				
				JTable jtable = new JTable (table, fieldnames);
				/*jtable.setSize(300,100);*/
				jtable.setPreferredScrollableViewportSize(new Dimension(300, 120));


				JScrollPane scrollPane = new JScrollPane(jtable); 
				scrollPane.setBounds (10, 10, 310, 200);
				dlg.getContentPane().add(scrollPane);
				/*dlg.pack();*/

				int x = 40;
				int y = 150;
				int dx = 30;
				int dy = 20;
				int w = 200;
				int h = 15;

				int h1 = y-60+reponses.size()*dy;
				if (h1 < 210)
					h1 = 210;
				/*
				JPanel pane = new JPanel();
				pane.setBounds (330, 150, 310, 400);
				dlg.getContentPane().add (pane);
				*/

				/*
				y = dispTree ("", reponses.size(), table, pane, x, y, dx, dy, w, h);
				y = dispTree ("null", reponses.size(), table, pane, x, y, dx, dy, w, h);
 				*/

				// Affichage de l'arboresceence des groupes 
				GroupTree gt = new GroupTree ("", reponses.size(), table, x, 15, dx, dy, w, h);
				/*gt.setBounds (10, 150, 310, y-60+reponses.size()*dy);*/
				gt.setBounds (360, 10, 310, h1);
				dlg.getContentPane().add(gt);

				/*dlg.pack();*/
				/*dlg.setSize (350, y+reponses.size()*dy+50);*/
				dlg.setSize (680, h1+10);

				dlg.show();


			}
		});
	m_admin.add (mi_listgroups);

	mi_listrights = new JMenuItem ("Liste des droits");
	mi_listrights.addActionListener (new ActionListener ()
		{
			public void actionPerformed (ActionEvent e)
			{
				// Boîte de dialogue affichant la liste des droits
				JDialog dlg = new JDialog (frame, language.equals("Francais")?"Liste des droits des utilisateurs":"List of users rights", false);
				dlg.setResizable(true);
				dlg.getContentPane().setLayout (new FlowLayout());
				dlg.setSize (800, 500);

				Vector reponses = new Vector();

				// Connexion au serveur
				sc.login (basefax, login, password);
				// Demande de la liste des droits
				sc.output ("LISTRIGHTS\n");
				// Lecture de la réponse
				for (String reponse=sc.input("any"); 
					reponse.length() > 0;
					reponse = sc.input("any"))	
					reponses.addElement(reponse);
			
				String fieldnames[];
				String fieldnames_fr[] = { "base", "groupe", "routeur", "ligne", "id", "appelant", "appelé", "opération", "action", "paramètre", "options" };
				String fieldnames_en[] = { "base", "group", "router", "line", "id", "calling", "called", "operation", "action", "parameter", "options" };
				if (language.equals("Francais"))
					fieldnames = fieldnames_fr;
				else
					fieldnames = fieldnames_en;
 
				Object[][]tabledroits = new Object[reponses.size()][11];
				String[] fields;

				for (int i=0; i<reponses.size(); i++)
				{
					fields = ((String)(reponses.elementAt(i))).split("\t");
					for (int j=0; j<11; j++)
					{
						if (fields[j].equals("") || 
						    fields[j].equals("null") ||
						    fields[j] == null ||
						    fields[j].equals("-1"))
							tabledroits[i][j] = ""; /*language.equals("Francais")?"tous":"all";*/
						else
							tabledroits[i][j] = fields[j];					
					}
				}
				
				JTable jtabledroits = new JTable (tabledroits, fieldnames);
				jtabledroits.setPreferredScrollableViewportSize(new Dimension(700, 400));
	
				JScrollPane scrollPane = new JScrollPane(jtabledroits);
				/*scrollPane.setSize (700, 450);*/

				/*
				pane.add (scrollPane, BorderLayout.CENTER);
				pane.add (statusline, BorderLayout.SOUTH);
				*/

				dlg.getContentPane().add(scrollPane);

				/*dlg.getContentPane().add(jtabledroits);*/
				dlg.show();

			}
		});
	m_admin.add(mi_listrights);


	mi_listusers = new JMenuItem ("Liste des utilisateurs");
	mi_listusers.addActionListener (new ActionListener ()
		{
			public void actionPerformed (ActionEvent e)
			{
				JDialog dlg = new JDialog (frame, language.equals("Francais")?"Liste des utilisateurs":"List of users", false);
				dlg.setResizable(true);
				dlg.getContentPane().setLayout (new FlowLayout());
				dlg.setSize (600, 400);

				Vector reponses = new Vector();
				Vector groupes = new Vector();

				// Demande au serveur de la liste des utilisateurs

				sc.login (basefax, login, password);
				sc.output ("LISTUSERS\n");
				for (String reponse=sc.input("any"); 
					reponse.length() > 0;
					reponse = sc.input("any"))	
				{
					reponses.addElement(reponse);
					String groupes1 = "";
					// Liste des groupes auxquels appartient un utilisateur, séparés par des virgules
					for (String reponse1=sc.input("any");
						reponse1.length() > 0;
						reponse1 = sc.input("any"))					
						/*groupes1 += reponse1 + " ";*/
					{
						if (groupes1 == "")
							groupes1 = reponse1;
						else
							groupes1 += ", " + reponse1;
					}
					groupes.addElement(groupes1);										
				}
 
				String fieldnames[];
				String fieldnames_fr[] = { "login",/* "mot de passe", "crédits", "bloques", */"groupes" };
				String fieldnames_en[] = { "login",/* "password", "credits", "locked", */"groups" };
				if (language.equals("Francais"))
					fieldnames = fieldnames_fr;
				else
					fieldnames = fieldnames_en;
 
				Object[][]table = new Object[reponses.size()][/*5*/ 2];
				String[] fields;				 

				for (int i=0; i<reponses.size(); i++)
				{
					fields = ((String)(reponses.elementAt(i))).split("\t");
					/*for (int j=0; j<4; j++)
						table[i][j] = fields[j];*/
					table[i][0] = fields[0];
					table[i][/*4*/1] = (String)(groupes.elementAt(i));
				}
				
				JTable jtable = new JTable (table, fieldnames);

				jtable.setPreferredScrollableViewportSize(new Dimension(500, 300));
	
				/* Largeurs des colonnes */
				jtable.getColumnModel().getColumn(/*4*/ 1).setPreferredWidth(150); 


				JScrollPane scrollPane = new JScrollPane(jtable); 

				dlg.getContentPane().add(scrollPane);

				dlg.show();


			}
		});
	m_admin.add (mi_listusers);


	mi_groups = new JMenuItem ("Modifier les groupes");
	mi_groups.addActionListener (new ActionListener ()
		{
			public void actionPerformed (ActionEvent e)
			{
 


				/* Boite de dialogue pour l'administration des groupes */

				String titre;
				if (language.equals ("Francais"))
					titre = "Administration des groupes d'utilisateurs";
				else
					titre = "Users groups administration";

				final MyDialog dlg = new MyDialog (frame, titre, false);
 
				dlg.setResizable (true);

				int lang;
				if (language.equals("Francais"))
					lang = 0;
				else
					lang = 1;
				String fieldcont;
						
				ButtonGroup bgop1;
				JRadioButton rbadd, rbremove;
				JComboBox cbgroup;
				JTextField tfsgroup = null;
 
 				{
 
				    /*
					BorderLayout layout = new BorderLayout();
				    	dlg.getContentPane().setLayout(layout);
				      dlg.setSize (500, 600);
 					*/
		
					// dlg.getContentPane().setLayout (new GridLayout(4,2));
					dlg.getContentPane().setLayout (null);

					// dlg.setSize (500, 200);
					dlg.setSize (400, 210);
 
					bgop1 = new ButtonGroup();
					/*cbadd = new CheckBox (language.equals("English")?"Add":"Ajouter", true, cbgop1);*/
					rbadd = new JRadioButton (language.equals("English")?"Add":"Ajouter", true);
					rbadd.setBounds (10, 20, 100, 20);
					bgop1.add(rbadd);
					dlg.getContentPane().add(rbadd);
					rbremove = new JRadioButton (language.equals("English")?"Remove":"Supprimer", false);
					rbremove.setBounds (120, 20, 100, 20);
					bgop1.add(rbremove);
                              dlg.getContentPane().add(rbremove);

					JLabel l1 = new JLabel(language.equals("English")?"   the group":"   le groupe");
					l1.setBounds (10, 50, 100, 20);
					dlg.getContentPane().add (l1);

					tfsgroup = new JTextField (30);
					tfsgroup.setBounds (120, 50, 200, 20);
					dlg.getContentPane().add(tfsgroup);
 
					JLabel l2 = new JLabel(language.equals("English")?"   in the group":"   dans le groupe");
					l2.setBounds (10, 80, 100, 20);
					dlg.getContentPane().add (l2);
 
					cbgroup = new JComboBox();
					cbgroup.setBounds (120, 80, 200, 20);

					/*** 
					Connection con = null;
                              String baseBDD = "droits";

					
					try
					{
						Class.forName("sun.jdbc.odbc.JdbcOdbcDriver").newInstance();
                        		con = DriverManager.getConnection("jdbc:odbc:" + baseBDD);

						// On parcourt les groupes 
						String requete = "SELECT * FROM \"groupes\"";
						ResultSet rs = con.createStatement().executeQuery (requete);
						while (rs.next())
						{
							String groupe = rs.getString("sousgroupe");
							cbgroup.addItem(groupe);
						}
					}
                                        catch(Exception e1)
					{
                                                JOptionPane.showMessageDialog(null, "connexion (1) : Erreur de connexion à la base de données " + baseBDD + " :\n" + e1.getMessage(),
										  " " + "Erreur", JOptionPane.ERROR_MESSAGE);
                                                return;
					}
					***/

					// On demande au serveur la liste des groupes

					boolean status = sc.login (basefax, login, password);
					sc.output ("LISTGROUPS\n");
					StringSet groupes = new StringSet();
					for (String reponse = sc.input("any"); 
						reponse.length() > 1; 
						reponse = sc.input("any"))
					{		
						String[] fields = reponse.split("\t");			
						/*cbgroup.addItem (fields[0]);	*/
						groupes.add(fields[0]);
					}
					sc.logout();
					for (int i=0; i<groupes.size(); i++)
					{
						cbgroup.addItem (groupes.elementAt(i));
					}
 /******/

					dlg.getContentPane().add(cbgroup);

 
  
					
				}
				
				JButton btn_ok = new JButton ("OK");
 				btn_ok.setBounds (10, 120, 100, 25);
				/* btn_ok.addActionListener (new devt (dlg)); */
 
				btn_ok.addActionListener (new ActionListener()
					{
						public void actionPerformed (ActionEvent e)
						{
							dlg.ok = true;
							dlg.dispose();
							
						}
					});


				dlg.getContentPane().add (btn_ok);
				
				JButton btn_cancel = new JButton (language.equals("Francais")?"Annuler":"Cancel");
				btn_cancel.setBounds (120, 120, 100, 25);	
				btn_cancel.addActionListener (new ActionListener()
					{
						public void actionPerformed (ActionEvent e)
						{
							dlg.ok = false;
							dlg.dispose();
						}
					});
				dlg.getContentPane().add(btn_cancel);

				dlg.setModal(true);
				dlg.show();
				System.out.println ("Dialog ok=" + dlg.ok);
	
				if (!dlg.ok)
				{
					return;
				}

/*
				String email = JOptionPane.showInputDialog (null, "Tapez l'adresse email du destinataire", "Envoi de fax par email", JOptionPane.PLAIN_MESSAGE);	
*/

/*
                        String email = tfemail.getText();
				String subject = tfsubject.getText();
				String text = tftext.getText();
*/

				boolean add = rbadd.isSelected();
				boolean remove = rbremove.isSelected();
 
				String sgroupe = tfsgroup.getText();
				String groupe = (String)(cbgroup.getSelectedItem());
 






	try
	{
	
	/* connexion */

	boolean status = sc.login (basefax, login, password);
	InputStream is = sc.is;
	OutputStream os = sc.os;

/*** le code ci-dessous est remplacé par sc.login ***

	Socket sock = new Socket ("localhost", 6020);
	InputStream is = sock.getInputStream();
	OutputStream os = sock.getOutputStream();
***/
	String com;
      String commande;
	boolean auth = false;
	boolean existe = false;
/***	
	// envoi du login et du mot de passe
	com = new String("USER\n" + login +"\n" + "PASS\n" + password +"\n");
	os.write(com.getBytes("US-ASCII"));
	os.flush();
	commande = new String(recoit_param(is, "alpha"));
	System.out.println ("received(1): " + commande);

	commande = new String(recoit_param(is, "any"));
	System.out.println ("received(2): " + commande);
 ***/

	if (add) com = "GROUP\nADD\n"; else com = "GROUP\nREMOVE\n";
      com += groupe + "\n" + sgroupe + "\n";
	System.out.println ("send: " + com + ".");
	os.write(com.getBytes("US-ASCII"));
	os.flush();
	commande = new String(recoit_param(is,"any"));
	System.out.println ("received: " + commande + ".");
	if (commande.equals ("NOTAUTH"))
	{
		JOptionPane.showMessageDialog(null, language.equals("English")?"You are not authorized to modify this group.":"Vous n'êtes pas autorisé à modifier ce groupe", "Administration", JOptionPane.WARNING_MESSAGE); 
	}
	else if (commande.equals ("ERROR"))
	{
		JOptionPane.showMessageDialog(null, language.equals("English")?"Error modifying rights database":"Erreur dans la modification de la base des droits", "Administration", JOptionPane.WARNING_MESSAGE); 
	}
 	else if (commande.equals ("OK"))
	{
		JOptionPane.showMessageDialog(null, language.equals("English")?"Modification done":"Modification effectuée", "Administration", JOptionPane.WARNING_MESSAGE); 
	}


	/* Déconnexion */

	sc.logout();
	/***
	com = new String("END\n");
	os.write(com.getBytes("US-ASCII"));
	os.flush();
	sock.close();
	***/
	System.out.println("Deconnecte");

 
	}
	catch (Exception e3)
	{
		/*label.setText ("Error: " + e.getMessage());*/
                e3.printStackTrace();
		statusline.setText ("Error: " + e3.getMessage());
	}
	 












			}
		});
	m_admin.add(mi_groups);

	

	mi_rights = new JMenuItem ("Modifier les droits");
	mi_rights.addActionListener (new ActionListener ()
		{
			public void actionPerformed (ActionEvent e)
			{
 


				/* Boite de dialogue pour l'administration des droits*/

				String titre;
				if (language.equals ("Francais"))
					titre = "Administration des droits des utilisateurs";
				else
					titre = "Users rights administration";

				final MyDialog dlg = new MyDialog (frame, titre, false);
 				JPanel pane;
 
				dlg.setResizable (true);

				int lang;
				if (language.equals("Francais"))
					lang = 0;
				else
					lang = 1;
				String fieldcont;
						
				ButtonGroup bgop1, bgop2;
				JRadioButton rbadd, rbremove, rbplus, rbmoins, rbtilda;
				JComboBox cbright, cbgroup;
				JTextField tfappelant = null;
				JTextField tfappele = null;
				JTextField tfparam = null;
				JTextField tfoptions = null;
				JTextField tfrtr = null;
				JTextField tfline = null;
				JTextField tfid = null;
				JTextField tfbase = null;
		
				int hl = 30; /* hauteur ligne */
				int el = 35; /* espacement lignes */

 				{
 
				    /*
					BorderLayout layout = new BorderLayout();
				    	dlg.getContentPane().setLayout(layout);
				      dlg.setSize (500, 600);
 					*/
		
					GridBagLayout gbl = new GridBagLayout();
					GridBagConstraints gbc = new GridBagConstraints();
					/*dlg.getContentPane().setLayout(gbl);*/

					/*dlg.getContentPane().setLayout (new FlowLayout()); */
					dlg.getContentPane().setLayout(null);
					dlg.setSize (510, 500);

					gbc.weightx = 1;
					gbc.weighty = 1;
 
				 

					pane = new JPanel();
					pane.setLayout (new FlowLayout());
				
					
					bgop1 = new ButtonGroup();
					/*cbadd = new CheckBox (language.equals("English")?"Add":"Ajouter", true, cbgop1);*/
					rbadd = new JRadioButton (language.equals("English")?"Add":"Ajouter", true);
					rbadd.setBounds(30,el,90,hl);
					bgop1.add(rbadd);
					gbc.gridx = 1;
					gbl.setConstraints (rbadd, gbc);
					dlg.getContentPane().add(rbadd);
					rbremove = new JRadioButton (language.equals("English")?"Remove":"Supprimer", false);
					rbremove.setBounds(120,el,90,hl);
					bgop1.add(rbremove);
					gbc.gridx = 2;

					gbl.setConstraints (rbremove, gbc);
                              dlg.getContentPane().add(rbremove);

					JLabel l1 = new JLabel(language.equals("English")?"the right":"le droit");
					l1.setBounds(30,9*el,80,hl);
					dlg.getContentPane().add(l1);
					

					bgop2 = new ButtonGroup();
					rbplus = new JRadioButton ("autorisation", true);
					rbplus.setBounds(110,9*el,100,hl);
					bgop2.add(rbplus);
					dlg.getContentPane().add(rbplus);
                              rbmoins = new JRadioButton ("interdiction", false);
					rbmoins.setBounds(220,9*el,90,hl);
					bgop2.add(rbmoins);
					dlg.getContentPane().add(rbmoins);
					/*
					rbtilda = new JRadioButton ("~", true);
					rbtilda.setBounds (270, 9*el, 40, hl);
					bgop2.add(rbtilda);
					dlg.getContentPane().add(rbtilda);
					*/

					// Affichage d'une combo box pour choisir l'action
					cbright = new JComboBox();
					cbright.addItem("LIST");
					cbright.addItem("SHOW");
					cbright.addItem("PRINT");
					cbright.addItem("FAX");
					cbright.addItem("EMAIL");
					cbright.addItem("SAVE");
					cbright.addItem("CLASS");
					cbright.addItem("DELETE");
					cbright.addItem("ADMIN");
					cbright.addItem("SCRIPT");
					cbright.setBounds(320,9*el,100,hl);
					dlg.getContentPane().add(cbright);


					JLabel l2 = new JLabel(language.equals("English")?"parameter : ":"paramètre : ");
					l2.setBounds(30,10*el,80,hl);
					dlg.getContentPane().add(l2);
					tfparam = new JTextField(30);
					tfparam.setBounds(120,10*el,300,hl);
					dlg.getContentPane().add(tfparam);

					JLabel l3 = new JLabel(language.equals("English")?"options : ":"options : ");
					l3.setBounds(30,11*el,80,hl);
					dlg.getContentPane().add(l3);
					tfoptions = new JTextField(10);
					tfoptions.setBounds(120,11*el,100,hl);
					dlg.getContentPane().add(tfoptions);

					JLabel lbase = new JLabel(language.equals("English")?"for the base":"pour la base");
					lbase.setBounds(30,2*el,110,hl);
					dlg.getContentPane().add(lbase);

					tfbase = new JTextField(30);
					tfbase.setBounds(150,2*el,200,hl);
					dlg.getContentPane().add(tfbase);

					JLabel l4 = new JLabel(language.equals("English")?"for the group":"pour le groupe");
					l4.setBounds(30,3*el,110,hl);
					dlg.getContentPane().add(l4);

					cbgroup = new JComboBox();

					/***
					Connection con = null;
                              String baseBDD = "droits";

					
					try
					{
						Class.forName("sun.jdbc.odbc.JdbcOdbcDriver").newInstance();
                        		con = DriverManager.getConnection("jdbc:odbc:" + baseBDD);

						// On parcourt les groupes 
						String requete = "SELECT * FROM \"groupes\"";
						ResultSet rs = con.createStatement().executeQuery (requete);
						while (rs.next())
						{
							String groupe = rs.getString("sousgroupe");
							cbgroup.addItem(groupe);
						}
					}
                                        catch(Exception e1)
					{
                                                JOptionPane.showMessageDialog(null, "connexion (1) : Erreur de connexion à la base de données " + baseBDD + " :\n" + e1.getMessage(),
										  " " + "Erreur", JOptionPane.ERROR_MESSAGE);
                                                return;
					}
					***/

					// On demande au serveur la liste des groupes pour créer la combo box pour le choix du groupe

					boolean status = sc.login (basefax, login, password);
					sc.output ("LISTGROUPS\n");
					StringSet groupes = new StringSet();
					for (String reponse = sc.input("any"); 
						reponse.length() > 1; 
						reponse = sc.input("any"))
					{		
						String[] fields = reponse.split("\t");			
						/*cbgroup.addItem (fields[0]);	*/
						groupes.add(fields[0]);
					}
					sc.logout();
					for (int i=0; i<groupes.size(); i++)
					{
						cbgroup.addItem (groupes.elementAt(i));
					}

 /******/

					cbgroup.setBounds(150,3*el,200,hl);
					dlg.getContentPane().add(cbgroup);

					JLabel l5 = new JLabel(language.equals("English")?"calling number : ":"numéro appelant : ");
					l5.setBounds(30,5*el,110,hl);
					dlg.getContentPane().add(l5);
					tfappelant = new JTextField(20);
					tfappelant.setBounds(150,5*el,200,hl);
					dlg.getContentPane().add(tfappelant);

					JLabel l6 = new JLabel(language.equals("English")?"called number : ":"numéro appelé : ");
					l6.setBounds(30,4*el,110,hl);
					dlg.getContentPane().add(l6);
					tfappele = new JTextField(20);
					tfappele.setBounds(150,4*el,200,hl);
					dlg.getContentPane().add(tfappele);

					JLabel lrtr = new JLabel(language.equals("English")?"router : ":"routeur : ");
					lrtr.setBounds(30,6*el,110,hl);
					dlg.getContentPane().add(lrtr);
		
					tfrtr = new JTextField(20);
					tfrtr.setBounds(150,6*el,200,hl);
					dlg.getContentPane().add(tfrtr);

					JLabel lline = new JLabel(language.equals("English")?"line : ":"ligne : ");
					lline.setBounds(30,7*el,110,hl);
					dlg.getContentPane().add(lline);
		
					tfline = new JTextField(20);
					tfline.setBounds(150,7*el,200,hl);
					dlg.getContentPane().add(tfline);

					JLabel lid = new JLabel("id : ");
					lid.setBounds(30,8*el,110,hl);
					dlg.getContentPane().add(lid);

					tfid = new JTextField(20);
					tfid.setBounds(150,8*el,200,hl);
					dlg.getContentPane().add(tfid);
					


                                        /*
					JLabel label;
					if (language.equals("English"))
						label = new JLabel ("Email address of the recipient : ");
					else
						label = new JLabel ("Adresse email du destinataire :");
					dlg.getContentPane().add (label);
					tfemail = new JTextField (30);
					dlg.getContentPane().add (tfemail);

					dlg.getContentPane().add (new JLabel(language.equals("English")?"Subject of the message : ":"Sujet du message :"));
					tfsubject = new JTextField (30);
					dlg.getContentPane().add (tfsubject);

					dlg.getContentPane().add (new JLabel(language.equals("English")?"Text of the message : ":"Texte du message :"));
					tftext = new JTextField (30);
					dlg.getContentPane().add (tftext);
                                        */
					
				}
				
				JButton btn_ok = new JButton ("OK");
 				btn_ok.setBounds(100,12*el,80,hl);
				/* btn_ok.addActionListener (new devt (dlg)); */
 
				btn_ok.addActionListener (new ActionListener()
					{
						public void actionPerformed (ActionEvent e)
						{
							dlg.ok = true;
							dlg.dispose();
							
						}
					});


				dlg.getContentPane().add (btn_ok);
				
				JButton btn_cancel = new JButton (language.equals("Francais")?"Annuler":"Cancel");
				btn_cancel.setBounds(300,12*el,80,hl);
				btn_cancel.addActionListener (new ActionListener()
					{
						public void actionPerformed (ActionEvent e)
						{
							dlg.ok = false;
							dlg.dispose();
						}
					});
				dlg.getContentPane().add(btn_cancel);

                        dlg.getContentPane().add(pane); 

				dlg.setModal(true);
				dlg.show();
				System.out.println ("Dialog ok=" + dlg.ok);
	
				if (!dlg.ok)
				{
					return;
				}

/*
				String email = JOptionPane.showInputDialog (null, "Tapez l'adresse email du destinataire", "Envoi de fax par email", JOptionPane.PLAIN_MESSAGE);	
*/

/*
                        String email = tfemail.getText();
				String subject = tfsubject.getText();
				String text = tftext.getText();
*/

				boolean add = rbadd.isSelected();
				boolean remove = rbremove.isSelected();
				boolean plus = rbplus.isSelected();
				boolean moins = rbmoins.isSelected();
				/*boolean tilda = rbtilda.isSelected();*/

				String action = (String)(cbright.getSelectedItem());
				String groupe = (String)(cbgroup.getSelectedItem());

				String base = tfbase.getText();
				String appelant = tfappelant.getText();
				String appele = tfappele.getText();
				String rtr = tfrtr.getText();
				String line = tfline.getText();
				String id = tfid.getText();
				String param = tfparam.getText();
				String options = tfoptions.getText();







	try
	{
	
	/* connexion */

	boolean status = sc.login (basefax, login, password);
	InputStream is = sc.is;
	OutputStream os = sc.os;

/*** le code ci-dessous est remplacé par sc.login ***

	Socket sock = new Socket ("localhost", 6020);
	InputStream is = sock.getInputStream();
	OutputStream os = sock.getOutputStream();
***/
	String com;
      String commande;
	boolean auth = false;
	boolean existe = false;
/***	
	// envoi du login et du mot de passe
	com = new String("USER\n" + login +"\n" + "PASS\n" + password +"\n");
	os.write(com.getBytes("US-ASCII"));
	os.flush();
	commande = new String(recoit_param(is, "alpha"));
	System.out.println ("received(1): " + commande);

	commande = new String(recoit_param(is, "any"));
	System.out.println ("received(2): " + commande);
 ***/

	// Envoi au serveur de la commande de modification des droits
	if (add) com = "RIGHT\nADD\n"; else com = "RIGHT\nREMOVE\n";
      if (plus) com += "+\n"; 
	else if (moins) com += "-\n";
	/*else if (tilda) com += "~\n";*/
	else com += "-\n";
	com += action + "\n" + param + "\n" + options + "\n" + base + "\n" + groupe + "\n" + rtr + "\n" + line + "\n" + id + "\n" + appelant + "\n" + appele + "\n";
	System.out.println ("send: " + com + ".");
	os.write(com.getBytes("US-ASCII"));
	os.flush();
	commande = new String(recoit_param(is,"any"));
	System.out.println ("received: " + commande + ".");
	if (commande.equals ("NOTAUTH"))
	{
		JOptionPane.showMessageDialog(null, language.equals("English")?"You are not authorized to do this modification of rights":"Vous n'êtes pas autorisé à effectuer cette modification des droits", "Administration", JOptionPane.WARNING_MESSAGE); 
	}
	else if (commande.equals ("ERROR"))
	{
		JOptionPane.showMessageDialog(null, language.equals("English")?"Error modifying rights database":"Erreur dans la modification de la base des droits", "Administration", JOptionPane.WARNING_MESSAGE); 
	}
 	else if (commande.equals ("OK"))
	{
		JOptionPane.showMessageDialog(null, language.equals("English")?"Modification done":"Modification effectuée", "Administration", JOptionPane.WARNING_MESSAGE); 
	}


	/* Déconnexion */

	sc.logout();
	/***
	com = new String("END\n");
	os.write(com.getBytes("US-ASCII"));
	os.flush();
	sock.close();
	***/
	System.out.println("Deconnecte");

 
	}
	catch (Exception e3)
	{
		/*label.setText ("Error: " + e.getMessage());*/
                e3.printStackTrace();
		statusline.setText ("Error: " + e3.getMessage());
	}
	 












			}
		});
	m_admin.add(mi_rights);



	mi_users = new JMenuItem ("Modifier les utilisateurs");
	mi_users.addActionListener (new ActionListener ()
		{
			public void actionPerformed (ActionEvent e)
			{
 


				/* Boite de dialogue pour l'administration des utilisateurs */

				String titre;
				if (language.equals ("Francais"))
					titre = "Administration des utilisateurs";
				else
					titre = "Users administration";

				final MyDialog dlg = new MyDialog (frame, titre, false);
 
				dlg.setResizable (true);

				int lang;
				if (language.equals("Francais"))
					lang = 0;
				else
					lang = 1;
				String fieldcont;
						
				ButtonGroup bgop1;
				JRadioButton rbadd, rbremove;
				JComboBox cbgroup;
				JTextField tflogin = null;
				JTextField tfpass = null;
				JTextField tfcredits = null;
				JTextField tfbloques = null;

 
 				{
 
				    /*
					BorderLayout layout = new BorderLayout();
				    	dlg.getContentPane().setLayout(layout);
				      dlg.setSize (500, 600);
 					*/
		
					dlg.getContentPane().setLayout (new GridLayout(7,2));
					dlg.setSize (500, 200);
 
					bgop1 = new ButtonGroup();
					/*cbadd = new CheckBox (language.equals("English")?"Add":"Ajouter", true, cbgop1);*/
					rbadd = new JRadioButton (language.equals("English")?"Add":"Ajouter", true);
					bgop1.add(rbadd);
					dlg.getContentPane().add(rbadd);
					rbremove = new JRadioButton (language.equals("English")?"Remove":"Supprimer", false);
					bgop1.add(rbremove);
                                        dlg.getContentPane().add(rbremove);

					dlg.getContentPane().add (new JLabel(language.equals("Francais")?"   le compte de l'utilisateur":"   the user account"));

					tflogin = new JTextField (30);
					dlg.getContentPane().add(tflogin);

					dlg.getContentPane().add (new JLabel(language.equals("English")?"   password":"   mot de passe"));
					tfpass = new JTextField (30);
					dlg.getContentPane().add(tfpass);

					dlg.getContentPane().add (new JLabel(language.equals("English")?"   credits":"   crédits"));
 					tfcredits = new JTextField (30);
					dlg.getContentPane().add(tfcredits);

					dlg.getContentPane().add (new JLabel(language.equals("English")?"   locked credits":"   crédits bloqués"));
					tfbloques = new JTextField (30);
					dlg.getContentPane().add(tfbloques);

					
					dlg.getContentPane().add (new JLabel(language.equals("English")?"   in the group":"   dans le groupe"));
 
					cbgroup = new JComboBox();
					

					/***
					Connection con = null;
                              String baseBDD = "droits";

					
					try
					{
						Class.forName("sun.jdbc.odbc.JdbcOdbcDriver").newInstance();
                        		con = DriverManager.getConnection("jdbc:odbc:" + baseBDD);

						// On parcourt les groupes 
						String requete = "SELECT * FROM \"groupes\"";
						ResultSet rs = con.createStatement().executeQuery (requete);
						while (rs.next())
						{
							String groupe = rs.getString("sousgroupe");
							cbgroup.addItem(groupe);
						}
					}
                                        catch(Exception e1)
					{
                                                JOptionPane.showMessageDialog(null, "connexion (1) : Erreur de connexion à la base de données " + baseBDD + " :\n" + e1.getMessage(),
										  " " + "Erreur", JOptionPane.ERROR_MESSAGE);
                                                return;
					}
					***/
					
					// On demande au serveur la liste des groupes pour remplir la combo box
					boolean status = sc.login (basefax, login, password);
					sc.output ("LISTGROUPS\n");
					StringSet groupes = new StringSet();
					for (String reponse = sc.input("any"); 
						reponse.length() > 1; 
						reponse = sc.input("any"))
					{		
						String[] fields = reponse.split("\t");			
						/*cbgroup.addItem (fields[0]);	*/
						groupes.add(fields[0]);
					}
					sc.logout();
					for (int i=0; i<groupes.size(); i++)
					{
						cbgroup.addItem (groupes.elementAt(i));
					}


					dlg.getContentPane().add(cbgroup);
					
 					
  
					
				}
				
				JButton btn_ok = new JButton ("OK");
 
				/* btn_ok.addActionListener (new devt (dlg)); */
 
				btn_ok.addActionListener (new ActionListener()
					{
						public void actionPerformed (ActionEvent e)
						{
							dlg.ok = true;
							dlg.dispose();
							
						}
					});


				dlg.getContentPane().add (btn_ok);
				
				JButton btn_cancel = new JButton (language.equals("Francais")?"Annuler":"Cancel");
				btn_cancel.addActionListener (new ActionListener()
					{
						public void actionPerformed (ActionEvent e)
						{
							dlg.ok = false;
							dlg.dispose();
						}
					});
				dlg.getContentPane().add(btn_cancel);

				dlg.setModal(true);
				dlg.show();
				System.out.println ("Dialog ok=" + dlg.ok);
	
				if (!dlg.ok)
				{
					return;
				}

/*
				String email = JOptionPane.showInputDialog (null, "Tapez l'adresse email du destinataire", "Envoi de fax par email", JOptionPane.PLAIN_MESSAGE);	
*/

/*
                        String email = tfemail.getText();
				String subject = tfsubject.getText();
				String text = tftext.getText();
*/

				boolean add = rbadd.isSelected();
				boolean remove = rbremove.isSelected();
 
				/*String sgroupe = tfsgroup.getText();*/
				String login1 = tflogin.getText();
				String pass = tfpass.getText();
				String credits = tfcredits.getText();
				String bloques = tfbloques.getText();
				String groupe = (String)(cbgroup.getSelectedItem());
 






	try
	{
	
	/* connexion */

	boolean status = sc.login (basefax, login, password);
	InputStream is = sc.is;
	OutputStream os = sc.os;

/*** le code ci-dessous est remplacé par sc.login ***

	Socket sock = new Socket ("localhost", 6020);
	InputStream is = sock.getInputStream();
	OutputStream os = sock.getOutputStream();
***/
	String com;
      String commande;
	boolean auth = false;
	boolean existe = false;
/***	
	// envoi du login et du mot de passe
	com = new String("USER\n" + login +"\n" + "PASS\n" + password +"\n");
	os.write(com.getBytes("US-ASCII"));
	os.flush();
	commande = new String(recoit_param(is, "alpha"));
	System.out.println ("received(1): " + commande);

	commande = new String(recoit_param(is, "any"));
	System.out.println ("received(2): " + commande);
 ***/

	// On envoie au serveur la commande de création ou suppression d'utilisateur
	if (add) com = "CLIENT\nADD\n"; else com = "CLIENT\nREMOVE\n";
      com += login1 + "\n" + pass + "\n" + credits + "\n" + bloques + "\n" + groupe + "\n" ;	
	System.out.println ("send: " + com + ".");
	os.write(com.getBytes("US-ASCII"));
	os.flush();
	commande = new String(recoit_param(is,"any"));
	System.out.println ("received: " + commande + ".");
	if (commande.equals ("NOTAUTH"))
	{
		JOptionPane.showMessageDialog(null, language.equals("English")?"You are not authorized to modify this group.":"Vous n'êtes pas autorisé à modifier ce groupe", "Administration", JOptionPane.WARNING_MESSAGE); 
	}
	else if (commande.equals ("ERROR"))
	{
		JOptionPane.showMessageDialog(null, language.equals("English")?"Error modifying rights database":"Erreur dans la modification de la base des droits", "Administration", JOptionPane.WARNING_MESSAGE); 
	}
 	else if (commande.equals ("OK"))
	{
		JOptionPane.showMessageDialog(null, language.equals("English")?"Modification done":"Modification effectuée", "Administration", JOptionPane.WARNING_MESSAGE); 
	}


	/* Déconnexion */

	sc.logout();
	/***
	com = new String("END\n");
	os.write(com.getBytes("US-ASCII"));
	os.flush();
	sock.close();
	***/
	System.out.println("Deconnecte");

 
	}
	catch (Exception e3)
	{
		/*label.setText ("Error: " + e.getMessage());*/
                e3.printStackTrace();
		statusline.setText ("Error: " + e3.getMessage());
	}
	 












			}
		});
	m_admin.add(mi_users);

      if (language.equals("English"))
		m_trt = new JMenu ("Treatment");
	else
		m_trt = new JMenu ("Traitement");
	m_trt.setIcon (new ImageIcon (icondir + "31-Be98 Setting.gif"));
	mb.add (m_trt);
	 


	mi_listscripts = new JMenuItem (language.equals("Francais")?"Liste":"List");
	mi_listscripts.addActionListener (new ActionListener ()
		{
			public void actionPerformed (ActionEvent e)
			{
				// Boîte de dialogue affichant la liste des scripts

				// Numéros de lignes des fax sélectionnés dans la liste
				final int[] sel_fax = tablefaxrec.getSelectedRows();

				JDialog dlg = new JDialog (frame, language.equals("Francais")?"Liste des traitements des fax reçus":"List of received faxes treatments", false);
				dlg.setResizable(true);
				dlg.getContentPane().setLayout (null /*new FlowLayout()*/);
				dlg.setSize (800, 650);

				JLabel l_exec = new JLabel (language.equals("Francais")?"Exécuter sur les fax sélectionnés":"Run on selected faxes");
				l_exec.setBounds (30, 20, 300, 20);
				dlg.getContentPane().add (l_exec);

				final JCheckBox cb_act = new JCheckBox (language.equals("Francais")?"les scripts actifs pour ces fax":"active scripts for these faxes");
				cb_act.setBounds (30, 50, 300, 20);
				dlg.getContentPane().add (cb_act);

				final JCheckBox cb_named = new JCheckBox (language.equals("Francais")?"les script sélectionnés":"selected scripts");
				cb_named.setBounds (30, 80, 300, 20);
				dlg.getContentPane().add (cb_named);
				Vector reponses = new Vector();

				// On demande au serveur la liste des scripts

				sc.login (basefax, login, password);
				sc.output ("LISTSCRIPTS\n");
				for (String reponse=sc.input("any"); 
					reponse.length() > 0;
					reponse = sc.input("any"))	
					reponses.addElement(reponse);
			
				String fieldnames[];
				String fieldnames_fr[] = { "Nom", "Position", "Statut", "Routeur", "Ligne", "Id", "Appelé", "Appelant", "Code" };
				String fieldnames_en[] = { "Name", "Position", "Status", "Router", "Line", "Id", "Called", "Calling", "Code" };
				if (language.equals("Francais"))
					fieldnames = fieldnames_fr;
				else
					fieldnames = fieldnames_en;
 
				System.out.println ("" + reponses.size() + " reponses");
				final String[][]table = new String[reponses.size()][9];
				String[] fields;

				/*String[][] tgroupes = new String[reponses.size()][2];*/

				for (int i=0; i<reponses.size(); i++)
				{
                              // System.out.println (" " + i + ": " + (String)(reponses.elementAt(i)));
					fields = ((String)(reponses.elementAt(i))).split("\u001e");
					/*System.out.println ("field code=" + fields[5]);*/
                                        
					for (int j=0; j<9; j++)
                                        {
                                                try
                                                {
									  /*System.out.println ("field code=" + fields[5]);*/
                                                        table[i][j] = fields[j];
									  if ((j==3 || j==4) && table[i][j].equals("-1"))
										table[i][j] = "";
                                                }
                                                catch (Exception e1)
                                                {
                                                        table[i][j] = "";
                                                }
                                        }
				}
				
				final JTable jtable = new JTable (table, fieldnames);
				/*jtable.setSize(300,100);*/
				jtable.setPreferredScrollableViewportSize(new Dimension(750,370));

				jtable.getColumnModel().getColumn(8).setPreferredWidth(400); 

				JScrollPane scrollPane = new JScrollPane(jtable); 
				scrollPane.setBounds (10, 150, 780, 380);
				dlg.getContentPane().add(scrollPane);
		
				JButton btn_modif = new JButton (language.equals("Francais")?"Modifier":"Modify");
				btn_modif.setBounds (10, 570, 80, 30);
				btn_modif.addActionListener (new ActionListener ()
					{
						public void actionPerformed (ActionEvent e)
						{
							int i = jtable.getSelectedRow();
							System.out.println ("modif code=" + table[i][5]);
							defineScript (table[i][0], table[i][1], table[i][2], table[i][3], table[i][4], table[i][5], table[i][6], table[i][7], table[i][8].replace('$','\n'));
						}
					});
				dlg.getContentPane().add (btn_modif);
				
				JButton btn_run = new JButton (language.equals("Francais")?"Exécuter":"Run");
				btn_run.setBounds (30, 110, 100, 20);
				btn_run.addActionListener (new ActionListener ()
					{
						public void actionPerformed (ActionEvent e)
						{

							// Envoi au serveur de la commande d'exécution des scripts

							sc.login (basefax, login, password);
							 
							String commande = "RUNSCRIPTS\n";
							int[] sel_scripts = jtable.getSelectedRows();
							int i;
							if (cb_act.isSelected())
								commande += "A\n";
							else
								commande += "N\n";
							if (cb_named.isSelected())
								commande += "S\n";
							else
								commande += "N\n";

							for (i=0; i<sel_scripts.length; i++)
							{
								commande += table[sel_scripts[i]][0];
								if (i < sel_scripts.length-1)
									commande += "\t";
							}
							/*commande += "\n";*/
							/* envoyer donnees fax en XML */
							for (i=0; i<sel_fax.length; i++)
							{
						 
								commande += "\n";
								/*
								for (int j=0; j<23; j++)
								{
									if (j>0) commande += "\t";
									commande += (String)tablefaxrec.getValueAt(sel_fax[i],j);
								}*/
								/*GenericMessage m = new GenericMessage ((String)vlistfaxd.elementAt(sel_fax[i]));*/
								int k = sel_fax[i];
								int p;
								/*System.out.println ("size=" + vlistfax.size() + ", " + tablefaxrec.getRowCount());*/
								int q;
								/*for (q=0; q<vlistfax.size(); q++)
									System.out.println ("" + q + ": " + vlistfax.elementAt(q));*/
								
								/*for (q=0; q<vlistfax.size(); q+=vlistfax.size()-1)*/
								/*for (q=0; q<=k; q++)*/
									
								/*
								q=vlistfax.size()-3; System.out.println ("" + q + ": " + vlistfax.elementAt(q));
								q++; System.out.println ("" + q + ": " + vlistfax.elementAt(q));
								q++; System.out.println ("" + q + ": " + vlistfax.elementAt(q));
								*/
								if (_reverse)
									p = /*vlistfax.size()*/ tablefaxrec.getRowCount() - k - 1;
								else
									p = k;
								System.out.println ("k=" + k + " p=" + p + " reverse=" + _reverse);
								/*
								System.out.println ("size=" + vlistfax.size() + " p=" + p);
								System.out.println ((String)vlistfax.elementAt(p));																
								GenericMessage m = new GenericMessage ((String)vlistfax.elementAt(p));
								*/
								GenericMessage m = new GenericMessage (((MessageInfo)(messages.elementAt(p))).rep);
								commande += m.toString();
							}
							commande += "\n\n";

							System.out.println (commande);
							sc.output (commande);
	
							String reponse = sc.input("any");
							System.out.println (reponse);
							
							sc.logout();

							
						}
					});
				dlg.getContentPane().add (btn_run);



				/*dlg.pack();*/ 
				dlg.show();


			}
		});
	m_trt.add (mi_listscripts);




	if (language.equals("English"))
		mi_def = new JMenuItem ("Define");
	else
		mi_def = new JMenuItem ("Définir");
	mi_def.addActionListener (new ActionListener ()
		{
			public void actionPerformed (ActionEvent e)
			{
                                defineScript ("", "", "", "", "", "", "", "", "");
			}
		});	
	m_trt.add (mi_def);

	if (language.equals("English"))
		mi_run = new JMenuItem ("Run");
	else
		mi_run = new JMenuItem ("Exécuter");
	mi_run.addActionListener (new ActionListener ()
		{
			public void actionPerformed (ActionEvent e)
			{
 				final int[] rows = tablefaxrec.getSelectedRows();
				
				JDialog dlg = new JDialog (frame, language.equals("Francais")?"Exécution de scripts":"Run scripts", false);
				dlg.setResizable(true);
				dlg.getContentPane().setLayout (null /*new FlowLayout()*/);
				dlg.setSize (400, 500);

				JLabel l_exec = new JLabel (language.equals("Francais")?"Exécuter sur les fax sélectionnés":"Run on selected faxes");
				l_exec.setBounds (30, 20, 300, 20);
				dlg.getContentPane().add (l_exec);

				JCheckBox cb_act = new JCheckBox (language.equals("Francais")?"les scripts actifs pour ces fax":"active scripts for these faxes");
				cb_act.setBounds (30, 50, 300, 20);
				dlg.getContentPane().add (cb_act);

				JCheckBox cb_named = new JCheckBox (language.equals("Francais")?"les script sélectionnés":"selected scripts");
				cb_named.setBounds (30, 80, 300, 20);
				dlg.getContentPane().add (cb_named);

 				JTable t_names = new JTable();


				
			}
		});
	/*m_trt.add (mi_run);*/


	if (language.equals("English"))
		m_lang = new JMenu ("Language");
	else
		m_lang = new JMenu ("Langue");
	m_lang.setIcon (new ImageIcon (icondir + "Terre2.gif"));
	mb.add(m_lang);

	/*JRadioButtonMenuItem*/ JMenuItem mi_francais = new JMenuItem ("Francais");
	mi_francais.setIcon (new ImageIcon (icondir + "Drpfran.gif"));
	mi_francais.addActionListener (new ActionListener ()
		{
			public void actionPerformed (ActionEvent e)
			{
				// Changement de langue : Francais
				/*setState(true);*/
				language = "Francais";
				lang = 0;

				/*
				// frame.setTitle ("Administration des fax reçus");
				frame.setTitle (textes.Admin_fax_rec[lang]);
				// label.setText("Liste des fax reçus");
				// m_file.setText("Fichier"); 
				// m_file.setText (textes.Fichier[lang]);
				// m_lang.setText("Langue");
				m_lang.setText (textes.Langue[lang]);
				// mi_login.setText("Changer de base");
				mi_login.setText (textes.chgbase[lang]);
				mi_quit.setText("Quitter");
				mi_listgroups.setText("Liste des groupes");
				mi_listrights.setText("Liste des droits");
				mi_listusers.setText("Liste des utilisateurs");
				mi_groups.setText("Modifier les groupes");
				mi_rights.setText("Modifier les droits");
				mi_users.setText("Modifier les utilisateurs");
				m_trt.setText("Traitement");
				mi_def.setText("Définir");
				btn_details.setText("Détails");
				btn_apercu.setText("Vignette");
				btn_view.setText("Voir");				
				btn_print.setText("Imprimer");
				btn_refax.setText("Refaxer");
				btn_email.setText("Envoyer par email");
				btn_save.setText("Sauvegarder");
				btn_delete.setText("Supprimer");
				btn_new.setText("Mettre à jour");
				btn_refresh.setText("Réafficher");
				btn_sort.setText("Trier");
				btn_sortrev.setText("Trier");
				btn_select.setText("Sélectionner");
				btn_undo.setText("Annuler");
				*/

				setTextLang(lang);
			}
		});
	m_lang.add(mi_francais);

	JMenuItem mi_english = new JMenuItem ("English");
	mi_english.setIcon (new ImageIcon (icondir + "Drpbrit.gif"));
	mi_english.addActionListener (new ActionListener ()
		{
			public void actionPerformed (ActionEvent e)
			{
				// Changement de langue : Anglais
				/*setState(true);*/
				language = "English";
				lang = 1;
				frame.setTitle ("Administration of received faxes");
				/*bel.setText("List of received faxes");*/   
				/* ile.setTextes("File"); */
				// m_file.setText (textes.Fichier[lang]);
				/*
				m_lang.setText("Language");
				mi_login.setText("Change base");
				mi_quit.setText("Quit");
				mi_listgroups.setText("List of groups");
				mi_listrights.setText("List of rights");
				mi_listusers.setText("List of users");
				mi_groups.setText("Modify groups");
				mi_rights.setText("Modify rights");
				mi_users.setText("Modify users");
				m_trt.setText("Treatment");
				mi_def.setText("Define");
				btn_details.setText("Details");
				btn_apercu.setText("Thumbnail");
				btn_view.setText("View");
				btn_print.setText("Print");
				btn_refax.setText("Refax");
				btn_email.setText("Send by email");
				btn_save.setText("Save");
				btn_delete.setText("Delete");
				btn_new.setText("Update");
				btn_refresh.setText("Refresh");
				btn_sort.setText("Sort");
				btn_sortrev.setText("Sort");
				btn_select.setText("Select");
				btn_undo.setText("Undo");
				*/

				setTextLang(lang);
			}
		});
	m_lang.add(mi_english);

	m_options = new JMenu ("Options");
	mb.add (m_options);
	mi_options = new JMenuItem ("Options");
	mi_options.addActionListener (new ActionListener ()
	{
		public void actionPerformed (ActionEvent e)
		{
			final MyDialog dlg = new MyDialog (frame, "Options", true);
			dlg.setResizable(true);
			dlg.getContentPane().setLayout (null /*new FlowLayout()*/);
			dlg.setSize (400, 500);

			// JLabel l_print = new JLabel (language.equals("Francais")?"Impression":"Printing");
			JLabel l_print = new JLabel (textes.Impression[lang]);

			l_print.setBounds (30, 20, 300, 20);
			dlg.getContentPane().add (l_print);

			ButtonGroup bg_print = new ButtonGroup();

			JRadioButton rb_direct = new JRadioButton (textes.directe[lang], true);
			rb_direct.setBounds(30,50,300,20);
			bg_print.add(rb_direct);
			dlg.getContentPane().add(rb_direct);

			// JRadioButton rb_server = new JRadioButton ("par serveur d'impression", true);
			JRadioButton rb_server = new JRadioButton (textes.srv_imp[lang], true);
			rb_server.setBounds(30,80,300,20);
			bg_print.add(rb_server);
			dlg.getContentPane().add(rb_server);

			// JRadioButton rb_lptps = new JRadioButton ("sur port LPT en Postscript", true);
			JRadioButton rb_lptps = new JRadioButton (textes.port_LPT_PS[lang], true);
			rb_lptps.setBounds(30,110,300,20);
			bg_print.add(rb_lptps);
			dlg.getContentPane().add(rb_lptps);

 			if (print_option == print_direct)
				rb_direct.setSelected(true);
			else if (print_option == print_server)
				rb_server.setSelected(true);
			else if (print_option == print_lptps)
				rb_lptps.setSelected(true);

 			JButton btn_ok = new JButton ("OK");
 			btn_ok.setBounds(30,150,80,20);
 
 			btn_ok.addActionListener (new ActionListener()
					{
						public void actionPerformed (ActionEvent e)
						{
							System.out.println("ok");
							dlg.ok = true;
							dlg.dispose();
							
						}
					});


			dlg.getContentPane().add (btn_ok);
				
			JButton btn_cancel = new JButton (language.equals("Francais")?"Annuler":"Cancel");
			btn_cancel.setBounds(120,150,80,20);
			btn_cancel.addActionListener (new ActionListener()
					{
						public void actionPerformed (ActionEvent e)
						{
							System.out.println("cancel");
							dlg.ok = false;
							dlg.dispose();
						}
					});
			dlg.getContentPane().add(btn_cancel);

			System.out.println ("dialog options");
			dlg.show();
			System.out.println ("ok=" + dlg.ok);

			if (!dlg.ok) return;

			if (rb_direct.isSelected())
				print_option = print_direct;
			else if (rb_server.isSelected())
				print_option = print_server;
			else if (rb_lptps.isSelected())
				print_option = print_lptps;
			else
				print_option = print_none;

			System.out.println ("print_option=" + print_option);

		}
        });

	m_options.add (mi_options); 

	mi_view = new JMenuItem ("Vues");
	mi_view.addActionListener (new ActionListener ()
	{
		public void actionPerformed (ActionEvent evt)
		{
			final MyDialog dlg = new MyDialog (frame, "Vues", false);
			dlg.setResizable (true);
			dlg.getContentPane().setLayout (null);
			dlg.setSize (600, 700);

			JLabel l1 = new JLabel ("Champ");
			l1.setBounds (15, 10, 50, 12);
			dlg.getContentPane().add(l1);

			JLabel l2 = new JLabel ("Taille");
			l2.setBounds (75, 10, 30, 12);
			dlg.getContentPane().add(l2);

			JLabel l3 = new JLabel ("Libellé");
			l3.setBounds (115, 10, 100, 12);
			dlg.getContentPane().add(l3);

			JLabel l4 = new JLabel ("Script");
			l4.setBounds (315, 10, 100, 12);
			dlg.getContentPane().add(l4);

			// final ViewTableModel vtm = new ViewTableModel();
			JTable tv = new JTable (vtm, new ViewTableColumnModel());

			tv.createDefaultColumnsFromModel(); 

			DefaultCellEditor editor = new DefaultCellEditor(new JTextField()); 
			editor.setClickCountToStart(0); 
			tv.setDefaultEditor(String.class, editor); 
			tv.setDefaultEditor(Object.class, editor); 

			tv.setBounds (10, 30, 550, 500);
			dlg.getContentPane().add(tv);

			JButton btn_ok = new JButton ("OK");
 			btn_ok.setBounds(10,540,100,20);
 			btn_ok.addActionListener (new ActionListener()
					{
						public void actionPerformed (ActionEvent e)
						{
							int i;
							String[] champ = new String[vtm.nr];
							int[] taille = new int[vtm.nr];
							String[] libelle = new String[vtm.nr];
							String[] script = new String[vtm.nr];
							for (i=0; i<vtm.nr; i++)
							{
								champ[i] = vtm.data[i][0];
								Integer I;
								try { I = new Integer (vtm.data[i][1]); }
								catch (Exception e1)
								{ I = new Integer(0); }
								taille[i] = I.intValue();
								libelle[i] = vtm.data[i][2];
								script[i] = vtm.data[i][3];
								System.out.println ("script[" + i + "] = " + script[i]);
							}
							
							try {
							String filename = "view.txt";
							PrintWriter q = new PrintWriter( new FileOutputStream(filename), false); 
							nFieldView.i = 0;
							for (int j=0; j<vtm.nr; j++)
							{
								if (!vtm.data[j][0].equals(""))
 								{
									q.println (vtm.data[j][0] + "\t" + vtm.data[j][1] + "\t" + vtm.data[j][2] + "\t" + vtm.data[j][3]);
									nFieldView.i++;
								}
							}
							q.close();
							} catch (Exception ex) { ex.printStackTrace(); }
						
							System.out.println ("creating faxrecn, host=" + host + " port=" + port + " base=" + basefax + " login=" + login + " nr=" + vtm.nr + " nFieldView.i=" + nFieldView.i + " taille 0=" + taille[0]);
							
							faxrecn fr1 = new faxrecn (false, host, port, basefax, login, password, true, nFieldView.i, champ, taille, libelle, script);
							update_thread updt1 = new update_thread(fr1);
							updt1.start();
							restart_thread restart = new restart_thread(fr1);
							restart.start();


							dlg.dispose();
					
						}
						public void actionPerformed1 (ActionEvent e)
						{
							System.out.println("ok");
							dlg.ok = true;

      model = new DefaultTableModel();
//	vMessages = new Vector();
System.out.print("[9]");
	// Liste des fax reçus
	tablefaxrec = new JTable(model);
System.out.print("[10]");
	model.addColumn("Media");
	for (int c=0; c<nColumns; c++)
		model.addColumn(columnNames[c]);
	tablefaxrec_selection();
	/*model.addColumn("***");*/
System.out.print("[11]");
	/*
	for (int l=0; l<n; l++)
		model.addRow(new String[23]);
	*/

	tablefaxrec.setPreferredScrollableViewportSize(new Dimension(500, 300 /*70*/));
System.out.print("[12]");
	/* Largeurs des colonnes */
	if (false)
	{
	tablefaxrec.getColumnModel().getColumn(0).setPreferredWidth(300); /* 400 */
	tablefaxrec.getColumnModel().getColumn(1).setPreferredWidth(0); /* 200 */
	tablefaxrec.getColumnModel().getColumn(2).setPreferredWidth(10); // (200); /* 300 */
	tablefaxrec.getColumnModel().getColumn(3).setPreferredWidth(0); /* 300 */
	tablefaxrec.getColumnModel().getColumn(4).setPreferredWidth(0);
	for (int i=5; i<=22; i++)
		tablefaxrec.getColumnModel().getColumn(i).setPreferredWidth(0); /* 40 */
	tablefaxrec.getColumnModel().getColumn(5).setPreferredWidth(60);
	tablefaxrec.getColumnModel().getColumn(6).setPreferredWidth(60);
	tablefaxrec.getColumnModel().getColumn(8).setPreferredWidth(60);
	tablefaxrec.getColumnModel().getColumn(15).setPreferredWidth(60);
	tablefaxrec.getColumnModel().getColumn(16).setPreferredWidth(60);
	tablefaxrec.getColumnModel().getColumn(17).setPreferredWidth(60);
	}

	tablefaxrec.getColumnModel().getColumn(0).setPreferredWidth(0);
	for (int i=0; i<nColumns; i++)
		tablefaxrec.getColumnModel().getColumn(i+1).setPreferredWidth(sc.aWidth[i]);
System.out.print("[13]");
	pane.remove(scrollPane);
	scrollPane = new JScrollPane(tablefaxrec);
System.out.print("[14]");
	pane.add (scrollPane, BorderLayout.CENTER);



							dlg.dispose();
							
						}
					});
			dlg.getContentPane().add (btn_ok);

			JButton btn_open = new JButton ("Ouvrir");
			btn_open.setBounds(120,540,100,20);
			btn_open.addActionListener (new ActionListener()
			{
				public void actionPerformed (ActionEvent evt)
				{

					JFileChooser fd = new JFileChooser();
					/*fd.setFileView(new nFileView(fd));*/
	
					if (language.equals("English"))
						fd.setDialogTitle("Open the view");
					else
						fd.setDialogTitle("Ouvrir la vue");
					int ret;
					ret = fd.showOpenDialog(frame);
					if (ret == JFileChooser.APPROVE_OPTION)
					{
					   try
					   {
						String filename = fd.getSelectedFile().getPath();
						System.out.println ("filename: " + filename);
						BufferedReader b = new BufferedReader( new InputStreamReader( new FileInputStream(filename))); 
						int i, j;
						for (i=0; i<vtm.nr; i++)
						for (j=0; j<vtm.nc; j++)
							vtm.data[i][j] = "";
						String line;	
						i=0;					
						while ((line = b.readLine()) != null)
						{
							System.out.println ("line read: " + line);
 							String[] field = line.split("\t");
							for (j=0; j<vtm.nc; j++)
                                                        {
                                                                try {
                                                                        vtm.data[i][j] = field[j];
                                                                } catch (Exception e) {
                                                                        vtm.data[i][j] = "";
                                                                }
                                                        }
							i++;
	
						}
						nFieldView.i = i;
						dlg.repaint();
						b.close();
					   }
					   catch (Exception e)
					   {
						e.printStackTrace();
					   }
					}



				}
			});
			dlg.getContentPane().add (btn_open);

			JButton btn_save = new JButton ("Enregistrer");
			btn_save.setBounds(230,540,100,20);
			btn_save.addActionListener (new ActionListener()
			{
				public void actionPerformed (ActionEvent evt)
				{
					
					JFileChooser fd = new JFileChooser();
					/*fd.setFileView(new nFileView(fd));*/
	
					if (language.equals("English"))
						fd.setDialogTitle("Save the view");
					else
						fd.setDialogTitle("Enregistrer la vue");
					int ret;
					ret = fd.showSaveDialog(frame);
					if (ret == JFileChooser.APPROVE_OPTION)
					{
					   try
					   {
						String filename = fd.getSelectedFile().getPath();
						PrintWriter q = new PrintWriter( new FileOutputStream(filename), false); 
						nFieldView.i = 0;
						for (int i=0; i<vtm.nr; i++)
						{
							if (!vtm.data[i][0].equals(""))
 							{
								q.println (vtm.data[i][0] + "\t" + vtm.data[i][1] + "\t" + vtm.data[i][2] + "\t" + vtm.data[i][3]);
								nFieldView.i++;
							}
						}
						q.close();
					   }
					   catch (Exception e)
					   {
						e.printStackTrace();
					   }
					}

				}
			});
			dlg.getContentPane().add (btn_save);


			dlg.show ();


		}
	
	});
	m_options.add (mi_view);


	frame.setJMenuBar(mb);

	System.out.println ("menu created");

	JToolBar tb = new JToolBar();

System.out.print ("[1]");
	/*JButton btlang = new JButton(new ImageIcon(getClass().getResource("images/chooseLang.gif")));*/
	final JButton btlang = new JButton ("Langue");
System.out.print ("[2]");
	btlang.setMargin(new Insets(1, 1, 1, 1));
System.out.print ("[3]");
	btlang.addActionListener( new ActionListener ()
		{
		  public void actionPerformed (ActionEvent e)
 		  {

		    JPopupMenu pm = new JPopupMenu("");
		    
		    for(int i = 0; i < languages.length; i++)
		    {
			boolean state = false;
			
			if(language.equals(languages[i]))
				state = true;
			JCheckBoxMenuItem lm;
			
			/*
			if(i < drapeaux.length && drapeaux[i] != null && !drapeaux[i].equals(""))
				lm = new JCheckBoxMenuItem(languages[i], new ImageIcon(getClass().getResource(drapeaux[i])), state);
			else
				lm = new JCheckBoxMenuItem(languages[i], new ImageIcon(getClass().getResource("images/drapeaux/unknown.gif")), state);
			lm.addActionListener(new evt(this, "ChangeLangue "+languages[i]));
			pm.add(lm);
			*/
			lm = new JCheckBoxMenuItem (languages[i]);
			/*
			lm.addActionListener (new ActionListener ()
				{
					public void actionPerformed (ActionEvent e)
					{
					}
				});
			*/
			lm.addActionListener(new changelangue(fr, languages[i]));
			pm.add(lm);
			
		    }
		    pm.show(btlang, 0, btlang.getHeight());
			
	        }
		});
System.out.print("[4]");
	/*tb.add(btlang);*/
	pane.add(tb);
System.out.print("[5]");
        final JLabel label = new JLabel(""); /* Titre */
System.out.print("[6]");
	statusline = new JLabel ("OK");
System.out.print("[7]");

	// JPanel panelabel = new JPanel ();
	// panelabel.setLayout (new FlowLayout());
	label.setText ("Host: " + host + "  Port: " + port + "  Base: " + basefax  + "  Login: " + login);
	// panelabel.add (label);
      // pane.add(panelabel, BorderLayout.NORTH);
	pane.add (label, BorderLayout.NORTH);

System.out.print("[8]");
	// tabledata = new Object[/*maxfax*/ n][24];

	/*tablefaxrec = new JTable (tabledata, columnNames);*/
      model = new DefaultTableModel();
	// model = new ListTableModel();
	vMessages = new Vector();
	vmessagesd = new Vector();
System.out.print("[9]");
	// Liste des fax reçus
	tablefaxrec = new JTable(model);
System.out.print("[10]");
	// model.addColumn("Media"); // modif vues
        System.out.println ("nColumns=" + nColumns);
	/*for (int c=0; c<nColumns; c++)
		model.addColumn(columnNames[c]);*/
	/* modif vues */
	for (int c=0; c<nfields_view; c++)
		model.addColumn (text_view[c]);
	tablefaxrec_selection();
	/*model.addColumn("***");*/
System.out.print("[11]");
	/*
	for (int l=0; l<n; l++)
		model.addRow(new String[23]);
	*/

	tablefaxrec.setPreferredScrollableViewportSize(new Dimension(500, 300 /*70*/));
System.out.print("[12]");
	/* Largeurs des colonnes */
	if (false)
	{
	tablefaxrec.getColumnModel().getColumn(0).setPreferredWidth(300); /* 400 */
	tablefaxrec.getColumnModel().getColumn(1).setPreferredWidth(0); /* 200 */
	tablefaxrec.getColumnModel().getColumn(2).setPreferredWidth(200); /* 300 */
	tablefaxrec.getColumnModel().getColumn(3).setPreferredWidth(0); /* 300 */
	tablefaxrec.getColumnModel().getColumn(4).setPreferredWidth(0);
	for (int i=5; i<=22; i++)
		tablefaxrec.getColumnModel().getColumn(i).setPreferredWidth(0); /* 40 */
	tablefaxrec.getColumnModel().getColumn(5).setPreferredWidth(60);
	tablefaxrec.getColumnModel().getColumn(6).setPreferredWidth(60);
	tablefaxrec.getColumnModel().getColumn(8).setPreferredWidth(60);
	tablefaxrec.getColumnModel().getColumn(15).setPreferredWidth(60);
	tablefaxrec.getColumnModel().getColumn(16).setPreferredWidth(60);
	tablefaxrec.getColumnModel().getColumn(17).setPreferredWidth(60);
	}

//        tablefaxrec.getColumnModel().getColumn(0).setPreferredWidth(0);
// modif vues
/*
	for (int i=0; i<nColumns; i++)
        {
                try {
                System.out.print ("sc=" + sc + " i=" + i );
                System.out.println (" width=" + sc.aWidth[i]);
                tablefaxrec.getColumnModel().getColumn(i+1).setPreferredWidth(sc.aWidth[i]);
                } catch (Exception e1) { e1.printStackTrace(); }
        }
*/
        System.out.println ("nfields_view=" + nfields_view);
        System.out.println ("size_view=" + size_view);
        // System.out.println ("length=" + size_view.length);
	for (int i=0; i<nfields_view; i++)
        {
                System.out.println ("i=" + i);
                System.out.println (" size=" + size_view[i]);
                tablefaxrec.getColumnModel().getColumn(i).setPreferredWidth(size_view[i]);
        }

System.out.print("[13]");
	scrollPane = new JScrollPane(tablefaxrec);
System.out.print("[14]");
	pane.add (scrollPane, BorderLayout.CENTER);
System.out.print("[15]");
	pane.add (statusline, BorderLayout.SOUTH);
System.out.print("[16]");
	// frame.getContentPane().add(label);

	JPanel paneb = new JPanel();
	paneb.setLayout (new BorderLayout());

	JPanel panev = new JPanel ();
	panev.setLayout (new BorderLayout());

	
	vignette = new Vignette(null);
	vignette.fr = this;
      // frame.getContentPane().add (vignette);
	// paneb.add (vignette, BorderLayout.WEST);
	panev.add (vignette, BorderLayout.CENTER);

	/*
	vsb = new JScrollBar(JScrollBar.HORIZONTAL, 30, 10, 0, 100);
	vsb.addAdjustmentListener (new AdjustmentListener () {
		public void adjustmentValueChanged(AdjustmentEvent e)
		{
			vignette.repaint();
		}
	});
	// paneb.add (vsb, BorderLayout.SOUTH);
	panev.add (vsb, BorderLayout.SOUTH);
	*/

	paneb.add (panev, BorderLayout.WEST);

	frame.getContentPane().add(pane);
System.out.print("[17]");
	// label.setText ("Liste des fax reçus");
	// label.setText ("Host: " + host + "  Port: " + port + "  Base: " + basefax  + "  Login: " + login);
	// label.setBounds (100, 120, 500, 30);
System.out.print("[18]");
	// label.setIcon (new ImageIcon("c:\\jacques\\icones\\para_bul.bmp"));
System.out.print("[19]");
	JPanel panebtn = new JPanel ();
System.out.print("[20]");
	panebtn.setLayout (new FlowLayout());
 
	System.out.println ("Creation des boutons...");

	// panebtn.add (vignette);
	btn_details = new JButton ("Détails");
	btn_details.setIcon (new ImageIcon (icondir + "detlicon.gif"));
	btn_details.addActionListener (new ActionListener() 
		{
			public void actionPerformed (ActionEvent e)
			{
				int i = tablefaxrec.getSelectedRow();
				Integer I = new Integer(i);
				System.out.println ("Détails du fax " + I.toString());
				/*String filename = (String)tablefaxrec.getValueAt(i,1);*/

				String titre;
				if (language.equals ("Francais"))
					titre = "Détails du fax";
				else
					titre = "Details of the fax";

				 
				JDialog dlg = new JDialog (frame, titre, false);
				dlg.setResizable (true);
				
				/*dlg.getContentPane().setLayout(null);*/
				/*
				JLabel lbl_filename = new JLabel ("Fichier : ");
				dlg.getContentPane().add (lbl_filename);
				JTextField tf_filename = new JTextField (filename);
				dlg.getContentPane().add (tf_filename);
				*/
				JLabel lbls[] = new JLabel[nColumns];
				JLabel vals[] = new JLabel[nColumns];
				int lang;
				if (language.equals("Francais"))
					lang = 0;
				else
					lang = 1;
				String fieldcont;
				
				if (i<0)
				{
					dlg.getContentPane().setLayout(new GridLayout(0,1,5,5));
				      dlg.setSize (300, 100);
					JLabel lbl_message;
					String message;
					if (language.equals("Francais"))
						message = "Vous devez sélectionner un fax dans la liste";
					else
						message = "You must select a fax in the list";
					lbl_message = new JLabel (message);
					dlg.getContentPane().add (lbl_message);
				}
				else
				{
				    dlg.getContentPane().setLayout(new GridLayout(0,2,5,5));
				    dlg.setSize (500, 600);
 				    for (int field=0; field<nColumns; field++)
				    {
			
					/*lbls[field].setText (noms_champs[field][lang]);*/
					/*lbls[field] = new JLabel ("   " + noms_champs[field][lang]);*/
					lbls[field] = new JLabel ("   " + sc.aDesc[field]);
					dlg.getContentPane().add (lbls[field]);
					// fieldcont = (String)tablefaxrec.getValueAt(i,field+1);
					fieldcont = (String)(((MessageInfo)(messages.elementAt(i))).message.values.elementAt(field+1));
					/*vals[field].setText (fieldcont);*/
					vals[field] = new JLabel (fieldcont);
					dlg.getContentPane().add (vals[field]);
				    }
				}
				
				JButton btn_ok = new JButton ("OK");
				/*
				btn_ok.addActionListener (new ActionListener ()
					{
					    public void actionPerformed (ActionEvent e)
					    {
						dlg.dispose();
					    }
					});
				*/
				
				btn_ok.addActionListener (new devt (dlg));
				/*dlg.getContentPane().add (btn_ok);*/
			
				dlg.show();
				
			}
		});
	panebtn.add (btn_details);

	System.out.println ("Bouton details cree");

	btn_apercu = new JButton ("Vignette");
	btn_apercu.addActionListener (new ActionListener() 
		{
			public void actionPerformed (ActionEvent e)
			{
				int i = tablefaxrec.getSelectedRow();
				Integer I = new Integer(i);
				System.out.println ("Vignette du fax " + I.toString());

				String filename = ((MessageInfo)(messages.elementAt(i))).message.getField("FILE");
				System.out.println ("Voir le fax " + I.toString() + " fichier:" + filename);

				/* connexion */

				boolean status = sc.login (basefax, login, password);
				InputStream is = sc.is;
				OutputStream os = sc.os;

				String com;
				String commande;

				// On demande au serveur de creer la vignette
				sc.output ("THUMBNAIL\n" + filename + "\n");

				String reponse;
				reponse = sc.input("any");

			}
		});
	panebtn.add (btn_apercu);

	btn_view = new JButton ("Voir");
	btn_view.setIcon (new ImageIcon (icondir + "Voir.gif"));
	btn_view.addActionListener (new ActionListener() 
		{
			public void actionPerformed (ActionEvent e)
			{
			    try
			    {
				// Numéro du fax sélectionné dans la liste
				int i = tablefaxrec.getSelectedRow();
				Integer I = new Integer(i);
				/*String filename = (String)tablefaxrec.getValueAt(i,1);*/
				/*String filename = ((GenericMessage)(vMessages.elementAt(i))).getField("FILE");*/
				// String filename = (String)tablefaxrec.getValueAt(i,index1("FILE"));
                                System.out.println ("vmessagesd=" + vmessagesd);
                        // String filename = ((GenericMessage)(vMessages.elementAt(tablefaxrec.getRowCount()-1-i))).getField("FILE");
                        String filename = ((MessageInfo)(messages.elementAt(i))).message.getField("FILE");
				System.out.println ("Voir le fax " + I.toString() + " fichier:" + filename);

				/* connexion */

				boolean status = sc.login (basefax, login, password);
				InputStream is = sc.is;
				OutputStream os = sc.os;

/*** le code ci-dessous est remplacé par sc.login ***

				Socket sock = new Socket ("localhost", 6020);
				InputStream is = sock.getInputStream();
				OutputStream os = sock.getOutputStream();
***/				
				String com;
				String commande;

/***
	// envoi du login et du mot de passe
	com = new String("USER\n" + login +"\n" + "PASS\n" + password +"\n");
	os.write(com.getBytes("US-ASCII"));
	os.flush();
	commande = new String(recoit_param(is, "alpha"));
	System.out.println ("received(1): " + commande);

	commande = new String(recoit_param(is, "alpha"));
	System.out.println ("received(2): " + commande);
***/

 
 

					// On demande au serveur le contenu du fax
					sc.output ("FAXDATA\n" + filename + "\n");

					String reponse;
					/*byte*/ int[] data;

					int nbrpages = 0;
					Vector pages = new Vector();
					Vector sizes = new Vector();
					for (;;)
					{
						reponse = sc.input ("any");
						if (!reponse.equals("FAXDATA"))
							break;

						reponse = sc.input ("any");
						Integer NP = new Integer(reponse);
						int np = NP.intValue();
			
						reponse = sc.input ("any");
						Integer N = new Integer(reponse);
						int n = N.intValue();

						/* data = sc.inputBytes (n); */
						data = sc.inputInts(n);
						pages.addElement (data);
						sizes.addElement (new Integer(n));

						reponse = sc.input ("any");
						nbrpages++;
					}

				   // pages contient les pages du fax et nbrpages le nombre de pages

				   if (nbrpages <= 0)
				   {
					System.out.println ("Donnees non recues");
					if (language.equals("English"))
						JOptionPane.showMessageDialog(null, "You are not authorized to access to this fax", "Reception of fax", JOptionPane.WARNING_MESSAGE);      
					else
						JOptionPane.showMessageDialog(null, "Vous n'êtes pas autorisé à accéder à ce fax", "Réception du fax", JOptionPane.WARNING_MESSAGE); 
				   }

					sc.logout ();


				// Création du viewer
				FaxViewer fv = new FaxViewer();
				fv.print_option = print_option;
				fv.language = language;
				fv.nbrpages = nbrpages;
				fv.pages = pages;
				fv.sizes = sizes;
				fv.titre = null;
				fv.frame = frame; /* pour compiler la 1ere fois enlever cette ligne 
									puis compiler FaxViewer puis remettre cette ligne et recompiler
								*/
				// Visualisation du fax
				int row = tablefaxrec.getSelectedRow();
				int col = tablefaxrec.getSelectedColumn();
				fv.view();
				tablefaxrec.setRowSelectionInterval (row, row);
				tablefaxrec.setColumnSelectionInterval (col, col);

/***
				String titre;
				if (language.equals ("Francais"))
					titre = "Visualisation du fax";
				else
					titre = "Visualisation of the fax";
				 
				final FaxImageDialog dlg = new FaxImageDialog (frame, titre, false);
				dlg.setResizable (true);
				
 
				JLabel lbls[] = new JLabel[nColumns];
				JLabel vals[] = new JLabel[nColumns];
 
				if (language.equals("Francais"))
					lang = 0;
				else
					lang = 1;
				String fieldcont;
				
				if (i<0)
				{
					dlg.getContentPane().setLayout(new GridLayout(0,1,5,5));
				      dlg.setSize (300, 100);
					JLabel lbl_message;
					String message;
					if (language.equals("Francais"))
						message = "Vous devez sélectionner un fax dans la liste";
					else
						message = "You must select a fax in the list";
					lbl_message = new JLabel (message);
					dlg.getContentPane().add (lbl_message);
				}
				else
				{
 
				    BorderLayout layout = new BorderLayout();
				    dlg.getContentPane().setLayout(layout);
				    dlg.setSize (880, 700);
  
				final FaxImage faximage;


				    faximage = new FaxImage();
				    faximage.curpage = 0;
				    faximage.nbrpages = nbrpages;
				    faximage.pages = pages;
				    faximage.sizes = sizes;
				    dlg.faximage = faximage;
				    dlg.getContentPane().add (faximage, BorderLayout.CENTER);
				
				
				final JScrollBar vsb = new JScrollBar (JScrollBar.VERTICAL);
				vsb.getModel().addChangeListener (new ChangeListener ()
					{
						public void stateChanged (ChangeEvent e)
						{
							dlg.faximage.vpos1 = dlg.faximage.vpos;
							dlg.faximage.vpos = vsb.getValue() * (faximage.imageHeight) / (vsb.getMaximum() - vsb.getMinimum());
							if (dlg.faximage.vpos != dlg.faximage.vpos1)
								dlg.faximage.paintfax();
						}
					});
				faximage.vsb = vsb;

 

				dlg.getContentPane().add (vsb, BorderLayout.EAST);

 
				}

				JPanel panebtnview = new JPanel();				
				panebtnview.setLayout (new FlowLayout());

 

				JButton btn_first = new JButton(); 
				btn_first.setIcon (new ImageIcon(icondir+"first.gif"));
			      btn_first.addActionListener (new ActionListener ()
					{
						public void actionPerformed (ActionEvent e)
						{
 
								dlg.faximage.curpage = 0;
								dlg.faximage.tv = null;
								dlg.faximage.paintfax();
								dlg.lpage.setText ("Page " + (dlg.faximage.curpage+1) + " / " + dlg.faximage.nbrpages);
 
						}
					});
				panebtnview.add (btn_first);                 				

				JButton btn_prev = new JButton (); 
				btn_prev.setIcon (new ImageIcon(icondir+"prev.gif"));
			      btn_prev.addActionListener (new ActionListener ()
					{
						public void actionPerformed (ActionEvent e)
						{
							if (dlg.faximage.curpage > 0)
							{
								dlg.faximage.curpage--;
								dlg.faximage.tv = null;
								dlg.faximage.paintfax();
								dlg.lpage.setText ("Page " + (dlg.faximage.curpage+1) + " / " + dlg.faximage.nbrpages);
							}				
						}
					});
				panebtnview.add (btn_prev);

				JLabel lpage = new JLabel();
				panebtnview.add (lpage);
				lpage.setText ("Page 1 / " + nbrpages);
 				dlg.lpage = lpage;

				JButton btn_next = new JButton(); 
				btn_next.setIcon (new ImageIcon(icondir+"next.gif"));
			      btn_next.addActionListener (new ActionListener ()
					{
						public void actionPerformed (ActionEvent e)
						{
							if (dlg.faximage.curpage < dlg.faximage.nbrpages-1)
							{
								dlg.faximage.curpage++;
								dlg.faximage.tv = null;
								dlg.faximage.paintfax();
								dlg.lpage.setText ("Page " + (dlg.faximage.curpage+1) + " / " + dlg.faximage.nbrpages);
							}
						}
					});
				panebtnview.add (btn_next);                 

				JButton btn_last = new JButton (); 
				btn_last.setIcon (new ImageIcon(icondir+"last.gif"));
			      btn_last.addActionListener (new ActionListener ()
					{
						public void actionPerformed (ActionEvent e)
						{
 
								dlg.faximage.curpage = dlg.faximage.nbrpages-1;
								dlg.faximage.tv = null;
								dlg.faximage.paintfax();
								dlg.lpage.setText ("Page " + (dlg.faximage.curpage+1) + " / " + dlg.faximage.nbrpages);
 
						}
					});
				panebtnview.add (btn_last);                 


				JButton btn_printpage = new JButton (language.equals("Francais")?"Imprimer":"Print");				
				btn_printpage.setIcon (new ImageIcon(icondir+"print3.gif"));
				btn_printpage.addActionListener (new ActionListener()
					  						public void actionPerformed (ActionEvent e)
						{

MyPainter mypainter = new MyPainter ("Ceci est un test d'impression");
FaxPainter faxpainter = new FaxPainter (dlg.faximage);
PrinterJob pj = PrinterJob.getPrinterJob();
PageFormat pf = pj.defaultPage();
pj.setPrintable(faxpainter,pf);

PrintRequestAttributeSet pras = new HashPrintRequestAttributeSet ();
pras.add (new PageRanges (1, dlg.faximage.nbrpages));


if ( pj.printDialog(pras) )
try {
pj.print(); }
catch(PrinterException pe) {
System.out.println("Erreur d'impression : " + pe.getMessage());}

	 
						}

						
					});
				panebtnview.add(btn_printpage);

 
				dlg.getContentPane().add (panebtnview, BorderLayout.NORTH);
				
				if (nbrpages > 0)
					dlg.show();
***/			
				


			    }
			    catch (Exception ex)
			    {
				ex.printStackTrace();
				System.out.println ("Error view: " + ex.getMessage());
			    }


			}
		});
	panebtn.add (btn_view);

	btn_print = new JButton ("Imprimer");
	btn_print.setIcon (new ImageIcon ("c:/jacques/icones/print3.gif"));
	btn_print.addActionListener (new ActionListener() 
		{
			public void actionPerformed (ActionEvent e)
			{
				String port = "";

				System.out.println("print");
				/* int i = tablefaxrec.getSelectedRow(); */
				// numéros des fax sélectionnés
				int[] sel_fax = tablefaxrec.getSelectedRows();				
				System.out.println (" " + sel_fax.length + " faxsélectionnés");

				boolean status_printdialog = false;

				PrinterJob pj = PrinterJob.getPrinterJob();
				System.out.println ("PrinterJob cree");
				PageFormat pf = pj.defaultPage();
	
				PrintRequestAttributeSet pras = new HashPrintRequestAttributeSet ();
				/*pras.add (new PageRanges (1, nbrpages));*/
				pras.add (PrintQuality.HIGH);
				System.out.println ("Attribute set defined");

				status_printdialog = true;

				if (sel_fax.length != 1)  /* plusieurs ou aucun fax sélectionnés */
				{
					status_printdialog = pj.printDialog(pras);
				}
				
					System.out.println("status printdialog="+status_printdialog);
					if (!status_printdialog)
						System.out.println("print cancelled");
					else
					{
						System.out.println ("printing...");
				
				
				flag_refresh = false;
				
				try
				{
				for (int j=0; j<sel_fax.length; j++)
				{
				int i = sel_fax[j];
				
				System.out.println ("Imprimer le fax " + i);

				String filename = ((MessageInfo)(messages.elementAt(i))).message.getField("FILE");
				/*String filename = ((GenericMessage)(vMessages.elementAt(i))).getField("FILE");*/

 
				/* connexion */

				boolean status = sc.login (basefax, login, password);
				InputStream is = sc.is;
				OutputStream os = sc.os;

				String com;
				String commande;
 
					sc.output ("FAXDATA\n" + filename + "\n");

					String reponse;
					/*byte*/ int[] data;

					int nbrpages = 0;
					Vector pages = new Vector();
					Vector sizes = new Vector();
					for (;;)
					{
						reponse = sc.input ("any");
						if (!reponse.equals("FAXDATA"))
							break;

						reponse = sc.input ("any");
						Integer NP = new Integer(reponse);
						int np = NP.intValue();
			
						reponse = sc.input ("any");
						Integer N = new Integer(reponse);
						int n = N.intValue();

						/* data = sc.inputBytes (n); */
						data = sc.inputInts(n);
						pages.addElement (data);
						sizes.addElement (new Integer(n));

						reponse = sc.input ("any");
						nbrpages++;
					}

				   if (nbrpages <= 0)
				   {
					System.out.println ("Donnees non recues, reponse:" + reponse);
					if (language.equals("English"))
						JOptionPane.showMessageDialog(null, "You are not authorized to access to received faxes", "Reception of fax", JOptionPane.WARNING_MESSAGE);      
					else
						JOptionPane.showMessageDialog(null, "Vous n'êtes pas autorisé à accéder aux fax reçus", "Réception du fax", JOptionPane.WARNING_MESSAGE); 
				   }

					sc.logout ();


				/*

				PrinterJob pj = PrinterJob.getPrinterJob();
				PageFormat pf = pj.defaultPage();

				PrintRequestAttributeSet pras = new HashPrintRequestAttributeSet ();
				*/
				if (sel_fax.length == 1) /* un seul fax sélectionné */
				{
					pras.add (new PageRanges (1, nbrpages));
					if (print_option == print_lptps)
					{
						final PortDialog dlg = new PortDialog (frame, "Impression", true);
						dlg.language = language;
/*
						dlg.setResizable(true);

						dlg.getContentPane().setLayout (null);  
						dlg.setSize (300, 400);

						JLabel lport = new JLabel(language.equals("Francais")?"Port de l'imprimante":"Printer port");
						lport.setBounds (30, 40, 100, 20);				
						dlg.getContentPane().add (lport);
				
						JTextField tport = new JTextField (30);
						tport.setBounds (140, 40, 120, 20);
						dlg.getContentPane().add (tport);

						JButton btn_ok = new JButton ("OK");
						btn_ok.setBounds (30, 300, 80, 20);
						btn_ok.addActionListener (new ActionListener()
						{
							public void actionPerformed (ActionEvent e)
							{
								dlg.ok = true;
								dlg.dispose();							
							}
						});
						dlg.getContentPane().add (btn_ok);
				
						JButton btn_cancel = new JButton (language.equals("Francais")?"Annuler":"Cancel");
						btn_cancel.setBounds (140, 300, 80, 20);
						btn_cancel.addActionListener (new ActionListener()
							{
								public void actionPerformed (ActionEvent e)
								{
									dlg.ok = false;
									dlg.dispose();
								}
							});
						dlg.getContentPane().add(btn_cancel);
*/
						dlg.show();
						port = dlg.tport.getText();
						System.out.println ("Port:" + port);

						status_printdialog = dlg.ok;
					}
					else
						status_printdialog =  pj.printDialog(pras);
					
				}

				if (status_printdialog)
				{
					/*JOptionPane.showInternalMessageDialog(null, 
	   language.equals("Francais")?"Impression en cours...":"Printing...",
language.equals("Francais")?"Administration des fax reçus":"Administration of received faxes", 
JOptionPane.INFORMATION_MESSAGE);*/

				/*
				Message m = new Message (frame, "Printing...");
				m.start();
					*/

					FaxImage fi = new FaxImage();
					fi.curpage = 0;
					fi.nbrpages = nbrpages;
					fi.pages = pages;
					fi.sizes = sizes;

					FaxPainter fp = new FaxPainter (fi);
					pj.setPrintable (fp, pf);
					try
					{
						if (print_option == print_direct)
						{
							System.out.println ("Impression directe");
							pj.print();
						}
						else if (print_option == print_server)
						{
							System.out.println ("Impression par serveur");
							PrintServerJob psj = new PrintServerJob ();
							psj.printer = pj.getPrintService().getName();
							psj.faximage = fi;
							psj.print();
						} 
						else if (print_option == print_lptps)
						{
							CustomPrinter p = new CustomPrinter ();
							p.port = port;
							p.faximage = fi;
							p.print_option = print_option;
							p.print();
						}
						
					}
					catch (Exception e1)
					{
						System.out.println ("Erreur d'impression : " + e1.getMessage());
					}


		/* ***
					for (int curpage=0; curpage<nbrpages; curpage++)
					{

		TiffViewer tv;

		System.out.println ("page " + curpage);
		int[] page = (int[])(pages.elementAt(curpage));
		System.out.println ("taille");
		int size = ((Integer)(sizes.elementAt(curpage))).intValue();
		System.out.println ("taille: " + size);
		System.out.println ("Création TiffViewer");
						 
			tv = new TiffViewer (page, size);

			// TiffViewer tv = new TiffViewer((int[])(pages.elementAt(curpage)),((Integer)(sizes.elementAt(curpage))).intValue()); 
			System.out.println ("Conversion");
			tv.convert();
			
			FaxPainter fp = new FaxPainter (tv);

			pj.setPrintable(fp,pf);
			// if ( pj.printDialog() )
			try {
			pj.print(); }
			catch(PrinterException pe) 
			{
				System.out.println("Erreur d'impression : " + pe.getMessage());
			}

					}
		*** */

					/*m.setVisible(false);*/
					/* m.stop(); */
				}

				}
                                }
                                catch (Exception e1)
				{
                                        e1.printStackTrace();
				}

				flag_refresh = true;
			
				}
				
			}
		});
	panebtn.add (btn_print);

	btn_refax = new JButton ("Refaxer");
	btn_refax.setIcon (new ImageIcon (icondir + "MODEM3.gif"));

	btn_refax.addActionListener (new ActionListener() 
		{
			public void actionPerformed (ActionEvent e)
			{
				Integer I = new Integer(tablefaxrec.getSelectedRow());
				int i = I.intValue();
				System.out.println ("Refaxer le fax " + I.toString());
				
				/*String filename = (String)tablefaxrec.getValueAt(i,1);*/
				int[] sel_fax = tablefaxrec.getSelectedRows();
				String filename = "";
				for (int j=0; j<sel_fax.length; j++)
				{
					if (j>0) filename += "\t";
					// filename += (String)tablefaxrec.getValueAt(sel_fax[j],index1("FILE"));
					filename += ((MessageInfo)(messages.elementAt(sel_fax[j]))).message.getField("FILE");
					/*filename += ((GenericMessage)(vMessages.elementAt(i))).getField("FILE");*/
				}
				

                        // String filename = ((MessageInfo)(messages.elementAt(i))).message.getField("FILE");

				String s_rtr = ((MessageInfo)(messages.elementAt(i))).message.getField("RTR");
				String s_line = ((MessageInfo)(messages.elementAt(i))).message.getField("LINE");
				String id = ((MessageInfo)(messages.elementAt(i))).message.getField("ID");
								
				// String s_rtr = (String)tablefaxrec.getValueAt(i,/*9*/index1("RTR"));
				// String s_line = (String)tablefaxrec.getValueAt(i,/*10*/index1("LINE"));
				// String id = (String)tablefaxrec.getValueAt(i,/*0*/index1("ID"));
				
				/*
				String s_rtr = ((GenericMessage)(vMessages.elementAt(i))).getField("RTR");
				String s_line = ((GenericMessage)(vMessages.elementAt(i))).getField("LINE");
				String id = ((GenericMessage)(vMessages.elementAt(i))).getField("ID");
				*/
				String called = ""; /* provisoirement */
				String calling = "";

				final MyDialog dlg = new MyDialog (frame, 
					language.equals("Francais")?"Refaxer le fax reçu":"Refax the received fax",
					 true);
				dlg.setResizable(true);

				dlg.getContentPane().setLayout (null /*new FlowLayout()*/);
				dlg.setSize (300, 400);

				String essais = "3";
				String priorite = "8";
				String delai = "120";
				String ident = "Fax-Mailer";

				File f = new File (configFile);
                                try {
				fichierINI fi = new fichierINI(null, f);
				if (f.exists())
				{
					essais = fi.getParametre ("Refax", "Retry");
					priorite = fi.getParametre ("Refax", "Priority");
					delai = fi.getParametre ("Refax", "Delay");
					ident = fi.getParametre ("Refax", "Id");
				}

                                } catch (Exception exc) { }

				JLabel lnum = new JLabel(language.equals("Francais")?"Numéro de fax":"Fax number");
				lnum.setBounds (30, 40, 100, 20);				
				dlg.getContentPane().add (lnum);
				
				JTextField tfnumfax = new JTextField (30);
				tfnumfax.setBounds (140, 40, 120, 20);
				dlg.getContentPane().add (tfnumfax);

				JLabel lretry = new JLabel(language.equals("Francais")?"Essais":"Retries");
				lretry.setBounds (30, 70, 100, 20);
				dlg.getContentPane().add (lretry);

				JTextField tfretry = new JTextField (30);
				tfretry.setBounds (140, 70, 120, 20);
				tfretry.setText (essais);
				dlg.getContentPane().add (tfretry);

				JLabel lpri = new JLabel (language.equals("Francais")?"Priorité":"Priority");
				lpri.setBounds (30, 100, 100, 20);
				dlg.getContentPane().add(lpri);

				JTextField tfpri = new JTextField (30);
				tfpri.setBounds (140, 100, 120, 20);
				tfpri.setText (priorite);
				dlg.getContentPane().add(tfpri);
		
				JLabel ldelay = new JLabel (language.equals("Francais")?"Delai":"Delay");
				ldelay.setBounds (30, 130, 100, 20);
				dlg.getContentPane().add(ldelay);

				JTextField tfdelay = new JTextField (30);
				tfdelay.setBounds (140, 130, 120, 20);
				tfdelay.setText (delai);
				dlg.getContentPane().add(tfdelay);

				JLabel lid = new JLabel (language.equals("Francais")?"Identifiant":"Identifier");
				lid.setBounds (30, 160, 100, 20);
				dlg.getContentPane().add(lid);

				JTextField tfid = new JTextField (30);
				tfid.setBounds (140, 160, 120, 20);
				tfid.setText(ident);
				dlg.getContentPane().add(tfid);

				JButton btn_ok = new JButton ("OK");
				btn_ok.setBounds (30, 300, 80, 20);
				btn_ok.addActionListener (new ActionListener()
					{
						public void actionPerformed (ActionEvent e)
						{
							dlg.ok = true;
							dlg.dispose();							
						}
					});
				dlg.getContentPane().add (btn_ok);
				
				JButton btn_cancel = new JButton (language.equals("Francais")?"Annuler":"Cancel");
				btn_cancel.setBounds (140, 300, 80, 20);
				btn_cancel.addActionListener (new ActionListener()
					{
						public void actionPerformed (ActionEvent e)
						{
							dlg.ok = false;
							dlg.dispose();
						}
					});
				dlg.getContentPane().add(btn_cancel);

				dlg.show();
				System.out.println ("refax: " + dlg.ok + " " + tfnumfax.getText());

				if (dlg.ok)
				{
					String numfax = tfnumfax.getText();
					String retry = tfretry.getText();
					String pri = tfpri.getText();
					String delay = tfdelay.getText();
					id = tfid.getText();

					sc.login (basefax, login, password);
					sc.output ("REFAX\n" 
						+ numfax + "\n" 
						+ retry + "\n"
						+ pri + "\n"
						+ delay + "\n"
						+ id + "\n"
						+ filename + "\n"
						/*
						+ s_rtr + "\n"
						+ s_line + "\n"
						+ id + "\n"
						+ called + "\n"
						+ calling + "\n"
						*/);
					String reponse = sc.input ("any");
					if (reponse.equals ("NOTAUTH"))
					{
						JOptionPane.showMessageDialog (null, 
							language.equals("English")?"You are not authorized to refax this fax":"Vous n'êtes pas autorisé à refaxer ce fax", 
							language.equals("Francais")?"Refaxer":"Refax", 
							JOptionPane.WARNING_MESSAGE); 
					}
			 		else if (reponse.equals ("OK"))
					{
						JOptionPane.showMessageDialog (null, 
							language.equals("English")?"This fax has been queued for being refaxed":"Ce fax a été mis en file d'attente pour être refaxé" , 
							language.equals("Francais")?"Refaxer":"Refax",  
							JOptionPane.WARNING_MESSAGE); 
					}
					else /*if (reponse.equals ("ERROR"))*/
					{
						JOptionPane.showMessageDialog (null, 
							language.equals("English")?"Error":"Erreur", 
							language.equals("Francais")?"Refaxer":"Refax",  
							JOptionPane.WARNING_MESSAGE); 
					}

				/* envoi en local 
					Fax f = new Fax ();
					f.numfax = tfnumfax.getText();
					
					sc.login (basefax, login, password);

					sc.output ("FAXDATA\n" + filename + "\n");

					String reponse;
					byte[] data;

					for (;;)
					{
						reponse = sc.input ("any");
						if (!reponse.equals("FAXDATA"))
							break;

						reponse = sc.input ("any");
						Integer NP = new Integer(reponse);
						int np = NP.intValue();
			
						reponse = sc.input ("any");
						Integer N = new Integer(reponse);
						int n = N.intValue();

						data = sc.inputBytes (n);
						f.addPage (data);

						reponse = sc.input ("any");
					}

					sc.logout ();

					f.send();
				*/
				}
				
				
			}
		});
	panebtn.add (btn_refax);

	btn_email = new JButton ("Envoyer par email");
	btn_email.setIcon (new ImageIcon (icondir + "mail2b.gif" /*"MAIL-OUT.gif"*/));
	btn_email.addActionListener (new ActionListener() 
		{
			public void actionPerformed (ActionEvent e)
			{
			   flag_refresh = false;
			   try
			   {

			   boolean flag_ps = true;
			   try
			   {
				int i = tablefaxrec.getSelectedRow();
				Integer I = new Integer(i);
				System.out.println ("Envoyer par email le fax " + I.toString());

				/*String filename = (String)tablefaxrec.getValueAt(i,1);*/
				int[] sel_fax = tablefaxrec.getSelectedRows();
				String filename = "";
				for (int j=0; j<sel_fax.length; j++)
				{
					if (j>0) filename += "\t";
					filename += ((MessageInfo)(messages.elementAt(sel_fax[j]))).message.getField("FILE");
					// filename += (String)tablefaxrec.getValueAt(sel_fax[j],/*1*/index1("FILE"));
					/*filename += ((GenericMessage)(vMessages.elementAt(sel_fax[j]))).getField("FILE");*/

				}

				String titre;
				if (language.equals ("Francais"))
					titre = "Envoi du fax par email";
				else
					titre = "Send the fax by email";

				final MyDialog dlg = new MyDialog (frame, titre, false);
				dlg.setResizable (true);

				int lang;
				if (language.equals("Francais"))
					lang = 0;
				else
					lang = 1;
				String fieldcont;
				
				JTextField tfsmtp = null;
				JTextField tfemail = null;
				JTextField tfsubject = null;
				JTextField tftext = null;
				JTextField tffrom = null;

				if (i<0)
				{
					dlg.getContentPane().setLayout(new GridLayout(0,1,5,5));
				      dlg.setSize (300, 100);
					JLabel lbl_message;
					String message;
					if (language.equals("Francais"))
						message = "Vous devez sélectionner un fax dans la liste";
					else
						message = "You must select a fax in the list";
					lbl_message = new JLabel (message);
					dlg.getContentPane().add (lbl_message);
				}
				else
				{
 
				    /*
					BorderLayout layout = new BorderLayout();
				    	dlg.getContentPane().setLayout(layout);
				      dlg.setSize (500, 600);
 					*/
		
					dlg.getContentPane().setLayout (new GridLayout(6,2));
					dlg.setSize (500, 200);

					dlg.getContentPane().add (new JLabel(language.equals("Francais")?"Serveur SMTP":"SMTP server"));
					tfsmtp = new JTextField(30);
					dlg.getContentPane().add(tfsmtp);                                                                            
 
					dlg.getContentPane().add (new JLabel(language.equals("Francais")?"Adresse email de l'expéditeur":"Email address of the sender"));
					tffrom = new JTextField (30);
					dlg.getContentPane().add (tffrom);

					JLabel label;
					if (language.equals("English"))
						label = new JLabel ("Email address of the recipient : ");
					else
						label = new JLabel ("Adresse email du destinataire :");
					dlg.getContentPane().add (label);
					tfemail = new JTextField (30);
					dlg.getContentPane().add (tfemail);

					dlg.getContentPane().add (new JLabel(language.equals("English")?"Subject of the message : ":"Sujet du message :"));
					tfsubject = new JTextField (30);
					dlg.getContentPane().add (tfsubject);

					dlg.getContentPane().add (new JLabel(language.equals("English")?"Text of the message : ":"Texte du message :"));
					tftext = new JTextField (30);
					dlg.getContentPane().add (tftext);
					
				}
				
				JButton btn_ok = new JButton ("OK");
				btn_ok.addActionListener (new ActionListener()
					{
						public void actionPerformed (ActionEvent e)
						{
							dlg.ok = true;
							dlg.dispose();
							
						}
					});
				dlg.getContentPane().add (btn_ok);
				
				JButton btn_cancel = new JButton (language.equals("Francais")?"Annuler":"Cancel");
				btn_cancel.addActionListener (new ActionListener()
					{
						public void actionPerformed (ActionEvent e)
						{
							dlg.ok = false;
							dlg.dispose();
						}
					});
				dlg.getContentPane().add(btn_cancel);

				dlg.setModal(true);
				dlg.show();


/*
				String email = JOptionPane.showInputDialog (null, "Tapez l'adresse email du destinataire", "Envoi de fax par email", JOptionPane.PLAIN_MESSAGE);	
*/
				String smtp = tfsmtp.getText();
				String from = tffrom.getText();
				String email = tfemail.getText();
				String subject = tfsubject.getText();
				String text = tftext.getText();

				if (!dlg.ok)
				{
					flag_refresh = true;
					return;
				}
				/* boolean aut = Droits.autorise (login, "", "", "EMAIL", email, 0); */
				boolean aut = true;
				if (!aut)
				{
					System.out.println ("Non autorisé");
					if (language.equals("English"))
						JOptionPane.showMessageDialog(null, "You are not authorized to send faxes to this address email", "Send fax by email", JOptionPane.WARNING_MESSAGE);      
					else
						JOptionPane.showMessageDialog(null, "Vous n'êtes pas autorisé à envoyer des fax à cette adresse email", "Envoi de fax par email", JOptionPane.WARNING_MESSAGE); 


				}
				else 
				{
				/* Envoi email */
	
				System.out.println ("Envoi email a " + email + ", sujet:" + subject + ", texte:" + text);




				/* connexion */

				boolean status = sc.login (basefax, login, password);
				InputStream is = sc.is;
				OutputStream os = sc.os;

				String com;
				String commande;

                                com = "MAIL\n" + filename + "\n" + smtp + "\n" + from + "\n" + email + "\n" + subject + "\n" + text.replace('\n','$') + "\n";

				sc.output (com);

					String reponse = sc.input ("any");
					if (reponse.equals ("NOTAUTH"))
					{
						JOptionPane.showMessageDialog (null, 
							language.equals("English")?"You are not authorized to send this fax to this email address":"Vous n'êtes pas autorisé à envoyer ce fax à cette adresse email", 
							language.equals("Francais")?"Envoi email":"Send email", 
							JOptionPane.WARNING_MESSAGE); 
					}
			 		else if (reponse.equals ("OK"))
					{
						JOptionPane.showMessageDialog (null, 
							language.equals("English")?"Fax envoyé par email":"Fax sent by email" , 
							language.equals("Francais")?"Envoi email":"Send email",  
							JOptionPane.WARNING_MESSAGE); 
					}
					else /*if (reponse.equals ("ERROR"))*/
					{
						JOptionPane.showMessageDialog (null, 
							language.equals("English")?"Error":"Erreur", 
							language.equals("Francais")?"Envoi email":"Send email",  
							JOptionPane.WARNING_MESSAGE); 
					}


                                  }
			   }
			   catch (Exception e1)
			   {
			   }
                           }
			   catch (Exception e2)
			   {
				e2.printStackTrace();
		  	   }
			   flag_refresh = true;
			}

			public void actionPerformed1 (ActionEvent e) /* local */
			{
                     try
                     {
				int i = tablefaxrec.getSelectedRow();
				Integer I = new Integer(i);
				System.out.println ("Envoyer par email le fax " + I.toString());

				String filename = ((MessageInfo)(messages.elementAt(i))).message.getField("FILE");
				/*String filename = ((GenericMessage)(vMessages.elementAt(i))).getField("FILE");*/


				/* connexion */

				boolean status = sc.login (basefax, login, password);
				InputStream is = sc.is;
				OutputStream os = sc.os;

/*** le code ci-dessous est remplacé par sc.login ***

				Socket sock = new Socket ("localhost", 6020);
				InputStream is = sock.getInputStream();
				OutputStream os = sock.getOutputStream();
***/
				String com;
				String commande;
/***


	// envoi du login et du mot de passe
	com = new String("USER\n" + login +"\n" + "PASS\n" + password +"\n");
	os.write(com.getBytes("US-ASCII"));
	os.flush();
	commande = new String(recoit_param(is, "alpha"));
	System.out.println ("received(1): " + commande);

	commande = new String(recoit_param(is, "alpha"));
	System.out.println ("received(2): " + commande);

***/

				com = new String ("FAXDATA\n" + filename + "\n");
				os.write(com.getBytes("US-ASCII"));
				os.flush();
				
					commande = new String(recoit_param(is, "any"));
					System.out.println ("received: " + commande);

				if (!commande.equals("FAXDATA"))
				{
					System.out.println ("Donnees non recues");
					if (language.equals("English"))
						JOptionPane.showMessageDialog(null, "You are not authorized to access to received faxes", "Reception of fax", JOptionPane.WARNING_MESSAGE);      
					else
						JOptionPane.showMessageDialog(null, "Vous n'êtes pas autorisé à accéder aux fax reçus", "Réception du fax", JOptionPane.WARNING_MESSAGE); 

				}
				else
				{

				/* Boite de dialogue pour demander l'adresse email */

				String titre;
				if (language.equals ("Francais"))
					titre = "Envoi du fax par email";
				else
					titre = "Send the fax by email";

				final MyDialog dlg = new MyDialog (frame, titre, false);
				dlg.setResizable (true);

				int lang;
				if (language.equals("Francais"))
					lang = 0;
				else
					lang = 1;
				String fieldcont;
				
				JTextField tfemail = null;
				JTextField tfsubject = null;
				JTextField tftext = null;

				if (i<0)
				{
					dlg.getContentPane().setLayout(new GridLayout(0,1,5,5));
				      dlg.setSize (300, 100);
					JLabel lbl_message;
					String message;
					if (language.equals("Francais"))
						message = "Vous devez sélectionner un fax dans la liste";
					else
						message = "You must select a fax in the list";
					lbl_message = new JLabel (message);
					dlg.getContentPane().add (lbl_message);
				}
				else
				{
 
				    /*
					BorderLayout layout = new BorderLayout();
				    	dlg.getContentPane().setLayout(layout);
				      dlg.setSize (500, 600);
 					*/
		
					dlg.getContentPane().setLayout (new GridLayout(4,2));
					dlg.setSize (500, 200);
 
					JLabel label;
					if (language.equals("English"))
						label = new JLabel ("Email address of the recipient : ");
					else
						label = new JLabel ("Adresse email du destinataire :");
					dlg.getContentPane().add (label);
					tfemail = new JTextField (30);
					dlg.getContentPane().add (tfemail);

					dlg.getContentPane().add (new JLabel(language.equals("English")?"Subject of the message : ":"Sujet du message :"));
					tfsubject = new JTextField (30);
					dlg.getContentPane().add (tfsubject);

					dlg.getContentPane().add (new JLabel(language.equals("English")?"Text of the message : ":"Texte du message :"));
					tftext = new JTextField (30);
					dlg.getContentPane().add (tftext);
					
				}
				
				JButton btn_ok = new JButton ("OK");
				btn_ok.addActionListener (new ActionListener()
					{
						public void actionPerformed (ActionEvent e)
						{
							dlg.ok = true;
							dlg.dispose();
							
						}
					});
				dlg.getContentPane().add (btn_ok);
				
				JButton btn_cancel = new JButton (language.equals("Francais")?"Annuler":"Cancel");
				btn_cancel.addActionListener (new ActionListener()
					{
						public void actionPerformed (ActionEvent e)
						{
							dlg.ok = false;
							dlg.dispose();
						}
					});
				dlg.getContentPane().add(btn_cancel);



		
				dlg.setModal(true);
				dlg.show();


/*
				String email = JOptionPane.showInputDialog (null, "Tapez l'adresse email du destinataire", "Envoi de fax par email", JOptionPane.PLAIN_MESSAGE);	
*/
				String email = tfemail.getText();
				String subject = tfsubject.getText();
				String text = tftext.getText();

				if (!dlg.ok)
					return;

				/* boolean aut = Droits.autorise (login, "", "", "EMAIL", email, 0); */
				boolean aut = true;
				if (!aut)
				{
					System.out.println ("Non autorisé");
					if (language.equals("English"))
						JOptionPane.showMessageDialog(null, "You are not authorized to send faxes to this address email", "Send fax by email", JOptionPane.WARNING_MESSAGE);      
					else
						JOptionPane.showMessageDialog(null, "Vous n'êtes pas autorisé à envoyer des fax à cette adresse email", "Envoi de fax par email", JOptionPane.WARNING_MESSAGE); 


				}
				else 
				{
				/* Envoi email */
	
				System.out.println ("Envoi email a " + email + ", sujet:" + subject + ", texte:" + text);

				Socket s1;
				PrintStream p1;
				DataInputStream d1;
				String recvreply;

				if (!email.equals(""))
				try
				{
					String server = "smtp.noos.fr";
					String dest = email;

					MailMsg m = new MailMsg();
					m.smtpServer = server;
					m.from = "jacques.bailhache@ifrance.com";
					m.addRecipient(dest);
					m.subject = subject;
					MailMsgPart part = new MailMsgPart();
        				part.setData (text, MimeEncoder.QUOTED);
        				m.addPart(part);
					MailMsgPart at;

					/***
					s1 = new Socket (server, 25);
					d1 = new DataInputStream (new BufferedInputStream (s1.getInputStream(), 2500));
					p1 = new PrintStream (new BufferedOutputStream (s1.getOutputStream(), 2500), true);
					BufferedWriter out = new BufferedWriter (new OutputStreamWriter(s1.getOutputStream(), "8859_1")); 


					recvreply = d1.readLine();
					System.out.println ("Server Response : " + recvreply);

					// a faire : email expediteur 
					p1.println("MAIL FROM: jacquesbailhache@ifrance.com");
					recvreply=d1.readLine(); 
					System.out.println("Server Response : " + recvreply);

					p1.println ("RCPT TO: " + dest);
					recvreply=d1.readLine(); 
					System.out.println("Server Response : " + recvreply);

					p1.println("DATA");
					recvreply=d1.readLine(); 
					System.out.println("Server Response : " + recvreply);

					p1.println("From : jacques");
					p1.println("Reply-To : asds@rocketmail.com");
					// p1.println("To : sdv@hotmail.com>");
					p1.println ("To : " + dest);
					p1.println("Subject : " + subject); 
					p1.println("Date : 5th Dec"); 

					p1.println("MIME-Version: 1.0");
					p1.println("Content-Type: multipart/mixed;");
					p1.println("boundary=\"----=_next_part\"");
					p1.println("");
					p1.println("This is a multipart message");
					p1.println("");
					p1.println("------=_next_part");

					p1.println("Content-Type: text/html; charset=us-ascii"); 
					p1.println("Content-Transfer-Encoding: 7-bit"); 
					p1.println("");
					p1.println(text);
					p1.println("");
 					***/
					
					// commande = new String(recoit_param(is, "any"));
					// System.out.println ("received: " + commande);
					

					for (int np=0;; np++)
					{	
								System.out.println ("np=" + np);

								commande = new String(recoit_param(is, "any"));
								System.out.println ("received: " + commande);
								Integer NP1 = new Integer(commande);
								int np1 = NP1.intValue();
								System.out.println ("np1=" + np1);

								commande = new String(recoit_param(is, "any"));
								System.out.println ("received: " + commande);
								Integer N = new Integer(commande);
								int n = N.intValue();
					 
								byte[]data = new byte[n];
								String strdata = "";
 
								for (int j=0; j<n; j++)
								{
									int c = is.read();
									byte b = (byte)c;
				 					data[j] = b;
									/*strdata += (char)c;*/
								}
				
								strdata = new String (data);
								
								commande = new String(recoit_param(is, "any"));
								System.out.println ("received: " + commande);
	
								commande = new String(recoit_param(is, "any"));
								System.out.println ("received: " + commande);

								/* data contient les données d'une page à envoyer en attachement */


								/*System.out.println ("Donnees:" + data);*/

								/*
								p1.println("------=_next_part");
								p1.println("Content-Type: application/octet-stream;");
								p1.println ("name=\"page" + (np1+1) + ".tif\""); 
								p1.println("Content-Transfer-Encoding: 8bit");
								p1.println("");
								p1.println(strdata);
								p1.println("");
								*/

								/***
								p1.println("------=_next_part");
								p1.println("Content-Type: application/octet-stream;");
								p1.println ("name=\"page" + (np1+1) + ".tif\""); 
								p1.println("Content-Disposition: attachment;filename=\"page" + (np1+1) + ".tif\"");
								p1.println("Content-Transfer-Encoding: base64");
								p1.println("");
								MIMEBase64.encodeData(strdata, out); 

								p1.println("");
								p1.println("");
								***/

								at = new MailMsgPart();
								at.ContentType = "Application";
								at.ContentSubType = "Octet-stream";
								at.FileName = "page" + (np1+1) + ".tif";
								at.Name = "page" + (np1+1) + ".tif";
								/*at.setData (strdata, MimeEncoder.BASE64);*/
								System.out.println ("Set binary data");
								at.setBinaryData (FaxMailerFunctions.stdTiffBytes(data));
								m.addPart (at);

								if (!commande.equals("FAXDATA"))
								{	
									m.mail();
									/***
									p1.println("------=_next_part--");
									p1.println("");

									p1.println(".");

									p1.flush();

									recvreply=d1.readLine(); 
									System.out.println("Server Response : " + recvreply);

									p1.println("QUIT");
									recvreply=d1.readLine(); 
									System.out.println("Server Response : " + recvreply);									 

									s1.close();
									System.out.println("Closed Connection with Server");
									***/

									/* Déconnexion */
                                                                        sc.logout();
                                                                        /***
									com = new String("END\n");
									os.write(com.getBytes("US-ASCII"));
									os.flush();
									sock.close();
                                                                        ***/
								 	break;			
								}
					}





				}
				catch (Exception exc)
				{
				}


				}

				}

			    }
			    catch (Exception ex)
			    {
				System.out.println ("Error: " + ex.getMessage());
			    }
			}
		});
	panebtn.add (btn_email);

	btn_save = new JButton ("Sauvegarder");
	btn_save.setIcon (new ImageIcon (icondir + "save.gif"));
	btn_save.addActionListener (new ActionListener() 
		{
			public void actionPerformed (ActionEvent e)
			{
                     try
                     {
				int i = tablefaxrec.getSelectedRow();
				Integer I = new Integer(i);
				System.out.println ("Sauvegarder le fax " + I.toString());

				String filename = ((MessageInfo)(messages.elementAt(i))).message.getField("FILE");
				/*String filename = ((GenericMessage)(vMessages.elementAt(i))).getField("FILE");*/

				JFileChooser fd = new JFileChooser();
				/*fd.setFileView(new nFileView(fd));*/

				if (language.equals("English"))
					fd.setDialogTitle("Save the fax");
				else
					fd.setDialogTitle("Sauvegarder le fax");
				int ret;
				ret = fd.showSaveDialog(frame);
				if (ret == JFileChooser.APPROVE_OPTION)
				{

/* connexion */

	boolean status = sc.login (basefax, login, password);
	InputStream is = sc.is;
	OutputStream os = sc.os;

/*** le code ci-dessous est remplacé par sc.login ***

				Socket sock = new Socket ("localhost", 6020);
				InputStream is = sock.getInputStream();
				OutputStream os = sock.getOutputStream();
				
				
***/
				String com;
				String commande;
/***

	// envoi du login et du mot de passe
	com = new String("USER\n" + login +"\n" + "PASS\n" + password +"\n");
	os.write(com.getBytes("US-ASCII"));
	os.flush();
	commande = new String(recoit_param(is, "alpha"));
	System.out.println ("received(1): " + commande);

	commande = new String(recoit_param(is, "alpha"));
	System.out.println ("received(2): " + commande);
***/


				com = new String ("FAXDATA\n" + filename + "\n");
				os.write(com.getBytes("US-ASCII"));
				os.flush();
				
				commande = new String(recoit_param(is, "any"));
				System.out.println ("received: " + commande);

				if (!commande.equals("FAXDATA"))
				{
					System.out.println ("Donnees non recues");
					if (language.equals("English"))
						JOptionPane.showMessageDialog(null, "You are not authorized to access to received faxes", "Reception of fax", JOptionPane.WARNING_MESSAGE);      
					else
						JOptionPane.showMessageDialog(null, "Vous n'êtes pas autorisé à accéder aux fax reçus", "Réception du fax", JOptionPane.WARNING_MESSAGE); 

				}
				else
				{
				for (int np=0;; np++)
				{	
				System.out.println ("np=" + np);

				commande = new String(recoit_param(is, "any"));
				System.out.println ("received: " + commande);
				Integer NP1 = new Integer(commande);
				int np1 = NP1.intValue();
				System.out.println ("np1=" + np1);

				commande = new String(recoit_param(is, "any"));
				System.out.println ("received: " + commande);
				Integer N = new Integer(commande);
				int n = N.intValue();
				/*String data = new String("");*/

				byte[]data = new byte[n];
				/*Vector datavect = new Vector();*/

				for (int j=0; j<n; j++)
				{
					byte b = (byte)is.read();
					/*
					System.out.print (b);
					System.out.print (" ");
					*/
					/*data += (char) b;*/
					/*datavect.addElement(new Byte(b));*/
					data[j] = b;
					
				}

				
				commande = new String(recoit_param(is, "any"));
				System.out.println ("received: " + commande);

				commande = new String(recoit_param(is, "any"));
				System.out.println ("received: " + commande);

				/*
				File f = new File(fd.getSelectedFile().getPath());
				FileWriter out = new FileWriter (f);
				*/
				String filename1 = new String (fd.getSelectedFile().getPath() + ".00" + new Integer(np1).toString());
				System.out.println ("Filename:" + filename1);
				RandomAccessFile raf = new RandomAccessFile (filename1, "rw");

				raf.write (data);
				raf.close();
				
				/*
				FileWriter out = new FileWriter(fichier); out.write(vue.texte.getText()); out.close(); 

				*/

				if (!commande.equals("FAXDATA"))
				{	
				

				/* Déconnexion */
				sc.logout();
				/***
				com = new String("END\n");
				os.write(com.getBytes("US-ASCII"));
				os.flush();
				sock.close();
				***/
			 	break;			
			
				}
				}
				}
				}

			    }
			    catch (Exception ex)
			    {
				System.out.println ("Error: " + ex.getMessage());
			    }


			}
		});
	panebtn.add (btn_save);

	btn_delete = new JButton ("Supprimer");
	btn_delete.setIcon (new ImageIcon (icondir + "Garbage1.gif"));
	btn_delete.addActionListener (new ActionListener()
		{
			public void actionPerformed (ActionEvent e)
			{
                     try
                     {
				/*
				int i = tablefaxrec.getSelectedRow();
				Integer I = new Integer(i);
				System.out.println ("Supprimer le fax " + I.toString());
				*/


				/*String filename = (String)tablefaxrec.getValueAt(i,1);*/
				int[] sel_fax = tablefaxrec.getSelectedRows();
				String filename = "";
				for (int j=0; j<sel_fax.length; j++)
				{
					if (j>0) filename += "\t";
					filename += (String)tablefaxrec.getValueAt(sel_fax[j],index1("FILE"));
					/*filename += ((GenericMessage)(vMessages.elementAt(sel_fax[j]))).getField("FILE");*/

				}

 
				/* connexion */

				boolean status = sc.login (basefax, login, password);
				InputStream is = sc.is;
				OutputStream os = sc.os;

				sc.output ("DELETE\n" + filename + "\n");
				String reponse = sc.input("any");

				if (reponse.equals("DELETE"))
					JOptionPane.showMessageDialog(null, 
						language.equals("Francais")?"Fax supprimé":"Fax deleted", 
						"", JOptionPane.WARNING_MESSAGE); 
				else
					JOptionPane.showMessageDialog(null,reponse,"",JOptionPane.WARNING_MESSAGE);
				
				sc.logout();
			   }
			   catch (Exception e1)
			   {
				System.out.println ("Error deleting fax: " + e1.getMessage());
			   }

			}
		});
	panebtn.add(btn_delete);

	btn_new = new JButton ("Mettre à jour");
	/*btn_new.setIcon (new ImageIcon (icondir + "refresh.gif"));*/
	btn_new.addActionListener (new ActionListener ()
		{
			public void actionPerformed (ActionEvent e)
			{
				flag_refresh=true;
				refresh ("", true, true);
				
			}
		});
	panebtn.add(btn_new);

	btn_refresh = new JButton ("Réafficher");
	btn_refresh.setIcon (new ImageIcon (icondir + "refresh.gif"));
	btn_refresh.addActionListener (new ActionListener ()
		{
			public void actionPerformed (ActionEvent e)
			{
				flag_refresh=true;
				refresh ("", true, false);
				
			}
		});
	panebtn.add(btn_refresh);

	btn_sort = new JButton ("Trier");
	btn_sort.setIcon (new ImageIcon (icondir + "order_as.gif"));
	btn_sort.addActionListener (new ActionListener() 
		{
			/** Tri local */
			public void actionPerformed (ActionEvent e)
			{				
				int cs = tablefaxrec.getSelectedColumn();
				trier (cs, false);
			}


			/** Tri par le serveur */
			public void actionPerformed1 (ActionEvent e)
			{
			
				String field;
				/*
				if (language.equals("English"))
					field = JOptionPane.showInputDialog (null, "Sort by :", "Sort the list of received faxes", JOptionPane.PLAIN_MESSAGE);	
				else
					field = JOptionPane.showInputDialog (null, "Trier par :", "Tri de la liste des fax reçus", JOptionPane.PLAIN_MESSAGE);	
				*/
	int cs = tablefaxrec.getSelectedColumn();

      if (cs != -1)
      {
	field = columnNames[cs];



	if (field != null)
		refresh ("ORDER BY " + fdl + field + fdl, false, false);

 
	}


			}
		});
	panebtn.add (btn_sort);

	btn_sortrev = new JButton ("Trier");
	btn_sortrev.setIcon (new ImageIcon (icondir + "order_ds.gif"));
	btn_sortrev.addActionListener (new ActionListener() 
		{
			/** Tri local */
			public void actionPerformed (ActionEvent e)
			{				
				int cs = tablefaxrec.getSelectedColumn();
				trier (cs, true);
			}
		});
	panebtn.add (btn_sortrev);



	btn_select = new JButton ("Sélectionner");
	btn_select.setIcon (new ImageIcon (icondir + "props.gif"));
	btn_select.addActionListener (new ActionListener() 
		{
			public void actionPerformed2 (ActionEvent e)
			{
				final int ls = tablefaxrec.getSelectedRow();
				final int cs = tablefaxrec.getSelectedColumn();

				if (ls<0 || cs<0)
				{
					JDialog dlg = new JDialog(frame); /**/
					dlg.getContentPane().setLayout(new FlowLayout());
				      dlg.setSize (300, 100);
					JLabel lbl_message;
					String message;
					/*
					if (language.equals("Francais"))
						message = "Veillez cliquer sur l'élément à sélectionner";
					else
						message = "Please click on the item to select";
					*/
					message = textes.CliqElem[lang];
					statusline.setText(message);
					lbl_message = new JLabel (message);
					dlg.getContentPane().add (lbl_message);
					/*dlg.show();*/
				} 
				else
				{
					statusline.setText("OK");
                                        final String fieldcont = (String)(tablefaxrec.getValueAt(ls,cs));


				}
			}

			public void actionPerformed (ActionEvent e)
			{
				System.out.println ("Selection");
				int ls = tablefaxrec.getSelectedRow();
				final int cs = tablefaxrec.getSelectedColumn();
				// String colname = columnNames[cs];
				// final String fieldcont = (String)tablefaxrec.getValueAt(ls,cs);
				String colname = columnNames[fieldnum(cs)-1];
				/*
				final String fieldcont = (String)(
				 ((MessageInfo)(messages.elementAt(ls))).message.values.elementAt(fieldnum(cs)));
				*/
                        final String fieldcont = (String)(tablefaxrec.getValueAt(ls,cs));
				System.out.println ("colname=" + colname + " fieldcont=" + fieldcont);
 
				if (ls<0 || cs<0)
				{
					JDialog dlg = new JDialog(frame); /**/
					dlg.getContentPane().setLayout(new FlowLayout());
				      dlg.setSize (300, 100);
					JLabel lbl_message;
					String message;
					/*
					if (language.equals("Francais"))
						message = "Veillez cliquer sur l'élément à sélectionner";
					else
						message = "Please click on the item to select";
					*/
					message = textes.CliqElem[lang];
					statusline.setText(message);
					lbl_message = new JLabel (message);
					dlg.getContentPane().add (lbl_message);
					/*dlg.show();*/
				} 
				else
				{
					statusline.setText("OK");
				}
				Vector vlistfaxs = new Vector();
                                /*int nfax = vlistfax.size();
                                System.out.println ("nfax=" + nfax);*/

				ExtVector messages_sel = messages.select (new Selector() {
					public boolean select (Object x)
					{
						System.out.println ("select: " + ((MessageInfo)x).values[fieldnum(cs)]);
						/*if (cs == index1("CNX") || cs == index1("DCNX"))
							return ((MessageInfo)x).values[cs].substring(0,10).equals(fieldcont.substring(0,10));
						else*/
							// return ((MessageInfo)x).values[fieldnum(cs)].equals(fieldcont);
                                                        // String fieldcont1 = ((MessageInfo)x).viewed_values[((MessageInfo)x).pos];
                                                        String fieldcont1 = ((MessageInfo)x).viewed_values[cs];
							boolean e =  fieldcont.equals(fieldcont1);
							System.out.println ("result of " + fieldcont + " = " + fieldcont1 + " : " + e);
							return e;
					}});
				pile.add(0,messages);
				messages = messages_sel;
				displayMessages(messages);
				
				
				/*
				for (int i=0; i<nfax; i++)
				{
					// Insérer le ième élément de vlistfax à la bonne place dans vlistfaxs 
					// String[] fields = ((String)(vlistfax.elementAt(i))).split("\t");
					String[] fields = getFields((String)(vlistfax.elementAt(i)));
					boolean ok;
					if (cs == 2 || cs == 3)
						ok = fields[cs].substring(0,10).equals(fieldcont.substring(0,10));
					else
						ok = fields[cs].equals(fieldcont);


							  boolean er = false;
							  try
							  {
							   double dp = Double.valueOf(fields[cs]).doubleValue();
							   // int ip = (int)dp;
							   double dp1 = Double.valueOf(fieldcont).doubleValue();   
								if (dp == dp1)
									ok = true;
							   
								//String sp = "" + ip;							   
							   //tablefaxrec.setValueAt (sp, i1, j);							  
							
							  }
                                		  catch (Exception e1)
							  {
								er = true;
							  }


					System.out.println ("i=" + i + " Selection <" + fieldcont + "> <" + fields[cs] + "> " + ok);
					if (ok)					
						vlistfaxs.add (vlistfax.elementAt(i));
				}
				displaylistfax (vlistfaxs, true, false);
				vvlistfax.add(0,vlistfax);
				vreverse.add(0,new Boolean(true));
				vlistfax = vlistfaxs;
				*/

			}

			/** Sélection par le serveur */
			public void actionPerformed1 (ActionEvent e)
			{
				/* 
				String field = JOptionPane.showInputDialog (null, "Trier par :", "Tri de la liste des fax reçus", JOptionPane.PLAIN_MESSAGE);	
	
	if (field != null)*/


	int ls = tablefaxrec.getSelectedRow();
	int cs = tablefaxrec.getSelectedColumn();
	String colname = columnNames[cs];
	String fieldcont = (String)tablefaxrec.getValueAt(ls,cs);

	/*com = new String ("LISTREC\nWHERE " + colname + " = '" + fieldcont + "'\n");*/

	refresh ("WHERE " + fdl + colname + fdl + " = '" + fieldcont + "'", true, false);

 
	 



			}
		});
	panebtn.add (btn_select);

	btn_undo = new JButton ("Annuler");
	btn_undo.setIcon (new ImageIcon (icondir + "undo.gif"));
      btn_undo.addActionListener (new ActionListener ()
		{
			public void actionPerformed (ActionEvent e)
			{
				if (pile.size() > 0)
				{
					System.out.println ("undo: " + ((ExtVector)(pile.elementAt(0))).size() + " messages");
					messages = (ExtVector)(pile.elementAt(0));
					pile.remove(0);
					displayMessages(messages);
				}
				/*
				vlistfax = (Vector)(vvlistfax.elementAt(0));
				vvlistfax.remove(0);
				boolean reverse = ((Boolean)(vreverse.elementAt(0))).booleanValue();
				vreverse.remove(0);
				displaylistfax (vlistfax, reverse, false);
				*/
			}
		});
	panebtn.add (btn_undo);

	setTextLang(0);

	/*panebtn.setSize (900,60);*/

	//pane.add (panebtn);
	paneb.add (panebtn, BorderLayout.CENTER);
	pane.add (paneb);
	
	System.out.println ("buttons created");

	}


	/*
	public faxrecn (String phost, String pbase, String plogin, String ppassword)
	{
	host = phost;
	basefax = pbase;
	login = plogin;
	password = ppassword;
	*/
	public faxrecn (boolean ask, String phost, int pport, String pbasefax, String plogin, String ppassword,
		 boolean view, int n, String[] champ, int[] taille, String[] libelle, String[] script)
	{
	flag_refresh=true;
	if (ask) login();
	else
	{
		host = phost;
		port = pport;
		basefax = pbasefax;
		login = plogin;
		password = ppassword;
		
		System.out.println ("faxrecn creation: host=" + host + " port=" + port + " base=" + basefax + " login=" + login);
			
		sc = new ServerConnection (host, port);
		System.out.println ("ServerConnection created");

		boolean status = sc.login (basefax, login, password);
		System.out.println ("Logged in");

		columnNames = sc.aField;
		System.out.println ("(1) col0:" + columnNames[0]);
		nColumns = columnNames.length;
		System.out.println ("Number of columns: " + nColumns);
		sc.logout();

	
	}
	flag_view = view;
	nfields_view = n; 
	System.out.println ("nfields_view=" + nfields_view);
	field_view = champ;
	size_view = taille;
        System.out.println ("size_view=" + size_view);
        if (size_view!=null)
        {
                System.out.println ("size.length=" + size_view.length);
                System.out.println ("size 0=" + size_view[0]);
        }
	text_view = libelle;
	script_view = script; 

	fr = this;
	pile = new ExtVector();
	/*
	vvlistfax = new Vector();
	vreverse = new Vector();
	*/
	/*init_frame();*/

      /*sc = new ServerConnection (host, port);*/
	
	  


	try
	{
	

      System.out.println ("init frame");
	init_frame ();
	System.out.println ("init frame done");

	/*refresh ("", true, false);*/

	initial_refresh();
/*

	vlistfaxl = loadVector ("faxrecn.txt");
	_reverse = true;
        System.out.println ("" + vlistfaxl.size() + " fax recus");
	if (vlistfaxl.size() == 0)
	{
		System.out.println ("Chargement depuis le serveur");
		refresh ("", true, false);
	}
	else
	{
		System.out.println ("Chargement en local");
		flag_local = true;
		refresh ("", true, false);
		flag_local = false;
	}
	

	last_refresh = new java.util.Date();
*/

 					
 
	}
	catch (Exception e)
	{
		/*label.setText ("Error: " + e.getMessage());*/

		e.printStackTrace();

                try
                {
                        statusline.setText ("Error: " + e.getMessage());
                }
                catch (Exception e1)
                {
                }
	}
	 


        frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
        frame.pack();
	  frame.setSize (900, 600);
        frame.setVisible(true);

/*
		System.out.println ("Creating update_thread");
		update_thread updt = new update_thread (fr);
		System.out.println ("Starting");
		updt.start();
		System.out.println ("update_thread started");
*/
    }

		

}

class changelangue implements ActionListener
{
	private faxrecn fr = null;
	private String langue;
	public changelangue (faxrecn pfr, String plangue)
	{
		fr = pfr;
		langue = plangue;
	}
	public void actionPerformed (ActionEvent e)
	{
		fr.language = langue;
		/*fr.remove_frame();*/
		/*fr.init_frame();*/
	}
}

/** Evènement fermeture de boîte de dialogue */
class devt implements ActionListener
{
	JDialog dlg;
	public devt (JDialog pdlg)
	{
		dlg = pdlg;
	}
	public void actionPerformed (ActionEvent e)
	{
		dlg.dispose ();
	}

}

/**
	Arborescence des groupes 
*/
class GroupTree extends JComponent
{
	public String root;
	public int n;
	public String[][] table;
	public int x, y, dx, dy, w, h;

	public GroupTree (String proot, int pn, String[][] ptable, 
		int px, int py, int pdx, int pdy, int pw, int ph)
	{
		root = proot;
		n = pn;
		table = ptable;
		x=px; y=py; dx=pdx; dy=pdy; w=pw; h=ph;
	}

	public Dimension getPreferredSize ()
	{
		return new Dimension (310, 400);
	}

/** Affiche une arborescence 
		@param root racine de l'arborescence à afficher
		@param n nombre d'éléments du tableau
		@param table tableau de données
		@param pane où afficher l'arborescence
		@param px abcisse
		@param py ordonnée
		@param dx décalage horizontal
		@param dy espacement des lignes
		@param w largeur des labels
		@param h hauteur des labels
	*/
	public int drawTree (int level, String root, int n, String[][] table, Graphics2D g,
		int px, int py, int dx, int dy, int w, int h)
	{
		int x = px;
		int y = py;
		int k = 0;
		for (int i=0; i<n; i++)
		{
			if (table[i][1].equals(root) || (root.equals("") && table[i][1].equals("null")))
			{
				k++;
				/*
				JLabel l = new JLabel (table[i][0]);
				l.setBounds (x, y, w, h);
				pane.add(l);
				*/
				for (int j=0; j<level; j++)
					/*g.drawLine (x-dx+5-j*dx, y-dy+((i==0&&j==0)?-h/2:1), x-dx+5-j*dx, y-h/2);*/
				{
					if (k==1 && j==0)
						g.drawLine (x-dx+5-j*dx, y-dy+1, x-dx+5-j*dx, y-h/2);
					else
						g.drawLine (x-dx+5-j*dx, y-dy-h/2, x-dx+5-j*dx, y-h/2);

				}
				g.drawLine (x-dx+5, y-h/2, x-5, y-h/2);
				g.setColor (Color.black);
				g.setFont (new Font ("Arial", Font.PLAIN, h));
				g.drawString (table[i][0], x, y);
				y = drawTree (level+1, table[i][0], n, table, g, x+dx, y+dy, dx, dy, w, h);
			}
		}
		return y;
	}

/**
	Dessine l'arbre
	@param g Graphics sur lequel l'arbre est dessiné
*/
	public void paint (Graphics g)
	{
		Graphics2D g2d = (Graphics2D) g;
		/*super.paintComponent(g);*/
		/*g2d.drawOval (50, 40, 120, 90);*/
		drawTree (0, root, n, table, g2d, x, y, dx, dy, w, h);
	}


}

class Vignette extends JPanel
{
	faxrecn fr;
	public int wa = 78; // 78; // 60
	public int ha = 55; // 110; // 80
	public int aw = 1;
	public int ah = 2;
	public JDialog dlg;
	JScrollBar vsb;
	
	Dimension preferredSize = new Dimension (aw*wa, ah*ha);

	Vignette (JScrollBar sb)
	{
		super ();

		setLayout (new BorderLayout());

		final Vignette v = this;

		if (sb == null)
		{
		vsb = new JScrollBar(JScrollBar.HORIZONTAL, 30, 10, 0, 100);
		vsb.addAdjustmentListener (new AdjustmentListener () {
			public void adjustmentValueChanged(AdjustmentEvent e)
			{
				v.repaint();
			}
		});
		// paneb.add (vsb, BorderLayout.SOUTH);
		add (vsb, BorderLayout.SOUTH);
		}
		else
		{
			vsb = sb;
		}

/*
		addMouseListener (new MouseListener()
		{
			public void mouseEntered (MouseEvent evt)
			{
				final int a = 4;
				Vignette v = new Vignette (vsb);
				v.fr = fr;
				v.a = a;
				
				dlg = new JDialog ((Frame)null, "Vignette", false);
				dlg.setResizable (true);
				dlg.getContentPane().setLayout(new BorderLayout());
				dlg.setSize (a*wa+10, a*ha+20);

				dlg.getContentPane().add (v, BorderLayout.CENTER);
				v.repaint();
				dlg.show();
			}

			public void mouseExited (MouseEvent evt)
			{
				dlg.hide();
			}

			public void mouseClicked (MouseEvent evt) {}
			public void mousePressed (MouseEvent evt) {}
			public void mouseReleased (MouseEvent evt) {}

		});
*/
		
	}

	public Dimension getPreferredSize()
	{
		// return preferredSize;
		return new Dimension (aw*wa+20, ah*ha+10);
	}


	public void paintComponent (Graphics g)
	{
	   try
	   {
		super.paintComponent(g);
		// g.setColor(Color.blue);
		// g.fillRect (30, 40, 50, 60); 

		System.out.println ("paintComponent");

		g.setColor (Color.white);
		g.fillRect (0, 0, aw*wa, ah*ha);
		g.setColor (Color.black);

		// dessiner la vignette
		int row = fr.tablefaxrec.getSelectedRow();
		System.out.println ("row=" + row);
		// String apercu = (String)fr.tablefaxrec.getValueAt (row, 25); 
		String apercu = ((MessageInfo)(fr.messages.elementAt(row))).message.getField("APERCU");
		System.out.println ("apercu=" + apercu);
		int i, j;
		byte[] b = apercu.getBytes("US-ASCII");
		for (i=0; i<ha; i++)
		{
		for (j=0; j<wa; j++)
		{
			System.out.print ("\ri=" + i + " j=" + j);	
			if (b[i*wa+j] >= 65 + vsb.getValue()/4)
				g.fillRect (aw*j, ah*i, aw, ah);
		}
		}
		System.out.println ("end paintComponent");
	   }
	   catch (Exception e)
	   {
	   }
      }

}

/** Affichage d'un fax */
class FaxImage extends JPanel
{
	
	Dimension preferredSize = new Dimension (870, 650);
	
	public int viewHeight = 640;

	public Vector pages, sizes;
	public int nbrpages, curpage;
	public int imageWidth, imageHeight;
	public int vpos, vpos1;
	public double zoom = 2.0;
	TiffViewer tv = null;
	JScrollBar vsb;

	public FaxImage ()
	{	
	}

	public Dimension getPreferredSize ()
	{
		return preferredSize;
	}
	
	public void paintComponent (Graphics g)
	{
	   try
	   {
		super.paintComponent(g);
		/*
		g.setColor(Color.blue);
		g.fillRect (30, 40, 50*(curpage+1), 60); 
		*/

		/*
		Tiff myTiff = new Tiff ();
		myTiff.read ((byte[])(pages.elementAt(curpage)));
		ImageProducer ip = (ImageProducer) (myTiff.getImageProducer(0));
		Image imgTiff = getToolkit().createImage(ip);
		g.drawImage (imgTiff, 10, 10, this);
		*/

		System.out.println ("page " + curpage);
		int[] page = (int[])(pages.elementAt(curpage));
		System.out.println ("taille");
		int size = ((Integer)(sizes.elementAt(curpage))).intValue();
		System.out.println ("taille: " + size);
		System.out.println ("Création TiffViewer");
		if (tv == null)
		{
			System.out.println ("tv null");
			tv = new TiffViewer (page, size);
			/* TiffViewer tv = new TiffViewer((int[])(pages.elementAt(curpage)),((Integer)(sizes.elementAt(curpage))).intValue()); */
			System.out.println ("Conversion");
			tv.convert();
			/*tv.createImage (Color.white, Color.black);*/
			BoundedRangeModel m = vsb.getModel();
			int extent = ((m.getMaximum() - m.getMinimum()) * (tv.imageHeight - viewHeight) / tv.imageHeight);			
			m.setExtent(extent);
			vsb.setModel(m);
			vsb.setBlockIncrement (extent*2/3);

		}
		else System.out.println ("tv non null");
		imageWidth = tv.imageWidth;
		imageHeight = tv.imageHeight;
		System.out.println ("Affichage");
		int wb, hb;
		/*if (1728 * zoom >= 1728)
			wb = 1728;
		else*/
			wb = (int)((double)1728 * zoom);
		hb = (int)((double)viewHeight * zoom);
		System.out.println ("zoom="+zoom + " wb=" + wb + " hb=" + hb);

		tv.draw (g, Color.white, Color.black, 
				3, 3, /*864*/ 900, viewHeight,
				0, vpos, wb, hb/*1728, viewHeight*/);
		/*tv.drawImageCreated (g, 0, (int)(-vpos/viewHeight*0.55*tv.img.getHeight(null)), 
			864, (int)(viewHeight+vpos/viewHeight*0.55*tv.img.getHeight(null)));*/
		
	   }
	   catch (Exception e)
	   {
		System.out.println ("Error paintComponent: " + e.toString());
	   }

	}
	
	public void paintfax ()
	{
		Graphics g = getGraphics();
		paintComponent (g);
	}
}

class FaxImageDialog extends JDialog
{
	public FaxImage faximage;
	/*JTextField tfpage;*/
	JLabel lpage;

	FaxImageDialog (JFrame frame, String titre, boolean modal)
	{
		super (frame, titre, modal);
	}	
}


class MyDialog extends JDialog
{
	public boolean ok;
	MyDialog (JFrame frame, String titre, boolean modal)
	{ 
		super (frame, titre, modal);
		ok = false;
	}
}

class PortDialog extends MyDialog
{
	String port;
	String language;
	public JTextField tport;
	PortDialog (JFrame frame, String titre, boolean modal)
	{ 
		super (frame, titre, modal);
                language = "francais";

		final PortDialog dlg = this;
						dlg.setResizable(true);

						dlg.getContentPane().setLayout (null /*new FlowLayout()*/);
						dlg.setSize (300, 400);

						JLabel lport = new JLabel(language.equals("Francais")?"Port de l'imprimante":"Printer port");
						lport.setBounds (30, 40, 100, 20);				
						dlg.getContentPane().add (lport);
				
						tport = new JTextField (30);
						tport.setBounds (140, 40, 120, 20);
						dlg.getContentPane().add (tport);

						JButton btn_ok = new JButton ("OK");
						btn_ok.setBounds (30, 300, 80, 20);
						btn_ok.addActionListener (new ActionListener()
						{
							public void actionPerformed (ActionEvent e)
							{
								dlg.ok = true;
								dlg.dispose();							
							}
						});
						dlg.getContentPane().add (btn_ok);
				
						JButton btn_cancel = new JButton (language.equals("Francais")?"Annuler":"Cancel");
						btn_cancel.setBounds (140, 300, 80, 20);
						btn_cancel.addActionListener (new ActionListener()
							{
								public void actionPerformed (ActionEvent e)
								{
									dlg.ok = false;
									dlg.dispose();
								}
							});
						dlg.getContentPane().add(btn_cancel);
	}
}

class MyPainter implements Printable
{
String message;

MyPainter (String s)
{
	message = s;
}

public int print(Graphics graphics,PageFormat pageFormat,int page) {
    if ( page >= 1 )
      return(Printable.NO_SUCH_PAGE);
    Graphics2D g2 =(Graphics2D) graphics;
    g2.setColor(Color.black);
    g2.drawString(message,300,300);
    g2.draw(new Rectangle(100,200,300,400));
    return(Printable.PAGE_EXISTS);
  }        
}

/** Impression d'un fax */
class FaxPainter implements Printable
{
	boolean onepage;

	TiffViewer tv;
	FaxImage fi;

	FaxPainter (TiffViewer ptv)
	{
		tv = ptv;
		fi = null;
		onepage = true;
	}
	
	FaxPainter (FaxImage pfi)
	{
		fi = pfi;
		tv = null;
		onepage = false;
	}

	
	public int print (Graphics g, PageFormat pf, int curpage)
	{
 		
		/* if (tv == null) */
		if (!onepage)
		{
			System.out.println ("page " + curpage);
			int[] page = (int[])(fi.pages.elementAt(curpage));
			System.out.println ("taille");
			int size = ((Integer)(fi.sizes.elementAt(curpage))).intValue();
			System.out.println ("taille: " + size);
			System.out.println ("Création TiffViewer");
 			System.out.println ("page.length=" + page.length);
		 	tv = new TiffViewer (page, size);
			/* TiffViewer tv = new TiffViewer((int[])(pages.elementAt(curpage)),((Integer)(sizes.elementAt(curpage))).intValue()); */
			System.out.println ("Conversion");
			tv.convert();
			/*
			BoundedRangeModel m = vsb.getModel();
			int extent = ((m.getMaximum() - m.getMinimum()) * (tv.imageHeight - viewHeight) / tv.imageHeight);			
			m.setExtent(extent);
			vsb.setModel(m);
			vsb.setBlockIncrement (extent*2/3);
			*/
		}

		if ((/*fi == null*/ onepage && curpage != 0) || (/*fi != null*/ !onepage && curpage >= fi.nbrpages))
			return Printable.NO_SUCH_PAGE;

		/* tv.drawImage (g, Color.white, Color.black, 600, 800); */

		/* tv.drawImage (g, Color.white, Color.black, 50, 80, 500, 700); */ /* pour l'ancienne version du routeurr */
		tv.createImage (Color.white, Color.black);
		System.out.println ("height="+tv.img.getHeight(null));
		if (tv.img.getHeight(null) < 1400)
			tv.drawImageCreated (g, 60, 80, 450, (int) (0.55 * tv.img.getHeight(null))); /* pour la nouvelle version du routeur */
		else /* mode fin */
			tv.drawImageCreated (g, 60, 80, 450, (int) (0.5 * 0.55 * tv.img.getHeight(null))); /* pour la nouvelle version du routeur */



		/*
		tv.draw (g, Color.white, Color.black, 			
			// 0, 0, tv.imageWidth/2, tv.imageHeight,
			0, 0, 800, 1200,
			0, 0, tv.imageWidth, tv.imageHeight);		
		*/

		return Printable.PAGE_EXISTS;
	}
}

/*
class Message extends Dialog
{
	String text;
	Message (Frame f, String ptext)
	{
		super (f, ptext, true);
		text = ptext;
		setLayout (new FlowLayout());
		add (new Label(text));
		pack();
		setVisible(true);
		setModal(false);
	}

}
*/

class Message extends Thread
{
	String text;
	JFrame frame;
	public Message (JFrame pframe, String ptext)
	{
		frame = pframe;
		text = ptext;
	}
	public void run ()
	{
		JOptionPane.showMessageDialog(null, "Printing...", "Administration of received faxes", JOptionPane.WARNING_MESSAGE); 

	}
}

class update_thread extends Thread
{
	faxrecn fr;
	public update_thread (faxrecn pfr)
	{
		fr = pfr;
		System.out.println ("update_thread cree");
	}
	public void run ()
	{
	    System.out.println ("update_thread running");
	    for (;;)
	    {
		/*System.out.println ("update_thread still running");*/
		try
		{
			sleep (60000);
			System.out.println ("refresh");
			// test // 
			fr.refresh ("", true, true);
		}
		catch (Exception e)
		{
			System.out.println (e.getMessage());
		}
	    }
	}
	

}

class restart_thread extends Thread
{
	faxrecn fr;
	public restart_thread (faxrecn pfr)
	{ 
		fr = pfr;
		System.out.println ("Restart thread created");
	}
	public void run ()
	{
		System.out.println ("restart thread running");
		for (;;)
		{
			try
			{
				sleep (600000);
				if (!fr.updt.isAlive())
				{
					System.out.println ("*** UPDATE THREAD STOPPED, RESTARTING ***");
					fr.updt.start();
				}
			}
			catch (Exception e)
			{
				e.printStackTrace();
			}
		}
		
	}


}


class CodeReceiver implements StringReceiver 
{
	JTextArea ta;
	CodeReceiver (JTextArea pta)
	{
		ta = pta;
	}
	public void receive (String str)
	{
		System.out.println ("Code received: " + str);
		ta.setText(str);
	}
}
