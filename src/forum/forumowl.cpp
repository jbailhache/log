// ObjectWindows - (C) Copyright 1992 by Borland International

#include <owl.h>
#include <dialog.h>
#include <edit.h>
#include <checkbox.h>
#include <listbox.h>
#include <editwnd.h>

#include <stdio.h>

#include <time.h>
/* #include "calcul.h" 

#define OP_Ajouter 0
#define OP_Multiplier 1
*/

#include "forumrc.h"
#include "forum.h"

sleep (int n)
{

}

class TTestDialog : public TDialog
{
public:
  TTestDialog(PTWindowsObject AParent, LPSTR AName);
/*    : TDialog(AParent, AName){}; */
  virtual void DisplayNode ();
  virtual void Clear ();
  virtual void OpenText (int flag, FILE *fh);
  virtual void Handle_Rechercher (RTMessage Msg)
	= [ID_FIRST + ID_Rechercher];
  virtual void Handle_Bouton_Superieur (RTMessage Msg)
	= [ID_FIRST + ID_Bouton_Superieur];

  virtual void Handle_Bouton_Precedent (RTMessage Msg)
	= [ID_FIRST + ID_Bouton_Precedent];
  virtual void Handle_Bouton_Suivant (RTMessage Msg)
  	= [ID_FIRST + ID_Bouton_Suivant];
  virtual void Handle_Bouton_1ereReponse (RTMessage Msg)
	= [ID_FIRST + ID_Bouton_1ereReponse];
  virtual void Handle_Bouton_DerniereReponse (RTMessage Msg)
  	= [ID_FIRST + ID_Bouton_DerniereReponse];

  virtual void Handle_Texte (RTMessage Msg)
	= [ID_FIRST + ID_Texte];
  virtual void Handle_Ajouter (RTMessage Msg)
  	= [ID_FIRST + ID_Ajouter];
  virtual void Handle_Selection (RTMessage Msg)
  	= [ID_FIRST + ID_Selection];
  virtual void Handle_Reponses (RTMessage Msg)
	= [ID_FIRST + ID_Reponses];
  virtual void SetNode (noeud n);
  PTEdit Edit_MotCle, Edit_Auteur, Edit_Numero, Edit_Titre, Edit_Date,
	Edit_Superieur, Edit_Precedent, Edit_Suivant,
	Edit_1ereReponse, Edit_DerniereReponse,
	Edit_Reponses, Edit_MotsCles, Edit_Correles;
  PTListBox ListBox_Selection, ListBox_Reponses, ListBox_MotsCles;
  PTEditWindow EditWindow_Texte;
  noeud Node;
  struct s_noeud sn;
  char MotCle [300], Auteur [300], Pseudo [300];
  int NSelectedNodes, NReponses;
  int FlagAjouter;
  FILE *f;
  noeud SelectedNode[1000], Reponse[1000];
/*
  virtual void HandleButtonMsg(RTMessage Msg)
    = [ID_FIRST + ID_BUTTON];
  virtual void HandleListBoxMsg(RTMessage Msg)
    = [ID_FIRST + ID_LISTBOX];
*/
/*
  virtual void Handle_PremierNombre (RTMessage Msg)
	= [ID_FIRST + ID_PremierNombre];
  virtual void Handle_DeuxiemeNombre (RTMessage Msg)
  	= [ID_FIRST + ID_DeuxiemeNombre];
  virtual void Handle_Calculer (RTMessage Msg)
	= [ID_FIRST + ID_Calculer];
  virtual void Handle_Hexa (RTMessage Msg)
	= [ID_FIRST + ID_Hexa];
  virtual void Handle_Ajouter (RTMessage Msg)
	= [ID_FIRST + ID_Addition];
  virtual void Handle_Multiplier (RTMessage Msg)
	= [ID_FIRST + ID_Multiplication];
  PTEdit Edit_PremierNombre;
  int PremierNombre;
  PTEdit Edit_DeuxiemeNombre;
  int DeuxiemeNombre;
  PTCheckBox CheckBox_Hexa;
  int Hexa;
  int op;
  PTEdit Edit_Resultat;
  int Resultat;
*/

};

