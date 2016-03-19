

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



public class FaxViewer
{
	public int nbrpages;
	public Vector pages;
	public Vector sizes;
	public String language;    
	public String titre = null;
	public JFrame frame;

	String icondir = "c:/jacques/icones/";

/*
	FaxViewer (int pnbrpages, Vector ppages, Vector psizes)
      {
		nbrpages = pnbrpages;
		pages = ppages;
		sizes = psizes;	
      }
	
	FaxViewer (String language)
	{
	}
*/

	public void view ()
      {
		if (titre == null)
		{
			if (language.equals ("Francais"))
					titre = "Visualisation du fax";
				else
					titre = "Visualisation of the fax";

		}

		final FaxImageDialog dlg = new FaxImageDialog (frame, titre, false);
		dlg.setResizable (true);

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
		
		final JScrollBar vsb = new JScrollBar (JScrollBar.VERTICAL /*, 0, viewHeight, 0, faximage.imageHeight-viewHeight*/);
		vsb.getModel().addChangeListener (new ChangeListener ()
					{
						public void stateChanged (ChangeEvent e)
						{
							dlg.faximage.vpos1 = dlg.faximage.vpos;
							dlg.faximage.vpos = vsb.getValue() * (faximage.imageHeight /*- viewHeight*/) / (vsb.getMaximum() - vsb.getMinimum());
							if (dlg.faximage.vpos != dlg.faximage.vpos1)
								dlg.faximage.paintfax();
						}
					});
		faximage.vsb = vsb;

		dlg.getContentPane().add (vsb, BorderLayout.EAST);

		JPanel panebtnview = new JPanel();				
		panebtnview.setLayout (new FlowLayout());


				JButton btn_first = new JButton(); /* (language.equals("Francais")?"Première page":"Dernière page");*/
				btn_first.setIcon (new ImageIcon(icondir+"first.gif"));
			      btn_first.addActionListener (new ActionListener ()
					{
						public void actionPerformed (ActionEvent e)
						{
 
								dlg.faximage.curpage = 0;
								dlg.faximage.tv = null;
								dlg.faximage.paintfax();
								/* dlg.tfpage.setText (new Integer(dlg.faximage.curpage+1).toString()); */
								dlg.lpage.setText ("Page " + (dlg.faximage.curpage+1) + " / " + dlg.faximage.nbrpages);
 
						}
					});
				panebtnview.add (btn_first);                 				

				JButton btn_prev = new JButton (); /*(language.equals("Francais")?"Page précédente":"Previous page");*/
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
								/*dlg.tfpage.setText (new Integer(dlg.faximage.curpage+1).toString());*/
								dlg.lpage.setText ("Page " + (dlg.faximage.curpage+1) + " / " + dlg.faximage.nbrpages);
							}				
						}
					});
				panebtnview.add (btn_prev);

				JLabel lpage = new JLabel();
				panebtnview.add (lpage);
				lpage.setText ("Page 1 / " + nbrpages);
 				dlg.lpage = lpage;

				JButton btn_next = new JButton(); /* (language.equals("Francais")?"Page suivante":"Next page");*/
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
								/* dlg.tfpage.setText (new Integer(dlg.faximage.curpage+1).toString()); */
								dlg.lpage.setText ("Page " + (dlg.faximage.curpage+1) + " / " + dlg.faximage.nbrpages);
							}
						}
					});
				panebtnview.add (btn_next);                 

				JButton btn_last = new JButton (); /*language.equals("Francais")?"Dernière page":"Last page");*/
				btn_last.setIcon (new ImageIcon(icondir+"last.gif"));
			      btn_last.addActionListener (new ActionListener ()
					{
						public void actionPerformed (ActionEvent e)
						{
 
								dlg.faximage.curpage = dlg.faximage.nbrpages-1;
								dlg.faximage.tv = null;
								dlg.faximage.paintfax();
								/* dlg.tfpage.setText (new Integer(dlg.faximage.curpage+1).toString()); */
								dlg.lpage.setText ("Page " + (dlg.faximage.curpage+1) + " / " + dlg.faximage.nbrpages);
 
						}
					});
				panebtnview.add (btn_last);                 


				JButton btn_printpage = new JButton (language.equals("Francais")?"Imprimer":"Print");				
				btn_printpage.setIcon (new ImageIcon(icondir+"print3.gif"));
				btn_printpage.addActionListener (new ActionListener()
					{
						public void actionPerformed (ActionEvent e)
						{

MyPainter mypainter = new MyPainter ("Ceci est un test d'impression");
FaxPainter faxpainter = new FaxPainter (dlg.faximage/*.tv*/);
PrinterJob pj = PrinterJob.getPrinterJob();
PageFormat pf = pj.defaultPage();
pj.setPrintable(faxpainter,pf);

PrintRequestAttributeSet pras = new HashPrintRequestAttributeSet ();
pras.add (new PageRanges (1, dlg.faximage.nbrpages));

if ( pj.printDialog(pras) )
try {
pj.print(); }
catch(PrinterException pe) {
System.out.println("Erreur d'impression : " + pe.getMessage());};


						}

						
					});
				panebtnview.add(btn_printpage);

				/*JButton btn_ok = new JButton (language.equals("Francais")?"Fermer":"Close");*/
				/*
				btn_ok.addActionListener (new ActionListener ()
					{
					    public void actionPerformed (ActionEvent e)
					    {
						dlg.dispose();
					    }
					});
				*/
				/*
				btn_ok.addActionListener (new devt (dlg));
				panebtnview.add (btn_ok);
				*/

				dlg.getContentPane().add (panebtnview, BorderLayout.NORTH);
				
				if (nbrpages > 0)
					dlg.show();





	}

	
}
