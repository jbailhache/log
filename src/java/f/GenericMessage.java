
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

public class GenericMessage
{
	public int nfields;
	public Vector names;
	public Vector values;
	
	public String xml;
	public int ptr;
	public char c;

	GenericMessage ()
	{
		nfields = 0;
		names = new Vector();
		values = new Vector();
	}

	GenericMessage (String s)
	{
		nfields = 0;
		names = new Vector();
		values = new Vector();

		xml = s;
		ptr = 0;
		c = xml.charAt(0);
		scan();		
	}

	public char getChar (boolean expected)
	{
		ptr++;
		if (ptr >= xml.length())
		{
			/*System.out.println ("Unexpected end of XML");*/
			c = (char)0;
		}
		else
			c = xml.charAt(ptr);
		if (!expected)
			System.out.print (c);
		return c;
	} 

	public char getChar ()
	{
		return getChar (true);
	}

	public void scanChar (char c1)
	{
		if (c != c1)
			System.out.println ("Character " + c + " found instead of " + c1);
		getChar();
	}

	public String scanString ()
	{
		String s = "";
		while (c != '<' && c != '>')
		{
			s = s + c;
			/*System.out.print ("[" + s + "]");*/
			getChar();
		}
		return s;
	}

	public String end_of_xml = "*** END ***";

	public String scanTag ()
	{
		while (c != '<' && c != (char)0) 
			getChar(false);
		if (c == (char)0)
			return end_of_xml;
		getChar();			
		String tag = scanString();
		scanChar('>');
		return tag;
	}

	public boolean scanItem ()
	{
		String tag = scanTag();
		if (tag.equals (end_of_xml) || tag.equals("/MESSAGE")) 
			return false;
		String value = scanString();
		if (value.length()>2)
		{
			if (value.substring(value.length()-2).equals(".0"))
				value = value.substring(0,value.length()-2);
		}
		String endtag = scanTag();
		/*System.out.println ("tag=" + tag + " value=" + value + " endtag=" + endtag);*/
		if (!endtag.equals("/"+tag))
			System.out.println ("Tags don't match: " + tag + " and " + endtag);
		nfields++;
		names.add(tag);
		values.add(value);
		return true;
	}

	public void scan ()
	{
		String tag = scanTag();
		if (!tag.equals("MESSAGE"))
			System.out.println ("Tag <MESSAGE> expected");
		while (scanItem()) {}
	}

	public String XML ()
	{
		String xml = "<MESSAGE>";
		for (int i=0; i<nfields; i++)
			xml += "<" + names.elementAt(i) + ">" + values.elementAt(i) + "</" + names.elementAt(i) + ">";		
		xml += "</MESSAGE>";
		return xml;
	}

	public String toString ()
	{
		return XML();
	}

	public String iaRep ()
	{
		String rep = "(MESSAGE";
		for (int i=0; i<nfields; i++)
			rep += " (" + names.elementAt(i) + " \"" + ((String)(values.elementAt(i))).replaceAll("\"","\\\\\"") + "\")";
		rep += ")";
		return rep;
	}

	public void add (String name, String value)
	{
		String value1 = value;
		try
		{
			value1 = value1.replace('\n',' ');
		}
		catch (Exception e) {}
		try
		{
			value1 = value1.replace('\r',' ');
		}
		catch (Exception e) {}
	
		nfields++;
		names.add(name);
		values.add(value1);
		/*values.add(value.replaceAll("\n"," ").replaceAll("\r"," "));*/

	}

	public String getName (int i)
	{
		return (String)names.elementAt(i);
	}

	public String getValue (int i)
	{
		return (String)values.elementAt(i);
	}

	public String getField (String name)
	{
		for (int i=0; i<nfields; i++)
		{
			if (names.elementAt(i).equals(name))
				return (String)values.elementAt(i);
		}
		return "";
	}