TTestDialog::TTestDialog (PTWindowsObject parent, LPSTR nom_res) :
	TDialog (parent, nom_res)
{
/*	ListBox_Selection = new TListBox (this, ID_Selection, 127, -1, 120, 40);
	ListBox_Selection->AddString (" ");
	ListBox_Selection->SetSelIndex (0);
*/
	EnableKBHandler ();
/*    Dialog = this;*/
	Edit_MotCle = new TEdit (this, ID_MotCle, 60);
	Edit_Auteur = new TEdit (this, ID_Auteur, 60);
	Edit_Numero = new TEdit (this, ID_Numero, 15);
	Edit_Titre = new TEdit (this, ID_Titre, 120);
	Edit_Date = new TEdit (this, ID_Date, 30);
	Edit_Superieur = new TEdit (this, ID_Edit_Superieur, 150);
	Edit_Precedent = new TEdit (this, ID_Edit_Precedent, 150);
	Edit_Suivant = new TEdit (this, ID_Edit_Suivant, 150);
	Edit_1ereReponse = new TEdit (this, ID_Edit_1ereReponse, 150);
	Edit_DerniereReponse = new TEdit (this, ID_Edit_DerniereReponse, 150);
/*	CONTROL "&Sélection", ID_Selection, "LISTBOX", LBS_NOTIFY | WS_CHILD | WS_VISIBLE | WS_BORDER | WS_VSCROLL, 127, -1, 120, 40
*/
	ListBox_Selection = new TListBox (this, ID_Selection/*, 2*127, 1, 2*120, 38*/);
	ListBox_Selection->Attr.Style &= ~LBS_SORT;

    ListBox_Reponses = new TListBox (this, ID_Reponses);
	ListBox_Reponses->Attr.Style &= ~LBS_SORT;

/*    ListBox_MotsCles = new TListBox (this, ID_MotsCles); */
	Edit_MotsCles = new TEdit (this, ID_MotsCles, 1000);
	Edit_Correles = new TEdit (this, ID_Correles, 1000);   

/*
	Edit_PremierNombre = new TEdit (this, ID_PremierNombre, 20);
	Edit_DeuxiemeNombre = new TEdit (this, ID_DeuxiemeNombre, 20);
	Edit_Resultat = new TEdit (this, ID_Resultat, 20);
	CheckBox_Hexa = new TCheckBox (this, ID_Hexa, NULL);
*/
	strcpy (Pseudo, "anonyme");
    FlagAjouter = 0;
    SetNode (0);
}

void TTestDialog::Clear ()
{
	Edit_Numero->SetText ("");
	Edit_Titre->SetText ("");
	Edit_Auteur->SetText ("");
	Edit_Date->SetText ("");
	Edit_Superieur->SetText ("");
	Edit_Precedent->SetText ("");
	Edit_Suivant->SetText ("");
	Edit_1ereReponse->SetText ("");
    Edit_DerniereReponse->SetText ("");

}

void TTestDialog::DisplayNode ()
{
char buf[1000];
struct s_noeud sn_sup, sn_crl, sn_rep, sn1;
int i;
noeud nr;

						sprintf (buf, "%d", Node);
						Edit_Numero->SetText (buf);

						Edit_Titre->SetText (sn.titre);
						Edit_Auteur->SetText (sn.auteur);
						strcpy (buf, my_asctime (localtime (&sn.date)));
						buf[strlen(buf)-1] = 0;
						Edit_Date->SetText (buf);

						acces_noeud (fread, sn.sup, &sn_sup);
						saf_s_noeud (buf, sn.sup, &sn_sup);
						Edit_Superieur->SetText (buf);

						acces_noeud (fread, sn.prec, &sn1);
						saf_s_noeud (buf, sn.prec, &sn1);
						Edit_Precedent->SetText (buf);

						acces_noeud (fread, sn.suiv, &sn1);
						saf_s_noeud (buf, sn.suiv, &sn1);
						Edit_Suivant->SetText (buf);

						acces_noeud (fread, sn.prem, &sn1);
						saf_s_noeud (buf, sn.prem, &sn1);
						Edit_1ereReponse->SetText (buf);

						acces_noeud (fread, sn.dern, &sn1);
						saf_s_noeud (buf, sn.dern, &sn1);
						Edit_DerniereReponse->SetText (buf);

						/*Edit_Reponses->SetText ("");*/
						ListBox_Reponses->ClearList ();
						NReponses = 0;
						nr = sn.prem;
						while (nr != NIL)
						{
							/*
							sprintf (buf, "nr = %d\n", nr);
							MessageBox (HWindow, buf, "message", MB_OK);
							*/
							acces_noeud (fread, nr, &sn1);
							saf_s_noeud (buf, nr, &sn1);
							/*if (nr != sn.prem)
								Edit_Reponses->Insert ("\r\n");
							Edit_Reponses->Insert (buf); */
							ListBox_Reponses->AddString (buf);
                            Reponse[NReponses++] = nr;
							nr = sn1.suiv;
                        }

						/* ListBox_MotsCles->ClearList (); */
                        Edit_MotsCles->SetText ("");
						for (i=0; i<sn.n_mots_cles; i++)
							/*ListBox_MotsCles->AddString (sn.mot_cle[i]);*/
						{
                        	if (i > 0)
								Edit_MotsCles->Insert ("\r\n");
							Edit_MotsCles->Insert (sn.mot_cle[i]);
						}

						Edit_Correles->SetText ("");
						for (i=0; i<sn.n_correles; i++)
						{
							if (i > 0)
								Edit_Correles->Insert ("\r\n");
                            acces_noeud (fread, sn.correle[i], &sn_crl);
							saf_s_noeud (buf, sn.correle[i], &sn_crl);
							Edit_Correles->Insert (buf);

						}



}

