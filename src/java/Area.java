
import java.io.*;

public class Area
{
	public static void main (String [] args) throws Exception
	{
		InputStreamReader reader = new InputStreamReader (System.in);
		BufferedReader input = new BufferedReader (reader);
		System.out.print ("Enter the radius: ");
		String text = input.readLine();
		Double x = new Double (text);
		double r = x.doubleValue();
		System.out.println ("The ara of a circle of radius " + r + " is " + Math.PI * r * r);
	}
}