	public boolean activates (String basedroits, String scriptname)
	{
	   System.out.println ("act " + basedroits + " " + scriptname + " " + toString());
         try
	   {
		Vector champs_testes = new Vector();


							String s_rtr = getField("RTR") /*field[9]*/ /*results.getString("RTR")*/;
							Double X;
                        		      try { X = new Double(s_rtr); }
		                              catch (Exception e) { X = new Double(-1); }
							int rtr = (int)(X.doubleValue());
		
							String s_line = getField("LINE") /*field[10]*/ /*results.getString("LINE")*/;
                        		      try { X = new Double(s_line); }
		                              catch (Exception e) { X = new Double(-1); }
							int line = (int)(X.doubleValue());
							/*System.out.println ("s_line="+s_line+" line="+line);*/
		
							String id = getField("ID") /*field[0]*/ /*results.getString("ID")*/;
							String called = ""; /* provisoirement */
							String calling = "";

							String s_hng = getField("HGN") /*field[6]*/;
							try { X = new Double(s_hng); }
							catch (Exception e) { X = new Double(0); }
							int hng = (int)(X.doubleValue());
							int status = hng;

							String s_duration = getField("TIME") /*field[4]*/;
							try { X = new Double(s_duration); }
							catch (Exception e) { X = new Double(0); }
							int duration = (int)(X.doubleValue());
							
							String s_pagenb = getField("PGENB") /*field[5]*/;
							try { X = new Double(s_pagenb); }
							catch (Exception e) { X = new Double(0); }
							int pagenb = (int)(X.doubleValue());

							String rfn = getField("FILE") /*field[1]*/;
							String xdate = getField("CNX") /*field[2]*/;
							String xtime = getField("DCNX") /*field[3]*/;

						System.out.println ("rtr=" + rtr + " line=" + line + " id=" + id);
						System.out.println ("hng=" + hng + " duration=" + duration + " pagenb=" + pagenb + " rfn=" + rfn + " xdate=" + xdate + " xtime=" + xtime);
						
			

		String baseBDD = basedroits /* "droits" */;
		String loginBDD = "faxmailer";
		String passwordBDD = "xvnrfk";

		 
			
	

		Connection con_scripts = DriverManager.getConnection("jdbc:odbc:" + baseBDD, loginBDD, passwordBDD);
			
		boolean ok = true;

		String requete;
		ResultSet rs;

		requete = "SELECT * FROM scripts WHERE \"name\" = '" + scriptname + "'";
		System.out.println (requete);
		rs = con_scripts.createStatement().executeQuery (requete);
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

			System.out.println ("login1=" + login1 + " rtr1=" + rtr1 + " line1=" + line1 + " id1=" + id1 + " status1=" + status1 + " name1=" + name1 + " called1=<" + called1 + "> calling1=<" + calling1 + ">");

			if (!(  
							    (status1 > 0) &&
							    (rtr1 == -1 || rtr1 == rtr) &&
							    (line1 == -1 || line1 == line) &&
							    (id1 == null || id1.equals("") || id1.equals("*") || id1.equals(id)) &&
							    (called1 == null || called1.equals("") || called1.equals("*") || called1.equals(called)) &&
 							    (calling1 == null || calling1.equals("") || called1.equals("*") || calling1.equals(calling))))
			{
				System.out.println ("Non active");
				con_scripts.close();		
				return false;

			}

		}
		

		requete = "SELECT * FROM criteres WHERE \"nom\" = '" + scriptname + "'";
		System.out.println (requete);
		rs = con_scripts.createStatement().executeQuery (requete);
 
		while (rs.next())
		{
			String rsScript = rs.getString("script");
			String rsChamp = rs.getString("champ");
			String rsValeur = rs.getString("valeur");
			System.out.println ("script:" + rsScript + " champ:" + rsChamp + " valeur:" + rsValeur);
			if (champs_testes.indexOf(rsChamp) == -1)
			{
				System.out.println ("non teste");
				champs_testes.add(rsChamp);
				if (!getField(rsChamp).equals(rsValeur))
				{
					System.out.println ("non active");
					ok = false;
					break;
				}
			}
		}		
		

		con_scripts.close();	
		System.out.println ("ok=" + ok);	
		return ok;
         }
	   catch (Exception e)
	   {
		System.out.println (e.toString());
		return false;
	   }
	}

	public static void main (String[] args)
	{
		String xml1 = "<MESSAGE><ID>Amplitude</ID><PGENB>3</PGENB></MESSAGE>";
		GenericMessage m = new GenericMessage(xml1);
		System.out.println (m.toString());
	}

}
