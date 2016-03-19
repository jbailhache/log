
import java.lang.*;
import java.util.*;
import java.applet.*;
import java.awt.*;
import java.awt.event.*;
import javax.swing.*;
import javax.swing.text.*;
import javax.swing.border.*;
import javax.swing.table.*;
import javax.swing.event.*;
import java.io.*;
import java.net.*;
import java.text.*;
import java.util.zip.*;
import java.sql.*;

public class FaxMailerFunctions
{
 	  public static int reverseBits(int x)
        {			
		int r = 0;
		for (int i=0; i<8; i++)
		{
			if ((x & (1<<i)) != 0)
			{
				r |= 1<<(7-i);
			}
		}
		return r;
        }

	public static void dumpString (String s)
	{
		int i;
		int x;
		for (i=0; i<s.length(); i++)
		{
			if (i % 0x10 == 0)
				System.out.print ("\n" + Integer.toHexString(i) + ":");
			x = s.charAt(i);
			System.out.print(" " + Integer.toHexString(x));
		}
	}

	public static String TiffTag (int nt, int type, int len, int val)
	{
		String t = "";
		t += (char)(nt%0x100);
		t += (char)(nt/0x100);
		t += (char)type;
		t += (char)0;
		t += (char)len;
		t += (char)0;
		t += (char)0;
		t += (char)0;
		t += (char)(val%0x100);
		t += (char)((val/0x100)%0x100);
		t += (char)((val/0x10000)%0x100);
		t += (char)((val/0x1000000)%0x100);
		return t;
	}

