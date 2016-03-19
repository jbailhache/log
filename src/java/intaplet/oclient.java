
import java.io.*;
import java.net.*;

public class oclient
{
 public static void main (String args[])
 {
  try
  {
   Socket s = new Socket ("127.0.0.1", 1234);
   DataOutputStream out = new DataOutputStream (s.getOutputStream());
   out.writeBytes ("Bonjour !");
   s.close();
  }
  catch (Exception e)
  {
   System.out.println (e.toString());
  }
 }
}
