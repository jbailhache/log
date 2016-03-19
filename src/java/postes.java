import java.sql.*;
import java.awt.*; 
import java.awt.event.*;
import java.net.*;
import java.io.*;

public class postes extends java.applet.Applet
{
	Button bouton[];
	boolean clic[];
	int n;

	public void init ()
	{
		bouton = new Button [40];
		clic = new boolean[40];

		for (int i=0; i<40; i++)
			clic[i] = false;		
	}
	
	public boolean action (Event evt, Object obj)
	{
		for (int i=0; i<=n; i++)
		{
			if (evt.target == bouton[i])
			{
				clic[i] = true;
				repaint ();
				try 
				{
					URL url = new URL ("detail.htm");
					getAppletContext().showDocument (url);
				}
				catch (Exception e)
				{
				
				}

				return true;
			}
		}
		return false;
	}

	public void paint (Graphics screen)
	{
		
		String ref;

		setLayout (null);
		Font f = new Font ("TimesRoman", Font.BOLD, 12);
		screen.setFont (f);
		screen.setColor (Color.blue);

		/*
		try 
		{
			URL url = new URL ("file:///c:/projects/recrut/detail.htm");
			getAppletContext().showDocument (url);
		}
		catch (Exception e)
		{
			screen.drawString (e.getMessage(), 5, 200);			
		}
		*/

		String data = "jdbc:odbc:postes";
		try
		{
 			Class.forName ("sun.jdbc.odbc.JdbcOdbcDriver");
			Connection co = DriverManager.getConnection (data, "", "");
			Statement st = co.createStatement ();
			ResultSet rec = st.executeQuery (
				"SELECT * FROM Postes");
			int i = 0;
			while (rec.next())
			{
				i++;
				ref = rec.getString("Reference");
				screen.drawString (ref, 5, 40 * i);
				bouton[i] = new Button (ref);
				bouton[i].reshape (200, 40*i, 60, 20);
				add (bouton[i]);
				if (clic[i])
					screen.drawString ("Clic", 150, 40*i);
				n = i;
			}
			st.close ();
		}
		catch (Exception e)
		{
			System.out.println ("Erreur " + e.getMessage());
		}
	}

}

