
import java.applet.*;
import java.awt.*;
 
public class anim extends Applet implements Runnable {

Thread runner;
int t = 0;

     double f(double x) {
	       return (Math.cos((x+t)/10)+1) * size().height / 2;
    }

    public void paint(Graphics g) 
    {
        g.setColor (new Color(0,0,0));
	  g.fillRect (0, 0, size().width, size().height);
        g.setColor (new Color(200,50,10));
        for (int x = 0 ; x < size().width ; x++) 
        {
	          g.drawLine(x, (int)f(x), x + 1, (int)f(x + 1));
        }
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
   Thread.sleep(100);
  }
  catch (InterruptedException e)
  {
  }
 }
}


}
 
