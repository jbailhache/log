
import java.applet.*;
import java.awt.*;
import java.net.*;

public class welcome extends Applet 
{
 private Button cay;
 private Button gary;

 public void init()
 {
  /* Container pane = getContentPane(); 
  pane.setLayout (new BorderLayout());  */
  Label label = new Label (getParameter("greeting"));
  /*pane.*/ add (label/*, "Center"*/);
  
  cay = new Button ("Cay");
  /*cay.addActionListener (this);*/
  add (cay);
  
  gary = new Button ("Gary");
  /*gary.addActionListener (this);*/
  add (gary);
 } 
 
 public boolean action (Event evt, Object source)
 {
  /*Object source = evt.getSource();*/
  String url;
  if (source == cay)
   url = "http://www.horstmann.com";
  else if (source == gary)
   url = "mailto:gary@thecornells.com";
  else
   return true;
  try
  {
   URL u = new URL (url);
   getAppletContext().showDocument(u);
   return true;
  }
  catch (Exception e)
  {
   showStatus ("Error " + e);
   return false;
  }
 }

 /*
 public void actionPerformed (ActionEvent evt)
 {
  Object source = evt.getSource();
  String url;
  if (source == cay)
   url = "http://www.horstmann.com";
  else if (source == gary)
   url = "mailto:gary@thecornells.com";
  else
   return;
  try
  {
   URL u = new URL (url);
   getAppletContext().showDocument(u);
  }
  catch (Exception e)
  {
   showStatus ("Error " + e);
  }
 }
 */

}

