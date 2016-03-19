
import java.applet.*;
import java.awt.*;
import java.net.*;
import java.lang.*;

import sun.net.*;
import sun.net.www.*;
import sun.net.www.http.*;
 
public class anima extends Applet implements Runnable
{
Thread runner;
int t = 0;

	Button bouton;
	TextArea texte;
	int clicked = 0;

	public void init ()
	{
		bouton = new Button ("Test");
		add (bouton);
		texte = new TextArea ("", 6, 20);
		add (texte);
	}


     double f(double x) {
	       return (Math.cos((x+t)/10)+1) * size().height / 2;
    }

    public void paint1(Graphics g) 
    {
        g.setColor (new Color(0,0,0));
	  g.fillRect (0, 0, size().width, size().height);
        g.setColor (new Color(200,50,10));
        for (int x = 0 ; x < size().width ; x++) 
        {
	          g.drawLine(x, (int)f(x), x + 1, (int)f(x + 1));
        }
   }

   double x_centre = 140;
   double y_centre = 90;
   double x_ampl = 100;
   double y_ampl = 40;
   long temps = 100;
   double vitesse = 0.05;
   int diametre = 40;

   double abcisse (int t)
   {
    return x_centre + x_ampl * Math.cos (vitesse * t);
   }

   double ordonnee (int t)
   {
    return y_centre + y_ampl * Math.sin (vitesse * t);
   }

   double x, y;

   public void paint (Graphics g)
   {
    /*if (t <= 1)*/
    {
     Image im = getImage (getCodeBase(), "space.gif");
     g.drawImage(im, 0, 0, this);
    }
    x = abcisse(t);
    y = ordonnee(t);
    /*
    Ellipse planete = new Ellipse (x, y, 20, 20);
    g.setColor (new Color (120, 200, 160));
    g.fill (planete);
    */
    g.setColor (new Color (120, 200, 160));
    g.fillOval ((int)x-diametre/2, (int)y-diametre/2, diametre, diametre);
   }

/*
public boolean action (Event evt, Object arg)
{
 t = 0; 
 return true;
}
*/

public boolean mouseDown (Event evt, int xm, int ym)
{
/* t = 0;*/
 int d2 = (xm-(int)x)*(xm-(int)x)+(ym-(int)y)*(ym-(int)y);
 String s = "x="+x+"\ny="+y+"\nxm="+xm+"\nym="+ym+"\nd2="+d2;
 texte.setText (s); 
 if (d2 < diametre*diametre/4)
 {
 try
 {
  URL url = new URL (getCodeBase(), "page.htm");
  AppletContext ctx = getAppletContext();
  ctx.showDocument (url);
 }
 catch (MalformedURLException e)
 {
 }
 }

 return true;
}

public void start ()
{
 if (runner == null)
 {
  runner = new Thread(this);
  runner.start();
 }
}

public void stop()
{
  runner = null;
}

public void run ()
{
 Thread thisThread = Thread.currentThread();
 while (runner == thisThread)
 {
  repaint();
  t++;
  try
  {
   Thread.sleep(temps);
  }
  catch (InterruptedException e)
  {
  }
 }
}


}
 