void TTestDialog::Handle_Rechercher (RTMessage Msg)
{
char buf [1000];
struct entete e;
struct s_noeud /*sn,*/ sn_sup;
noeud n;
int mc;
int flag;
					printf ("Rechercher\n");
 
 					/*
					GetDlgItemText (hWndDlg,
						ID_MotCle,
						MotCle, sizeof(MotCle)-1);
					*/
					Edit_Numero->GetText (buf, sizeof(buf)-1);
					printf ("Numero : <%s>\n", buf);
				   if (*buf)
				   {
						n = atoi (buf);
                        SetNode (n);
						/*
						Node = n;

						acces_noeud (fread, n, &sn);
						saf_s_noeud (buf, n, &sn);
						printf ("*** %s ***\n", buf);

						DisplayNode ();
                        */
/*
						Edit_Titre->SetText (sn.titre);
						Edit_Auteur->SetText (sn.auteur);
						strcpy (buf, my_asctime (localtime (&sn.date)));
						buf[strlen(buf)-1] = 0;
						Edit_Date->SetText (buf);

						acces_noeud (fread, sn.sup, &sn_sup);
						saf_s_noeud (buf, sn.sup, &sn_sup);
						Edit_Superieur->SetText (buf);
*/
                        

				   }
				   else
				   {
				   Edit_Titre->GetText (buf, sizeof(buf)-1);
				   if (*buf)
				   {
					SetNode (chercher_titre (buf));
				   }
				   else
                   {
					Edit_MotCle->GetText (MotCle, sizeof(MotCle)-1);
					/*	
					GetDlgItemText (hWndDlg,
						ID_Auteur,
						Auteur, sizeof(Auteur)-1);
					*/
					Edit_Auteur->GetText (Auteur, sizeof(Auteur)-1);

					printf ("\tMot-clé: %s\n\tAuteur: %s\n", MotCle, Auteur);

					acces_entete (fread, &e);

					ListBox_Selection->ClearList ();
					NSelectedNodes = 0;

					for (n=0; n<e.n_noeuds; n++)
					{
						acces_noeud (fread, n, &sn);
						flag = 0;
						if (!*MotCle)
							flag = 1;
                        else
						for (mc=0; mc<sn.n_mots_cles; mc++)
							if (!my_strncmp (MotCle, sn.mot_cle[mc], 0x7FFF))
							{
                            	flag = 1;
							}
					
						if (flag && (!*Auteur || !my_strncmp (Auteur, sn.auteur, 0x7FFF)))
						{
						/* Noeud selectionne */
                            SelectedNode[NSelectedNodes++] = n;
							saf_s_noeud (buf, n, &sn);
							printf ("Selection <%s>\n", buf);
							/*
							if (hSelection == NULL)
							{
                            	printf ("Creation liste\n");
								hSelection = CreateWindow ("LISTBOX", NULL,
									LBS_STANDARD | WS_CHILD | WS_VISIBLE,
									0, 0, 0, 0, hWndDlg, ID_Selection, hInst, NULL);
                            }
							SendMessage (hSelection, LB_ADDSTRING, 0,
								(LONG)buf);
							*/
							ListBox_Selection->AddString (buf);
                                               
						}
					}                         	
					ListBox_Selection->SetSelIndex (0);
					EnableKBHandler ();
					SetNode (SelectedNode [ListBox_Selection->GetSelIndex()]);

				   }
                   }

}

