/* Auteur: Nicolas JANEY */
/* nico@univ-fcomte.fr   */
/* Novembre 2001         */

import java.lang.*;
import java.util.*;
import java.awt.*;
import java.awt.event.*;
import java.awt.event.*;
import java.awt.print.*;
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
 



public class ImpressionApp /*extends Applet*/
                        /*implements ActionListener,Printable*/ {

  static MyPainter mypainter;

  public static void main (String[] argv)
  {
  /*public void init() {*/
    /*setBackground(Color.white);*/
    mypainter = new MyPainter();
    JDialog dlg = new JDialog();
    dlg.setSize (300, 200);
    JButton b = new JButton("Test d'impression");
    dlg.getContentPane().add(b);
    b.addActionListener(new ActionListener() 
	{

  public void actionPerformed(ActionEvent e) {
    PrinterJob pj = PrinterJob.getPrinterJob();
    PageFormat pf = pj.defaultPage();
    pj.setPrintable(mypainter,pf);
    if ( pj.printDialog() )
      try {
        pj.print(); }
      catch(PrinterException pe) {
        System.out.println("Erreur d'impression" + pe.toString());};
  }

  



	});
    dlg.show();
  }

  /*public String getAppletInfo() {
    return "L'impression sous Java 2D.";
  }*/

}

class MyPainter implements Printable
{
public int print(Graphics graphics,PageFormat pageFormat,int page) {
    if ( page >= 1 )
      return(Printable.NO_SUCH_PAGE);
    Graphics2D g2 =(Graphics2D) graphics;
    g2.setColor(Color.black);
    g2.drawString("Ceci est un test d'impression",300,300);
    g2.draw(new Rectangle(100,200,300,400));
    return(Printable.PAGE_EXISTS);
  }        
}