import javax.swing.*;
import java.awt.event.*;

public class appli extends JLabel {
    int nclick = 0;

    public appli() {
         setText("Bonjour !");
    }

   
    public static void main(String [] args) {
	JFrame f = new JFrame();
	f.addWindowListener(new WindowAdapter() {
	    public void windowClosing(WindowEvent e) {
		System.exit(0);
	    }
	});
        f.setContentPane(new appli());
	f.pack();
	f.setVisible(true);
    }
}
