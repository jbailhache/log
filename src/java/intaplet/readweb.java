
import java.io.*;
import java.net.*;
 
public class readweb
{
 public static void main (String args[])
 {
  try
  {
   Socket s = new Socket ("127.0.0.1", 80);
   DataOutputStream out = new DataOutputStream (s.getOutputStream());
   DataInputStream in = new DataInputStream (s.getInputStream());
   String line;
   out.writeBytes ("GET /new/debat/debats.php3\n");
   while ((line = in.readLine()) != null)
   {
    System.out.println ("read: " + line);
   }
   s.close();
  }
  catch (Exception e)
  {
   System.out.println (e.toString());
  }
 }
}
