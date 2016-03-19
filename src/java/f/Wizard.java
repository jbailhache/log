

import java.lang.*;
import java.util.*;
import java.awt.*;
import java.awt.event.*;
import java.awt.event.*;
import java.awt.print.*;
import java.awt.datatransfer.*;
import java.awt.datatransfer.Clipboard.*;
import javax.swing.*;
import javax.swing.border.*;
import javax.swing.table.*;
import javax.swing.event.*;
import javax.swing.text.*;
import javax.swing.filechooser.*;
import javax.print.attribute.*;
import javax.print.attribute.standard.*;
import java.text.*;
import java.io.*;
import java.net.*;
import java.util.zip.*;
import java.sql.*;
import com.java4less.rmail.*;
import com.java4less.rmime.*;

import java.awt.image.*;
import lizard.tiff.Tiff;

import java.beans.*;

class ptrint
{
	int val;
	ptrint (int pval)
	{
		val = pval;
	}
}

class DefaultReceiver implements StringReceiver 
{
	public void receive (String str)
	{
		System.out.println ("String received: " + str);
	}
}

class GenCode implements ActionListener
{
	Wizard w;
	GenCode (Wizard pw)
	{
		w = pw;
	}
	public void actionPerformed (ActionEvent e)
	{
		try
		{
			w.updateFields();
			String code = "% Script de traitement des fax recus %";
			for (int i=0; i<w.prog.size(); i++)
			{
				Instruction instr = (Instruction)(w.prog.elementAt(i));
				System.out.println ("Instruction " + instr.op);
				/*
				for (int j=0; j<instr.nargs; j++)
				{
				}
				*/
				String instr1 = instr.toString();
				String code1 = instr.code();
				code += "\n%# " + instr1 + " %\n" + code1;
			}

			System.out.println ("Code: " + code);

			try
			{
			FileOutputStream out = new FileOutputStream ("code.txt");
			out.write (code.getBytes("ISO-8859-1"));
			out.close();
			}
			catch (Exception e1)
			{
				System.out.println (e1.toString());
			}
			Clipboard cb = w.getToolkit().getSystemClipboard();
			StringSelection sel = new StringSelection(code);
			cb.setContents (sel, sel);

			w.rec.receive(code);

		}
		catch (Exception e1)
		{
			System.out.println (e1.toString());
		}	
	}
}

class ModifArg implements ActionListener /* PropertyChangeListener*/
{
	Wizard w;
	int i;
	int j;
	JTextField tf;

	ModifArg (Wizard pw, int pi, int pj, JTextField ptf)
	{
		w = pw;
		i = pi;
		j = pj;
		tf = ptf;
	}
	public void actionPerformed (ActionEvent e)
	/*public void propertyChange (PropertyChangeEvent evt)*/
	{
		String text = tf.getText();
            ((Instruction)(w.prog.elementAt(i))).args[j] = tf.getText();
		System.out.println ("Texte: " + text);
	}
}

class SelectOp implements ActionListener
{
	Wizard w;
	int i;
	JComboBox cbop;
	Textes textes = new Textes();
	int numlang;
	int y;

	SelectOp (Wizard pw, int pi, JComboBox pcbop, int pnumlang, int py)
	{
		w = pw;
		i = pi;
		cbop = pcbop;
		numlang = pnumlang;
		y = py;
	}

 
				public void actionPerformed (ActionEvent e)
				{
					w.updateFields();
					String op = textes.instr_unknown[numlang];
			
					for (int j=0; j<Instruction.ops.length; j++)
					{
						String op1 = new Instruction(Instruction.ops[j]).text()[numlang];
						if (((String)(cbop.getSelectedItem())).equals(op1))
						{
							op = Instruction.ops[j];
							break;
						}
					}
                              /* l.setText(op); */
                              ((Instruction)(w.prog.elementAt(i))).op = op;
					int n = ((Instruction)(w.prog.elementAt(i))).getnargs();
					((Instruction)(w.prog.elementAt(i))).args = new String[n];
					for (int k=0; k<n; k++)
						((Instruction)(w.prog.elementAt(i))).args[k] = ((Instruction)(w.prog.elementAt(i))).defaultargs()[k][numlang];
					System.out.println ("i=" + i + " size=" + w.prog.size());
					/*
					if (i == w.prog.size() - 1)
					{
						Instruction instr = new Instruction ("END");
						w.prog.addElement(instr);
					}
					*/
					if (op.equals("CLIENT"))
						w.prog.add (i+1, new Instruction("ENDCLIENT"));
                              w.displayProg (w.prog, 40);
					w.f.repaint();
				}

}

