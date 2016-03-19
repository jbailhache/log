
import java.applet.*;
import java.awt.*;
import java.net.*;
import java.lang.*;
import java.io.*;
 
import java.util.Vector;
import java.awt.event.*;
import javax.swing.*;
import javax.swing.text.*;
import javax.swing.border.*;
import javax.swing.table.*;
import javax.swing.event.*;
import java.text.*;
import java.util.zip.*;
import java.sql.*;

import com.java4less.rmail.*;
import com.java4less.rmime.*;

class interp
{

 expr global;
 Applet aplet;
 expr prog_frame;
 Graphics graph;
 TextArea trace;
 TextArea messages;
 String carspec;
 Socket sock;

 interp ()
 {
  carspec = " #\"\\;:()[]{}^%&";
 }

 public boolean equal (expr x, expr y)
 {
  if (!x.noeud.equals(y.noeud))
   return false;
  if (x.valeur != y.valeur)
   return false;
  if (!x.chaine.equals(y.chaine))
   return false;
  if (x.arite != y.arite)
   return false;
  if (x.objet != y.objet) 
   return false;
  for (int i=0; i<x.arite; i++)
  {
   if (!equal(x.sousexpr[i], y.sousexpr[i]))
    return false;
  }
  return true;
 }


 public expr subst (expr ca, expr par, expr dans)
 {
  if (equal (ca, dans))
   return par; 
  else
  {
   expr se[] = new expr[dans.arite];
   for (int j=0; j<dans.arite; j++)
    se[j] = subst (ca, par, dans.sousexpr[j]);
   expr y = new expr (dans.noeud, dans.valeur, dans.chaine, 
	dans.objet, dans.arite, se);
   return y;
  }    
 }


 expr getarg (double n, expr args)
 {
  /*System.out.println ("getarg("+n+","+args.rep(carspec)+")");*/
  expr args1 = args;
  for (int i=0; i<n; i++)
  {
   args1 = args1.sousexpr[1];
   /*System.out.println(i+":"+args1.rep(carspec));*/
  }
  /*System.out.println ("final:"+args1.rep(carspec));*/
  return args1.sousexpr[0];
 }

 expr setarg (expr x, double n, expr args)
 {
  if (n == 0)
   return new expr ("PUSH", x, args.sousexpr[1]);
  else
   return new expr ("PUSH", args.sousexpr[0], 
	setarg (x, n-1, args.sousexpr[1]));
 }

 expr pushctx (expr x, expr ctx)
 {
  return new expr ("CTX", ctx.sousexpr[0], 
	new expr ("PUSH", x, ctx.sousexpr[1]),
	ctx.sousexpr[2], ctx.sousexpr[3]);
 }

 expr popctx (int n, expr ctx)
 {
  expr p = ctx.sousexpr[1];
  for (int i=0; i<n; i++)
   p = p.sousexpr[1];
  return new expr ("CTX", ctx.sousexpr[0].sousexpr[1], p,
	ctx.sousexpr[2], ctx.sousexpr[3]);
 }

