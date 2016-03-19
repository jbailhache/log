
import java.awt.*;
import java.applet.*;

public class testact extends Applet
{
	Button bouton;
	int clicked = 0;

	public void init ()
	{
		bouton = new Button ("Test");
		add (bouton);
	}

	public boolean action (Event evt, Object arg)
	{
		if (evt.target instanceof Button)
			return actionBouton ((String)arg);
		else
			return true;
	}

	public void paint (Graphics g)
	{
		if (clicked > 0)
                        g.drawString ("Clicked !", 70, 30);
	}
	
	public boolean actionBouton (String label)
	{
		if (label.equals ("Test"))
			clicked += 1;
		repaint ();
		return true;
	}
			
}