class Delete implements ActionListener
{
	Wizard w;
	int i;
	int y;
	Delete (Wizard pw, int pi, int py)
	{
		w = pw;
		i = pi;
		y = py;
	}
	public void actionPerformed (ActionEvent e)
	{
		System.out.println ("delete " + i);
		w.updateFields();
		w.prog.removeElementAt(i);
		w.updateScrollbar();
		w.displayProg(w.prog,40);
	}
}

class Insert implements ActionListener
{
	Wizard w;
	int i;
	int y;
	Insert (Wizard pw, int pi, int py)
	{
		w = pw;
		i = pi;
		y = py;
	}
	public void actionPerformed (ActionEvent e)
	{
		System.out.println ("insert " + i);
		w.updateFields();
		// Instruction instr = new Instruction ("PRINTFAX");
		Instruction instr = new Instruction ("CODE", "");
		w.prog.insertElementAt(instr,i);
		w.updateScrollbar();
		w.displayProg(w.prog,40);
	}
}

class Scroll implements ChangeListener
{
	Wizard w;
	Scroll (Wizard pw)
	{
		w = pw;
	}
	public void stateChanged (ChangeEvent evt)
	{
		w.vpos = (float)(w.vsb.getValue()) / (float)(w.vsb.getMaximum() - w.vsb.getMinimum());
		w.updateFields();		
		w.displayProg(w.prog,40);
	}
}

public class Wizard extends Component
{
	StringReceiver rec;

	/*JFrame f;*/
	JDialog f;
	JPanel pane;
	JLabel l;
	Textes textes = new Textes();
	JScrollBar vsb;

	int numlang;
	int dy = 20;
	int hl = 18;

	int dispHeight = 650;
	int progHeight;
	int shift;
	float vpos = 0;

	Vector prog = new Vector();

	Vector fields = new Vector();

	public static void main (String[] args)
	{
		Wizard w = new Wizard((String)null, new DefaultReceiver());
		/*
		System.out.println ("showing");
		w.f.show();
		System.out.println ("end");
		*/
	}

	public int addInstr (int y1)
      {
		final ptrint y = new ptrint(y1);
		final JComboBox cbop = new JComboBox();
		for (int i=0; i<Instruction.ops.length; i++)
		{
			cbop.addItem(new Instruction(Instruction.ops[i]).text()[numlang]);
		}		
		cbop.setBounds (30, y.val, 200, hl);
		y.val += dy;
		cbop.addActionListener (new ActionListener ()
			{
				public void actionPerformed (ActionEvent e)
				{
					int i;
					String op = textes.instr_unknown[numlang];
					for (i=0; i<Instruction.ops.length; i++)
					{
						String op1 = new Instruction(Instruction.ops[i]).text()[numlang];
						if (((String)(cbop.getSelectedItem())).equals(op1))
						{
							op = Instruction.ops[i];
							break;
						}
					}
					l.setText(op);
		
					int nargs = new Instruction(op).getnargs();

					for (i=0; i<nargs; i++)
					{
						String ltext = new Instruction(op).argnames()[i][numlang];
						System.out.println ("ltext=" + ltext + " y=" + y.val);
						JLabel l = new JLabel (ltext);
						l.setBounds (30, y.val, 200, hl);
						pane.add(l);

						JTextField tf = new JTextField ();
						tf.setBounds (240, y.val, 300, hl);
						pane.add (tf);

						y.val += dy;
					}

					y.val += 5;
		
					f.repaint();

					addInstr (y.val);

				}
			});

		pane.add(cbop);

		return 0;

      }

