
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
 
public class TrtClient extends JComponent
{
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
	public int port_reseau = 6029;
	/** indique si le serveur est en mode de reception */
	public boolean running = false;

	/**
	 * le main appelle le constructeur
	 */
	public static void main(String args[])
	{
		System.out.println ("fmserver nouvelle version");
		TrtClient sv = new TrtClient();
    	}

	/**
	 * Constructeur, initialise la fenetre principale
	 */
	public TrtClient()
	{
		GraphicsEnvironment ge = GraphicsEnvironment.getLocalGraphicsEnvironment();
		GraphicsDevice[] gs = ge.getScreenDevices();
		GraphicsDevice gd = gs[0];
		GraphicsConfiguration gc = gd.getDefaultConfiguration();
		JFrame f = new JFrame("Serveur de traitement des fax re�us c�t� client - FAX-Mailer JAVA");
		f.setDefaultCloseOperation(WindowConstants.DO_NOTHING_ON_CLOSE);
		mainWindow = f;

		init_menus(f);
		f.getContentPane().setLayout(new BorderLayout());
		// on ajoute le bouton de fermeture
		f.addWindowListener(new wevtTrtClient(this, f, true));
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
			      append("OK\nConnexion accept�e\n", Color.lightGray, true, true, false);
				clientTrtClient cl = new clientTrtClient(this, sc);
				cl.start();
			}
		}
		catch(Exception e)
		{
			e.printStackTrace();
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
		finTrtClient q = new finTrtClient(this);
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
	}

}

/**
 * gestionnaire d evenements de fenetres
 */
class wevtTrtClient extends WindowAdapter
{
	private Window wnd = null;
	private boolean exit = false;
	private TrtClient sv = null;
	
	public wevtTrtClient(TrtClient psv, Window pWnd, boolean pexit)
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
class clientTrtClient extends Thread
{
	String fdl = "\""; /* d�limiteur de nom de champ, " pour Pervasive */


	private Socket sc = null;
	private TrtClient sv = null;
	public String term = "";
      public String _login, _password;

        public String loginBDD = "faxmailer";
        public String passwordBDD = "xvnrfk";

	public clientTrtClient(TrtClient psv, Socket psc)
	{
		sv = psv;
		sc = psc;
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
			return str;
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

	public void run()
	{
 		Object[][] tabfaxrec = new Object[1000][23];
		
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
				commande = new String(recoit_param(is, /* "alpha" */ "any"));
				/*System.out.println ("Recu: " + commande);*/
				System.out.println ("Commande recue");

				interp intrp = new interp();
				entree e = new entree(commande);
				expr top = e.lirexpr();
				System.out.println ("Toplevel: " + top.rep());
				expr global = new expr("GLOBAL",1000);
				intrp.global = global;
				/* intrp.prog_frame = new expr(sv.mainWindow); */
				intrp.container = sv.mainWindow;
				System.out.println ("Demarrage de l'interprete...");
				expr r = intrp.run (top, new expr("Test"));
				System.out.println ("Result: " + r.rep());

				if(commande.equals(""))
				{
					erreur = true;
					break;
				}
				else if(commande.equals("\n"))
				   continue;


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
		}
		catch(Exception e)
		{
			e.printStackTrace();
		}
		sv.changeNumTransactions(-1);
	}



}

/**
 * thread client
 */
class finTrtClient extends Thread
{
	private TrtClient sv = null;
	
	public finTrtClient(TrtClient psv)
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

