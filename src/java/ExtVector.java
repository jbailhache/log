
import java.util.*;

interface Selector 
{
	public boolean select (Object x);
}

interface Sorter
{
	public int compare (Object x, Object y);
}

interface Mapper
{
	public Object map (Object x);
}

interface Comparator
{
	public boolean compare (Object x, Object y);
}

class FalseComparator implements Comparator
{
	public boolean compare (Object x, Object y)
	{
		return false;
	}
}

public class ExtVector extends Vector
{
	public ExtVector reverse ()
	{
		ExtVector r = new ExtVector ();
		for (int i=0; i<this.size(); i++)
			r.add (0, this.elementAt(i));
		return r;
	}

	public ExtVector select (Selector s)
	{
		ExtVector r = new ExtVector ();
		for (int i=0; i<this.size(); i++)
		{
			if (s.select(elementAt(i)))
				r.add (elementAt(i));
		}
		return r;
	}

	public ExtVector map (Mapper m)
	{
		ExtVector r = new ExtVector ();
		for (int i=0; i<size(); i++)
		{
			r.add (m.map (elementAt(i)));
		}
		return r;
	}

	public ExtVector sort (Sorter s)
	{
		ExtVector r = new ExtVector();
		for (int i=0; i<size(); i++)
		{
			boolean cont = true;
			for (int j=0; j<i && cont; j++)
			{
				if (s.compare (elementAt(i), r.elementAt(j)) < 0)
				{
					r.add (j, elementAt(i));
					cont = false;
				}
			}
			if (cont)
				r.add (elementAt(i));
		}
		return r;		
	}

	public void append (ExtVector v, Comparator c)
	{
		for (int i=0; i<v.size(); i++)
		{
			boolean found = false;
			for (int j=0; j<size() && !found; j++)
			{
				if (c.compare (v.elementAt(i), elementAt(j)))
					found = true;
			}
			if (!found)
				add(v.elementAt(i));
		}
	}

	public void insert (ExtVector v, Comparator c)
	{
		for (int i=v.size()-1; i>=0; i--)
		{
			boolean found = false;
			for (int j=0; j<size() && !found; j++)
			{
				if (c.compare (v.elementAt(i), elementAt(j)))
					found = true;
			}
			if (!found)
				add(0,v.elementAt(i));
		}
	}

	public static void main (String[] args)
	{
		ExtVector v = new ExtVector();
		v.add ("aaa");
		v.add ("bbbbbbbb");
		v.add ("ccccc");
		v.add ("ddddddd");
		v.add ("eeee");
		ExtVector r = v.reverse();
		int i;
		for (i=0; i<r.size(); i++)
			System.out.println ((String)(r.elementAt(i)));
		ExtVector s = v.select (new Selector () { 
			public boolean select (Object str)
			{
				return ((String)str).length() > 3;
			} });
		for (i=0; i<s.size(); i++)
			System.out.println ((String)(s.elementAt(i)));
		ExtVector t = v.sort (new Sorter () {
			public int compare (Object x, Object y)
			{
				if (((String)x).length() > ((String)y).length())
					return 1;
				else
					return -1;
			} });
		for (i=0; i<t.size(); i++)
			System.out.println ((String)(t.elementAt(i)));
		System.out.println();
		v.append(s, new Comparator () {
			public boolean compare (Object x, Object y)
			{
				return false;
			}});
 		/*
		v.insert(s, new Comparator () {
			public boolean compare (Object x, Object y)
			{
				return false;
			}});
		*/
		v.insert (s, new FalseComparator());
		for (i=0; i<v.size(); i++)
			System.out.println ((String)(v.elementAt(i)));



	}

}