	public void updateFields ()
	{
					System.out.println ("update fields");
					for (int k=0; k<prog.size(); k++)
					{
						System.out.println ("Instruction " + k);
						for (int l=0; l<((Instruction)(prog.elementAt(k))).getnargs(); l++)
						{
							/*String text = (String)(((Vector)(fields.elementAt(k))).elementAt(l));*/
							Vector fields1 = (Vector)(fields.elementAt(k));
							Object o = (fields1.elementAt(l));              
							/*System.out.println ("o=" + o);*/
							JTextField tf = (JTextField)o;					
							String text = tf.getText();
							System.out.println (" " + k + " " + l + " : " + text);
							((Instruction)(prog.elementAt(k))).args[l] = text;
						}
					}
					System.out.println ("fin update fields");
	}

	public int getHeight ()
	{
		int h = 60 + dy;
		for (int i=0; i<prog.size(); i++)
		{
			String op = ((Instruction)(prog.elementAt(i))).op;
			int nargs = new Instruction(op).getnargs();
			h += (nargs+2) * dy;
		}
		return h;
	}

	public void updateScrollbar ()
	{
		
		progHeight = getHeight();
		if (progHeight < dispHeight)
			progHeight = dispHeight;
		shift = -(int)((float)(progHeight - dispHeight) * vpos);	
		int va = (vsb.getMaximum() - vsb.getMinimum()) * dispHeight/progHeight; 
		System.out.println ("progHeight=" + progHeight + " getHeight()=" + getHeight() 
			+ " shift=" + shift + " va=" + va);
		/*vsb.setVisibleAmount(va);*/
		
	}

	public void displayProg (Vector prog, int y)
	{
	   try
	   {
		pane.removeAll();
	
		progHeight = getHeight();
		if (progHeight < dispHeight)
			progHeight = dispHeight;
		shift = -(int)((float)(progHeight - dispHeight) * vpos);	
		/*vsb.setVisibleAmount (dispHeight/progHeight);*/

		System.out.println ("dispHeight=" + dispHeight + " getHeight()=" + getHeight() +
			" progHeight=" + progHeight + 
			" vpos=" + vpos + " shift=" + shift);
		
		JButton btn_gen = new JButton ("Générer le code");
		btn_gen.setBounds (30, shift+10, 150, hl);
		btn_gen.addActionListener (new GenCode (this));
		pane.add (btn_gen);

		/*
		vsb = new JScrollBar (JScrollBar.VERTICAL);
		vsb.setBounds (570, shift+10+dy, 20, 650-dy);
		vsb.getModel().addChangeListener (new Scroll(this));
		pane.add (vsb);
		*/

		int y1 = shift+40;
		
		fields = new Vector();
		for (int i=0; i<prog.size(); i++)
		{
			Vector fields1 = new Vector();
			System.out.println ("Instruction " + i);

			JComboBox cbop = new JComboBox();
                  for (int j=0; j<Instruction.ops.length; j++)
			{
                        String op1 = Instruction.ops[j];
				cbop.addItem(new Instruction(op1).text()[numlang]);
                        if (((Instruction)(prog.elementAt(i))).op.equals(op1))
                                        cbop.setSelectedIndex(j);
			}		
			cbop.setBounds (30, y1, 200, hl); 			
			System.out.println ("y cbop=" + y1);

                  cbop.addActionListener (new SelectOp (this, i, cbop, numlang, y1));
			pane.add (cbop);					 

			JButton btn_delete = new JButton ("Supprimer");
			btn_delete.setBounds (240, y1, 120, hl);
			btn_delete.addActionListener (new Delete(this,i,y1));
			pane.add (btn_delete);

			JButton btn_insert = new JButton ("Insérer");
			btn_insert.setBounds (370, y1, 120, hl);
			btn_insert.addActionListener (new Insert(this,i,y1));
			pane.add (btn_insert);
		 
			y1 += dy;
		
                        String op = ((Instruction)(prog.elementAt(i))).op;
 
					int nargs = new Instruction(op).getnargs();

					System.out.println ("op=" + op + " nargs=" + nargs);

                              for (int j=0; j<nargs; j++)
					{
						String ltext = new Instruction(op).argnames()[j][numlang];
						System.out.println ("ltext=" + ltext + " y=" + y1);
						JLabel l = new JLabel (ltext);
                                    l.setBounds (30, y1, 200, hl);
						pane.add(l);

                                    JTextField tf = new JTextField (((Instruction)(prog.elementAt(i))).args[j]);
                                    tf.setBounds (240, y1, 300, hl);
						tf.addActionListener/*addPropertyChangeListener*/ (new ModifArg(this,i,j,tf));							
						pane.add (tf);
						fields1.add(tf);

						y1 += dy;
					}

					y1 += dy;
					System.out.println ("y1=" + y1);
			
			fields.add(fields1);
		
		}
		System.out.println ("End prog");
		f.repaint();
		f.setSize (f.getSize().width+1, f.getSize().height);
		f.setSize (f.getSize().width-1, f.getSize().height);
		System.out.println ("End display");
	   }
	   catch (Exception e)
	   {
		System.out.println ("displayProg:" + e.getMessage());
	   }
	}

