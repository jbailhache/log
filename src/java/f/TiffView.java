
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
import java.text.*;
import java.io.*;
import java.net.*;
import java.util.zip.*;
import java.sql.*;
import com.java4less.rmail.*;
import com.java4less.rmime.*;

import java.awt.image.*;
import lizard.tiff.Tiff;


public class TiffView 
{
	public static void main (String[] argv)
	{
		try
		{
			String filename = argv[0];
			System.out.println ("View TIFF file " + filename);
			RandomAccessFile raf = new RandomAccessFile (filename, "r");
			System.out.println ("File open");
			int b;
			Vector datavect = new Vector();
			int n = 0;
			try
			{
				System.out.println ("Reading data...");
				while (true)
				{
					b = raf.readByte();
					if (b<0)
						b += 256;
					datavect.addElement (new Integer(b));
					n++;
				}
			}
			catch (EOFException e)
			{				
				System.out.println ("End of file");
			}
			int[] data = new int[n];
			System.out.println ("Copying");
			for (int k=0; k<n; k++)
				data[k] = ((Integer)(datavect.elementAt(k))).intValue();

			System.out.println ("Create viewer");
			FaxViewer fv = new FaxViewer();
			System.out.println ("Viewer created");
			fv.language = "Francais";
			fv.frame = null;
			fv.titre = "Image TIFF";
			fv.nbrpages = 1;

			Vector sizes = new Vector();
			sizes.addElement(new Integer(n));
			fv.sizes = sizes;

			Vector pages = new Vector();
			pages.addElement (data);
			fv.pages = pages;

			System.out.println ("Viewing");
			fv.view();
			System.out.println ("Done");


			
		}
		catch (Exception e)
		{
			System.out.println (e.getMessage());
		}
	}
}