	public static String stdTiff (String page)
	{
		dumpString (page.substring(0,300));
		String std = "";
		int StripOffsets = 266;
		int i, j;

		if (page.charAt(0) == 18)
		 page = page.substring(1);
		if (page.charAt(0) == 0)
		{
			std += (char)0x49;
			std += (char)0x49;
			std += (char)0x2A;
			std += (char)0;
			std += (char)8;
			std += (char)0;
			std += (char)0;
			std += (char)0;
			std += (char)0x12;
			std += (char)0;
			std += TiffTag (254, 4, 1, 2);
			std += TiffTag (0x100, 4, 1, 0x6C0);
			std += TiffTag (0x101, 4, 1, 0x9CF);
			std += TiffTag (0x102, 3, 1, 1);
			std += TiffTag (0x103, 3, 1, 3);
			std += TiffTag (0x106, 3, 1, 0);
			std += TiffTag (0x10A, 3, 1, 1); // FillOrder 1 ou 2 
			std += TiffTag (0x111, 4, 1, StripOffsets);
			std += TiffTag (0x115, 3, 1, 1);
			std += TiffTag (0x116, 4, 1, 0x9CF);
			std += TiffTag (0x117, 4, 1, page.length()-266); // longueur
			std += TiffTag (0x11A, 5, 1, 0xFA);
			std += TiffTag (0x11B, 5, 1, 0x102);
			std += TiffTag (0x124, 4, 1, 4); // T4Options
			std += TiffTag (0x128, 3, 1, 2);
			std += TiffTag (0x129, 3, 2, 0x10000); // PageNumber
			std += TiffTag (326, 3, 1, 0);
			std += TiffTag (327, 3, 1, 0);
			for (i=0; i</*60*/24; i++)
				std += (char)0;
			std += (char)0xCC;
			std += (char)0;
			std += (char)0;
			std += (char)0;
			std += (char)1;
			std += (char)0;
			std += (char)0;
			std += (char)0;
			std += (char)0x62;
			std += (char)0;
			std += (char)0; 
			std += (char)0;
			std += (char)1;
			std += (char)0;
			std += (char)0;
			std += (char)0;
			// std += page.substring(266);
			for (i=StripOffsets; i<page.length(); i++)
				std += (char)reverseBits(page.charAt(i));
		}
		else
		{
			int ImageLength = 0;
			int nt;
			int tagofs;

			char[] a = page.toCharArray();

			int ifdofs = a[4] + 0x100 * a[5] + 0x10000 * a[6] + 0x1000000 * a[7];
			int nbrtags = a[ifdofs] + 0x100 * a[ifdofs+1];

			int n = page.length()-StripOffsets;
			System.out.println ("\nlength=" + page.length() 
				+ " StripOffsets=" + StripOffsets 
				+ " n=" + n);

			for (i=nbrtags-1; i>=0; i--)
			{
				tagofs = ifdofs + 2 + i * 12;
				for (j=0; j<12; j++)
					a[tagofs+12+j] = a[tagofs+j];					
			}
			
			nbrtags += 3;
			a[ifdofs] = (char)(nbrtags%0x100);
			a[ifdofs+1] = (char)((nbrtags/0x100)%0x100);

			for (i=0; i<nbrtags; i++)
			{
				tagofs = ifdofs + 2 + i * 12;
				nt = a[tagofs] + 0x100 * a[tagofs+1];
				
				if (i == 0)
				{
					a[tagofs] = (char)254;
					a[tagofs+1] = (char)0;
					a[tagofs+2] = (char)4;
					a[tagofs+3] = (char)0;
					a[tagofs+4] = (char)1;
					a[tagofs+5] = (char)0;
					a[tagofs+6] = (char)0;
					a[tagofs+7] = (char)0;
					a[tagofs+8] = (char)2;
					a[tagofs+9] = (char)0;
					a[tagofs+10] = (char)0;
					a[tagofs+11] = (char)0;
				}
				else if (i == nbrtags-2)
				{
					a[tagofs] = (char)(326%0x100);
					a[tagofs+1] = (char)(326/0x100);
					a[tagofs+2] = (char)3;
					a[tagofs+3] = (char)0;
					a[tagofs+4] = (char)1;
					a[tagofs+5] = (char)0;
					a[tagofs+6] = (char)0;
					a[tagofs+7] = (char)0;
					a[tagofs+8] = (char)0;
					a[tagofs+9] = (char)0;
					a[tagofs+10] = (char)0;
					a[tagofs+11] = (char)0;
				}
				else if (i == nbrtags-1)
				{
					a[tagofs] = (char)(327%0x100);
					a[tagofs+1] = (char)(327/0x100);
					a[tagofs+2] = (char)3;
					a[tagofs+3] = (char)0;
					a[tagofs+4] = (char)1;
					a[tagofs+5] = (char)0;
					a[tagofs+6] = (char)0;
					a[tagofs+7] = (char)0;
					a[tagofs+8] = (char)0;
					a[tagofs+9] = (char)0;
					a[tagofs+10] = (char)0;
					a[tagofs+11] = (char)0;
				}
				else if (nt == 0x101)
				{
					ImageLength = a[tagofs+8] 
						+ 0x100 * a[tagofs+9]			
						+ 0x10000 * a[tagofs+10]
						+ 0x1000000 * a[tagofs+11];
					System.out.println ("Found ImageLength=" + ImageLength);
				}
				else if (nt == 0x10A)
				{
					a[tagofs+8] = (char)1;
					a[tagofs+9] = (char)0;
					a[tagofs+10] = (char)0;
					a[tagofs+11] = (char)0;
				}
				else if (nt == 0x116)
				{
					System.out.println ("Store ImageLength=" + ImageLength);
					a[tagofs+8] = (char)(ImageLength%0x100);
					a[tagofs+9] = (char)((ImageLength/0x100)%0x100);
					a[tagofs+10] = (char)((ImageLength/0x10000)%0x100);
					a[tagofs+11] = (char)((ImageLength/0x1000000) % 0x100);
				}
				else if (nt == 0x111)
				{
					System.out.println ("Store StripOffsets=" + StripOffsets);
					a[tagofs+8] = (char)(StripOffsets%0x100);
					a[tagofs+9] = (char)((StripOffsets/0x100)%0x100);
					a[tagofs+10] = (char)((StripOffsets/0x10000)%0x100);
					a[tagofs+11] = (char)((StripOffsets/0x1000000)%0x100);
				}
				else if (nt == 0x117)
				{
					System.out.println ("Store StripByteCount=" + n);
					a[tagofs+8] = (char)(n%0x100);
					a[tagofs+9] = (char)((n/0x100)%0x100);
					a[tagofs+10] = (char)((n/0x10000)%0x100);
					a[tagofs+11] = (char)((n/0x1000000)%0x100);
				}
				else if (nt == 292)
				{
					a[tagofs+8] = (char)4;
					a[tagofs+9] = (char)0;
					a[tagofs+10] = (char)0;
					a[tagofs+11] = (char)0;
				}
				else if (nt == 0x129)
				{
					a[tagofs+8] = (char)0;
					a[tagofs+9] = (char)0;
				}
				
			}

			for (i=StripOffsets; i<page.length(); i++)
				a[i] = (char)reverseBits(a[i]);


/*
			a[114] = a[30];
			a[115] = a[31];
			a[116] = a[32];
			a[117] = a[33];
			
			a[90] = (char)(StripOffsets%0x100);
			a[91] = (char)((StripOffsets/0x100)%0x100);
			a[92] = (char)((StripOffsets/0x10000)%0x100);
			a[93] = (char)((StripOffsets/0x1000000)%0x100);

			int n = page.length()-StripOffsets;
			
			a[126] = (char)(n%0x100);
			a[127] = (char)((n/0x100)%0x100);
			a[128] = (char)((n/0x10000)%0x100);
			a[129] = (char)((n/0x1000000)%0x100);
*/

			std = new String(a);			
		}
		dumpString(std.substring(0,300));
		return std;

	}

	public static byte[] stdTiffBytes (byte[] data)
	{
		try 
		{
			String s = new String (data, "ISO-8859-1");
			String t = stdTiff (s);
			byte[] std = t.getBytes ("ISO-8859-1");
			return std;
		}
		catch (Exception e)
		{
			e.printStackTrace();
			byte[] std = new byte[0];
			return std;
		}
	}

