
#include <stdio.h>
#include <errno.h>

#define P_ID 0
#define P_X 1
#define P_Y 2
#define P_W 3
#define P_H 4
#define P_STYLE 5

#define GEN_DEFINE_API 110
#define GEN_DECL_API 120
#define GEN_HANDLE_API 130
#define GEN_CREATE_API 140
#define GEN_COMMAND_API 150
#define GEN_INIT_RES 160
#define GEN_CREATE_MOB 170
#define GEN_SIZE 180

#define GEN_CREATE_OWL 310

struct resource_item
{
	char name[30];
	char ctlclass[30];
	char caption[300];
	char param[8][120];
	long param_int[8];
	int id, X, Y, W, H, style;
	char source[300];

};

/*
struct object_desc
{
	char objclass[30];
	char caption[300];

};
*/

compile (FILE *in, FILE *out, int obj);

char name[60];

char W[80], H[80];

int max_item = 0;

char config[300];

/*char gen[30];*/

#define COMMAND0 100

analyse_line_a (char *line_in, struct resource_item *res)
{
int i, j, k, l;
	strcpy (res->source, line_in);
	res->ctlclass[0] = 0;
	for (k=0; k<6; k++)
		/* res->param[k][0] = 0; */
		strcpy (res->param[k], "000");
	l = strlen (line_in);
	for (i=0; i<l && line_in[i] == ' ' || line_in[i] == '\t'; i++);
	j = 0;
	for (; i<l && line_in[i] != ' '; i++)
		res->name[j++] = line_in[i];
	res->name[j] = 0;
	for (; i<l && line_in[i] == ' '; i++);
	if (line_in[i] == '"')
	{
		i++;
		j = 0;
		for (; i<l && line_in[i] != '"'; i++)
			res->caption[j++] = line_in[i];
		res->caption[j] = 0;
		for (; i<l && line_in[i]!=','; i++);
	}
	else
	{
		res->caption[0] = 0;
		i--;
	}

	if (!strcmp(res->name,"CONTROL"))
	{
    		i++;
		j = 0;
		for (; i<l && line_in[i]!=','; i++)
			res->param[P_ID][j++] = line_in[i];
		res->param[P_ID][j] = 0;

		i++;
		j = 0;
		for (; i<l && line_in[i]!=','; i++)
			res->ctlclass[j++] = line_in[i];
		res->ctlclass[j] = 0;

		i++;
		j = 0;
		for (; i<l && line_in[i]!=','; i++)
			res->param[P_STYLE][j++] = line_in[i];
		res->param[P_STYLE][j] = 0;

		for (k=P_X; k<=P_H; k++)
		{
			i++;
			if (i>=l)
				break;
			j = 0;
			for (; i<l && line_in[i]!=','; i++)
				res->param[k][j++] = line_in[i];
			res->param[k][j] = 0;

		}

	}
	else
	{
		for (k=0; k<6; k++)
		{
			i++;
			if (i>=l)
				break;
			j = 0;
			for (; i<l && line_in[i]!=','; i++)
				res->param[k][j++] = line_in[i];
			res->param[k][j] = 0;
		}

	}
}

analyse_line_b (struct resource_item *res)
{
int i;
	for (i=0; i<6; i++)
		res->param_int[i] = atoi (res->param[i]);
	if (!strcmp (res->name, "LTEXT"))
		strcpy (res->ctlclass, "\"STATIC\"");
	else if (!strcmp (res->name, "EDITTEXT"))
	{
		strcpy (res->ctlclass, "\"EDIT\"");
		strcat (res->param[P_STYLE], " | ES_AUTOHSCROLL");
	}
	else if (!strcmp (res->name, "PUSHBUTTON"))
	{
		strcpy (res->ctlclass, "\"BUTTON\"");
		strcat (res->param[P_STYLE], " | BS_PUSHBUTTON");
	}
}

analyse_line (char *line_in, struct resource_item *res)
{
	analyse_line_a (line_in, res);
	analyse_line_b (res);
}

generate_line_source (struct resource_item *res, /* char *line_out */ FILE *out, int n)
{
static int n1 = 0;
	n1++;
	fprintf (out, "\n\t/*\n%s\n\t%s (%s) \"%s\" ID=%s X=%s Y=%s W=%s H=%s STYLE=%s\n\t*/\n",
		res->source,
		res->name, res->ctlclass, res->caption,
		res->param[P_ID], res->param[P_X], res->param[P_Y],
		res->param[P_W], res->param[P_H], res->param[P_STYLE]);
}

generate_line_create_OWL (struct resource_item *res, /* char *line_out */ FILE *out, int n)
{
	generate_line_source (res, out, n);
	if (!strcmp (res->name, "LTEXT"))
	{
		fprintf (out, "\tobj%d = new TBStatic (this, %s, \"%s\", %s, %s, %s, %s, 300, 0);\n",
			n, res->param[P_ID], res->caption,
			res->param[P_X], res->param[P_Y], res->param[P_W], res->param[P_H]);
		fprintf (out, "\tobj%d->Attr.Style = %s;\n",
			n, res->param[P_STYLE]);

	}
	else if (!strcmp (res->name, "EDITTEXT"))
	{
	/*
	EDITTEXT ID_KATEG, 44, 40, 55, 14, ES_LEFT | WS_CHILD | WS_VISIBLE | WS_BORDER | WS_TABSTOP
	EDITTEXT () "" ID=ID_KATEG X= 44 Y= 40 W= 55 H= 14 STYLE= ES_LEFT | WS_CHILD | WS_VISIBLE | WS_BORDER | WS_TABSTOP
	*/
		/* p_edit1 = new TEdit (this, 501, "Test", 10, 4, 120, 28, 300, 0, NULL); */
		/* objn = new TEdit (this, ID_KATEG, "", 44, 40, 55, 14, 300, 0);
		   objn->Attr.Style = ES_LEFT ...*/
		fprintf (out, "\tobj%d = new TEdit (this, %s, \"%s\", %s, %s, %s, %s, 300, 0);\n",
			n, res->param[P_ID], res->caption,
			res->param[P_X], res->param[P_Y], res->param[P_W], res->param[P_H]);
		fprintf (out, "\tobj%d->Attr.Style = %s;\n",
			n, res->param[P_STYLE]);




	}

	else if (!strcmp (res->name, "PUSHBUTTON"))
	{
		fprintf (out, "\tobj%d = new TButton (this, %s, \"%s\", %s, %s, %s, %s, 0);\n",
			n, res->param[P_ID], res->caption,
			res->param[P_X], res->param[P_Y], res->param[P_W], res->param[P_H]);
		fprintf (out, "\tobj%d->Attr.Style = %s;\n",
			n, res->param[P_STYLE]);


	}

	else
	{
		fprintf (out, "\t/* Unknown resource %s */\n", res->name);
    }

}

int is_letter (char c)
{
	if (c >= 'A' && c <= 'Z')
		return 1;
	if (c >= 'a' && c <= 'z')
		return 1;
	return 0;
}

generate_line_define_API (struct resource_item *res, FILE *out, int n)
{
char *s;
	s = res->param[P_ID];
	while (*s == ' ')
		s++;
	/* fprintf (out, "#define ID_CTLOBJ%d %d\n", n, 100+n); */
	if (is_letter(*s))
	{
		fprintf (out, "#define %-20.20s (COMMAND0 + %d)\n",
			res->param[P_ID], n);
	}

}

generate_handle (struct resource_item *res, FILE *out, int n)
{
char *s;
	s = res->param[P_ID];
	while (*s == ' ')
		s++;
 	if (is_letter(*s))
		fprintf (out, "HandleCtl%s%s", name, s);
	else
		fprintf (out, "HandleCtl%s%d", name, n);
}
 
generate_var (struct resource_item *res, FILE *out, int n)
{
	/* if (atoi (res->param[P_ID])) */
		fprintf (out, "hCtlObj%s[%d]", name, n);

}

