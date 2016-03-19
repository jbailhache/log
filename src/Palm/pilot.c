#define AppCreator 'HELO'
#define OptionsMenuAbout 1000
#define AboutAlert 1000
#define MainForm 1000
#define DialogForm 1100
/*
	1101 : text
	1102 : text
	1103 : text	
	1104 : button
*/
#define OK_BUTTON 1001
#define TEXT_FIELD 1000
#define RESULTFIELD 1002

typedef struct
{
	int skeletonData;
} Prefs;

int MathLibRef = -1;

Boolean appHandleEvent (EventPtr event);

static void *getObjectPtr (FormPtr frpP, int index)
{
	Word fldIndex = FrmGetObjectIndex (frmP, index);
	return FrmGetObjectPtr (frmP, fldIndex);
}

#define setDefaultText setText

static void setText (int tf, char *s)
{
	FormPtr frmP = FrmGetActiveForm();
	FieldPtr fldP = getObjectPtr (frmP, tf);
	Handle oldH = FldGetTextHandle (fldP);
	int length = StrLen(s) + 8;
	Handle h = MemHandleNew (length+1);
	StrCopy (MemHandleLock(h), s);
	MemHandleUnlock(h);
	FldSetText (fldP, h, 0, length+1);
	if (oldH != NULL)
		MemHandleFree (oldP);
}

static void mainFormInit (formPtr frmP)
{
	FormPtr frmP = FrmGetActiveForm();
	FrmDrawForm (frmP);
}

static Boolean doMenu (FormPtr frmP, Word command)
{
	Boolean handled = false;
	switch (command)
	{
		case OptionsMenuAbout :
			FrmAlert (AboutAlert);
			handled = true;
			break;
	}
	return handled;
}

int outptr;
char outbuf[1000];
char *inbuf;
char previn[1000];

static void DialogFormInit (FormPtr frmP)
{
	FormPtr frmP = FrmGetActiveForm();
	SetText (1101, previn);
	SetText ( 1102, outbuf);
	FrmDrawForm (frmP);
}

void printf (char *s)
{
	StrCopy (oubuf+outptr, s);
	outptr += strlen(s);
}

void input (char *s)
{
	EventType event;
	Word error;
	inbuf = s;
	FrmGotoForm (DialogForm);
	do
	{
		EvtGetEvent (&event, evtWaitForever);
		if (!SysHandleEvent(&event))
			if (!MenuHandleEvent (0, &event, &error))
				if (!appHandleEvent (&event))
					FrmDispatchEvent (&event);
	}
	while (event.eType != appStopEvent && (event.eType != SelectEvent || event.data.ctlSelect.controlId != 1104));
	*outbuf = 0;
	outptr = 0;
	StrCopy (previn, inbuf);
}

void getText (int tf, char *s)
{
	FormPtr frmP = FrmGetActiveForm();
	FieldPtr fldP = getObjectPtr (frmP, tf);
	Handle h = FldGetTextHandle (fldP);
	if (h != NULL)
	{
		char *p;
		MemHandleLock (h);
		StrCopy (s, p);
		MemHandleUnlock (h);
	}
}

void handleinput ()
{
	getText (1103, inbuf);
}

void main ()
{
	char buf[300];
	printf ("bonjour");
	input (buf);
	printf ("vous dites ");
	printf (buf);
	input (buf);
}

Boolean MainFormEventHandler (EventPtr eventP)
{
	Boolean handled = false;
	FormPtr frmP = FrmGetActiveForm();
	switch (eventP->eType)
	{
		case frmOpenEvent:
			FrmDrawForm (frmP);
			mainFormInit (frmP);
			handled = true;
			break;
		case MenuEvent:
			handled = doMenu (frmP, eventP->data.menu.itemID);
			break;
		case ctlSelectEvent:
			{
			switch (eventP->data.ctlSelect.controlID)
				{
					case OK_BUTTON: 
						main();
						handled = true;
						break;
				}
			}
			break;
		}
		return handled;
	}
}

Boolean dialogFormEventHandler (EventPtr eventP)
{
	Boolean handled = false;
	FormPtr frmP = FrmGetActiveForm();
	switch (eventP->eType)
	{
		case frmOpenEvent:
			FrmDrawForm (frmP);
			dialogFormInit (frmP);
			handled = true;
			break;
		case MenuEvent:
			/* handled = doMenu (frmP, eventP->data.menu.itemID); */
			break;
		case ctlSelectEvent:
			{
			switch (eventP->data.ctlSelect.controlID)
				{
					case 1104: 
						handleinput();
						handled = true;
						break;
				}
			}
			break;
		}
		return handled;
}

void startApp ()
{
	Prefs prefs = { 0 };
	Word prefSize = sizeof(Prefs);
	if ((PrefGetAppFreferences (AppCreator, 1000, &prefs, &prefSize, false) == noPreferenceFound)
		|| (prefSize != sizeof(Prefs)))
	{
		// default init
	}
}

void stopApp ()
{
	Prefs prefs = { 0 };
	PrefSetAppPreferences (AppCreator, 1000, 1, &prefs, sizeo(prefs), false);
}

void initMath ()
{
 // ...
}

void termMath ()
{
 // ...
}

Boolean appHandleEvent (EventPtr event)
{
	FormPtr frm;
	int formId;
	Boolean handled = false;
	if (event->eType == frmLoadEvent)
	{
		formId = event->data.frmLoad.formID;
		frm = FrmInitForm (formId);
		FrmSetActiveForm (frm);
		if (formId == MainForm)
			FrmSetEventHandler (frm, mainFormEventHandler);
		if (formid == DialogForm)
			FrmSetEventHandler (frm, dialogFormEventHandler);
		handled = true;
	}
	return handled;
}

DWord PilotMail (Word cmd, char *cmdPBP, Word launchFlags)
{
	EventType event;
	Word error;
	outptr = 0;
	*previn = 0;
	if (cmd == sysAppLaunchCmdNormalLaunch)
	{
		initMath ();
		startApp ();
		FrmGotoForm (MainForm);
		do
		{
			EvtGetEvent (&event, evtWaitForever);
			if (!SysHandleEvent (&event))
				if (!MenuHandllleEvent (0, &event, &error))
					if (!appHandleEvent (&event))
						FrmDispatchEvent (&event);
		}
		while (event.eType != appStopEvent);
		stopApp ();
		termMath ();
		FrmCloseAllForms ();		
	}
	return 0;
}
