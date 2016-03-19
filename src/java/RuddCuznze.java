import java.lang.*;
import java.util.*;
import java.applet.*;
import java.awt.*;
import java.awt.event.*;
import java.awt.image.*;
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
import lizard.tiff.*;

    /** Affichage de fichiers TIFF compressés */
    public class TiffViewer {

	  public boolean[][] bitmap;
	  public int[] pixels;
	  Image img;
	  public int xgraphics, ygraphics, /* où afficher l'image */
		xbitmap, ybitmap, width, height, /* la partie du bitmap qu'on affiche */
		imageWidth, imageHeight; /* taille totale */
	  Graphics graphics;

	  /* public String filename = "c:\\jacques\\tiffy\\images\\00441.tif";    */
	  /* public String filename = "c:\\jacques\\tiffy\\images\\multipage.tif";   */
	  public String filename = "fax.tif";   

	  public int[] data;
	  public int ptr;
	  public int code_t4; /* bits t4 cadrés MSB */
 	  public int pos_code_t4; /* pointeur bit code_t4 à partir de LSB=0 */
	  public int t4size;
	  public int maxbits = 13;
        public int EOT4 = -1;
	  public int nbits, npixels;
	  int[][] tnbits, tnpixels;
	  int StripOffsets;
	  int stripoffset = 274;

	  public void trace (String s)
	  {
		/*System.out.println(s);*/
	  }

	  public void trace (int x)
	  {
		trace (" " + x);
	  }

	  public int unsign (/*byte*/ int b)
	  {
		/*
		if (b > 0)
			return b;
		else
			return b+256;
		*/
		return b;
	  }


	  public int shift (int x) /* cadrage LSB */
	  {
		return (x>>(32-maxbits)) & ((1<<maxbits)-1);
	  }
 
	  /** Fin des données */
	  public boolean eot4 () 
	  {
		return (ptr >= t4size); 
		/* return (ptr > stripoffset + 60);  */
			/* Test */	
  	  }

	  public int rev(int x)
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

	  /** Lecture d'un octet */
	  public int get_byte_t4 ()
	  {
		if (eot4())
			return EOT4;
		trace ("get_byte_t4->" + Integer.toHexString(data[ptr]));
		return rev(unsign(data[ptr++]));		
        }
 	
	  public int get_code_t4_a (int n)
        {
		code_t4 <<= n; 							/* on évacue les bits traités */
		pos_code_t4 += n; 						/* on décale le pointeur bit */
		while ((pos_code_t4 > (32 - maxbits)) && !eot4()) 	/* tant qu'on ne dépasse pas maxbits et qu'il y a encore des bits */
		{
			pos_code_t4 -= 8;						/* on avance le pointeur pour lire 1 octet */
			code_t4 |= get_byte_t4() << pos_code_t4;		/* on place l'octet lu dans code_t4 */
		}
		return shift(code_t4);
	  }

	  public int get_code_t4 (int n) 
	  {
		get_code_t4_a (n);
		if (shift(code_t4) == 0 || shift(code_t4) == 1)
		{
			while (shift(code_t4) != 2 && !eot4())
			{
				if (shift(code_t4) == 3)
					break; /* bad */
 				get_code_t4_a (1);
			}
		}
		trace ("code_t4=" + Integer.toHexString(code_t4) + " get_code_t4(" + n + ")->" + Integer.toHexString(shift(code_t4)));
		return shift(code_t4);
	  }

	  public int get_item_t4 (int color) /* retourne le nombre de pixels de la couleur color */
	  {
		boolean cont = true;
		while (cont)
		{
		   nbits = tnbits[color][shift(code_t4)];
		   npixels = tnpixels[color][shift(code_t4)];
		   if (nbits == -2)
		   {
			System.out.print ("?");
			get_code_t4 (1);
			if (eot4()) return 0;
		   }
		   else cont = false;
		}
		trace ("********************************* Color" + color + " " + npixels + " pixels, " + nbits + " bits");
		get_code_t4 (nbits);
		trace ("get_item_t4("+color+")->"+npixels);
		return npixels;
	  }

	  public void init_get_t4 ()
	  {
		pos_code_t4 = 32 /*31*/;
		get_code_t4(0);
		trace("T1");
	  }

        /*
	  public static void main(String[] args) {
            new EssaiViewTiff();
        }
	  */

	  /** Construction des tables de conversion */
	  public void buildT4Tables ()
	  {
					tnbits = new int[2][0x2000];
			 		tnpixels = new int[2][0x2000];

					T4Tables t4 = new T4Tables();
					for (int i=0; i<109; i++)									
						trace ("T4 white: " + t4.WhiteCodes[i][0] + " " + t4.WhiteCodes[i][1] + " " + t4.WhiteCodes[i][2]);
					for (int i=0; i<109; i++)
						 trace ("T4 black: " + t4.BlackCodes[i][0] + " " + t4.BlackCodes[i][1] + " " + t4.BlackCodes[i][2]);    
					for (int color=0; color<2; color++)
					for (int code=0; code<0x2000; code++)
					{
						/* On cherche un code t4 correspondant au début de code */
						int p = 2;
						for (int nb=2; nb<=maxbits; nb++)
						{
							p *= 2;
							/* On cherche si les nbits premiers bits de code correspondent à un code t4 */
							boolean trouve = false;
							for (int i=0; i<109; i++)
							{
 								if (color == 0)
								{
									if (t4.WhiteCodes[i][0] == nb &&
									    t4.WhiteCodes[i][1] == ((code >> (maxbits-nb)) & (p-1)))
									{
										tnbits[color][code] = nb;
										tnpixels[color][code] = t4.WhiteCodes[i][2];
										trouve = true; 
										break;
									}
								}
								else
								{
									if (t4.BlackCodes[i][0] == nb &&
									    t4.BlackCodes[i][1] == ((code >> (maxbits-nb)) & (p-1)))
									{
										tnbits[color][code] = nb;
										tnpixels[color][code] = t4.BlackCodes[i][2];
										trouve = true; 
										break;
									}
								}
							}
							if (trouve)
								break;
						}

					}
					/*					
					for (int color=0; color<2; color++)
					for (int code=0; code<0x2000; code++)							 
							trace ("Couleur" + color + " " + Integer.toHexString(code) + ": " + tnbits[color][code] + " " + tnpixels[color][code]);
					*/


	  }

	  /** Convertit les données TIFF compressées contenues dans data, 
		résultat dans bitmap */
	  public void convert () 
	  {
					
					Graphics2D g2d = (Graphics2D)graphics;

					/* analyse des données TIFF dans data */
					int offset = unsign(data[4]) + 256 * (unsign(data[5]) + 256 * (unsign(data[6]) + 256 * unsign(data[7])));
					trace ("offset: " + offset);

					int dec = unsign(data[offset]) + 256 * unsign(data[offset+1]);
					trace (dec);
			
					StripOffsets = 0;
					int[] values = new int[1000];

					for (int i=0; i<dec; i++)
					{
						int tag = unsign(data[offset+2]) + 256 * unsign(data[offset+3]);
						int typ = unsign(data[offset+4]) + 256 * unsign(data[offset+5]);
						int lng = unsign(data[offset+6]) + 256 * (unsign(data[offset+7]) 
							+ 256 * (unsign(data[offset+8]) + 256 * unsign(data[offset+9])));
						int val = unsign(data[offset+10]) + 256 * (unsign(data[offset+11])
							+ 256 * (unsign(data[offset+12]) + 256 * unsign(data[offset+13])));
						 
						trace ("DE " + i + ": " + tag + " " + typ + " " + lng + " " + val);

						values[tag] = val;

						/*if (tag == 273) StripOffsets = val;*/
						switch (tag)
						{
							case 256: imageWidth = val; break;
							case 257: imageHeight = val; break;
							case 273: StripOffsets = val; break;
							default: break;
							
						}
						
						

						offset += 12;
					}
					trace ("StripOffsets=" + StripOffsets);

					System.out.println ("imageWidth=" + imageWidth);

					/*buildT4Tables();*/

					/* stripoffset = data[StripOffsets] + 256 * (data[StripOffsets+1] + 256 * (data[StripOffsets+2] + 256 * data[StripOffsets+3])); */
					stripoffset = StripOffsets; 

					ptr = stripoffset;

					int color = 0;
					int nl = 0;
					int nc = 0;
					init_get_t4();
					trace ("T2");
					bitmap = new boolean[imageHeight][imageWidth];
					while (!eot4())
				      {
						trace ("T3");
						int npix = get_item_t4 (color);
						if (eot4()) break;
						if (npix == -1 /* EOL */)
						{
							trace ("Fin de ligne");
							color = 0;
							nl++;
							nc = 0;
						}
						else if (npix == -2) /* invalide */
						{
							
						}
						else /*if (npix > 0)*/
						{
							trace ("Couleur " + color + ": " + npix + " pixels");	
							
							int frh = 1;
							int frv = 1;
							/*
							if (color == 0)
								g2d.setColor(Color.white);
							else
								g2d.setColor(Color.black);
							*/
							/*
							for (int i=0; i<npix; i++)
								g2d.fillRect((nc+i)/frh, nl/frh, 1, 1);  
							*/
							/* g2d.drawLine (nc/frh, nl/frv, (nc+npix-1)/frh, nl/frv); */
							trace ("T4 " + imageHeight + " " + imageWidth);
							for (int i=0; i<npix; i++)
							{
								/*trace ("pixel " + nl + " " + (nc+i) + ": " + (color==1));*/								
								if (nl >= 0 && nl < imageHeight && nc+i >= 0 && nc+i < imageWidth)
								{
									bitmap[nl][nc+i] = color == 1;																 
								}
							}
							trace ("Line " + color + " " + nl + " " + nc + " " + npix);
							nc += npix;
							if (npix < 64)
								color = 1-color;
						}
						/*else
						{
						}*/
					}					
        }

	  public boolean[][] getBitmap ()
	  {
		if (bitmap == null)
			convert();
		return bitmap;
	  }

	  int argb (Color c)
	  {
		return 0x1000000 * c.getAlpha() 
			+ 0x10000 * c.getRed() 
			+ 0x100 * c.getGreen()
			+ c.getRed();
	  }

	  public void createPixels (Color background, Color ink, int width, int height)
	  {
		getBitmap();
		int i=0;
		pixels = new int[width*height];
		for (int y=0; y<height; y++)
		for (int x=0; x<width; x++)
		{
			int y1 = (y*imageHeight) / height;
			int x1 = (x*imageWidth) / width;
			boolean p = bitmap[y1][x1];
			int c;
			if (p)
				c = (int) (argb(ink));
			else
				c = (int) (argb(background));

			/* System.out.println ("y1=" + y1 + ", x1=" + x1 + " : " + p + " - " + c); */
 
			pixels[y*width+x] = c;
			 
			/*pixels[y*width+x] = (int) (bitmap[y*imageHeight/height][x*imageWidth/width] ? argb(ink) : argb(background));*/
		}
	  }	

	  public void createPixels (Color background, Color ink)
	  {
		createPixels (background, ink, imageWidth, imageHeight);
		/*
		getBitmap();
		int i=0;
		pixels = new int[imageWidth*imageHeight];
		for (int y=0; y<imageHeight; y++)
		for (int x=0; x<imageWidth; x++)
			pixels[y*imageWidth+x] = (int) (bitmap[y][x] ? argb(ink) : argb(background));
	 	*/
	  }

	  public int[] getPixels (Color background, Color ink)
	  {
		/*if (pixels == null)*/
		createPixels (background, ink);
		return pixels;
	  }

	  public void createImage (Color background, Color ink)
	  {
		createPixels (background, ink);
		DirectColorModel dcm = new DirectColorModel (32, 0xff0000, 0xff00, 0xff);
		Canvas c = new Canvas();
		img = c.createImage (new MemoryImageSource (imageWidth, imageHeight, dcm, pixels, 0, imageWidth));
	  }

	  public void createImage (Color background, Color ink, int width, int height)
	  {
		createPixels (background, ink, width, height);
		DirectColorModel dcm = new DirectColorModel (32, 0xff0000, 0xff00, 0xff);
		Canvas c = new Canvas();
		img = c.createImage (new MemoryImageSource (width, height, dcm, pixels, 0, width));
	  }

	  public Image getImage (Color background, Color ink)
	  {
		createImage (background, ink);
		return img;
	  }

	  /** Dessine l'image */
	  public void drawImage (Graphics g, Color background, Color ink)
	  {
		createImage (background, ink);
		g.drawImage (img, 0, 0, null);
	  }

	  /** Dessine l'image */
	  public void drawImage1 (Graphics g, Color background, Color ink, int x, int y, int width, int height)
	  {
		createImage (background, ink, width, height);
		/*Image img1 = img.getScaledInstance (width, height, img.SCALE_SMOOTH); */
		g.drawImage (img, x, y, null);
	  }

	  /** Dessine l'image */
	  public void drawImage (Graphics g, Color background, Color ink, int width, int height)
	  {
		createImage (background, ink);
		Image img1 = img.getScaledInstance (width, height, img.SCALE_SMOOTH); 
		g.drawImage (img1, 0, 0, null);
	  }

	  /** Dessine l'image */
 	  public void drawImage (Graphics g, Color background, Color ink, int x, int y, int width, int height)
	  {
		createImage (background, ink);
		Image img1 = img.getScaledInstance (width, height, img.SCALE_SMOOTH); 
		g.drawImage (img1, x, y, null);
	  }

	  /** Dessine l'image
		@param g Objet Graphics sur lequel on dessine
		@param background couleur de fond
		@param ink couleur d'encre
		@param xg abcisse où dessiner l'image
		@param yg ordonnée où dessiner l'image
		@param wg largeur de l'image dessinée
		@param hg hauteur de l'image dessinée
		@param xb abcisse du coin supérieur gauche de la portion de l'image à dessiner
		@param yb ordonnée
		@param wb largeur
		@param hb hauteur
		*/
	  public void draw (Graphics g, Color background, Color ink, 
						  int xg, int yg, int wg, int hg, 
						  int xb, int yb, int wb, int hb)
	  {
		/*if (bitmap == null)
			convert();*/
		getBitmap();
		Graphics2D g2d = (Graphics2D)g;
		for (int l=0; l<hg; l++)
		for (int c=0; c<wg; c++)
		{
		   int lb = yb+l*hb/hg;
		   int cb = xb+c*wb/wg;
		   if (cb >= 0 && cb < imageWidth && lb >= 0 && lb < imageHeight)
		   {
			if (bitmap[lb][cb])
				g2d.setColor(ink);
			else
				g2d.setColor(background);

			g2d.fillRect (xg+c, yg+l, 1, 1);
		   }
		}
	  }

	/*
	  public void test ()
	  {
            // Create a frame
            JFrame frame = new JFrame();
    
            // Add a component with a custom paint method
            frame.getContentPane().add(new TestComponent());
    
            // Display the frame
            int frameWidth = 900;
            int frameHeight = 600;
            frame.setSize(frameWidth, frameHeight);
            frame.setVisible(true);


	  }
	
	  class TestComponent extends JComponent
	  {
		public void paint (Graphics g)
	      {
		}

	  }
	*/

	  public void initTiffViewer ()
	  {
		bitmap = null;
		pixels = null;
		buildT4Tables();
        }

	  public void TiffViewer()
	  {
		initTiffViewer();
	  }

	  TiffViewer (int[] pdata, int pt4size)
	  {
		trace ("Création TiffViewer taille " + pt4size);
		initTiffViewer();
		data = pdata;
		t4size = pt4size;
		trace ("Création effectuée");
	  }

	  TiffViewer (byte[] pdata, int size)
        {
		initTiffViewer();
		data = new int[size];
		for (int i=0; i<size; i++)
			data[i] = unsign(pdata[i]);
		t4size = size;
        }

        public void EssaiViewTiff() {
            // Create a frame
            JFrame frame = new JFrame();
    
            // Add a component with a custom paint method
            frame.getContentPane().add(new MyComponent());
    
            // Display the frame
            int frameWidth = 900;
            int frameHeight = 600;
            frame.setSize(frameWidth, frameHeight);
            frame.setVisible(true);
        }
    
        class MyComponent extends JComponent {
            // This method is called whenever the contents needs to be painted
            public void paint(Graphics g) {
                // Retrieve the graphics context; this object is used to paint shapes
                Graphics2D g2d = (Graphics2D)g;
    
                // Draw an oval that fills the window
                int x = 0;
                int y = 0;
                int width = getSize().width-1;
                int height = getSize().height-1;
                /*g2d.drawOval(x, y, width, height);*/
		    /*
			g2d.setColor(Color.red);
		    	g2d.fillRect (x, y, width, height);
			*/
		    try
		    {
		    RandomAccessFile raf = new RandomAccessFile (filename, "r");

					byte b;
					int n = 0;
 
					Vector datavect = new Vector();
					try
					{
						while (true)
						{
							b = raf.readByte();
 
							datavect.addElement(new Byte(b));
							n++;
						}
					}
					catch (EOFException e)
					{
						/* normal, on ne fait rien */
					}
					catch (Exception e)
					{
						System.out.println ("Autre exception");
						System.out.println (e.toString());
						
					}	
					data = new int[n];
					/*datavect.copyInto (data);*/
					t4size = n;
					for (int k=0; k<n; k++)
					{
						data[k] = ((Byte)(datavect.elementAt(k))).byteValue();
						if (data[k] < 0)
							data[k] += 256;
					}

					int offset = data[4] + 256 * (data[5] + 256 * (data[6] + 256 * data[7]));
					trace ("offset: " + offset);

					int dec = data[offset] + 256 * data[offset+1];
					trace (dec);
			
					StripOffsets = 0;
					int[] values = new int[1000];

					for (int i=0; i<dec; i++)
					{
						int tag = data[offset+2] + 256 * data[offset+3];
						int typ = data[offset+4] + 256 * data[offset+5];
						int lng = data[offset+6] + 256 * (data[offset+7] 
							+ 256 * (data[offset+8] + 256 * data[offset+9]));
						int val = data[offset+10] + 256 * (data[offset+11]
							+ 256 * (data[offset+12] + 256 * data[offset+13]));
						 
						trace ("DE " + i + ": " + tag + " " + typ + " " + lng + " " + val);

						values[tag] = val;

						if (tag == 273)
							StripOffsets = val;

						offset += 12;
					}
					trace ("StripOffsets=" + StripOffsets);

					tnbits = new int[2][0x2000];
			 		tnpixels = new int[2][0x2000];

					T4Tables t4 = new T4Tables();
					for (int i=0; i<109; i++)									
						trace ("T4 white: " + t4.WhiteCodes[i][0] + " " + t4.WhiteCodes[i][1] + " " + t4.WhiteCodes[i][2]);
					for (int i=0; i<109; i++)
						 trace ("T4 black: " + t4.BlackCodes[i][0] + " " + t4.BlackCodes[i][1] + " " + t4.BlackCodes[i][2]);    
					for (int color=0; color<2; color++)
					for (int code=0; code<0x2000; code++)
					{
						/* On cherche un code t4 correspondant au début de code */
						int p = 2;
						for (int nb=2; nb<=maxbits; nb++)
						{
							p *= 2;
							/* On cherche si les nbits premiers bits de code correspondent à un code t4 */
							boolean trouve = false;
							for (int i=0; i<109; i++)
							{
 								if (color == 0)
								{
									if (t4.WhiteCodes[i][0] == nb &&
									    t4.WhiteCodes[i][1] == ((code >> (maxbits-nb)) & (p-1)))
									{
										tnbits[color][code] = nb;
										tnpixels[color][code] = t4.WhiteCodes[i][2];
										trouve = true; 
										break;
									}
								}
								else
								{
									if (t4.BlackCodes[i][0] == nb &&
									    t4.BlackCodes[i][1] == ((code >> (maxbits-nb)) & (p-1)))
									{
										tnbits[color][code] = nb;
										tnpixels[color][code] = t4.BlackCodes[i][2];
										trouve = true; 
										break;
									}
								}
							}
							if (trouve)
								break;
						}

					}
					/*					
					for (int color=0; color<2; color++)
					for (int code=0; code<0x2000; code++)							 
							trace ("Couleur" + color + " " + Integer.toHexString(code) + ": " + tnbits[color][code] + " " + tnpixels[color][code]);
					*/

					/* stripoffset = data[StripOffsets] + 256 * (data[StripOffsets+1] + 256 * (data[StripOffsets+2] + 256 * data[StripOffsets+3])); */
					stripoffset = StripOffsets; 

					ptr = stripoffset;

					int color = 0;
					int nl = 0;
					int nc = 0;
					init_get_t4();
					while (!eot4())
				      {
						int npix = get_item_t4 (color);
						if (eot4()) break;
						if (npix == -1 /* EOL */)
						{
							trace ("Fin de ligne");
							color = 0;
							nl++;
							nc = 0;
						}
						else /*if (npix > 0)*/
						{
							trace ("Couleur " + color + ": " + npix + " pixels");	
							
							int frh = 1;
							int frv = 1;
							if (color == 0)
								g2d.setColor(Color.white);
							else
								g2d.setColor(Color.black);
							/*
							for (int i=0; i<npix; i++)
								g2d.fillRect((nc+i)/frh, nl/frh, 1, 1);  
							*/
							g2d.drawLine (nc/frh, nl/frv, (nc+npix-1)/frh, nl/frv);
							trace ("Line " + color + " " + nl + " " + nc + " " + npix);
							nc += npix;
							if (npix < 64)
								color = 1-color;
						}
						/*else
						{
						}*/
					}					

		
			}
			catch (Exception e)
			{
				System.out.println(e.getMessage());
			}

            }
        }
    }