class TEditWindowTexte : public TEditWindow
{
public:
	int FlagAjouter;
	FILE *f;
	TEditWindowTexte (PTWindowsObject parent, LPSTR title,
		int flag, FILE *fh, PTModule module=NULL) :
		TEditWindow (parent, title, module)
	{
		FlagAjouter = flag;
        f = fh;
	};
	virtual BOOL CanClose ();
};

/*
TEdit::CMEditDelete (RTMessage Msg)
{
}
*/

TEditWindowTexte::CanClose ()
{
char buf[1000];
int i;
int status;
/*	Editor->GetText (buf, sizeof(buf)-1);
	printf ("Texte editeur : ");
	for (i=0; i<strlen(buf); i++)
		printf ("%02X ", buf[i]);
	printf (".\n");
*/
	printf ("CanClose\n");
	if (FlagAjouter)
	{
    	printf ("FlagAjouter\n");
		for (i=0; i<Editor->GetNumLines(); i++)
		{
			status = Editor->GetLine (buf, sizeof(buf), i);
			if (status)
				fprintf (f, "%s\r\n", buf);
			printf ("i=%d: GetLine -> %d, ligne = <%s>\n",
				i, status, buf);
		}
		status = fclose (f);
		printf ("close -> %d\n", status);

    }
	return 1;
}

void TTestDialog::Handle_Ajouter (RTMessage Msg)
{
char buf [1000];
char filename [200];
/* FILE *f; */
int i, status;

	/* Clear (); */
	my_strncpy (sn.auteur, Pseudo, sizeof(sn.auteur));

    Edit_Titre->GetText (buf, sizeof(buf)-1);
	my_strncpy (sn.titre, buf, sizeof(sn.titre));

	time (&sn.date);
	
    sn.n_mots_cles = 0;
	for (i=0; i<Edit_MotsCles->GetNumLines(); i++)
	{
		if (sn.n_mots_cles >= NMC)
			break;
		status = Edit_MotsCles->GetLine (sn.mot_cle[sn.n_mots_cles], sizeof(sn.mot_cle[i])-1, i);
		if (status)
			sn.n_mots_cles++;        
        
        	
	}

	sn.n_correles = 0;
/* #if 0 */
	for (i=0; i<Edit_Correles->GetNumLines(); i++)
	{
		status = Edit_Correles->GetLine (buf, sizeof(buf)-1, i);
		if (status)
		{
			if (sn.n_correles >= NCORRELES)
            	break;
        	status = sscanf (buf, "%d", &sn.correle[sn.n_correles]);
			if (status)
				sn.n_correles++;
		}
	}
/* #endif */

	sprintf (filename, "%08lX.MES", sn.date);
	f = fopen (filename, "wb");
	if (f == NULL)
	{
		sprintf (buf, "Error %d opening text file");
		MessageBox (HWindow, buf, "Error", MB_OK);
	}
	else
	{
		printf ("Fichier <%s> ouvert en écriture\n", sn.fichier);
		my_strncpy (sn.fichier, filename, sizeof(sn.fichier));
        FlagAjouter = 1;
		OpenText (1, f);
		printf ("apres OpenText\n");

        FlagAjouter = 0;
        /* fclose (f); */
	}
	SetNode (aj_noeud (Node, &sn));

}

void TTestDialog::OpenText (int flag, FILE *fh)
{
	EditWindow_Texte = new TEditWindowTexte (this, "Texte", flag, fh);
	EditWindow_Texte->Attr.Style |= WS_POPUPWINDOW | WS_CAPTION | WS_EX_DLGMODALFRAME | WS_SYSMENU;
	EditWindow_Texte->Attr.X = 10;
	EditWindow_Texte->Attr.Y = 120;
	EditWindow_Texte->Attr.W = 600 /*500*/;
	EditWindow_Texte->Attr.H = 200 /*115*/;
	GetApplication()->MakeWindow (EditWindow_Texte);
}

