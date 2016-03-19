
import java.net.*;
import java.io.*;

class iserver
{
 public static void main (String args[])
 {
  try
  {
   ServerSocket s = new ServerSocket (1234);
   Socket c = s.accept();
   DataInputStream in = new DataInputStream (c.getInputStream());
   String line;
   System.out.println ("Reading...");
   while ((line = in.readLine()) != null)
    System.out.println ("received: " + line);
   c.close();
   s.close();
  }
  catch (Exception e)
  {
   System.out.println (e.toString());
  }
 }
}
