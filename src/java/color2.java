
import java.applet.*;
import java.awt.*;
/*import java.awt.event.*;*/
import java.net.*;
import java.lang.*;
 
import sun.net.*;
import sun.net.www.*;
import sun.net.www.http.*;
  
public class color extends Applet implements Runnable
{
Thread runner;
int t = 0;

	Image image;
	int larg, haut;

	Button bouton;
	TextArea texte;
	int clicked = 0;

      Image im;

	int lettre_F[][] = {
	{1,1,1,1,1,1,1,1},
      {1,1,1,1,1,1,1,1},
      {1,1,0,0,0,0,0,0},
	{1,1,0,0,0,0,0,0},
      {1,1,1,1,1,0,0,0},
	{1,1,1,1,1,0,0,0},
	{1,1,0,0,0,0,0,0},
	{1,1,0,0,0,0,0,0},
	{1,1,0,0,0,0,0,0}};

	int htp1 = 6;
      int ltp1 = 11;
      int texte_planete1[][] = {
      {0,0,1,0,0,0,1,0,0,0,1},
      {0,1,0,1,0,0,1,0,0,0,1},
	{1,0,0,0,1,0,0,1,0,1,0},
	{1,1,1,1,1,0,0,1,0,1,0},
	{1,0,0,0,1,0,0,0,1,0,0},
	{1,0,0,0,1,0,0,0,1,0,0}
	};

	int htp = 6;
      int ltp = 23;
      int texte_planete[][] = {
      {0,0,1,1,1,0,0,0,1,0,0,0,1,0,0,0,1,0,0,0,1,0,0},
      {0,0,0,1,0,0,0,1,0,1,0,0,1,0,0,0,1,0,0,1,0,1,0},
	{0,0,0,1,0,0,1,0,0,0,1,0,0,1,0,1,0,0,1,0,0,0,1},
	{0,0,0,1,0,0,1,1,1,1,1,0,0,1,0,1,0,0,1,1,1,1,1},
	{1,0,0,1,0,0,1,0,0,0,1,0,0,0,1,0,0,0,1,0,0,0,1},
	{0,1,1,0,0,0,1,0,0,0,1,0,0,0,1,0,0,0,1,0,0,0,1}
	};



	public void init ()
	{
            im = getImage (getCodeBase(), "images/avternet.jpg");
		/*
		bouton = new Button ("Test");
		add (bouton);
		texte = new TextArea ("", 1, 20);
		add (texte);
		*/
	  larg=Integer.parseInt(getParameter("width"));
	  haut=Integer.parseInt(getParameter("height"));
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
   long temps = 50;
   double vitesse = 0.02;
   double omega = 0.1;
   int diametre = 40;
   int decalvert = -30;

   double abcisse (int t)
   {
    return x_centre + x_ampl * Math.cos (vitesse * t);
   }

   double ordonnee (int t)
   {
    return y_centre + y_ampl * Math.sin (vitesse * t) + decalvert;
   }

   double x, y;

   public void paint (Graphics g)
   {
    update(g);
   }

   public void update (Graphics g)
   {
    g.drawImage (image, 0, 0, null);
   }

   public void avance ()
   {
    image = createImage (larg, haut);
    Graphics g = image.getGraphics();
    g.setColor (new Color (0, 0, 128));
    g.fillRect (0, 0, 700, 200);
    /*if (t <= 1)*/
    {
     Image im = getImage (getCodeBase(), "images/avternet.jpg"); 
     /*g.drawImage(im, 0, 0, this);*/
    }

    double pi = 3.1415926;
    int k;
    for (int i=0; i<128; i++)
    for (int j=0; j<128; j++)
    {
     k = (int) (32.0 * (1.0 - Math.sin(pi/32.0*i)) *
			     (1.0 - Math.sin(pi/32.0*j)));
     int b = i*2;
     int v = j*2;
     int r = k*2;
     g.setColor(new Color(r,v,b));
     g.drawLine(i,j,i,j);
    }
    g.setColor (new Color ((int)(128+100*Math.sin(0.15*t)), 
				   (int)(128+100*Math.sin(0.16*t)),
				   (int)(128+100*Math.sin(0.17*t))));
    Font fonte = new Font ("Arial", Font.PLAIN, 60);
    g.setFont (fonte);
    /*g.drawString ("Avenet", 30, 100+decalvert);*/
    String societe = getParameter("societe");
    /*g.drawString (societe, 30, 100+decalvert);*/

    /*
    g.setColor (new Color (200, 150, 40));
    fonte = new Font ("Courier", Font.PLAIN, 30);
    g.setFont (fonte);
    g.drawString ("Cr�ation de sites internet", 140, 110+decalvert);
    */
    x = abcisse(t);
    y = ordonnee(t);
    /*
    Ellipse planete = new Ellipse (x, y, 20, 20);
    g.setColor (new Color (120, 200, 160));
    g.fill (planete);
    */
    /*g.setColor (new Color (0, 128, 255));*/
/*
    g.setColor (new Color ((int)(128+100*Math.sin(0.15*t)),
				   (int)(128+100*Math.sin(0.16*t)),
				   (int)(128+100*Math.sin(0.17*t))));
    g.fillOval ((int)x-diametre/2, (int)y-diametre/2, diametre, diametre);
    for (int i=0; i<htp; i++)
    for (int j=0; j<ltp; j++)
    {
     if (texte_planete[i][j]>0)
     {
	int yp = (int)y + (i - 4);
      double phi = - omega * t + (2.0 / diametre) * (j-3);
      if (Math.cos(phi) > 0)
      {
	 int xp = (int)(x + diametre/2*Math.sin(phi));
       g.setColor (new Color (10, 5, 20));
       g.drawLine (xp, yp, xp, yp);  
      }
     }
    }
*/

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
/* texte.setText (s); */
 if (d2 < diametre*diametre/4)
 {
 try
 {
  URL url = new URL (getCodeBase(), "logo.java");
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
   avance();
  }
  catch (InterruptedException e)
  {
   break;
  }
 }
}

/*    public void actionPerformed(ActionEvent e) {
	setVisible(false);
    }
*/

}
 
