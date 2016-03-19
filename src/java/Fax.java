
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
 
public class Fax
{
	String numfax;
	Vector pages;
	String name, id, lst, flid;
	int retry, pri, hash, status, delay, pgnb;

	Fax ()
	{
		pages = new Vector();
		name = "B.A.T.";
		id = "B.A.T.";
		retry = 3;
		pri = 9;
		hash = 1;
		status = 0;
		delay = 120;
		flid = "AMPLITUDE";
		pgnb = 0;
	}

	public void addPage (byte[] data)
	{
		pages.addElement (data);
		pgnb++;
	}

	public String send ()
	{
		/* pgnb = pages.size(); */
		System.out.println ("Send " + pgnb + " pages to " + numfax);

		try
		{
			String baseBDD = "REC1"; /* a faire : parametrage */
			Connection con = null;
			Class.forName("sun.jdbc.odbc.JdbcOdbcDriver").newInstance();
                  con = DriverManager.getConnection("jdbc:odbc:" + baseBDD);

			int ref = 0;
			
			String requete = "SELECT * FROM \"SEND\"";
			ResultSet rs = con.createStatement().executeQuery (requete);
			while (rs.next())
			{
				int ref1 = rs.getInt ("REF");
				System.out.println ("REF: " + ref1);
				if (ref1 > ref)
					ref = ref1;
			}
			ref++;

			String dir = "C:\\FMW\\SENDTMP\\000000" + ref + ".001\\";
				/* A faire : parametrage */
			File fdir = new File(dir);
			fdir.mkdir();
			String lst = dir + "00000001";

			PrintWriter pw = new PrintWriter (new FileOutputStream (lst), false);
			for (int p=0; p<pgnb; p++)
			{
				String filename = dir + "FAX.00" + p;
				pw.println (filename);
				RandomAccessFile raf = new RandomAccessFile (filename, "rw");
				raf.write ((byte[])(pages.elementAt(p)));
				raf.close();
			}
			pw.close();

/*
			requete = "INSERT INTO \"SEND\" (\"REF\", \"NAME\", \"FAX\", \"ID\", \"LST\", \"RETRY\", \"PGNB\", \"PRI\", \"HASH\", \"STATUS\", \"DELAY\", \"FLID\") VALUES (" 
				+ ref + ", '"
				+ name + "', '" 
				+ numfax + "', '" 
				+ id + "', '"
				+ lst + "', "
				+ retry + ", " 
				+ pgnb + ", "
				+ pri + ", "
				+ hash + ", "
				+ status + ", "
				+ delay + ", '"
				+ flid + "')";

			requete = "INSERT INTO \"SEND\" (\"NAME\", \"FAX\", \"ID\", \"LST\", \"RETRY\", \"PGNB\", \"PRI\", \"HASH\", \"STATUS\", \"DELAY\", \"FLID\") VALUES ('" 
				+ name + "', '" 
				+ numfax + "', '" 
				+ id + "', '"
				+ lst + "', "
				+ retry + ", " 
				+ pgnb + ", "
				+ pri + ", "
				+ hash + ", "
				+ status + ", "
				+ delay + ", '"
				+ flid + "')";
*/

			requete = "INSERT INTO \"SEND\" (\"FAX\", \"LST\", \"RETRY\", \"PGNB\", \"PRI\", \"HASH\", \"DELAY\", \"FLID\") VALUES ('" 
				+ numfax + "', '" 
				+ lst + "', "
				+ retry + ", " 
				+ pgnb + ", "
				+ pri + ", "
				+ hash + ", "
				+ delay + ", '"
				+ flid + "')";




			System.out.println (requete);

			con.setAutoCommit(false);
			Statement st = con.createStatement();
			st.executeUpdate (requete);
			con.commit();
			return "OK";

		}
		catch (Exception e)
		{
			System.out.println ("Error queuing fax to send: " + e.getMessage());
			return ("Error queuing fax to send" + e.getMessage());
		}
	}
}