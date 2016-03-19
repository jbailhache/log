<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.0 Transitional//EN">
<!-- saved from url=(0047)http://www.webb.net/sites/log/src/lcs/expr.java -->
<HTML><HEAD>
<META content="text/html; charset=windows-1252" http-equiv=Content-Type>
<META content="MSHTML 5.00.2014.210" name=GENERATOR></HEAD>
<BODY><XMP>import java.io.*;
 
public class expr
{
	public String node;
	public int level;
	public expr prem[];
	public int nprem;

	expr (String nod, int np, expr p0, expr p1, expr p2)
	{
 		node = nod;
		level = 0;
		nprem = np;
		prem = new expr[3];
		prem[0] = p0;
		prem[1] = p1;
		prem[2] = p2;
	}
	
	expr (String nod, int l, int np, expr p0, expr p1, expr p2)
	{
 		node = nod;
		level = l;
		nprem = np;
		prem = new expr[3];
		prem[0] = p0;
		prem[1] = p1;
		prem[2] = p2;
	}

	void print (PrintStream out)
	{
		if (this == null)
			out.print ("null");
		else
		{
			out.print (node);
			if (level != 0)
				out.print (level);
			if (nprem > 0)
 			{
				out.print ("(");
				prem[0].print (out);
				for (int i=1; i<nprem; i++)
				{
					out.print (",");
					prem[i].print (out);
				}
				out.print (")");
			}
 		}	
	}
	
}
</XMP></BODY></HTML>
