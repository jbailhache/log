<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.0 Transitional//EN">
<!-- saved from url=(0046)http://logweb.terrashare.com/src/java/map.java -->
<HTML><HEAD>
<META http-equiv=Content-Type content="text/html; charset=windows-1252">
<META content="MSHTML 5.50.4522.1800" name=GENERATOR></HEAD>
<BODY><XMP>import java.io.*;

class map
{
        public static void trace (String s)
        {
                /* System.out.print (" [" + s + "] ");  */
        }
        public static void tabul (int level)
        {
                for (int j=0; j<level; j++)
                        System.out.print ("\t");
        }
        public static void dir (String pathname, String relpath, int level)
        {
                String[] l;
                try
                {
                        File d = new File (pathname); 
                        l = d.list ();
                        trace ("list called");
                }
                catch (Exception e)
                {
                        trace ("EXC1");
                        return;
                }
                trace ("1");
                /*
                tabul (level);
                trace ("2");
                int len = l.length;
                System.out.println ("<ul>");
                */
                trace ("3");
                for (int i=0; i<l.length; i++)
                {                        
                        trace ("i="+i);
                        if (i == 0)
                        {
                                tabul (level);
                                System.out.println ("<ul>");
                        }
                        trace (" *** ");
                        tabul (level);
                        System.out.println (
                                "<li><a href=\"" + relpath+"/"+l[i] + "\">" 
                                + l[i] + "</a>");
                        try 
                        { 
                                dir (pathname + "\\" + l[i], 
                                     relpath + "/" + l[i], level+1);
                        }
                        catch (Exception e) { trace ("EXC2"); } 
                }
                tabul (level);        
                System.out.println ("</ul>");
        }

        public static void main (String[] args) throws java.io.IOException
        {
                /* System.out.println ("Directory listing"); */
                BufferedReader in = new BufferedReader 
                        (new InputStreamReader (System.in));
                /* System.out.print ("Pathname ? "); */
                String pathname = in.readLine ();
                dir (pathname, ".", 0);
        }
}

</XMP></BODY></HTML>
