
import java.net.*;
import java.io.*;

public class tcpserv
{
 ServerSocket server;

 public tcpserv (int port) throws IOException
 {
  server = new ServerSocket (port);
  System.out.println ("Port: " + server.getLocalPort());
 }

 public void launch ()
 {
  Socket service;
  while (true)
  {
   try
   {
    service = server.accept();
    System.out.println ("Connexion");
    PrintStream output = new PrintStream (service.getOutputStream());
    BufferedInputStream input = new BufferedInputStream (service.getInputStream());
    byte[] buf = new byte[1000];
    int status = input.read(buf);

    System.out.println ("Status=" + status);
/*
    for (int i=0; i<1000; i++)
     System.out.print ((char)(buf[i]));
*/
    String data = "";
    for (int i=0; i<1000; i++)
     data = data + (char)(buf[i]);
    System.out.println ("Received: " + data);

    output.println ("Bonjour !<p>");

    /* output.println ("Received:<p><pre>" + data + "</pre>"); */
    
    service.close ();
   }
   catch (Exception e)
   {
    System.err.println ("Error in server: " + e);
    continue;
   }
  }
 }

 public static void main (String[] args) throws Exception
 {
  tcpserv tcps;
  int port = 1234;
  tcps = new tcpserv (port);
  tcps.launch();
 }

}
