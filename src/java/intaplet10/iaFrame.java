
import java.applet.*;
import java.awt.*;
import java.awt.event.*;
import java.net.*;
import java.lang.*;
 
class iaFrame extends Frame
{
 expr prog_frame;
 String carspec;
 Applet aplet;
 expr global;

 iaFrame (String titre)
 {
  super(titre);
 }

 public boolean handleEvent (Event evt)
 {
  
		if (evt.id == 503)
			return false;
            if (evt.id == (int)(WindowEvent.WINDOW_CLOSING) || evt.id == (int)(WindowEvent.WINDOW_CLOSED))
			dispose();
		if (prog_frame == null)
		{
		 switch (evt.id)
		 {
/*
		 	case Event.ACTION_EVENT: 
				return action (evt, evt.arg);
                                
			case Event.MOUSE_DOWN:
				return mouseDown (evt, evt.x, evt.y);
  */                             
		  	default: 
			      return false;
		 }
 		}
		interp intrp = new interp();
            if (carspec != null)
             intrp.carspec = carspec;
		intrp.global = global;
                intrp.aplet = aplet;
                String s = "";
                try
                {
                 s = (String)(evt.arg);
                }
                catch (Exception e)
                {
                }
                expr x = intrp.run(prog_frame, new expr ("EVENT",
			new expr((Object)evt),
			new expr((double)evt.id),
			new expr((Object)evt.arg),
                        new expr("STR",s),
			new expr((double)evt.x),
			new expr((double)evt.y) 
			));
		return x.valeur>0; 
 

  
 }
}