	Wizard (String code, StringReceiver prec)
	{
         try
	   {
		numlang = 0;

		rec = prec;
		
		/*f = new JFrame ("Définition d'un traitement de fax reçus");*/
		f = new JDialog ((JFrame)null, "Définition d'un traitement de fax reçu", true);
		f.setSize (600, 700);
 		/*f.setDefaultCloseOperation(JFrame.DISPOSE_ON_CLOSE);*/
		
		pane = new JPanel();
		pane.setLayout(null);

		/*f.setContentPane(pane);*/

		f.getContentPane().setLayout (new BorderLayout());
		f.getContentPane().add (pane, BorderLayout.CENTER); 

		vsb = new JScrollBar (JScrollBar.VERTICAL);
		vsb.setBounds (570, shift+10+dy, 20, 650-dy);
		vsb.getModel().addChangeListener (new Scroll(this));
		f.getContentPane().add (vsb, BorderLayout.EAST); 
		
		/*
		JScrollPane sp = new JScrollPane (pane);
		f.getContentPane().add (sp, BorderLayout.CENTER);		
		*/

		l = new JLabel ("Hello");
		l.setBounds (30, 10, 60, hl);
		pane.add(l);

		JButton b = new JButton ("Test");
		b.setBounds (120, 10, 70, hl);
		b.addActionListener (new ActionListener () 
			{
				public void actionPerformed (ActionEvent e)
				{
					l.setText("Bonjour");
				}
			});
		pane.add(b);

		int y = 40;
		
		String text;
		if (code != null)
			text = code;
		else
		{
		Clipboard cb = getToolkit().getSystemClipboard();
		Transferable data = cb.getContents(this);
		text = (String) data.getTransferData(DataFlavor.stringFlavor);
		System.out.println ("Clipboard contains: " + text);
		}
		if (text.equals(""))								
		{
		prog.add (new Instruction ("BEGIN"));
		/*
		prog.add (new Instruction ("CLIENT", "localhost", "6029"));
		prog.add (new Instruction ("MESSAGE", "\"Fax recu\"", "concat \"Vous avez recu un fax de\" id", "400", "100"));
		prog.add (new Instruction ("GETFAXPAGES", "localhost", "6020"));
		prog.add (new Instruction ("PRINTFAX"));
		prog.add (new Instruction ("VIEWFAX", "\"Fax recu\""));		
		prog.add (new Instruction ("ENDCLIENT"));
		prog.add (new Instruction ("MAILFAX", "", "", "", "", ""));
		*/
		prog.add (new Instruction ("END"));
		}
		else
		{
			String[] lines = text.split("\n");
			for (int i=0; i<lines.length; i++)
			{
			      if (lines[i].length() > 4)
				if (lines[i].substring(0,3).equals("%# "))	
				{
					String str = lines[i].substring(3,lines[i].length()-2);
					System.out.println(str);
					Instruction instr = new Instruction (str);
					prog.add (instr);
				}
			}
			if (prog.size() == 0)
			{
				prog.add(new Instruction("BEGIN"));
				prog.add(new Instruction("END"));
			}
		}

		updateScrollbar();
		displayProg (prog, 40);		 

		/*
		for (;;)
		{
			y = addInstr (y);
			if (y == 0)
				break;
		}
		*/

		f.show();
	   }
	   catch (Exception e)
	   {
		System.out.println (e.getMessage());
	   }
	}

/*
	Wizard ()
	{
		Wizard ((String)null);
	}
*/

}