generate_line_decl_API (struct resource_item *res, FILE *out, int n)
{
	/* fprintf (out, "HWND hCtlObj%d;\n", n); */
	fprintf (out, "HWND ");
	generate_var (res, out, n);
	fprintf (out, ";\n");
}

generate_line_handle_API (struct resource_item *res, FILE *out, int n)
{
	generate_line_source (res, out, n);
/*	fprintf (out, "\nvoid HandleCtl%s%d ()\n", name, n); */
	fprintf (out, "void ");
	generate_handle (res, out, n);
	fprintf (out, " (TMessage *pmsg)\n");
	fprintf (out, "{\n");
	fprintf (out, "/* Add code here */\n\n");
	fprintf (out, "}\n");

}

generate_line_create_API (struct resource_item *res, FILE *out, int n)
{
	generate_line_source (res, out, n);

	fprintf (out, "\t\t\thCtlObj%s[%d] = CreateWindow (%s, \"%s\",\n",
		name, n, res->ctlclass, res->caption);
	fprintf (out, "\t\t\t\t%s,\n", res->param[P_STYLE]);
	fprintf (out, "\t\t\t\t%s*2, %s*3/2, %s*2, %s*3/2,\n",
		res->param[P_X], res->param[P_Y], res->param[P_W], res->param[P_H]);
	fprintf (out, "\t\t\t\thWnd, COMMAND0+%d, hInst, NULL);\n", n);

}

generate_line_create_mob (struct resource_item *res, FILE *out, int n)
{
	generate_line_source (res, out, n);

	fprintf (out, "\t\t\thCtlObj%s[%d] = CreateWindow (Res%s[%d].ctlclass, Res%s[%d].caption,\n",
		name, n, name, n, name, n);
	fprintf (out, "\t\t\t\tRes%s[%d].param_int[P_STYLE],\n",
		name, n);
	fprintf (out, "\t\t\t\tRes%s[%d].param_int[P_X]*2, Res%s[%d].param_int[P_Y]*3/2, Res%s[%d].param_int[P_W]*2, Res%s[%d].param_int[P_H]*3/2,\n",
		name, n, name, n, name, n, name, n);
	fprintf (out, "\t\t\t\thWnd, COMMAND0+%d, hInst, NULL);\n", n);

}

generate_line_size (struct resource_item *res, FILE *out, int n)
{
	generate_line_source (res, out, n);

	fprintf (out, "\t\t\tMoveWindow (hCtlObj%s[%d],\n", name, n);
	fprintf (out, "\t\t\t\tRes%s[%d].param_int[P_X]*LOWORD(lParam)/(%s),\n",
		name, n, W);
	fprintf (out, "\t\t\t\tRes%s[%d].param_int[P_Y]*HIWORD(lParam)/(%s),\n",
		name, n, H);
	fprintf (out, "\t\t\t\tRes%s[%d].param_int[P_W]*LOWORD(lParam)/(%s),\n",
		name, n, W);
	fprintf (out, "\t\t\t\tRes%s[%d].param_int[P_H]*HIWORD(lParam)/(%s),\n",
		name, n, H);
	fprintf (out, "\t\t\t\tTRUE);\n");

}

generate_line_command_API (struct resource_item *res, FILE *out, int n)
{
	fprintf (out, "\t\t\t\tcase COMMAND0+%d:\n", n /*res->param[P_ID]*/);
	/* fprintf (out, "\t\t\t\t\tHandleCtl%s%d();\n", name, n); */
	fprintf (out, "\t\t\t\t\t");
	generate_handle (res, out, n);
	fprintf (out, " (&msg);\n");
	fprintf (out, "\t\t\t\t\tbreak;\n");
}

generate_line_init_res (struct resource_item *res, FILE *out, int n)
{
int i;
/*
struct resource_item
{
	char name[30];
	char ctlclass[30];
	char caption[300];
	char param[8][120];
	int id, X, Y, W, H, style;
	char source[300];

};
*/
	/* fprintf (out, "\n"); */
	generate_line_source (res, out, n);

	fprintf (out, "\tstrcpy (Res%s[%d].name, \"%s\");\n",
		name, n, res->name);
	fprintf (out, "\tstrcpy (Res%s[%d].ctlclass, %s);\n",
		name, n, res->ctlclass);
	fprintf (out, "\tstrcpy (Res%s[%d].caption, \"%s\");\n",
		name, n, res->caption);
	for (i=0; i<6; i++)
	{
		/*
		fprintf (out, "\tstrcpy (Res%s[%d].param[%d], \"%s\");\n",
			name, n, i, res->param[i]);
		*/
		if (i != 0)
			fprintf (out, "\tRes%s[%d].param_int[%d] = %s;\n",
				name, n, i, res->param[i]);
	}
/*
	fprintf (out, "\tstrcpy (Res%s[%d].source, \"%s\");\n",
		name, n, res->source);
*/
}