	public static String create_thumbnail (String file, String faxdir)
	{
						System.out.println ("Create thumbnail");
						/*** création de la vignette ***/

						/*
						String str_npages = m.getField("PGENB");
						Integer I1;
						try { I1 = new Integer(str_npages); }
						catch (Exception e)
						{ I1 = new Integer(0); };
						int npages = I1.intValue();
						System.out.println ("" + npages + " pages");
						*/

						FaxImage fi = new FaxImage();
						fi.curpage = 0;
						fi.nbrpages = 1; // npages;
						fi.pages = new Vector();
						fi.sizes = new Vector();

						String apercu = "???";

						for (int j=0; j<1; j++)
								    // npages
						{
							String filename = faxdir + "\\" 
							 + file /*m.getField("FILE")*/ + ".00" + j;
							System.out.println ("filename: " + filename);
							RandomAccessFile raf;
							try
                                          {
                                                raf = new RandomAccessFile (filename, "r");
                                          }
                                          catch (Exception e)
                                          {
                                                System.out.println ("Fin des fichiers");
                                                break;
                                          }
							byte b;
							int n1 = 0;
							Vector datavect = new Vector();
							try
							{
								while (true)
								{
									b = raf.readByte();
									datavect.addElement(new Integer(b));
									n1++;
								}
							}
							catch (EOFException e)
							{
								// normal, on ne fait rien 
							}
							catch (Exception e)
							{
								System.out.println ("Autre exception");
								e.printStackTrace();
								break;
							}			
							int[] data = new int[n1];
							for (int k=0; k<n1; k++)
								data[k] = ((Integer)(datavect.elementAt(k))).intValue();
							
							fi.pages.add(data);
							fi.sizes.add(new Integer(n1));

							TiffViewer tv = new TiffViewer (data, n1);
							tv.convert();
							// resultat Boolean tv.bitmap[l][c]

							int wa = 78; // 78; /*60;*/
							int ha = 55; //110; /*80;*/

							System.out.println ("wa=" + wa + " ha=" + ha);

							apercu = "";
							int x, y;
							
							/*		
							for (y=0; y<ha; y++)
							{
								for (x=0; x<wa; x++)
								{
									if (tv.bitmap[y*tv.imageHeight/ha][x*tv.imageWidth/wa])
										apercu += "1";
									else
										apercu += "0";  										
								}
							}*/
							int[][] a = new int[ha][wa];

							for (y=0; y<ha; y++)
							for (x=0; x<wa; x++)
								a[y][x] = 0;

							for (y=0; y<tv.imageHeight; y++)
							for (x=0; x<tv.imageWidth; x++)
							{
								if (tv.bitmap[y][x])
									a[y*ha/tv.imageHeight][x*wa/tv.imageWidth]++;
							}
			
							for (y=0; y<ha; y++)
							for (x=0; x<wa; x++)
							{
								/*
								if (a[y][x] > 1)
									apercu += "1";
								else
									apercu += "0";
								*/
								byte[] b1 = new byte[1];
								int c = (65+a[y][x]*ha*wa/8/60/80);
								if (c > 90) c = 90;
								b1[0] = (byte)c;

								try {
									apercu += new String (b1, "US-ASCII");
								} catch (Exception e) {
									apercu += "0";
								}
							}
							
							// m.add ("APERCU", apercu);

							// Compression de la vignette
							/*
							System.out.println ("Taille non compressee: " + apercu.length());
							try
							{
							ByteArrayOutputStream os = new ByteArrayOutputStream();
							GZIPOutputStream zip = new GZIPOutputStream (os);
							zip.write (apercu.getBytes("US-ASCII"), 0, apercu.length());
							zip.finish();
							byte[] bt = os.toByteArray();
							int i;
							apercu = "";
							for (i=0; i<bt.length; i++)
							{
								String hex = Integer.toHexString(bt[i]);
								if (hex.length() < 2)
									hex = "0" + hex;
								else if (hex.length() > 2)
									hex = hex.substring (hex.length()-2);
								apercu += hex;
								
								// if (bt[i] < 16)
								//	apercu += "0";
								// apercu += Integer.toHexString (bt[i]);
								
							}
							System.out.println ("Taille compressee: " + apercu.length());							
							System.out.println ("Vignette compressée: " + apercu);
							}
							catch (Exception e)
							{
								apercu = "";
							}
							// fin compression
							*/

							// Enregistrer la vignette dans la base fax 
							Connection con1 = null;
							try
							{
								Class.forName("sun.jdbc.odbc.JdbcOdbcDriver").newInstance();
                  					con1 = DriverManager.getConnection("jdbc:odbc:fax");

								String requete = 
								"INSERT INTO fax (\"FILE\", \"APERCU\") VALUES ('" 
								+ file /*m.getField("FILE")*/ + "', '" 
								+ apercu + "')";
								System.out.println (requete);

								con1.setAutoCommit(false);
								Statement st = con1.createStatement();
								st.executeUpdate (requete);
								con1.commit();

				                        con1.close();

							}
							catch (Exception eCon)
							{
								eCon.printStackTrace();
							}

							
						}			
						
						/*** Fin vignette ***/
						System.out.println ("end thumbnail");
						return apercu;
	}

}