 expr evol (expr ctx)
 {
  expr prog = ctx.sousexpr[0];
  expr pile = ctx.sousexpr[1];
  expr args = ctx.sousexpr[2];
  expr env  = ctx.sousexpr[3];
  expr instr = prog.sousexpr[0];
  String op = instr.noeud;
  expr cont = prog.sousexpr[1];
  expr r;

  r = ctx;

  /*System.out.println (ctx.rep());*/

  /*System.out.print (op);*/
  if (op.equals ("qt"))
  {
   if (instr.arite == 1)
    r = new expr ("CTX", cont, new expr("PUSH",instr.sousexpr[0],pile), args, env);
   else
    r = new expr ("CTX", cont, new expr("PUSH",instr,pile), args, env);  
  }
  else if (op.equals ("eval"))
   r = new expr ("CTX", new expr ("SEQ", pile.sousexpr[0], cont),
	pile.sousexpr[1], args, env);
  else if (op.equals ("evol"))
   r = new expr ("CTX", cont, new expr ("PUSH", evol(pile.sousexpr[0]), pile.sousexpr[1]),
	args, env);
  else if (op.equals ("SEQ"))
   r = new expr ("CTX", new expr ("SEQ", instr.sousexpr[0], new expr ("SEQ", instr.sousexpr[1], cont)),
	pile, args, env);
  else if (op.equals ("in"))
   r = new expr ("CTX", cont, pile.sousexpr[1], 
	new expr ("PUSH", pile.sousexpr[0], args), env);
  else if (op.equals ("ex"))
   r = new expr ("CTX", cont, pile, args.sousexpr[1], env);
  else if (op.equals ("LDB"))
  {
   expr args1 = new expr ("PUSH", pile.sousexpr[0], args);
   expr prog1 = new expr ("SEQ", instr.sousexpr[0], 
			new expr ("SEQ", new expr("ex"), cont));
   r = new expr ("CTX", prog1, pile.sousexpr[1], args1, env);
  }
  else if (op.equals ("arg"))
   r = new expr ("CTX", cont, new expr ("PUSH", getarg (instr.valeur, args), pile),
	args, env);
  else if (op.equals ("set"))
  {
   double val;
   if (instr.arite == 1)
    val = instr.sousexpr[0].valeur;
   else
    val = instr.valeur;
   r = new expr ("CTX", cont, pile.sousexpr[1],
	setarg (pile.sousexpr[0], val, args), env);
  }
  else if (op.equals ("Set"))
  {
   System.out.println ("set " + pile.niveau(0).rep());
   r = new expr ("CTX", cont, pile.sousexpr[1].sousexpr[1],
	setarg (pile.niveau(1), pile.niveau(0).valeur, args), env);
  }
  else if (op.equals ("eqthen"))
  {
   int x;
   if (equal (pile.sousexpr[0], pile.sousexpr[1].sousexpr[0]))
    x = 0;
   else
    x = 1;
   r = new expr ("CTX", new expr ("SEQ", instr.sousexpr[x], cont),
	pile.sousexpr[1].sousexpr[1], args, env);
  }
  else if (op.equals ("nop"))
   r = new expr ("CTX", cont, pile, args, env);
  else if (op.equals ("sign"))
  {
   double s = 0;
   if (pile.sousexpr[0].valeur > 0)
    s = 1;
   if (pile.sousexpr[0].valeur < 0)
    s = -1;
   r = new expr ("CTX", cont, new expr ("PUSH", new expr("NBR",s), pile.sousexpr[0]),
	args, env);
  }
  else if (op.equals ("plus"))
  {
   expr x;
   x = new expr ("NBR", pile.sousexpr[0].valeur + pile.sousexpr[1].sousexpr[0].valeur);
   r = new expr ("CTX", cont, new expr ("PUSH", x, pile.sousexpr[1].sousexpr[1]),
	args, env);
  }    
  else if (op.equals ("times"))
  {
   expr x;
   x = new expr ("NBR", pile.sousexpr[0].valeur * pile.sousexpr[1].sousexpr[0].valeur);
   r = new expr ("CTX", cont, new expr ("PUSH", x, pile.sousexpr[1].sousexpr[1]),
	args, env);
  }    
  else if (op.equals ("loop"))
   r = new expr ("CTX", new expr ("SEQ", instr.sousexpr[0], prog),
	pile, args, env);
  else if (op.equals ("exit"))
   r = new expr ("CTX", cont.sousexpr[1], pile, args, env);
  else if (op.equals ("arity"))
  {
   expr x;
   x = new expr ("NBR", pile.sousexpr[0].arite);
   r = new expr ("CTX", cont, new expr ("PUSH", x, pile.sousexpr[1]),
	args, env);
  }
  else if (op.equals ("node"))
   r = new expr ("CTX", cont, new expr ("PUSH", new expr (pile.sousexpr[0].noeud), pile.sousexpr[1]),
	args, env);
  else if (op.equals ("subexpr"))
   r = new expr ("CTX", cont, new expr ("PUSH", pile.sousexpr[0].sousexpr[(int)(pile.sousexpr[1].sousexpr[0].valeur)], pile.sousexpr[1].sousexpr[1]),
	args, env);
  /*
   r = new expr ("CTX", cont, new expr ("PUSH", pile.sousexpr[1].sousexpr[0].sousexpr[(int)(pile.sousexpr[0].valeur)], pile.sousexpr[1].sousexpr[1]),
	args, env);
  */

  else if (op.equals ("adse"))
  {
   expr x = pile.sousexpr[1].sousexpr[0];
   expr[] se = new expr[x.arite+1];
   for (int i=0; i<x.arite; i++)
    se[i] = x.sousexpr[i];
   se[x.arite] = pile.sousexpr[0];
   expr y = new expr (x.noeud, x.valeur, x.chaine, x.arite+1, se);
   r = new expr ("CTX", cont, new expr ("PUSH", y, pile.sousexpr[1].sousexpr[1]),
	args, env);
  }
  else if (op.equals ("copy"))
  {
   expr x = pile.sousexpr[0];
   expr y = new expr (x.noeud, x.valeur, x.chaine, x.objet, x.arite, x.sousexpr);
   r = new expr ("CTX", cont, new expr ("PUSH", y, pile.sousexpr[1]),
	args, env);
  }
  else if (op.equals ("mkexpr"))
  {
   String n = pile.sousexpr[0].noeud;
   int a = (int)(pile.sousexpr[1].sousexpr[0].valeur);
   expr x = new expr (n, a);
   r = new expr ("CTX", cont, new expr ("PUSH", x, pile.sousexpr[1].sousexpr[1]),
	args, env);
  }
  else if (op.equals ("rplac"))
  {
   expr x = pile.sousexpr[0];
   int i = (int)(pile.sousexpr[1].sousexpr[0].valeur);
   expr y = pile.sousexpr[1].sousexpr[1].sousexpr[0];
   x.sousexpr[i] = y;
   r = new expr ("CTX", cont, pile.sousexpr[1].sousexpr[1].sousexpr[1],
	args, env);
  }
  else if (op.equals ("mkstr"))
  {
   String s = "";
   for (int i=0; i<pile.niveau(0).valeur; i++)
    /*s += " ";*/
    s += (char)(int)(pile.niveau(1).valeur);
   r = pushctx (new expr("STR",s), popctx(2,ctx));
  }
  else if (op.equals ("concat"))
  {
   String s = pile.niveau(0).chaine + pile.niveau(1).chaine;
   /* System.out.println ("concat: " + s); */
   r = pushctx (new expr ("STR", s), popctx(2,ctx));
  }
  else if (op.equals ("charat"))
  {
   /*double x = pile.niveau(0).chaine.charAt((int)(pile.niveau(1).valeur));*/
   double x = pile.niveau(0).getdata()[((int)(pile.niveau(1).valeur))];
   r = pushctx (new expr(x), popctx(2,ctx));
  }
  /*
  else if (op.equals ("charset"))
  {
   
  }
  */
  else if (op.equals ("callcc"))
  {
   expr prog1 = new expr ("SEQ", instr.sousexpr[0], cont);
   expr ctx1 = new expr ("CTX", cont, pile, args, env);
   r = new expr ("CTX", prog1, new expr ("PUSH", ctx1, pile),
	args, env); 
  }
  else if (op.equals ("rcallcc"))
  {
   expr x = pile.sousexpr[1].sousexpr[0];
   expr ctx1 = pile.sousexpr[0];
   /*System.out.println("ctx1="+ctx1.rep());*/
   expr prog1 = ctx1.sousexpr[0];
   r = new expr ("CTX", prog1, 
	new expr ("PUSH", x, ctx1.sousexpr[1]), ctx1.sousexpr[2], ctx1.sousexpr[3]);
  }
  else if (op.equals ("excallcc"))
  {
   expr x = pile.sousexpr[1].sousexpr[0];
   expr ctx1 = pile.sousexpr[0];
   expr prog1 = new expr ("SEQ", x, ctx1.sousexpr[0]);
   r = new expr ("CTX", prog1, 
	ctx1.sousexpr[1], ctx1.sousexpr[2], ctx1.sousexpr[3]);
  }
  else if (op.equals ("printout"))
  {
   System.out.print (pile.sousexpr[0].chaine);
   r = new expr ("CTX", cont, pile.sousexpr[1], args, env);
  }
  else if (op.equals ("rep"))
  {
   r = new expr ("CTX", cont, 
	new expr ("PUSH", new expr ("STR", pile.sousexpr[0].rep(carspec)), pile.sousexpr[1]),
	args, env);
  }
  else if (op.equals ("compil"))
  {
   entree e = new entree (pile.sousexpr[0].chaine);
   e.carspec = carspec;
   expr x = e.lirexpr();
   r = new expr ("CTX", cont, new expr ("PUSH", x, pile.sousexpr[1]),
	args, env);
  }
  else if (op.equals ("setspechar"))
  {
   carspec = pile.niveau(0).chaine;
   r = popctx(1,ctx);
  }
  else if (op.equals ("readin"))
  {
   System.out.println ("\n" + ctx.rep());
   String line = "";
   byte[] b = new byte[1];
   char c;
   for (;;)
   {
    try
    {
     System.in.read (b);
     c = (char)(b[0]);
    }
    catch (Exception e)
    {
     System.out.println ("*** INPUT ERROR ***");
     c = '\n';
    }
    if (c == '\n' || c == '\r')
     break;
    line += c;
   }
   r = new expr ("CTX", cont,
	new expr ("PUSH", new expr ("STR", line), pile),
	args, env);
  }
  else if (op.equals ("global"))
  {
   r = new expr ("CTX", cont, new expr ("PUSH", global, pile),
	args, env);
  } 
  else if (op.equals ("ERROR"))
  {
   String s = "\nError: " + instr.sousexpr[0].chaine + instr.sousexpr[1].rep();
   System.out.println (s);
   if (messages != null)
    messages.setText (messages.getText() + s);
   r = popctx(0,ctx);
  }
  else if (op.equals ("subst"))
  {
   r = pushctx (subst (pile.niveau(0), pile.niveau(1), pile.niveau(2)), popctx(3,ctx));
  }
  else if (op.equals ("getimage"))
  {
   expr x = new expr (aplet.getImage(aplet.getCodeBase(),pile.sousexpr[0].chaine));
   r = pushctx (x, popctx(1,ctx));
  }
  else if (op.equals ("applet"))
   r = pushctx (new expr((Object)aplet), popctx(0,ctx));
  else if (op.equals ("panel"))
   r = pushctx (new expr((Object)new Panel()), popctx(0,ctx));
  else if (op.equals ("add"))
  {
   Container p = (Container)(pile.niveau(0).objet);
   String where = pile.niveau(1).chaine;
   Component c = (Component)(pile.niveau(2).objet);
   if (where.equals(""))
    p.add (c);
   else
    p.add (where, c);
   r = popctx (3, ctx);
  }
/*
  else if (op.equals ("setlayout"))
  {
   Container p = (Container)(pile.niveau(0).objet);
   Layout l = (Layout)(pile.niveau(1).objet);
   p.setLayout(l);
   r = popctx (2, ctx);
  }
  else if (op.equals ("flowlayout"))
   r = pushctx (new expr ((Object)new FlowLayout()), popctx(0,ctx));
  else if (op.equals ("borderlayout"))  
   r = pushctx (new expr ((Object)new BorderLayout()), popctx(0,ctx));
  else if (op.equals ("gridlayout"))
   r = pushctx (new expr ((Object)
          new GridLayout ((int)(pile.niveau(0).valeur),
                          (int)(pile.niveau(1).valeur)
                         )),
	popctx(2,ctx));
  */
  else if (op.equals ("setflowlayout"))
  {
   Container p = (Container)(pile.niveau(0).objet);
   FlowLayout l = new FlowLayout();
   p.setLayout(l);
   r = popctx(1,ctx);
  }
  else if (op.equals ("setborderlayout"))
  {
   Container p = (Container)(pile.niveau(0).objet);
   BorderLayout l = new BorderLayout();
   p.setLayout(l);
   r = popctx(1,ctx);
  }
  else if (op.equals ("setgridlayout"))
  {
   Container p = (Container)(pile.niveau(0).objet);
   GridLayout l = new GridLayout((int)(pile.niveau(1).valeur),(int)(pile.niveau(2).valeur));
   p.setLayout(l);
   r = popctx(3,ctx);
  }

  else if (op.equals ("button"))
  {
   Button b;
   b = new Button (pile.sousexpr[0].chaine);
   /*aplet.add (b);*/
   expr x = new expr ((Object)b);
   r = pushctx (x, popctx(1,ctx));
  }

  else if (op.equals ("textarea"))
  {
   TextArea t;
   t = new TextArea (pile.niveau(0).chaine, 
	               (int)(pile.niveau(1).valeur),
     			   (int)(pile.niveau(2).valeur));
   /*aplet.add (t);*/
   r = pushctx (new expr((Object)t), popctx(3,ctx));
  }
  else if (op.equals ("settext"))
  {
   TextComponent t = (TextComponent)(pile.niveau(0).objet);
   String s = pile.niveau(1).chaine;
   /* System.out.println ("settext: " + s); */
   t.setText(s);
   r = popctx(2,ctx);
  }
  else if (op.equals ("gettext"))
  {
   TextComponent t = (TextComponent)(pile.niveau(0).objet);
   String s = t.getText();
   /* System.out.println ("gettext: " + s); */
   r = pushctx (new expr ("STR", s), popctx(1,ctx));
  }
  else if (op.equals ("label"))
  {
   Label c;
   c = new Label (pile.niveau(0).chaine);
   r = pushctx (new expr((Object)c), popctx(1,ctx));
  }
  else if (op.equals ("textfield"))
  {
   TextField c;
   c = new TextField (pile.niveau(0).chaine, (int)(pile.niveau(1).valeur));
   r = pushctx (new expr((Object)c), popctx(2,ctx));
  }
/*
  else if (op.equals ("setechochar"))
  {
   TextField c = (TextField)(pile.niveau(0).objet);
   c.setEchoChar (pile.niveau(1).chaine.charAt(0));
   r = popctx (2, ctx);
  }
*/
  else if (op.equals ("checkboxgroup"))
  {
   CheckboxGroup c = new CheckboxGroup();
   r = pushctx (new expr((Object)c), popctx(0,ctx));
  }
  else if (op.equals ("getcurrent"))
  {
   CheckboxGroup g = (CheckboxGroup)(pile.niveau(0).objet);
   Checkbox c = g.getCurrent();
   r = pushctx (new expr((Object)c), popctx(1,ctx));
  }
  else if (op.equals ("getlabel"))
  {
   Checkbox c = (Checkbox)(pile.niveau(0).objet);
   r = pushctx (new expr("STR",c.getLabel()), popctx(1,ctx));
  }
  else if (op.equals ("checkbox"))
  {
   Checkbox c;
   CheckboxGroup g = (CheckboxGroup) (pile.niveau(1).objet);
   boolean b = pile.niveau(2).valeur>0;
   if (g == null)
    c = new Checkbox (pile.niveau(0).chaine);
   else
    c = new Checkbox (pile.niveau(0).chaine, g, b);
   r = pushctx (new expr ((Object)c), popctx(3,ctx));
  }
  else if (op.equals ("getstate"))
  {
   Checkbox c = (Checkbox) (pile.niveau(0).objet);
   expr x;
   if (c.getState())
    x = new expr ((double)1);
   else
    x = new expr ((double)0);
   r = pushctx (x, popctx(1,ctx));
  }
  else if (op.equals ("setstate"))
  {
   Checkbox c = (Checkbox) (pile.niveau(0).objet);
   c.setState (pile.niveau(1).valeur>0);
   r = popctx(2,ctx);
  }
/*
  else if (op.equals ("checkbox"))
  {
   Checkbox c;
   boolean flag;
   if (pile.niveau(1).valeur > 0)
    flag = true;
   else
    flag = false;
   c = new Checkbox (pile.niveau(0).chaine, flag);
   aplet.add (c);
   r = pushctx (new expr ((Object)c), popctx(2,ctx));
  }
  */
  else if (op.equals ("choice"))
  {
   Choice c = new Choice ();
   r = pushctx (new expr((Object)c), popctx(0,ctx));
  }
  else if (op.equals ("additem"))
  {
   if (pile.niveau(0).objet instanceof Choice)
   {
    Choice c = (Choice)(pile.niveau(0).objet);
    c.addItem (pile.niveau(1).chaine);
   }
   else if (pile.niveau(0).objet instanceof List)
   {
    List c = (List)(pile.niveau(0).objet);
    c.addItem (pile.niveau(1).chaine);
   }
   r = popctx(2,ctx);
  }
  else if (op.equals ("getselecteditem"))
  {
   String s = "";
   if (pile.niveau(0).objet instanceof Choice)
   {   
    Choice c = (Choice)(pile.niveau(0).objet);
    s = c.getSelectedItem();
   }
   else if (pile.niveau(0).objet instanceof List)
   {
    List c = (List)(pile.niveau(0).objet);
    s = c.getSelectedItem();
   }
   r = pushctx (new expr("STR",s), popctx(1,ctx));
  }
  else if (op.equals ("list"))
  {
   List c = new List ((int)(pile.niveau(0).valeur), pile.niveau(1).valeur>0);
   r = pushctx (new expr((Object)c), popctx(2,ctx)); 
  }
  else if (op.equals ("frame"))
  {
   iaFrame f = new iaFrame (pile.niveau(0).chaine);
   f.prog_frame = prog_frame;
   f.carspec = carspec;
   f.aplet = aplet;
   f.global = global;
   /*f.show();*/
   /*f.resize ((int)(pile.niveau(1).valeur), (int)(pile.niveau(2).valeur));*/
   r = pushctx (new expr ((Object)f),popctx (1, ctx));
  }
  else if (op.equals ("show"))
  {
   Frame f = (Frame)(pile.niveau(0).objet);
   f.show();
   r = popctx(1,ctx);
  }
  else if (op.equals ("hide"))
  {
   Frame f = (Frame)(pile.niveau(0).objet);
   f.hide();
   r = popctx(1,ctx);
  }
  else if (op.equals ("resize"))
  {
   Frame f = (Frame)(pile.niveau(0).objet);
   f.resize ((int)(pile.niveau(1).valeur), (int)(pile.niveau(2).valeur));
   r = popctx (3, ctx);
  }
  else if (op.equals ("menubar"))
  {
   MenuBar c = new MenuBar();
   r = pushctx (new expr((Object)c), popctx(0,ctx));
  }
  else if (op.equals ("setmenubar"))
  {
   Frame f = (Frame)(pile.niveau(0).objet);   
   MenuBar b = (MenuBar)(pile.niveau(1).objet);
   f.setMenuBar(b);
   r = popctx(2,ctx);
  }
  else if (op.equals ("menu"))
  {
   Menu c = new Menu (pile.niveau(0).chaine);
   r = pushctx (new expr((Object)c), popctx(1,ctx));
  }
  else if (op.equals ("addmenuitem"))
  {
   Menu c = (Menu)(pile.niveau(0).objet);
   c.add(new MenuItem(pile.niveau(1).chaine));
   r = popctx(2,ctx);
  }
  else if (op.equals ("addmenu"))
  {
   MenuBar b = (MenuBar)(pile.niveau(0).objet);
   Menu m = (Menu)(pile.niveau(1).objet);
   b.add(m);
   r = popctx(2,ctx);
  }
  else if (op.equals ("repaint"))
  {
   aplet.repaint();
   r = popctx(0,ctx);
  }
  else if (op.equals ("setcolor"))
  {
   expr c = pile.niveau(0);
   /*System.out.println ("Set color:" + c.rep());*/
   graph.setColor (new Color ((int)(c.sousexpr[0].valeur),
				      (int)(c.sousexpr[1].valeur),
				      (int)(c.sousexpr[2].valeur)));
   r = popctx(1,ctx);
  }
  else if (op.equals ("setfont"))
  {
   /*System.out.println ("Font: " + pile.niveau(0).chaine);*/
   Font f = new Font (pile.niveau(0).chaine, Font.PLAIN, (int)(pile.niveau(2).valeur));
   graph.setFont (f);
   r = popctx(3,ctx);
  }
  else if (op.equals ("drawstring"))
  {
   expr coord = pile.niveau(1);
   /*System.out.println ("Coord=" + coord.rep());*/
   String s = pile.niveau(0).chaine;
   graph.drawString (s, (int)(coord.sousexpr[0].valeur),
				(int)(coord.sousexpr[1].valeur));
   r = popctx (2, ctx);
  }
  else if (op.equals ("drawline"))
  {
   expr a = pile.niveau(0);
   expr b = pile.niveau(1);
   graph.drawLine ((int)(a.sousexpr[0].valeur),
			 (int)(a.sousexpr[1].valeur),
			 (int)(b.sousexpr[0].valeur),
			 (int)(b.sousexpr[1].valeur));
   r = popctx(2,ctx);
  }
  else if (op.equals ("fillrect"))
  {
   expr a = pile.niveau(0);
   expr b = pile.niveau(1);
   graph.fillRect ((int)(a.sousexpr[0].valeur),
			 (int)(a.sousexpr[1].valeur),
			 (int)(b.sousexpr[0].valeur),
			 (int)(b.sousexpr[1].valeur));
   r = popctx(2,ctx);
  }
  else if (op.equals ("filloval"))
  {
   expr a = pile.niveau(0);
   expr b = pile.niveau(1);
   /*System.out.println ("filloval: " + a.rep() + ", " + b.rep());*/
   graph.fillOval ((int)(a.sousexpr[0].valeur),
			 (int)(a.sousexpr[1].valeur),
			 (int)(b.sousexpr[0].valeur),
			 (int)(b.sousexpr[1].valeur));
   r = popctx(2,ctx);
  }
  else if (op.equals ("getparameter"))
  {
   String s = aplet.getParameter (pile.niveau(0).chaine);
   r = pushctx (new expr("STR", s), popctx(1,ctx));
  }
  else if (op.equals ("showdocument"))
  {
   String s = pile.niveau(0).chaine;
   try
   {
    URL url = new URL (aplet.getCodeBase(), s);
    AppletContext c = aplet.getAppletContext();
    c.showDocument(url);
   }
   catch (Exception e)
   {
   }
   r = popctx(1,ctx);
  }
  else if (op.equals ("socket"))
  {
   try 
   {
    Socket s = new Socket (pile.niveau(0).chaine, (int)(pile.niveau(1).valeur));
    sock = s;
    System.out.print ("Socket: ");
    System.out.println (s);
    expr x = new expr((Object)s);
    r = pushctx (x, popctx(2,ctx));
   }
   catch (Exception e)
   {
    r = pushctx (new expr ("STR", e.toString()+" host=" + pile.niveau(0).chaine + " port=" + (int)(pile.niveau(1).valeur)), popctx(2,ctx));
   }
  }
  else if (op.equals ("socketoutstream"))
  {
   try 
   {
    Socket s = new Socket (pile.niveau(0).chaine, (int)(pile.niveau(1).valeur));
    sock = s;
    System.out.print ("Socket: ");
    System.out.println (s);
    DataOutputStream out = new DataOutputStream (s.getOutputStream());
    expr x = new expr((Object)out);
    r = pushctx (x, popctx(2,ctx));
   }
   catch (Exception e)
   {
    r = pushctx (new expr ("STR", e.toString()), popctx(2,ctx));
   }
  }
  else if (op.equals ("datainputstream"))
  {
   try
   {
    Socket s = (Socket)(pile.niveau(0).objet);
    DataInputStream dis = new DataInputStream (s.getInputStream());
    r = pushctx (new expr((Object)dis), popctx(1,ctx));
   }
   catch (Exception e)
   {
    r = pushctx (new expr ("STR", e.toString()), popctx(1,ctx));
   }
  }
  else if (op.equals ("readline"))
  {
   try 
   {
    DataInputStream dis = (DataInputStream) (pile.niveau(0).objet);
    String s = dis.readLine();
    r = pushctx (new expr("STR",s), popctx(1,ctx));
   }
   catch (Exception e)
   { 
    r = pushctx (new expr("STR",e.toString()), popctx(1,ctx));
   }
  }
  else if (op.equals ("readbyte"))
  {
   Object o = pile.niveau(0).objet;
   try 
   {
    if (o instanceof DataInputStream)
    {
     DataInputStream dis = (DataInputStream) o;
     byte[] b = new byte[1];
     dis.read(b);
     double x = b[0];
     r = pushctx (new expr(x), popctx(1,ctx));
    }
    else if (o instanceof RandomAccessFile)
    {
     RandomAccessFile f = (RandomAccessFile) o;
     double x = (double) f.readByte();
     r = pushctx (new expr(x), popctx(1,ctx));
    }
    else
    {
     r = pushctx (new expr("STR","Invalid input stream"), popctx(1,ctx));
    }
   }
   catch (Exception e)
   { 
    r = pushctx (new expr("STR",e.toString()), popctx(1,ctx));
   }
  }
  else if (op.equals ("nullstring"))
  {
   expr x = new expr ("STR", "");
   x.chaine = null;
   r = pushctx (x, popctx(0,ctx));
  }
  else if (op.equals ("serversocket"))
  {
   try
   {
    ServerSocket s = new ServerSocket ((int)(pile.niveau(0).valeur));
    r = pushctx (new expr((Object)s), popctx(1,ctx));
   }
   catch (Exception e)
   { 
    r = pushctx (new expr("STR",e.toString()), popctx(1,ctx));
   }
  }
  else if (op.equals ("accept"))
  {
   try
   {
    ServerSocket s = (ServerSocket) (pile.niveau(0).objet);
    Socket c = s.accept();
    r = pushctx (new expr((Object)c), popctx(1,ctx));
   } 
   catch (Exception e)
   { 
    r = pushctx (new expr("STR",e.toString()), popctx(1,ctx));
   }  
  }
  else if (op.equals ("dataoutputstream"))
  {
   try
   {
    Socket s = (Socket) (pile.niveau(0).objet);
    DataOutputStream dos = new DataOutputStream (s.getOutputStream());
    r = pushctx (new expr ((Object)dos), popctx(1,ctx));
   }
   catch (Exception e)
   {
    r = pushctx (new expr("STR",e.toString()), popctx(1,ctx));
   }
  }
  else if (op.equals ("writebytes"))
  {
   Object o = pile.niveau(0).objet;
   String s = pile.niveau(1).chaine;  
   byte[] data = pile.niveau(1).getdata();
   System.out.println ("writeBytes " + s);
   try
   {
    if (o instanceof DataOutputStream ||o instanceof OutputStream)
    {	
     DataOutputStream dos = (DataOutputStream) o;
     dos.writeBytes(s);
     /*dos.write(data);*/
     dos.flush();
     r = pushctx (new expr((double)1), popctx(2,ctx));
    }
    else if (o instanceof RandomAccessFile)
    {
     RandomAccessFile f = (RandomAccessFile) o;
     f.writeBytes(s); 
     /*f.write(data);*/
     r = pushctx (new expr((double)1), popctx(2,ctx));
    }
    else
    {
    	r = pushctx (new expr("STR","Invalid output stream:"+o), popctx(2,ctx));     
    }      
   }
   catch (Exception e)
   {
    r = pushctx (new expr("STR",e.toString()), popctx(2,ctx));
   }
  }
  else if (op.equals ("write"))
  {
   Object o = pile.niveau(0).objet;
   String s = pile.niveau(1).chaine;  
   byte[] data = pile.niveau(1).getdata();
   System.out.println ("writeBytes " + s);
   try
   {
    if (o instanceof DataOutputStream ||o instanceof OutputStream)
    {	
     DataOutputStream dos = (DataOutputStream) o;
     /*dos.writeBytes(s);*/
     dos.write(data);
     dos.flush();
     r = pushctx (new expr((double)1), popctx(2,ctx));
    }
    else if (o instanceof RandomAccessFile)
    {
     RandomAccessFile f = (RandomAccessFile) o;
     /*f.writeBytes(s); */
     f.write(data);
     r = pushctx (new expr((double)1), popctx(2,ctx));
    }
    else
    {
    	r = pushctx (new expr("STR","Invalid output stream:"+o), popctx(2,ctx));     
    }      
   }
   catch (Exception e)
   {
    r = pushctx (new expr("STR",e.toString()), popctx(2,ctx));
   }
  }
  else if (op.equals ("writesocket"))
  {
   try 
   {
    Socket sk = (Socket) (pile.niveau(0).objet);
    System.out.print ("socket: ");
    System.out.println (sk);
    /*Socket sk = sock;*/
    String str = pile.niveau(1).chaine;
    DataOutputStream out = new DataOutputStream (sk.getOutputStream());
    out.writeBytes (str);
    r = pushctx (new expr((double)1), popctx(2,ctx));
   }
   catch (Exception e)
   {
    r = pushctx (new expr("STR",e.toString()), popctx(2,ctx));
   }
  }
  else if (op.equals ("writeurl"))
  {
   try 
   {
    Socket sk = new Socket (pile.niveau(0).chaine, (int)(pile.niveau(1).valeur));
    String str = pile.niveau(2).chaine;
    DataOutputStream out = new DataOutputStream (sk.getOutputStream());
    out.writeBytes (str);
    sk.close();
    r = pushctx (new expr((double)1), popctx(3,ctx));
   }
   catch (Exception e)
   {
    r = pushctx (new expr("STR",e.toString()), popctx(3,ctx));
   }
  }
  else if (op.equals ("file"))
  {
	try
	{
		RandomAccessFile f = new RandomAccessFile (pile.niveau(1).chaine, pile.niveau(0).chaine);
	      r = pushctx (new expr ((Object)f), popctx(2,ctx));
	}
	catch (Exception e)
	{
		r = pushctx (new expr("STR",e.toString()), popctx(2,ctx));
	}
  }
  else if (op.equals ("readall"))
  {
   try 
   {
    RandomAccessFile f = (RandomAccessFile) (pile.niveau(0).objet);
    byte b;
    int n = 0;
    Vector datavect = new Vector();
    try
    {
     while (true)
     {
      b = f.readByte();
      datavect.addElement (new Byte(b));
	n++;
     }
    } 
    catch (EOFException e) { }
    byte[] data = new byte[n];
    for (int k=0; k<n; k++)
	data[k] = ((Byte)(datavect.elementAt(k))).byteValue();
    /*String s = new String(data,"ISO-8859-1");*/
    r = pushctx (new expr("STR",data), popctx(1,ctx));
    r = pushctx (new expr("STR",""), r);

   }
   catch (Exception e)
   { 
    r = pushctx (new expr("STR",e.toString()), popctx(1,ctx));
   }
   
  }
/*
  else if (op.equals ("freadbyte"))
  {
   try 
   {
    RandomAccessFile f = (RandomAccessFile) (pile.niveau(0).objet);
    double x = (double) f.read();
    r = pushctx (new expr(x), popctx(1,ctx));
    r = pushctx (new expr("STR",""), r);
   }
   catch (Exception e)
   { 
    r = pushctx (new expr("STR",e.toString()), popctx(1,ctx));
   }
  }
*/
  else if (op.equals ("mailmsg"))
  {
   try
   {
    MailMsg m = new MailMsg();
    m.smtpServer = pile.niveau(0).chaine;
    m.from = pile.niveau(1).chaine;
    m.subject = pile.niveau(2).chaine;

    r = pushctx (new expr ((Object)m), popctx(3,ctx));
   }
   catch (Exception e)
   {
    r = pushctx (new expr("STR",e.toString()), popctx(3,ctx));
   }
  }
  else if (op.equals ("addrecipient"))
  {
   try
   {
    MailMsg m = (MailMsg) (pile.niveau(0).objet);
    m.addRecipient (pile.niveau(1).chaine);
    r = pushctx (new expr("STR",""), popctx (2, ctx));
   }
   catch (Exception e)
   {
    r = pushctx (new expr("STR",e.toString()), popctx(2,ctx));
   }
  }
  else if (op.equals ("sendmail"))
  {
   try
   {
    MailMsg m = (MailMsg) (pile.niveau(0).objet);
    m.mail();
    r = pushctx (new expr("STR",""), popctx (1, ctx));
   }
   catch (Exception e)
   {
    r = pushctx (new expr("STR",e.toString()), popctx(1,ctx));
   }
  }
  else if (op.equals ("addpart"))
  {
   try
   {
    MailMsg m = (MailMsg) (pile.niveau(0).objet);
    MailMsgPart p = new MailMsgPart();
    if (!pile.niveau(1).chaine.equals(""))
	p.ContentType = pile.niveau(1).chaine;
    if (!pile.niveau(2).chaine.equals(""))
	p.ContentSubType = pile.niveau(2).chaine;
    if (!pile.niveau(3).chaine.equals(""))
	p.FileName = pile.niveau(3).chaine;
    if (!pile.niveau(4).chaine.equals(""))
	p.Name = pile.niveau(4).chaine;
    if (pile.niveau(5).chaine.equals("text"))
    {
	p.setData (pile.niveau(6).chaine, MimeEncoder.QUOTED);
	m.addPart(p);
      r = pushctx (new expr("STR",""), popctx (7, ctx));
    }
    else if (pile.niveau(5).chaine.equals("binary"))
    {
      /*p.setBinaryData (pile.niveau(6).chaine.getBytes("ISO-8859-1"));*/
	p.setBinaryData (pile.niveau(6).getdata());
	m.addPart(p);
      r = pushctx (new expr("STR",""), popctx (7, ctx));
    }
    else
    { 
      r = pushctx (new expr("STR","Bad format: "+pile.niveau(5).chaine), popctx (7, ctx));	
    }
   }
   catch (Exception e)
   {
    r = pushctx (new expr("STR",e.toString()), popctx(7,ctx));
   }
   
  }

  else if (op.equals ("test1"))
  {
  try
  {
   String url = pile.niveau(0).chaine;
   int port = (int)(pile.niveau(1).valeur);
   String text = pile.niveau(2).chaine;
   Socket sk = new Socket (url, port);
   sock = sk;
/*
   expr x = new expr((Object)sk);
   r = pushctx (x, popctx(3,ctx));

   return r;
  }
  catch (Exception e)
  {
   System.out.println (e.toString());
   r = pushctx (new expr("STR",e.toString()), popctx(3,ctx));
  }
 }
 else if (op.equals ("test2"))
 {
   String text = pile.niveau(2).chaine;

  try
  {
   Socket sk = sock;
*/
   DataOutputStream out = new DataOutputStream (sk.getOutputStream());
   out.writeBytes (text);
   /*s.close();*/
   r = pushctx(new expr((double)1),popctx(3,ctx));
  }
  catch (Exception e)
  {
   System.out.println (e.toString());
   r = pushctx (new expr("STR",e.toString()), popctx(3,ctx));
  }

  }
  else if (op.equals ("close"))
  {
   Object o = pile.niveau(0).objet;
   try
   {
    if (o instanceof ServerSocket)
    {
     ServerSocket s = (ServerSocket) o;
     s.close();
    }
    else if (o instanceof Socket)
    {
     Socket s = (Socket) o;
     s.close();
    }
    else if (o instanceof RandomAccessFile)
    {
	RandomAccessFile f = (RandomAccessFile) o;
	f.close();
    }
    r = pushctx (new expr("STR",""), popctx(1,ctx));
   }
   catch (Exception e)
   {
    r = pushctx (new expr("STR",e.toString()), popctx(1,ctx));
   }

  }
  else if (op.equals ("readweb"))
  {
   String url = pile.niveau(0).chaine;
   int port = (int)(pile.niveau(1).valeur);
   String path = pile.niveau(2).chaine;
   String page = "";
   expr x;

   try
   {
    Socket s = new Socket (url, port);
    DataOutputStream out = new DataOutputStream (s.getOutputStream());
    DataInputStream in = new DataInputStream (s.getInputStream());
    String line;
    out.writeBytes ("GET /new/debat/debats.php3\n");
    while ((line = in.readLine()) != null)
    {
     /*System.out.println ("read: " + line);*/
     page += line + "\n";
    }
    s.close();
    x = new expr("STR",page);
    x.valeur = 1;
    r = pushctx (x, popctx(3,ctx));
   }
   catch (Exception e)
   {
    /*System.out.println (e.toString());*/
    x = new expr("STR",e.toString());
    x.valeur = 0;
    r = pushctx (x, popctx(3,ctx));
   }
  }
  else if (op.equals ("ACTION_EVENT"))
   r = pushctx (new expr ((double) Event.ACTION_EVENT), popctx(0,ctx));
  else if (op.equals ("MOUSE_DOWN"))
   r = pushctx (new expr ((double) Event.MOUSE_DOWN), popctx(0,ctx));
  else if (op.equals ("MOUSE_MOVE"))
   r = pushctx (new expr ((double) Event.MOUSE_MOVE), popctx(0,ctx));

  else if (op.equals ("parsenbr"))
  {
   Double x = new Double (pile.niveau(0).chaine);
   double y = x.doubleValue();
   r = pushctx (new expr(y), popctx(1,ctx));
  }
  else if (op.equals ("repnbr"))
  {
   String s = "" + pile.niveau(0).valeur;
   r = pushctx (new expr("STR",s), popctx(1,ctx));
  }
  else if (op.equals ("div"))
  {
   double x = pile.niveau(0).valeur;
   double y = pile.niveau(1).valeur;
   double z = x / y;
   r = pushctx (new expr (z), popctx(2,ctx));
  }  
  else if (op.equals ("sin"))
  {
   r = pushctx (new expr (Math.sin (pile.niveau(0).valeur)), 
	popctx(1,ctx));
  }
  else if (op.equals ("cos"))
  {
   r = pushctx (new expr (Math.cos (pile.niveau(0).valeur)), 
	popctx(1,ctx));
  }
  else if (op.equals ("exp"))
  {
   /*System.out.println ("op=exp");*/
   r = pushctx (new expr (Math.exp (pile.niveau(0).valeur)), 
	popctx(1,ctx));
   /*System.out.println ("r="+r.rep());*/
  }
  else if (op.equals ("log"))
  {
   r = pushctx (new expr (Math.log (pile.niveau(0).valeur)), 
	popctx(1,ctx));
  }
  else if (instr.arite > 0)
  {
   expr x = new expr (instr.noeud);
   for (int i=0; i<instr.arite; i++)
   {
    x = new expr ("SEQ", x, new expr ("SEQ", instr.sousexpr[i], new expr("adse")));
    r = new expr ("CTX", new expr ("SEQ", x, cont), pile, args, env);
   }
  }
  else
  {
    r = pushctx (instr, popctx(0,ctx));
/*
    System.out.println ("undefined instruction: " + op);
    r = new expr ("CTX", cont, pile, args, env);
*/
  }
  
  return r;
  
 }
 