compile_API (FILE *in, FILE *out, char *in_filename)
{
	fprintf (out, "/* Windows API source generated from resource file %s */\n",
		in_filename);

/*
	fprintf (out, "LONG FAR PASCAL WndProc (HWND, WORD, WORD, LONG);\n");
	fprintf (out, "HANDLE hInst\n\n");

	compile (in, out, GEN_DECL_API);
*/

	fprintf (out, "\n");
	fprintf (out, "#include <windows.h>\n");
	fprintf (out, "\n");
	fprintf (out, "#define COMMAND0 100\n");
	fprintf (out, "\n");
	fprintf (out, "#define ID_TEST 101\n");

	compile (in, out, GEN_DEFINE_API);

	fprintf (out, "\n");
	fprintf (out, "HANDLE hInst;\n");
	fprintf (out, "HWND hWnd%s;\n", name);
	fprintf (out, "\n");
	/* fprintf (out, "static char szAppName[] = \"Windows API application\"\
;\n");  */
	fprintf (out, "static char szAppName%s[] = \"Appli%s\";\n", name, name);
	fprintf (out, "\n");
	fprintf (out, "HWND hTest;\n");

	/* compile (in, out, GEN_DECL_API); */
	fprintf (out, "\nHWND hCtlObj%s[120];\n\n", name);

	fprintf (out, "InitWnd%s ()\n{\n/* Add code here */\n\n}\n", name);

	compile (in, out, GEN_HANDLE_API);

	fprintf (out, "\n");
	fprintf (out, "long FAR PASCAL _export WndProc%s (HWND hWnd, WORD Me\
ssage, WORD wParam, LONG lParam)\n", name);
	fprintf (out, "{\n");
	fprintf (out, "\tswitch (Message)\n");
	fprintf (out, "\t{\n");
	fprintf (out, "\t\tcase WM_CREATE:\n");
#ifdef TEST
	fprintf (out, "\t\t\thTest = CreateWindow (\"BUTTON\", \"Test\",\n");
	fprintf (out, "\t\t\t\tBS_PUSHBUTTON | WS_CHILD | WS_VISIBLE,\n");
	fprintf (out, "\t\t\t\t30, 10, 40, 15, hWnd, ID_TEST, hInst, NULL);\n");
#endif

	compile (in, out, GEN_CREATE_API);

	fprintf (out, "\t\t\tInitWnd%s ();\n", name);
	fprintf (out, "\t\t\tbreak;\n");
	fprintf (out, "\t\tcase WM_COMMAND:\n");
	fprintf (out, "\t\t\tswitch (wParam)\n");
	fprintf (out, "\t\t\t{\n");
#ifdef TEST
	fprintf (out, "\t\t\t\tcase ID_TEST:\n");
	fprintf (out, "\t\t\t\t\tMessageBox (hWnd, \"Test\", \"Test\", MB_OK);\n");
	fprintf (out, "\t\t\t\t\tbreak;\n");
#endif
	compile (in, out, GEN_COMMAND_API);

	fprintf (out, "\t\t\t}\n");
	fprintf (out, "\t\t\tbreak;\n");
	fprintf (out, "\t\tdefault:\n");
	fprintf (out, "\t\t\treturn(DefWindowProc(hWnd, Message, wParam, lPar\
am));\n");
	fprintf (out, "\t}\n");
	fprintf (out, "\treturn NULL;\n");
	fprintf (out, "}\n");
	fprintf (out, "\n");
	fprintf (out, "int PASCAL WinMain (HANDLE hInstance, HANDLE hPrevI\
nstance, LPSTR lpszCmdLine, int nCmdShow)\n");
	fprintf (out, "{\n");
	/* fprintf (out, "HWND hWnd;\n"); */
	fprintf (out, "MSG msg;\n");
	fprintf (out, "WNDCLASS wndclass;\n");
	fprintf (out, "\t_InitEasyWin ();\n");
	fprintf (out, "\thInst = hInstance;\n");
	fprintf (out, "\n");
	fprintf (out, "\tif (!hPrevInstance)\n");
	fprintf (out, "\t{\n");
	fprintf (out, "\t\twndclass.style = CS_HREDRAW | CS_VREDRAW;\n");
	fprintf (out, "\t\twndclass.lpfnWndProc = WndProc%s;\n", name);
	fprintf (out, "\t\twndclass.cbClsExtra = 0;\n");
	fprintf (out, "\t\twndclass.cbWndExtra = 0;\n");
	fprintf (out, "\t\twndclass.hInstance = hInstance;\n");
	fprintf (out, "\t\twndclass.hIcon = LoadIcon(NULL, IDI_APPLICATION);\n");
	fprintf (out, "\t\twndclass.hCursor = LoadCursor(NULL, IDC_ARROW);\n");
	fprintf (out, "\t\twndclass.hbrBackground  = GetStockObject(WHITE_BR\
USH);\n");
	fprintf (out, "\t\twndclass.lpszMenuName = NULL;\n");
	fprintf (out, "\t\twndclass.lpszClassName  = szAppName%s;\n", name);
	fprintf (out, "\n");
	fprintf (out, "\t\tif (!RegisterClass(&wndclass))\n");
	fprintf (out, "\t\t\treturn FALSE;\n");
	fprintf (out, "\t}\n");
	fprintf (out, "\n");
	fprintf (out, "\thWnd%s = CreateWindow (szAppName%s, \"Windows API progr\
am\",\n", name, name);
	fprintf (out, "\t\tWS_OVERLAPPEDWINDOW | WS_VSCROLL | WS_HSCROLL,\n");
	fprintf (out, "\t\tCW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_U\
SEDEFAULT,\n");
	fprintf (out, "\t\tNULL, NULL, hInstance, NULL);\n");
	fprintf (out, "\n");
	fprintf (out, "\t\tif (!hWnd%s)\n", name);
	fprintf (out, "\t\t\treturn FALSE;\n");
	fprintf (out, "\n");
	fprintf (out, "\t\tShowWindow (hWnd%s, nCmdShow);\n", name);
	fprintf (out, "\t\tUpdateWindow (hWnd%s);\n", name);
	fprintf (out, "\n");
	fprintf (out, "\t\twhile(GetMessage(&msg, NULL, 0, 0))\n");
	fprintf (out, "\t\t{\n");
	fprintf (out, "\t\t\tTranslateMessage(&msg);\n");
	fprintf (out, "\t\t\tDispatchMessage(&msg);\n");
	fprintf (out, "\t\t}\n");
	fprintf (out, "\n");
	fprintf (out, "\t\treturn(msg.wParam);\n");
	fprintf (out, "\n");
	fprintf (out, "}\n");

}

compile_comb (FILE *in, FILE *out, char *in_filename)
{
	fprintf (out, "/* Windows API source generated from resource file %s */\n",
		in_filename);
	fprintf (out, "\n");
	fprintf (out, "#include <owl.h>\n");
	fprintf (out, "#include <window.h>\n");
	fprintf (out, " \n");
	fprintf (out, "HANDLE hInst;\n");
	fprintf (out, "HWND hWnd%s;\n", name);
	fprintf (out, "\n");
	fprintf (out, "#define NCTL 120\n");
	fprintf (out, "HWND hCtlObj%s[NCTL];\n", name);
	fprintf (out, "\n");
	fprintf (out, "static char szAppName%s[] = \"Appli%s\";\n", name, name);
	fprintf (out, "\n");
	fprintf (out, "#define COMMAND0 100\n");
	fprintf (out, "\n");
	fprintf (out, "#define hCtlId%s(id) (hCtlObj%s[(id)-COMMAND0])\n",
		name, name);
	fprintf (out, "\n");

	compile (in, out, GEN_DEFINE_API);

	fprintf (out, "\n");
	fprintf (out, "InitWnd%s ()\n{\n/* Add code here */\n\n}\n", name);

	compile (in, out, GEN_HANDLE_API);

	fprintf (out, "\n");
	fprintf (out, "LRESULT CALLBACK WndProc%s (HWND hWnd, UINT Message, \
WPARAM wParam, LPARAM lParam)\n", name);
	fprintf (out, "{\n");
	fprintf (out, "\tswitch (Message)\n");
	fprintf (out, "\t{\n");
	fprintf (out, "\t\tcase WM_CREATE:\n");
	fprintf (out, "        \t/* create control objects hCtlObj%s[i] with \
CreateWindow */\n", name);

	compile (in, out, GEN_CREATE_API);

	fprintf (out, "\t\t\tInitWnd%s ();\n", name);

	fprintf (out, " \t\t\tbreak;\n");
	fprintf (out, "\t\tcase WM_COMMAND:\n");
	fprintf (out, "\t\t\tswitch (wParam)\n");
	fprintf (out, "\t\t\t{\n");
	fprintf (out, "\t\t\t\t/* case COMMAND0+i:\n");
	fprintf (out, "\t\t\t\t\tHandleCtl%si();\n", name);
	fprintf (out, "\t\t\t\t\tbreak; */\n");

	compile (in, out, GEN_COMMAND_API);

	fprintf (out, " \t\t\t}\n");
	fprintf (out, "\t\t\tbreak;\n");
	fprintf (out, "\t\tdefault:\n");
	fprintf (out, "\t\t\treturn(DefWindowProc(hWnd, Message, wParam, lPar\
am));\n");
	fprintf (out, "\t}\n");
	fprintf (out, "\treturn NULL;\n");
	fprintf (out, "}\n");
	fprintf (out, "\n");
	fprintf (out, "class TAppliWindow%s : public TWindow\n", name);
	fprintf (out, "{\n");
	fprintf (out, "public:\n");
	fprintf (out, "\t/* declare control objects PT... */\n");
	fprintf (out, "\t/* PTButton Button1; */\n");
	fprintf (out, "    PTWindowsObject pWndCtlObj[NCTL];\n");
	fprintf (out, "\tHWND hWndCtlObj[NCTL];\n");
	fprintf (out, " \n");
	fprintf (out, "\tTAppliWindow%s (PTWindowsObject parent, LPSTR title)\
 :\n", name);
	fprintf (out, "\t\tTWindow (parent, title)\n");
	fprintf (out, "\t{\n");
	fprintf (out, "\t\tEnableKBHandler ();\n");
	fprintf (out, "        /* create control objects PT... with new T.\
.. */ \n");
	fprintf (out, "\t\t/* Button1 = new TButton (this, 101, \"&Test 1\", 5\
0, 80, 40, 20, FALSE); */\n");
	fprintf (out, "\t}\n");
	fprintf (out, "\n");
	fprintf (out, "\tLPSTR GetClassName ()\n");
	fprintf (out, "\t{\n");
	fprintf (out, "    \treturn szAppName%s;\n", name);
	fprintf (out, "\t}\n");
	fprintf (out, "\n");
	fprintf (out, "\tvirtual BOOL Create ()\n");
	fprintf (out, "\t{\n");
	fprintf (out, "\tBOOL status;\n");
	/* fprintf (out, "\t\thWnd = HWindow;\n"); */
	fprintf (out, "\t\tstatus = TWindow::Create ();\n");
	fprintf (out, "\t\thWnd%s = HWindow;\n", name);
	fprintf (out, "        /* create objects hWndCtlObj[i] with Create\
Window */ \n");
	fprintf (out, " \t\treturn status;\n");
	fprintf (out, "    }\n");
	fprintf (out, "\t/* void HandleWndCtli (RTMessage Msg) = [ID_FIRST \
+ COMMAND0 + i] { ... } */\n");
	fprintf (out, "};\n");
	fprintf (out, "\n");
	fprintf (out, "TAppliWindow%s *Wnd%s;\n", name, name);
	fprintf (out, "\n");
	fprintf (out, "class TAppli%s : public TApplication\n", name);
	fprintf (out, "{\n");
	fprintf (out, "public:\n");
	fprintf (out, "\tTAppli%s (LPSTR AName, HINSTANCE hInstance, HINSTANC\
E hPrevInstance,\n", name);
	fprintf (out, "\t\tLPSTR lpCmdLine, int nCmdShow) :\n");
	fprintf (out, "\t\tTApplication (AName, hInstance, hPrevInstance, lp\
CmdLine, nCmdShow)\n");
	fprintf (out, "\t{\n");
	fprintf (out, "\t}\n");
	fprintf (out, "\tvirtual void InitMainWindow ()\n");
	fprintf (out, "\t{\n");
	fprintf (out, "\t\tWnd%s = new TAppliWindow%s (NULL, Name);\n", name, name);
	fprintf (out, "\t\tMainWindow = Wnd%s;\n", name);
	fprintf (out, "    }\n");
	fprintf (out, "};\n");
	fprintf (out, "\n");
	fprintf (out, "int PASCAL WinMain (HINSTANCE hInstance, HINSTANCE \
hPrevInstance,\n");
	fprintf (out, "\tLPSTR lpCmdLine, int nCmdShow)\n");
	fprintf (out, "{\n");
	fprintf (out, "WNDCLASS wndclass;\n");
	fprintf (out, "\n");
	fprintf (out, "\tif (!hPrevInstance)\n");
	fprintf (out, "\t{\n");
	fprintf (out, "\t\twndclass.style = CS_HREDRAW | CS_VREDRAW;\n");
	fprintf (out, "\t\twndclass.lpfnWndProc = WndProc%s;\n", name);
	fprintf (out, "\t\twndclass.cbClsExtra = 0;\n");
	fprintf (out, "\t\twndclass.cbWndExtra = 0;\n");
	fprintf (out, "\t\twndclass.hInstance = hInstance;\n");
	fprintf (out, "\t\twndclass.hIcon = LoadIcon(NULL, IDI_APPLICATION);\n");
	fprintf (out, "\t\twndclass.hCursor = LoadCursor(NULL, IDC_ARROW);\n");
	fprintf (out, "\t\twndclass.hbrBackground  = GetStockObject(WHITE_BR\
USH);\n");
	fprintf (out, "\t\twndclass.lpszMenuName = NULL;\n");
	fprintf (out, "\t\twndclass.lpszClassName  = szAppName%s;\n", name);
	fprintf (out, "\n");
	fprintf (out, "\t\tif (!RegisterClass(&wndclass))\n");
	fprintf (out, "\t\t\treturn FALSE;\n");
	fprintf (out, "\t}\n");
	fprintf (out, "\n");
	fprintf (out, "\thInst = hInstance;\n");
	fprintf (out, "\tTAppli%s Appli (szAppName%s, hInstance, hPrevInstance,\
 lpCmdLine, nCmdShow);\n", name, name);
	fprintf (out, "\tAppli.Run ();\n");
	fprintf (out, "\treturn Appli.Status;\n");
	fprintf (out, "}\n");

}