void TTestDialog::Handle_Texte (RTMessage Msg)
{
FILE *f;
char buf [1000];
char *status;
char filename[200];

	/*	f = fopen (sn.fichier, "rb"); */

	if (!strncmp (sn.fichier, "MEMO:", 5))
		sprintf (filename, "C:\\FORUM\\%s", sn.fichier + 5);
	else
    	sprintf (filename, "%s", sn.fichier);

	f = fopen (filename, "rb");

	if (f == NULL)
	{
		sprintf (buf, "Cannot open message text file <%s>", filename);
		MessageBox (HWindow, buf, "Error", MB_OK);
        return;
	}
	printf ("Fichier <%s> ouvert\n", sn.fichier);
	OpenText (0, NULL);
	/* EditWindow_Texte->Editor->Insert ("aaa\rbbb\nccc\r\nddd\n\reee"); */
	for (;;)
	{
		status = fgets (buf, sizeof(buf)-1, f);
        printf ("Texte <%s> lu\n", buf);
		if (status == NULL)
			break;
		EditWindow_Texte->Editor->Insert (buf);
        printf ("Texte <%s> affiché\n", buf);
	}
    fclose (f);

}

void TTestDialog::Handle_Bouton_Superieur (RTMessage Msg)
{
	SetNode (sn.sup);
}

void TTestDialog::Handle_Bouton_Precedent (RTMessage Msg)
{
	SetNode (sn.prec);
}

void TTestDialog::Handle_Bouton_Suivant (RTMessage Msg)
{
	SetNode (sn.suiv);
}

void TTestDialog::Handle_Bouton_1ereReponse (RTMessage Msg)
{
	SetNode (sn.prem);
}

void TTestDialog::Handle_Bouton_DerniereReponse (RTMessage Msg)
{
	SetNode (sn.dern);
}

void TTestDialog::Handle_Selection (RTMessage Msg)
{
	switch (Msg.LP.Hi)
	{
		case LBN_SELCHANGE:
			SetNode (SelectedNode [ListBox_Selection->GetSelIndex()]);
			break;
	}

}

void TTestDialog::Handle_Reponses (RTMessage Msg)
{
	switch (Msg.LP.Hi)
	{
		case LBN_DBLCLK:
			SetNode (Reponse[ListBox_Reponses->GetSelIndex()]);
			break;
	}
}

void TTestDialog::SetNode (noeud n)
{
	if (n != NIL)
    {
		Node = n;
		acces_noeud (fread, Node, &sn);
		DisplayNode ();
	}
}

#if 0
void TTestDialog::Handle_PremierNombre (RTMessage Msg)
{
char buf[50];
	Edit_PremierNombre->GetText (buf, sizeof(buf)-1);
	sscanf (buf, Hexa?"%x":"%d", &PremierNombre);
	printf ("Premier nombre tapé = %d.\n", PremierNombre);
}

void TTestDialog::Handle_DeuxiemeNombre (RTMessage Msg)
{
char buf[50];
	Edit_DeuxiemeNombre->GetText (buf, sizeof(buf)-1);
	sscanf (buf, Hexa?"%x":"%d", &DeuxiemeNombre);
	printf ("Deuxieme nombre tapé = %d.\n", DeuxiemeNombre);

}

void TTestDialog::Handle_Ajouter (RTMessage Msg)
{
	printf ("Ajouter\n");
	op = OP_Ajouter;
}

void TTestDialog::Handle_Multiplier (RTMessage Msg)
{
	printf ("Multiplier\n");
	op = OP_Multiplier;
}

void TTestDialog::Handle_Calculer (RTMessage Msg)
{
char buf[50];
	printf ("Calculer\n");
	switch (op)
	{
		case OP_Ajouter:
			Resultat = PremierNombre + DeuxiemeNombre;
			break;
		case OP_Multiplier:
			Resultat = PremierNombre * DeuxiemeNombre;
			break;
	}
	if (Hexa)
		sprintf (buf, "%X", Resultat);
	else
        	sprintf (buf, "%d", Resultat);
	Edit_Resultat->SetText (buf);
}