 expr run (expr prog)
 {
  if (prog == null)
	return new expr ((double)0);
  /*System.out.println ("Run prog " + prog.rep() + " without arg");*/
  expr ctx = new expr ("CTX", new expr ("SEQ", prog, new expr("HALT")), 
	new expr("EMPTY"), new expr("EMPTY"), new expr("EMPTY"));
  if (trace != null)
   trace.setText ("Running...");


  for (;;)
  {
   /*try
   {*/
    /*trace.setText (ctx.rep());*/
    ctx = evol (ctx);
   /*}
   catch (Exception e)
   {
    System.out.println (e.toString());
    trace.setText(e.toString());
   }*/
   /*System.out.println (ctx.rep());*/
   if (ctx.sousexpr[0] == null)
   {
    trace.setText ("Error (1)");
    break;
   }
   if (ctx.sousexpr[0].noeud.equals ("HALT"))
    break;
   if (ctx.sousexpr[0].sousexpr[0] == null)
   {
    trace.setText ("Error (2)");
    break;
   }
   if (ctx.sousexpr[0].sousexpr[0].noeud.equals("HALT"))
     break;
   
  }
  if (ctx.sousexpr[1] != null)
  if (ctx.sousexpr[1].arite > 0)
   return ctx.sousexpr[1].sousexpr[0];

  return new expr ((double)0);

 }