compile_mob (FILE *in, FILE *out, char *in_filename)
{
	fprintf (out, "/* Windows API source generated from resource file %s */\n",
		in_filename);
	fprintf (out, "\n");
	fprintf (out, "#include <owl.h>\n");
	fprintf (out, "#include <window.h>\n");
	fprintf (out, "#include <string.h>\n");
	fprintf (out, " \n");
	fprintf (out, "HANDLE hInst;\n");
	fprintf (out, "HWND hWnd%s;\n", name);
	fprintf (out, "\n");
	fprintf (out, "#define NCTL 60\n");
	fprintf (out, "HWND hCtlObj%s[NCTL];\n", name);
	fprintf (out, "\n");
	fprintf (out, "static char szAppName%s[] = \"Appli%s\";\n", name, name);
	fprintf (out, "\n");
	fprintf (out, "#define COMMAND0 100\n");
	fprintf (out, "\n");
	fprintf (out, "#define hCtlId%s(id) (hCtlObj%s[(id)-COMMAND0])\n",
		name, name);
	fprintf (out, "\n");

	fprintf (out, "#define P_ID 0\n");
	fprintf (out, "#define P_X 1\n");
	fprintf (out, "#define P_Y 2\n");
	fprintf (out, "#define P_W 3\n");
	fprintf (out, "#define P_H 4\n");
	fprintf (out, "#define P_STYLE 5\n");
	fprintf (out, "\n");

	fprintf (out, "struct resource_item\n");
	fprintf (out, "{\n");
	fprintf (out, "\tchar name[15];\n");
	fprintf (out, "\tchar ctlclass[15];\n");
	fprintf (out, "\tchar caption[80];\n");
	/* fprintf (out, "\tchar param[8][60];\n"); */
	fprintf (out, "\tlong param_int[8];\n");
	fprintf (out, "\tint id, X, Y, W, H, style;\n");
	/* fprintf (out, "\tchar source[150];\n"); */
	fprintf (out, "\t};\n");
	fprintf (out, "\n");

	fprintf (out, "struct resource_item Res%s[NCTL];\n\n", name);

	fprintf (out, "InitRes%s ()\n{\n", name);

	compile (in, out, GEN_INIT_RES);

	fprintf (out, "\n\treturn 0;\n}\n\n");

	compile (in, out, GEN_DEFINE_API);

	fprintf (out, "\n");
	fprintf (out, "InitWnd%s ()\n{\n/* Add code here */\n\n\treturn 0;\n}\n", name);

	compile (in, out, GEN_HANDLE_API);

	fprintf (out, "\n");
	fprintf (out, "LRESULT CALLBACK WndProc%s (HWND hWnd, UINT Message, \
WPARAM wParam, LPARAM lParam)\n", name);
	fprintf (out, "{\n");
	fprintf (out, "int nCtl;\n");
	fprintf (out, "\tswitch (Message)\n");
	fprintf (out, "\t{\n");

	fprintf (out, "\t\tcase WM_CREATE:\n");
	fprintf (out, "        \t/* create control objects hCtlObj%s[i] with \
CreateWindow */\n", name);

	/* compile (in, out, GEN_CREATE_MOB); */

	fprintf (out, "\t\t\tfor (nCtl=1; nCtl<=%d; nCtl++)\n", max_item);
	fprintf (out, "\t\t\t{\n");

	fprintf (out, "\t\t\t\thCtlObj%s[nCtl] = CreateWindow (Res%s[nCtl].ctlclass, Res%s[nCtl].caption,\n",
		name, name, name);
	fprintf (out, "\t\t\t\t\tRes%s[nCtl].param_int[P_STYLE],\n",
		name);
	fprintf (out, "\t\t\t\t\tRes%s[nCtl].param_int[P_X]*2, Res%s[nCtl].param_int[P_Y]*3/2, Res%s[nCtl].param_int[P_W]*2, Res%s[nCtl].param_int[P_H]*3/2,\n",
		name, name, name, name);
	fprintf (out, "\t\t\t\t\thWnd, COMMAND0+nCtl, hInst, NULL);\n");

	fprintf (out, "\t\t\t}\n");

	fprintf (out, "\t\t\tInitWnd%s ();\n", name);


	fprintf (out, " \t\t\tbreak;\n");

	fprintf (out, "\t\tcase WM_SIZE:\n");

	/* compile (in, out, GEN_SIZE); */

	fprintf (out, "\t\t\tfor (nCtl=1; nCtl<=%d; nCtl++)\n", max_item);
	fprintf (out, "\t\t\t{\n");

	fprintf (out, "\t\t\t\tMoveWindow (hCtlObj%s[nCtl],\n", name);
	fprintf (out, "\t\t\t\t\tRes%s[nCtl].param_int[P_X]*LOWORD(lParam)/(%s),\n",
		name, W);
	fprintf (out, "\t\t\t\t\tRes%s[nCtl].param_int[P_Y]*HIWORD(lParam)/(%s),\n",
		name, H);
	fprintf (out, "\t\t\t\t\tRes%s[nCtl].param_int[P_W]*LOWORD(lParam)/(%s),\n",
		name, W);
	fprintf (out, "\t\t\t\t\tRes%s[nCtl].param_int[P_H]*HIWORD(lParam)/(%s),\n",
		name, H);
	fprintf (out, "\t\t\t\t\tTRUE);\n");

	fprintf (out, "\t\t\t}\n");

	fprintf (out, " \t\t\tbreak;\n");



	fprintf (out, "\t\tcase WM_COMMAND:\n");
	fprintf (out, "\t\t\tswitch (wParam)\n");
	fprintf (out, "\t\t\t{\n");
	fprintf (out, "\t\t\t\t/* case COMMAND0+i:\n");
	fprintf (out, "\t\t\t\t\tHandleCtl%si();\n", name);
	fprintf (out, "\t\t\t\t\tbreak; */\n");

	compile (in, out, GEN_COMMAND_API);

	fprintf (out, " \t\t\t}\n");
	fprintf (out, "\t\t\tbreak;\n");
	fprintf (out, "\t\tdefault:\n");
	fprintf (out, "\t\t\treturn(DefWindowProc(hWnd, Message, wParam, lPar\
am));\n");
	fprintf (out, "\t}\n");
	fprintf (out, "\treturn NULL;\n");
	fprintf (out, "}\n");
	fprintf (out, "\n");
	fprintf (out, "class TAppliWindow%s : public TWindow\n", name);
	fprintf (out, "{\n");
	fprintf (out, "public:\n");
	fprintf (out, "\t/* declare control objects PT... */\n");
	fprintf (out, "\t/* PTButton Button1; */\n");
	fprintf (out, "    PTWindowsObject pWndCtlObj[NCTL];\n");
	fprintf (out, "\tHWND hWndCtlObj[NCTL];\n");
	fprintf (out, " \n");
	fprintf (out, "\tTAppliWindow%s (PTWindowsObject parent, LPSTR title)\
 :\n", name);
	fprintf (out, "\t\tTWindow (parent, title)\n");
	fprintf (out, "\t{\n");
	fprintf (out, "\t\tEnableKBHandler ();\n");
	fprintf (out, "        /* create control objects PT... with new T.\
.. */ \n");
	fprintf (out, "\t\t/* Button1 = new TButton (this, 101, \"&Test 1\", 5\
0, 80, 40, 20, FALSE); */\n");
	fprintf (out, "\t}\n");
	fprintf (out, "\n");
	fprintf (out, "\tLPSTR GetClassName ()\n");
	fprintf (out, "\t{\n");
	fprintf (out, "    \treturn szAppName%s;\n", name);
	fprintf (out, "\t}\n");
	fprintf (out, "\n");
	fprintf (out, "\tvirtual BOOL Create ()\n");
	fprintf (out, "\t{\n");
	fprintf (out, "\tBOOL status;\n");
	/* fprintf (out, "\t\thWnd = HWindow;\n"); */
	fprintf (out, "\t\tstatus = TWindow::Create ();\n");
	fprintf (out, "\t\thWnd%s = HWindow;\n", name);
	fprintf (out, "        /* create objects hWndCtlObj[i] with Create\
Window */ \n");
	fprintf (out, " \t\treturn status;\n");
	fprintf (out, "    }\n");
	fprintf (out, "\t/* void HandleWndCtli (RTMessage Msg) = [ID_FIRST \
+ COMMAND0 + i] { ... } */\n");
	fprintf (out, "};\n");
	fprintf (out, "\n");
	fprintf (out, "TAppliWindow%s *Wnd%s;\n", name, name);
	fprintf (out, "\n");
	fprintf (out, "class TAppli%s : public TApplication\n", name);
	fprintf (out, "{\n");
	fprintf (out, "public:\n");
	fprintf (out, "\tTAppli%s (LPSTR AName, HINSTANCE hInstance, HINSTANC\
E hPrevInstance,\n", name);
	fprintf (out, "\t\tLPSTR lpCmdLine, int nCmdShow) :\n");
	fprintf (out, "\t\tTApplication (AName, hInstance, hPrevInstance, lp\
CmdLine, nCmdShow)\n");
	fprintf (out, "\t{\n");
	fprintf (out, "\t}\n");
	fprintf (out, "\tvirtual void InitMainWindow ()\n");
	fprintf (out, "\t{\n");
	fprintf (out, "\t\tWnd%s = new TAppliWindow%s (NULL, Name);\n", name, name);
	fprintf (out, "\t\tMainWindow = Wnd%s;\n", name);
	fprintf (out, "\t\tWnd%s->Attr.W = (%s)*2;\n", name, W);
	fprintf (out, "\t\tWnd%s->Attr.H = (%s)*3/2+20;\n", name, H);
	fprintf (out, "    }\n");
	fprintf (out, "};\n");
	fprintf (out, "\n");
	fprintf (out, "int PASCAL WinMain (HINSTANCE hInstance, HINSTANCE \
hPrevInstance,\n");
	fprintf (out, "\tLPSTR lpCmdLine, int nCmdShow)\n");
	fprintf (out, "{\n");
	fprintf (out, "WNDCLASS wndclass;\n");
	fprintf (out, "\n");

	fprintf (out, "\tInitRes%s ();\n", name);

	fprintf (out, "\tif (!hPrevInstance)\n");
	fprintf (out, "\t{\n");
	fprintf (out, "\t\twndclass.style = CS_HREDRAW | CS_VREDRAW;\n");
	fprintf (out, "\t\twndclass.lpfnWndProc = WndProc%s;\n", name);
	fprintf (out, "\t\twndclass.cbClsExtra = 0;\n");
	fprintf (out, "\t\twndclass.cbWndExtra = 0;\n");
	fprintf (out, "\t\twndclass.hInstance = hInstance;\n");
	fprintf (out, "\t\twndclass.hIcon = LoadIcon(NULL, IDI_APPLICATION);\n");
	fprintf (out, "\t\twndclass.hCursor = LoadCursor(NULL, IDC_ARROW);\n");
	fprintf (out, "\t\twndclass.hbrBackground  = GetStockObject(WHITE_BR\
USH);\n");
	fprintf (out, "\t\twndclass.lpszMenuName = NULL;\n");
	fprintf (out, "\t\twndclass.lpszClassName  = szAppName%s;\n", name);
	fprintf (out, "\n");
	fprintf (out, "\t\tif (!RegisterClass(&wndclass))\n");
	fprintf (out, "\t\t\treturn FALSE;\n");
	fprintf (out, "\t}\n");
	fprintf (out, "\n");
	fprintf (out, "\thInst = hInstance;\n");
	fprintf (out, "\tTAppli%s Appli (szAppName%s, hInstance, hPrevInstance,\
 lpCmdLine, nCmdShow);\n", name, name);
	fprintf (out, "\tAppli.Run ();\n");
	fprintf (out, "\treturn Appli.Status;\n");
	fprintf (out, "}\n");

}


