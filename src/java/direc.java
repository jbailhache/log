
import java.io.*;

class direc
{
        public static void dir (String pathname, int level)
        {
                File d = new File (pathname); 
                String[] l = d.list ();
                for (int i=0; i<l.length; i++)
                {                        
                        for (int j=0; j<level; j++)
                                System.out.print ("\t");
                        System.out.println (l[i]);
                        try 
                        {
                                dir (pathname + "\\" + l[i], level+1);
                        }
                        catch (Exception e) { }
                }
        }

        public static void main (String[] args) throws java.io.IOException
        {
                System.out.println ("Directory listing");
                BufferedReader in = new BufferedReader 
                        (new InputStreamReader (System.in));
                System.out.print ("Pathname ? ");
                String pathname = in.readLine ();
                dir (pathname, 0);
        }
}