void TTestDialog::Handle_Hexa (RTMessage Msg)
{
	Hexa = CheckBox_Hexa->GetCheck() == BF_CHECKED;
	Hexa = !Hexa;
	if (Hexa)
		CheckBox_Hexa->SetCheck (BF_CHECKED);
	else
		CheckBox_Hexa->SetCheck (BF_UNCHECKED);
	printf ("Hexa = %d.\n", Hexa);
}

#endif

/*
void TTestDialog::HandleButtonMsg(RTMessage)
{
  SendDlgItemMsg(ID_LISTBOX, LB_ADDSTRING, 0, (LONG)"Item 1");
  SendDlgItemMsg(ID_LISTBOX, LB_ADDSTRING, 0, (LONG)"Item 2");
  SendDlgItemMsg(ID_LISTBOX, LB_ADDSTRING, 0, (LONG)"Item 3");
  SendDlgItemMsg(ID_LISTBOX, LB_ADDSTRING, 0, (LONG)"Item 4");
  SendDlgItemMsg(ID_LISTBOX, LB_ADDSTRING, 0, (LONG)"Item 5");
  SendDlgItemMsg(ID_LISTBOX, LB_ADDSTRING, 0, (LONG)"Item 6");
  SendDlgItemMsg(ID_LISTBOX, LB_ADDSTRING, 0, (LONG)"Item 7");
}

void TTestDialog::HandleListBoxMsg(RTMessage Msg)
{
  DWORD Idx;
  char SelectedText[10];

  if ( Msg.LP.Hi == LBN_SELCHANGE )
  {
    Idx = SendDlgItemMsg(ID_LISTBOX, LB_GETCURSEL, 0, 0L);
    SendDlgItemMsg(ID_LISTBOX, LB_GETTEXT, (WORD)Idx, (DWORD)SelectedText);
    MessageBox(HWindow, SelectedText, "You selected", MB_OK);
  }
}
*/

class TTestWindow : public TWindow
{
public:
  TTestWindow(PTWindowsObject AParent, LPSTR ATitle);
#ifdef MENU
  virtual void CMTest(RTMessage Msg)
    = [CM_FIRST + CM_TEST];
#endif
};

TTestWindow::TTestWindow(PTWindowsObject AParent, LPSTR ATitle)
  : TWindow(AParent, ATitle)
{
  EnableKBHandler ();
#ifdef MENU
  AssignMenu("COMMANDS");
}

void TTestDialog::WMInitDialog (RTMessage Msg)
{
	printf ("Init dialog\n");
	SetNode (0);
}

void TTestDialog::SetupWindow ()
{
	printf ("SetupWindow\n");
	TWindow::SetupWindow ();
	SetNode (0);
}

void TTestWindow::CMTest(RTMessage)
{
#endif
  /*GetApplication()->ExecDialog(new TTestDialog(this, "Forum"));*/
  TTestDialog *dial;
  dial = new TTestDialog (this, "Forum");
  /*
  dial->MakeWindow (); 	non defini
  dial->Create ();      ne marche pas
  dial->SetupWindow (); inaccessible
  */
  dial->SetNode (0);
  GetApplication()->ExecDialog (dial);
#ifndef MENU
exit (0);
#endif
}

class TTestApp : public TApplication
{
public:
  TTestApp(LPSTR AName, HINSTANCE hInstance, HINSTANCE hPrevInstance,
    LPSTR lpCmdLine, int nCmdShow) :
    TApplication(AName, hInstance, hPrevInstance, lpCmdLine, nCmdShow) {};
  virtual void InitMainWindow();
};

void TTestApp::InitMainWindow()
{
  MainWindow = new TTestWindow(NULL, Name);  
/*  GetApplication()->ExecDialog(new TTestDialog(this, "CALCUL")); */
}

open_forum ();

int PASCAL WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
  LPSTR lpCmdLine, int nCmdShow)
{
  TTestApp TestApp("Calcul", hInstance, hPrevInstance,
    lpCmdLine, nCmdShow);
  _InitEasyWin ();
	open_forum ();
/*  GetApplication()->ExecDialog(new TTestDialog(this, "CALCUL")); */
  TestApp.Run();
  return (TestApp.Status);
}