 void rung (expr prog, expr g)
 {
  global = g;
  run (prog);  
 }

 expr run (expr prog, expr x)
 {
  if (prog == null)
	return new expr ((double)0);
  /*System.out.println ("Run prog " + prog.rep() + " with " + x.rep());*/
  expr ctx = new expr ("CTX", new expr ("SEQ", prog, new expr("HALT")), 
	new expr ("PUSH", x, new expr("EMPTY")), new expr("EMPTY"), new expr("EMPTY"));
  for (;;)
  {
   /*try
   {*/
    ctx = evol (ctx);
   /*}
   catch (Exception e)
   {
    trace.setText(e.toString());
   }*/
   if (ctx.sousexpr[0].noeud.equals ("HALT"))
    break;
   if (ctx.sousexpr[0].sousexpr[0].noeud.equals("HALT"))
    break;
  }
  /*System.out.println ("Last ctx: " + ctx.rep());*/
  if (ctx.sousexpr[1] != null)
  if (ctx.sousexpr[1].arite > 0)
   return ctx.sousexpr[1].sousexpr[0];

  return new expr ((double)0);
 }

 expr runctx (expr ctx1)
 {
  expr ctx = ctx1;
  for (;;)
  {
   ctx = evol (ctx);
   if (ctx.sousexpr[0].noeud.equals ("HALT"))
    break;
   if (ctx.sousexpr[0].sousexpr[0].noeud.equals("HALT"))
    break;
  }
  return ctx.sousexpr[1].sousexpr[0];
 }

}