compile_gen (FILE *in, FILE *out, char *in_filename, char *gen)
{
	fprintf (out, "/* Windows API source generated from resource file %s */\n",
		in_filename);
	fprintf (out, "\n");
	fprintf (out, "#include <owl.h>\n");
	fprintf (out, "#include <window.h>\n");
	fprintf (out, "#include <string.h>\n");
	fprintf (out, "#include <ctype.h>\n");
	fprintf (out, "\n");
	fprintf (out, "%s\n", config);
	fprintf (out, "\n");
	fprintf (out, "HANDLE hInst;\n");
	fprintf (out, "HWND hWnd%s;\n", name);
	fprintf (out, "\n");
	fprintf (out, "#define NCTL 60\n");
	fprintf (out, "HWND hCtlObj%s[NCTL];\n", name);
	fprintf (out, "\n");
	fprintf (out, "int Focus%s;\n", name);
	fprintf (out, "\n");
	fprintf (out, "static char szAppName%s[] = \"Appli%s\";\n", name, name);
	fprintf (out, "static char WndCaption%s[] = \"%s\";\n", name, name);
	fprintf (out, "\n");
	fprintf (out, "#define COMMAND0 %d\n", COMMAND0);
	fprintf (out, "\n");
	fprintf (out, "#define hCtlId%s(id) (hCtlObj%s[(id)-COMMAND0])\n",
		name, name);
	fprintf (out, "\n");

	fprintf (out, "#define Wid%s (%s)\n", name, W);
	fprintf (out, "#define Hei%s (%s)\n", name, H);
	fprintf (out, "\n");

	fprintf (out, "#define P_ID 0\n");
	fprintf (out, "#define P_X 1\n");
	fprintf (out, "#define P_Y 2\n");
	fprintf (out, "#define P_W 3\n");
	fprintf (out, "#define P_H 4\n");
	fprintf (out, "#define P_STYLE 5\n");
	fprintf (out, "\n");

	fprintf (out, "struct resource_item\n");
	fprintf (out, "{\n");
	fprintf (out, "\tchar name[15];\n");
	fprintf (out, "\tchar ctlclass[15];\n");
	fprintf (out, "\tchar caption[80];\n");
	/* fprintf (out, "\tchar param[8][60];\n"); */
	fprintf (out, "\tlong param_int[8];\n");
	fprintf (out, "\tint id, X, Y, W, H, style;\n");
	/* fprintf (out, "\tchar source[150];\n"); */
	fprintf (out, "\t};\n");
	fprintf (out, "\n");

	fprintf (out, "struct resource_item Res%s[NCTL];\n\n", name);

	fprintf (out, "InitRes%s ()\n{\n", name);

	compile (in, out, GEN_INIT_RES);

	fprintf (out, "\n#define NCTL%s %d\n\n", name, max_item);

	fprintf (out, "\n\treturn 0;\n}\n\n");

	compile (in, out, GEN_DEFINE_API);

	fprintf (out, "\n");
    fprintf (out, "int InitWnd%s ();\n", name);

	fprintf (out, "class TAppliWindow%s : public TWindow\n", name);
	fprintf (out, "{\n");
	fprintf (out, "public:\n");
	fprintf (out, "\t/* declare control objects PT... */\n");
	fprintf (out, "\t/* PTButton Button1; */\n");
	fprintf (out, "    PTWindowsObject pWndCtlObj[NCTL];\n");
	fprintf (out, "\tHWND hWndCtlObj[NCTL];\n");
	fprintf (out, " \n");
	fprintf (out, "\tTAppliWindow%s (PTWindowsObject parent, LPSTR title)\
 :\n", name);
	fprintf (out, "\t\tTWindow (parent, title)\n");
	fprintf (out, "\t{\n");
	fprintf (out, "\t\tEnableKBHandler ();\n");
	fprintf (out, "        /* create control objects PT... with new T.\
.. */ \n");
	fprintf (out, "\t\t/* Button1 = new TButton (this, 101, \"&Test 1\", 5\
0, 80, 40, 20, FALSE); */\n");
	fprintf (out, "\t}\n");
	fprintf (out, "\n");
	fprintf (out, "\tLPSTR GetClassName ()\n");
	fprintf (out, "\t{\n");
	fprintf (out, "    \treturn szAppName%s;\n", name);
	fprintf (out, "\t}\n");
	fprintf (out, "\n");
	fprintf (out, "\tvirtual BOOL Create ()\n");
	fprintf (out, "\t{\n");
	fprintf (out, "\tBOOL status;\n");
	/* fprintf (out, "\t\thWnd = HWindow;\n"); */
	fprintf (out, "\t\tstatus = TWindow::Create ();\n");
	fprintf (out, "\t\thWnd%s = HWindow;\n", name);
	fprintf (out, "        /* create objects hWndCtlObj[i] with Create\
Window */ \n");
	fprintf (out, " \t\treturn status;\n");
	fprintf (out, "    }\n");
	fprintf (out, "\t/* void HandleWndCtli (RTMessage Msg) = [ID_FIRST \
+ COMMAND0 + i] { ... } */\n");
	fprintf (out, "};\n");
	fprintf (out, "\n");
	fprintf (out, "TAppliWindow%s *Wnd%s;\n", name, name);
	fprintf (out, "\n");
	fprintf (out, "class TAppli%s : public TApplication\n", name);
	fprintf (out, "{\n");
	fprintf (out, "public:\n");
	fprintf (out, "\tTAppli%s (LPSTR AName, HINSTANCE hInstance, HINSTANC\
E hPrevInstance,\n", name);
	fprintf (out, "\t\tLPSTR lpCmdLine, int nCmdShow) :\n");
	fprintf (out, "\t\tTApplication (AName, hInstance, hPrevInstance, lp\
CmdLine, nCmdShow)\n");
	fprintf (out, "\t{\n");
	fprintf (out, "\t}\n");
	fprintf (out, "\tvirtual void InitMainWindow ()\n");
	fprintf (out, "\t{\n");
	fprintf (out, "\t\tWnd%s = new TAppliWindow%s (NULL, Name);\n", name, name);
	fprintf (out, "\t\tMainWindow = Wnd%s;\n", name);
	fprintf (out, "\t\tWnd%s->Attr.W = (Wid%s)*2;\n", name, name);
	fprintf (out, "\t\tWnd%s->Attr.H = (Hei%s)*3/2+20;\n", name, name);
	fprintf (out, "    }\n");
	fprintf (out, "};\n");
	fprintf (out, "\n");


	compile (in, out, GEN_HANDLE_API);

	fprintf (out, "\n");
	fprintf (out, "LRESULT CALLBACK WndProc%s (HWND hWnd, UINT Message, \
WPARAM wParam, LPARAM lParam)\n", name);
	fprintf (out, "{\n");
	fprintf (out, "int nCtl;\n");

	fprintf (out, "struct TMessage msg;\n");
	fprintf (out, "\tmsg.Receiver = hWnd;\n");
	fprintf (out, "\tmsg.Message = Message;\n");
	fprintf (out, "\tmsg.WParam = wParam;\n");
	fprintf (out, "\tmsg.LParam = lParam;\n");
	fprintf (out, "\tmsg.Result = 0;\n");

	fprintf (out, "\tswitch (Message)\n");
	fprintf (out, "\t{\n");

	fprintf (out, "\t\tcase WM_CREATE:\n");
	fprintf (out, "\t\t\tWnd%s->HWindow = hWnd%s;\n", name, name);
	fprintf (out, "        \t/* create control objects hCtlObj%s[i] with \
CreateWindow */\n", name);

	/* compile (in, out, GEN_CREATE_MOB); */

	fprintf (out, "\t\t\tfor (nCtl=1; nCtl<=NCTL%s; nCtl++)\n", name);
	fprintf (out, "\t\t\t{\n");

	fprintf (out, "\t\t\t\thCtlObj%s[nCtl] = CreateWindow (Res%s[nCtl].ctlclass, Res%s[nCtl].caption,\n",
		name, name, name);
	fprintf (out, "\t\t\t\t\tRes%s[nCtl].param_int[P_STYLE],\n",
		name);
	fprintf (out, "\t\t\t\t\tRes%s[nCtl].param_int[P_X]*2, Res%s[nCtl].param_int[P_Y]*3/2, Res%s[nCtl].param_int[P_W]*2, Res%s[nCtl].param_int[P_H]*3/2,\n",
		name, name, name, name);
	fprintf (out, "\t\t\t\t\thWnd, COMMAND0+nCtl, hInst, NULL);\n");

	fprintf (out, "\t\t\t}\n");

	fprintf (out, "\t\t\tFocus%s = 1;\n", name);
	fprintf (out, "\t\t\tSetFocus (hCtlObj%s[Focus%s]);\n", name, name); 

	fprintf (out, "\t\t\tInitWnd%s ();\n", name);


	fprintf (out, "\t\t\tbreak;\n");

	fprintf (out, "\t\tcase WM_CLOSE:\n");
	fprintf (out, "\t\t\tfor (nCtl=1; nCtl<=NCTL%s; nCtl++)\n", name);
	fprintf (out, "\t\t\t\tDestroyWindow (hCtlObj%s[nCtl]);\n", name);
	/* fprintf (out, "\t\t\tPostQuitMessage (0);\n"); */
	fprintf (out, "\t\t\treturn DefWindowProc(hWnd, Message, wParam, lPar\
am);\n");

	/*fprintf (out, "\t\t\tbreak;\n");*/

	fprintf (out, "\t\tcase WM_SIZE:\n");

	/* compile (in, out, GEN_SIZE); */

	fprintf (out, "\t\t\tfor (nCtl=1; nCtl<=NCTL%s; nCtl++)\n", name);
	fprintf (out, "\t\t\t{\n");

	fprintf (out, "\t\t\t\tMoveWindow (hCtlObj%s[nCtl],\n", name);
	fprintf (out, "\t\t\t\t\tRes%s[nCtl].param_int[P_X]*LOWORD(lParam)/(Wid%s),\n",
		name, name);
	fprintf (out, "\t\t\t\t\tRes%s[nCtl].param_int[P_Y]*HIWORD(lParam)/(Hei%s),\n",
		name, name);
	fprintf (out, "\t\t\t\t\tRes%s[nCtl].param_int[P_W]*LOWORD(lParam)/(Wid%s),\n",
		name, name);
	fprintf (out, "\t\t\t\t\tRes%s[nCtl].param_int[P_H]*HIWORD(lParam)/(Hei%s),\n",
		name, name);
	fprintf (out, "\t\t\t\t\tTRUE);\n");

	fprintf (out, "\t\t\t}\n");

	fprintf (out, " \t\t\tbreak;\n");



	fprintf (out, "\t\tcase WM_COMMAND:\n");
	fprintf (out, "\t\t\tswitch (wParam)\n");
	fprintf (out, "\t\t\t{\n");
	fprintf (out, "\t\t\t\t/* case COMMAND0+i:\n");
	fprintf (out, "\t\t\t\t\tHandleCtl%si();\n", name);
	fprintf (out, "\t\t\t\t\tbreak; */\n");

	compile (in, out, GEN_COMMAND_API);

	fprintf (out, " \t\t\t}\n");
	fprintf (out, "\t\t\tbreak;\n");
	fprintf (out, "\t\tdefault:\n");
	fprintf (out, "\t\t\treturn DefWindowProc(hWnd, Message, wParam, lPar\
am);\n");
	fprintf (out, "\t}\n");
	fprintf (out, "\treturn NULL;\n");
	fprintf (out, "}\n");
	fprintf (out, "\n");

	fprintf (out, "InitWnd%s ()\n{\n/* Add code here */\n\n\treturn 0;\n}\n", name);

 	fprintf (out, "char CtlKey (char *s)\n");
	fprintf (out, "{\n");
	fprintf (out, "int i, l;\n");
	fprintf (out, "\tl = strlen (s);\n");
	fprintf (out, "\tfor (i=0; i<l-1; i++)\n");
	fprintf (out, "\t{\n");
	fprintf (out, "\t\tif (s[i] == '&')\n");
	fprintf (out, "        \treturn s[i+1];\n");
	fprintf (out, "\t}\n");
	fprintf (out, "\treturn 0;\n");
	fprintf (out, "\n");
	fprintf (out, "}\n");
	fprintf (out, "\n");
	fprintf (out, "int KBHandler (MSG *pmsg, HWND hWnd, int NbrCtl, st\
ruct resource_item *Res, HWND *hCtlObj, int *pFocus)\n");
	fprintf (out, "{\n");
	fprintf (out, "int PrevFocus, nCtl;\n");
/*    fprintf (out, "LONG style;\n"); */
    fprintf (out, "HWND hw;\n");
	fprintf (out, "\t\tif (pmsg->message == WM_CHAR)\n");
	fprintf (out, "\t\t{\n");
	fprintf (out, "\t\t\tif (pmsg->wParam == '\\t')\n");
	fprintf (out, "\t\t\t{\n");

	fprintf (out, "\t\t\t  hw = GetFocus ();\n");
	fprintf (out, "\t\t\t  for (nCtl=1; nCtl<=NbrCtl; nCtl++)\n");
	fprintf (out, "\t\t\t  {\n");
	fprintf (out, "\t\t\t\tif (hw == hCtlObj[nCtl])\n");
	fprintf (out, "\t\t\t\t\t*pFocus = nCtl;\n");
	fprintf (out, "\t\t\t  }        \n");

	fprintf (out, "              PrevFocus = *pFocus;\n");
	fprintf (out, "\t\t\t  do\n");
	fprintf (out, "              {\n");
	fprintf (out, "\t\t\t\tif (GetKeyState (VK_SHIFT) >= 0)\n");
	fprintf (out, "                {\n");
	fprintf (out, "\t\t\t\t\tif (*pFocus < NbrCtl)\n");
	fprintf (out, "\t\t\t\t\t\t(*pFocus)++;\n");
	fprintf (out, "\t\t\t\t\telse\n");
	fprintf (out, "\t\t\t\t\t\t*pFocus = 1;\n");
	fprintf (out, "\t\t\t\t}\n");
	fprintf (out, "\t\t\t\telse\n");
	fprintf (out, "\t\t\t\t{\n");
	fprintf (out, "\t\t\t\t\tif (*pFocus > 1)\n");
	fprintf (out, "\t\t\t\t\t\t(*pFocus)--;\n");
	fprintf (out, "\t\t\t\t\telse\n");
	fprintf (out, "\t\t\t\t\t\t*pFocus = NbrCtl;\n");
	fprintf (out, "\t\t\t\t}\n");
	/* fprintf (out, "\t\t\t\t GetWindowLong (hCtlObj[*pFocus], GWL_STYLE); */
	fprintf (out, "\t\t\t  }\n");
/*	fprintf (out, "\t\t\t  while (!strcmp (Res[*pFocus].name, \"LTEXT\") &&\n");*/
	fprintf (out, "\t\t\t  while (!(GetWindowLong (hCtlObj[*pFocus], GWL_STYLE) & WS_TABSTOP) &&\n");
 	fprintf (out, "\t\t\t\t\t (*pFocus != PrevFocus));\n");
	fprintf (out, "\t\t\t  SetFocus (hCtlObj[*pFocus]);\n");
	fprintf (out, " \t\t\t  return 1;\n");
	fprintf (out, "\t\t\t}\n");
	fprintf (out, "        }\n");
	fprintf (out, " \t\tif (pmsg->message == WM_SYSCHAR)\n");
	fprintf (out, "\t\t{\n");
	fprintf (out, "\t\t\t\tfor (nCtl=1; nCtl<=NbrCtl; nCtl++)\n");
	fprintf (out, "\t\t\t\t{\n");
	fprintf (out, "\t\t\t\t\tif (toupper(CtlKey (Res[nCtl].caption)) == tou\
pper(pmsg->wParam))\n");
	fprintf (out, "\t\t\t\t\t{\n");
	fprintf (out, "\t\t\t\t\t\t*pFocus = nCtl;\n");
	fprintf (out, "\t\t\t\t\t\tSetFocus (hCtlObj[*pFocus]);\n");
	fprintf (out, "\t\t\t\t\t\tif (!strcmp (Res[nCtl].name, \"PUSHBUTTON\"))\n");
	fprintf (out, "\t\t\t\t\t\t\tSendMessage (hWnd, WM_COMMAND, COMMAND0+nCtl\
, 0);\n");
	fprintf (out, " \t\t\t\t\t\treturn 1;\n");
	fprintf (out, "\t\t\t\t\t} \n");
	fprintf (out, "                }\n");
	fprintf (out, " \t\t}\n");
	fprintf (out, "\t\treturn 0;\n");
	fprintf (out, "}\t\n");
	fprintf (out, "\n");

	fprintf (out, "int PASCAL WinMain (HINSTANCE hInstance, HINSTANCE \
hPrevInstance,\n");
	fprintf (out, "\tLPSTR lpCmdLine, int nCmdShow)\n");
	fprintf (out, "{\n");
	fprintf (out, "WNDCLASS wndclass;\n");
	fprintf (out, "MSG msg;\n");
	fprintf (out, "\n");

	fprintf (out, "\thInst = hInstance;\n");

	fprintf (out, "\tInitRes%s ();\n", name);

	fprintf (out, "\tif (!hPrevInstance)\n");
	fprintf (out, "\t{\n");
	fprintf (out, "\t\twndclass.style = CS_HREDRAW | CS_VREDRAW;\n");
	fprintf (out, "\t\twndclass.lpfnWndProc = WndProc%s;\n", name);
	fprintf (out, "\t\twndclass.cbClsExtra = 0;\n");
	fprintf (out, "\t\twndclass.cbWndExtra = 0;\n");
	fprintf (out, "\t\twndclass.hInstance = hInstance;\n");
	fprintf (out, "\t\twndclass.hIcon = LoadIcon(NULL, IDI_APPLICATION);\n");
	fprintf (out, "\t\twndclass.hCursor = LoadCursor(NULL, IDC_ARROW);\n");
	fprintf (out, "\t\twndclass.hbrBackground  = GetStockObject(WHITE_BR\
USH);\n");
	fprintf (out, "\t\twndclass.lpszMenuName = NULL;\n");
	fprintf (out, "\t\twndclass.lpszClassName  = szAppName%s;\n", name);
	fprintf (out, "\n");
	fprintf (out, "\t\tif (!RegisterClass(&wndclass))\n");
	fprintf (out, "\t\t\treturn FALSE;\n");
	fprintf (out, "\t}\n");
	fprintf (out, "\n");

/*	if (gen[1] == 'O')
	{*/
	fprintf (out, "#ifdef OWL%s\n", name);
/*	fprintf (out, "\thInst = hInstance;\n"); */
	fprintf (out, "\tTAppli%s Appli (szAppName%s, hInstance, hPrevInstance,\
 lpCmdLine, nCmdShow);\n", name, name);
	fprintf (out, "\tAppli.Run ();\n");
	fprintf (out, "\treturn Appli.Status;\n");
	fprintf (out, "#else\n");
	/*
	}
	else if (gen[1] == 'A')
	{
	*/
	/* 	Wnd = new TAppliWindow (NULL, MWCAPTION); */
	fprintf (out, "\tWnd%s = new TAppliWindow%s (NULL, WndCaption%s);\n",
		name, name, name);
	fprintf (out, "\thWnd%s = CreateWindow (szAppName%s, WndCaption%s,\n",
    	name, name, name);
 	fprintf (out, "\t\tWS_OVERLAPPEDWINDOW | WS_VSCROLL | WS_HSCROLL,\n");
	fprintf (out, "\t\tCW_USEDEFAULT, CW_USEDEFAULT, (Wid%s)*2, (Hei%s)*3/2+20,\n",
		name, name);
	fprintf (out, "\t\tNULL, NULL, hInstance, NULL);\n");
	fprintf (out, "\n");
	fprintf (out, "\tif (!hWnd%s)\n", name);
	fprintf (out, "\t\treturn FALSE;\n");
	fprintf (out, "\n");
	fprintf (out, "\tShowWindow (hWnd%s, nCmdShow);\n", name);
	fprintf (out, "\tUpdateWindow (hWnd%s);\n", name);
	fprintf (out, "\n");
	fprintf (out, "\twhile (GetMessage(&msg, NULL, 0, 0))\n");
	fprintf (out, "\t{\n");

	fprintf (out, "\t\tif (!KBHandler (&msg, hWnd%s, NCTL%s, Res%s, hCtlObj%s, &Focus%s))\n",
		name, name, name, name, name);
	fprintf (out, "\t\t{\n");

	fprintf (out, "\t\t\tTranslateMessage (&msg);\n");
	fprintf (out, "\t\t\tDispatchMessage (&msg);\n");

	fprintf (out, "\t\t}\n");

	fprintf (out, "\t}\n");
	fprintf (out, "\n");
	fprintf (out, "\treturn(msg.wParam);\n");
	/*}*/
	fprintf (out, "#endif\n");
	fprintf (out, "}\n");

}

