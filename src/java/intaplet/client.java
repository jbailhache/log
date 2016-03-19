
import java.io.*;
import java.net.*;

public class client
{
 public static void main (String args[])
 {
  try
  {
   Socket s = new Socket ("127.0.0.1", 1234);
   DataInputStream in = new DataInputStream (s.getInputStream());
   String line;
   while ((line = in.readLine()) != null)
    System.out.println("received: "+line);
  }
  catch (Exception e)
  {
   System.out.println (e.toString());
  }
 }
}
