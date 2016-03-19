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
import lizard.tiff.*;

    public class TestTiffViewer {
        public static void main(String[] args) {
            new TestTiffViewer();
        }
        TestTiffViewer() {
            // Create a frame
            JFrame frame = new JFrame();
    
            // Add a component with a custom paint method
            frame.getContentPane().add(new MyComponent());
    
            // Display the frame
            int frameWidth = 600;
            int frameHeight = 500;
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
                g2d.drawOval(x, y, width, height);

		    int[] data;
		    String filename = "fax.tif";
		    int t4size;
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

					TiffViewer tv = new TiffViewer (data, t4size);
					tv.convert();
					tv.draw (g, new Color(200,210,240) , new Color(10,20,40),
						30, 20, 300, 200, 100, 60, 600, 400); 
			}
			catch (Exception e)
			{
				System.out.println(e.getMessage());
			}


            }
        }
    }
