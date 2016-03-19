
import java.applet.*;
import java.awt.*;
import java.net.*;
import java.lang.*;
 
public class intaplet extends Applet implements Runnable
{
      Thread runner;
      int t = 0;
     	Image image;
	
	expr global;

	expr prog_init;
	expr prog_step;
	expr prog_event;
	expr prog_mousedown;
	expr prog_action;
      expr prog_frame;

	TextArea messages; 
      TextArea trace;

      int n_action;

      int larg, haut;

      long temps = 50;
 
	String carspec;
/*
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
*/

	
	public void init ()
	{
   		
		/*System.out.println ("action: " + Event.ACTION_EVENT);*/

		messages = new TextArea ("", 3, 60);
            /*add (messages);*/
            messages.setText ("Messages");

		trace = new TextArea ("", 5, 40);
		/*add (trace);*/
		trace.setText ("Traces");

		global = new expr ("GLOBAL", 60);

		String src;
		entree e;

		carspec = getParameter ("spechar");
		if (carspec == null)
   		 carspec = " #\"\\;:()[]{}^%&";
		System.out.println ("spechar: " + carspec);

		String common;
		common = getParameter ("common");
	      if (common == null)
		 common = "";
		
		src = getParameter ("init");
	      if (src == null)
		 prog_init = null;
	      else
		{
		 e = new entree (common+src);
		 if (carspec != null)
		  e.carspec = carspec;
		 e.messages = messages;
		 prog_init = e.lirexpr();
		}

		src = getParameter ("step");
		if (src == null)
		 prog_step = null;
		else
		{
		 e = new entree (common+src);
		 if (carspec != null)
		  e.carspec = carspec;
		 e.messages = messages;
		 prog_step = e.lirexpr();
		}

		src = getParameter ("event");
            if (src == null)
             prog_event = null;
            else
            {
		 e = new entree (common+src);
		 if (carspec != null)
		  e.carspec = carspec;
		 e.messages = messages;
		 prog_event = e.lirexpr();
            }

		src = getParameter ("common+mousedown");
		if (src == null)
		 prog_mousedown = null;
	      else
	      {
		 e = new entree (src);
		 if (carspec != null)
		  e.carspec = carspec;
             e.messages = messages;
		 prog_mousedown = e.lirexpr();
		}

		src = getParameter ("common+action");
		if (src == null)
		 prog_action = null;
		{
		 e = new entree (src);
		 if (carspec != null)
		  e.carspec = carspec;
             e.messages = messages;
		 prog_action = e.lirexpr();
		}

		src = getParameter ("common+frame");
		if (src == null)
		 prog_frame = null;
		{
		 e = new entree (src);
		 if (carspec != null)
		  e.carspec = carspec;
             e.messages = messages;
		 prog_frame = e.lirexpr();
		}

		src = getParameter ("time");
            if (src != null)
		{
			Long x = new Long (src);
			temps = x.longValue();
            }	

                /*trace.setText (prog_init.rep());*/

		interp intrp = new interp();
            if (carspec != null)
             intrp.carspec = carspec;
		intrp.global = global;
		intrp.aplet = this;
            intrp.prog_frame = prog_frame;
		intrp.trace = trace;
		intrp.trace.setText ("before run");
		intrp.run(prog_init);


/*	
            im = getImage (getCodeBase(), "etoiles.gif");

		bouton = new Button ("Test");
		add (bouton);
	Object o = null;
	o = bouton;

		texte = new TextArea ("", 1, 20);
   		add (texte);
*/
	  larg=Integer.parseInt(getParameter("width"));
	  haut=Integer.parseInt(getParameter("height"));

	}

/*
     double f(double x) {
	       return (Math.cos((x+t)/10)+1) * size().height / 2;
    }

  
   double x_centre = 140;
   double y_centre = 90;
   double x_ampl = 100;
   double y_ampl = 40;
   long temps = 50;
   double vitesse = 0.02;
   double omega = 0.1;
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
*/

   public void paint (Graphics g)
   {
    update(g);
   }

   public void update (Graphics g)
   {
    if (image != null)
     g.drawImage (image, 0, 0, null);
   }

   public void avance ()
   {


    /*System.out.println ("Test");*/
    image = createImage (larg, haut);
    Graphics g = image.getGraphics(); 

    /*g.drawString (new Integer(n_action).toString(), 10, 20);*/


		interp intrp = new interp();
            if (carspec != null)
             intrp.carspec = carspec;
		intrp.global = global;
		intrp.aplet = this;
            intrp.graph = g;
		expr a = new expr((double)t);
		/*System.out.println ("Before run " + prog_step.rep() + " with " + a.rep());*/
		intrp.run(prog_step,a);


/*

    g.setColor (new Color (0, 0, 0));
    g.fillRect (0, 0, 299, 199);
    
    g.drawImage(im, 0, 0, this);
    
    x = abcisse(t);
    y = ordonnee(t);
 
    g.setColor (new Color (0, 128, 255));
    g.fillOval ((int)x-diametre/2, (int)y-diametre/2, diametre, diametre);
    for (int i=0; i<8; i++)
    for (int j=0; j<7; j++)
    {
     if (lettre_F[i][j]>0)
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

public boolean handleEvent (Event evt)
{
		if (evt.id == 503)
			return false;
		if (prog_event == null)
		{
		 switch (evt.id)
		 {
		 	case Event.ACTION_EVENT: 
				return action (evt, evt.arg);
                                
			case Event.MOUSE_DOWN:
				return mouseDown (evt, evt.x, evt.y);
                               
		  	default: 
			      return false;
		 }
 		}
		interp intrp = new interp();
            if (carspec != null)
             intrp.carspec = carspec;
		intrp.global = global;
		intrp.aplet = this;
                String s = "";
                try
                {
                 s = (String)(evt.arg);
                }
                catch (Exception e)
                {
                }
		expr x = intrp.run(prog_event, new expr ("EVENT",
			new expr((Object)evt),
			new expr((double)evt.id),
			new expr((Object)evt.arg),
                        new expr("STR",s),
			new expr((double)evt.x),
			new expr((double)evt.y) 
			));
		return x.valeur>0; 
 

}
 
public boolean mouseDown (Event evt, int xm, int ym)
{


		interp intrp = new interp();
            if (carspec != null)
             intrp.carspec = carspec;
		intrp.global = global;
		intrp.aplet = this;
		expr x = intrp.run(prog_mousedown, new expr("COORD", new expr((double)xm), new expr((double)ym)));
            return x.valeur>0;

/* texte.setText ("Clic");

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
*/
 /*return true;*/
}

public boolean action (Event evt, Object arg)
{
	n_action++;

		interp intrp = new interp();
		intrp.global = global;
		intrp.aplet = this;
                String s = "";
                try
                {
                 s = (String)arg;
                }
                catch (Exception e)
                {
                }
		expr x = intrp.run(prog_action,
             new expr("ACTION", 
                      new expr((Object)evt), 
                      new expr((Object)arg), 
                      new expr (s)));

	return x.valeur>0;
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

 

}
 