generate_line (struct resource_item *res, FILE *out, int obj, int n)
{
	switch (obj)
	{
		case GEN_CREATE_OWL:
			generate_line_create_OWL (res, out, n);
			break;
		case GEN_DEFINE_API:
			generate_line_define_API (res, out, n);
			break;
		case GEN_DECL_API:
			generate_line_decl_API (res, out, n);
			break;
		case GEN_HANDLE_API:
			generate_line_handle_API (res, out, n);
			break;
		case GEN_CREATE_API:
			generate_line_create_API (res, out, n);
			break;
		case GEN_COMMAND_API:
			generate_line_command_API (res, out, n);
			break;
		case GEN_INIT_RES:
			generate_line_init_res (res, out, n);
			break;
		case GEN_CREATE_MOB:
			generate_line_create_mob (res, out, n);
			break;
		case GEN_SIZE:
			generate_line_size (res, out, n);
			break;
		default:
			printf ("Unknown object %d\n", obj);

	}
}

compile_line (char *line_in, /*char *line_out*/ FILE *out, int obj, int n)
{
struct resource_item res;

	analyse_line (line_in, &res);
	generate_line (&res, /*line_*/out, obj, n);
}

compile (FILE *in, FILE *out, int obj)
{
int n;
char line_in[1000];
	n = 1;
	fseek (in, 0, SEEK_SET);
	for (;;)
	{
		fgets (line_in, sizeof(line_in), in);
		if (feof(in))
			break;
		line_in[strlen(line_in)-1] = 0;

		if (n >= max_item)
			max_item = n;

		compile_line (line_in, /*line_*/out, obj, n++);
		/* fprintf (out, "%s\n", line_out); */
	}
}

