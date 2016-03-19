
import java.net.*;
import java.io.*;

class server
{
 public static void main (String args[])
 {
  try
  {
   ServerSocket s = new ServerSocket (1234);
   Socket c = s.accept();
   DataOutputStream out = new DataOutputStream (c.getOutputStream());
   out.writeBytes ("Bonjour !");
   c.close();
   s.close();
  }
  catch (Exception e)
  {
   System.out.println (e.toString());
  }
 }
}
