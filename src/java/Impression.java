/* Auteur: Nicolas JANEY */
/* nico@univ-fcomte.fr   */
/* Novembre 2001         */

import java.awt.*;
import java.awt.geom.*;
import java.awt.event.*;
import java.applet.*;
import java.awt.print.*;

public class Impression extends Applet
                        implements ActionListener,Printable {

  public void init() {
    setBackground(Color.white);
    Button b = new Button("Test d'impression");
    add(b);
    b.addActionListener(this);
  }

  public String getAppletInfo() {
    return "L'impression sous Java 2D.";
  }

  public void actionPerformed(ActionEvent e) {
    PrinterJob pj = PrinterJob.getPrinterJob();
    PageFormat pf = pj.defaultPage();
    pj.setPrintable(this,pf);
    if ( pj.printDialog() )
      try {
        pj.print(); }
      catch(PrinterException pe) {
        System.out.println("Erreur d'impression");};
  }

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