nop() { ; }


main (int argc, char *argv[])
{
FILE *in, *out;
char filename_in[200], filename_out[200];
char line_in[1000], line_out[1200];
/* struct resource_item res[300]; */
char gen[30];
char *in_filename;
char buf[60];

    if (argc >= 4)
    {
	strcpy (gen, argv[1]);

	in = fopen (argv[2], "r");
	in_filename = argv[2];
	if (in == NULL)
	{
		perror (argv[2]);
		return errno;
	}

	out = fopen (argv[3], "w");
	if (out == NULL)
	{
		perror (argv[3]);
		return errno;
	}
    }
    else
    {
    	printf ("Generate ? ");
	gets (gen);

get_in:
	printf ("Input file ? ");
	gets (filename_in);
	in_filename = filename_in;
	in = fopen (filename_in, "r");
	if (in == NULL)
	{
		perror (filename_in);
		goto get_in;
	}

get_out:
	printf ("Output file ? ");
	gets (filename_out);
	out = fopen (filename_out, "w");
	if (out == NULL)
	{
		perror (filename_out);
		goto get_out;
	}
    }
	nop();

	printf ("Config ? ");
	gets (config);

	printf ("Name ? ");
	gets (name);

	printf ("W ? ");
	gets (W);

	printf ("H ? ");
	gets (H);

	if (*gen == 'O')
		compile (in, out, GEN_CREATE_OWL);
	else if (*gen == 'A')
		compile_API (in, out, in_filename);
	else if (*gen == 'C')
		compile_comb (in, out, in_filename);
	else if (*gen == 'M')
		compile_mob (in, out, in_filename);
	else
		compile_gen (in, out, in_filename, gen);

	fclose (in);
	fclose (out);
}
