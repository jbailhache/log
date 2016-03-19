
    import java.awt.*;
    import javax.swing.*;
    import java.awt.image.*;
    import java.awt.image.PixelGrabber.*;

    
    public class visu {
        public static void main(String[] args) {
            new visu();
        }
        visu() {
            // Create a frame
            JFrame frame = new JFrame();
    
            // Add a component with a custom paint method
            frame.getContentPane().add(new MyComponent());
    
            // Display the frame
            int frameWidth = 300;
            int frameHeight = 300;
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
		    /*Image img = getToolkit().createImage ("c:\\jacques\\a.GIF");*/
		    /*Image img = Toolkit.getDefaultToolkit().getImage ("c:\\jacques\\a.GIF");*/
		    Image img = new ImageIcon ("c:\\jacques\\a.GIF") . getImage();
		    System.out.println ("img="+img);
		    g.drawImage (img, 0, 0, null);
                g2d.drawOval(x, y, width, height);
		    int w = 300;
		    int h = 200;
		    int[] pixels = new int[w*h];
		    
		    PixelGrabber pg = new PixelGrabber (img, 0, 0, w, h, pixels, 0, w);
		    try
		    {
			pg.grabPixels();
		    }
		    catch (Exception e)
		    {
			System.out.println (e);
		    }
	    
		    for (int xa=0; xa<w; xa++)		  
		    for (int ya=0; ya<h; ya++)
		    for (int xb=0; xb<w; xb++)
		    { System.out.println (" " + xa + " " + ya + " " + xb);
		    for (int yb=0; yb<h; yb++)
		    {
			    float s = 0;
			    for (int xp=0; xp<w; xp++)
			    for (int yp=0; yp<h; yp++)
			    {
				if (pixels[xp+yp*w] == 0)
				{
					float xpa = xa - xp;
					float ypa = ya - yp;
					float xpb = xb - xp;
					float ypb = yb - yp;
				      float t = (xpa*xpb + ypa*ypb) / ( xpa*xpa + ypa*ypa + xpb*xpb + ypb*ypb );
					s += t;
				}
		    	    }
			    float v = 1+2*s;
			    g.setColor (new Color(v, v, v));
			    g.drawLine (xa, ya, xb, yb);
		    }
		    }
            }
        }
    }
